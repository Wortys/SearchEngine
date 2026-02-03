#include "InvertedIndex.h"
#include <thread>
#include <sstream>
#include <algorithm>

void InvertedIndex::UpdateDocumentBase(std::vector<std::string> input_docs) {
    docs = input_docs;
    freq_dictionary.clear();
    
    std::vector<std::thread> threads;

    auto processDocument = [this](size_t doc_id, const std::string& content) {
        std::map<std::string, size_t> local_word_count;
        std::istringstream iss(content);
        std::string word;

        while (iss >> word) {
            local_word_count[word]++;
        }

        std::lock_guard<std::mutex> lock(mtx); 
        for (const auto& [word, count] : local_word_count) {
            Entry entry{doc_id, count};
            freq_dictionary[word].push_back(entry);
        }
    };

    for (size_t i = 0; i < input_docs.size(); ++i) {
        threads.emplace_back(processDocument, i, input_docs[i]);
    }

    for (auto& t : threads) {
        t.join();
    }
}

std::vector<Entry> InvertedIndex::GetWordCount(const std::string& word) {
    if (freq_dictionary.count(word)) {
        return freq_dictionary[word];
    }
    return {};
}