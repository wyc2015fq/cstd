# 用Python给朋友圈增添一些有意思的小东西 - zhusongziye的博客 - CSDN博客





2018年11月27日 20:32:06[zhusongziye](https://me.csdn.net/zhusongziye)阅读数：116标签：[用Python给朋友圈增添一些有意思的小东西																[微信九宫格](https://so.csdn.net/so/search/s.do?q=微信九宫格&t=blog)](https://so.csdn.net/so/search/s.do?q=用Python给朋友圈增添一些有意思的小东西&t=blog)
个人分类：[Python语言](https://blog.csdn.net/zhusongziye/article/category/7131144)








都知道发朋友圈有"九宫格"的说话，特别对于有着"强迫症"的小伙伴而言，一共集齐九张图，才能让自己心满意足.....但是有时候很尴尬，那么这次小编就教一招，用Python轻松解决这个问题，而且还能让你朋友圈B格提升一个档次！

首先咱们要准备好Python环境和需要的模块！

没有Python的朋友可以去百度下载安装一下！

模块主要是用到 PIL 那是不是直接pip install PIL 呢

![](https://img-blog.csdnimg.cn/20181124154250619.png)

很明显不可以，正确的安装姿势：

```
pip install pillow
```

咱们先看看效果图吧！

![](https://img-blog.csdnimg.cn/20181124154315698.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3podXNvbmd6aXll,size_16,color_FFFFFF,t_70)

是不是把一张完好的图拆分成九张图了呢？

那怎么看看 朋友圈里面是怎么样的

![](https://img-blog.csdnimg.cn/20181124154337532.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3podXNvbmd6aXll,size_16,color_FFFFFF,t_70)

是不是感觉档次瞬间就不一样了呢！！

原图是这样的

![](https://img-blog.csdnimg.cn/20181124154358320.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3podXNvbmd6aXll,size_16,color_FFFFFF,t_70)

代码

```
from PIL import Image  
import sys  
#先将 input image 填充为正方形  
def fill_image(image):  
    width, height = image.size      
    #选取长和宽中较大值作为新图片的  
    new_image_length = width if width > height else height      
    #生成新图片[白底]  
    new_image = Image.new(image.mode, (new_image_length, new_image_length), color='white')   #注意这个函数！  
    #将之前的图粘贴在新图上，居中   
    if width > height:#原图宽大于高，则填充图片的竖直维度  #(x,y)二元组表示粘贴上图相对下图的起始位置,是个坐标点。  
        new_image.paste(image, (0, int((new_image_length - height) / 2)))  
    else:  
        new_image.paste(image, (int((new_image_length - width) / 2),0))      
    return new_image  
def cut_image(image):
    width, height = image.size
    item_width = int(width / 3)  #因为朋友圈一行放3张图。
    box_list = []
    # (left, upper, right, lower)
    for i in range(0,3):
        for j in range(0,3):
            #print((i*item_width,j*item_width,(i+1)*item_width,(j+1)*item_width))
            box = (j*item_width,i*item_width,(j+1)*item_width,(i+1)*item_width)
            box_list.append(box)
    image_list = [image.crop(box) for box in box_list]
    return image_list
#保存  
def save_images(image_list):  
    index = 1   
    for image in image_list:  
        image.save(str(index) + '.png', 'PNG')  
        index += 1  
if __name__ == '__main__':  
    file_path = "4.jpg"  
    image = Image.open(file_path)     
    #image.show()  
    image = fill_image(image)  
    image_list = cut_image(image)  
    save_images(image_list)
```




