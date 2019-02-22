set terminal pdf size 8,6
set output "results/fig8.pdf"
set title "Inspector Overhead Break Point For Executor Speed Up"
set datafile separator ","
set boxwidth 0.8 relative
unset border
set border 3
set logscale y 2
set ylabel "Number of Executor Execution"
set yrange [0.01:1024]
#set xtics rotate by 45 offset 0,-5
#set xtic rotate by -45 scale 0
#set offset -0.6,0,0,0
set xtic offset 1,0
set xtics nomirror
set ytics nomirror
set xtic rotate by -45 scale 0
set ytics ("2^{-6}" 0.015625,"2^{-4}" 0.0625 , "2^{-2}" 0.25, "2^0" 1, "2^2" 4, "2^4" 16, "2^6" 64, "2^8" 256)
set style data histograms
set style histogram cluster gap 2
set style fill solid 1.0 border lt -1

plot 'results/fig8.csv' using 2:xtic(1) title columnheader(2), for [i=3:6] '' using i title columnheader(i)
