# 教你如何使用css隐藏input的光标 - weixin_33985507的博客 - CSDN博客
2017年12月07日 00:28:32[weixin_33985507](https://me.csdn.net/weixin_33985507)阅读数：1
> 
今天公司的ui突然跑过来问我一个问题：“如何在不影响操作的情况下，把input的光标隐藏了？”。
我相信很多人会跟我一样，觉得这是个什么狗屁需求，输入框不要光标这不是反人类吗？可惜我们只是个小小的撸码仔，没有反驳的权利只能默默接受...
在网上搜索了很多方式：用div模拟，设置readonly，设置disabled，设置自动blur等等方式，发现都不能满足需求，最后还是找到一位大神提供的方法完美解决。
### 首先隐藏光标
```
<style>
    input{
      color: transparent;
    }
  </style>
```
因为光标是跟随文字的，所以我们把文字的color设置为透明，光标就不见了耶~
但是问题来了，文字都透明了要输入框有啥用？别着急，请往下看~
### 把文字给显示出来
```
<style>
    input{
      color: transparent;
      text-shadow: 0 0 0 #000;
    }
  </style>
```
在input上设置text-shadow，文字是透明的但是我们可以用文字阴影来代替文字的颜色，这样就完美解决啦。
![image.png](http://upload-images.jianshu.io/upload_images/4416278-ac1026441eee0e7f.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/1240)
