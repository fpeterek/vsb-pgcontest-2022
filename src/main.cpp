//
// Created by fpeterek on 14.01.22.
//

#include <iostream>
#include <chrono>
#include <vector>
#include <cassert>
#include <mutex>
#include <thread>

#include <blockingconcurrentqueue.h>

#include "query.hpp"
#include "record_loader.hpp"
#include "similarity_join.hpp"


namespace mc = moodycamel;


std::vector<Query> loadQueries() {

    std::string line;
    std::vector<Query> queries;

    while (std::getline(std::cin, line)) {
        Query collection;

        // Only Jaccard Index is supported for now
        assert(line == "JS");

        std::getline(std::cin, line);
        collection.threshold = std::stof(line);

        std::getline(std::cin, collection.file);
        queries.emplace_back(collection);
    }

    return queries;
}

void loadRecords(const Query & query, mc::BlockingConcurrentQueue<std::vector<Record>> & queue) {
    RecordLoader rl { query, queue };
    while (rl.loadQueries());
    queue.enqueue(std::vector<Record>{});
}

void handleQuery(const Query & query) {

    mc::BlockingConcurrentQueue<std::vector<Record>> queue;
    std::thread loaderThread(loadRecords, std::ref(query), std::ref(queue));

    SimilarityJoin sj { query.threshold };
    std::vector<Record> records {};

    while (true) {
        queue.wait_dequeue(records);

        if (records.empty()) {
            break;
        }

        for (const auto & rec : records) {
            sj.add(rec);
        }
    }

    loaderThread.join();

    // sj.printIndices();
    std::cout << sj.getResult() << std::endl;
}


int main(int, const char **) {
    // Turns off synchronization with C stdio for faster I/O performance
    std::ios::sync_with_stdio(false);

    std::vector<Query> queries = loadQueries();

    auto start = std::chrono::system_clock::now();

    for (auto & query : queries) {
        handleQuery(query);
    }

    auto end = std::chrono::system_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count();
    std::cerr << duration << " ms" << std::endl;

    return 0;
}
