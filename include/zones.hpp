// NOTE: a majority of this file was "generated" using AI tools (specifically, using Gemini 2.5 Pro).
#pragma once

#include <unordered_map>
#include <string>
#include <vector>
#include <algorithm>

namespace timelib {

    struct TimezoneInfo {
        std::string official_name;
        std::vector<std::string> aliases;
        std::string description;
    };

    class TimezoneMap {
    public:
        TimezoneMap() {
            initializeTimezones();
        }

        std::string getOfficialName(const std::string& alias) const {
            std::string lower_alias = alias;
            std::transform(lower_alias.begin(), lower_alias.end(), lower_alias.begin(), ::tolower);

            if (const auto it = alias_to_official_.find(lower_alias); it != alias_to_official_.end()) {
                return it->second;
            }
            return alias;
        }

        bool hasTimezone(const std::string& name) const {
            std::string lower_name = name;
            std::transform(lower_name.begin(), lower_name.end(), lower_name.begin(), ::tolower);

            return alias_to_official_.find(lower_name) != alias_to_official_.end();
        }

        std::vector<std::string> getTimezoneAliases(const std::string& timezone) const {
            std::string lower_timezone = timezone;
            std::transform(lower_timezone.begin(), lower_timezone.end(), lower_timezone.begin(), ::tolower);

            const auto it = alias_to_official_.find(lower_timezone);
            const std::string official_name = (it != alias_to_official_.end()) ? it->second : timezone;

            if (const auto tz_it = timezones_.find(official_name); tz_it != timezones_.end()) {
                return tz_it->second.aliases;
            }
            return {};
        }

        const TimezoneInfo* getTimezoneInfo(const std::string& name) const {
            std::string lower_name = name;
            std::transform(lower_name.begin(), lower_name.end(), lower_name.begin(), ::tolower);

            const auto it = alias_to_official_.find(lower_name);
            const std::string official_name = (it != alias_to_official_.end()) ? it->second : name;

            const auto tz_it = timezones_.find(official_name);
            if (tz_it != timezones_.end()) {
                return &tz_it->second;
            }
            return nullptr;
        }

        void addTimezoneAlias(const std::string& official_name,
                             const std::vector<std::string>& aliases) {
            addTimezoneInternal(official_name, aliases, "");
        }

    private:
        std::unordered_map<std::string, TimezoneInfo> timezones_;
        std::unordered_map<std::string, std::string> alias_to_official_;

