cmake_minimum_required (VERSION 3.13)

project (uploadFile CXX)

find_package(azure-storage-blobs-cpp CONFIG REQUIRED)

add_executable(uploadFile uploadFile.cpp)

target_link_libraries(uploadFile PRIVATE Azure::azure-storage-blobs)