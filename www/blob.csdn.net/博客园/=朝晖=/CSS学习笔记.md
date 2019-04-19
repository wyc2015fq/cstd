# CSS学习笔记 - =朝晖= - 博客园
# [CSS学习笔记](https://www.cnblogs.com/dhcn/p/7116172.html)
07年的一篇blog，转到这个博客上
1、要想元素相对于父元素决对定位，则父元素必须定位设置成relative。
2、要想width属性跨浏览器可用，则需要设置该元素的float属性。
3、父元素设置float或添加clear元素以试float元素的父元素能够有正确的空间。 
4、a:link visited hover active,必须以这样的顺序定义style,且定义a的button效果时，推荐a:link, a:visited 一起定义。
5、定义line-height可使单行文本垂直居中。
6、display:block的子元素会使其inline的父元素也block
7、text-indext:大负值，一个隐藏文本又兼顾屏幕阅读器用户的方法。
8、无论是基于浮动的布局,还是基于空白变的布局,在HTML中都是先出现maincontent,在css中也是先布局maincontent.
9、CSS bug的常见解决方案：将position属性设置为relative/将dispaly属性设置为inline(在浮动元素上)或者设置宽度或高度等尺寸。
10、table-layout 属性设置为 fixed 加设置td的height(width可选)也是一种处理td内字符串过长的一种方法，详参:对于 table 来说，假如 table-layout 属性设置为 fixed ，则 td 对象支持带有默认值为 hidden 的 overflow 属性。如果设为 scroll 或者 auto ，那么超出 td 尺寸的内容将被剪切。如果设为 visible ，将导致额外的文本溢出到右边或左边（视 direction 属性设置而定）的单元格。
11、背景图像是一种重要的美化站点的css技术。
12、不要要顶层乱用align=center元素属性方式定位，其对子元素的影响还得专门建层消除。
13、CSS调试阶段一定要分文件分模块调试，以便定位冲突点。
14、IE8的form内不要乱用button元素，要用的话就有div做的伪button.

