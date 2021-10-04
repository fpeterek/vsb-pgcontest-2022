cmake_minimum_required(VERSION 2.6)
project(VSBPGContest21)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fopenmp -fno-rtti -Wall")
set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -g -fsanitize=address")
set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} -g -flto -ftree-vectorize -march=native")

add_executable(contest src/self_similarity_join.cpp)
