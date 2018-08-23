# A simple script which uses the "editdistance" package
# to determine the best match for a single string
# NJ, Aug 2018

import editdistance

# Let's load the entire dictionary into a list
f = open("./dict.txt",'r')
my_dict = f.readlines()
f.close()

# Now we compare each dictionary entry with a string;
# we'll keep track of the best entry we've seen so far:
f = open("./wiki_misspell.txt",'r')
for line in f:
	string = line.strip()
	bestv = 10000000 # This is intentionally overkill
	bests = ""
	for entry in my_dict:
    	thisv = editdistance.eval(string,entry.strip())
    	if (thisv < bestv):
        	# Note that this script only updates the best entry when it is better than all of the previous ones
        	# It turns out that this is actually a really bad strategy
        	bests = entry.strip()
        	bestv = thisv
        	
	print(string,bests,bestv)
