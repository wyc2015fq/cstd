# 母板页中的事件Page_Load居然不执行 - 左直拳的马桶_日用桶 - CSDN博客
2011年09月27日 16:22:00[左直拳](https://me.csdn.net/leftfist)阅读数：1410标签：[javascript																[c#](https://so.csdn.net/so/search/s.do?q=c#&t=blog)](https://so.csdn.net/so/search/s.do?q=javascript&t=blog)
个人分类：[.NET](https://blog.csdn.net/leftfist/article/category/94497)
母板页的开头声明中，有这么个属性：<%@ Master Language="C#" MasterPageFile="~/master/base.master" AutoEventWireup="true" CodeFile="show.master.cs" Inherits="master_show" %>
本来默认是true，不知道是做了什么，有一个变成了false。好了，发觉该母板页中的事件Page_Load居然能不执行。改回“true”就可以鸟。
这个属性，大概类似javascript中的事件向上提升之类的东西吧。
