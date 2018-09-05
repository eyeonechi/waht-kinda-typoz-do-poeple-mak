# "C:\Users\liruizhi\Desktop\KT kode\correct.txt"
# function implemention
__author__ = 'Ruizhi Li-941371'
import pdb
import time
start = time.clock()
#**********************function definetion*******************************
#----------------f----needleman wunch-----------------
def needleman_dist(str1,str2):  # str1 is mispelling word # str2 is dictionary word
	ins = -1
	dele = -1
	repl = -1 
	match = 1
	ls1 = len(str1)
	ls2 = len(str2)
	js = 0  # row
	k = 1  #(column)
	l = 0
	valtab =[[0 for i in range(ls2+1)] for i in range(ls1+1)]
	valtab1 = valtab[0]
	#    -------matrix initialization--------------
	for valtab1 in valtab[0]:
		valtab[0][js] = js*dele
		js = js+1
	while k < ls1+1:
		valtab[k][0] = k*ins
		k = k+1
	#    print(valtab1)
	#    -------matrix initialization--------------
	k = 1
	js = 1
	while js < ls1+1:
		while k < ls2+1:
			if str1[js-1] == str2[k-1]:
					valtab[js][k] = max(valtab[js-1][k-1]+match,valtab[js-1][k]+dele,valtab[js][k-1]+ins)
					k = k+1
			else:
#					pdb.set_trace()
					valtab[js][k] = max(valtab[js-1][k-1]+repl,valtab[js-1][k]+dele,valtab[js][k-1]+ins)	  
					k = k+1
		k = 1
		js = js+1
	return valtab[-1][-1]#	print('distance=',valtab[-1][-1])

#**********************function definetion*******************************

num = 0
#**********************data initialization*******************************
#----------------d----correct set-----------------
ffc = open('C:\\Users\\liruizhi\\Desktop\\KT kode\\correct.txt','r')
myfilec = ffc.readlines()
memcorrect = ['null' for num in range(len(myfilec))]

#----------------d----dictionary set--------------
ffd = open('C:\\Users\\liruizhi\\Desktop\\KT kode\\dictionary.txt','r')
myfiled = ffd.readlines()
memdictionary = ['null' for num in range(len(myfiled))]

#----------------d----mispelling set--------------
ffm = open('C:\\Users\\liruizhi\\Desktop\\KT kode\\misspell.txt','r')
myfilem = ffm.readlines()
memmispell = ['null' for num in range(len(myfilem))]
#---------standardization---------------
i = 0
for fflinec in myfilec:
    newfilec = fflinec.strip('\n')      #delete "\n" from original txt
    memcorrect[i] = newfilec
    i = i + 1
#   print ('第 %d 行是：%s 长度为：%d CODE 是：%s' % (i,newfile,len(newfile),newfile.encode()))
i = 0
for fflined in myfiled:
    newfiled = fflined.strip('\n')
    memdictionary[i] = newfiled
    i = i + 1
i = 0
for fflinem in myfilem:
    newfilem = fflinem.strip('\n')
    memmispell[i] = newfilem
    i = i + 1	
#**********************data initialization*******************************
#        three data set are : memmispell.                              .
#                 memcorrect                                          .
#.............memdictionary............................................. 
info = []
min_distance = -10000
right_count = 0
count = 0
count_dict = 0
count_dict_effect = 0
right_count_3 = 0
right_count_5 = 0
#valtab_mispel =[0 for i in range(10)]
ffr = open('C:\\Users\\liruizhi\\Desktop\\KT kode\\rusult_needle_filter.txt','w')   #result set
#pdb.set_trace()
while count < len(memmispell): 
    ffr.write('word:'+memmispell[count]+'   ')
    while count_dict < len(myfiled):
        if not(len(memmispell[count]) > 2*len(memdictionary[count_dict]) or len(memdictionary[count_dict])>2*len(memmispell[count])):
            global_dist = needleman_dist(memmispell[count],memdictionary[count_dict])
            if  count_dict_effect<10:
                info.append((global_dist,memdictionary[count_dict]))
                info.sort(reverse = True)
                min_distance = min(info)[0]          #instant mininum distance in the list for one word ,updated in every loop
            else:
                if global_dist > min_distance:
                    info.append((global_dist,memdictionary[count_dict]))
                    info.sort(reverse = True)
                    info.pop()
                    min_distance = min(info)[0]
            count_dict_effect = count_dict_effect+1
        count_dict = count_dict+1
    ffr.write('potential diatance_word:'+str(info[0])+str(info[1])+str(info[2])+str(info[3])+str(info[4]))
    if (memcorrect[count] == info[0][1])or(memcorrect[count] == info[1][1])or(memcorrect[count] == info[2][1])or(memcorrect[count] == info[3][1])or(memcorrect[count] == info[4][1]):
        if(memcorrect[count] == info[0][1])or(memcorrect[count] == info[1][1])or(memcorrect[count] == info[2][1]):
            if (memcorrect[count] == info[0][1]):
                ffr.write('   1M   \n')
                right_count = right_count+1
            else:
                ffr.write('   2-3M   \n')
                right_count_3 = right_count_3+1
        else:
            ffr.write('   4-5M   \n')
            right_count_5 = right_count_5+1
    else:
        ffr.write('   N   \n')
    count = count + 1
    print('%d word finished'%(count))
    count_dict = 0
    count_dict_effect = 0
    min_distance = -10000
    info[:] = []
ffr.write('1 given word total num for %d match is %d \n'%(count,right_count))
ffr.write('3 given word total num for %d match is %d \n'%(count,right_count_3+right_count))
ffr.write('5 given word total num for %d match is %d \n'%(count,right_count_5+right_count_3+right_count))
print((time.clock() - start))

ffc.close()
ffd.close()
ffm.close()
ffr.close()