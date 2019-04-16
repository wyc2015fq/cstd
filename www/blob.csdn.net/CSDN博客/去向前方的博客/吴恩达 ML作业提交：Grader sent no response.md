# 吴恩达 ML作业提交：Grader sent no response - 去向前方的博客 - CSDN博客





2019年03月31日 14:12:07[Big_quant](https://me.csdn.net/lvsehaiyang1993)阅读数：80标签：[机器学习																[问题解决](https://so.csdn.net/so/search/s.do?q=问题解决&t=blog)](https://so.csdn.net/so/search/s.do?q=机器学习&t=blog)
个人分类：[程序基础																[深度学习](https://blog.csdn.net/lvsehaiyang1993/article/category/7659754)](https://blog.csdn.net/lvsehaiyang1993/article/category/7669336)







# 目录




### 文章目录
- [目录](#_0)
- [前言](#_2)
- [正文](#_4)
- [解决方案](#_18)




# 前言

最近在补习吴恩达machinelearning 的课程，上完了课，肯定是要做作业的，做作业没有问题，但在提交作业时遇到了问题，记录如下，方便后来人。

# 正文

错误提示：

```
**! Submission failed: 错误使用 submitWithConfiguration>validateResponse (line 158)
Grader sent no response


Function: validateResponse
FileName: E:\Matlab\machine-learning-ex1\ex1\lib\submitWithConfiguration.m
LineNumber: 158

Please correct your code and resubmit.**
```

# 解决方案

1.先要自行在Windows系统上安装curl并添加系统环境变量（[安装curl](https://jingyan.baidu.com/article/e3c78d64897d303c4c85f52d.html)），总之成功以后在CMD输入“curl -help”是有反应的，重启Matlab并在Matlab中输入“System(‘curl -V’)”也是有反馈的。

2.在作业文件夹的lib文件夹里找到’submitWithConfiguration.m’这个文件，修改其中的131行和134行，在POST后加上-s参数。改成下面这样。

json_command = sprintf(‘echo jsonBody=%s | curl -k -X POST **-s -d** @- %s’, body, url);

3.我就是这样解决的，submit恢复正常，之后做exercise可能还要修改’submitWithConfiguration.m’这个文件。
问题解决：

```
==                                   Part Name |     Score | Feedback
==                                   --------- |     ----- | --------
==                            Warm-up Exercise |   0 /  10 | 
==           Computing Cost (for One Variable) |  40 /  40 | Nice work!
==         Gradient Descent (for One Variable) |  50 /  50 | Nice work!
==                       Feature Normalization |   0 /   0 | 
==     Computing Cost (for Multiple Variables) |   0 /   0 | 
==   Gradient Descent (for Multiple Variables) |   0 /   0 | 
==                            Normal Equations |   0 /   0 | 
==                                   --------------------------------
==                                             |  90 / 100 | 
==
```






