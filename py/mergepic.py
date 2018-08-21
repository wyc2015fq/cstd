#!/usr/bin/python3
#encoding=utf-8

import numpy as np
from PIL import Image
import glob,os
import sortfile

def mergepic(pathname, outpic):
    files=glob.glob(pathname)
    num=len(files)
    #print(files)

    files = sortfile.sort_insert_filename(files)
    print(files[0])

    baseimg=Image.open(files[0])
    sz=baseimg.size
    basemat=np.atleast_2d(baseimg)

    for i in range(1,num):
        file=files[i]
        im=Image.open(file)
        if im.size!=im:
            im=im.resize(sz,Image.ANTIALIAS)
        mat=np.atleast_2d(im)
        print(file)
        basemat=np.append(basemat,mat,axis=1)
    final_img=Image.fromarray(basemat)
    final_img.save(outpic)


prefix = 'D:/code/pudn/ocr/AstarLight/CPS-OCR-Engine/ocr/dataset/test/00000'
mergepic(prefix+'/*', prefix+'merged.png')
