# Ubuntu 8.04 Install insight - sandro_zhang的专栏 - CSDN博客
2013年07月09日 17:11:18[sandro_zhang](https://me.csdn.net/sandro_zhang)阅读数：576
                
Ubuntu 8.04 Install insight
1,Download package from http://sourceware.org/insight/downloads.php
2,untar it to user folder,like /home/me/insight
3,cd /home/me/insight folder, run:
a,./configure 
b,make
c,make install
4,if everything's ok,just run "insight" command for start insight.
or run "insight ./foo" for debug your app.
5,You may meet follow errors, here give you some advises.
a,error: 
When you "make", the follow errors:
/root/tk8.4.16/unix/../generic/tk3d.c:1279: error: ‘TkBorder’ has no member named ‘resourceRefCount’
/root/tk8.4.16/unix/../generic/tk3d.c:1280: error: ‘Tk_FakeWin’ has no member named ‘display’
/root/tk8.4.16/unix/../generic/tk3d.c:1280: error: ‘Tk_FakeWin’ has no member named ‘screenNum’
/root/tk8.4.16/unix/../generic/tk3d.c:1280: error: ‘TkBorder’ has no member named ‘screen’
/root/tk8.4.16/unix/../generic/tk3d.c:1281: error: ‘Tk_FakeWin’ has no member named ‘atts’
/root/tk8.4.16/unix/../generic/tk3d.c:1281: error: ‘TkBorder’ has no member named ‘colormap’
/root/tk8.4.16/unix/../generic/tk3d.c:1301: error: ‘TkDisplay’ has no member named ‘borderTable’
/root/tk8.4.16/unix/../generic/tk3d.c:1301: error: ‘TkDisplay’ has no member named ‘borderTable’
/root/tk8.4.16/unix/../generic/tk3d.c:1306: error: ‘TkBorder’ has no member named ‘nextPtr’
resolve:
You need install this:
sudo apt-get install libx11-dev
b,error:
When you "make", the follow error:
error: no termcap library found
resolve:
You need install ncurses:
apt-get install libncurses5-dev
