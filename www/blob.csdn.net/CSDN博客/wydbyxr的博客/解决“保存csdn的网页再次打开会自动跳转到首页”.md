# 解决“保存csdn的网页再次打开会自动跳转到首页” - wydbyxr的博客 - CSDN博客
2018年11月30日 09:56:53[whitenightwu](https://me.csdn.net/wydbyxr)阅读数：907
右键用文本编辑器打开离线保存的网页，搜索关键词onerror，找到下面这段：
```
<img src="" onerror='setTimeout(function(){if(!/(csdn.net|iteye.com|baiducontent.com|googleusercontent.com|360webcache.com|sogoucdn.com|bingj.com|baidu.com)$/.test(window.location.hostname)){window.location.href="\x68\x74\x74\x70\x73\x3a\x2f\x2f\x77\x77\x77\x2e\x63\x73\x64\x6e\x2e\x6e\x65\x74"}},3000);'>
```
不要心慈手软，直接删除后保存即可。
