#pragma once
#include <algorithm>
#include <vector>
#include <iostream>

template <typename Iterator>
class IteratorRange {
public:
    IteratorRange(Iterator begin, Iterator end) :
    begin_(begin),
    end_(end),
    size_(distance(begin, end))
    {
    }
    
    Iterator begin() const {
        return begin_;
    }
    
    Iterator end() const {
        return end_;
    }
    
    size_t size() const {
        return size_;
    }
    
private:
    Iterator begin_;
    Iterator end_;
    size_t size_;
};

template <typename Iterator>
std::ostream& operator<<(std::ostream& out, IteratorRange<Iterator> range) {
    for (Iterator it = range.begin(); it != range.end(); ++it) {
        out << *it;
    }
    return out;
}

template <typename Iterator>
class Paginator {
public:
    Paginator(Iterator begin, Iterator end, size_t page_size) {
        for (Iterator it = begin; it != end; ++ it) {
            if (distance(begin, it) == page_size) {
                pages_.push_back(IteratorRange<Iterator>{begin, it});
                begin = it;
            } 
        }
        pages_.push_back(IteratorRange<Iterator>{begin, end});
    }
      
   auto begin() const {
        return pages_.begin();
    }
        
    auto end() const {
        return pages_.end();
    }
    
private:
    std::vector<IteratorRange<Iterator>> pages_;
};

template <typename Container>
auto Paginate(const Container& c, size_t page_size) {
    return Paginator(begin(c), end(c), page_size);
}