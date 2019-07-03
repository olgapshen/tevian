FROM ubuntu:18.04
RUN apt-get update
RUN apt-get install -y qt5-default curl git build-essential
RUN cd ~
RUN git clone https://github.com/olgapshen/tevian.git
RUN cd tevian
RUN cd /usr/local/lib
RUN ldconfig