cmake_minimum_required(VERSION 3.15)
project(aws-loabot NONE)

# dependency
include(FetchContent)

## dependency: aws-lambda-runtime
find_package(aws-lambda-runtime)

## dependency: aws-sdk-cpp
find_package(AWSSDK REQUIRED COMPONENTS lambda)

# subprojects
add_subdirectory(aws-loabot-handler)
add_subdirectory(aws-loabot-router)