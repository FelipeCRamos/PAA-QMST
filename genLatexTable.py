def readFromFile(fileName):
    f = open(fileName, "r")
    return f.read()

def genInstancesProps():
    instancesProps = []

    nVals = [10, 15, 20, 25, 30]
    dVals = [33, 67, 100]
    CVals = [10, 100]
    QVals = [10, 100]

    for n in nVals:
        for d in dVals:
            for c in CVals:
                for q in QVals:
                    instancesProps.append((n, d, c, q))

    return instancesProps


def genResultDicts():
    BKs = [
    350,    3122,    646,    3486,    255,    2042,    488,    2404,    239,    1815,    426,    2197,
    745,    6539,    1236,    7245,    659,    5573,    966,    6188,    620,    5184,    975,    5879,
    1379,    12425,    1972,    13288,    1252,    10893,    1792,    11893,    1174,    10215,    1544,
    11101,    2185,    19976,    2976,    21176,    2023,    18251,    2546,    19207,    1943,    17411,
    2471,    18370,    3205,    29731,    4070,    31077,    2998,    27581,    3649,    28777,    2874,
    26146,    3483,    27314
    ]
    x = zip(genInstancesProps(), BKs)
    return dict(x)

def extractFileName(path):
    return path.split('/')[-1]

def extractTupleFromFileName(fileName):
    fileName = fileName.replace('n', ';')
    fileName = fileName.replace('d', ';')
    fileName = fileName.replace('C', ';')
    fileName = fileName.replace('c', ';')
    fileName = fileName.replace('Q', ';')
    fileName = fileName.replace('q', ';')
    fileName = fileName.replace('s', ';')
    _, nStr, dStr, CStr, cStr, QStr, qStr, _ = fileName.split(';')
    return (int(nStr), int(dStr), int(CStr), int(QStr))

def mountTables(text, resultDicts):
    outputBest = "$n$ & $d$ & $C$ & $Q$ & BK & BPS & #BPS & Gap & BTS & #BPS & Gap \\\\\\hline\n"
    outputMean = "$n$ & $d$ & $C$ & $Q$ & BK & MeanPS & Gap & MeanTS & Gap \\\\\\hline\n"
    outputTime = "$n$ & $d$ & $C$ & $Q$ & MeanPS & MeanTS \\\\\\hline\n"
    for line in text.split('\n'):
        if line == "": continue
        filePath, bestPS, countBestPS, meanPS, meanTimePS, bestTS, countBestTS, meanTS, meanTimeTS, _ = line.split(';')
        bestPS = int(bestPS)
        countBestPS = int(countBestPS)
        meanPS = float(meanPS)
        meanTimePS = float(meanTimePS)
        bestTS = int(bestTS)
        countBestTS = int(countBestTS)
        meanTS = float(meanTS)
        meanTimeTS = float(meanTimeTS)
        fileName = extractFileName(filePath)
        # print(fileName, bestPS, countBestPS, meanPS, meanTimePS, bestTS, countBestTS, meanTS, meanTimeTS)
        tuple = extractTupleFromFileName(fileName)
        bestKnown = resultDicts[tuple]
        bestPSGap = ((bestPS / bestKnown) - 1) * 100
        meanPSGap = ((meanPS / bestKnown) - 1) * 100
        bestTSGap = ((bestTS / bestKnown) - 1) * 100
        meanTSGap = ((meanTS / bestKnown) - 1) * 100


        head = "{} & {} & {} & {}".format(*tuple)

        lineBest = "{} & {} & {} & {:.2f}\\% & {} & {} & {:.2f}\\%".format(bestKnown, bestPS, countBestPS, bestPSGap, bestTS, countBestTS, bestTSGap)
        outputBest += head + " & " + lineBest + '\\\\\n'

        lineMean = "{} & {:.2f} & {:.2f}\\% & {:.2f} & {:.2f}\\%".format(bestKnown, meanPS, meanPSGap, meanTS, meanTSGap)
        outputMean += head + " & " + lineMean + '\\\\\n'

        lineTime = "{:.3f} & {:.3f}".format(meanTimePS, meanTimeTS)
        outputTime += head + " & " + lineTime + '\\\\\n'

    print(outputBest)


text = readFromFile('out')
mountTables(text, genResultDicts())
