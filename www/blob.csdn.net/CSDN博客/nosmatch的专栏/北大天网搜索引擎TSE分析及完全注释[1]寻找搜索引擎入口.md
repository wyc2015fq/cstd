# 北大天网搜索引擎TSE分析及完全注释[1]寻找搜索引擎入口 - nosmatch的专栏 - CSDN博客
2012年02月24日 10:05:25[nosmatch](https://me.csdn.net/HDUTigerkin)阅读数：1194
本着黑客精神我将陆续把最近分析注释TSE搜索引擎的心得发布出来，老鸟，大虾，大牛，高手飘过就是了，若愿意浪费指点下小弟的在下不甚感激，有问题的朋友直接留言讨论。由于本人水平有限，分析和翻译难免有错大家见笑了。
上学期拜读了James F.Kurose著的《计算机网络-自顶向下方法与internet特色(第三版阴影)》，觉得写得确实不错(希望没看的朋友一定要买来看看)，自己也来搞个高自顶向下的学习方法，先从用户看得到的东西出发分析研究搜索引擎，下面我们就来看看各大搜索引擎搜索界面的代码，你所需要特别注意的是form表单中的action
雅虎[http://www.yohoo.com/](http://www.yohoo.com/)：
![](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/None.gif)<form name=s1 style="margin-bottom:0" action="http://search.yahoo.com/search">
![](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/None.gif)<table cellpadding=0 cellspacing=0 border=0><tr><td>
![](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/None.gif)<input type=text size=30 name=p title="enter search terms here">
![](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/None.gif)<input type=submit value=Search></td><td><font face=arial size=-2>·
![](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/None.gif)<a href="http://search.yahoo.com/search/options?p=">advanced search</a><br>·
![](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/None.gif)<a href="http://buzz.yahoo.com/">most popular</a></font></td></tr></table></form>
谷歌[http://www.g.cn](http://www.g.cn/)：
![](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/None.gif)<form method=GET action=/search><tr><td nowrap>
![](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/None.gif)<font size=-1><input type=text name=q size=41 maxlength=2048 value="jrckkyy" title="Google 搜索"><input type=submit name=btnG value="Google 搜索"><input type=hidden name=complete value=1><input type=hidden name=hl value="zh-CN"><input type=hidden name=newwindow value=1><input type=hidden name=sa value="2"></font></td></tr></form>
百度[http://www.baidu.com](http://www.baidu.com/)：
![](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/None.gif)<form name=f2 action="/s">
![](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/None.gif)<tr valign="middle">
![](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/None.gif)<td nowrap>
![](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/None.gif)<input type=hidden name=ct value="0">
![](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/None.gif)<input type=hidden name=ie value="gb2312">
![](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/None.gif)<input type=hidden name=bs value="jrckkyy">
![](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/None.gif)<input type=hidden name=sr>
![](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/None.gif)<input type=hidden name=z value="">
![](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/None.gif)<input type=hidden name=cl value=3>
![](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/None.gif)<input type=hidden name=f value=8>
![](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/None.gif)<input name=wd size="35" class=i value="jrckkyy" maxlength=100>
![](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/None.gif)<input type=submit value=百度一下><input type=button value=结果中找 onclick="return bq(f2,1,0);"></td>
![](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/None.gif)<td nowrap><a href="http://utility.baidu.com/quality/quality_form.php?word=jrckkyy">与百度对话</a></td>
![](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/None.gif)</tr>
![](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/None.gif)</form>
天网[http://www.tianwang.com/](http://www.tianwang.com/)：
![](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/None.gif)<form name=f action="/cgi-bin/tw" method=get>
![](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/None.gif)<td valign=center width=634 background=images/index_image_02.gif>
![](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/None.gif)<table height=46 cellspacing=0 cellpadding=0 width=600 align=right  border=0>
![](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/None.gif)<tbody>
![](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/None.gif)<tr>
![](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/None.gif)<td height=50>
![](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/None.gif)<table cellspacing=0 cellpadding=0 width=600 border=0>
![](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/None.gif)<tbody>
![](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/None.gif)<tr>
![](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/None.gif)<td width="524" height="30" valign="bottom">
![](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/None.gif)<div align="center"><input name="word" type="text" size="40" maxlength="255" onClick="this.focus();checkWord(this,1)" onblutesr='checkWord(this,0)' value='请输入资源名称'>
![](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/None.gif)<font color=#ffffff>
![](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/None.gif)<select onChange=reRange(this.selectedIndex) name=range>
<script language=javascript>
![](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/InBlock.gif)<!--
![](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/ExpandedSubBlockStart.gif)for(var i =0; i < rescode.length; i++) {
![](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/ExpandedSubBlockStart.gif)if(i ==0) {
![](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/InBlock.gif)                                   document.write('<option value="0" selected>'+ rescode[i][0] +'</option>');
![](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/ExpandedSubBlockStart.gif)                               }else{
![](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/InBlock.gif)                                   document.write('<option value="'+ i +'">'+ rescode[i][0] +'</option>');
![](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/ExpandedSubBlockEnd.gif)                               }
![](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/ExpandedSubBlockEnd.gif)                           }
![](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/InBlock.gif)                           document.f.range.selectedIndex =0;
![](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/InBlock.gif)-->
![](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/ExpandedBlockEnd.gif)</script>
![](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/None.gif)</select>
![](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/None.gif)</font>-<font color=#ffffff>
![](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/None.gif)<select name=cd>
<script language=javascript>
![](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/InBlock.gif)<!--
![](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/InBlock.gif)var ind = document.f.range.selectedIndex;
![](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/InBlock.gif)var len = (rescode[ind].length -1) /2;
![](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/InBlock.gif)var sel =0;
![](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/ExpandedSubBlockStart.gif)for(var i =0; i < len; i++) {
![](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/InBlock.gif)                               document.write('<option value="'+ rescode[ind][2*i+1] +'">'+ rescode[ind][2*i+2] +'</option>');
![](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/InBlock.gif)if(rescode[ind][2*i+1] ==0)
![](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/InBlock.gif)                                   sel = i;
![](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/ExpandedSubBlockEnd.gif)                           }
![](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/InBlock.gif)                           document.f.cd.selectedIndex = sel;
![](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/InBlock.gif)-->
![](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/ExpandedBlockEnd.gif)</script>
![](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/None.gif)</select>
![](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/None.gif)</font></div>
![](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/None.gif)</td>
![](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/None.gif)<td width="71" valign="bottom"><input id=submit2 type=image height=22 width=40 src="images/so2.gif" align=absMiddle name=submit></td>
![](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/None.gif)</tr>
![](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/None.gif)<tr>
![](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/None.gif)<td colspan=3 height=25 class=style16>
![](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/None.gif)<div align=center></div>
![](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/None.gif)</td>
![](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/None.gif)</tr>
![](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/None.gif)</tbody>
![](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/None.gif)</table>
![](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/None.gif)</td>
![](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/None.gif)</tr>
![](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/None.gif)</tbody>
![](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/None.gif)</table>
![](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/None.gif)</td>
![](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/None.gif)</form>
测试服务器TSE：
![](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/None.gif)<form method="get" action="/cgi-bin/index/TSESearch" name="tw">
![](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/None.gif)<td width="100%" height="25" align="center">
![](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/None.gif)<input type="text" name="word" size="55">
![](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/None.gif)<input type="submit" value=" 搜索" name="www">
![](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/None.gif)</td>
![](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/None.gif)<input type="hidden" name="cdtype" value="GB">
![](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/None.gif)</form>
由以上几个form的属性可以看出全部采用的是get方法，CGI做为处理程序，也就是C/C++，CGI全称是“公共网关界面”(Common Gateway Interface)，HTTP服务器与你的或其它机器上的程序进行“交谈”的一种工具，其程序须运行在网络服务器上。CGI逐渐被近几年来的PHP，JAVA，ASP，PERL，Python，Ruby等动态语言所取代。但是其在速度和运行效率上的优势是无法取代的。
以下是TSE CGI入口程序注释，其他搜索引擎的入口也应该类似
/**
![](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/InBlock.gif) * 程序翻译说明
![](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/InBlock.gif) * @Copyright (c) 2008, 研发部
![](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/InBlock.gif) * All rights reserved.
![](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/InBlock.gif) *
![](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/InBlock.gif) * @filesource  TSESearch.cpp
![](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/InBlock.gif) * @author  jrckkyy <jrckkyy@163.com>
![](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/InBlock.gif) *
![](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/InBlock.gif) * Let's start
![](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/InBlock.gif) *
![](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/ExpandedBlockEnd.gif)*/
![](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/None.gif)#include <stdio.h>
![](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/None.gif)#include <stdlib.h>
![](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/None.gif)#include <string.h>
![](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/None.gif)#include <sys/types.h>
![](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/None.gif)#include <sys/stat.h>
![](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/None.gif)#include <fcntl.h>
![](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/None.gif)#include <sys/time.h>
![](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/None.gif)#include <unistd.h>
![](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/None.gif)
![](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/None.gif)#include <iostream>
![](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/None.gif)#include <fstream>
![](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/None.gif)#include <list>
![](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/None.gif)
![](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/None.gif)#include "Comm.h"//包含2个索引和1个数据文件
![](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/None.gif)#include "Query.h"//包含数据查询处理头文件
![](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/None.gif)#include "Document.h"//html文档处理头文件
![](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/None.gif)#include "StrFun.h"//字符串处理头文件
![](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/None.gif)#include "ChSeg/Dict.h"//字元字典处理头文件
![](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/None.gif)#include "ChSeg/HzSeg.h"
![](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/None.gif)#include "DisplayRst.h"//返回查询结果页面头文件，返回结果分为头部，中部，底部
![](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/None.gif)
![](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/None.gif)usingnamespace std;
![](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/None.gif)
/*
![](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/InBlock.gif) * A inverted file(INF) includes a term-index file & a inverted-lists file.
![](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/InBlock.gif) * A inverted-lists consists of many bucks(posting lists).
![](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/InBlock.gif) * The term-index file is stored at vecTerm, and
![](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/InBlock.gif) * the inverted-lists is sored at mapBuckets.
![](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/ExpandedBlockEnd.gif)*/
![](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/None.gif)
/**
![](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/InBlock.gif) * 程序翻译说明
![](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/InBlock.gif) * 搜索程序入口前台关键字提交到该cgi程序 例如：./cgi-bin/index/TSESearch?word=123&start=1
![](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/InBlock.gif) * 倒排文件包括一个记录检索词文件和一个倒排列表文件。
![](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/InBlock.gif) * 倒排列表包含很多标志（提交名单）。
![](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/InBlock.gif) * 记录检索词文件使用vecTerm来排序，和倒排列表是用mapBuckets来排序。
![](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/InBlock.gif) *
![](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/InBlock.gif) * @access  public
![](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/InBlock.gif) * @param   int char 参数的汉字说明 用于接收前台get传递的参数
![](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/InBlock.gif) * @return  string 0
![](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/ExpandedBlockEnd.gif)*/
![](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/None.gif)int main(int argc, char* argv[])
{
![](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/InBlock.gif)struct timeval begin_tv, end_tv;
![](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/InBlock.gif)struct timezone tz;
![](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/InBlock.gif)
![](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/InBlock.gif)    CDict iDict;
![](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/InBlock.gif)    map<string, string> dictMap, mapBuckets;
![](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/InBlock.gif)    vector<DocIdx> vecDocIdx;    //Document。h
![](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/InBlock.gif)
![](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/InBlock.gif)    CQuery iQuery;
![](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/InBlock.gif)    iQuery.GetInputs();        //具体程序开始执行
![](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/InBlock.gif)// current query & result page number
![](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/InBlock.gif)    iQuery.SetQuery();
![](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/InBlock.gif)    iQuery.SetStart();
![](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/InBlock.gif)
![](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/InBlock.gif)// begin to search
![](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/InBlock.gif)//开始具体搜索程序
![](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/InBlock.gif)    gettimeofday(&begin_tv,&tz);    //开始计时获取程序运行时间差
![](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/InBlock.gif)
![](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/InBlock.gif)    iQuery.GetInvLists(mapBuckets);        //将所有字符集存入映射变量中    瓶颈所在
![](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/InBlock.gif)    iQuery.GetDocIdx(vecDocIdx);        //将倒排索引存入向量中        瓶颈所在
![](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/InBlock.gif)
![](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/InBlock.gif)    CHzSeg iHzSeg;        //include ChSeg/HzSeg.h
![](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/InBlock.gif)    iQuery.m_sSegQuery = iHzSeg.SegmentSentenceMM(iDict, iQuery.m_sQuery);    //将get到的查询变量分词分成 "我/        爱/        你们/    的/        格式"
![](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/InBlock.gif)
![](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/InBlock.gif)    vector<string> vecTerm;
![](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/InBlock.gif)    iQuery.ParseQuery(vecTerm);        //将以"/"划分开的关键字一一顺序放入一个向量容器中
![](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/InBlock.gif)
![](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/InBlock.gif)set<string> setRelevantRst; 
![](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/InBlock.gif)    iQuery.GetRelevantRst(vecTerm, mapBuckets, setRelevantRst); 
![](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/InBlock.gif)
![](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/InBlock.gif)    gettimeofday(&end_tv,&tz);
![](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/InBlock.gif)// search end
![](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/InBlock.gif)//搜索完毕
![](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/InBlock.gif)
![](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/InBlock.gif)//下面开始显示
![](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/InBlock.gif)    CDisplayRst iDisplayRst; 
![](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/InBlock.gif)    iDisplayRst.ShowTop(); 
![](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/InBlock.gif)
![](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/InBlock.gif)float used_msec = (end_tv.tv_sec-begin_tv.tv_sec)*1000
![](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/InBlock.gif)+((float)(end_tv.tv_usec-begin_tv.tv_usec))/(float)1000; 
![](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/InBlock.gif)
![](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/InBlock.gif)    iDisplayRst.ShowMiddle(iQuery.m_sQuery,used_msec, 
![](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/InBlock.gif)            setRelevantRst.size(), iQuery.m_iStart);
![](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/InBlock.gif)
![](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/InBlock.gif)    iDisplayRst.ShowBelow(vecTerm,setRelevantRst,vecDocIdx,iQuery.m_iStart); 
![](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/InBlock.gif)
![](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/InBlock.gif)return0;
![](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/InBlock.gif)
![](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/ExpandedBlockEnd.gif)}
[复制](#)[搜索](#)
[复制](#)[搜索](#)
