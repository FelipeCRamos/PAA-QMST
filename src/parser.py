#!/usr/bin/env python3

import sys
import re

def removeTabsAndDelimiters(string):
    if '\t' in string:
        string = string.replace('\t', ' ')

    if ';' in string:
        string = string.replace(';','')

    return string

def parseFile(filename = "tests/10-30/n010d033C010c001Q010q001s-3i1.txt"):
    with open(filename, 'r') as file:
        params = dict()
        sets = dict()

        isOnQ = False
        for line in file.read().split('\n'):
            line = re.split(r'\s', line)
            paramKey = ''
            paramValue = ''

            setKey = ''
            setValue = ''

            if not isOnQ:
                if line[0] == 'param':
                    paramKey = line[1]
                    paramValue = line[3:-1] if line[-1] == ';' else ['NF']
                elif line[0] == 'set':
                    setKey = line[1]
                    setValue = line[3:-1] if line[-1] == ';' else ['NF']
                # -----
                if len(paramKey): # if param
                    if paramKey != 'q':
                        params[paramKey] = paramValue if len(paramValue) > 1 else int(paramValue[0])
                    else:
                        isOnQ = True
                        continue
                elif len(setKey): # if set
                    if setKey == 'Edges':
                        sets['Edges'] = setValue

            else:
                params['q'] = line[:-1]
                break

        return (params, sets)

FILENAME = ""

params, sets = [], []
if not len(FILENAME):
    params, sets = parseFile()
else:
    params, sets = parseFile(FILENAME)

sets = [s.replace('(', '').replace(')', '').split(',') for s in sets['Edges']]

even = params['q'][0::2]
odd = params['q'][1::2]

evenC = params['c'][0::2]
oddC = params['c'][1::2]

removeBrack = lambda x: x.replace('[', '').replace(']', '').split(',')

params['q'] = [(removeBrack(evenEl), oddEl) for evenEl, oddEl in zip(even, odd)]
params['c'] = [(removeBrack(evenEl), oddEl) for evenEl, oddEl in zip(evenC, oddC)]


outputFilename = "test.txt"
outputFile = open(outputFilename, 'w')

for key in params:
    print('#'+ ' ' + key)
    outputFile.write("# " + key + "\n")
    if type(params[key]) == type(list()):
        for value in params[key]:
            line = " ".join(value[0]) + " " + " ".join(value[1])
            outputFile.write(line + "\n")
            print(line)

    else:
        print(params[key])
        outputFile.write(str(params[key]) + "\n")

print("# Edges")
outputFile.write("# Edges\n")

for se in sets:
    t = " ".join(map(str, se))
    outputFile.write(t + "\n")
    print(t)

print('# eof')
outputFile.write("# eof")
