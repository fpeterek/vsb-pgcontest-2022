//
// Created by fpeterek on 14.01.22.
//

#ifndef VSBPGCONTEST21_INVERTED_INDEX_HPP
#define VSBPGCONTEST21_INVERTED_INDEX_HPP

#include <unordered_set>
#include <vector>
#include <limits>

#include "robin_hood.hpp"

#include "record.hpp"


class InvertedIndex {

    robin_hood::unordered_flat_map<uint32_t, std::vector<uint32_t>> map;
    std::vector<uint32_t> sizes;
    uint32_t count = 0;

public:
    typedef decltype(map)::const_iterator const_iterator;
    void add(const Record & record);
    // bool attrHasRecord(uint32_t attr, uint32_t record);
    // bool recordHasAttr(uint32_t record, uint32_t attr);
    uint32_t items() const;

    const std::vector<uint32_t> & operator[](uint32_t key);

    const_iterator find(uint32_t key) const;
    const_iterator cbegin() const;
    const_iterator cend() const;

    std::uint32_t sizeOf(uint32_t key);
};


#endif //VSBPGCONTEST21_INVERTED_INDEX_HPP
