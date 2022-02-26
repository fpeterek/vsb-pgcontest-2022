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

    std::ifstream in;
    mc::BlockingConcurrentQueue<std::vector<Record>> & queue;
    uint32_t buffer[50'000]{};
    std::vector<Record> records;

public:

    RecordLoader(const Query & q, mc::BlockingConcurrentQueue<std::vector<Record>> & queue);

    bool loadQuery();
    bool loadQueries();
};

#endif //VSBPGCONTEST21_RECORD_LOADER_HPP
