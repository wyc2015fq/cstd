# OpenCV—python图像标注（批量文件重命名） - wsp_1138886114的博客 - CSDN博客





2018年09月26日 11:33:46[SongpingWang](https://me.csdn.net/wsp_1138886114)阅读数：262








```python
```python
"""
批量修改文件名：我这里用于对图片数据集标注
注意：输入目录与输出目录相同才行
"""
import os


input_dir = "../SandBox/label_pictures"
output_dir = "../SandBox/label_pictures"

if os.path.exists(output_dir):
    os.mkdir(output_dir)

for fname in os.listdir(input_dir):
    if "_0" in fname:
        print(str(fname))
    else:
        fname0 = str(fname).split(".")[0]
        fname_new = fname0+"_1"+".tif"
        print("修改后：",fname_new)
        # os.renames(文件路径/旧文件名，文件路径/新文件名)
        os.renames("../SandBox/label_pictures/"+fname,"../SandBox/label_pictures/"+ fname_new)
```
```



