# OpenCV—Python 对比度与亮度调整 - wsp_1138886114的博客 - CSDN博客





2018年09月17日 20:26:45[SongpingWang](https://me.csdn.net/wsp_1138886114)阅读数：814








### 对比度与亮度调整方法1

对图片对比度和亮度的调整一般通过下面公式计算：
$$g(x,y) = a*f(x,y)+b$$
- $f(x,y)$ 代表源图像 x行，y列的像素点的c通道的数值
- $g(x,y)$ 代表目标图像x行，y列的像素点的c通道的数值
- $a$ 参数（a>0）表示放大的倍数（一般在0.0~3.0之间）
- $b$ 参数一般称为偏置，用来调节亮度

```python
```python
import  cv2

img=cv2.imread('cat.jpg')
cv2.imshow('original_img',img)
rows,cols,channels=img.shape
dst=img.copy()

a=0.5
b=80
for i in range(rows):
    for j in range(cols):
        for c in range(3):
            color=img[i,j][c]*a+b
            if color>255:           # 防止像素值越界（0~255）
                dst[i,j][c]=255
            elif color<0:           # 防止像素值越界（0~255）
                dst[i,j][c]=0
                
cv2.imshow('dst',dst)
cv2.waitKey(0)
cv2.destroyAllWindows()
```
```

![在这里插入图片描述](https://img-blog.csdn.net/20180925202026183?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3dzcF8xMTM4ODg2MTE0/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)

### 对比度与亮度调整方法2

addWeighted函数：计算两个图像阵列的加权和

```python
```python
"""
函数详解：
addWeighted(InputArray_src1, 
            double_alpha, 
            InputArray_src2, 
            double_beta, 
            double_gamma, 
            OutputArray_dst, 
            int_dtype=-1
            );
"""

一共有七个参数：前4个是两张要合成的图片及它们所占比例，
                            第5个double gamma起微调作用，
                            第6个OutputArray dst是合成后的图片，
                            第7个输出的图片的类型（可选参数，默认-1）
```
```

有公式得出两个图片加成输出的图片为：**dst = src1 * alpha+src2 * beta + gamma**

```python
```python
def contrast_img(img1, c, b):  # 亮度就是每个像素所有通道都加上b
    rows, cols, channels = img1.shape

    # 新建全零(黑色)图片数组:np.zeros(img1.shape, dtype=uint8)
    blank = np.zeros([rows, cols, channels], img1.dtype)
    dst = cv2.addWeighted(img1, c, blank, 1-c, b)
    cv2.imshow('original_img', img)
    cv2.imshow("contrast_img", dst)

img = cv2.imread(r"./cat.png", cv2.IMREAD_COLOR)
contrast_img(img, 1.3, 3)
cv2.waitKey(0)
cv2.destroyAllWindows()
```
```

![在这里插入图片描述](https://img-blog.csdn.net/20180925203502449?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3dzcF8xMTM4ODg2MTE0/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)



