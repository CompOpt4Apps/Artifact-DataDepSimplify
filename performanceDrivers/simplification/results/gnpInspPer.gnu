set terminal pdf
set output "insp.pdf"
set title "Inspector Overhead Break Point For Executor Speed Up"
set datafile separator ","
set boxwidth 0.8 relative
unset border
set border 3
set logscale y 2
set ylabel "Number of Executor Execution"
set yrange [0.06:128]
#set xtics rotate by 45 offset 0,-5
set xtic rotate by -45 scale 0
set style data histograms
set style histogram cluster gap 2
set style fill solid 1.0 border lt -1

plot 'insp.csv' using 2:xtic(1) title columnheader(2), for [i=3:4] '' using i title columnheader(i)
