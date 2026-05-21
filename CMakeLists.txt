cmake_minimum_required(VERSION 3.10)

project(RingBuffer VERSION 1.0.0 LANGUAGES C CXX)


set(SOURCE_FILES 
    # src/main.c 
    src/ring.c
    src/utils.c)

add_library(${PROJECT_NAME} STATIC ${SOURCE_FILES})
# add_executable(${PROJECT_NAME} ${SOURCE_FILES})

target_include_directories(${PROJECT_NAME} PRIVATE ${CMAKE_SOURCE_DIR}/include)