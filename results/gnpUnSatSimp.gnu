set terminal pdf size 5,1.8
set rmargin 5
set bmargin 8
set output "unsatSimp.pdf"
set title "Dependence Simplification Impact on Runtime Inspector Number and Complexity"
#set key left above inside
set datafile separator ","
#set boxwidth 0.8 relative
#unset border
#set border 3
#set size 1, 2
set ylabel "Number of runtime inspector"
set yrange [0:35]
#set ytics 2
#set xtics rotate by 45 offset 0,-5
set xtic rotate by -45 scale 0
set style data histograms
set style histogram rowstacked
set boxwidth 0.5 relative
set style fill solid 1.0 border -1

plot newhistogram "In. Chol.", 'ic0_csc.csv' using 2 t "Less than equal to Kernel" lt rgb "#329632", '' using 3:xtic(1) t "More than Kernel" lt rgb "#c89696", \
newhistogram "L. Chol.", 'staticleftChol_csc.csv' using 2 t "" lt rgb "#329632", '' using 3:xtic(1) t "" lt rgb "#c89696", \
newhistogram "I. LU0", 'ilu0_csr.csv' using 2 t "" lt rgb "#329632", '' using 3:xtic(1) t "" lt rgb "#c89696", \
newhistogram "FS CSC", 'fs_csc.csv' using 2 t "" lt rgb "#329632", '' using 3:xtic(1) t "" lt rgb "#c89696", \
newhistogram "FS CSR", 'fs_csr.csv'  using 2 t "" lt rgb "#329632", '' using 3:xtic(1) t "" lt rgb "#c89696", \
newhistogram "GS CSR", 'gs_csr.csv' using 2 t "" lt rgb "#329632", '' using 3:xtic(1) t "" lt rgb "#c89696", \
newhistogram "SpMV", 'spMV_Mul_csr.csv' using 2 t "" lt rgb "#329632", '' using 3:xtic(1) t "" lt rgb "#c89696"


#newhistogram "GS BCSR", 'gs_bcsr.csv' using 2 t "" lt rgb "#329632", '' using 3:xtic(1) t "" lt rgb "#c89696", \

