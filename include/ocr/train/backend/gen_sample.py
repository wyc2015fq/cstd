
import sys
import cv2 as cv
import numpy as np
import random
import matplotlib.pyplot as plt


#draw circle

def randint(min_int,max_int):
    return int(min_int + (max_int-min_int)*random.random())

def randi(min_int,max_int):
    lis = []
    for i in range(len(max_int)):
        lis.append(randint(min_int[i], max_int[i]))
    return tuple(lis)

def circle(img, a, c, r, color):
    img2 = img.copy()
    cv.circle(img2,c, r, color, -1)
    img3 = img*(1-a) + img2*a
    return img3

# 定义旋转rotate函数
def rotate(image1, angle, center=None, scale=1.0):
    k = 50
    image = cv.copyMakeBorder(image1,k,k,k,k,cv.BORDER_REFLECT)
    # 获取图像尺寸
    (h, w) = image.shape[:2]
    # 若未指定旋转中心，则将图像中心设为旋转中心
    if center is None:
        center = (w / 2, h / 2)

    # 执行旋转
    M = cv.getRotationMatrix2D(center, angle, scale)
    rotated = cv.warpAffine(image, M, (w, h))

    # 返回旋转后的图像
    out = rotated[k:(h-k), k:(w-k)]
    return out

def randd(a, b):
    t = random.random()
    return t*(b-a)+a

def rand_crop(img, a, b):
    h,w = img.shape
    c = b-a
    while(1):
        x1 = int(randd(0, c*w))
        y1 = int(randd(0, c*h))
        x2 = int(randd((1-c)*w, w))
        y2 = int(randd((1-c)*h, h))
        if y1>y2:
            y1,y2=y2,y1
        if x1>x2:
            x1,x2=x2,x1
        h1 = y2-y1
        w1 = x2-x1
        t = (h1*w1)/(h*w)
        if t>a and t<=b:
            #print(t, x1, x2, y1, y2)
            return img[y1:y2, x1:x2]



def add_shader(img):
    img3 = img.copy()
    img3 = rotate(img3, randint(-10, 10))
    img3 = rand_crop(img3, 0.8, 1)
    h,w = img3.shape
    k = 20
    #img3 = img3[randint(0,k):h-k,randint(0,k):w-k]
    h,w = img3.shape
    #print(h,w)
    g = 10
    k = 255
    r = 0.5+0.5*random.random()
    img3 = circle(img3,0.1,randi([0,0],[w,h]), randi([50],[255])[0], randi([g,g,g],[k,k,k]))
    img3 = circle(img3, 0.1,randi([w*2/3,0],[w,h]), randi([10],[100])[0], randi([g,g,g],[k,k,k]))

    r = 3+2*int(random.random()*2)
    kernel = cv.getStructuringElement(cv.MORPH_RECT,(r, r))
    if random.random()<0.5:
        img3 = cv.dilate(img3,kernel)
    else:
        img3 = cv.erode(img3,kernel)

    if random.random()>0.5:
        k = 1/(1+random.random())
    else:
        k = 1+random.random()
    img3 = np.power(img3/float(np.max(img3)), k)
    return img3


