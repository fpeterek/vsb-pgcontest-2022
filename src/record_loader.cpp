//
// Created by fpeterek on 14.01.22.
//

#include "record_loader.hpp"

#include <fstream>
#include <iostream>
#include <algorithm>

RecordLoader::RecordLoader(const Query & q, mc::BlockingConcurrentQueue<std::vector<Record>> & queue) :
    in(q.file), queue(queue) {

    if (not in) {
        std::cerr << "Could not open " << q.file << std::endl;
        std::exit(1);
    }
}


bool RecordLoader::loadQuery() {

    const uint32_t record_size = loadSize();

    // Ignore records of size 0
    if (not record_size) {
        return true;
    }

    if (isFinished()) {
        return false;
    }

    auto toCopy = std::min(record_size, (std::uint32_t)availableItems());

    std::vector<uint32_t> vec;
    vec.reserve(record_size);

    loadToVector(vec, toCopy);

    if (toCopy < record_size) {
        toCopy = record_size - toCopy;
        loadToBuffer();
        loadToVector(vec, toCopy);
    }

    std::sort(vec.begin(), vec.end());
    vec.erase(std::unique(vec.begin(), vec.end()), vec.end());

    records.emplace_back(std::move(vec));

    return true;
}

bool RecordLoader::loadQueries() {
    bool res = true;

    constexpr size_t batch_size = 128;

    records.reserve(batch_size);

    for (size_t i = 0; i < batch_size; ++i) {
        if (not loadQuery()) {
            res = false;
            break;
        }
    }

    if (records.size()) {
        queue.enqueue(std::move(records));
        records = std::vector<Record>();
    }

    return res;
}

void RecordLoader::loadToVector(std::vector<std::uint32_t> & dest, const std::uint32_t toCopy) {
    // std::cout << idx << " - " << idx+toCopy << std::endl;
    std::copy(buffer.begin()+idx, buffer.begin()+idx+toCopy, std::back_inserter(dest));
    idx += toCopy;
}

void RecordLoader::loadToBuffer() {
    in.read((char*)buffer.data(), bufferSize*sizeof(std::uint32_t));
    idx = 0;
}

bool RecordLoader::isFinished() const {
    return isEmpty() and lastRead() < buffer.size(); // and (not in or in.eof());
}

bool RecordLoader::isEmpty() const {
    return idx >= lastRead();
}

std::size_t RecordLoader::availableItems() const {
    return lastRead() - idx;
}

std::uint32_t RecordLoader::loadSize() {
    if (isEmpty()) {
        loadToBuffer();
    }
    return buffer[idx++];
}

std::uint32_t RecordLoader::lastRead() const {
    return in.gcount() / sizeof(std::uint32_t);
}


