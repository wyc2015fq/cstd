# PADS蛇形走线技巧V0[1].2 - xqhrs232的专栏 - CSDN博客
2015年08月06日 15:18:59[xqhrs232](https://me.csdn.net/xqhrs232)阅读数：1675
原文地址::[http://wenku.baidu.com/link?url=qdWZTqodOyYholhCAl_dD--GysJekA6ZWAEXdVFxv4iXps8tAe4RSs0pkQsxL2tR382V8HDPHConQXOZNZL7xOF6NJUtoZv2k7t7PouzIye](http://wenku.baidu.com/link?url=qdWZTqodOyYholhCAl_dD--GysJekA6ZWAEXdVFxv4iXps8tAe4RSs0pkQsxL2tR382V8HDPHConQXOZNZL7xOF6NJUtoZv2k7t7PouzIye)
相关文章
1、[转载]PADS常见问题全集----[http://blog.sina.com.cn/s/blog_92c081670101hevt.html](http://blog.sina.com.cn/s/blog_92c081670101hevt.html)

怎样使用PADS中本身自带的特性阻抗计算功能？
1、在setup/layer definition中把需要定义为地或电源层相应层定义为CAM PLANE。
2、并在layer thinkness中输入你的层迭的结构，比如各层的厚度、板材的介电常数等。
通过以上的设置，选定某一根网络并按CTRL+Q，就可以看到该网络相关的特性阻抗、延时等。
