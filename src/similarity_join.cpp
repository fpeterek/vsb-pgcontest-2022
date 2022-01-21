//
// Created by fpeterek on 14.01.22.
//

#include "similarity_join.hpp"

#include <cmath>


SimilarityJoin::SimilarityJoin(const double threshold) : threshold(threshold) { }

uint64_t SimilarityJoin::getResult() const {
    return result;
}

void SimilarityJoin::add(const Record & record) {
    allPairs(record);
    indices[record.size()].add(record);
}

void SimilarityJoin::allPairs(const Record & record) {


    // Hack allowing me to circumvent rounding issues...
    const float minDiv = record.size() * threshold;
    const float minFloor = std::floor(minDiv);
    const std::uint32_t minSize = (minDiv - minFloor) < 0.0005 ? minFloor : std::ceil(minDiv);

    // Hack allowing me to circumvent rounding issues...
    const float maxDiv = record.size() / threshold;
    const float maxFloor = std::floor(maxDiv);
    const std::uint32_t maxSize = (maxDiv - maxFloor) > 0.9995f ? std::round(maxDiv) : maxFloor;

    for (std::uint32_t i = minSize; i <= maxSize; ++i) {
        allPairsForSize(record, i);
    }
}

void SimilarityJoin::allPairsForSize(const Record & record, const uint32_t size) {
    auto & index = indices[size];
    const auto numElements = index.items();

    if (not numElements) {
        return;
    }

    if (table.size() > numElements) {
        std::fill(table.begin(), table.begin() + numElements, 0);
    } else {
        std::fill(table.begin(), table.end(), 0);
        table.resize(numElements, 0);
    }

    for (std::size_t i = 0; i < record.size(); ++i) {
        const auto attribute = record[i];
        for (const auto item : index[attribute]) {
            ++table[item];
        }
    }

    for (std::size_t i = 0; i < numElements; ++i) {
        const auto intersect = table[i];
        const auto unionSize = record.size() + size - intersect;
        const float jaccard = (float)intersect / unionSize;

        if (jaccard >= threshold) {
            result += intersect;
        }
    }

}
