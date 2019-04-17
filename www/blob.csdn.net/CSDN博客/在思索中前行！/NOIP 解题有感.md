# NOIP 解题有感 - 在思索中前行！ - CSDN博客





2014年10月09日 16:46:16[_Tham](https://me.csdn.net/txl16211)阅读数：641标签：[编程经验																[经验总结](https://so.csdn.net/so/search/s.do?q=经验总结&t=blog)](https://so.csdn.net/so/search/s.do?q=编程经验&t=blog)
个人分类：[NOI 竞赛辅导](https://blog.csdn.net/txl16211/article/category/1746713)








算法方面：




在搜索问题上，包括贪心等没有固定算法的题目，还有输出格式（包括输入格式）特别容易出错。这也是解题选手的弱点。




1.做搜索题把步骤先用文字写下来，再转换成代码，以避免敲代码时疏漏某个条件。




2.贪心先把思路想全面在动键盘。




2、输入输出及数据处理经验




平时注意总结输入输出的问题。

比如对于多组数据while(scanf()!=EOF)可以判断是否文末，而while(cin)可以直接判断。

以特殊值收尾的情况，如果是两个零不要侥幸只判一个。等等。




尽量避免使用gets()，当输入的数据中既含有数字，又包含字符串的时候，gets()会吃掉回车造成意想不到的错误。




注意系统环境，在linux下64位为longlong，lld% ；在windows平台则是__int64, I64d%。




