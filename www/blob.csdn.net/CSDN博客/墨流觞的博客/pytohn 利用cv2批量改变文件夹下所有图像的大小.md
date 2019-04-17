# pytohn 利用cv2批量改变文件夹下所有图像的大小 - 墨流觞的博客 - CSDN博客





2018年10月09日 19:39:31[墨氲](https://me.csdn.net/dss_dssssd)阅读数：165








```python
```python
import cv2
import os

# 修改之后的图片大小
size_ = (1920, 1080)
# directory为文件所在的目录， 
# 将会将修改后的图片保存在 directory下resized_image文件夹中

directory = r'C:\Users\steven\Pictures\picture'
image_to_save_directory = os.path.join(directory, 'resized_image')
# 新建文件夹
if not os.path.isdir(image_to_save_directory):
    os.mkdir(image_to_save_directory)
file_names = os.listdir(directory)
# print(file_names)


for i in file_names:
    file_path = os.path.join(directory, i)
    # print(file_path)
    img = cv2.imread(file_path)
    im2 = cv2.resize(img, size_, interpolation=cv2.INTER_CUBIC)

    saved_path = os.path.join(image_to_save_directory, i)
    cv2.imwrite(saved_path, im2)

print(' image  resized successfully')
```
```



