//
// Created by fpeterek on 14.01.22.
//

#include "record_loader.hpp"

#include <fstream>
#include <iostream>
#include <algorithm>



std::vector<Record> RecordLoader::loadRecords(const Query & q) {

    std::ifstream file(q.file, std::ios::binary);
    if (not file) {
        std::cerr << "Could not open " << q.file << std::endl;
        std::exit(1);
    }

    std::vector<Record> records;

    uint32_t buffer[50'000];

    while (true) {
        uint32_t record_size;
        file.read((char*)&record_size, sizeof(record_size));
        if (file.eof()) {
            break;
        }

        file.read((char*)buffer, sizeof(uint32_t) * record_size);

        std::vector<uint32_t> vec { buffer, buffer+record_size };

        std::sort(vec.begin(), vec.end());
        vec.erase(std::unique(vec.begin(), vec.end()), vec.end());

        records.emplace_back(std::move(vec));
    }

    return records;
}

