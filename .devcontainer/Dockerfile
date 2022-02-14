ARG VARIANT=ubuntu-21.04
FROM amazonlinux:2

# add epel repository
RUN yum -y install https://dl.fedoraproject.org/pub/epel/epel-release-latest-7.noarch.rpm

# dependency from package
RUN yum groupinstall -y "Development Tools"
RUN yum -y install libcurl-devel cmake3 openssl openssl-devel libsodium-devel

# dependency from source
RUN git clone https://github.com/awslabs/aws-lambda-cpp.git \
    && cd aws-lambda-cpp \
    && git checkout tags/v0.2.7 \
    && mkdir build && cd build \
    && cmake3 .. -DCMAKE_BUILD_TYPE=Release \
    && make && make install

RUN git clone --recurse-submodules https://github.com/aws/aws-sdk-cpp.git \
    && cd aws-sdk-cpp \
    && git checkout tags/1.9.193 \
    && mkdir build && cd build \
    && cmake3 .. -DCMAKE_BUILD_TYPE=Release -DBUILD_SHARED_LIBS=OFF -DBUILD_ONLY="lambda" -DCPP_STANDARD=17 -DENABLE_UNITY_BUILD=ON \
    && make && make install

RUN git clone https://github.com/google/gumbo-parser.git \
    && cd gumbo-parser \
    && git checkout tags/v0.10.1 \
    && ./autogen.sh \
    && ./configure \
    && make && make install

# cdk node
RUN yum -y install nodejs npm --disablerepo=* --enablerepo=epel

RUN curl "https://awscli.amazonaws.com/awscli-exe-linux-x86_64.zip" -o "awscliv2.zip" \
    && unzip awscliv2.zip \
    && ./aws/install

RUN npm install -g aws-cdk

