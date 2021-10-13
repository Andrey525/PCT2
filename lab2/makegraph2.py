#!/usr/bin/env python3

import matplotlib.pyplot as plt
import numpy as np

def main():
    datafiles = ["monte-carlo_7.dat", "monte-carlo_8.dat", "linear.dat"]
    labels = ["n = 10^7", "n = 10^8", "Linear speedup"]
    cm = 1/2.54  # centimeters in inche
    fig = plt.figure(figsize=(16*cm, 10*cm))
    ax = fig.add_subplot(111)
    colors=["blue", "red", "black"]
    ax.set(xlabel="Processes", ylabel="Speedup", title="Monte-carlo")

    # Draw data files
    for i in range(len(datafiles)):
        data = np.loadtxt(datafiles[i])
        x = data[:, 0]
        y = data[:, 2]
        ax.plot(x,y,'o-',markersize=2,c=colors[i],label=labels[i])
        ax.legend();

    fig.savefig('monte-carlo.png', dpi=300)

if __name__ == "__main__":
    main()