set terminal png size 800,600
set output 'testPlot.png'
set title 'testing plotting 2d' 
set xlabel 'DL 0' 
set ylabel 'DL 1' 
set title 'testing plotting 2d' 
plot 'Outputs/scatter2D.dat' using 1:2 notitle pt 7 ps 2
