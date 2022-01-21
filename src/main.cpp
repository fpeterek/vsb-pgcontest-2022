//
// Created by fpeterek on 14.01.22.
//

#include <iostream>
#include <chrono>
#include <vector>
#include <cassert>
#include <mutex>

#include "query.hpp"
#include "record_loader.hpp"
#include "similarity_join.hpp"


int main(int, const char **) {
    // Turns off synchronization with C stdio for faster I/O performance
    std::ios::sync_with_stdio(false);

    std::string line;
    std::vector<Query> queries;
    RecordLoader rl;

    std::istream & in = std::cin;

    auto start = std::chrono::system_clock::now();
    while (std::getline(in, line)) {
        Query collection;

        // Only Jaccard Index is supported for now
        assert(line == "JS");

        std::getline(in, line);
        collection.threshold = std::stof(line);

        std::getline(in, collection.file);
        queries.emplace_back(collection);
    }

    struct ResultType {
        size_t order;
        uint64_t result;
    };

    std::vector<ResultType> results;
    std::mutex resMutex;

    #pragma omp parallel for default(none) shared(queries, results, resMutex)
    for (size_t i = 0; i < queries.size(); ++i) {
        const auto & query = queries[i];
        const auto records = RecordLoader().loadRecords(query);
        SimilarityJoin sj { query.threshold };
        for (const auto & record : records) {
            sj.add(record);
        }
        try {
            std::lock_guard lock(resMutex);
            results.emplace_back(ResultType { i, sj.getResult() });
        } catch (const std::exception & e) {
            std::exit(-1);
        }
    }

    std::sort(results.begin(), results.end(),
              [](const ResultType & l, const ResultType & r) -> bool { return l.order < r.order; } );

    for (const auto & res : results) {
        std::cout << res.result << std::endl;
    }

    auto end = std::chrono::system_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count();
    std::cerr << duration << " ms" << std::endl;

    return 0;
}
