# LaTeX超链接hyperref - weixin_33985507的博客 - CSDN博客
2018年03月20日 14:09:00[weixin_33985507](https://me.csdn.net/weixin_33985507)阅读数：27
## package
调用package：
```
\usepackage{hyperref}
\hypersetup{
    colorlinks=true,
    linkcolor=blue,
    filecolor=blue,      
    urlcolor=blue,
    citecolor=cyan,
}
```
注意：颜色可以选取其他颜色，比如：blue、yellow、green等。
## 引用
两种引用方法：
```
\url{http://www.xiaoledeng.cn}
```
或者
```
\href{http://www.xiaoledeng.cn}{Xiao-Le Deng的博客}
```
如果是邮箱的话，加入mailto命令即可。
```
\href{mailto:xiaoledeng@gmail.com}{xiaoledeng@gmail.com}
```
