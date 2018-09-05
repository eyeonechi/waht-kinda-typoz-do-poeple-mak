#------create-dictionary-soundex-code------------
#-------------added by Ruizhili--------
import pdb
import time
start = time.clock()
def soundex(str):
# digits are soundex alphabet values  
    digits = '01230120022455012623010202'
    soundex = ''
    sp = ''
    for letter in str.lower():   # avoid upper&lower confusing
	    if letter.isalpha():
                    if not sp:
               # store first letter               
                        sp = letter   
                    d = digits[ord(letter) - ord('a')]
            # duplicate consecutive soundex digits are skipped
                    if not soundex or (d != soundex[-1]):
                        soundex += d
    print (soundex)
    # replace first digit with first letter
    soundex = sp + soundex[1:]
    # remove all 0s from the soundex code
    soundex = soundex.replace('0', '')
    # return soundex code padded to len characters
    return (soundex + (4 * '0'))[:4]
    
    
#----------------d----dictionary set--------------
ffd = open('C:\\Users\\liruizhi\\Desktop\\KT kode\\dictionary.txt','r')
ffr = open('C:\\Users\\liruizhi\\Desktop\\dictionary_soundex_code.txt','w')   #result set
myfiled = ffd.readlines()
#memdictionary = ['null' for num in range(len(myfiled))]

i = 0
for fflined in myfiled:
    newfiled = fflined.strip('\n')
    ffr.write(str(newfiled)+' '+str(soundex(str(newfiled)))+'\n')
#    memdictionary[i] = newfiled
    i = i + 1
    print('%d word finished'%(i))
print((time.clock() - start))
ffd.close()
ffr.close()