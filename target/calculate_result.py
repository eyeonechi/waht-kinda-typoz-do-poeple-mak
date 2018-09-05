import sys

with open(sys.argv[1]) as f:
    data = []
    for line in f:
        data.append(line)
    accuracy_denominator = len(data)
    accuracy_numerator = 0.0
    recall_denominator = len(data)
    recall_numerator = 0.0
    precision_denominator = 0.0
    precision_numerator = 0.0
    for line in data:
        lineA = line.split(':')
        lineB = lineA[0].strip().split(',')
        misspell = lineB[0].strip()
        correct = lineB[1].strip()
        lineC = lineA[1].strip().split(',')
        matches = []
        for match in lineC:
            if match != '':
                matches.append(match.strip())
        #print(misspell, correct, matches)
        precision_denominator += float(len(matches))
        if len(matches) > 0:
            if matches[0] == correct:
                accuracy_numerator += 1.0
        for match in matches:
            if match == correct:
                precision_numerator += 1.0
                recall_numerator += 1.0

    accuracy = accuracy_numerator / accuracy_denominator
    precision = precision_numerator / precision_denominator
    recall = recall_numerator / recall_denominator
    print("accuracy = " + str(accuracy * 100) + "%")
    print("precision = " + str(precision * 100) + "%")
    print("recall = " + str(recall * 100) + "%")
