#include "ConverterJSON.h"
#include <filesystem>

using json = nlohmann::json;

std::vector<std::string> ConverterJSON::GetTextDocuments() {
    std::vector<std::string> documents;
    
    std::ifstream file("config.json");
    if (!file.is_open()) {
        throw std::runtime_error("Config file is missing");
    }

    json config;
    file >> config;

    if (!config.contains("config")) {
        throw std::runtime_error("Config file is empty");
    }

    std::cout << "Starting " << config["config"]["name"] << "..." << std::endl;

    for (const auto& f : config["files"]) {
        std::string path = f;
        std::ifstream docFile(path);
        if (docFile.is_open()) {
            std::string content((std::istreambuf_iterator<char>(docFile)), std::istreambuf_iterator<char>());
            documents.push_back(content);
        } else {
            std::cerr << "File not found: " << path << std::endl;
        }
    }
    return documents;
}

int ConverterJSON::GetResponsesLimit() {
    std::ifstream file("config.json");
    if (!file.is_open()) return 5;

    json config;
    file >> config;
    
    if (config["config"].contains("max_responses")) {
        return config["config"]["max_responses"];
    }
    return 5;
}

std::vector<std::string> ConverterJSON::GetRequests() {
    std::vector<std::string> requests;
    std::ifstream file("requests.json");
    if (!file.is_open()) return requests;

    json j;
    file >> j;
    
    if (j.contains("requests")) {
        for (const auto& req : j["requests"]) {
            requests.push_back(req);
        }
    }
    return requests;
}

void ConverterJSON::putAnswers(std::vector<std::vector<std::pair<int, float>>> answers) {
    json jAnswers;
    
    for (int i = 0; i < answers.size(); ++i) {
        std::string requestId = "request" + std::string(3 - std::to_string(i + 1).length(), '0') + std::to_string(i + 1);
        
        json jEntry;
        if (answers[i].empty()) {
            jEntry["result"] = "false";
        } else {
            jEntry["result"] = "true";
            json jRelevance = json::array();
            
            for (const auto& pair : answers[i]) {
                json jRank;
                jRank["docid"] = pair.first;
                jRank["rank"] = pair.second; 
                jRelevance.push_back(jRank);
            }
            jEntry["relevance"] = jRelevance;
        }
        jAnswers["answers"][requestId] = jEntry;
    }

    std::ofstream file("answers.json");
    file << std::setw(4) << jAnswers;
}