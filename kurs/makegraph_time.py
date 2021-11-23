#!/usr/bin/env python3

import matplotlib.pyplot as plt
import numpy as np
import matplotlib.ticker as ticker

def main():
    datafiles = ["serial_time.dat", "parallel_time_8.dat"]
    labels = ["serial time", "parallel time at 8 processes"]
    cm = 1/2.54  # centimeters in inche
    fig = plt.figure(figsize=(16*cm, 10*cm))
    ax = fig.add_subplot(111)
    colors=["black", "blue"]
    ax.set(xlabel="n * 10^6", ylabel="Time, s", title="Time of serial and parallel computings")
    ax.xaxis.set_major_locator(ticker.MultipleLocator(10))
    #plt.xticks([10, 8, 16, 32])

    # Draw data files
    for i in range(len(datafiles)):
        data = np.loadtxt(datafiles[i])
        x = data[:, 1] / 1000000
        y = data[:, 2]
        ax.plot(x,y,'o-',markersize=2,c=colors[i],label=labels[i])
        ax.legend();

    fig.savefig('Quicksort_time_graph.png', dpi=300)

if __name__ == "__main__":
    main()