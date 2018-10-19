#!/usr/bin/python
# -*- coding: UTF-8 -*-
 
for i in range(1,5):
    for j in range(1,5):
        for k in range(1,5):
            if( i != k ) and (i != j) and (j != k):
                print(i,j,k)


for i in range(3): print(i);print(i)


def f(a,b,c):
    return a+b+c

print(f(1,c=1,b=2))