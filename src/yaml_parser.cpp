#include <yaml-cpp/yaml.h>
#include <cstring>
#include <cstdlib>
#include "db_adapter.h"

extern "C" {

size_t count_db_config_yaml(const char* filename) {
    YAML::Node config = YAML::LoadFile(filename);
    return config["databases"].IsDefined() ? config["databases"].size() : 0;
}

size_t parse_db_config_yaml(const char* filename, db_conf* db_array) {
    YAML::Node config = YAML::LoadFile(filename);

    // Load default block if it exists
    YAML::Node default_cfg = config["default"];

    size_t count = 0;
    for (auto db_node : config["databases"]) {
        db_conf* db = &db_array[count];

        // Helper lambda to get string with default
        auto get_str = [&](const YAML::Node& node, const char* key, const char* def) -> const char* {
            if (node[key].IsDefined()) return node[key].as<std::string>().c_str();
            if (default_cfg[key].IsDefined()) return default_cfg[key].as<std::string>().c_str();
            return def;
        };

        // Helper lambda to get int with default
        auto get_int = [&](const YAML::Node& node, const char* key, int def) -> uint16_t {
            if (node[key].IsDefined()) return node[key].as<int>();
            if (default_cfg[key].IsDefined()) return default_cfg[key].as<int>();
            return def;
        };

        // Strings
        strncpy(db->name, get_str(db_node, "name", "unknown"), sizeof(db->name)-1);
        db->name[sizeof(db->name)-1] = '\0';

        strncpy(db->host, get_str(db_node, "host", "localhost"), sizeof(db->host)-1);
        db->host[sizeof(db->host)-1] = '\0';

        strncpy(db->dbname, get_str(db_node, "db_name", ""), sizeof(db->dbname)-1);
        db->dbname[sizeof(db->dbname)-1] = '\0';

        strncpy(db->user, get_str(db_node, "user", "root"), sizeof(db->user)-1);
        db->user[sizeof(db->user)-1] = '\0';

        // Adapter
        db->adapter = db_node["adapter"].IsDefined() ? db_node["adapter"].as<uint32_t>() :
                      (default_cfg["adapter"].IsDefined() ? default_cfg["adapter"].as<uint32_t>() : 0);

        // Port
        db->port = get_int(db_node, "port", 5432);

        // Password from environment
        const char* pass_env = get_str(db_node, "password_env", nullptr);
        const char* pass = pass_env ? std::getenv(pass_env) : nullptr;
        strncpy(db->password, pass ? pass : "", sizeof(db->password)-1);
        db->password[sizeof(db->password)-1] = '\0';

        // Retry policy
        db->policy.max_attempts = (db_node["retry"]["max_attempts"].IsDefined()) ?
                                    db_node["retry"]["max_attempts"].as<int>() :
                                    (default_cfg["retry"]["max_attempts"].IsDefined() ? 
                                     default_cfg["retry"]["max_attempts"].as<int>() : 3);

        db->policy.interval_seconds = (db_node["retry"]["interval_seconds"].IsDefined()) ?
                                        db_node["retry"]["interval_seconds"].as<int>() :
                                        (default_cfg["retry"]["interval_seconds"].IsDefined() ? 
                                         default_cfg["retry"]["interval_seconds"].as<int>() : 5);

        count++;
    }

    return count;
}

} // extern "C"
