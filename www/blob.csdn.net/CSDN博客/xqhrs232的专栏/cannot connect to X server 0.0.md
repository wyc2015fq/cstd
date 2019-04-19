# cannot connect to X server :0.0 - xqhrs232的专栏 - CSDN博客
2016年10月09日 16:49:50[xqhrs232](https://me.csdn.net/xqhrs232)阅读数：5580
个人分类：[Linux/Ubuntu/Fedora](https://blog.csdn.net/xqhrs232/article/category/906931)
原文地址::[http://stackoverflow.com/questions/646930/cannot-connect-to-x-server-0-0-with-a-qt-application](http://stackoverflow.com/questions/646930/cannot-connect-to-x-server-0-0-with-a-qt-application)
相关文章
1、cannot connect to X server :0.0 
 ----[http://rrewwe.blog.163.com/blog/static/9946464200952595753555/](http://rrewwe.blog.163.com/blog/static/9946464200952595753555/)
2、[Cannot
 connect to X server :0.0 as superuser](http://unix.stackexchange.com/questions/111831/cannot-connect-to-x-server-0-0-as-superuser)----[http://unix.stackexchange.com/questions/111831/cannot-connect-to-x-server-0-0-as-superuser](http://unix.stackexchange.com/questions/111831/cannot-connect-to-x-server-0-0-as-superuser)
|||24down vote[favorite](http://stackoverflow.com/questions/646930/cannot-connect-to-x-server-0-0-with-a-qt-application#)11|Compiling on Fedora 10.I have just started my first qt GUI application. I used all the default settings.Its just a simple form. It builds OK without any errors. But when I try and run the application. I get the following message:```cpp```Starting /home/rob/projects/qt/test1/test1/test1...No protocol specifiedtest1: cannot connect to X server :0.0``````Thanks for any advice,|The general causes for this are as follows:- DISPLAY not set in the environment.Solution:```cpp```export DISPLAY=:0.0./myQtCmdHere``````( This one doesn't appear to be the one at fault though, as its saying which X display its trying to connect to. Also, its not always 0.0, but most of the time it is )- Non-Authorised User trying to run the X ApplicationSolution ( as X owning user, ie: yourself )```cpp`xhost +local:root   # where root is the local user you want to grant access to.````|24down vote[favorite](http://stackoverflow.com/questions/646930/cannot-connect-to-x-server-0-0-with-a-qt-application#)11|Compiling on Fedora 10.I have just started my first qt GUI application. I used all the default settings.Its just a simple form. It builds OK without any errors. But when I try and run the application. I get the following message:```cpp```Starting /home/rob/projects/qt/test1/test1/test1...No protocol specifiedtest1: cannot connect to X server :0.0``````Thanks for any advice,|
|----|----|----|----|
|24down vote[favorite](http://stackoverflow.com/questions/646930/cannot-connect-to-x-server-0-0-with-a-qt-application#)11|Compiling on Fedora 10.I have just started my first qt GUI application. I used all the default settings.Its just a simple form. It builds OK without any errors. But when I try and run the application. I get the following message:```cpp```Starting /home/rob/projects/qt/test1/test1/test1...No protocol specifiedtest1: cannot connect to X server :0.0``````Thanks for any advice,| | |
The general causes for this are as follows:
- 
DISPLAY not set in the environment.
Solution:
```cpp
```
export DISPLAY=:0.0
./myQtCmdHere
```
```
( This one doesn't appear to be the one at fault though, as its saying which X display its trying to connect to. Also, its not always 0.0, but most of the time it is )
- 
Non-Authorised User trying to run the X Application
Solution ( as X owning user, ie: yourself )
```cpp
`xhost +local:root   # where root is the local user you want to grant access to.`
```
|24down vote[favorite](http://stackoverflow.com/questions/646930/cannot-connect-to-x-server-0-0-with-a-qt-application#)11|Compiling on Fedora 10.I have just started my first qt GUI application. I used all the default settings.Its just a simple form. It builds OK without any errors. But when I try and run the application. I get the following message:```cpp```Starting /home/rob/projects/qt/test1/test1/test1...No protocol specifiedtest1: cannot connect to X server :0.0``````Thanks for any advice,|
