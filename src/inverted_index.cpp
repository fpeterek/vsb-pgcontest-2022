//
// Created by fpeterek on 14.01.22.
//

#include "inverted_index.hpp"


void InvertedIndex::add(const Record & record) {
    for (size_t i = 0; i < record.size(); ++i) {
        map[record[i]].emplace_back(count);
    }
    ++count;
}

/*bool InvertedIndex::attrHasRecord(const uint32_t attr, const uint32_t record) {
    return map[attr].count(record);
}

bool InvertedIndex::recordHasAttr(uint32_t record, uint32_t attr) {
    return attrHasRecord(attr, record);
}*/

uint32_t InvertedIndex::items() const {
    return count;
}

const std::vector<uint32_t> & InvertedIndex::operator[](const uint32_t key) {
    return map[key];
}
