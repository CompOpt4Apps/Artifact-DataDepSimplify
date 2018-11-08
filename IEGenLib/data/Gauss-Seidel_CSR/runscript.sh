cd ../..
g++ -O3 -o simplifyDriver drivers/simplification.cc -I src build/src/libiegenlib.a -lisl -std=c++11
cd data/Gauss-Seidel_CSR/
time ../../simplifyDriver gs_csr.json > gs_csr.out 2> /dev/null
echo ""
echo "The output is written to gs_csr.out"
echo ""
