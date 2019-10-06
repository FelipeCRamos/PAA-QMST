#!/usr/bin/python

import matplotlib
import matplotlib.pyplot as plt
import numpy as np
import os
import sys
import threading

makeTests = True

def isSmallerThan(string):
    try:
        return True if int(string[:2]) < 60 else False
    except:
        return False

def autolabel(rects):
    for rect in rects:
        height = rect.get_height()
        ax.annotate('{}'.format(height),
                    xy=(rect.get_x() + rect.get_width() / 2, height),
                    xytext=(0, 3),  # 3 points vertical offset
                    textcoords="offset points",
                    ha='center', va='bottom')


file_list = sorted([str(filename) for filename in os.listdir("tests/10-30/") if isSmallerThan(filename)])

# Make tests
command_list = list()
for file in file_list:
    command = "./agmq tests/10-30/{} >> log.txt".format(file)
    if makeTests:
        print("Sending test to the file: {}...".format(file))
        os.system(command)

infos = dict()
with open('log.txt', 'r') as logFile:
    logFile = logFile.read().split('\n')
    for filename, logEntry in zip(file_list, logFile):
        entry = logEntry.split(';')
        dicEntry = dict()
        for name, arg in zip(['nodes', 'edge', 'result', 'time', 'calls'], entry):
            dicEntry[name] = arg
        infos[filename] = dicEntry
        print(filename, infos[filename])

backtrackSetPlot = [ (int(infos[file]['time']), file) for file in infos ]
branchAndBoundSetPlot = [ (int(infos[file]['time']) + 40000, file) for file in infos ]

print(sorted(backtrackSetPlot, key=lambda x: x[1]))

backTrackTimes = [ x[0] for x in backtrackSetPlot ]
backTrackEdgeNumbers = [ x[1] for x in backtrackSetPlot ]

branchAndBoundTimes = [ x[0] for x in branchAndBoundSetPlot ]
branchAndBoundEdgeNumbers = [ x[1] for x in branchAndBoundSetPlot ]

#  print(backTrackTimes)
#  print(branchAndBoundTimes)

plt.plot(backTrackEdgeNumbers, backTrackTimes, 'b.', label='Backtrack')
plt.plot(branchAndBoundEdgeNumbers, branchAndBoundTimes, 'r.', label='Branch and Bound')
plt.legend()
plt.xticks(rotation='vertical')
plt.title("Backtrack vs. Branch and bound - Time comparison", fontsize='medium')

plt.savefig(sys.argv[1] if len(sys.argv) > 1 else "test.png", dpi=300, bbox_inches='tight')
print("Figure saved! Bye")
