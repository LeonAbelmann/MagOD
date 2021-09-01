#!/usr/bin/env python3
import os
import sys
import numpy as np
import matplotlib.pyplot as plt

#filename = "f151/f151_0.csv"

# MagOD order of fields:
# Time[ms],Looppar,Vdiode[V],Vled[V],Vscat[V],Temperature[C],
# Ix[A],Iy[A],Iz[A],LED_type

def graph(filename,offset):
    data = np.genfromtxt(filename, delimiter=",", names=
                        ["Time","Looppar","Vdiode","Vled","Vscat",
                            "Temperature","Ix","Iy","Iz","LED_type"])
    x = data['Time']
    y = data['Vscat']

    x = x - x[0] -220000
    y = 3.0 - y - offset
    offset = offset + 0.00
    plt.plot(x/1000,y,
             'k',linestyle='-',marker='.')
    return offset


if __name__ == "__main__":
    offset = 0
    for filename in sys.argv[1:]:
        print(filename)
        offset = graph(filename, offset)
    ax = plt.gca()
    ax.set_xlim(left=0,right=200)
    plt.ylabel("Vdiode / V")
    plt.xlabel("Time / s")
    plt.savefig("plot.pdf", format="pdf")
    plt.show()
