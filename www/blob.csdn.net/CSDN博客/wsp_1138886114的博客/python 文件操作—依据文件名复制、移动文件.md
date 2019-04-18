# python 文件操作—依据文件名复制、移动文件 - wsp_1138886114的博客 - CSDN博客





2018年11月13日 15:18:29[SongpingWang](https://me.csdn.net/wsp_1138886114)阅读数：201








### 通过 excel 获取数据集

数据集放在一个总文件夹中，excel中对图像标识做了记录，我们需要通过读取excel中的图像文件名来获取相应的图像。

实例文件结构如下：

```
源目录
 ├── 二级目录1
 │   ├──example_01.jpg
 │   └──example_02.jpg
 ├── 二级目录2
 │   ├──example_03.jpg
 │   ├──example_04.jpg
 │   └──example_05.jpg
目标目录
```

excel 文件：img_list.xlsx
![在这里插入图片描述](https://img-blog.csdnimg.cn/20181113151420416.PNG?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3dzcF8xMTM4ODg2MTE0,size_16,color_FFFFFF,t_70)
```python
# -*- coding: utf-8 -*-
import xlrd
import os
import shutil

def read_excel(excel_path):
    workbook = xlrd.open_workbook(excel_path)
    sheet = workbook.sheet_by_index(0)
    nrows = sheet.nrows
    list1 = []
    for i in range(nrows):
        list0 = str(sheet.row_values(i)[0])
        list1.append(list0[-14:])
    return list1


def get_img(original_path):
    img_list = []
    for root_dir,dir_name,filenames in os.walk(original_path):
        for filename in filenames:
            img_list.append(root_dir+"/"+filename)
    return img_list


def copy_img_move(original_path,Target_path,excel_path):
    list1 = read_excel(excel_path)
    img_list = get_img(original_path)
    for img in img_list:
        print("img",img)
        if img[len(original_path)+1:] in list1:
            shutil.copy(img,Target_path)
    print("处理完成！")


if __name__ == '__main__':
    original_path = "D:/图片/图片"
    Target_path = "D:/图片/有问题的图片"
    excel_path = "D:/图片/img_list.xlsx"
    copy_img_move(original_path,Target_path,excel_path)
```

其它

```python
# -*- coding: utf-8 -*-
import xlrd
import os
import shutil

def read_excel(excel_path):
    workbook = xlrd.open_workbook(excel_path)
    sheet = workbook.sheet_by_name("3210")
    nrows = sheet.nrows
    list1 = []
    for i in range(nrows):
        list1.append(sheet.row_values(i)[0][-25:]) #获取第1列数据后25个字符
    print("list1",list1)
    return list1


def file_and_forder(original_path):
    folder_filename_list = []
    for root_dir,dir_name,filenames in os.walk(original_path):
        for filename in filenames:
            if filename.endswith(".jpg"):
                folder_filename_list.append(root_dir+"/"+filename)
    print("copy folder_filename_list", folder_filename_list[-25:])
    return folder_filename_list


def copy_img_move(original_path,Target_path,excel_path):
    list1 = read_excel(excel_path)
    folder_filename_list = file_and_forder(original_path)
    for filename_single in folder_filename_list:
        if filename_single[-31:] in list1:
            print("copy filename", filename_single[-25:])
            shutil.copy(filename_single,Target_path)
        else:
            print("image not exist")
    print("处理完成！")


if __name__ == '__main__':
    original_path = "D:/20181228图片/图片"
    Target_path = "D:/20181228图片/有问题的图片/3210"
    excel_path = "D:/20181130图片/select.xlsx"
    copy_img_move(original_path,Target_path,excel_path)
```

### 通过 json 获取数据集

```python
import json
import shutil
import os
from glob import glob
from tqdm import tqdm

# 该json文件中种类，先生成59个文件夹，用于放置图像文件
try:
    for i in range(0,59):
        os.mkdir("./data/train/" + str(i))
except:
    pass
    
file_train = json.load(open("./data/labels/train_annotations.json","r",encoding="utf-8"))
file_val = json.load(open("./data/labels/validation_annotations.json","r",encoding="utf-8"))

file_list = file_train + file_val

for file in tqdm(file_list):
    filename = file["image_id"]
    origin_path = "./data/images/" + filename
    ids = file["disease_class"]
    if ids ==  44:
        continue
    if ids == 45:
        continue
    if ids > 45:
        ids = ids -2
    save_path = "./data/train/" + str(ids) + "/"
    shutil.copy(origin_path,save_path)
```

### 2 通过 json 获取数据集

```python
import os
import shutil
import json


def get_img(Img_path):
    img_paths = []
    for root_dir, dir_name, filenames in os.walk(Img_path):
        for filename in filenames:
            img_paths.append(root_dir+"/"+filename)
    return img_paths

def get_json(Json_file):
    json_paths = []
    for root_dir, dir_name, filenames in os.walk(Json_file):
        for filename in filenames:
            json_paths.append(root_dir+"/"+filename)
    return json_paths

def move_or_copy_file(Img_path,Json_file,target_dir):
    error_code = []
    error_msg = []
    types = []
    img_paths = get_img(Img_path)
    json_paths = get_json(Json_file)
    for json_file in json_paths:
        with open(json_file,"rb") as file:
            strings = file.read()
            dict1 = json.loads(strings.decode("utf-8"))
            error_code.append(dict1["error_code"])
            error_msg.append(dict1["error_msg"])
            types.append(dict1["type"])
        Target_dir = target_dir+"/"+dict1["type"]
        if os.path.exists(Target_dir):
            shutil.copy(Img_path+"/"+json_file[43:-4]+".jpg",Target_dir)
        else:
            os.mkdir(Target_dir)
            shutil.copy(Img_path + "/" + json_file[43:-4] + ".jpg", Target_dir)

if __name__ == '__main__':
    Img_path = "D:/img"
    Json_file = "D:/json"
    target_dir = "D:/class_img"
    move_or_copy_file(Img_path, Json_file, target_dir)
```




