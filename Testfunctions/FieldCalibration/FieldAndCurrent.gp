reset

#set terminal
set terminal pdf enhanced size 6,4 font "Helvetica, 15"
set output "FieldAndCurrent.pdf"

#Source file
# File structure
#set datafile commentschars "#"
set datafile separator ","
f = "data.csv"


#2021-07-21 17:17:05:651361
fmt = "%Y-%m-%d %H:%M:%S" # Time format in the data file

# Consider x-axis as timed data
set xdata time
set timefmt fmt 
set format x "%S" # Format on the x axis

len(x,y,z) = sgn(x+y+z)*sqrt(x*x + y*y + z*z)

# Labels and legend
set xlabel "time / s" offset 0,0.5 font "Helvetica, 15"
set autoscale x
#set xrange [0:350]
#set xrange ["2021-04-27 1:00:00" :"2021-04-27 23:00:00"]
#set xrange ["2021-05-06 12:00:00" :"2021-05-06 23:00:00"]
#set xtics 2*60*60 offset 0,0 font "Helvetica,15"
#set mxtics 4

set key top left font "Helvetica, 10"


# time, current (A), field (T) 

set ylabel "Current / A" offset 1,0 font "Helvetica, 15"
set ytics font "Helvetica,15"
set yrange [-3:3]
#set ytics 20
#set mytics 5
#set format y "%.0f"

set y2label "Field / T" font "Helvetica, 15"
set y2tics font "Helvetica, 15" textcolor rgb "red"
#set y2tics 1./1000 
#set my2tics 5

plot f using 1:($2) with lines lw 3 lc rgb 'black' title "Current",\
     f using 1:(len($3,$4,$5)) axis x1y2 with lines lw 3 lc rgb 'red' title "Field"

set output
