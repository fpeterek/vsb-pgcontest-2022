//
// Created by fpeterek on 12.01.22.
//

#ifndef VSBPGCONTEST21_RECORD_HPP
#define VSBPGCONTEST21_RECORD_HPP

#include <cstdint>
#include <vector>


class Record {
    std::vector<uint32_t> elements;

public:

    std::size_t size() const;
    std::uint32_t operator[](std::size_t index) const;

    Record(std::vector<uint32_t> data);
    Record(Record && other) noexcept;

    Record & operator=(Record && other) noexcept;
};

#endif //VSBPGCONTEST21_RECORD_HPP
