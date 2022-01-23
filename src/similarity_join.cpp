//
// Created by fpeterek on 14.01.22.
//

#include "similarity_join.hpp"

#include <cmath>
#include <functional>
#include <mutex>
#include <iostream>

#include <omp.h>


SimilarityJoin::SimilarityJoin(const double threshold) : threshold(threshold) {
    for (std::uint32_t i = 1; i <= 50'000; ++i) {
        indices[i] = {};
    }
    tableVector.resize(omp_get_max_threads());
}

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

    std::mutex mutex;

    #pragma omp parallel for default(none) shared(minSize, maxSize, record, mutex, result, std::cout)
    for (std::uint32_t i = minSize; i <= maxSize; ++i) {
        const auto subresult = allPairsForSize(record, i, omp_get_thread_num());

        std::lock_guard lock(mutex);
        result += subresult;
    }
}

std::size_t SimilarityJoin::allPairsForSize(const Record & record, const uint32_t size, const uint32_t thread) {
    auto & index = indices[size];
    const auto numElements = index.items();

    std::size_t sum = 0;

    if (not numElements) {
        return 0;
    }

    std::vector<uint32_t> & table = tableVector[thread];

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
            sum += intersect;
        }
    }

    return sum;
}
