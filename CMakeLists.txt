cmake_minimum_required(VERSION 3.10)
project(mod-lab06-markov)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Find Google Test
find_package(GTest REQUIRED)
include_directories(${GTEST_INCLUDE_DIRS})

# Shared library
add_library(${PROJECT_NAME} SHARED src/textgen.cpp)
target_include_directories(${PROJECT_NAME} PRIVATE include)
set_target_properties(${PROJECT_NAME} PROPERTIES OUTPUT_NAME "mod-lab06-markov")

# Main executable
add_executable(${PROJECT_NAME}.info src/main.cpp)
target_link_libraries(${PROJECT_NAME}.info ${PROJECT_NAME})

# Test executable
add_executable(${PROJECT_NAME}.test test/tests.cpp)
target_link_libraries(${PROJECT_NAME}.test 
    ${PROJECT_NAME} 
    ${GTEST_LIBRARIES} 
    pthread
)

# Cpplint
find_program(CPPLINT cpplint)
if(CPPLINT)
    add_custom_target(cpplint
        COMMAND ${CPPLINT} --filter=-build/include_subdir,-whitespace/comments,+whitespace/indent --recursive .
        WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}
        COMMENT "Running cpplint"
    )
endif()
