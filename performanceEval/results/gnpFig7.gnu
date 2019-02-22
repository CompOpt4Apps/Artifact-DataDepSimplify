set terminal pdf size 8,6
set output "results/fig7.pdf"
set title "Executor Speed Up"
set key left
set datafile separator ","
set boxwidth 0.8 relative
unset border
set border 3
set ylabel "Wavefront Executor Speed up"
set yrange [0:8]
#set xtics rotate by 45 offset 0,-5
#set xtic rotate by -45 scale 0
#set offset -0.6,0,0,0
set xtic offset 1,0
set xtics nomirror
set ytics nomirror
set xtic rotate by -45 scale 0
set style data histograms
set style histogram cluster gap 2
set style fill solid 1.0 border lt -1

plot 'results/fig7.csv' using 2:xtic(1) title columnheader(2), for [i=3:6] '' using i title columnheader(i)

