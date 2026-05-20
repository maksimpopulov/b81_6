cmake_minimum_required(VERSION 3.10)
project(mod-lab06-markov)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(header_path ${PROJECT_SOURCE_DIR}/include)
set(header ${header_path}/textgen.h)
set(src ${PROJECT_SOURCE_DIR}/src/textgen.cpp)

add_library(${PROJECT_NAME} SHARED
    ${src}
)

target_include_directories(${PROJECT_NAME}
    PUBLIC ${header_path}
)

add_executable(${PROJECT_NAME}.info ${PROJECT_SOURCE_DIR}/src/main.cpp)
target_link_libraries(${PROJECT_NAME}.info ${PROJECT_NAME})

add_executable(${PROJECT_NAME}.test ${PROJECT_SOURCE_DIR}/test/tests.cpp)
target_link_libraries(${PROJECT_NAME}.test ${PROJECT_NAME})
