#include "time.hpp"
#include "location.hpp"
#include "zones.hpp"
#include <iostream>
#include <sstream>
#include <iomanip>
#include <algorithm>

namespace timelib {

TimeConverter::TimeConverter() {
    conversion_pattern_ = std::regex(
        R"(^(?:convert|change|switch|make|what is|whats)\s+(.+?)\s+(?:from|frm|in|at)\s+(.+?)\s+(?:to|2|in|as)\s+(.+?)\s*$)",
        std::regex_constants::icase);

    implicit_conversion_pattern_ = std::regex(
        R"(^(.+?)\s+(?:in|at)\s+(.+?)\s+(?:to|as|in)\s+(.+?)\s*$)",
        std::regex_constants::icase);

    query_pattern_ = std::regex(
        R"(^(?:what is|what's|whats|wats|tell me|show me)?\s*(?:the)?\s*(?:current|local)?\s*(time|now|.+?)\s+(?:in|at|for|of)\s+(.+?)\s*$)",
        std::regex_constants::icase);

    difference_pattern_ = std::regex(
        R"(^(?:what's|whats|wats|what is)?\s*(?:the)?\s*time(?:s)?\s*(?:difference|diff|offset)\s*(?:between|b/w|from)\s+(.+?)\s*(?:and|&|to|2)\s+(.+?)\s*$)",
        std::regex_constants::icase);
}

QueryResult TimeConverter::processQuery(const ParsedQuery& query) {
    if (!query.is_valid) {
        return {"", ErrorCode::InvalidQuery, "Could not understand query. Check location or time format."};
    }

    switch (query.type) {
        case QueryType::CurrentTime:
            return getCurrentTimeIn(query.location_a);
        case QueryType::Conversion:
            return convertTime(query);
        case QueryType::Difference:
            return calculateTimeDifference(query);
        case QueryType::Invalid:
        default:
            return {"", ErrorCode::InvalidQuery, "Query appears to be invalid."};
    }
}

ParsedQuery TimeConverter::parseInput(const std::string& input) const {
    ParsedQuery query;
    std::smatch match;
    std::string cleaned_input = std::regex_replace(input, std::regex(R"(^\s+|\s+$)"), "");

    if (std::regex_match(cleaned_input, match, difference_pattern_)) {
        query.type = QueryType::Difference;
        query.location_a = normalizeLocation(match[1].str());
        query.location_b = normalizeLocation(match[2].str());
        query.is_valid = true;
        return query;
    }

    if (std::regex_match(cleaned_input, match, conversion_pattern_)) {
        query.type = QueryType::Conversion;
        parseTimeString(match[1].str(), query);
        query.location_a = normalizeLocation(match[2].str());
        query.location_b = normalizeLocation(match[3].str());
        query.is_valid = isValidTime(query.hour, query.minute);
        return query;
    }

    if (std::regex_match(cleaned_input, match, implicit_conversion_pattern_)) {
        parseTimeString(match[1].str(), query);
        if (isValidTime(query.hour, query.minute)) {
            query.type = QueryType::Conversion;
            query.location_a = normalizeLocation(match[2].str());
            query.location_b = normalizeLocation(match[3].str());
            query.is_valid = true;
            return query;
        }
        query = ParsedQuery();
    }

    if (std::regex_match(cleaned_input, match, query_pattern_)) {
        std::string time_keyword = match[1].str();
        std::string lower_time_keyword = time_keyword;
        std::transform(lower_time_keyword.begin(), lower_time_keyword.end(), lower_time_keyword.begin(), ::tolower);

        if (lower_time_keyword == "time" || lower_time_keyword == "now") {
            query.type = QueryType::CurrentTime;
            query.location_a = normalizeLocation(match[2].str());
            query.is_valid = true;
        } else {
            query.type = QueryType::Conversion;
            parseTimeString(time_keyword, query);
            query.location_b = normalizeLocation(match[2].str());
            try {
                query.location_a = date::current_zone()->name();
            } catch (const std::exception&) {
                query.is_valid = false;
                return query;
            }
            query.is_valid = isValidTime(query.hour, query.minute);
        }
        return query;
    }

    query.is_valid = false;
    return query;
}

QueryResult TimeConverter::calculateTimeDifference(const ParsedQuery& query) {
    auto tz_a_str = resolveTimezone(query.location_a);
    if (!tz_a_str) return {"", ErrorCode::UnknownLocation, "Unknown location: " + query.location_a};

    auto tz_b_str = resolveTimezone(*query.location_b);
    if (!tz_b_str) return {"", ErrorCode::UnknownLocation, "Unknown location: " + *query.location_b};

    try {
        auto zone_a = date::locate_zone(*tz_a_str);
        auto zone_b = date::locate_zone(*tz_b_str);

        auto now = std::chrono::system_clock::now();
        auto info_a = zone_a->get_info(now);
        auto info_b = zone_b->get_info(now);

        auto offset_diff = info_a.offset - info_b.offset;
        auto hours = std::chrono::duration_cast<std::chrono::hours>(offset_diff);
        auto minutes = std::chrono::duration_cast<std::chrono::minutes>(offset_diff % std::chrono::hours(1));

        std::stringstream ss;
        std::string name_a = query.location_a; name_a[0] = toupper(name_a[0]);
        std::string name_b = (*query.location_b); name_b[0] = toupper(name_b[0]);

        ss << name_a << " (" << info_a.abbrev << ") is ";
        if (offset_diff.count() == 0) {
            ss << "in the same timezone as " << name_b << " (" << info_b.abbrev << ").";
        } else {
            if (hours.count() != 0) ss << std::abs(hours.count()) << "h";
            if (minutes.count() != 0) ss << " " << std::abs(minutes.count()) << "m";

            ss << (offset_diff > std::chrono::seconds(0) ? " ahead of " : " behind ");
            ss << name_b << " (" << info_b.abbrev << ").";
        }
        return {ss.str(), ErrorCode::Success, ""};
    } catch (const std::exception& e) {
        return {"", ErrorCode::ProcessingError, "Error calculating time difference: " + std::string(e.what())};
    }
}

QueryResult TimeConverter::convertTime(const ParsedQuery& query) {
    const auto source_tz_str = resolveTimezone(query.location_a);
    if (!source_tz_str) return {"", ErrorCode::UnknownLocation, "Unknown source location: " + query.location_a};

    if (!query.location_b) return {"", ErrorCode::NoTargetLocation, "No target location specified for the conversion."};
    const auto target_tz_str = resolveTimezone(*query.location_b);
    if (!target_tz_str) return {"", ErrorCode::UnknownLocation, "Unknown target location: " + *query.location_b};

    try {
        const auto date = getCurrentDate();
        const auto local_tp = date::local_days{date} +
                        std::chrono::hours{query.hour} +
                        std::chrono::minutes{query.minute};

        const auto source_zone = date::locate_zone(*source_tz_str);
        const auto source_time = date::make_zoned(source_zone, local_tp, date::choose::earliest);

        const auto target_zone = date::locate_zone(*target_tz_str);
        const auto target_time = date::make_zoned(target_zone, source_time.get_sys_time());

        std::stringstream ss;
        ss << formatTime(source_time) << " in " << query.location_a
           << " is " << formatTime(target_time) << " in " << *query.location_b;
        return {ss.str(), ErrorCode::Success, ""};

    } catch (const std::exception& e) {
        return {"", ErrorCode::ProcessingError, "Error converting time: " + std::string(e.what())};
    }
}

QueryResult TimeConverter::getCurrentTimeIn(const std::string& location) {
    const auto tz = resolveTimezone(location);
    if (!tz) return {"", ErrorCode::UnknownLocation, "Unknown location: " + location};
    try {
        const auto zone = date::locate_zone(*tz);
        const auto time_in_seconds = std::chrono::time_point_cast<std::chrono::seconds>(std::chrono::system_clock::now());
        const auto zoned_time = date::make_zoned(zone, time_in_seconds);
        const std::string result = "The current time in " + location + " is " + formatTime(zoned_time);
        return {result, ErrorCode::Success, ""};
    } catch (const std::exception& e) {
        return {"", ErrorCode::ProcessingError, "Error getting current time: " + std::string(e.what())};
    }
}

void TimeConverter::parseTimeString(const std::string& time_str, ParsedQuery& result) {
    std::string lower_time = time_str;
    std::transform(lower_time.begin(), lower_time.end(), lower_time.begin(), ::tolower);

    if (lower_time == "noon") { result.hour = 12; result.minute = 0; return; }
    if (lower_time == "midnight") { result.hour = 0; result.minute = 0; return; }

    const std::regex time_pattern(R"((\d{1,2})(?::(\d{2}))?\s*(am|pm)?)", std::regex_constants::icase);
    if (std::smatch time_match; std::regex_search(lower_time, time_match, time_pattern)) {
        const auto hour_opt = parseHour(time_match[1].str());
        if (!hour_opt) return;

        result.hour = *hour_opt;
        result.minute = time_match[2].matched ? parseMinute(time_match[2].str()).value_or(0) : 0;

        if (time_match[3].matched) {
            if (const std::string ampm = time_match[3].str(); ampm == "pm" && result.hour != 12) result.hour += 12;
            else if (ampm == "am" && result.hour == 12) result.hour = 0;
        }
    } else {
        result.hour = -1;
    }
}

std::string TimeConverter::formatTime(const date::zoned_time<std::chrono::seconds>& zt) {
    return date::format("%b %d, %I:%M %p (%Z)", zt);
}

std::optional<std::string> TimeConverter::resolveTimezone(const std::string& location_or_zone) {
    try {
        date::locate_zone(location_or_zone);
        return location_or_zone;
    } catch(...) { }

    static LocationMap location_map;
    static TimezoneMap timezone_map;

    std::string lower_loc = normalizeLocation(location_or_zone);

    if (location_map.hasLocation(lower_loc)) return location_map.getTimezone(lower_loc);
    if (timezone_map.hasTimezone(lower_loc)) return timezone_map.getOfficialName(lower_loc);

    return std::nullopt;
}

date::year_month_day TimeConverter::getCurrentDate() {
    return date::floor<date::days>(std::chrono::system_clock::now());
}

bool TimeConverter::isValidTime(const int hour, const int minute) {
    return hour >= 0 && hour < 24 && minute >= 0 && minute < 60;
}

std::optional<int> TimeConverter::parseHour(const std::string& hour_str) {
    try {
        int hour = std::stoi(hour_str);
        return (hour >= 0 && hour <= 24) ? std::optional<int>(hour) : std::nullopt;
    } catch (...) { return std::nullopt; }
}

std::optional<int> TimeConverter::parseMinute(const std::string& minute_str) {
    try {
        int minute = std::stoi(minute_str);
        return (minute >= 0 && minute <= 59) ? std::optional<int>(minute) : std::nullopt;
    } catch (...) { return std::nullopt; }
}

std::string TimeConverter::normalizeLocation(const std::string& location) {
    std::string normalized = location;
    normalized.erase(0, normalized.find_first_not_of(" \t\n\r\f\v"));
    normalized.erase(normalized.find_last_not_of(" \t\n\r\f\v") + 1);
    std::transform(normalized.begin(), normalized.end(), normalized.begin(), ::tolower);
    return normalized;
}

}
