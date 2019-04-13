
# 批处理-通过mono把c\#编译成dll - AYE89 - 博客园|[AYE89](https://www.cnblogs.com/eniac1946/)
|coding & learning|
|posts - 153, comments - 3, trackbacks - 0, articles - 2

|
|导航
|[博客园](https://www.cnblogs.com/)|[首页](https://www.cnblogs.com/eniac1946/)|[新随笔](https://i.cnblogs.com/EditPosts.aspx?opt=1)|[联系](https://msg.cnblogs.com/send/AYE89)![订阅](//www.cnblogs.com/images/xml.gif)|[订阅](https://www.cnblogs.com/eniac1946/rss)|[管理](https://i.cnblogs.com/)
|公告


|[批处理-通过mono把c\#编译成dll](https://www.cnblogs.com/eniac1946/p/7245137.html)
|Posted on|2017-07-27 15:32|[AYE89](https://www.cnblogs.com/eniac1946/)|阅读(|...|) 评论(|...|)|[编辑](https://i.cnblogs.com/EditPosts.aspx?postid=7245137)|[收藏](#)
|::copyright@cjy
@echo off
::mcs.exe address
set addrMcs=D:\Program Files\Unity\Editor\Data\MonoBleedingEdge\lib\mono\4.5
::UnityEngine.dll address
set addrUEdll=D:\UnityEngine.dll
::source c\# file address
set addrscript=D:\unity_proj\motion1\Assets\Scripts\
set FileName=TestLib.dll

cd /d %addrMcs%
echo %cd%
:begin
echo Begin C\# ==> DLL...
mcs -r:%addrUEdll% -target:library %addrscript%TestLib.cs
copy %addrscript%%FileName% D:\%FileName%
@pause
goto begin







|[刷新评论](javascript:void(0);)|[刷新页面](#)|[返回顶部](#top)






|
|Powered by:
|博客园
|Copyright © AYE89
|
