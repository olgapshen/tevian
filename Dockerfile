FROM ubuntu:18.04
RUN apt-get update
RUN apt-get install -y qt5-default
RUN mkdir -p /var/package
COPY tevian_1.0-1.deb /var/package/
RUN dpkg -i /var/package/tevian_1.0-1.deb
RUN cd /usr/local/lib
RUN ldconfig