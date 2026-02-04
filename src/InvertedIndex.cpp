#include "InvertedIndex.h"
#include <thread>
#include <sstream>
#include <algorithm>
#include <mutex>

void InvertedIndex::UpdateDocumentBase(std::vector<std::string> input_docs) {
    docs = input_docs;
    freq_dictionary.clear();
    
    std::vector<std::thread> threads;
    std::mutex mtx;
    auto processDocument = [this, &mtx](size_t doc_id, const std::string& content) {
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
        if (t.joinable()) {
            t.join();
        }
    }

    for (auto& [word, entries] : freq_dictionary) {
        std::sort(entries.begin(), entries.end(), [](const Entry& a, const Entry& b) {
            return a.doc_id < b.doc_id;
        });
    }

}

std::vector<Entry> InvertedIndex::GetWordCount(const std::string& word) {
    if (freq_dictionary.count(word)) {
        return freq_dictionary[word];
    }
    return {};
}