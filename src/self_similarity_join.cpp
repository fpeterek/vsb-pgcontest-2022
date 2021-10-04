#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <chrono>
#include <cassert>
#include <algorithm>

using Element = uint32_t;
using Record = std::vector<Element>;

std::vector<Record> load_records(std::string filename)
{
    std::ifstream file(filename, std::ios::binary);
    if (!file.is_open()) {
        std::cerr << "Could not open " << filename << std::endl;
        std::exit(1);
    }

    std::vector<Record> records;

    while (true) {
        uint32_t record_size;
        file.read((char*) &record_size, sizeof(record_size));
        if (file.eof()) break;

        Record record;
        for (uint32_t i = 0; i < record_size; i++)
        {
            Element element;
            file.read((char*) &element, sizeof(element));
            record.push_back(element);
        }

        // Sort elements
        std::sort(record.begin(), record.end());

        // Remove duplicates (https://stackoverflow.com/a/1041939/1107768)
        record.erase(std::unique(record.begin(), record.end()), record.end());
        records.push_back(record);
    }

    return records;
}

struct OverlapResult {
    uint64_t intersection_count{};
    uint64_t union_count{};
};

/**
 * Compute overlap of two sets using sorted set intersection/union count.
 * Assumes that the records are sorted and do not contain duplicate elements.
 */
OverlapResult compute_overlap(const Record& a, const Record& b)
{
    OverlapResult overlap{};

    auto iter_a = a.begin();
    auto iter_b = b.begin();
    while (iter_a != a.end() && iter_b != b.end()) {
        if (*iter_a == *iter_b) {
            overlap.intersection_count++;
            iter_a++;
            iter_b++;
        }
        else if (*iter_a < *iter_b) {
            iter_a++;
        } else {
            iter_b++;
        }
        overlap.union_count++;
    }
    while (iter_a != a.end()) {
        overlap.union_count += 1;
        iter_a++;
    }
    while (iter_b != b.end()) {
        overlap.union_count += 1;
        iter_b++;
    }

    return overlap;
}

uint64_t compute_distances(std::vector<Record> records, double threshold)
{
    uint64_t result = 0;

    for (size_t i = 0; i < records.size(); i++)
    {
        for (size_t j = i + 1; j < records.size(); j++)
        {
            const auto& record_a = records[i];
            const auto& record_b = records[j];
            auto overlap = compute_overlap(record_a, record_b);

            float jaccard_index = (float) overlap.intersection_count / (float) overlap.union_count;
            if (jaccard_index >= threshold) {
                result += overlap.intersection_count;
            }
        }
    }

    return result;
}

struct Query
{
    std::string file;
    float threshold{};
};

void print_records(const std::vector<Record>& records)
{
    for (const auto& record: records)
    {
        for (size_t i = 0; i < record.size(); i++)
        {
            std::cerr << record[i];
            if (i < records.size() - 1)
            {
                std::cerr << " ";
            }
        }

        std::cerr << std::endl;
    }
}

int main()
{
	// Turns off synchronization with C stdio for faster I/O performance
	std::ios::sync_with_stdio(false);

	std::string line;
	std::vector<Query> queries;

    std::istream& in = std::cin;

	auto start = std::chrono::system_clock::now();
	while (std::getline(in, line))
	{
		Query collection;

        // Only Jaccard Index is supported for now
        assert(line == "JS");

		std::getline(in, line);
        collection.threshold = std::stof(line);

		std::getline(in, collection.file);
        queries.push_back(collection);
	}

	for (const auto& query : queries)
	{
		auto records = load_records(query.file);
		std::cout << compute_distances(records, query.threshold) << std::endl;
	}

	auto end = std::chrono::system_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count();
    std::cerr << duration << " ms" << std::endl;

	return 0;
}
