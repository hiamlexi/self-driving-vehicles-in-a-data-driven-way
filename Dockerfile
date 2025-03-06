##################################################
# Section 1: Build the application
FROM registry.git.chalmers.se/courses/dit638/students/2024-group-12/ubuntu:18.04 as builder

RUN apt-get update -y && \
    apt-get upgrade -y && \
    apt-get dist-upgrade -y

RUN DEBIAN_FRONTEND=noninteractive TZ=Etc/UTC \
        apt-get install -y --no-install-recommends \
        cmake \
        build-essential \
        libopencv-dev \
        gcovr

ADD . /opt/sources
WORKDIR /opt/sources

RUN cd /opt/sources && \
    mkdir build && \
    cd build && \
    cmake -D CMAKE_BUILD_TYPE=Release .. && \
    make && make test && cp main /tmp && \
    gcovr --xml-pretty --exclude-unreachable-branches --print-summary -o coverage.xml --root .. --filter \.\./src/ && \
    gcovr --html --exclude-unreachable-branches --print-summary -o coverage.html --root .. --filter \.\./src/

##################################################
# Section 2: Bundle the application.
FROM registry.git.chalmers.se/courses/dit638/students/2024-group-12/ubuntu:18.04

ENV DEBIAN_FRONTEND noninteractive

RUN apt-get update -y && \  
    apt-get upgrade -y && \
    apt-get dist-upgrade -y

RUN apt-get install -y --no-install-recommends \
        libopencv-core3.2 \
        libopencv-highgui3.2 \
        libopencv-imgproc3.2 

COPY --from=builder /tmp/main /opt/main

ENTRYPOINT ["/opt/main"]