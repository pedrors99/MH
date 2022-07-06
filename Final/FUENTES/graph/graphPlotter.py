# -*- coding: utf-8 -*-
import numpy as np
import matplotlib.pyplot as plt
from matplotlib.ticker import ScalarFormatter
import matplotlib.ticker as ticker

def readData(filename):
    """Lee los datos de fichero"""

    data = np.genfromtxt(filename, delimiter=",", dtype=np.double)
    return data[:, 0], data[:, 1]


def plotGraphs():
    datasets = ["ionosphere", "parkinsons", "spectf-heart"]
    algorithms = ["charges", "chargesBl", "chargesMemetic", "chargesEuclidean", "chargesReset", "chargesMaxIter"]

    for i in range(1, 6):
        for dataset in datasets:
            fig = plt.figure()
            ax = fig.add_subplot(111)
            for algorithm in algorithms:
                filename = "data/" + dataset + str(i) + "-" + algorithm + ".txt"
                x, y = readData(filename)
                plt.plot(x, 100 - 100 * y, label=algorithm)

            title = dataset.capitalize() + " - Partición " + str(i)
            plt.title(title)
            plt.xlabel("Evaluaciones")
            plt.ylabel("100 - Agregación")
            plt.legend(loc="upper right")

            ax.set_yscale('log')
            ax.set_yticks([10, 15, 20, 30, 40])
            ax.get_yaxis().set_major_formatter(ticker.ScalarFormatter())

            plt.savefig("plots/" + dataset + str(i) + ".pdf")
            plt.close()

if __name__ == "__main__":
    plotGraphs()