# Resin4下JSP文件导出问题的解决 - =朝晖= - 博客园
# [Resin4下JSP文件导出问题的解决](https://www.cnblogs.com/dhcn/p/7105439.html)
       之前我在Resin3下采用JSP代码对一些硬盘上的文件作读取以后再输出或者生成一些特殊格式文件(如Excel)再输出供下载，这些文件输出JSP代码在Resin4以后输出的文件都产生错误无法正常读取，我估计可能是Resin对JSP的输出作了一个Servlet做了一定的特殊处理，我把生成文件输出代码转移到Servle中以后，代码效果其生成文件又正常了。

