cmake_minimum_required(VERSION 3.13)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_INSTALL_PREFIX ${CMAKE_SOURCE_DIR}/install)
project(aws-loabot-router)

# dependency
include(FetchContent)

## dependency: aws-lambda-runtime
find_package(aws-lambda-runtime)

## dependency: aws-sdk-cpp
find_package(AWSSDK REQUIRED COMPONENTS lambda)

## dependency: sodium
FetchContent_Declare(Sodium
    GIT_REPOSITORY https://github.com/robinlinden/libsodium-cmake.git
)
FetchContent_GetProperties(Sodium)
if(NOT sodium_POPULATED)
  FetchContent_Populate(Sodium)
  add_subdirectory(${sodium_SOURCE_DIR} ${sodium_BINARY_DIR})
endif()

# project
add_executable(${PROJECT_NAME}
    src/main.cpp
    src/validate.cpp
    src/message.cpp
    src/router.cpp
    src/loabot_router.cpp)

target_include_directories(${PROJECT_NAME} PRIVATE include)
target_link_libraries(${PROJECT_NAME} PRIVATE AWS::aws-lambda-runtime ${AWSSDK_LINK_LIBRARIES} sodium)
target_compile_features(${PROJECT_NAME} PRIVATE "cxx_std_17")
target_compile_options(${PROJECT_NAME} PRIVATE -pedantic -Wall -Werror -Wextra)

aws_lambda_package_target(${PROJECT_NAME})

add_subdirectory(test)
