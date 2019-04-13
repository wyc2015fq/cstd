
# Servlet 工程 web.xml 中的 servlet 和 servlet-mapping 标签 - 高科的专栏 - CSDN博客

2017年10月25日 14:39:08[高科](https://me.csdn.net/pbymw8iwm)阅读数：562标签：[serverlet																](https://so.csdn.net/so/search/s.do?q=serverlet&t=blog)[java																](https://so.csdn.net/so/search/s.do?q=java&t=blog)[
							](https://so.csdn.net/so/search/s.do?q=serverlet&t=blog)个人分类：[java																](https://blog.csdn.net/pbymw8iwm/article/category/7248124)[serverlet																](https://blog.csdn.net/pbymw8iwm/article/category/7248125)[
							](https://blog.csdn.net/pbymw8iwm/article/category/7248124)


摘录某个工程的 web.xml 文件片段：
![](https://img-blog.csdn.net/20130811104901000)

访问顺序为1—>2—>3—>4，其中2和3的值必须相同。
url-pattern 标签中的值是要在浏览器地址栏中输入的 url，可以自己命名，这个 url 访问名为 servlet-name 中值的 servlet，两个 servlet-name 标签的值必须相同，因为通过 servlet 标签中的 servlet-name 标签映射到 servlet-class 标签中的值，最终访问 servlet-class 标签中的 class。
还有，web.xml 的 / 表示的是http://localhost:8080/ + 工程名
而.jsp页面的action，加了/表示绝对路径http://localhost:8080/ ，没有加 / 表示的是相对路径

