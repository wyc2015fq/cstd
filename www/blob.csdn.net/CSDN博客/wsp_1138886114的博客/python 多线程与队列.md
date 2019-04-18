# python 多线程与队列 - wsp_1138886114的博客 - CSDN博客





2019年01月17日 11:27:41[SongpingWang](https://me.csdn.net/wsp_1138886114)阅读数：102











### 文章目录
- [Queue队列](#Queue_2)
- [队列-单线程](#_23)
- [队列-多线程](#_102)
- [队列-多线程—图像增强实例](#_234)
- [多线程-创建图像缩略图（等比缩放图像）](#_416)



各位好，之前写了[多线程](https://blog.csdn.net/wsp_1138886114/article/details/84336206)，但是在实际的生产中，往往情况比较复杂，要处理一批任务（比如要处理列表中所有元素），这时候不可能创建很多的线程，线程过多反而不好，还会造成资源开销太大，这时候想到了队列。


### Queue队列

Queue用于建立和操作队列，常和threading类一起用来建立一个简单的线程队列。

Queue.Queue(maxsize)　　FIFO（先进先出队列）

Queue.LifoQueue(maxsize)　　LIFO（先进后出队列）

Queue.PriorityQueue(maxsize)　　为优先级越高的越先出来,对于一个队列中的所有元素组成的entries，优先队列优先返回的一个元素是sorted(list(entries))[0]。至于对于一般的数据，优先队列取什么东西作为优先度要素进行判断，官方文档给出的建议是一个tuple如(priority, data)，取priority作为优先度。

如果设置的maxsize小于1，则表示队列的长度无限长- **FIFO是常用的队列**，常用的方法有：

**Queue.qsize()**　　 返回队列大小
**Queue.empty()**　　判断队列是否为空
**Queue.full()**　　    判断队列是否满了
**Queue.get([block[,timeout]])**　　从队列头删除并返回一个item，block默认为True，表示当队列为空却去get的时候会阻塞线程，等待直到有有item出现为止来get出这个item。如果是False的话表明当队列为空你却去get的时候，会引发异常。

在block为True的情况下可以再设置timeout参数。表示当队列为空，get阻塞timeout指定的秒数之后还没有get到的话就引发Full异常。

**Queue.put(…[,block[,timeout]])**　　向队尾插入一个item，同样若block=True的话队列满时就阻塞等待有空位出来再put，block=False时引发异常。

同get的timeout，put的timeout是在block为True的时候进行超时设置的参数。
**Queue.task_done()**　　从场景上来说，处理完一个get出来的item之后，调用task_done将向队列发出一个信号，表示本任务已经完成。
**Queue.join()**　　监视所有item并阻塞主线程，直到所有item都调用了task_done之后主线程才继续向下执行。这么做的好处在于，假如一个线程开始处理最后一个任务，它从任务队列中拿走最后一个任务，此时任务队列就空了但最后那个线程还没处理完。当调用了join之后，主线程就不会因为队列空了而擅自结束，而是等待最后那个线程处理完成了。

### 队列-单线程

```python
import threading
import queue
import time


class worker(threading.Thread):
    def __init__(self, queue):
        threading.Thread.__init__(self)
        self.queue = queue
        self.thread_stop = False

    def run(self):
        while not self.thread_stop:
            print("thread%d %s: waiting for tast" % (self.ident, self.name))
            try:
                task = q.get(block=True, timeout=2)  # 接收消息
            except queue.Empty:
                print("Nothing to do! I will go home!")
                self.thread_stop = True
                break
            print("tasking: %s ,task No:%d" % (task[0], task[1]))
            print("I am working")
            time.sleep(3)
            print("work finished!")
            q.task_done()                           # 完成一个任务
            res = q.qsize()                         # 判断消息队列大小(队列中还有几个任务)
            if res > 0:
                print("fuck! Still %d tasks to do" % (res))

    def stop(self):
        self.thread_stop = True


if __name__ == "__main__":
    q = queue.Queue(3)                                    # 创建队列（大小为3）
    worker = worker(q)                                    # 将队列加入类中
    worker.start()                                        # 启动类
    q.put(["produce cup!", 1], block=True, timeout=None)  # 向队列中添加元素，产生任务消息
    q.put(["produce desk!", 2], block=True, timeout=None)
    q.put(["produce apple!", 3], block=True, timeout=None)
    q.put(["produce banana!", 4], block=True, timeout=None)
    q.put(["produce bag!", 5], block=True, timeout=None)
    print("***************leader:wait for finish!")
    q.join()                                             # 等待所有任务完成
    print("***************leader:all task finished!")


输出：
thread9212 Thread-1: waiting for tast
tasking: produce cup! ,task No:1
I am working
work finished!
fuck! Still 3 tasks to do
thread9212 Thread-1: waiting for tast
tasking: produce desk! ,task No:2
I am working
***************leader:wait for finish!
work finished!
fuck! Still 3 tasks to do
thread9212 Thread-1: waiting for tast
tasking: produce apple! ,task No:3
I am working
work finished!
fuck! Still 2 tasks to do
thread9212 Thread-1: waiting for tast
tasking: produce banana! ,task No:4
I am working
work finished!
fuck! Still 1 tasks to do
thread9212 Thread-1: waiting for tast
tasking: produce bag! ,task No:5
I am working
work finished!
thread9212 Thread-1: waiting for tast
***************leader:all task finished!
Nothing to do!i will go home!
```

### 队列-多线程

```python
import threading
import time
from queue import Queue

img_lists = ['lipei_00006.mp3','lipei_00007.mp3','lipei_00012.mp3','lipei_00014.mp3',
             'lipei_00021.mp3','lipei_00027.mp3','lipei_00028.mp3','lipei_00035.mp3',
             'lipei_00039.mp3','lipei_00044.mp3','lipei_00047.mp3','lipei_00049.mp3',
             'lipei_00057.mp3','lipei_00058.mp3','lipei_00059.mp3','lipei_00061.mp3',
             'lipei_00066.mp3','lipei_00068.mp3','lipei_00070.mp3','lipei_00081.mp3',
             'lipei_00087.mp3','lipei_00104.mp3','lipei_00106.mp3','lipei_00117.mp3',
             'lipei_00123.mp3','lipei_00129.mp3',]

q = Queue(10)

class Music_Cols(threading.Thread):
    def __init__(self, name):
        super().__init__(name=name)

    def run(self):
        global img_lists
        global q
        while True:
            try:
                music = img_lists.pop(0)
                q.put(music)
            except IndexError:
                break

class Music_Play(threading.Thread):
    def __init__(self, name):
        super().__init__(name=name)

    def run(self):
        global q
        while True:
            if q.not_empty:
                music = q.get()
                print('{}正在播放{}'.format(threading.current_thread(), music))
                time.sleep(5)
                q.task_done()
                print('{}播放结束'.format(music))
            else:
                break


if __name__ == '__main__':
    mc_thread = Music_Cols('music_cols')
    mc_thread.setDaemon(True)       # 设置为守护进程，主线程退出时，子进程也kill掉
    mc_thread.start()               # 启动进程
    for i in range(5):              # 设置线程个数（批量任务时，线程数不必太大，注意内存及CPU负载）
        mp_thread = Music_Play('music_play')
        mp_thread.setDaemon(True)
        mp_thread.start()
    q.join()                        # 线程阻塞（等待所有子线程处理完成，再退出）

输出：
<Music_Play(music_play, started daemon 1068)>正在播放lipei_00006.mp3
<Music_Play(music_play, started daemon 1072)>正在播放lipei_00007.mp3
<Music_Play(music_play, started daemon 4920)>正在播放lipei_00012.mp3
<Music_Play(music_play, started daemon 3880)>正在播放lipei_00014.mp3
<Music_Play(music_play, started daemon 5400)>正在播放lipei_00021.mp3
lipei_00014.mp3播放结束
... ...
<Music_Play(music_play, started daemon 1068)>正在播放lipei_00117.mp3
lipei_00066.mp3播放结束
<Music_Play(music_play, started daemon 1072)>正在播放lipei_00123.mp3
lipei_00104.mp3播放结束
<Music_Play(music_play, started daemon 4920)>正在播放lipei_00129.mp3
lipei_00123.mp3播放结束
lipei_00117.mp3播放结束
lipei_00087.mp3播放结束
lipei_00106.mp3播放结束
lipei_00129.mp3播放结束
```

或者(效果与上述一样)

```python
import threading
import time
from queue import Queue


img_lists = ['lipei_00006.mp3','lipei_00007.mp3','lipei_00012.mp3','lipei_00014.mp3',
             'lipei_00021.mp3','lipei_00027.mp3','lipei_00028.mp3','lipei_00035.mp3',
             'lipei_00039.mp3','lipei_00044.mp3','lipei_00047.mp3','lipei_00049.mp3',
             'lipei_00057.mp3','lipei_00058.mp3','lipei_00059.mp3','lipei_00061.mp3',
             'lipei_00066.mp3','lipei_00068.mp3','lipei_00070.mp3','lipei_00081.mp3',
             'lipei_00087.mp3','lipei_00104.mp3','lipei_00106.mp3','lipei_00117.mp3',
             'lipei_00123.mp3','lipei_00129.mp3',]

q = Queue(10)


class Music_Cols(threading.Thread):
    def __init__(self, name):
        super().__init__(name=name)

    def run(self):
        while True:
            try:
                music = img_lists.pop(0)
                q.put(music)
            except IndexError:
                break

class Music_Play(threading.Thread):
    def __init__(self, name):
        super().__init__(name=name)

    def run(self):
        while True:
            if q.not_empty:
                music = q.get()
                print('{}正在播放{}'.format(threading.current_thread(), music))
                time.sleep(5)
                q.task_done()
                print('{}播放结束'.format(music))
            else:
                break


if __name__ == '__main__':
    mc_thread = Music_Cols('music_cols')
    mc_thread.setDaemon(True)       # 设置为守护进程，主线程退出时，子进程也kill掉
    mc_thread.start()               # 启动进程
    for i in range(5):              # 设置线程个数（批量任务时，线程数不必太大，注意内存及CPU负载）
        mp_thread = Music_Play('music_play')
        mp_thread.setDaemon(True)
        mp_thread.start()
    q.join()                        # 线程阻塞（等待所有子线程处理完成，再退出）
```

### 队列-多线程—图像增强实例

```python
"""
开启多线程：图像增强
"""
import os
import random
import queue
import numpy as np
import cv2
import time
import threading

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
            Suffix_name = ['.png', '.jpg', '.tif', '.jpeg']
            if filename.endswith(tuple(Suffix_name)):
                img_path = root_path+"/"+filename
                img_path_list.append(img_path)
    return  img_path_list


class IMG_QUEUE(threading.Thread):
    def __init__(self, name):
        super().__init__(name=name)

    def run(self):
        while True:
            try:
                img_path = img_path_list.pop(0)
                q.put(img_path)
            except IndexError:
                break

class IMG_AUG(threading.Thread):
    def __init__(self, name):
        super().__init__(name=name)
        self.q = q

    def run(self):
        while True:
            if q.not_empty:
                img_path = q.get()
                try:
                    print("doing...")
                    img_array = cv2.imread(img_path)
                    Affine_transfor_img = Affine_transformation(img_array)
                    cv2.imwrite(output_dir + "/" + img_path[len(input_dir):-4] + '_Affine_transfor.png', Affine_transfor_img)

                    res_rotate = random_rotate_img(img_array)
                    cv2.imwrite(output_dir + "/" + img_path[len(input_dir):-4] + '_rotate_img.png',res_rotate)

                    GAMMA_IMG = random_gamma_transform(img_array, 0.3)
                    cv2.imwrite(output_dir + "/" + img_path[len(input_dir):-4] + '_GAMMA_IMG.png',GAMMA_IMG)

                    res_flip = random_flip_img(img_array)
                    cv2.imwrite(output_dir + "/" + img_path[len(input_dir):-4] + '_flip_img.png',res_flip)

                    G_Noiseimg = gaussian_noise(img_array)
                    cv2.imwrite(output_dir + "/" + img_path[len(input_dir):-4] + '_G_Noise_img.png',G_Noiseimg)

                    HSV_IMG = random_hsv_transform(img_array, 2, 0.3, 0.6)
                    cv2.imwrite(output_dir + "/" + img_path[len(input_dir):-4] + '_HSV_IMG.png',HSV_IMG)
                except:
                    print("图像格式错误！")
                    pass
                q.task_done()
            else:
                break


if __name__ == '__main__':
    input_dir = './cccc'
    output_dir = './eeee'
    start_time = time.time()            # 开始计时
    img_path_list = get_img(input_dir)  # 获取图像数据

    q = queue.Queue(10)                 # 设置队列元素个数
    my_thread = IMG_QUEUE('IMG_QUEUE')  # 实例化
    my_thread.setDaemon(True)           # 设置为守护进程，主线程退出时，子进程也kill掉
    my_thread.start()                   # 启动进程

    for i in range(5):                  # 设置线程个数（批量任务时，线程数不必太大，注意内存及CPU负载）
        mp_thread = IMG_AUG('IMG_AUG')
        mp_thread.setDaemon(True)
        mp_thread.start()
    q.join()                            # 线程阻塞（等待所有子线程处理完成，再退出）
    end_time = time.time()
    print("Total Spend time：", str((end_time - start_time) / 60)[0:6] + "分钟")
```

### 多线程-创建图像缩略图（等比缩放图像）

```python
import os
from PIL import Image
import threading
import time
import queue


def get_img(input_dir):
    img_path_list = []
    for (root_path,dirname,filenames) in os.walk(input_dir):
        for filename in filenames:
            Suffix_name = ['.png', '.jpg', '.tif', '.jpeg']
            if filename.endswith(tuple(Suffix_name)):
                img_path = root_path+"/"+filename
                img_path_list.append(img_path)
    return  img_path_list

class IMG_QUEUE(threading.Thread):
    def __init__(self, name):
        super().__init__(name=name)

    def run(self):
        while True:
            try:
                img_path = img_path_list.pop(0)
                q.put(img_path)
            except IndexError:
                break

class IMG_RESIZE(threading.Thread):
    def __init__(self, name):
        super().__init__(name=name)

    def run(self):
        while True:
            if q.not_empty:
                img_path = q.get()
                try:
                    im = Image.open(img_path)
                    im.thumbnail((size, size))
                    print(im.format, im.size, im.mode)
                    im.save(img_path, 'JPEG')
                except:
                    print("图像格式错误！")
                    pass
                q.task_done()
            else:
                break



if __name__=='__main__':
    input_dir = 'D:\\20190112_20190114_all' #需要创建缩略图，图片的目录
    start_time = time.time()            # 开始计时
    img_path_list = get_img(input_dir)  # 获取图像数据

    size = 800
    q = queue.Queue(100)                # 设置队列元素个数
    my_thread = IMG_QUEUE('IMG_QUEUE')  # 实例化
    my_thread.setDaemon(True)           # 设置为守护进程，主线程退出时，子进程也kill掉
    my_thread.start()                   # 启动进程

    for i in range(5):                  # 设置线程个数（批量任务时，线程数不必太大，注意内存及CPU负载）
        mp_thread = IMG_RESIZE(str(i))
        mp_thread.setDaemon(True)
        mp_thread.start()
    q.join()                            # 线程阻塞（等待所有子线程处理完成，再退出）
    end_time = time.time()              # 计时结束
    print("Total Spend time：", str((end_time - start_time) / 60)[0:6] + "分钟")
```






