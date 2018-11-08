cd ../..
g++ -O3 -o simplifyDriver drivers/simplification.cc -I src build/src/libiegenlib.a -lisl -std=c++11
cd data/ILU0_CSR/
time ../../simplifyDriver ilu0.json > ilu0.out 2> /dev/null
echo ""
echo "The output is written to ilu0.out"
echo ""
