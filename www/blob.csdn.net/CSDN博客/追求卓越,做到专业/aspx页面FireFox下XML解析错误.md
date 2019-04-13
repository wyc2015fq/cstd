
# aspx页面FireFox下XML解析错误 - 追求卓越,做到专业 - CSDN博客


2013年10月21日 16:39:58[Waldenz](https://me.csdn.net/enter89)阅读数：636个人分类：[ASP.NET																](https://blog.csdn.net/enter89/article/category/1235183)


此问题在IE,Chrome下都没有，但是在FireFox的低版本中会出现。如果页面上有下载文件功能，上次下载完成后，没有使用Response.End关闭，就会导致下次不论点击打开任何页面，都会出现这个问题。

