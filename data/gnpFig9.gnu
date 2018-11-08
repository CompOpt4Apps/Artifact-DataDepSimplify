set terminal pdf
set output "results/figure9.pdf"
#set title "Figure 9"
set datafile separator ","
set boxwidth 0.8 relative
unset border
set border 3
set ylabel "Number of Relations in each Complexity"
set yrange [0:20]
#set xtics rotate by 45 offset 0,-5
set xtic rotate by -45 scale 0
set style data histograms
set style histogram cluster gap 2
set style fill solid 1.0 border lt -1

plot 'results/figure9.csv' using 2:xtic(1) title columnheader(2), for [i=3:6] '' using i title columnheader(i)
#plot 'results/figure9.csv' using 2:xtic(1) title columnheader(2), for [i=3:5] '' using i title columnheader(i), '' using (stringcolumn(6) eq "0" ? "All Found Unsatisfiable" : ""):xtic(1) title columnheader(6) w labels rotate by 90