def add_shader_neg(img):
    img3 = img.copy()
    if 0:
        img3 = rotate(img3, randint(-30, 30))
        img3 = rand_crop(img3, 0.1, 0.6)

    if 0:
        t = randint(30, 360-30)
        img3 = rotate(img3, t)

    if 1:
        img3 = rotate(img3, 180)
        img3 = rand_crop(img3, 0.1, 1)
        img4 = img3.copy()
        kernel = cv.getStructuringElement(cv.MORPH_RECT,(3, 3))
        img3 = cv.erode(img3,kernel)

    h,w = img3.shape
    k = 20
    #img3 = img3[randint(0,k):h-k,randint(0,k):w-k]
    h,w = img3.shape
    #print(h,w)
    g = 10
    k = 255
    r = 0.5+0.5*random.random()
    img3 = circle(img3,0.1,randi([0,0],[w,h]), randi([50],[255])[0], randi([g,g,g],[k,k,k]))
    img3 = circle(img3, 0.1,randi([w*2/3,0],[w,h]), randi([10],[100])[0], randi([g,g,g],[k,k,k]))

    r = 3+2*int(random.random()*2)
    kernel = cv.getStructuringElement(cv.MORPH_RECT,(r, r))
    if random.random()<0.5:
        img3 = cv.dilate(img3,kernel)
    else:
        img3 = cv.erode(img3,kernel)

    if random.random()>0.5:
        k = 1/(1+random.random())
    else:
        k = 1+random.random()
    img3 = np.power(img3/float(np.max(img3)), k)
    return img3


#print(randi(img.shape))
#print(randi([255,255,255]))

#cv.circle(img,(447,63), 63, (255,255,255), -1)
if 0:
    img = cv.imread('E:/OCR_Line/demo_images/018.jpg', 0)
    #print(img.shape)
    #img = cv.cvtColor(img,cv.COLOR_BGR2RGB)
    img2 = cv.resize(img, (30,20),interpolation=cv.INTER_AREA)
    plt.subplot(221)
    plt.imshow(img, cmap=plt.cm.gray)
    plt.subplot(222)
    plt.imshow(img2, cmap=plt.cm.gray)

    img3 = add_shader(img);
    plt.subplot(223)
    plt.imshow(img3, cmap=plt.cm.gray)
    img4 = cv.resize(img3, (60,40),interpolation=cv.INTER_AREA)
    plt.subplot(224)
    plt.imshow(img4, cmap=plt.cm.gray)
    #plt.xticks([])
    #plt.yticks([])
    plt.show()

if 0:
    img = cv.imread('E:/OCR_Line/demo_images/018.jpg', 0)
    for i in range(100):
        print(i)
        img3 = add_shader(img)
        img4 = cv.resize(img3, (60,40),interpolation=cv.INTER_AREA)
        img5 = (img4*255).astype(np.uint8);
        cv.imwrite("E:/OCR_Line/adaboost/pos/%05d.bmp" % i, img5)
    #plt.show()

def readtxtlist(fn):
    flist=[]
    with open(fn,'r') as f:
        for line in f:
            flist.append(line.strip('\n'))
    return flist

if 0:
    li = readtxtlist('./list.txt')
    for j in range(len(li)):
        img = cv.imread(li[j], 0)
        for i in range(50):
            print(j, i)
            img3 = add_shader(img)
            img4 = cv.resize(img3, (60,40),interpolation=cv.INTER_AREA)
            img5 = (img4*255).astype(np.uint8);
            img5 = cv.equalizeHist(img5)
            cv.imwrite("E:/OCR_Line/adaboost/pos/%02d_%05d.bmp" % (j, i), img5)
    #plt.show()

if 1:
    li = readtxtlist('./list.txt')
    for j in range(len(li)):
        img = cv.imread(li[j], 0)
        for i in range(200):
            print(j, i)
            img3 = add_shader_neg(img)
            img4 = cv.resize(img3, (60,40),interpolation=cv.INTER_AREA)
            img5 = (img4*255).astype(np.uint8);
            img5 = cv.equalizeHist(img5)
            cv.imwrite("E:/OCR_Line/adaboost/neg4/%02d_%05d.bmp" % (j, i), img5)
    #plt.show()

if 0:
    flist = readtxtlist('E:/www/news.ifeng.com/jpg/list.txt')
    #print(result)
    for i in range(len(flist)):
        fn = flist[i]
        print(fn)
        img = cv.imread(fn, 0)
        if img.shape(0)>10:
            img4 = cv.resize(img, (60,40),interpolation=cv.INTER_AREA)
            cv.imwrite("E:/OCR_Line/adaboost/neg/%05d.bmp" % i, img4)
