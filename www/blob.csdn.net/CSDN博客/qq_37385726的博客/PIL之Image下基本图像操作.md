# PIL之Image下基本图像操作 - qq_37385726的博客 - CSDN博客





2018年08月18日 17:48:24[啧啧啧biubiu](https://me.csdn.net/qq_37385726)阅读数：347








# **目录**

[1.从文件夹中打开图片](#1.%E4%BB%8E%E6%96%87%E4%BB%B6%E5%A4%B9%E4%B8%AD%E6%89%93%E5%BC%80%E5%9B%BE%E7%89%87)

[2.显示图片尺寸](#2.%E6%98%BE%E7%A4%BA%E5%9B%BE%E7%89%87%E5%B0%BA%E5%AF%B8)

[3.放大或压缩图片](#3.%E6%94%BE%E5%A4%A7%E6%88%96%E5%8E%8B%E7%BC%A9%E5%9B%BE%E7%89%87)

# 1.从文件夹中打开图片

```python
#显示图片
img = Image.open("./Image/use_Crop.jpg")
img.show()
```

# 2.显示图片尺寸

```python
img = Image.open("./Image/use_Crop.jpg")
#显示图片尺寸
print(img.size)
```

# 3.放大或压缩图片

```python
img = Image.open("./Image/use_Crop.jpg")
print('size_initial: ',img.size)
img = img.resize((600,600))
print('compaction: ',img.size)
img = img.resize((1000,1000))
print('enlarge: ',img.size)
```

size_initial:  (1131, 600)

compaction:  (600, 600)

enlarge:  (1000, 1000)







