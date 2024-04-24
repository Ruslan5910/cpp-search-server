#include "document.h"

Document::Document(int id, double relevance, int rating)
    : id(id)
    , relevance(relevance)
    , rating(rating) {
    }

std::ostream& operator<<(std::ostream& out, Document document) {
    out << std::string("{ document_id = ");
    out << document.id << std::string(", relevance = ") << document.relevance << std::string(", rating = ") << document.rating;
    out << std::string(" }");
    return out;
}