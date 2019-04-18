# Flex security问题：ActionScript Error #2148 - weixin_33985507的博客 - CSDN博客
2011年08月01日 11:01:00[weixin_33985507](https://me.csdn.net/weixin_33985507)阅读数：5
转自[http://blog.csdn.net/wolfcyl/article/details/6049232](http://blog.csdn.net/wolfcyl/article/details/6049232)
当用Flex的HttpServie控件请求xml数据时，会莫名其妙的错误。如果有打印出错误，会显示 Error #2148。
没有的的话页面没有显示。
只需要更改编译器的安全参数设置  -use-network=false   即可。
具体操作如下：
在FlexBuilder下就是 Project->Properties->Flex Compiler->Additional Compiler Arguments
即 项目 - > 属性 - > Flex编译器 -> 附加的编译器参数
![](http://hi.csdn.net/attachment/201012/2/0_1291252959Acu1.gif)
