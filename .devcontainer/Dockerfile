ARG VARIANT=ubuntu-21.04
FROM mcr.microsoft.com/vscode/devcontainers/cpp:0-${VARIANT}

RUN apt-get update && export DEBIAN_FRONTEND=noninteractive
RUN apt-get install -y ninja-build

RUN apt-get -y install libcurl4-openssl-dev
RUN git clone https://github.com/awslabs/aws-lambda-cpp.git \
    && cd aws-lambda-cpp \
    && mkdir build && cd build \
    && cmake .. -DCMAKE_BUILD_TYPE=Release \
    && make && make install

RUN apt-get -y install zlib1g-dev libssl-dev
RUN git clone --recurse-submodules https://github.com/aws/aws-sdk-cpp.git \
    && cd aws-sdk-cpp \
    && mkdir build && cd build \
    && cmake .. -DCMAKE_BUILD_TYPE=Release -DBUILD_SHARED_LIBS=OFF -DBUILD_ONLY="s3;lambda;secretsmanager" -DCPP_STANDARD=17 -DENABLE_UNITY_BUILD=ON \
    && make && make install

RUN apt-get install -y nodejs npm \
    && npm install -g aws-cdk typescript

RUN curl "https://awscli.amazonaws.com/awscli-exe-linux-x86_64.zip" -o "awscliv2.zip" \
    && unzip awscliv2.zip \
    && ./aws/install