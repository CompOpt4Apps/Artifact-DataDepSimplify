cd ../..
g++ -O3 -o simplifyDriver drivers/simplification.cc -I src build/src/libiegenlib.a -lisl -std=c++11
cd data/SpSpMUL_CSC/
time ../../simplifyDriver SpMM.json > SpMM.out 2> /dev/null
echo ""
echo "The output is written to SpMM.out"
echo ""
