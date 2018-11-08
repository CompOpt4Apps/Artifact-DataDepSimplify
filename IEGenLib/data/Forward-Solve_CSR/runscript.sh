cd ../..
g++ -O3 -o simplifyDriver drivers/simplification.cc -I src build/src/libiegenlib.a -lisl -std=c++11
cd data/Forward-Solve_CSR/
time ../../simplifyDriver forwSolCSR.json > forwSolCSR.out 2> /dev/null
echo ""
echo "The output is written to forwSolCSR.out"
echo ""
