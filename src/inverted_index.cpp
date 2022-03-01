//
// Created by fpeterek on 14.01.22.
//

#include "inverted_index.hpp"


void InvertedIndex::add(const Record & record) {
    for (size_t i = 0; i < record.size(); ++i) {
        map[record[i]].emplace_back(count);
    }
    sizes.emplace_back(record.size());
    ++count;
}

uint32_t InvertedIndex::items() const {
    return count;
}

const std::vector<uint32_t> & InvertedIndex::operator[](const uint32_t key) {
    return map[key];
}

std::uint32_t InvertedIndex::sizeOf(uint32_t key) {
    return sizes[key];
}

InvertedIndex::const_iterator InvertedIndex::find(const uint32_t key) const {
    return map.find(key);
}

InvertedIndex::const_iterator InvertedIndex::cbegin() const {
    return map.cbegin();
}

InvertedIndex::const_iterator InvertedIndex::cend() const {
    return map.cend();
}
