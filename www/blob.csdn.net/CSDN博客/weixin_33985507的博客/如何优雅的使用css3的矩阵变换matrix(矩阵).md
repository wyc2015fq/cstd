# 如何优雅的使用css3的矩阵变换matrix(矩阵) - weixin_33985507的博客 - CSDN博客
2017年08月16日 09:07:08[weixin_33985507](https://me.csdn.net/weixin_33985507)阅读数：1
在写文章的开始首先致谢张鑫旭大神的一篇文章：《理解CSS3 transform中的Matrix(矩阵)》，因为这篇文章给了我很大的启发，虽然文章写得足够清晰明了**但是对于我这个不懂矩阵的人还是很难使用上matrix 这个高大上的玩意儿！****如果你不懂矩阵也没关系**！
## 一、概念
matrix其实是可以代替：偏移量（translate）,缩放（scale），斜切（skew），旋转（rotate），四大功能的，任意一个matrix样式改变而来的形状也都能通过以上四个功能实现，它们是互通的。
## 二、理解
假定matrix的六个参数用字母表示如下：transform: matrix(a,b,c,d,e,f);
- 
e和f 代表着偏移量translate，x和y轴
- 
a和d 代表着缩放比例scale,x 和y轴
- 
b和c 代表着斜切skew（具体参数和角度关系为, b-->tanθ y轴    c-->tanθ x轴 ,我们具体操作的时候还是要使用小数的）
- 
abcd 中的ad代表缩放(scale),bc代表者斜切(skew); abcd四个参数代表着旋转，这你可能难以理解，请继续往下看。
> 
旋转是可以理解为, **旋转=规律的缩放+规律的斜切** 这里的意思是旋转可以用缩放和斜切一起用来得到, 两者联系在于这个角度θ。具体如下：
matrix(cosθ,sinθ,-sinθ,cosθ,0,0)
![图片描述](https://image-static.segmentfault.com/761/476/761476533-5993bad8db518_articlex)
然后你就知道了，为啥6个数能做到，translate，scale，skew，rotate了，因为一个对应两个参数， 原来最后一个rotate 被缩放和斜切给替代了(看到这里建议去看看实例中，如何用矩阵实现旋转的，还有斜切和缩放如何实现旋转。)
![图片描述](https://image-static.segmentfault.com/234/900/2349006618-5993bb33c6788_articlex)
```
<!DOCTYPE html PUBLIC "-//W3C//DTD XHTML 1.0 Transitional//EN" "http://www.w3.org/TR/xhtml1/DTD/xhtml1-transitional.dtd">
<html xmlns="http://www.w3.org/1999/xhtml">
<head>
<meta http-equiv="Content-Type" content="text/html; charset=utf-8" />
<title>理解矩阵变换</title>
<style type="text/css">
*{margin: 0; padding: 0;}
.box{ background:green; width: 200px; height: 200px; margin: 50px auto;}
/* 理解旋转是如何使用前四个参数的 */
.b1{transform: rotate(30deg);}
/* 矩阵具体写法 */
.b2{ transform: matrix(0.86603,0.5,-0.5,0.86603,1,1);}
/* 用斜切和变形来模拟,同时这里的参数是对应矩阵里的参数 ，斜切中的30deg的意思是,矩阵中的 sin30*  和 -sin30* 这两个斜切参数*/
.b3{ transform:skew(-30deg,30deg) scale(0.86603,0.86603);}
</style>
</head>
<body>
    <div class="box b1"></div>
    <div class="box b2"></div>
    <div class="box b3"></div>
    <div class="box"></div>
</body>
</html>
```
## 三、rotate占用了四个参数，skew跟scale怎么办
Matrix( a , b , c , d , e , f );    
**直接先旋转 算出abcd的值，再算出skew对应的bc，scale中的ad，然后加一起就可以实现矩阵实现多个变换了，别忘了正负符号**
最后一个ef就是偏移量，单独处理即可！这样大家书写Matrix的六个参数就非常的简单的了。
Matrix(a1+a2 , b1+b2 , c1+c2 , d , f);
