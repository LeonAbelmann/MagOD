#!/usr/bin/env python3
# Run as:
# cmag.py f0000.csv
# cmag.py f0001.csv f0001.csv
# cmapy.py f*.csv
# etc

# Outputs to plot.pdf
import os
import sys
import numpy as np
import matplotlib.pyplot as plt

# MagOD order of fields:
# Time[ms],Looppar,Vdiode[V],Vled[V],Vscat[V],Temperature[C],
# Ix[A],Iy[A],Iz[A],LED_type

# Plot diode voltage as function of time
def plotVdiode(ax,time,v,LED,sec):
    # Determine plot color
    switcher={
        0 :  'r',
        1 :  'g',
        2 :  'b',
        }
    colors=[]
    for i in LED:
        colors.append(switcher.get(i,"k"))
    # Plot graph:
    ax.scatter(time/1000,v,c=colors)
    # Plot vertical lines at loop steps
    for timeval in sec:
        ax.axvline(timeval/1000,color='k',linewidth=0.5)
    # Determine step height
    return

# Plot coil currents as function of time
def plotCoilCur(ax,time,ix,iy,iz,first = False):
    ax.plot(time/1000,ix,
             'k',linestyle='-',marker='.',label='x')
    ax.plot(time/1000,iy,
             'r',linestyle='-',marker='.',label='y')
    ax.plot(time/1000,iz,
             'b',linestyle='-',marker='.',label='z')
    if first:
        ax.legend()
        first = False
    return first

# Get timepoints with transitions is currents
def getSections(time,looppar):
    sec = [time[0]]
    # Search for the next transition
    for i in range(len(looppar)-1):
        if not(looppar[i] == looppar[i+1]):
            sec.append(time[i])
    return sec

if __name__ == "__main__":
    fig,ax = plt.subplots(2)
    ax[0].set(xlabel="Time / s", ylabel="Vdiode / V")
    ax[1].set(xlabel="Time / s", ylabel="Coil current / A")
    first = True # Plot legends etc only for the first graph
    for filename in sys.argv[1:]:
            print(filename)
            data = np.genfromtxt(filename, delimiter=",", names=
                                ["Time","Looppar","Vdiode","Vscat","Vled",
                                "Temperature","Ix","Iy","Iz","LED_type"])
            sec = getSections(data['Time'],data['Looppar'])
            plotVdiode(ax[0],data['Time'],data['Vdiode'],
                               data["LED_type"],sec)
            first = plotCoilCur(ax[1],data['Time'],
                                data['Ix'],data['Iy'],data['Iz'],
                                first = first)
    #ax[1].set_ylim(top=5)
    fig.tight_layout()
    fig.savefig("cmag.pdf", format="pdf")
    #plt.show()
