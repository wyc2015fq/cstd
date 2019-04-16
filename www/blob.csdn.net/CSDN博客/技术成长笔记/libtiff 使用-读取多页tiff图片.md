# libtiff 使用-读取多页tiff图片 - 技术成长笔记 - CSDN博客





2017年03月26日 08:29:52[zhixuhao](https://me.csdn.net/u012931582)阅读数：5071








libtiff 官网地址:  [http://www.libtiff.org/](http://www.libtiff.org/)

安装python版 libtiff 可以直接采取pip安装:

```
pip install libtiff
```

读取和写入单页tiff图片:

```
>>> from libtiff import TIFF
>>> # to open a tiff file for reading:
>>> tif = TIFF.open('filename.tif', mode='r')
>>> # to read an image in the currect TIFF directory and return it as numpy array:
>>> image = tif.read_image()
>>> # to read all images in a TIFF file:
>>> for image in tif.iter_images(): # do stuff with image
>>> # to open a tiff file for writing:
>>> tif = TIFF.open('filename.tif', mode='w')
>>> # to write a image to tiff file
>>> tif.write_image(image)
```

读取多页tiff并且拆分成多个单页tiff图片:

```
>>>from libtiff import TIFF3D,TIFF
>>>imgdir = TIFF3D.open("filename.tif")
>>>imgarr = imgdir.read_image()
>>>for i in range(imgarr.shape[0]):
    imgname = t + "/" + str(i) + ".tif"
    img = TIFF.open(imgname,'w')
    img.write_image(imgarr[i])
```

libtiff 的文档实在是太少,甚至没有源码的注释详细,所以如果想要了解更多的使用方法,需要阅读源码. 

查看源码的位置:

```
>>>pip show -f libtiff
```

对于我来说,安装位置在/home/zhixuhao/.local/lib/python2.7/site-packages 

在该目录下找到libtiff文件夹,里面就是源码, 主要的函数和类都在libtiff_ctypes.py 里面



