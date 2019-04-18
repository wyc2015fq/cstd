# python 多进程 - wsp_1138886114的博客 - CSDN博客





2019年03月02日 16:16:26[SongpingWang](https://me.csdn.net/wsp_1138886114)阅读数：51








在几次测试过程中，发现 python 多线程与多进程 对IO密集型操作有明显的加速作用，对计算密集型的操作并没有什么效果。
- 若是你的运算是计算密集型（比如对图像进行各种矩阵运算，添加高斯噪声等）python的多线程或者多进程并没有很大帮助，你可以使用 C++ 来实现 多线程 或者 多进程。
- 若是你的运算是 IO 密集型（比如批量读取图像，移动，复制，重命名等）python的多线程或者多进程将会帮你节约大量的时间。（当然你也可以使用C++，不过python简单，代码量少。）

### 多进程—图像增强的代码实例（三个不同的函数实现三个进程）

```python
"""
开启多进程：图像增强
"""
import os
import random
import numpy as np
import cv2
import multiprocessing
import time

def Affine_transformation(img_array):
    rows, cols = img_array.shape[:2]
    pointsA = np.float32([[30, 80], [180, 60], [80, 230]])  # 左偏
    pointsB = np.float32([[60, 50], [220, 70], [20, 180]])  # 右偏
    pointsC = np.float32([[70, 60], [180, 50], [50, 200]])  # 前偏
    pointsD = np.float32([[40, 50], [210, 60], [70, 180]])  # 后偏

    points1 = np.float32([[50, 50], [200, 50], [50, 200]])
    points2 = random.choice((pointsA, pointsB, pointsC, pointsD))

    matrix = cv2.getAffineTransform(points1, points2)
    Affine_transfor_img = cv2.warpAffine(img_array, matrix, (cols, rows))
    return Affine_transfor_img

def random_rotate_img(img):
    rows, cols= img.shape[:2]
    angle = random.choice([25, 90, -25, -90, 180])
    Matrix = cv2.getRotationMatrix2D((cols / 2, rows / 2), angle, 1)
    res = cv2.warpAffine(img, Matrix, (cols, rows), borderMode=cv2.BORDER_CONSTANT)
    return res

def random_hsv_transform(img, hue_vari, sat_vari, val_vari):
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
    flip_val = [0,1,-1]
    random_flip_val = random.choice(flip_val)
    res = cv2.flip(img, random_flip_val)
    return res

def clamp(pv):     #防止像素溢出
    if pv > 255:
        return 255
    if pv < 0:
        return 0
    else:
        return pv

def gaussian_noise(image):   # 加高斯噪声
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
            Suffix_name = ['.png', '.jpg', '.tif', '.jpeg']
            if filename.endswith(tuple(Suffix_name)):
                img_path = root_path+"/"+filename
                img_path_list.append(img_path)
    return  img_path_list


def worker_1(img_path_list,input_dir,output_dir,):
    for img_path in img_path_list:
        print("worker_1 doing",img_path)
        img_array = cv2.imread(img_path)
        Affine_transfor_img = Affine_transformation(img_array)
        cv2.imwrite(output_dir + "/" + img_path[len(input_dir):-4] + '_Affine_transfor.png',Affine_transfor_img)

        res_rotate = random_rotate_img(img_array)
        cv2.imwrite(output_dir + "/" + img_path[len(input_dir):-4] + '_rotate_img.png',res_rotate)

        GAMMA_IMG = random_gamma_transform(img_array, 0.3)
        cv2.imwrite(output_dir + "/" + img_path[len(input_dir):-4] + '_GAMMA_IMG.png', GAMMA_IMG)

def worker_2(img_path_list,input_dir,output_dir,):
    for img_path in img_path_list:
        print("worker_2 doing",img_path)
        img_array = cv2.imread(img_path)
        G_Noiseimg = gaussian_noise(img_array)
        cv2.imwrite(output_dir + "/" +img_path[len(input_dir):-4] + '_G_Noise_img.png',G_Noiseimg)

def worker_3(img_path_list,input_dir,output_dir,):
    for img_path in img_path_list:
        print("worker_3 doing",img_path)
        img_array = cv2.imread(img_path)
        res_flip = random_flip_img(img_array)
        cv2.imwrite(output_dir + "/" +img_path[len(input_dir):-4] + '_flip_img.png', res_flip)

        HSV_IMG = random_hsv_transform(img_array, 2, 0.3, 0.6)
        cv2.imwrite(output_dir + "/" +img_path[len(input_dir):-4] + '_HSV_IMG.png', HSV_IMG)


if __name__ == "__main__":
    input_dir = "./cccc"
    output_dir = "./eeee"
    start_time = time.time()  # 开始计时
    img_path_list = get_img(input_dir)

    p1 = multiprocessing.Process(target = worker_1, args = (img_path_list,input_dir,output_dir,))
    p2 = multiprocessing.Process(target = worker_2, args = (img_path_list,input_dir,output_dir,))
    p3 = multiprocessing.Process(target = worker_3, args = (img_path_list,input_dir,output_dir,))

    p1.start()
    p2.start()
    p3.start()

    p1.join()
    p2.join()
    p3.join()
    end_time = time.time()
    print("Total Spend time：", str((end_time - start_time) / 60)[0:6] + "分钟")
```

待续



