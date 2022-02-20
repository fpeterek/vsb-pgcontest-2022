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
    mc::BlockingConcurrentQueue<Record> & queue;
    uint32_t buffer[50'000]{};

public:

    RecordLoader(const Query & q, mc::BlockingConcurrentQueue<Record> & queue);

    bool loadQuery();
};

#endif //VSBPGCONTEST21_RECORD_LOADER_HPP
