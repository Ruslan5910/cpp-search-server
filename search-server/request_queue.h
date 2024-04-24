#pragma once
#include <deque>
#include "search_server.h"

class RequestQueue {
public:
    explicit RequestQueue(const SearchServer& search_server);
    template <typename DocumentPredicate>
    std::vector<Document> AddFindRequest(const std::string& raw_query, DocumentPredicate document_predicate);
    std::vector<Document> AddFindRequest(const std::string& raw_query, DocumentStatus status);
    std::vector<Document> AddFindRequest(const std::string& raw_query);
    
    int GetNoResultRequests() const;
    
private:
    struct QueryResult {
        std::string query;
        size_t results_num;
    };
    const SearchServer& search_server_;
    std::deque<QueryResult> requests_;
    const static int min_in_day_ = 1440;
    int no_results_req_;
    int current_time_;
}; 

template <typename DocumentPredicate>
    std::vector<Document> RequestQueue::AddFindRequest(const std::string& raw_query, DocumentPredicate document_predicate) {
        auto result = search_server_.FindTopDocuments(raw_query, document_predicate);
        ++current_time_;
        if (current_time_ <= min_in_day_) {
            requests_.push_front({raw_query, result.size()});
        }
        if (current_time_ > min_in_day_) {
            requests_.pop_back();
            requests_.push_front({raw_query, result.size()});
        }
        if (result.size() == 0) {
            ++no_results_req_;
        }
        return result;
    }