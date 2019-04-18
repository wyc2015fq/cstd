# 网页布局，关于外层div不能随内层div自动伸缩的问题 - z69183787的专栏 - CSDN博客
2014年02月08日 09:41:43[OkidoGreen](https://me.csdn.net/z69183787)阅读数：1577

        之前经常是这样布局的，用一个div（id=Main）包住左边div（id=DivLeft）和右边div（id=DivRight）,Main随着DivLeft和DivRight内容的高度而自动伸缩，从而达到动态布局的效果。
#Main｛
             width：980px
             height：auto;
             margin:0 auto;//使Main居中布局
              ｝
#DivLeft｛
                    width:250px;
                    height:auto;
                    float:left;
                ｝
#DivRight｛
                    width：730px;
                    height:auto;
                    float:right;
                  ｝
可是这样的效果达到了左边一个div右边一个div的目的，可以Main这个父div并没有将他们包含，致使网页凌乱。
后来在网上找到一种解决方法，是这样的，我也不太明白具体的含义。反正达到效果：
![](https://img-my.csdn.net/uploads/201208/10/1344560109_6829.jpg)
<!DOCTYPE html PUBLIC "-//W3C//DTD XHTML 1.0 Transitional//EN" "http://www.w3.org/TR/xhtml1/DTD/xhtml1-transitional.dtd">
这段不能去掉，因为可能网页要遵循这种规范。
还有一个问题，就是比如左边div（DivLeft）当它的内容越来越多的时候，内容并不完全能自动在div里自动换行，所以有些文字会超出div的边缘，这样也会造成网页的错乱。
解决方法是在这个div上应用这两个样式：word-wrap:break-word;  overflow:hidden;
经过实践，上面的做法只在火狐和谷歌浏览器可用。有时候在IE中不能。
解决方法：在父div也应用float：left，或者在添加一个空的div，应用clear：left
主要是这个样式：
**[css]**[view
 plain](http://blog.csdn.net/hougelou/article/details/7849883#)[copy](http://blog.csdn.net/hougelou/article/details/7849883#)
- .box:after  
- {  
- content: ".";  
- display: block;  
- height: 0px;  
- clear: both;  
- visibility: hidden;  
- }  
有时候在IE6、7下面还是不行，那么可以在父容器中，加入overflow:hidden;
