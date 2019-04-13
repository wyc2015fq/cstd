
# C 实现 删除字符串空白符的函数 strtrim - 嵌入式Linux - CSDN博客

2018年11月12日 16:05:04[写代码的篮球球痴](https://me.csdn.net/weiqifa0)阅读数：119



# 说在前面的话
字符串操作在很多C语言比赛里面都有涉及，最近公众号里面的C语言比赛，都是两个关于字符串操作的题目，希望大家认真看题目。
# 直接上代码
`/*************************************************************************
	> File Name: trim.c
	> Author: 
	> Mail: 
	> Created Time: Mon 12 Nov 2018 10:16:04 AM CST
 ************************************************************************/
#include<stdio.h>
/*****************************************************************************
 函 数 名  : strtrim
 功能描述  : 删除行首、行尾空白符号
 输入参数  : char *is  
 输出参数  : 无
 返 回 值  : char *
 调用函数  : 
 被调函数  : 
 
 修改历史      :
  1.日    期   : 2018年11月12日
    作    者   : strtrim
    修改内容   : 新生成函数
*****************************************************************************/
char * strtrim(char *s) {
    char *p = s;
    char *q = s;
    //去掉行首的空格
    while (*p==' ' || *p=='\t') 
        ++p;
    //赋值
    while ((p != NULL)&&(q != NULL)&&(*p != '\0')&&(*p != '\0'))
    {
        *q++ = *p++;
    };
	//删除'\0'字符 注意是 -2 上面q++是先操作再自加
    q -= 2;
    //去掉行末的空格
    while (*q==' ' || *q=='\t') 
        --q;
    //给字符串添加字符结束标志
    *(q+1) ='\0';
    //这里的return s要注意看好
    //因为p q经过一系列操作后，已经不是原来的位置，越界了 ，s还在原来位置，所以return s才是正确的。
    return s;
}
/*****************************************************************************
 函 数 名  : strtrimc
 功能描述  : 删除字符串空白符,包括行首和行尾
 输入参数  : char * s
 输出参数  : 无
 返 回 值  : char *
 调用函数  : 
 被调函数  : 
 
 修改历史      :
  1.日    期   : 2018年11月12日
    作    者   : strtrim
    修改内容   : 新生成函数
*****************************************************************************/
char * strtrimc( char * s )
{
    char * p1 = s;
	char * p2 = s;
	while(*p1 != '\0')
	{
		while(*p1 == ' ' || *p1 == '\t')
		{
			p1 ++;
		}
		* p2 ++ = *p1 ++;
	}
	*p2 = '\0';
	return (s);
}
void main(void)
{
	char str[1000] = {" adf asdfasldf jjjjjjjjj    iiii "};
	char str1[1000] = {" adf asdfasldf jjjjjjjjj    iiii "};
	puts("Start main.");
    printf("%s\n",strtrimc(str));
	printf("%s\n",strtrim(str1));
}`
# 注意问题
## 1 常量区
刚开始的时候，我写的是char * s = "adfasdfasdfsf adfasdf adfasdfad ";
然后出现段错误，这里定义的s，保存在的是常量区，所以不能做赋值等操作，大家对字符串操作的时候要特别注意。
## 2指针的返回值问题
![在这里插入图片描述](https://img-blog.csdnimg.cn/20181112155821777.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3dlaXFpZmEw,size_16,color_FFFFFF,t_70)
原来我返回的是p1 ,p1 p2 s原来是相等的，但是经过一系列运算后，他们的值已经不相等了，他们的值如上面显示那样，所以返回值一直要返回s。
[
            ](https://img-blog.csdnimg.cn/20181112155821777.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3dlaXFpZmEw,size_16,color_FFFFFF,t_70)

