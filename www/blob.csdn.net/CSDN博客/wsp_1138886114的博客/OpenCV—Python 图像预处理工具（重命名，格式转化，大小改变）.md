# OpenCV—Python 图像预处理工具（重命名，格式转化，大小改变） - wsp_1138886114的博客 - CSDN博客





2019年03月08日 11:06:23[SongpingWang](https://me.csdn.net/wsp_1138886114)阅读数：101











### 文章目录
- [图像重命名](#_1)
- [图像格式转化](#_35)
- [图像大小改变(resize)](#resize_58)
- [图像大小改变(多线程)](#_78)




##### 图像重命名

```python
import os


class BatchRename():
    """
    批量重命名文件夹中的图片文件
    """
    def __init__(self,path):
        self.path = path

    def rename(self):
        filelist = os.listdir(self.path)
        total_num = len(filelist)
        i = 1
        for item in filelist:
            Suffix_name = ['.png', '.jpg', '.jpeg', '.tif']
            if item.endswith(tuple(Suffix_name)):
                n = 5 - len(str(i))
                src = os.path.join(os.path.abspath(self.path), item)
                dst = os.path.join(os.path.abspath(self.path), "horse_"+str(0) * n + str(i) + '.jpg')
                try:
                    os.rename(src, dst)
                    print('converting %s --to--> %s ' % (src, dst))
                    i = i + 1
                except:
                    continue
        print('total %d to rename & converted %d jpgs' % (total_num, i))

if __name__ == '__main__':
    path = "D:/\image_horse"        # 图片文件夹路径
    demo = BatchRename(path).rename()
```

##### 图像格式转化

```python
import os
from PIL import Image

input_dir = "D:\\检测分类所有标注数据汇总\\800X800_class\\医疗票据_800X800\\体检报告"
output_dir = "D:\\检测分类所有标注数据汇总\\800X800_class\\医疗票据_800X800\\体检报告_jpg"
def image2jpg(input_dir,output_dir):
    no_pass = []
    for item in os.listdir(input_dir):
        try:
            file_path = os.path.join(input_dir,item)
            out_path = os.path.join(output_dir,item[:-4]+'.jpg')
            print(out_path)
            Image.open(file_path).save(out_path)
        except:
            no_pass.append(item)
            continue
    return no_pass

no_pass = image2jpg(input_dir,output_dir)
print("无法识别的图片共{}张".format(len(no_pass)),no_pass)
```

##### 图像大小改变(resize)

```python
import os
import glob
from PIL import Image

def thumbnail_pic(path,size):
    a=glob.glob(path+r'\\*.jpg')
    for x in a:
        name=os.path.join(path,x)
        im=Image.open(name)
        im.thumbnail((size,size))
        print(im.format,im.size,im.mode)
        im.save(name,'JPEG')
    print('Done!')

if __name__=='__main__':
    path=r'D:\\20190116_800X800_img\\50_juanlian'
    thumbnail_pic(path,size=800)
```

##### 图像大小改变(多线程)

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
    input_dir = 'D:\\800X800_class\\票据_800X800\\img_jpg'
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



