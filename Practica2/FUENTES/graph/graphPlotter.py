# -*- coding: utf-8 -*-
import numpy as np
import matplotlib.pyplot as plt

def readData(filename):
    """Lee los datos de fichero"""

    data = np.genfromtxt(filename, delimiter=",", dtype=np.double)
    return data[:, 0], data[:, 1]


def plotGraphs():
    datasets = ["ionosphere", "parkinsons", "spectf-heart"]
    algorithms = ["localSearch", "aggBlx", "aggCa", "ageBlx", "ageCa", "am10", "am01", "am01mej"]

    for i in range(1, 6):
        for dataset in datasets:
            for algorithm in algorithms:
                filename = "data/" + dataset + str(i) + "-" + algorithm + ".txt"
                x, y = readData(filename)
                plt.plot(x, 1-y, label=algorithm)

            title = dataset.capitalize() + " - Partición " + str(i)
            plt.title(title)
            plt.xlabel("Iteraciones")
            plt.ylabel("Agregación")
            plt.legend(loc="lower right")

            plt.yscale('log')
            plt.gca().invert_yaxis()

            plt.savefig("plots/" + dataset + str(i) + ".pdf")
            plt.close()

if __name__ == "__main__":
    plotGraphs()