# python之验证码生成 - weixin_33985507的博客 - CSDN博客
2019年01月01日 02:20:38[weixin_33985507](https://me.csdn.net/weixin_33985507)阅读数：1
今天向大家总结一下python在做项目时用到的验证码生成工具：**gvcode**与**captcha**
## gvcode
全称：graphic-verification-code
安装：
`pip install gvcode`
使用：
```
import gvcode
s, v = gvcode.generate()    #序列解包
s.show()    #显示生成的验证码图片
print(v)    #打印验证码字符串
```
效果：
![clipboard.png](https://image-static.segmentfault.com/196/041/1960410707-5c2b22fa9510c_articlex)
## captcha
安装：
`pip install captcha`
使用：
```
from captcha.image import ImageCaptcha
from random import randint
list = ['0', '1', '2', '3', '4', '5', '6', '7', '8', '9',
        'a', 'b', 'c', 'd', 'e', 'f', 'g', 'h', 'i', 'j', 'k', 'l', 'm', 'n', 'o', 'p', 'q', 'r', 's', 't', 'u', 'v', 'w', 'x', 'y', 'z',
        'A', 'B', 'C', 'D', 'E', 'F', 'G', 'H', 'I', 'J', 'K', 'L', 'M', 'N', 'O', 'P', 'Q', 'R', 'S', 'T', 'U', 'V', 'W', 'X', 'Y', 'Z']
chars = ''
for i in range(4):
    chars += list[randint(0, 62)]
image = ImageCaptcha().generate_image(chars)
image.show()
```
效果：
![clipboard.png](https://image-static.segmentfault.com/366/796/3667960977-5c2b22506ddd1_articlex)
