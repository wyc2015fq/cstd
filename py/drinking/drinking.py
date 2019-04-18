

#import random
from random import *

def mysample(p):
    s = sum(p)
    x = random()*s
    t = 0
    for i in range(len(p)):
        t += p[i]
        if t>=x:
            return i
    return 0

all_size = 8
per_size = 2
p = [1, 0.25, 0.5]
inv_p = list(map(lambda x:1/x, p))
#all = sum(p)

#sample(range(2), prob=[])
#print(all)

tt = list(map(lambda x:mysample(inv_p), range(10000)))
#print(tt)

size = all_size
cnt=[0]*len(p)
for i in tt:
    if size>0:
        if size>p[i]:
            size-=p[i]
        else:
            size=0
    else:
        cnt[i]+=1
        size = all_size
        size-=p[i]

ss = sum(cnt)
pp = list(map(lambda x:x/ss, cnt))
print(pp)
