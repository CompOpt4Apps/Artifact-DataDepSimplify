cd ../..
g++ -O3 -o simplifyDriver drivers/simplification.cc -I src build/src/libiegenlib.a -lisl -std=c++11
cd data/Forward-Solve_CSC/
time ../../simplifyDriver forwSolCSC.json > forwSolCSC.out 2> /dev/null
echo ""
echo "The output is written to forwSolCSC.out"
echo ""
