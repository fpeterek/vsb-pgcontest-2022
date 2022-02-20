//
// Created by fpeterek on 14.01.22.
//

#include "record_loader.hpp"

#include <fstream>
#include <iostream>
#include <algorithm>

RecordLoader::RecordLoader(const Query & q, mc::BlockingConcurrentQueue<Record> & queue) :
    in(q.file), queue(queue) {

    if (not in) {
        std::cerr << "Could not open " << q.file << std::endl;
        std::exit(1);
    }
}


bool RecordLoader::loadQuery() {

    uint32_t record_size;
    in.read((char*)&record_size, sizeof(record_size));
    if (in.eof()) {
        return false;
    }

    // Ignore records of size 0
    if (not record_size) {
        return true;
    }

    in.read((char*)buffer, sizeof(uint32_t) * record_size);

    std::vector<uint32_t> vec { buffer, buffer+record_size };

    std::sort(vec.begin(), vec.end());
    vec.erase(std::unique(vec.begin(), vec.end()), vec.end());

    queue.enqueue(Record(std::move(vec)));

    return true;
}


