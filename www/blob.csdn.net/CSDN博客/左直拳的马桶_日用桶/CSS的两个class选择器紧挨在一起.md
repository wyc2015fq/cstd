# CSS的两个class选择器紧挨在一起 - 左直拳的马桶_日用桶 - CSDN博客
2016年05月11日 20:07:15[左直拳](https://me.csdn.net/leftfist)阅读数：15555标签：[css																[class																[before																[active](https://so.csdn.net/so/search/s.do?q=active&t=blog)](https://so.csdn.net/so/search/s.do?q=before&t=blog)](https://so.csdn.net/so/search/s.do?q=class&t=blog)](https://so.csdn.net/so/search/s.do?q=css&t=blog)
个人分类：[Web](https://blog.csdn.net/leftfist/article/category/674421)
有一段HTML代码：
`<a class="glyphicons white no-js cogwheel" href="#" target="_blank"><i></i></a>`
对应有CSS style:
```
.glyphicons.white i:before {
  color: #fff;
}
.glyphicons.white:hover i:before {
  color: #b6ff00;
}
```
那么出现的效果是： 
一个图标，齿轮是白的 
![这里写图片描述](https://img-blog.csdn.net/20160511200027955)
当鼠标移到齿轮时，齿轮变黄绿： 
![这里写图片描述](https://img-blog.csdn.net/20160511200125441)
解释一下这段CSS代码给自己听
```
/*当有元素同时包含 "glyphicons" 和 "white" class，并且拥有子元素<i>的时候，在<i>前面设置颜色为白色的style*/
.glyphicons.white i:before {
  color: #fff;
}
/*当有元素同时包含 "glyphicons" 和 "white" class，并且拥有子元素<i>的时候，当鼠标悬停其上，则在<i>前面设置颜色为#b6ff00(黄绿)的style*/
.glyphicons.white:hover i:before {
  color: #b6ff00;
}
```
