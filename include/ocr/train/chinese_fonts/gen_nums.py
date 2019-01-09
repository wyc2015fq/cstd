
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

def randi(min_int,max_int):
    lis = []
    for i in range(len(max_int)):
        lis.append(randint(min_int[i], max_int[i]))
    return tuple(lis)


def randint(min_int,max_int):
    return int(min_int + (max_int-min_int)*random.random())

def randint1(a, b):
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

def odd(k):
    if (k % 2) == 0:
        return k+1
    return k

def add_erode(img, k):
    k = odd(k)
    kernel = cv2.getStructuringElement(cv2.MORPH_RECT, (k, k))
    img = cv2.erode(img, kernel)
    return img


def add_dilate(img, k):
    k = odd(k)
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

def circle(img, a, c, r, color):
    img2 = img.copy()
    cv.circle(img2,c, r, color, -1)
    img3 = img*(1-a) + img2*a
    return img3

cv = cv2

# 定义旋转rotate函数
def rotate(image1, angle, center=None, scale=1.0):
    k = 50
    image = cv2.copyMakeBorder(image1,k,k,k,k,cv.BORDER_REFLECT)
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

def add_shader(img):
    img3 = img.copy()
    img3 = rotate(img3, randint(-10, 10))
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


# 字体 字体*.ttc的存放路径一般是： /usr/share/fonts/opentype/noto/ 查找指令locate *.ttc
if 1:
    outpath = 'E:/OCR_Line/chars'
    fontfiles = ['huawenxihei.ttf', 'fz-v4.0.ttf']
    pic_per_char = 1000
else:
    outpath = 'E:/OCR_Line/chars1'
    fontfiles = ['fz-v4.0.ttf']
    fontfiles = ['huawenxihei.ttf']
    pic_per_char = 20
    
def normal2(mu, left, right):
    k = np.random.normal(0, 1, 1)
    if k<0:
        return -k*left
    else:
        return k*right

def normalint(mu, left, right):
    return int(normal2(mu, left, right))

charset = '0123456789X'
img_OpenCV = cv2.imread('01.jpg')

if 1:
    fontfiles = os.listdir('D:/fonts/ttf/')
    random.shuffle(fontfiles)
    fontfiles = fontfiles[:200] + ['huawenxihei.ttf', 'fz-v4.0.ttf']
    pic_per_char = 20

#mser = cv2.MSER_create()
index = 0
print(fontfiles)
for fontfile in fontfiles:
    (shotname,extension) = os.path.splitext(fontfile)
    font = ImageFont.truetype('D:/fonts/ttf/'+fontfile, 48)
    for ch in charset:
        str = ch
        # print(dir(font))
        size = font.getsize(str)
        print(ch, size)
        path = '%s/%s/%s' % (outpath, 'images', ch)
        mkdir(path)
        # 需要先把输出的中文字符转换成Unicode编码形式
        #if not isinstance(str, unicode):        str = str.decode('utf8')
        cw = 50
        ch = 50
        for i in range(0, pic_per_char):
            position = (cw, ch)
            w = cw+position[0] + size[0]
            h = ch+position[1] + size[1]
            x = randint(50, 250)+300
            y = randint(50, 300)
            # print(size)
            while 1:
                b = randint(0, 255)
                f = randint(0, 255)
                if abs(b-f)>30:
                    break

            bgColor = (b, b, b)
            fillColor = (f,f,f)
            #fillColor = (randint(0, bgColor[0]), randint(0, bgColor[1]), randint(0, bgColor[2]))
            #img_OpenCV2 = img_OpenCV[y:y+h, x:x+w]
            img_OpenCV2 = np.ones((h,w,3),dtype=np.uint8)
            cv2.rectangle(img_OpenCV2, (0, 0), (w, h), bgColor, -1)
            img_PIL = Image.fromarray(cv2.cvtColor(img_OpenCV2, cv2.COLOR_BGR2RGB))
            draw = ImageDraw.Draw(img_PIL)
            draw.text(position, str, font=font, fill=fillColor)
            img_PIL = img_PIL.rotate(randint(-5, 5))
            # print(dir(font))
            # 使用PIL中的save方法保存图片到本地
            # img_PIL.save('02.jpg', 'jpeg')

            # 转换回OpenCV格式
            #im = cv2.cvtColor(np.asarray(img_PIL), cv2.COLOR_RGB2BGR)
            im = cv2.cvtColor(np.asarray(img_PIL), cv2.COLOR_RGB2GRAY)
            k=7
            if rand()>0.5:
                if b>f:
                    im = add_erode(im, randint(1,k))
                else:
                    im = add_dilate(im, randint(1,k))
            else:
                if b>f:
                    im = add_erode(im, randint(1,k))
                else:
                    im = add_dilate(im, randint(1,k))

            im = add_noise(im, randint(10,50))
            if 1:
                im1 = add_shader(im)
                k=0.4
                im = im1*k + im*(1-k)
            #im = HSVequalizeHist(im)
            im = motion_blur(im, randint(2, 5), randint(0, 360))
            im = gauss_blur(im, 5, randint(5, 10))
            cw1 = cw
            cw2 = w-cw
            ch1 = ch
            ch2 = h-ch
            if 1:
                cw1 = cw1+normalint(0,-10,5)
                cw2 = cw2+normalint(0,-5,10)
                ch1 = ch1+normalint(0,-20,5)
                ch2 = ch2+normalint(0,-5,20)
            im = im[ch1:ch2, cw1:cw2]
            #gray = im
            #regions, box = mser.detectRegions(gray)
            #print(box)
            if im.shape[0]>0 and im.shape[1]>0:
                im = cv2.resize(im, (28, 28))
                #cv2.imshow("print chinese to image",img_OpenCV)
                # cv2.waitKey()
                fn = '%s/%d_%s.jpg' % (path, index, shotname)
                index = index + 1
                print(h, w, fn)
                #cv2.imwrite(fn, im)
                cv2.imencode('.jpg', im)[1].tofile(fn)

