ARG VARIANT=ubuntu-21.04
FROM mcr.microsoft.com/vscode/devcontainers/cpp:0-${VARIANT}

RUN apt-get update && export DEBIAN_FRONTEND=noninteractive


RUN apt-get -y install libcurl4-openssl-dev
RUN git clone https://github.com/awslabs/aws-lambda-cpp.git \
    && cd aws-lambda-cpp \
    && mkdir build \
    && cd build\
    && cmake .. -DCMAKE_BUILD_TYPE=Release \
    && make && make install