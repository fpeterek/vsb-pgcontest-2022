//
// Created by fpeterek on 14.01.22.
//

#ifndef VSBPGCONTEST21_SIMILARITY_JOIN_HPP
#define VSBPGCONTEST21_SIMILARITY_JOIN_HPP

#include <cstdint>
#include <string>
#include <limits>

#include "robin_hood.hpp"

#include "record.hpp"
#include "inverted_index.hpp"


class SimilarityJoin {

    // record size -> index
    robin_hood::unordered_flat_map<uint32_t, InvertedIndex> indices;
    // std::vector<uint32_t> table;
    std::vector<std::vector<uint32_t>> tableVector;

    const double threshold;

    std::size_t result = 0;

    std::size_t allPairsForSize(const Record & record, uint32_t size, uint32_t thread);
    void allPairs(const Record & record);

public:
    SimilarityJoin(double threshold);

    uint64_t getResult() const;

    void add(const Record & record);
};

#endif //VSBPGCONTEST21_SIMILARITY_JOIN_HPP
