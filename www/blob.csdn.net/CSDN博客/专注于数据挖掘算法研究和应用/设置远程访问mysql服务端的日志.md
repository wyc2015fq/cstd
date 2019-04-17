# 设置远程访问mysql服务端的日志 - 专注于数据挖掘算法研究和应用 - CSDN博客





2011年09月27日 12:58:00[fjssharpsword](https://me.csdn.net/fjssharpsword)阅读数：2364标签：[mysql																[insert																[user																[网络](https://so.csdn.net/so/search/s.do?q=网络&t=blog)
个人分类：[Linux																[Windows](https://blog.csdn.net/fjssharpsword/article/category/6480443)](https://blog.csdn.net/fjssharpsword/article/category/6480442)





1.网络上常见方法是设置user表内root 用户的host字段为%，但实际上无法实现远程访问，因为root不允许被远程访问；



2.因此要实现远程访问，需要为mysql新建用户，并启用和root一样的权限，这样就远程访问时就用新用户非root；直接user表insert一个用户即可；](https://so.csdn.net/so/search/s.do?q=user&t=blog)](https://so.csdn.net/so/search/s.do?q=insert&t=blog)](https://so.csdn.net/so/search/s.do?q=mysql&t=blog)




