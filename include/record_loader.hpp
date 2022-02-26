//
// Created by fpeterek on 14.01.22.
//

#ifndef VSBPGCONTEST21_RECORD_LOADER_HPP
#define VSBPGCONTEST21_RECORD_LOADER_HPP

#include <vector>
#include <fstream>
#include <variant>

#include <blockingconcurrentqueue.h>

#include "record.hpp"
#include "query.hpp"

namespace mc = moodycamel;

class RecordLoader {

    // 1 MB worth of 32 bit integers
    static constexpr std::size_t bufferSize = 1024*1024 / sizeof(std::uint32_t);

    std::ifstream in;
    mc::BlockingConcurrentQueue<std::vector<Record>> & queue;
    std::vector<Record> records;
    std::array<std::uint32_t, bufferSize> buffer;
    std::size_t idx = bufferSize;

    void loadToVector(std::vector<std::uint32_t> & dest, std::uint32_t toCopy);
    void loadToBuffer();
    std::uint32_t loadSize();

    std::uint32_t lastRead() const;
    std::size_t availableItems() const;
    bool isEmpty() const;
    bool isFinished() const;

public:

    RecordLoader(const Query & q, mc::BlockingConcurrentQueue<std::vector<Record>> & queue);

    bool loadQuery();
    bool loadQueries();
};

#endif //VSBPGCONTEST21_RECORD_LOADER_HPP
