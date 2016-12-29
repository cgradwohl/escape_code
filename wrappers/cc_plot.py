import os
import sys
import math
import matplotlib.pyplot as plt
import numpy as np

if len(sys.argv) < 1:
    print( 'Format: python3 plot_CC.py <PATH TO GRAPH>' )
    sys.exit()
else:
    os.system('../exe/cc_plotData '+sys.argv[1])
    data = np.loadtxt('../wrappers/CCplotData.txt')
    x,y = data[:, 0], data[:,1]
    plt.semilogx(x,y, marker='o', linestyle='None', color='g')

    '''formats plot name and title'''
    graphPath = sys.argv[1]
    plotName = graphPath[10:len(graphPath)].replace("edges","png")
    plotTitle = plotName.replace(".png", "")

    '''label plot'''
    plt.xlabel('Vertex Degree')
    plt.ylabel('Cluster Coefficient')
    plt.suptitle('CC Plot for "'+plotTitle+'" Graph')

    plt.savefig('../plots/'+plotName)
    os.remove('../wrappers/CCplotData.txt')
