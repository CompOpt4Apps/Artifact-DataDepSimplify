cd ../..
g++ -O3 -o simplifyDriver drivers/simplification.cc -I src build/src/libiegenlib.a -lisl -std=c++11
cd data/LeftLU_CSC/
time ../../simplifyDriver leftLU_static.json > leftLU_static.out 2> /dev/null
echo ""
echo "The output is written to leftLU_static.out"
echo ""
