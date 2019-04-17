# Solve error: Cannot open include file: 'X11/Xlocale.h': No such file or directory - Grandyang - 博客园







# [Solve error: Cannot open include file: 'X11/Xlocale.h': No such file or directory](https://www.cnblogs.com/grandyang/p/5202588.html)







When you use FLTK with VS2010, you may get the error: fatal error C1083: Cannot open include file: 'X11/Xlocale.h': No such file or directory.



The way to solve this is quite simple, just add the following line before '#include <FL/Fl.H>'

```
#define WIN32
```














