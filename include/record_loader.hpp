//
// Created by fpeterek on 14.01.22.
//

#ifndef VSBPGCONTEST21_RECORD_LOADER_HPP
#define VSBPGCONTEST21_RECORD_LOADER_HPP

#include <vector>

#include "record.hpp"
#include "query.hpp"


class RecordLoader {

public:
    std::vector<Record> loadRecords(const Query & q);
};

#endif //VSBPGCONTEST21_RECORD_LOADER_HPP
