
# MTK平台Android4.4 拍照默认图片格式修改 - 嵌入式Linux - CSDN博客

2016年02月01日 13:57:35[写代码的篮球球痴](https://me.csdn.net/weiqifa0)阅读数：1263



## 因为摄像头效果要调试，需要把摄像头拍照的照片格式修改了
晚上看了一下资料，这个链接，这个链接比较有用
[http://www.cnblogs.com/peterzd/archive/2012/10/11/2695640.html](http://www.cnblogs.com/peterzd/archive/2012/10/11/2695640.html)
里面有一段话这样写：
**
Environment.getExternalStoragePublicDirectory(Environment.DIRECTORY_PICTURES) 这个方法返回一个标准的，分享的，并被推荐的目录，用来存放图片和Video。如果被用户卸载了，文件也会存在。为了防止与用户已存在的文件冲突，你应该再创建一个子目录用来存放自己应用的图片。如下面的例子。这个方法在API Level 8以上可以使用，更早的设备，可以查看其他方法。
Context.getExternalFilesDir(Environment.DIRECTORY_PICTURES) ，这个方法返回一个标准的用来存放你的应用的图片和Video的地方。如果应用被卸载，这里的文件也会被卸载。其他应用也可以操作这里的文件。
**
然后我就在我的app里面找getExternalStoragePublicDirectory 这个关键字
```python
packages/apps/Camera/src/
```
```python
com
```
```python
/android/camera/Storage
```
```python
.java
```
这里面就找到了修改保存文件后缀的代码，修改之
diff 如下：
```python
index eb9f46a.
```
```python
.620
```
```python
bdbf
```
```python
100755
```
```python
--- a/packages/apps/Camera/src/com/android/camera/Storage.java
+++ b/packages/apps/Camera/src/com/android/camera/Storage.java
@@ -
```
```python
474
```
```python
,
```
```python
7
```
```python
+
```
```python
474
```
```python
,
```
```python
7
```
```python
@@
```
```python
public
```
```python
class
```
```python
Storage {
         }
```
```python
else
```
```python
if
```
```python
(pictureType == PICTURE_TYPE_JPS) {
```
```python
return
```
```python
title +
```
```python
".jps"
```
```python
;
         }
```
```python
else
```
```python
if
```
```python
(pictureType == PICTURE_TYPE_JPG){
-
```
```python
return
```
```python
title +
```
```python
".jpg"
```
```python
;
+
```
```python
return
```
```python
title +
```
```python
".raw"
```
```python
;
         }
```
```python
else
```
```python
{
```
```python
// this case we just want return the
```
```python
// source data,such as intermedia data
```
```python
@@ -
```
```python
492
```
```python
,
```
```python
7
```
```python
+
```
```python
492
```
```python
,
```
```python
7
```
```python
@@
```
```python
public
```
```python
class
```
```python
Storage {
         }
```
```python
else
```
```python
if
```
```python
(pictureType == PICTURE_TYPE_JPS) {
```
```python
return
```
```python
"image/x-jps"
```
```python
;
         }
```
```python
else
```
```python
{
-
```
```python
return
```
```python
"image/jpeg"
```
```python
;
+
```
```python
return
```
```python
"image/raw"
```
```python
;
         }
     }
```