        void initializeTimezones() {
            // the only reasonable timezone
            addTimezoneInternal("UTC", {"utc", "gmt", "zulu", "z", "coordinated universal time"}, "Coordinated Universal Time");

            // us timezones
            addTimezoneInternal("America/New_York", {"est", "edt", "eastern", "et", "eastern time"}, "Eastern Time (US)");
            addTimezoneInternal("America/Chicago", {"cst", "cdt", "central", "ct", "central time"}, "Central Time (US)");
            addTimezoneInternal("America/Denver", {"mst", "mdt", "mountain", "mt", "mountain time"}, "Mountain Time (US)");
            addTimezoneInternal("America/Los_Angeles", {"pst", "pdt", "pacific", "pt", "pacific time"}, "Pacific Time (US)");
            addTimezoneInternal("America/Phoenix", {"mst", "arizona time"}, "Mountain Standard Time (Arizona, no DST)");
            addTimezoneInternal("America/Anchorage", {"akst", "akdt", "alaska", "alaska time"}, "Alaska Time");
            addTimezoneInternal("Pacific/Honolulu", {"hst", "hast", "hawaii"}, "Hawaii-Aleutian Standard Time");

            // eu timezone
            addTimezoneInternal("Europe/London", {"bst", "gmt", "wet", "western european time"}, "Western European Time");
            addTimezoneInternal("Europe/Paris", {"cet", "cest", "central european time"}, "Central European Time");
            addTimezoneInternal("Europe/Helsinki", {"eet", "eest", "eastern european time"}, "Eastern European Time");
            addTimezoneInternal("Europe/Moscow", {"msk", "moscow time"}, "Moscow Time");

            // asian timezones
            addTimezoneInternal("Asia/Kolkata", {"ist", "india standard time"}, "India Standard Time");
            addTimezoneInternal("Asia/Shanghai", {"cst", "china standard time"}, "China Standard Time");
            addTimezoneInternal("Asia/Tokyo", {"jst", "japan standard time"}, "Japan Standard Time");
            addTimezoneInternal("Asia/Dubai", {"gst", "gulf standard time"}, "Gulf Standard Time");
            addTimezoneInternal("Asia/Seoul", {"kst", "korea standard time"}, "Korea Standard Time");
            addTimezoneInternal("Asia/Singapore", {"sgt", "singapore time"}, "Singapore Time");

            // oceanic timezones
            addTimezoneInternal("Australia/Sydney", {"aest", "aedt", "australian eastern time"}, "Australian Eastern Time (Sydney, Melbourne)");
            addTimezoneInternal("Australia/Brisbane", {"aest", "brisbane time"}, "Australian Eastern Standard Time (Brisbane)");
            addTimezoneInternal("Australia/Adelaide", {"acst", "acdt", "australian central time"}, "Australian Central Time (Adelaide)");
            addTimezoneInternal("Australia/Darwin", {"acst", "darwin time"}, "Australian Central Standard Time (Darwin)");
            addTimezoneInternal("Australia/Perth", {"awst", "australian western time"}, "Australian Western Standard Time (Perth)");

            // south american timezones
            addTimezoneInternal("America/Sao_Paulo", {"brt", "brst", "brasilia time"}, "Brasilia Time");
            addTimezoneInternal("America/Argentina/Buenos_Aires", {"art", "argentina time"}, "Argentina Time");

            // military timezones
            addTimezoneInternal("Etc/GMT-1", {"a", "alpha"}, "Alpha Time Zone (UTC+1)");
            addTimezoneInternal("Etc/GMT-2", {"b", "bravo"}, "Bravo Time Zone (UTC+2)");
            addTimezoneInternal("Etc/GMT-3", {"c", "charlie"}, "Charlie Time Zone (UTC+3)");
            addTimezoneInternal("Etc/GMT-4", {"d", "delta"}, "Delta Time Zone (UTC+4)");
            addTimezoneInternal("Etc/GMT-5", {"e", "echo"}, "Echo Time Zone (UTC+5)");
            addTimezoneInternal("Etc/GMT-6", {"f", "foxtrot"}, "Foxtrot Time Zone (UTC+6)");
            addTimezoneInternal("Etc/GMT-7", {"g", "golf"}, "Golf Time Zone (UTC+7)");
            addTimezoneInternal("Etc/GMT-8", {"h", "hotel"}, "Hotel Time Zone (UTC+8)");
            addTimezoneInternal("Etc/GMT-9", {"i", "india"}, "India Time Zone (UTC+9)");
            addTimezoneInternal("Etc/GMT-10", {"k", "kilo"}, "Kilo Time Zone (UTC+10)");
            addTimezoneInternal("Etc/GMT-11", {"l", "lima"}, "Lima Time Zone (UTC+11)");
            addTimezoneInternal("Etc/GMT-12", {"m", "mike"}, "Mike Time Zone (UTC+12)");
            addTimezoneInternal("Etc/GMT+1", {"n", "november"}, "November Time Zone (UTC-1)");
            addTimezoneInternal("Etc/GMT+2", {"o", "oscar"}, "Oscar Time Zone (UTC-2)");
            addTimezoneInternal("Etc/GMT+3", {"p", "papa"}, "Papa Time Zone (UTC-3)");
            addTimezoneInternal("Etc/GMT+4", {"q", "quebec"}, "Quebec Time Zone (UTC-4)");
            addTimezoneInternal("Etc/GMT+5", {"r", "romeo"}, "Romeo Time Zone (UTC-5)");
            addTimezoneInternal("Etc/GMT+6", {"s", "sierra"}, "Sierra Time Zone (UTC-6)");
            addTimezoneInternal("Etc/GMT+7", {"t", "tango"}, "Tango Time Zone (UTC-7)");
            addTimezoneInternal("Etc/GMT+8", {"u", "uniform"}, "Uniform Time Zone (UTC-8)");
            addTimezoneInternal("Etc/GMT+9", {"v", "victor"}, "Victor Time Zone (UTC-9)");
            addTimezoneInternal("Etc/GMT+10", {"w", "whiskey"}, "Whiskey Time Zone (UTC-10)");
            addTimezoneInternal("Etc/GMT+11", {"x", "x-ray"}, "X-ray Time Zone (UTC-11)");
            addTimezoneInternal("Etc/GMT+12", {"y", "yankee"}, "Yankee Time Zone (UTC-12)");

            // etc
            addTimezoneInternal("Etc/GMT+12", {"utc-12"}, "UTC-12:00");
            addTimezoneInternal("Etc/GMT+11", {"utc-11"}, "UTC-11:00");
            addTimezoneInternal("Etc/GMT+10", {"utc-10"}, "UTC-10:00");
            addTimezoneInternal("Etc/GMT+9", {"utc-9"}, "UTC-09:00");
            addTimezoneInternal("Etc/GMT+8", {"utc-8"}, "UTC-08:00");
            addTimezoneInternal("Etc/GMT+7", {"utc-7"}, "UTC-07:00");
            addTimezoneInternal("Etc/GMT+6", {"utc-6"}, "UTC-06:00");
            addTimezoneInternal("Etc/GMT+5", {"utc-5"}, "UTC-05:00");
            addTimezoneInternal("Etc/GMT+4", {"utc-4"}, "UTC-04:00");
            addTimezoneInternal("Etc/GMT+3", {"utc-3"}, "UTC-03:00");
            addTimezoneInternal("Etc/GMT+2", {"utc-2"}, "UTC-02:00");
            addTimezoneInternal("Etc/GMT+1", {"utc-1"}, "UTC-01:00");
            addTimezoneInternal("Etc/GMT-1", {"utc+1"}, "UTC+01:00");
            addTimezoneInternal("Etc/GMT-2", {"utc+2"}, "UTC+02:00");
            addTimezoneInternal("Etc/GMT-3", {"utc+3"}, "UTC+03:00");
            addTimezoneInternal("Etc/GMT-4", {"utc+4"}, "UTC+04:00");
            addTimezoneInternal("Etc/GMT-5", {"utc+5"}, "UTC+05:00");
            addTimezoneInternal("Etc/GMT-6", {"utc+6"}, "UTC+06:00");
            addTimezoneInternal("Etc/GMT-7", {"utc+7"}, "UTC+07:00");
            addTimezoneInternal("Etc/GMT-8", {"utc+8"}, "UTC+08:00");
            addTimezoneInternal("Etc/GMT-9", {"utc+9"}, "UTC+09:00");
            addTimezoneInternal("Etc/GMT-10", {"utc+10"}, "UTC+10:00");
            addTimezoneInternal("Etc/GMT-11", {"utc+11"}, "UTC+11:00");
            addTimezoneInternal("Etc/GMT-12", {"utc+12"}, "UTC+12:00");
            addTimezoneInternal("Etc/GMT-13", {"utc+13"}, "UTC+13:00");
            addTimezoneInternal("Etc/GMT-14", {"utc+14"}, "UTC+14:00");
        }

        void addTimezoneInternal(const std::string& official_name,
                                const std::vector<std::string>& aliases,
                                const std::string& description) {
            TimezoneInfo info;
            info.official_name = official_name;
            info.aliases = aliases;
            info.description = description;
            timezones_[official_name] = info;

            std::string lower_official = official_name;
            std::transform(lower_official.begin(), lower_official.end(), lower_official.begin(), ::tolower);
            alias_to_official_[lower_official] = official_name;

            for (const auto& alias : aliases) {
                std::string lower_alias = alias;
                std::transform(lower_alias.begin(), lower_alias.end(), lower_alias.begin(), ::tolower);
                if (alias_to_official_.find(lower_alias) == alias_to_official_.end()) {
                    alias_to_official_[lower_alias] = official_name;
                }
            }
        }
    };

}