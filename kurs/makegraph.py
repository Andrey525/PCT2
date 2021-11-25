#!/usr/bin/env python3

import matplotlib.pyplot as plt
import numpy as np
import matplotlib.ticker as ticker

def main():
    datafiles = ["linear.dat", "parallel_100.dat", "parallel_50.dat"]
    labels = ["linear speedup", "n = 100 000 000", "n = 50 000 000"]
    cm = 1/2.54  # centimeters in inche
    fig = plt.figure(figsize=(16*cm, 10*cm))
    ax = fig.add_subplot(111)
    colors=["black", "blue", "darkgreen", "red"]
    ax.set(xlabel="Processes", ylabel="Speedup", title="Quicksort parallel")
    plt.xticks([2, 4, 8])

    # Draw data files
    for i in range(len(datafiles)):
        data = np.loadtxt(datafiles[i])
        x = data[:, 0]
        y = data[:, 2]
        ax.plot(x,y,'o-',markersize=2,c=colors[i],label=labels[i])
        ax.legend();

    fig.savefig('Quicksort.png', dpi=300)

if __name__ == "__main__":
    main()