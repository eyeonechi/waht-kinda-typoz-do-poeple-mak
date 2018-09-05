# "C:\Users\liruizhi\Desktop\KT kode\correct.txt"
# function implemention
__author__ = 'Ruizhi Li-941371'
#import pdb
import time
#**********************function definetion*******************************
start = time.clock()

def lev_dist(s1, s2):
    m=len(s1)+1
    n=len(s2)+1
    tbl = {}
    for i in range(m): tbl[i,0]=i
    for j in range(n): tbl[0,j]=j
#    pdb.set_trace()
    for i in range(1, m):
        for j in range(1, n):
            cost = 0 if s1[i-1] == s2[j-1] else 1
            tbl[i,j] = min(tbl[i, j-1]+1, tbl[i-1, j]+1, tbl[i-1, j-1]+cost)
    return tbl[i,j]

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
max_distance = 1
right_count = 0
right_count_3 = 0
right_count_5 = 0
count = 376
count_dict = 0
count_dict_effect = 0
#valtab_mispel =[0 for i in range(10)]
ffr = open('C:\\Users\\liruizhi\\Desktop\\KT kode\\rusult_lev.txt','w')   #result set
#pdb.set_trace()
while count < len(memmispell): 
    ffr.write('word:'+memmispell[count]+'   ')
    while count_dict < len(myfiled):
#        if not(len(memmispell[count]) > 2*len(memdictionary[count_dict]) or len(memdictionary[count_dict])>2*len(memmispell[count])):
        global_dist = lev_dist(memmispell[count],memdictionary[count_dict])
        if  count_dict<10:
            info.append((global_dist,memdictionary[count_dict]))
            info.sort()
            max_distance = max(info)[0]          #instant mininum distance in the list for one word ,updated in every loop
        else:
            if global_dist < max_distance:        # the smaller ,the better
                info.append((global_dist,memdictionary[count_dict]))
                info.sort()
                info.pop()
                max_distance = max(info)[0]
#            count_dict_effect = count_dict_effect+1
        count_dict = count_dict+1
    ffr.write('potential diatance_word:'+str(info[0])+str(info[1])+str(info[2])+str(info[3])+str(info[4]))
#    pdb.set_trace()
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
    max_distance = 1
    info[:] = []
ffr.write('1 given word total num for %d match is %d \n'%(count,right_count))
ffr.write('3 given word total num for %d match is %d \n'%(count,right_count_3+right_count))
ffr.write('5 given word total num for %d match is %d \n'%(count,right_count_5+right_count_3+right_count))

print((time.clock() - start))
#print(max_distance)

ffc.close()
ffd.close()
ffm.close()
ffr.close()