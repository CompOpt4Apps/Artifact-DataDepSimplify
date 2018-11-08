cd ../..
g++ -O3 -o simplifyDriver drivers/simplification.cc -I src build/src/libiegenlib.a -lisl -std=c++11
cd data/SpMV_CSR/
time ../../simplifyDriver SpMV.json > SpMV.out 2> /dev/null
echo ""
echo "The output is written to SpMV.out"
echo ""
