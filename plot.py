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

def getInfos(filename):
    infos = dict()
    with open(filename, 'r') as logFile:
        logFile = logFile.read().split('\n')
        for filename, logEntry in zip(file_list, logFile):
            entry = logEntry.split(';')
            dicEntry = dict()
            for name, arg in zip(['nodes', 'edge', 'result', 'time', 'calls'], entry):
                dicEntry[name] = arg
            infos[filename] = dicEntry
            #  print(filename, infos[filename])
    return infos



file_list = sorted([str(filename) for filename in os.listdir("tests/10-30/") if isSmallerThan(filename)])

# Make tests
command_list = list()
for file in file_list:
    command = "./agmq tests/10-30/{}".format(file)
    if makeTests:
        print("Sending test to the file: {}...".format(file))
        os.system(command)

# -----------------------------------------------------------------------------------
# interpretaćão de informacoes
exit() # desativada

infos = getInfos('log-backtrack.txt')
infos_bb = getInfos('log-bb.txt')
#  with open('log.txt', 'r') as logFile:
    #  logFile = logFile.read().split('\n')
    #  for filename, logEntry in zip(file_list, logFile):
        #  entry = logEntry.split(';')
        #  dicEntry = dict()
        #  for name, arg in zip(['nodes', 'edge', 'result', 'time', 'calls'], entry):
            #  dicEntry[name] = arg
        #  infos[filename] = dicEntry
        #  print(filename, infos[filename])

backtrackSetPlot = [ (int(infos[file]['calls']), file) for file in infos ]
branchAndBoundSetPlot = [ (int(infos_bb[file]['calls']), file) for file in infos_bb ]

#  print(sorted(backtrackSetPlot, key=lambda x: x[1]))

backTrackTimes = [ x[0] for x in backtrackSetPlot ]
backTrackEdgeNumbers = [ x[1] for x in backtrackSetPlot ]

branchAndBoundTimes = [ x[0] for x in branchAndBoundSetPlot ]
branchAndBoundEdgeNumbers = [ x[1] for x in branchAndBoundSetPlot ]

#  print(backTrackTimes)
#  print(branchAndBoundTimes)

plt.plot(backTrackEdgeNumbers, backTrackTimes, 'b.', label='Backtrack', alpha=0.5)
plt.plot(branchAndBoundEdgeNumbers, branchAndBoundTimes, 'r.', label='Branch and Bound', alpha=0.5)
plt.legend()
plt.xticks(rotation='vertical')
plt.title("Backtrack vs. Branch and bound - Comparação de chamadas", fontsize='medium')

plt.savefig(sys.argv[1] if len(sys.argv) > 1 else "graph-calls.png", dpi=300, bbox_inches='tight')
print("Figure saved! Bye")
