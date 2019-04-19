# CSS伪类:before 和 :after - 左直拳的马桶_日用桶 - CSDN博客
2017年04月14日 16:20:15[左直拳](https://me.csdn.net/leftfist)阅读数：7457
CSS用了许久，对一些伪类熟视无睹，从不想着去搞清楚一下。比如说这个
```
:before
:after
```
其实，`:before` 表示该标记前面的样式，反之 `:after` 代表设置后面的样式。网页上常常看到有些文字前或后有小图标，用的就是这种样式： 
![这里写图片描述](https://img-blog.csdn.net/20170414154446255?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvbGVmdGZpc3Q=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
对应的代码有：
```xml
<button type="button" class="btn text" onclick="Search();"><i class="icon-search"></i> 查询</button>
<button type="button" onclick="clearSerach()" class="btn"><i class="icon-trash"></i> 清除</button>
```
图标正是`<i>`这里设置出来的。众所周知，`<i>` 只是一个代表斜体的标记而已，之所以用在这里，只是看中它不占地方，方便设置`:before :after` 而已。
那究竟如何设置图标呢？关键在于 `content` 这个CSS属性。 `content` 代表插入内容，并且似乎常跟`:before :after` 配对使用。像上面这个例子，就可以是
```
.icon-search:before{
    content: "\e000";/* 放大镜符号 */
    font-size: 16px;
}
```
有关这个content属性，还可以对应许多内容，具体可参考 
[http://www.zhangxinxu.com/wordpress/2010/04/css-content%E5%86%85%E5%AE%B9%E7%94%9F%E6%88%90%E6%8A%80%E6%9C%AF%E4%BB%A5%E5%8F%8A%E5%BA%94%E7%94%A8/](http://www.zhangxinxu.com/wordpress/2010/04/css-content%E5%86%85%E5%AE%B9%E7%94%9F%E6%88%90%E6%8A%80%E6%9C%AF%E4%BB%A5%E5%8F%8A%E5%BA%94%E7%94%A8/)
如果想将这些插入内容去掉怎么办呢？比如说，伸缩搜索框，按钮未点击前有“查找”二字，点击后字消失： 
![这里写图片描述](https://img-blog.csdn.net/20170414161657395?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvbGVmdGZpc3Q=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
![这里写图片描述](https://img-blog.csdn.net/20170414161708652?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvbGVmdGZpc3Q=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
代码就可以这样写：
```
.sb-icon-search:before {
    content: "\e000";
}
.sb-icon-search:after {
    content: "\20查\20找";/* \20 代表空格 */
    color: #555;
}
.sb-search.sb-search-open .sb-icon-search:after,
.no-js .sb-search .sb-icon-search:after {
    content: "";
}
```
将 content 设为”“，就啥都没有了。
[源代码下载](http://download.csdn.net/detail/leftfist/9814434)
