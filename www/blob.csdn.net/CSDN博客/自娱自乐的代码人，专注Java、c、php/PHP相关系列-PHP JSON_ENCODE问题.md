# PHP相关系列 - PHP JSON_ENCODE问题 - 自娱自乐的代码人，专注Java、c、php - CSDN博客





2012年08月01日 18:24:09[initphp](https://me.csdn.net/initphp)阅读数：621
个人分类：[PHP相关系列](https://blog.csdn.net/initphp/article/category/2609095)









今天在json_encode的时候，中文字是null

后来检查发现不是utf-8编码，改成utf-8编码后就OK了

记录一下！



