cmake_minimum_required(VERSION 2.6)
project(VSBPGContest21)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fopenmp -fno-rtti -Wall")
set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -g -fsanitize=address")
set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} -g -flto -ftree-vectorize -march=native")

include_directories(include/ borrowed/)

add_executable(
    contest
    src/self_similarity_join.cpp
    include/record.hpp
    src/record.cpp
    borrowed/robin_hood.hpp
    src/main.cpp include/query.hpp include/record_loader.hpp src/record_loader.cpp include/similarity_join.hpp src/similarity_join.cpp include/inverted_index.hpp src/inverted_index.cpp include/blocking_queue.hpp include/end.hpp)
