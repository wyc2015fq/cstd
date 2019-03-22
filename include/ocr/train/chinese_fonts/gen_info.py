#! /usr/bin/env python
# -*- coding: utf-8 -*-
from gen_utils import *
from allcharset import *
import os

outpath = 'E:/OCR_Line/lines/han100w/'
imgoutpath = outpath+'img'
root = outpath
if 0:
    dirs = dirlist(imgoutpath)

    infos = []

    iter = 0

    # 输出所有文件和文件夹
    for fn in dirs:
        (filepath, tempfilename) = os.path.split(fn)
        (filename, extension) = os.path.splitext(tempfilename)
        a = filename.split('_')
        b = list(map(int, a))
        la = ' '.join(map(str, b[1:]))
        s = '%s %s' % (fn, la)
        infos.append(s)
        iter = iter+1
        if iter%1000==0:
            print(iter)

    if 1:
        count = len(infos)
        count_train = int(count*0.8)

        outtxt = root+'list.txt'
        savelines(infos, root+'infos.txt')
        savelines(infos[:count_train], root+'train.txt')
        savelines(infos[count_train:], root+'test.txt')
    

f_infos = open(root+'infos.txt', 'w')
f_train = open(root+'train.txt', 'w')
f_test = open(root+'test.txt', 'w')

num = 200
count_train = int(num*0.8)
for i in range(0,num):
    filepath = "%s/%d"%(imgoutpath, i)
    print(filepath)
    ff = f_test
    if i<count_train:
        ff = f_train
    else:
        ff = f_test
    for fi in os.listdir(filepath):
        fn = os.path.join(filepath, fi)
        (filepath, tempfilename) = os.path.split(fn)
        (filename, extension) = os.path.splitext(tempfilename)
        a = filename.split('_')
        b = list(map(int, a))
        la = ' '.join(map(str, b[1:]))
        s = '%s %s' % (fn, la)
        f_infos.write(s+'\n')
        ff.write(s+'\n')

        
f_infos.close()
f_train.close()
f_test.close()
