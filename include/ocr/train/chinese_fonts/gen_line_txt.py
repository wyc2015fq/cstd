#! /usr/bin/env python
# -*- coding: utf-8 -*-
from gen_utils import *
from allcharset import *


chars = '0123456789函数则是相当于'
chars = '01'

if 1:
    #random.shuffle(fontfiles)
    #fontfiles = fontfiles[:200] + ['huawenxihei.ttf', 'fz-v4.0.ttf']
    fontfiles = listdir('D:/fonts/ttf_han/')
    fontfiles = listdir('D:/fonts/ttf/')
    fontfiles = fullpath('D:/fonts/ttf/', ['huawenxihei.ttf', 'fz-v4.0.ttf'])
    outpath = 'D:/OCR_Line/lines/en'
    chars = '0123456789X'

if 1:
    #random.shuffle(fontfiles)
    #fontfiles = fontfiles[:200] + ['huawenxihei.ttf', 'fz-v4.0.ttf']
    fontfiles = listdir('D:/fonts/ttf/')
    fontfiles = fullpath('D:/fonts/ttf/', ['huawenxihei.ttf', 'fz-v4.0.ttf'])
    fontfiles = listdir('D:/fonts/ttf_han/')
    outpath = 'E:/OCR_Line/lines/han200w'
    chars = nums+ens+hans[0:4000]
    other_chars = other+fuhao

root = outpath + '/'
infos_fn = root+'infos.txt'

imgoutpath = outpath+'/img'
mkdir(outpath)
mkdir(imgoutpath)
fonts = []

for fontfile in fontfiles:
    font = ImageFont.truetype(fontfile, fontsize)
    fonts.append(font)


if 1:
    print(len(chars))
    labels = ['blank']

    for i in chars:
        labels.append(i)

    savelines(labels, root+'label.txt')

infos = []

def gen_char():
    if rand() < 0.9:
        return chars[randint(0, len(chars))]
    return ' '

beginid = 0

if 0:
    for root, dirs, files in os.walk(imgoutpath):  # path 为根目录
        for t in files:
            a = t.split('.', 1)[0]
            a = a.split('_')[0]
            i = int(a)
            if i>=beginid:
                beginid = i+1

infos = []
if 0:
    if os.path.exists(infos_fn):
        with open(infos_fn, 'r') as file_to_read:
            for line in file_to_read.readlines(): # 整行读取数据
                line=line.strip('\n')
                if len(line)>0:
                    infos.append(line)

base = 10000
num = 100*base

for i in range(beginid, num):
    if 1:
        j = i//base
        if i%base==0:
            print(j)
        fn = '%s/%d' % (imgoutpath, j)
        if not os.path.exists(fn):
            mkdir(fn)
        la = '_'.join(map(str, ss))
        fn = '%s/%d/%d_%s.jpg' % (imgoutpath, j, i, la)
        im = cv2.resize(im, (280, 32))
        la = ' '.join(map(str, ss))
        s = '%s %s' % (fn, la)
        infos.append(s)
        #print(h, w, fn)
        # cv2.imwrite(fn, im)
        cv2.imencode('.jpg', im)[1].tofile(fn)

