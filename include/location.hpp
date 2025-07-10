// NOTE: a majority of this file was "generated" using AI tools (specifically, using Gemini 2.5 Pro).
#pragma once

#include <unordered_map>
#include <string>
#include <vector>
#include <algorithm>

namespace timelib {

    struct LocationInfo {
        std::string official_name;
        std::string timezone;
        std::vector<std::string> aliases;
    };

    class LocationMap {
    public:
        LocationMap() {
            initializeLocations();
        }

        std::string getTimezone(const std::string& location) const {
            std::string lower_location = location;
            std::transform(lower_location.begin(), lower_location.end(), lower_location.begin(), ::tolower);

            if (const auto it = alias_to_location_.find(lower_location); it != alias_to_location_.end()) {
                if (const auto loc_it = locations_.find(it->second); loc_it != locations_.end()) {
                    return loc_it->second.timezone;
                }
            }
            return "";
        }

        bool hasLocation(const std::string& location) const {
            std::string lower_location = location;
            std::transform(lower_location.begin(), lower_location.end(), lower_location.begin(), ::tolower);

            return alias_to_location_.find(lower_location) != alias_to_location_.end();
        }

        std::vector<std::string> getLocationAliases(const std::string& location) const {
            std::string lower_location = location;
            std::transform(lower_location.begin(), lower_location.end(), lower_location.begin(), ::tolower);

            if (const auto it = alias_to_location_.find(lower_location); it != alias_to_location_.end()) {
                if (const auto loc_it = locations_.find(it->second); loc_it != locations_.end()) {
                    return loc_it->second.aliases;
                }
            }
            return {};
        }

        const LocationInfo* getLocationInfo(const std::string& location) const {
            std::string lower_location = location;
            std::transform(lower_location.begin(), lower_location.end(), lower_location.begin(), ::tolower);

            if (const auto it = alias_to_location_.find(lower_location); it != alias_to_location_.end()) {
                const auto loc_it = locations_.find(it->second);
                if (loc_it != locations_.end()) {
                    return &loc_it->second;
                }
            }
            return nullptr;
        }

        void addLocation(const std::string& name, const std::string& timezone,
                        const std::vector<std::string>& aliases = {}) {
            addLocationInternal(name, timezone, aliases);
        }

    private:
        std::unordered_map<std::string, LocationInfo> locations_;
        std::unordered_map<std::string, std::string> alias_to_location_;

