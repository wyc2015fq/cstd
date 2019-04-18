# Chrome浏览器使用js刷新页面后参数丢失。 - z69183787的专栏 - CSDN博客
2013年03月01日 13:05:28[OkidoGreen](https://me.csdn.net/z69183787)阅读数：2946
Chorme浏览器中使用诸如
window.location.reload() 或者 window.location.refresh() 都会丢失 原页面中 url 参数
目前先修改为 form.submit();
该问题暂时没有解决。。
