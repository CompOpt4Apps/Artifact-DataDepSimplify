cd ../..
g++ -O3 -o simplifyDriver drivers/simplification.cc -I src build/src/libiegenlib.a -lisl -std=c++11
cd data/IC0_CSR/
time ../../simplifyDriver ic0_csr.json > ic0_csr.out 2> /dev/null
echo ""
echo "The output is written to ic0_csr.out"
echo ""
