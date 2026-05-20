cmake_minimum_required(VERSION 3.10)
project(mod-lab06-markov)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Пути к файлам
set(header_path ${PROJECT_SOURCE_DIR}/include)
set(src_path ${PROJECT_SOURCE_DIR}/src)

# Библиотека
add_library(${PROJECT_NAME} SHARED
    ${src_path}/textgen.cpp
)

target_include_directories(${PROJECT_NAME}
    PUBLIC ${header_path}
    PRIVATE ${src_path}
)

# Исполняемый файл генератора
add_executable(${PROJECT_NAME}.info
    ${src_path}/main.cpp
)
target_link_libraries(${PROJECT_NAME}.info ${PROJECT_NAME})

# Исполняемый файл тестов
add_executable(${PROJECT_NAME}.test
    ${PROJECT_SOURCE_DIR}/test/tests.cpp
)
target_link_libraries(${PROJECT_NAME}.test ${PROJECT_NAME})

# Для совместимости со старыми скриптами - создаём симлинк
add_custom_command(TARGET ${PROJECT_NAME}.test POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E create_symlink
        ${CMAKE_CURRENT_BINARY_DIR}/${PROJECT_NAME}.test
        ${CMAKE_CURRENT_BINARY_DIR}/test/${PROJECT_NAME}.test
    DEPENDS ${PROJECT_NAME}.test
)

# Создаём папку test в build
file(MAKE_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/test)

# Включаем тестирование
enable_testing()
add_test(NAME test_markov COMMAND ${PROJECT_NAME}.test)
