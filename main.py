from numpy import int64
import pandas as pd
import matplotlib.pyplot as plt

size = "size(kB)"
speed = "speed(us)"

def plotData(fileName: str, name: str, frequency: int64):
    csvfile = pd.read_csv(fileName, usecols=[size, speed])
    csvfile.plot(x=size, y=speed, kind='line')
    plt.title(name)
    plt.scatter(csvfile[size], csvfile[speed])
    plt.xticks(ticks=[i for i in range(0, csvfile[size].max(), frequency)])
    plt.show()

plotData("file.csv", "size-speed cache comparison", 32)
plotData("file_cont.csv", "contiguous memory iteration comparison", 5000)
plotData("file_n_cont.csv", "non-contiguous memory iteration comparison", 5000)