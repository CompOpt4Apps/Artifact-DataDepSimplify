#!/usr/bin/env bash

# Step 0: Installation prerequisites:
sudo apt-get install git wget tar unzip make autoconf automake cmake libtool default-jdk default-jre flex bison python-dev texinfo gnuplot-x11 evince


# Step 1: Installing boost:
mkdir boost
export BOOSTHOME=$(pwd)/boost
wget http://sourceforge.net/projects/boost/files/boost/1.60.0/boost_1_60_0.tar.gz
tar -xvzf boost_1_60_0.tar.gz
cd boost_1_60_0/
./bootstrap.sh --prefix=$BOOSTHOME --with-libraries=chrono,date_time,filesystem,iostreams,program_options,random,regex,signals,system,thread,wave
./b2  --prefix=$BOOSTHOME -sNO_BZIP2=1 install
cd ..


# Step 2: Installing rose compiler:
mkdir rose_build
export ROSE_SRC=$(pwd)/rose
export ROSEHOME=$(pwd)/rose_build
export LD_LIBRARY_PATH=$LD_LIBRARY_PATH:${BOOSTHOME}/lib
export LD_LIBRARY_PATH=$LD_LIBRARY_PATH:${ROSEHOME}/lib
git clone https://github.com/rose-compiler/rose
cd rose
git checkout 365bb6857c
./build
cd $ROSEHOME
${ROSE_SRC}/configure --prefix=$ROSEHOME --enable-languages=c,c++ --with-boost=${BOOSTHOME} --without-java
make -j6 install-rose-library FRONTEND_CXX_VENDOR_AND_VERSION2=gnu-5.3
cd ..


# Step 3: Installing the IEGenLib library:
export IEGENHOME=$(pwd)/IEGenLib/iegen
cd IEGenLib
./configure
make
make install
cd ..


# Step 4: Installing the CHILL compiler:
cd chill
mkdir build
cd build
cmake .. -DROSEHOME=$ROSEHOME -DBOOSTHOME=$BOOSTHOME -DIEGENHOME=$IEGENHOME
make -j6
cd ../..


# Step 5: Building the artifact driver:
g++ -O3 -o simplifyDriver simplification.cc -I IEGenLib/src IEGenLib/build/src/libiegenlib.a -lisl -std=c++11


