FROM ubuntu:18.04
RUN apt-get update
RUN apt-get install -y qt5-default curl git build-essential cmake
RUN cd ~
RUN git clone https://github.com/olgapshen/tevian.git
RUN cd tevian
RUN mkdir build
RUN cd build
RUN cmake ..
RUN make -j4
RUN cpack
RUN dpkg -i tevian-1.0-1-Linux.deb
RUN cd /usr/local/lib
RUN ldconfig