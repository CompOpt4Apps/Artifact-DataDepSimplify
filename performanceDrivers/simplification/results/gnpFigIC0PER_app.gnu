set terminal pdf
set output "results/ic0_perf_app.pdf"
set title "Incomplete Cholesky"
#set key left above inside
set datafile separator ","
#set boxwidth 0.8 relative
#unset border
#set border 3
set ylabel "Run time normalized to serial execution"
set yrange [0:8]
#set xtics rotate by 45 offset 0,-5
set xtic rotate by -45 scale 0
set style data histograms
set style histogram rowstacked
set boxwidth 0.5 relative
set style fill solid 1.0 border lt -1

plot 'results/ic0_insp_exec_per_app.csv' using 2 t "Inspector:Dependence", '' using 3 t "Inspector:LevelBuild", '' using 4:xtic(1) t "Executor"

