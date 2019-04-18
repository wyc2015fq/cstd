# 科学网—How to design other Trend Pages - 白图格吉扎布的博文




# How to design other Trend Pages                           

已有 816 次阅读2017-11-28 13:19|个人分类:[CLUB](http://blog.sciencenet.cn/home.php?mod=space&uid=333331&do=blog&classid=171794&view=me)|系统分类:[科研笔记](http://blog.sciencenet.cn/home.php?mod=space&do=blog&view=all&uid=333331&catid=1)



How to design other Trend Pages from DAILY

Next, I will explain how to use page Daily page to generate Weekly, Bi-Weekly, and Monthly Trends:

Copy and paste the page Daily, and name it as Weekly.

Go to Page DATA

From GZ, group5 (GV3:GZ32) columns as This week，

group second 5(GQ3:GU32) columns as last week，

group the third5 (GL3:GP32) columns as before week，



Go back to Page WKLY:

put =AVERAGE(DATA!GV3:DATA!GZ3) in (F3): and 

put=AVERAGE(DATA!GQ3:DATA!GU3) in (E3):

put=AVERAGE(DATA!GL3:DATA!GP3) in (D3):



Copy (D3:F3)and Paste to (D4:F32)

Check the Values to make shure: 

D3==AVERAGE(DATA!GL3:DATA!GP3)

E3=AVERAGE(DATA!GQ3:DATA!GU3)

F3=AVERAGE(DATA!GV3:DATA!GZ3)

And 

D2=SQRT(SUMSQ(WKLY!D3:D32))

E2=SQRT(SUMSQ(WKLY!E3:E32))

F2=SQRT(SUMSQ(WKLY!F3:F32))



G2=E2/D2

H2=F2/E2

G3=E3/D3/G$2

H3=F3/E3/H$2



I3=H3/G3

J3=H3*G3

The weekly page is done.



Repeat these, but group the DATA 10 columns, instead of 5, per Bi-Week from GZ

To generate a 2WKLY



Repeat, but group the DATA 20 columns, instead of 5, per MONTH from GZ

To generate a MONTHLY



Pages of TRENDS are done.





转载本文请联系原作者获取授权，同时请注明本文来自白图格吉扎布科学网博客。
链接地址：[http://blog.sciencenet.cn/blog-333331-1087260.html](http://blog.sciencenet.cn/blog-333331-1087260.html)

上一篇：[PAGE OF TRENDS](blog-333331-1086720.html)
下一篇：[How to design Page of COSINE](blog-333331-1087263.html)


