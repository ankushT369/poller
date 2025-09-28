#include <yaml-cpp/yaml.h>
#include <cstring>   // for strncpy
#include <cstdlib>   // for getenv
#include "db_adapter.h"

extern "C" {

size_t count_dbs_config_yaml(const char* filename) {
    YAML::Node config = YAML::LoadFile(filename);
    return config["databases"].size();
}

size_t parse_db_config_yaml(const char* filename, db_conn* db_array, int max_dbs) {
    YAML::Node config = YAML::LoadFile(filename);
    YAML::Node default_cfg = config["default"];

    size_t count = 0;
    for (auto db_node : config["databases"]) {
        //if (count >= max_dbs) break;

        YAML::Node merged = default_cfg;
        for (auto it : db_node) {
            merged[it.first] = it.second;
        }

        db_conn* db = &db_array[count];

        // Copy strings safely
        strncpy(db->name, merged["name"].as<std::string>().c_str(), sizeof(db->name)-1);
        db->name[sizeof(db->name)-1] = '\0';

        strncpy(db->adapter_name, merged["adapter"].as<std::string>().c_str(), sizeof(db->adapter_name)-1);
        db->adapter_name[sizeof(db->adapter_name)-1] = '\0';

        strncpy(db->host, merged["host"].as<std::string>().c_str(), sizeof(db->host)-1);
        db->host[sizeof(db->host)-1] = '\0';

        db->port = merged["port"].as<int>();

        strncpy(db->dbname, merged["db_name"].as<std::string>().c_str(), sizeof(db->dbname)-1);
        db->dbname[sizeof(db->dbname)-1] = '\0';

        strncpy(db->user, merged["user"].as<std::string>().c_str(), sizeof(db->user)-1);
        db->user[sizeof(db->user)-1] = '\0';

        // Password from environment variable
        const char* pass = std::getenv(merged["password_env"].as<std::string>().c_str());
        strncpy(db->password, pass ? pass : "", sizeof(db->password)-1);
        db->password[sizeof(db->password)-1] = '\0';

        // Retry policy
        db->policy.max_attempts = merged["retry"]["max_attempts"].as<int>();
        db->policy.interval_seconds = merged["retry"]["interval_seconds"].as<int>();

        count++;
    }

    return count; // number of databases parsed
}

} // extern "C"

