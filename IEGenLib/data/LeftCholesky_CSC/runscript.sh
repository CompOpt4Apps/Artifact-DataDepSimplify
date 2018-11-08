cd ../..
g++ -O3 -o simplifyDriver drivers/simplification.cc -I src build/src/libiegenlib.a -lisl -std=c++11
cd data/LeftCholesky_CSC/
time ../../simplifyDriver leftCholesky_static.json > leftCholesky_static.out 2> /dev/null
echo ""
echo "The output is written to leftCholesky_static.out"
echo ""
