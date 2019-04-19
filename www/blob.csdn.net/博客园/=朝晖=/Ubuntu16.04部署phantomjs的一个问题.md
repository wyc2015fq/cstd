# Ubuntu16.04部署phantomjs的一个问题 - =朝晖= - 博客园
# [Ubuntu16.04部署phantomjs的一个问题](https://www.cnblogs.com/dhcn/p/7743714.html)
      首先phantomjs是作为pyspider的一个外部依赖组件部署的。
      apt安装完出现问题：
```
QXcbConnection: Could not connect to display 
PhantomJS has crashed. Please read the bug reporting guide at
<http://phantomjs.org/bug-reporting.html> and file a bug report.
Aborted
```
      在/usr/bin/phantomjs的合适位置加上以下设置代码：
```
export QT_QPA_PLATFORM=offscreen
export QT_QPA_FONTDIR=/usr/share/fonts
```
     phantomjs即可正常运行。
参考资料：https://bugs.debian.org/cgi-bin/bugreport.cgi?bug=817277

