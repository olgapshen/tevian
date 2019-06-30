#!/bin/sh

cd build/

cmake -G "Unix Makefiles" -DCMAKE_BUILD_TYPE=Debug
make

cd ..

mkdir -p tevian_1.0-1/usr/local/bin
mkdir -p tevian_1.0-1/usr/local/lib

cp build/libtevian_lib.so tevian_1.0-1/usr/local/lib/
cp build/tevian_cli tevian_1.0-1/usr/local/bin/
cp build/tevian_ui tevian_1.0-1/usr/local/bin/

dpkg-deb --build tevian_1.0-1