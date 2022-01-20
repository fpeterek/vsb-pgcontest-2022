#include <cstdint>
#include <cstddef>


void insert_into(uint32_t * const arr, size_t & items, const uint32_t value) {
    if (not items) {
        arr[0] = value;
        ++items;
        return;
    }

    size_t left = 0;
    size_t right = items-1;
    size_t index = (left + right) / 2;

    while (left < right) {
        if (arr[index] == value) {
            return;
        }
        if (arr[index] < value) {
            left = index + 1;
        } else {
            right = index;
        }
        index = (left + right) / 2;
    }

    index += 1 * (arr[index] < value);

    if (arr[index] == value) {
        return;
    }

    for (size_t i = items; i > index; --i) {
        arr[i] = arr[i-1];
    }
    arr[index] = value;
    ++items;
}


