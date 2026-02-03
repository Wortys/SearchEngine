#include <iostream>
#include "ConverterJSON.h"
#include "InvertedIndex.h"
#include "SearchServer.h"

int main() {
    try {
        ConverterJSON converter;
        auto docs = converter.GetTextDocuments();
        auto requests = converter.GetRequests();
        int maxResponses = converter.GetResponsesLimit();

        if (docs.empty()) {
            std::cerr << "No documents loaded!" << std::endl;
            return 0;
        }

        InvertedIndex idx;
        idx.UpdateDocumentBase(docs);
        std::cout << "Indexing finished." << std::endl;

        SearchServer srv(idx);
        auto searchResults = srv.search(requests);
        std::cout << "Search finished." << std::endl;

        std::vector<std::vector<std::pair<int, float>>> answers;
        
        for (const auto& result : searchResults) {
            std::vector<std::pair<int, float>> converted_result;
            int count = 0;
            for (const auto& relIndex : result) {
                if (count >= maxResponses) break;
                converted_result.push_back({(int)relIndex.doc_id, relIndex.rank});
                count++;
            }
            answers.push_back(converted_result);
        }

        converter.putAnswers(answers);
        std::cout << "Results saved to answers.json" << std::endl;

    } catch (const std::exception& e) {
        std::cerr << "Critical error: " << e.what() << std::endl;
    }
    return 0;
}