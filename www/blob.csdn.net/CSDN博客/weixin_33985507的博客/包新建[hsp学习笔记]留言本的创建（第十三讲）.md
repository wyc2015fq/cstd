# 包新建[hsp学习笔记]留言本的创建（第十三讲） - weixin_33985507的博客 - CSDN博客
2013年05月31日 21:24:00[weixin_33985507](https://me.csdn.net/weixin_33985507)阅读数：3
最近使用开发的过程中出现了一个小问题，顺便记录一下原因和方法--包新建
    hsp 留言本的创立
    1.新建各种包
    2.建任务
    create table users2(
    id number primary key,
    name varchar2(64) unique,
    passwd varchar2(128) not null
    )
    create table message(
    id number primary key,
    每日一道理 
微笑，是春天里的一丝新绿，是秋日里的一缕阳光，是骄阳下的一片浓荫，是冬雪中的一株梅红……微笑着去面对吧，你会感到人生是那样的温馨与甜蜜！
    sender varchar2(64) not null,
    getter varchar2(64) not null,
    content varchar2(4000) not null,
    sendTime date default sysdate,
    attachment varchar2(128)
    )
文章结束给大家分享下程序员的一些笑话语录： 
关于编程语言
    如果 C++是一把锤子的话，那么编程就会变成大手指头。
    如果你找了一百万只猴子来敲打一百万个键盘，那么会有一只猴子会敲出一 段 Java 程序，而其余的只会敲出 Perl 程序。
    一阵急促的敲门声，“谁啊！”，过了 5 分钟，门外传来“Java”。
    如果说 Java 很不错是因为它可以运行在所有的操作系统上，那么就可以说 肛交很不错，因为其可以使用于所有的性别上。
--------------------------------- 
           原创文章 By            
包和新建
--------------------------------- 
