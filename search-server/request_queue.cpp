#include "request_queue.h"

RequestQueue::RequestQueue(const SearchServer& search_server)
    : search_server_(search_server)
    , no_results_req_(0)
    , current_time_(0)
    {
    }

std::vector<Document> RequestQueue::AddFindRequest(const std::string& raw_query, DocumentStatus status) {
    auto result = search_server_.FindTopDocuments(raw_query, status);
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

std::vector<Document> RequestQueue::AddFindRequest(const std::string& raw_query) {
    auto result = search_server_.FindTopDocuments(raw_query);
    ++current_time_;
    if (current_time_ <= min_in_day_) {
        requests_.push_front({raw_query, result.size()});
    }
    if (current_time_ > min_in_day_) {
        requests_.pop_back();
        requests_.push_front({raw_query, result.size()});
        --no_results_req_;
    }
    if (result.size() == 0) {
        ++no_results_req_;
    }
    return result;
}

int RequestQueue::GetNoResultRequests() const {
    return no_results_req_;
}