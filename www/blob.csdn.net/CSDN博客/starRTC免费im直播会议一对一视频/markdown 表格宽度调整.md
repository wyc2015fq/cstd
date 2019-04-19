# markdown 表格宽度调整 - starRTC免费im直播会议一对一视频 - CSDN博客
2018年05月09日 19:48:37[starRTC免费IM直播会议一对一视频](https://me.csdn.net/elesos)阅读数：2637
先在浏览器中看看 HTML是怎样的：
<table><thead><tr><th>名称</th><th>值</th><th>备注</th></tr></thead><tbody><!-- 省略 tbody 内容 --></tbody></table>
利用浏览器上的开发者工具我们可以调试一下，把【名称】左边的 <th> 改为：
<thstyle="width: 100px;">
- 1
  看起来似乎不错。回到 **Markdown** 上，在原表格前添加 CSS 代码，类似这样：
<style>table th:first-of-type {    width: 100px;}</style><!-- 下方是表格的 Markdown 语法 --!>
首先 <th> 存在于 <table> 中；其次 th:first-of-type 的意思是每个 <th> 为其父级的第一个元素，这里指的就是围绕着【名称】的 <th>。同理第二、三个使用 th:nth-of-type(2)、th:nth-of-type(3) 就可以了，以此类推。 
  上述的 th:first-of-type 等于 th:nth-of-type(1)。
