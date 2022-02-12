ARG VARIANT=ubuntu-21.04
FROM amazonlinux:2

RUN yum -y install https://dl.fedoraproject.org/pub/epel/epel-release-latest-7.noarch.rpm

RUN yum groupinstall -y "Development Tools"
RUN yum -y install libcurl-devel cmake3 openssl openssl-devel

RUN git clone https://github.com/awslabs/aws-lambda-cpp.git \
    && cd aws-lambda-cpp \
    && mkdir build && cd build \
    && cmake3 .. -DCMAKE_BUILD_TYPE=Release \
    && make && make install

RUN git clone --recurse-submodules https://github.com/aws/aws-sdk-cpp.git \
    && cd aws-sdk-cpp \
    && mkdir build && cd build \
    && cmake3 .. -DCMAKE_BUILD_TYPE=Release -DBUILD_SHARED_LIBS=OFF -DBUILD_ONLY="lambda" -DCPP_STANDARD=17 -DENABLE_UNITY_BUILD=ON \
    && make && make install

RUN curl -o- https://raw.githubusercontent.com/nvm-sh/nvm/v0.34.0/install.sh | bash \
    && . ~/.nvm/nvm.sh \
    && nvm install node \
    && npm install -g aws-cdk typescript

RUN curl "https://awscli.amazonaws.com/awscli-exe-linux-x86_64.zip" -o "awscliv2.zip" \
    && unzip awscliv2.zip \
    && ./aws/install

RUN yum install -y libsodium-devel
