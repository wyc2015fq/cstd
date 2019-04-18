# OpenCV—python 图像增强 - wsp_1138886114的博客 - CSDN博客





2018年10月21日 10:40:36[SongpingWang](https://me.csdn.net/wsp_1138886114)阅读数：1292








### 一、图像增强（该代码有问题）

```python
import os
import cv2
import random
import numpy as np


def random_crop(img, area_ratio, hw_vari):
    """
    :param img:
    :param area_ratio: 裁剪画面占原画面比例(0,0.5)
    :param hw_vari:    扰动占原高宽比的范围(0.2)
    x0: 左上角横坐标
    y0: 左上角纵坐标
    w:  裁剪宽度
    h:  裁剪高度
    :return:
    """
    h, w = img.shape[:2]
    hw_delta = np.random.uniform(-hw_vari, hw_vari)
    hw_mult = 1 + hw_delta
    w_crop = int(round(w*np.sqrt(area_ratio*hw_mult)))
    if w_crop > w - 2:
        w_crop = w - 2
    h_crop = int(round(h*np.sqrt(area_ratio/hw_mult)))
    if h_crop > h - 2:
        h_crop = h - 2
    x0 = np.random.randint(0, w-w_crop-1)
    y0 = np.random.randint(0, h-h_crop-1)
    crop_image = lambda img, x0, y0, w, h: img[y0:y0 + h, x0:x0 + w]
    return crop_image


def random_rotate(img, angle_vari, p_crop):
    """
    :param img:
    :param angle_vari: 旋转角度（逆时针）180
    :param p_crop:     去黑边裁剪比例0.5
    :return:
    """
    angle = np.random.uniform(-angle_vari, angle_vari)
    crop = False if np.random.random() > p_crop else True

    h, w = img.shape[:2]
    angle %= 360
    M_rotate = cv2.getRotationMatrix2D((w / 2, h / 2), angle, 1)
    img_rotated = cv2.warpAffine(img, M_rotate, (w, h))

    if crop:
        angle_crop = angle % 180
        if angle_crop > 90:
            angle_crop = 180 - angle_crop
        theta = angle_crop * np.pi / 180.0
        hw_ratio = float(h) / float(w)
        tan_theta = np.tan(theta)
        numerator = np.cos(theta) + np.sin(theta) * tan_theta
        r = hw_ratio if h > w else 1 / hw_ratio
        denominator = r * tan_theta + 1
        crop_mult = numerator / denominator
        w_crop = int(round(crop_mult * w))
        h_crop = int(round(crop_mult * h))
        x0 = int((w - w_crop) / 2)
        y0 = int((h - h_crop) / 2)
        img_rotated = lambda img, x0, y0, w, h: img[y0:y0 + h, x0:x0 + w]
    return img_rotated


def random_hsv_transform(img, hue_vari, sat_vari, val_vari):
    """
    :param img:
    :param hue_vari: 色调变化比例范围
    :param sat_vari: 饱和度变化比例范围
    :param val_vari: 明度变化比例范围
    :return:
    """
    hue_delta = np.random.randint(-hue_vari, hue_vari)
    sat_mult = 1 + np.random.uniform(-sat_vari, sat_vari)
    val_mult = 1 + np.random.uniform(-val_vari, val_vari)

    img_hsv = cv2.cvtColor(img, cv2.COLOR_BGR2HSV).astype(np.float)
    img_hsv[:, :, 0] = (img_hsv[:, :, 0] + hue_delta) % 180
    img_hsv[:, :, 1] *= sat_mult
    img_hsv[:, :, 2] *= val_mult
    img_hsv[img_hsv > 255] = 255
    return cv2.cvtColor(np.round(img_hsv).astype(np.uint8), cv2.COLOR_HSV2BGR)


def random_gamma_transform(img, gamma_vari):
    log_gamma_vari = np.log(gamma_vari)
    alpha = np.random.uniform(-log_gamma_vari, log_gamma_vari)
    gamma = np.exp(alpha)
    gamma_table = [np.power(x / 255.0, gamma) * 255.0 for x in range(256)]
    gamma_table = np.round(np.array(gamma_table)).astype(np.uint8)
    return cv2.LUT(img, gamma_table)

def random_flip_img(img):
    """
    0 = X axis, 1 = Y axis,  -1 = both
    :param img:
    :return:
    """
    flip_val = [0,1,-1]
    random_flip_val = random.choice(flip_val)
    res = cv2.flip(img, random_flip_val)
    return res

if __name__ == '__main__':

    output_dir = "./temp_dir"
    input_dir = "./temp_dir"
    index = 1
    for child_dir in os.listdir(input_dir):
        child_path = os.path.join(input_dir, child_dir)
        for dir_image in os.listdir(child_path):
            print(child_path)
            Suffix_name = ['.png', '.jpg', '.jpeg']
            if dir_image.endswith(tuple(Suffix_name)):
                img = cv2.imread(os.path.join(child_path, dir_image))

                random_crop_img = random_crop(img,0.2,0.2)
                cv2.imwrite(child_path + '/crop_img' + str(index) + '.png', random_crop_img)

                res_rotate = random_rotate(img, 180, 0.5)
                cv2.imwrite(child_path + '/rotate_img' + str(index) + '.png', res_rotate)

                HSV_IMG = random_hsv_transform(img,2,2,3)
                cv2.imwrite(child_path + '/HSV_IMG' + str(index) + '.png', HSV_IMG)

                GAMMA_IMG = random_gamma_transform(img,0.2)
                cv2.imwrite(child_path + '/GAMMA_IMG' + str(index) + '.png', GAMMA_IMG)

                res_flip = random_flip_img(img)
                cv2.imwrite(child_path + './flip_img'+ str(index) + '.png', res_flip)

                index+=1
```

### 二、图像增强

获取目录下所有文件夹以及其里面的图片文件并对其执行图像增强

```python
import os
import cv2
import random
import numpy as np
from scipy import *


def Affine_transformation(img):
    """
    :param img:
    :return:
    """
    h, w = img.shape[:2]
    points1 = np.float32([[50, 50], [200, 50], [50, 200]])
    points2 = np.float32([[30, 80], [200, 50], [80, 220]])

    matrix = cv2.getAffineTransform(points1, points2)
    Affine_transfor_img = cv2.warpAffine(img, matrix, (h, w))
    return Affine_transfor_img

def random_rotate_img(img, min_angle, max_angle):
    '''
        random rotation an image

    :param img:         image to be rotated
    :param min_angle:   min angle to rotate
    :param max_angle:   max angle to rotate
    :return:            image after random rotated
    '''
    if not isinstance(img, list):
        img = [img]

    angle = random.randint(min_angle, max_angle)
    center = (img[0].shape[0] / 2, img[0].shape[1] / 2)
    rot_matrix = cv2.getRotationMatrix2D(center, angle, scale=1.0)

    res = []
    for img_inst in img:
        img_inst = cv2.warpAffine(img_inst, rot_matrix, dsize=img_inst.shape[:2], borderMode=cv2.BORDER_CONSTANT)
        res.append(img_inst)
    if len(res) == 0:
        res = res[0]
    return res

def random_hsv_transform(img, hue_vari, sat_vari, val_vari):
    """
    :param img:
    :param hue_vari: 色调变化比例范围(0,360)
    :param sat_vari: 饱和度变化比例范围(0,1)
    :param val_vari: 明度变化比例范围(0,1)
    :return:
    """
    hue_delta = np.random.randint(-hue_vari, hue_vari)
    sat_mult = 1 + np.random.uniform(-sat_vari, sat_vari)
    val_mult = 1 + np.random.uniform(-val_vari, val_vari)

    img_hsv = cv2.cvtColor(img, cv2.COLOR_BGR2HSV).astype(np.float)
    img_hsv[:, :, 0] = (img_hsv[:, :, 0] + hue_delta) % 180
    img_hsv[:, :, 1] *= sat_mult
    img_hsv[:, :, 2] *= val_mult
    img_hsv[img_hsv > 255] = 255
    return cv2.cvtColor(np.round(img_hsv).astype(np.uint8), cv2.COLOR_HSV2BGR)

def random_gamma_transform(img, gamma_vari):
    """
    :param img:
    :param gamma_vari:
    :return:
    """
    log_gamma_vari = np.log(gamma_vari)
    alpha = np.random.uniform(-log_gamma_vari, log_gamma_vari)
    gamma = np.exp(alpha)
    gamma_table = [np.power(x / 255.0, gamma) * 255.0 for x in range(256)]
    gamma_table = np.round(np.array(gamma_table)).astype(np.uint8)
    return cv2.LUT(img, gamma_table)

def random_flip_img(img):
    """
    0 = X axis, 1 = Y axis,  -1 = both
    :param img:
    :return:
    """
    flip_val = [0,1,-1]
    random_flip_val = random.choice(flip_val)
    res = cv2.flip(img, random_flip_val)
    return res


def SaltAndPepper(img):   #定义添加椒盐噪声的函数
    """
    :param img:
    :return:
    """
    SP_NoiseImg=img
    percetage = np.random.randint(3) * 0.1
    SP_NoiseNum=int(percetage*img.shape[0]*img.shape[1])
    for i in range(SP_NoiseNum):
        randX=random.random_integers(0,img.shape[0]-1)
        randY=random.random_integers(0,img.shape[1]-1)
        if random.random_integers(0,1)==0:
            SP_NoiseImg[randX,randY]=0
        else:
            SP_NoiseImg[randX,randY]=255
    return SP_NoiseImg

def clamp(pv):     #防止像素溢出
    if pv > 255:
        return 255
    if pv < 0:
        return 0
    else:
        return pv

def gaussian_noise(image):   # 加高斯噪声
    """
    :param image:
    :return:
    """
    h, w, c = image.shape
    for row in range(h):
        for col in range(w):
            s = np.random.normal(0, 20, 3)
            b = image[row, col, 0] # blue
            g = image[row, col, 1] # green
            r = image[row, col, 2] # red
            image[row, col, 0] = clamp(b + s[0])
            image[row, col, 1] = clamp(g + s[1])
            image[row, col, 2] = clamp(r + s[2])
    return image



if __name__ == '__main__':
    input_dir = "./face_dataset/test_faces"
    index = 1
    for child_dir in os.listdir(input_dir):
        child_path = input_dir +"/"+ child_dir
        for dir_image in os.listdir(child_path):
            Suffix_name = ['.png', '.jpg', '.jpeg']
            if dir_image.endswith(tuple(Suffix_name)):
                img = cv2.imread(child_path +"/"+ dir_image)
                print("正在处理：",child_path +"/"+ dir_image,"...")

                Affine_transfor_img = Affine_transformation(img)
                cv2.imwrite(child_path + "/" + str(dir_image[:-4]) + '_Affine_transfor_img' + str(index) + '.png', Affine_transfor_img)

                res_rotate = random_rotate_img(img, 20, 70)
                cv2.imwrite(child_path +"/" +str(dir_image[:-4])+ '_rotate_img' + str(index) + '.png', res_rotate[0])

                HSV_IMG = random_hsv_transform(img,2,0.3,0.6)
                cv2.imwrite(child_path +"/" +str(dir_image[:-4])+'_HSV_IMG' + str(index) + '.png', HSV_IMG)

                GAMMA_IMG = random_gamma_transform(img,0.3)
                cv2.imwrite(child_path +"/" +str(dir_image[:-4])+'_GAMMA_IMG' + str(index) + '.png', GAMMA_IMG)

                res_flip = random_flip_img(img)
                cv2.imwrite(child_path +"/" +str(dir_image[:-4])+'_flip_img'+ str(index) + '.png', res_flip)

                # SP_NoiseImg = SaltAndPepper(img)
                # cv2.imwrite(child_path + "/" + str(dir_image[:-4]) + '_SP_Noise_img' + str(index) + '.png', SP_NoiseImg)

                G_Noiseimg = gaussian_noise(img)
                cv2.imwrite(child_path + "/" + str(dir_image[:-4]) + '_G_Noise_img' + str(index) + '.png', G_Noiseimg)

                index+=1
```

### 开启多线程

```python
import os
import cv2
import random
import time
import numpy as np
import threading
from scipy import *


def Affine_transformation(img):
    """
    :param img:
    :return:
    """
    # pointA = np.float32([[70, 30], [200, 50], [20,180]])
    # pointB = np.float32([[30, 80], [200, 50], [80, 220]])
    h, w = img.shape[:2]
    points1 = np.float32([[50, 50], [200, 50], [50, 200]])
    points2 = np.float32([[random.choice([30,40]), random.choice([70,80])],
                          [200, 50], [random.choice([70,80]), random.choice([210,220])]])
    matrix = cv2.getAffineTransform(points1, points2)
    Affine_transfor_img = cv2.warpAffine(img, matrix, (h, w))
    return Affine_transfor_img

def random_rotate_img(img, min_angle, max_angle):
    '''
        random rotation an image

    :param img:         image to be rotated
    :param min_angle:   min angle to rotate
    :param max_angle:   max angle to rotate
    :return:            image after random rotated
    '''
    if not isinstance(img, list):
        img = [img]

    angle = random.randint(min_angle, max_angle)
    center = (img[0].shape[0] / 2, img[0].shape[1] / 2)
    rot_matrix = cv2.getRotationMatrix2D(center, angle, scale=1.0)

    res = []
    for img_inst in img:
        img_inst = cv2.warpAffine(img_inst, rot_matrix, dsize=img_inst.shape[:2], borderMode=cv2.BORDER_CONSTANT)
        res.append(img_inst)
    if len(res) == 0:
        res = res[0]
    return res

def random_hsv_transform(img, hue_vari, sat_vari, val_vari):
    """
    :param img:
    :param hue_vari: 色调变化比例范围(0,360)
    :param sat_vari: 饱和度变化比例范围(0,1)
    :param val_vari: 明度变化比例范围(0,1)
    :return:
    """
    hue_delta = np.random.randint(-hue_vari, hue_vari)
    sat_mult = 1 + np.random.uniform(-sat_vari, sat_vari)
    val_mult = 1 + np.random.uniform(-val_vari, val_vari)

    img_hsv = cv2.cvtColor(img, cv2.COLOR_BGR2HSV).astype(np.float)
    img_hsv[:, :, 0] = (img_hsv[:, :, 0] + hue_delta) % 180
    img_hsv[:, :, 1] *= sat_mult
    img_hsv[:, :, 2] *= val_mult
    img_hsv[img_hsv > 255] = 255
    return cv2.cvtColor(np.round(img_hsv).astype(np.uint8), cv2.COLOR_HSV2BGR)

def random_gamma_transform(img, gamma_vari):
    """
    :param img:
    :param gamma_vari:
    :return:
    """
    log_gamma_vari = np.log(gamma_vari)
    alpha = np.random.uniform(-log_gamma_vari, log_gamma_vari)
    gamma = np.exp(alpha)
    gamma_table = [np.power(x / 255.0, gamma) * 255.0 for x in range(256)]
    gamma_table = np.round(np.array(gamma_table)).astype(np.uint8)
    return cv2.LUT(img, gamma_table)

def random_flip_img(img):
    """
    0 = X axis, 1 = Y axis,  -1 = both
    :param img:
    :return:
    """
    flip_val = [0,1,-1]
    random_flip_val = random.choice(flip_val)
    res = cv2.flip(img, random_flip_val)
    return res


def SaltAndPepper(img):   #定义添加椒盐噪声的函数
    """
    :param img:
    :return:
    """
    SP_NoiseImg=img
    percetage = np.random.randint(3) * 0.1
    SP_NoiseNum=int(percetage*img.shape[0]*img.shape[1])
    for i in range(SP_NoiseNum):
        randX=random.random_integers(0,img.shape[0]-1)
        randY=random.random_integers(0,img.shape[1]-1)
        if random.random_integers(0,1)==0:
            SP_NoiseImg[randX,randY]=0
        else:
            SP_NoiseImg[randX,randY]=255
    return SP_NoiseImg

def clamp(pv):     #防止像素溢出
    if pv > 255:
        return 255
    if pv < 0:
        return 0
    else:
        return pv

def gaussian_noise(image):   # 加高斯噪声
    """
    :param image:
    :return:
    """
    h, w, c = image.shape
    for row in range(h):
        for col in range(w):
            s = np.random.normal(0, 20, 3)
            b = image[row, col, 0] # blue
            g = image[row, col, 1] # green
            r = image[row, col, 2] # red
            image[row, col, 0] = clamp(b + s[0])
            image[row, col, 1] = clamp(g + s[1])
            image[row, col, 2] = clamp(r + s[2])
    return image



def get_img(input_dir):
    img_path_list = []
    for (root_path,dirname,filenames) in os.walk(input_dir):
        for filename in filenames:
            img_path = root_path+"/"+filename
            img_path_list.append(img_path)
    return  img_path_list



def start0(input_dir):
    index = 1
    img_path_list = get_img(input_dir)
    for img_path in img_path_list:
        Suffix_name = ['.png', '.jpg', '.jpeg']
        if img_path.endswith(tuple(Suffix_name)):
            print("线程1：处理图像",img_path)
            img = cv2.imread(img_path)
            GAMMA_IMG = random_gamma_transform(img, 0.3)
            cv2.imwrite(img_path[:-4] + '_GAMMA_IMG' + str(index) + '.png', GAMMA_IMG)

            res_flip = random_flip_img(img)
            cv2.imwrite(img_path[:-4] + '_flip_img' + str(index) + '.png', res_flip)

            G_Noiseimg = gaussian_noise(img)
            cv2.imwrite(img_path[:-4] + '_G_Noise_img' + str(index) + '.png', G_Noiseimg)
            index += 1

def start1(input_dir):
    index = 1
    img_path_list = get_img(input_dir)
    for img_path in img_path_list:
        Suffix_name = ['.png', '.jpg', '.jpeg']
        if img_path.endswith(tuple(Suffix_name)):
            print("线程2：处理图像", img_path)
            img = cv2.imread(img_path)

            Affine_transfor_img = Affine_transformation(img)
            cv2.imwrite(img_path[:-4] + '_Affine_transfor_img' + str(index) + '.png',Affine_transfor_img)

            res_rotate = random_rotate_img(img, 20, 70)
            cv2.imwrite(img_path[:-4] + '_rotate_img' + str(index) + '.png', res_rotate[0])

            HSV_IMG = random_hsv_transform(img, 2, 0.3, 0.6)
            cv2.imwrite(img_path[:-4] + '_HSV_IMG' + str(index) + '.png', HSV_IMG)
            index += 1


input_dir = "./All_faces"
threads = []
t1 = threading.Thread(target=start0,args=(input_dir,))
threads.append(t1)
t2 = threading.Thread(target=start1,args=(input_dir,))
threads.append(t2)

if __name__ == '__main__':
    start_time = time.time()
    for t in threads:
        t.setDaemon(True)
        t.start()
    for t in threads:
        t.join()

    end_time = time.time()
    print("done! \n Total Spend time：",str((end_time - start_time)/60)[0:6]+"分钟")
```



