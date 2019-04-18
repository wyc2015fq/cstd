# 调用百度AI开放平台实现图片文字识别 - wsp_1138886114的博客 - CSDN博客





2018年10月17日 11:21:30[SongpingWang](https://me.csdn.net/wsp_1138886114)阅读数：479








闲话少说：

注册百度账号，获取APP_ID，API_KEY，SECRECT_KEY

查看SDK，下面是我调用识别图片

安装baidu-api
```python
import os
import csv

from aip import AipOcr

APP_ID = '1180xxxx'
API_KEY = 'cChcRf45eLIRSQTqxxxxxxxx'
SECRECT_KEY = '12xG8GVcqS6I8doYwcSyGaCexxxxxxxx'
client = AipOcr(APP_ID, API_KEY, SECRECT_KEY)

input_dir = "./cut_picture"
list1 = []
for (path, dirnames, filenames) in os.walk(input_dir):
    list1.append(filenames)


def get_file_content(filePath):
    with open(filePath, 'rb') as fp:
        return fp.read()


Result = []
for j in list1[0]:
    path1 = input_dir + "/" + j
    print("药品图片文字：", path1)
    image = get_file_content(path1)

    """ 调用文字识别 """
    result = client.basicAccurate(image)
    print("识别输出：", result)
    Result.append(result)

    """ 如果有可选参数 """
    options = {}
    options["detect_direction"] = "true"
    options["probability"] = "true"

    """ 带参数调用通用文字识别（高精度版） """
    client.basicAccurate(image, options)
    print(result)

with open("./药品图片文字识别结果.csv", "w") as csvfile:
    write = csv.writer(csvfile)
    write.writerow(Result)
```

2

```python
import os
import csv
import json

from aip import AipOcr


APP_ID = '1180xxxx'
API_KEY = 'cChcRf45eLIRSQTqvsFUxxxx'
SECRECT_KEY = '12xG8GVcqS6I8doYwcSyGaCenLqKxxxx'
client = AipOcr(APP_ID, API_KEY, SECRECT_KEY)

def get_img(input_dir):            #获取所有图片
    img_path = []
    for (path, dirnames, filenames) in os.walk(input_dir):
        for filename in filenames:
            img_path.append(path+"/"+filename)
    return img_path

def get_file_content(filePath):   #读取图片
    with open(filePath, 'rb') as fp:
        return fp.read()


def main(input_dir):
    data_all = []
    img_path = get_img(input_dir)
    for filePath in img_path:
        image = get_file_content(filePath)
        """ 调用通用文字识别（高精度版） """
        result = client.basicAccurate(image)
        data_all.append(result)
        print("result:", result)
    return data_all

if __name__ == "__main__":
    data_all = main("./img")
    with open("./识别结果1.csv", "w") as csvfile:
        write = csv.writer(csvfile)
        write.writerow(data_all)
```




