set terminal png size 800,600
set output 'testFin.png'
set title '' 
set xlabel 'date' 
set ylabel 'val1' 
set title '' 
plot 'Outputs/scatter2D.dat' using 1:2 notitle pt 7 ps 2
