cmake_minimum_required(VERSION 3.15)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_INSTALL_PREFIX ${CMAKE_SOURCE_DIR}/install)
project(aws-loabot-handler)

find_package(aws-lambda-runtime)
find_package(AWSSDK REQUIRED COMPONENTS lambda)

add_executable(${PROJECT_NAME}
    src/main.cpp
    src/message.cpp)

target_include_directories(${PROJECT_NAME} PRIVATE include)
target_link_libraries(${PROJECT_NAME} PRIVATE AWS::aws-lambda-runtime ${AWSSDK_LINK_LIBRARIES})
target_compile_features(${PROJECT_NAME} PRIVATE "cxx_std_17")
target_compile_options(${PROJECT_NAME} PRIVATE -pedantic -Wall -Werror -Wextra)

install(TARGETS ${PROJECT_NAME})
aws_lambda_package_target(${PROJECT_NAME})
