# -*- coding: utf-8 -*-
import numpy as np
import matplotlib.pyplot as plt
import os
import re


def readData(filename):
    """Lee los datos de fichero"""

    print('Leyendo datos...')
    data = np.genfromtxt(filename, delimiter=",", dtype=np.double)
    print('Datos leídos.\n')
    return data[:, 0], data[:, 1], data[:, 2]


def plotGraphs():
    data = "data"
    directory = os.fsencode(data)
        
    for file in os.listdir(directory):
        filename = os.fsdecode(file)

        if ".txt" in filename:
            name = filename.replace('.txt', '')
            title = name.capitalize()
            title = re.sub(r"([0-9]+(\.[0-9]+)?)",r" \1 ", title).strip()
            
            X, clas, red = readData(data + os.sep + filename)
            plt.plot(X, clas*100, marker='.', linestyle=':', label ="Tasa de clasificación", c="green")
            plt.plot(X, red*100, marker='.', linestyle=':', label="Tasa de reducción", c="red")
            plt.plot(X, red*50 + clas*50, marker='.', linestyle=':', label="Agregación", c="blue")
            plt.title(title)
            plt.xlabel("Iteraciones")
            plt.ylabel("%")
            plt.legend()
            plt.savefig("plots/" + name + ".pdf")
            plt.close()


if __name__ == "__main__":
    plotGraphs()