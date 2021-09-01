reset

#set terminal
set terminal pdf enhanced size 6,4 font "Helvetica, 15"
set output "FieldVsCurrent.pdf"

#Source file
# File structure
#set datafile commentschars "#"
set datafile separator ","
f = "data.csv"

len(x,y,z) = -1*sgn(x+y+z)*sqrt(x*x + y*y + z*z)

f(x) = a*x + b
fit f(x) f using 2:(len($3,$4,$5)) via a, b

# Labels and legend
set xlabel "Current / A" offset 0,0.5 font "Helvetica, 15"
set autoscale x
#set xrange [0:350]
#set xrange ["2021-04-27 1:00:00" :"2021-04-27 23:00:00"]
#set xrange ["2021-05-06 12:00:00" :"2021-05-06 23:00:00"]
#set xtics 2*60*60 offset 0,0 font "Helvetica,15"
#set mxtics 4

set key top left font "Helvetica, 10"


# time, current (A), field (T) 

set ylabel "Field / T" offset 1,0 font "Helvetica, 15"
set ytics font "Helvetica,15"
set autoscale y
#set ytics 20
#set mytics 5
#set format y "%.0f"

set label sprintf("a = %3.6f T/A", a) at -2,0.003 font "Helvetica, 14"
set label sprintf("b = %3.6f T", b) at -2,0.002 font "Helvetica, 14"


plot f using ($2):(len($3,$4,$5)) with points \
     pointtype 7 pointsize 1 lc rgb 'black' notitle ,\
     f(x) with lines lw 2 linecolor rgb "red" notitle


set output
