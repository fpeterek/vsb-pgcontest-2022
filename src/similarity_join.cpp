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
    tableVector.resize(omp_get_max_threads());

    splits.emplace_back(std::pair { 1u, 1u });

    uint32_t last = 1;
    const double splitRatio = threshold + (1 - threshold)*0.7;
    while (last < 50'000) {
        const auto begin = last+1;
        const auto end = std::min(std::uint32_t(begin / splitRatio), 50'000u);
        splits.emplace_back(std::pair { begin, end });
        last = end;
        indices.emplace_back();
    }
}

uint64_t SimilarityJoin::getResult() const {
    return result;
}

void SimilarityJoin::add(const Record & record) {
    allPairs(record);
    size_t split = 0;
    while (true) {
        const auto & [begin, end] = splits[split];
        if (record.size() < begin) {
            break;
        }
        ++split;
    }
    indices[split].add(record);
}

void SimilarityJoin::allPairs(const Record & record) {

    constexpr float error = std::numeric_limits<float>::epsilon() * 2;
    constexpr float oneMinusError = 1 - error;

    // Hack allowing me to circumvent rounding issues...
    const float minDiv = record.size() * threshold;
    const float minFloor = std::floor(minDiv);
    const std::uint32_t minSize = (minDiv - minFloor) < error ? minFloor : std::ceil(minDiv);

    // Hack allowing me to circumvent rounding issues...
    const float maxDiv = record.size() / threshold;
    const float maxFloor = std::floor(maxDiv);
    const std::uint32_t maxSize = (maxDiv - maxFloor) > oneMinusError ? std::round(maxDiv) : maxFloor;

    std::mutex mutex;

    size_t begin = 0;
    size_t end = 0;

    for (const auto & [low, high] : splits) {
        if (low <= maxSize) {
            ++end;
        }
        if (high < minSize) {
            ++begin;
        }
    }

    #pragma omp parallel for default(none) shared(begin, end, record, mutex, result, std::cout)
    for (std::uint32_t i = begin; i <= end; ++i) {
        const auto subresult = allPairsForSize(record, i, omp_get_thread_num());

        std::lock_guard lock(mutex);
        result += subresult;
    }
}

std::size_t SimilarityJoin::allPairsForSize(const Record & record, const uint32_t split, const uint32_t thread) {
    auto & index = indices[split];
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

    // #pragma omp parallel for default(none) shared(record, index, table)
    for (std::size_t i = 0; i < record.size(); ++i) {
        const auto attribute = record[i];
        for (const auto item : index[attribute]) {
            ++table[item];
        }
    }

    // #pragma omp parallel for reduction(+:sum) default(none) shared(record, index, table, numElements)
    for (std::size_t i = 0; i < numElements; ++i) {
        const auto intersect = table[i];
        const auto unionSize = record.size() + index.sizeOf(i) - intersect;
        const float jaccard = (float)intersect / unionSize;

        if (jaccard >= threshold) {
            sum += intersect;
        }
    }

    return sum;
}

void SimilarityJoin::printIndices() const {
    std::size_t size = 1;
    for (const auto & index : indices) {
        if (index.items()) {
            std::cout << size << ": " << index.items() << std::endl;
        }
        ++size;
    }
}
