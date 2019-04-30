from collections import defaultdict as dd
import pandas as pd
import matplotlib.pyplot as plt

with open("DonneesExperience1.txt", "r") as f:
    abs = []
    ord = []
    for cnt, line in enumerate(f):
        if "." in line[16:-1] and "frequency" not in line[16:-1]:
            abs.append(line[:12])
            ord.append(float(line[16:-1]))
    temp = ord[0:-1:7]
    press = ord[1:-1:7]
    hum = ord[2:-1:7]
    gas = ord[3:-1:7]
    f1 = ord[4:-1:7]
    f2 = ord[5:-1:7]
    f3 = ord[6:-1:7]
    print(ord)
    print(abs[3:-1:7])
    print(gas)
    plt.plot(abs[3:-1:7], gas)
    plt.show()