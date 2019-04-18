# js无提示框，关闭当前窗口 - z69183787的专栏 - CSDN博客
2012年10月26日 16:37:01[OkidoGreen](https://me.csdn.net/z69183787)阅读数：907
个人分类：[Javascript-概述](https://blog.csdn.net/z69183787/article/category/2175033)
```java
<script>
window.opener = null;
window.open('','_self');
window.close();
</script>
```
