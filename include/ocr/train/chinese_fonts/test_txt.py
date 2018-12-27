
import cv2
import os
import sys
import numpy as np
import random
from PIL import Image, ImageDraw, ImageFont

ch = u'1advafsdf'
bch = ch.encode(encoding='utf-8')
print(type(bch))


sys.path.append('/test')  # 要用绝对路径
print(sys.path)  # 查看模块路径


def rand():
    return random.random()


def randint(a, b):
    return random.randint(a, b)


def randint1(b):
    return random.randint(0, b)


def mkdir(path):
    isExists = os.path.exists(path)
    if not isExists:
        os.makedirs(path)


def gauss_blur(image, ksize, sigma):
    return cv2.GaussianBlur(image, (ksize,ksize), sigma, sigma)


def motion_blur(image, degree=12, angle=45):
    image = np.array(image)
    # 这里生成任意角度的运动模糊kernel的矩阵， degree越大，模糊程度越高
    M = cv2.getRotationMatrix2D((degree / 2, degree / 2), angle, 1)
    motion_blur_kernel = np.diag(np.ones(degree))
    motion_blur_kernel = cv2.warpAffine(
        motion_blur_kernel, M, (degree, degree))

    motion_blur_kernel = motion_blur_kernel / degree
    blurred = cv2.filter2D(image, -1, motion_blur_kernel)

    # convert to uint8
    cv2.normalize(blurred, blurred, 0, 255, cv2.NORM_MINMAX)
    blurred = np.array(blurred, dtype=np.uint8)
    return blurred


def add_noise(img, n):
    for i in range(n):  # 添加点噪声
        temp_x = np.random.randint(0, img.shape[0])
        temp_y = np.random.randint(0, img.shape[1])
        img[temp_x][temp_y] = 255
    return img


def add_erode(img, k):
    if rand() > 0.5:
        kernel = cv2.getStructuringElement(cv2.MORPH_RECT, (k, k))
        img = cv2.erode(img, kernel)
    return img


def add_dilate(img, k):
    if rand() > 0.5:
        kernel = cv2.getStructuringElement(cv2.MORPH_RECT, (k, k))
        img = cv2.dilate(img, kernel)
    return img


def bytes2hexstr(a_bytes):
    return ''.join(['%02x' % b for b in a_bytes])


def YUVequalizeHist(img):
    imgYUV = cv2.cvtColor(img, cv2.COLOR_BGR2YCrCb)
    channelsYUV = cv2.split(imgYUV)
    channelsYUV[0] = cv2.equalizeHist(channelsYUV[0])
    channels = cv2.merge(channelsYUV)
    result = cv2.cvtColor(channels, cv2.COLOR_YCrCb2BGR)
    return result

def HSVequalizeHist(img):
    imgHSV = cv2.cvtColor(img, cv2.COLOR_BGR2HSV)
    channelsHSV = cv2.split(imgHSV)
    channelsHSV[2] = cv2.equalizeHist(channelsHSV[2])
    channels = cv2.merge(channelsHSV)
    result = cv2.cvtColor(channels, cv2.COLOR_HSV2BGR)
    return result

def equalizeHist(img):
    (b, g, r) = cv2.split(img)
    bH = cv2.equalizeHist(b)
    gH = cv2.equalizeHist(g)
    rH = cv2.equalizeHist(r)
    # 合并每一个通道
    result = cv2.merge((bH, gH, rH))
    return result

def mul(im, s):
    im = im.astype(np.float32)*s
    return im.astype(np.uint8)

# 字体 字体*.ttc的存放路径一般是： /usr/share/fonts/opentype/noto/ 查找指令locate *.ttc
if 0
    outpath = 'E:/OCR_Line/chars'
    fontfiles = ['huawenxihei.ttf', 'fz-v4.0.ttf']
    pic_per_char = 1000
else:
    outpath = 'E:/OCR_Line/chars1'
    fontfiles = ['huawenxihei.ttf']
    fontfiles = ['fz-v4.0.ttf']
    pic_per_char = 10
    

charset = '0123456789X'
img_OpenCV = cv2.imread('01.jpg')

for fontfile in fontfiles:
    (shotname,extension) = os.path.splitext(fontfile)
    font = ImageFont.truetype(fontfile, 48)
    for ch in charset:
        str = ch
        # print(dir(font))
        size = font.getsize(str)
        print(ch, size)
        path = '%s/%s/%s' % (outpath, shotname, ch)
        mkdir(path)
        # 需要先把输出的中文字符转换成Unicode编码形式
        #if not isinstance(str, unicode):        str = str.decode('utf8')

        for i in range(0, pic_per_char):
            xoff =  - randint(0, 5)
            yoff =  - randint(0, 5)
            position = (5+randint(0, 2)+xoff, 5+randint(0, 2)+yoff)
            w = 10+position[0] + size[0] + xoff
            h = 10+position[1] + size[1] + yoff
            x = randint(50, 250)+300
            y = randint(50, 300)
            # print(size)
            fillColor = (randint1(255), randint1(255), randint1(255))
            c = randint(0, 150)
            fillColor = (c, c, c)
            img_OpenCV2 = img_OpenCV[y:y+h, x:x+w]
            if rand()>0.5:
                img_OpenCV2 = mul(img_OpenCV2, 1/(1+0.2*rand()))
            img_PIL = Image.fromarray(cv2.cvtColor(img_OpenCV2, cv2.COLOR_BGR2RGB))
            draw = ImageDraw.Draw(img_PIL)
            draw.text(position, str, font=font, fill=fillColor)
            img_PIL = img_PIL.rotate(randint(-5, 5))
            # print(dir(font))
            # 使用PIL中的save方法保存图片到本地
            # img_PIL.save('02.jpg', 'jpeg')

            # 转换回OpenCV格式
            im = cv2.cvtColor(np.asarray(img_PIL), cv2.COLOR_RGB2BGR)
            im = add_noise(im, randint(10,20))
            if 1:
                im = add_erode(im, 3)
                im = add_dilate(im, 3)

            #im = HSVequalizeHist(im)
            im = motion_blur(im, randint(2, 5), randint(0, 360))
            im = gauss_blur(im, randint(1,3)*2+1, randint(5, 10))
            tosize = (32, 32)
            im = im[5:(h-5), 5:(w-5)]
            im = cv2.resize(im, tosize)
            #cv2.imshow("print chinese to image",img_OpenCV)
            # cv2.waitKey()
            fn = '%s/%d.jpg' % (path, i)
            print(h, w, fn)
            #cv2.imwrite(fn, im)
            cv2.imencode('.jpg', im)[1].tofile(fn)

