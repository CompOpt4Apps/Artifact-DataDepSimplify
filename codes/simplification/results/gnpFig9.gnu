set terminal pdf
set output "results/ic0_perf.pdf"
set title "Incomplete Cholesky (without approaximation)"
set datafile separator ","
set boxwidth 0.8 relative
unset border
set border 3
set ylabel "Run time normalized to serial execution time"
set yrange [0:10]
#set xtics rotate by 45 offset 0,-5
set xtic rotate by -45 scale 0
set style data histograms
set style histogram rowstacked
set boxwidth 0.5
set style fill solid 1.0 border lt -1

#plot 'results/figure9.csv' using 2:xtic(1) title columnheader(2), for [i=3:6] '' using i title columnheader(i)

plot 'results/figure9.csv' using 2 t "Inspector:Dependence", '' using 3 t "Inspector:LevelBuild", '' using 4:xtic(1) t "Executor"

