set terminal png
set output 'times.png'

set title "Execution duration"
set xlabel "Algorithm"
set ylabel "Time (seconds)"

set xrange [-0.5:5.5]

set logscale y 10
set grid ytics
set grid mytics

set style function linespoints
set style line 1 lw 1 lc rgb "black" ps 2 pt 4
set style line 2 lw 1 lc rgb "blue" ps 2 pt 5 
set style line 3 lw 1 lc rgb "purple" ps 2 pt 6
set style line 4 lw 1 lc rgb "brown" ps 2 pt 7
set style line 5 lw 1 lc rgb "red" ps 2 pt 8

plot "data.dat" using 2:3:xtic(1) with linespoints ls 1 title "C/gcc", \
     "data.dat" using 2:4:xtic(1) with linespoints ls 2 title "C/MSVC", \
     "data.dat" using 2:5:xtic(1) with linespoints ls 3 title "Go", \
     "data.dat" using 2:6:xtic(1) with linespoints ls 4 title "Java", \
     "data.dat" using 2:7:xtic(1) with linespoints ls 5 title "Python"

set output 'normalised.png'

set title "Duration relative to C/gcc"
set xlabel "Algorithm"
set ylabel "Time (multiples of C/gcc time)"

set xrange [-0.5:5.5]

set logscale y 10
set grid ytics
set grid mytics

set style function linespoints
set style line 1 lw 1 lc rgb "black" ps 2 pt 4
set style line 2 lw 1 lc rgb "blue" ps 2 pt 5 
set style line 3 lw 1 lc rgb "purple" ps 2 pt 6
set style line 4 lw 1 lc rgb "brown" ps 2 pt 7
set style line 5 lw 1 lc rgb "red" ps 2 pt 8

plot "data.dat" using 2:($3/$3):xtic(1) with linespoints ls 1 title "C/gcc", \
     "data.dat" using 2:($4/$3):xtic(1) with linespoints ls 2 title "C/MSVC", \
     "data.dat" using 2:($5/$3):xtic(1) with linespoints ls 3 title "Go", \
     "data.dat" using 2:($6/$3):xtic(1) with linespoints ls 4 title "Java", \
     "data.dat" using 2:($7/$3):xtic(1) with linespoints ls 5 title "Python"