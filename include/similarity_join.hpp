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
    std::vector<InvertedIndex> indices;
    std::vector<std::vector<uint32_t>> tableVector;

    std::vector<std::pair<std::uint32_t, std::uint32_t>> splits;
    std::vector<std::uint32_t> splitCounters;

    const double threshold;

    std::size_t result = 0;

    std::size_t allPairsForSize(const Record & record, uint32_t split, uint32_t thread);
    void allPairs(const Record & record);

public:
    SimilarityJoin(double threshold);

    uint64_t getResult() const;

    void add(const Record & record);
    void index(const Record & record);

    void printIndices() const;
};

#endif //VSBPGCONTEST21_SIMILARITY_JOIN_HPP
