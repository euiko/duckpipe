project(duckpipe)
cmake_minimum_required(VERSION 3.17)

if(POLICY CMP0003)
    cmake_policy(SET CMP0003 NEW)
endif()

if(POLICY CMP0012)
    cmake_policy(SET CMP0012 NEW)
endif()

option(BUILD_STATIC "Build static binary (only support static compiler usually with musl-libc)" OFF)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(THIRDPARTY_DIR thirdparty)

find_package(Threads REQUIRED)
find_package(c-ares REQUIRED)
find_package(OpenSSL REQUIRED)
find_package(Drogon CONFIG REQUIRED)
find_package(Boost REQUIRED COMPONENTS program_options)
find_package(unofficial-brotli CONFIG REQUIRED)

set(ENABLE_SANITIZER OFF)
set(ENABLE_UBSAN OFF)
set(EXTENSION_STATIC_BUILD ON)
set(BUILD_PARQUET_EXTENSION ON)
set(BUILD_HTTPFS_EXTENSION ON)
set(BUILD_JSON_EXTENSION ON)
set(BUILD_JEMALLOC_EXTENSION OFF)
set(BUILD_EXCEL_EXTENSION ON)
set(BUILD_UNITTESTS OFF)
set(DUCKDB_LIBRARIES duckdb)
set(DUCKDB_INCLUDE_DIR ${THIRDPARTY_DIR}/duckdb/src/include)
add_subdirectory(${THIRDPARTY_DIR}/duckdb)

add_executable(duckpipe
    src/main.cpp
    src/app.cpp
)

if(BUILD_STATIC)
    set(BUILD_STATIC_LIBRARIES -static)
    set(DUCKDB_LIBRARIES duckdb_static)
endif(BUILD_STATIC)

# set target alias to enable linking with drogon
add_library(c-ares_lib ALIAS c-ares::cares)
add_library(Brotli_lib ALIAS unofficial::brotli::brotlienc)

target_include_directories(duckpipe PRIVATE
    include
    ${Boost_INCLUDE_DIR}
    ${DUCKDB_INCLUDE_DIR}
)
target_link_libraries(duckpipe PRIVATE
    ${BUILD_STATIC_LIBRARIES}
    Threads::Threads
    c-ares::cares
    unofficial::brotli::brotlidec
    ${OPENSSL_LIBRARIES}
    ${DROGON_LIBRARIES}
    ${DUCKDB_LIBRARIES}
)
