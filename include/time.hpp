#pragma once

#include <string>
#include <optional>
#include <chrono>
#include <regex>
#include <date/date.h>
#include <date/tz.h>

namespace timelib {
    enum class QueryType {
        Conversion,
        CurrentTime,
        Difference,
        Invalid
    };

    enum class ErrorCode {
        Success = 0,
        InvalidQuery,
        InvalidTime,
        UnknownLocation,
        ProcessingError,
        NoTargetLocation
    };

    struct ParsedQuery {
        QueryType type = QueryType::Invalid;
        int hour = -1;
        int minute = -1;
        std::string location_a;
        std::optional<std::string> location_b;
        bool is_valid = false;
    };

    struct QueryResult {
        std::string result;
        ErrorCode code = ErrorCode::Success;
        std::string error_message;
    };

    class TimeConverter {
    public:
        TimeConverter();
        ParsedQuery parseInput(const std::string& input) const;
        static QueryResult processQuery(const ParsedQuery& query);

    private:
        std::regex conversion_pattern_;
        std::regex query_pattern_;
        std::regex difference_pattern_;
        std::regex implicit_conversion_pattern_;

        static QueryResult getCurrentTimeIn(const std::string& location);
        static QueryResult convertTime(const ParsedQuery& query);
        static QueryResult calculateTimeDifference(const ParsedQuery& query);
        static std::string formatTime(const date::zoned_time<std::chrono::seconds>& zt);
        static void parseTimeString(const std::string& time_str, ParsedQuery& result);
        static std::optional<std::string> resolveTimezone(const std::string& location_or_zone);
        static date::year_month_day getCurrentDate();

        static bool isValidTime(int hour, int minute);
        static std::optional<int> parseHour(const std::string& hour_str);
        static std::optional<int> parseMinute(const std::string& minute_str);
        static std::string normalizeLocation(const std::string& location);
    };

}
