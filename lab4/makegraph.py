#!/usr/bin/env python3

import matplotlib.pyplot as plt
import numpy as np
import matplotlib.ticker as ticker

def main():
    datafiles = ["linear.dat", "parallel_1000.dat", "parallel_10000.dat"]
    labels = ["linear speedup", "speedup at n = 1000", "speedup at n = 10000"]
    cm = 1/2.54  # centimeters in inche
    fig = plt.figure(figsize=(16*cm, 10*cm))
    ax = fig.add_subplot(111)
    colors=["black", "blue", "red"]
    ax.set(xlabel="Processes", ylabel="Speedup", title="Solution of stationary 2D Laplace Equation Using 2D Decomposition")
    # ax.xaxis.set_major_locator(ticker.MultipleLocator(8))
    plt.xticks([8, 16, 32])

    # Draw data files
    for i in range(len(datafiles)):
        data = np.loadtxt(datafiles[i])
        x = data[:, 0]
        y = data[:, 2]
        ax.plot(x,y,'o-',markersize=2,c=colors[i],label=labels[i])
        ax.legend();

    fig.savefig('2D_Dec.png', dpi=300)

if __name__ == "__main__":
    main()