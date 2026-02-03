#include "SearchServer.h"
#include <sstream>
#include <set>
#include <algorithm>
#include <cmath>
#include <map>

std::vector<std::vector<RelativeIndex>> SearchServer::search(const std::vector<std::string>& queries_input) {
    std::vector<std::vector<RelativeIndex>> result;

    for (const auto& query : queries_input) {
        std::set<std::string> unique_words;
        std::istringstream iss(query);
        std::string word;
        while (iss >> word) {
            unique_words.insert(word);
        }

        std::map<size_t, size_t> doc_relevance;
        size_t max_relevance = 0;

        for (const auto& w : unique_words) {
            auto entries = _index.GetWordCount(w);
            for (const auto& entry : entries) {
                doc_relevance[entry.doc_id] += entry.count;
                if (doc_relevance[entry.doc_id] > max_relevance) {
                    max_relevance = doc_relevance[entry.doc_id];
                }
            }
        }

        std::vector<RelativeIndex> ranked_result;
        for (const auto& [doc_id, abs_score] : doc_relevance) {
            float rank = 0.0f;
            if (max_relevance > 0) {
                rank = static_cast<float>(abs_score) / max_relevance;
            }
            ranked_result.push_back({doc_id, rank});
        }

        std::sort(ranked_result.begin(), ranked_result.end(), [](const RelativeIndex& a, const RelativeIndex& b) {
            if (std::abs(a.rank - b.rank) > 1e-6) {
                return a.rank > b.rank;
            }
            return a.doc_id < b.doc_id;
        });

        result.push_back(ranked_result);
    }

    return result;
}