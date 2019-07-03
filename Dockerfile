FROM ubuntu:18.04
RUN apt-get update
RUN apt-get install -y qt5-default curl git build-essential cmake
WORKDIR /root
RUN git clone https://github.com/olgapshen/tevian.git
WORKDIR tevian
RUN mkdir build
WORKDIR build
RUN cmake ..
RUN make -j4
RUN cpack
RUN dpkg -i tevian-1.0-1-Linux.deb
WORKDIR /usr/local/lib
RUN ldconfig
WORKDIR /root