#!/usr/bin/env python3

import matplotlib.pyplot as plt
import numpy as np

def main():
    datafiles = ["linear.dat", "sgemv_28000.dat", "sgemv_45000.dat"]
    labels = ["n = linear", "n = m = 28000", "n = m = 45000"]
    cm = 1/2.54  # centimeters in inche
    fig = plt.figure(figsize=(16*cm, 10*cm))
    ax = fig.add_subplot(111)
    colors=["black", "blue", "red"]
    ax.set(xlabel="Processes", ylabel="Speedup", title="SGEMV")

    # Draw data files
    for i in range(len(datafiles)):
        data = np.loadtxt(datafiles[i])
        x = data[:, 0]
        y = data[:, 2]
        ax.plot(x,y,'o-',markersize=2,c=colors[i],label=labels[i])
        ax.legend();

    fig.savefig('sgemv.png', dpi=300)

if __name__ == "__main__":
    main()