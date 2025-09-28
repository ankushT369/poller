#include <yaml-cpp/yaml.h>
#include <iostream>
#include <vector>
#include <string>
#include <cstdlib> // for getenv

// Structs
struct RetryPolicy {
    int max_attempts;
    int interval_seconds;
};

struct DatabaseConfig {
    std::string name;
    std::string adapter;
    std::string host;
    int port;
    std::string db_name;
    std::string user;
    std::string password; // will come from env
    RetryPolicy retry;
};

// Helper: merge defaults into a node
YAML::Node mergeWithDefault(const YAML::Node &node, const YAML::Node &defaults) {
    YAML::Node result = node;
    for (auto it : defaults) {
        if (!result[it.first]) {  // Only copy if missing
            result[it.first] = it.second;
        }
    }
    return result;
}

int main() {
    YAML::Node config = YAML::LoadFile("config.yaml");
    YAML::Node default_cfg = config["default"];

    std::vector<DatabaseConfig> all_databases;

    for (auto db_node : config["databases"]) {
        YAML::Node merged = mergeWithDefault(db_node, default_cfg);

        DatabaseConfig db;
        db.name = merged["name"].as<std::string>();
        db.adapter = merged["adapter"].as<std::string>();
        db.host = merged["host"].as<std::string>();
        db.port = merged["port"].as<int>();
        db.db_name = merged["db_name"].as<std::string>();
        db.user = merged["user"].as<std::string>();

        // Load password from environment variable
        std::string env_var = merged["password_env"].as<std::string>();
        const char* pass = std::getenv(env_var.c_str());
        db.password = pass ? pass : "";  // empty string if env var not set

        db.retry.max_attempts = merged["retry"]["max_attempts"].as<int>();
        db.retry.interval_seconds = merged["retry"]["interval_seconds"].as<int>();

        all_databases.push_back(db);
    }

    // Print all DB info
    for (const auto &db : all_databases) {
        std::cout << "====================\n";
        std::cout << "Name: " << db.name << "\n";
        std::cout << "Adapter: " << db.adapter << "\n";
        std::cout << "Host: " << db.host << "\n";
        std::cout << "Port: " << db.port << "\n";
        std::cout << "DB Name: " << db.db_name << "\n";
        std::cout << "User: " << db.user << "\n";
        std::cout << "Password: " << (db.password.empty() ? "<not set>" : "<hidden>") << "\n";
        std::cout << "Retry: " << db.retry.max_attempts 
                  << " attempts, interval " << db.retry.interval_seconds << "s\n";
    }
}
