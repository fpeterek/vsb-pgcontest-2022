//
// Created by fpeterek on 12.01.22.
//

#include "record.hpp"


Record::Record(std::vector<uint32_t> data) : elements(std::move(data)) { }

Record::Record(Record && other) noexcept {
    elements = std::move(other.elements);
}

Record & Record::operator=(Record && other) noexcept {
    elements = std::move(other.elements);
    return *this;
}

std::uint32_t Record::operator[](const std::size_t index) const {
    return elements[index];
}

std::size_t Record::size() const {
    return elements.size();
}

bool Record::empty() const {
    return elements.empty();
}



