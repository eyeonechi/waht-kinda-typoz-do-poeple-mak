with open ('2gram_output_birkbeck.txt') as f:
    with open ('../data/birkbeck_correct.txt') as g:
        corrects = []
        i = 0
        for line in g:
            corrects.append(line.strip())
        for line in f:
            try:
                A = line.split('   potential diatance_word:(')
                misspell = A[0].split(':')[1]
                B = A[1].split(')   ')[0]
                C = B.split(')(')
                D = [item.split(', ') for item in C]
                E = [item[1].strip("'") for item in D]
                str = "".join([item + ',' for item in E])
                out = ""
                out += misspell + ", "  +corrects[i] + ": " + str
                print(out)
                i += 1
            except IndexError:
                continue