        void initializeLocations() {
            addLocationInternal("new york", "America/New_York", {"nyc", "ny", "new york city", "manhattan", "eastern time", "et"});
            addLocationInternal("los angeles", "America/Los_Angeles", {"la", "l.a.", "la county", "pacific time", "pt"});
            addLocationInternal("chicago", "America/Chicago", {"chi", "central time", "ct"});
            addLocationInternal("san francisco", "America/Los_Angeles", {"sf", "bay area", "san fran", "frisco"});
            addLocationInternal("london", "Europe/London", {"london", "londn", "uk", "britain", "england", "great britain", "united kingdom"});
            addLocationInternal("paris", "Europe/Paris", {"paris", "pari", "france"});
            addLocationInternal("berlin", "Europe/Berlin", {"berlin", "berln", "germany"});
            addLocationInternal("tokyo", "Asia/Tokyo", {"tokyo", "tokio", "japan"});
            addLocationInternal("mumbai", "Asia/Kolkata", {"mumbai", "bombay", "bombai"});
            addLocationInternal("delhi", "Asia/Kolkata", {"delhi", "new delhi", "india"});
            addLocationInternal("sydney", "Australia/Sydney", {"sydney", "sydny", "australia"});
            addLocationInternal("melbourne", "Australia/Melbourne", {"melbourne", "melbrn"});
            addLocationInternal("houston", "America/Chicago", {"houston"});
            addLocationInternal("phoenix", "America/Phoenix", {"phoenix", "arizona", "mountain time no dst"});
            addLocationInternal("philadelphia", "America/New_York", {"philly"});
            addLocationInternal("san antonio", "America/Chicago", {"san antonio"});
            addLocationInternal("san diego", "America/Los_Angeles", {"san diego"});
            addLocationInternal("dallas", "America/Chicago", {"dallas", "dfw"});
            addLocationInternal("san jose", "America/Los_Angeles", {"san jose"});
            addLocationInternal("austin", "America/Chicago", {"austin", "atx"});
            addLocationInternal("jacksonville", "America/New_York", {"jacksonville", "jax"});
            addLocationInternal("indianapolis", "America/Indiana/Indianapolis", {"indy"});
            addLocationInternal("columbus", "America/New_York", {"columbus"});
            addLocationInternal("seattle", "America/Los_Angeles", {"seattle"});
            addLocationInternal("denver", "America/Denver", {"denver", "mountain time", "mt"});
            addLocationInternal("washington dc", "America/New_York", {"washington", "dc", "d.c."});
            addLocationInternal("boston", "America/New_York", {"boston"});
            addLocationInternal("detroit", "America/Detroit", {"detroit"});
            addLocationInternal("nashville", "America/Chicago", {"nashville"});
            addLocationInternal("memphis", "America/Chicago", {"memphis"});
            addLocationInternal("portland", "America/Los_Angeles", {"portland", "pdx"});
            addLocationInternal("las vegas", "America/Los_Angeles", {"vegas"});
            addLocationInternal("miami", "America/New_York", {"miami"});
            addLocationInternal("atlanta", "America/New_York", {"atlanta", "atl"});
            addLocationInternal("new orleans", "America/Chicago", {"nola"});
            addLocationInternal("honolulu", "Pacific/Honolulu", {"honolulu", "hawaii", "hst"});
            addLocationInternal("anchorage", "America/Anchorage", {"anchorage", "alaska", "akst"});
            addLocationInternal("toronto", "America/Toronto", {"toronto", "the 6ix"});
            addLocationInternal("montreal", "America/Toronto", {"montreal"});
            addLocationInternal("vancouver", "America/Vancouver", {"vancouver", "yvr"});
            addLocationInternal("calgary", "America/Edmonton", {"calgary"});
            addLocationInternal("edmonton", "America/Edmonton", {"edmonton"});
            addLocationInternal("ottawa", "America/Toronto", {"ottawa"});
            addLocationInternal("quebec city", "America/Toronto", {"quebec"});
            addLocationInternal("winnipeg", "America/Winnipeg", {"winnipeg"});
            addLocationInternal("halifax", "America/Halifax", {"halifax"});
            addLocationInternal("st johns", "America/St_Johns", {"st. john's", "newfoundland"});
            addLocationInternal("sao paulo", "America/Sao_Paulo", {"sao paulo", "sp", "brazil"});
            addLocationInternal("buenos aires", "America/Argentina/Buenos_Aires", {"buenos aires", "ba", "argentina"});
            addLocationInternal("rio de janeiro", "America/Sao_Paulo", {"rio"});
            addLocationInternal("bogota", "America/Bogota", {"bogota", "colombia"});
            addLocationInternal("lima", "America/Lima", {"lima", "peru"});
            addLocationInternal("santiago", "America/Santiago", {"santiago", "chile"});
            addLocationInternal("caracas", "America/Caracas", {"caracas", "venezuela"});
            addLocationInternal("quito", "America/Guayaquil", {"quito", "ecuador"});
            addLocationInternal("la paz", "America/La_Paz", {"la paz", "bolivia"});
            addLocationInternal("montevideo", "America/Montevideo", {"montevideo", "uruguay"});
            addLocationInternal("asuncion", "America/Asuncion", {"asuncion", "paraguay"});
            addLocationInternal("mexico city", "America/Mexico_City", {"mexico city", "cdmx", "mexico"});
            addLocationInternal("havana", "America/Havana", {"havana", "cuba"});
            addLocationInternal("panama city", "America/Panama", {"panama"});
            addLocationInternal("san jose cr", "America/Costa_Rica", {"san jose", "costa rica"});
            addLocationInternal("kingston", "America/Jamaica", {"kingston", "jamaica"});
            addLocationInternal("santo domingo", "America/Santo_Domingo", {"santo domingo", "dominican republic"});
            addLocationInternal("madrid", "Europe/Madrid", {"madrid", "spain"});
            addLocationInternal("rome", "Europe/Rome", {"rome", "italy"});
            addLocationInternal("moscow", "Europe/Moscow", {"moscow", "russia"});
            addLocationInternal("kyiv", "Europe/Kyiv", {"kyiv", "kiev", "ukraine"});
            addLocationInternal("amsterdam", "Europe/Amsterdam", {"amsterdam", "netherlands", "holland"});
            addLocationInternal("brussels", "Europe/Brussels", {"brussels", "belgium"});
            addLocationInternal("vienna", "Europe/Vienna", {"vienna", "austria"});
            addLocationInternal("zurich", "Europe/Zurich", {"zurich", "switzerland"});
            addLocationInternal("athens", "Europe/Athens", {"athens", "greece"});
            addLocationInternal("stockholm", "Europe/Stockholm", {"stockholm", "sweden"});
            addLocationInternal("oslo", "Europe/Oslo", {"oslo", "norway"});
            addLocationInternal("copenhagen", "Europe/Copenhagen", {"copenhagen", "denmark"});
            addLocationInternal("helsinki", "Europe/Helsinki", {"helsinki", "finland"});
            addLocationInternal("dublin", "Europe/Dublin", {"dublin", "ireland"});
            addLocationInternal("lisbon", "Europe/Lisbon", {"lisbon", "portugal"});
            addLocationInternal("prague", "Europe/Prague", {"prague", "czech republic"});
            addLocationInternal("warsaw", "Europe/Warsaw", {"warsaw", "poland"});
            addLocationInternal("budapest", "Europe/Budapest", {"budapest", "hungary"});
            addLocationInternal("bucharest", "Europe/Bucharest", {"bucharest", "romania"});
            addLocationInternal("istanbul", "Europe/Istanbul", {"istanbul", "turkey"});
            addLocationInternal("beijing", "Asia/Shanghai", {"beijing", "peking"});
            addLocationInternal("shanghai", "Asia/Shanghai", {"shanghai", "china"});
            addLocationInternal("hong kong", "Asia/Hong_Kong", {"hong kong", "hk"});
            addLocationInternal("singapore", "Asia/Singapore", {"singapore", "sg"});
            addLocationInternal("seoul", "Asia/Seoul", {"seoul", "south korea"});
            addLocationInternal("bangkok", "Asia/Bangkok", {"bangkok", "thailand"});
            addLocationInternal("dubai", "Asia/Dubai", {"dubai", "uae", "united arab emirates"});
            addLocationInternal("jakarta", "Asia/Jakarta", {"jakarta", "indonesia"});
            addLocationInternal("manila", "Asia/Manila", {"manila", "philippines"});
            addLocationInternal("ho chi minh city", "Asia/Ho_Chi_Minh", {"saigon", "vietnam"});
            addLocationInternal("taipei", "Asia/Taipei", {"taipei", "taiwan"});
            addLocationInternal("kuala lumpur", "Asia/Kuala_Lumpur", {"kuala lumpur", "malaysia"});
            addLocationInternal("karachi", "Asia/Karachi", {"karachi", "pakistan"});
            addLocationInternal("dhaka", "Asia/Dhaka", {"dhaka", "bangladesh"});
            addLocationInternal("riyadh", "Asia/Riyadh", {"riyadh", "saudi arabia"});
            addLocationInternal("baghdad", "Asia/Baghdad", {"baghdad", "iraq"});
            addLocationInternal("tehran", "Asia/Tehran", {"tehran", "iran"});
            addLocationInternal("jerusalem", "Asia/Jerusalem", {"jerusalem", "israel"});
            addLocationInternal("kabul", "Asia/Kabul", {"kabul", "afghanistan"});
            addLocationInternal("yekaterinburg", "Asia/Yekaterinburg", {"yekaterinburg"});
            addLocationInternal("vladivostok", "Asia/Vladivostok", {"vladivostok"});
            addLocationInternal("cairo", "Africa/Cairo", {"cairo", "egypt"});
            addLocationInternal("lagos", "Africa/Lagos", {"lagos", "nigeria"});
            addLocationInternal("kinshasa", "Africa/Kinshasa", {"kinshasa", "drc"});
            addLocationInternal("johannesburg", "Africa/Johannesburg", {"johannesburg", "joburg", "south africa"});
            addLocationInternal("nairobi", "Africa/Nairobi", {"nairobi", "kenya"});
            addLocationInternal("addis ababa", "Africa/Addis_Ababa", {"addis ababa", "ethiopia"});
            addLocationInternal("casablanca", "Africa/Casablanca", {"casablanca", "morocco"});
            addLocationInternal("accra", "Africa/Accra", {"accra", "ghana"});
            addLocationInternal("algiers", "Africa/Algiers", {"algiers", "algeria"});
            addLocationInternal("dakar", "Africa/Dakar", {"dakar", "senegal"});
            addLocationInternal("brisbane", "Australia/Brisbane", {"brisbane"});
            addLocationInternal("perth", "Australia/Perth", {"perth"});
            addLocationInternal("adelaide", "Australia/Adelaide", {"adelaide"});
            addLocationInternal("canberra", "Australia/Sydney", {"canberra"});
            addLocationInternal("darwin", "Australia/Darwin", {"darwin"});
            addLocationInternal("auckland", "Pacific/Auckland", {"auckland", "new zealand"});
            addLocationInternal("wellington", "Pacific/Auckland", {"wellington"});
            addLocationInternal("fiji", "Pacific/Fiji", {"fiji"});
            addLocationInternal("papeete", "Pacific/Tahiti", {"tahiti"});
        }

        void addLocationInternal(const std::string& name, const std::string& timezone,
                                const std::vector<std::string>& aliases) {
            LocationInfo info;
            info.official_name = name;
            info.timezone = timezone;
            info.aliases = aliases;

            locations_[name] = info;

            std::string lower_name = name;
            std::transform(lower_name.begin(), lower_name.end(), lower_name.begin(), ::tolower);
            alias_to_location_[lower_name] = name;

            for (const auto& alias : aliases) {
                std::string lower_alias = alias;
                std::transform(lower_alias.begin(), lower_alias.end(), lower_alias.begin(), ::tolower);
                alias_to_location_[lower_alias] = name;
            }
        }
    };

}