# python 文件操作—获取文件、重命名、复制、移动 - wsp_1138886114的博客 - CSDN博客





2018年12月16日 10:34:47[SongpingWang](https://me.csdn.net/wsp_1138886114)阅读数：392
所属专栏：[python基础及相关](https://blog.csdn.net/column/details/23328.html)












### 文章目录
- [一、获取文件](#_1)
- [二、批量重命名文件](#_37)
- [三、文件复制、移动](#_79)




### 一、获取文件

```python
import os 



-----------------------获取文件夹下所有需要的文件名-------------------------

def list_dictionary_codes(root_dir):
    paths_list = []
    for parent, dirNames, fileNames in os.walk(root_dir):
        for name in fileNames:
            ext = ['.h', '.m', '.xib', '.json', '.c', '.cpp', '.mm', '.md']
            if name.endswith(tuple(ext)):
                paths_list.append(os.path.join(parent, name))
    return paths_list


-----------------------获取文件夹下所有文件名-------------------------

def read_name_list(path):
    import os
    name_list = []
    for child_dir in os.listdir(path):
        name_list.append(path+"/"+child_dir)
    return name_list
    
name_lists = read_name_list("./images")
print (name_lists)

输出：
['./images/example_01.jpg', './images/example_02.jpg', 
'./images/example_03.jpg', './images/example_04.jpg']
```

### 二、批量重命名文件

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
                n = 6 - len(str(i))
                src = os.path.join(os.path.abspath(self.path), item)
                dst = os.path.join(os.path.abspath(self.path), str(0) * n + str(i) + '.jpg')
                try:
                    os.rename(src, dst)
                    print('converting %s--to-->%s' % (src, dst))
                    i = i + 1
                except:
                    continue
        print('total %d to rename & converted %d jpgs' % (total_num, i))

if __name__ == '__main__':
    path = "./cccc"        # 图片文件夹路径
    demo = BatchRename(path).rename()

输出：
converting D:\python_script\cccc\www (1).jpg --to--> D:\python_script\cccc\000001.jpg
converting D:\python_script\cccc\www (2).jpg --to--> D:\python_script\cccc\000003.jpg
converting D:\python_script\cccc\www (3).jpg --to--> D:\python_script\cccc\000004.jpg
converting D:\python_script\cccc\www (4).jpg --to--> D:\python_script\cccc\000005.jpg
converting D:\python_script\cccc\www (5).jpg --to--> D:\python_script\cccc\000006.jpg
total 10 to rename & converted 11 jpgs
```

### 三、文件复制、移动

```python
import os
import shutil

def list_dictionary_codes(root_dir):
    paths_list = []
    for parent, dirNames, fileNames in os.walk(root_dir):
        for name in fileNames:
            ext = ['.jpg', '.png', '.jpeg', '.json'] #需要移动文件的后缀名
            if name.endswith(tuple(ext)):
                paths_list.append(os.path.join(parent, name))
    return paths_list

def copy_move_file(root_dir,target_path):
    paths_list = list_dictionary_codes(root_dir)
    for file_path in paths_list:
        shutil.copy(file_path,target_path)
        print("正在复制文件：",file_path)
    print("done!")

if __name__ == '__main__':
    root_dir = "./dddd"                         # 源文件目录
    target_path = "./ffff"                      # 目标目录
    copy_move_file(root_dir, target_path)

以上为文件复制，若需要移动文件只需要：
将   shutil.copy(file_path,target_path)
改为 shutil.move(file_path,target_path)
```



