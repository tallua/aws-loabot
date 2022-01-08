cmake_minimum_required(VERSION 3.15)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_INSTALL_PREFIX ${CMAKE_SOURCE_DIR}/install)
project(aws-loabot-router)

include(FetchContent)

find_package(aws-lambda-runtime)
find_package(AWSSDK REQUIRED COMPONENTS lambda)
FetchContent_Declare(Sodium
    GIT_REPOSITORY https://github.com/robinlinden/libsodium-cmake.git
)
FetchContent_MakeAvailable(Sodium)


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

install(TARGETS ${PROJECT_NAME})
aws_lambda_package_target(${PROJECT_NAME})
