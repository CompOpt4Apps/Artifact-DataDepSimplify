# Building the performance drivers
g++ -O3 -o performance performance.cc -std=c++11
cmake -DBUILDTYPE=release .
cmake -DBUILDTYPE=release .
cmake -DBUILDTYPE=release .
make


#running the driver
./performance execList.txt
