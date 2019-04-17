# matlab  均值和协方差 - 家家的专栏 - CSDN博客





2013年12月10日 09:39:54[依海之燕](https://me.csdn.net/yihaizhiyan)阅读数：824









The following matlab syntax assumes that the ![m](https://inst.eecs.berkeley.edu/~ee127a/book/login/eqs/13952041964-130.png) data points in ![mathbf{R}^n](https://inst.eecs.berkeley.edu/~ee127a/book/login/eqs/1996980945799249722-130.png) are
 collected in a ![n times m](https://inst.eecs.berkeley.edu/~ee127a/book/login/eqs/3405331112867365515-130.png) matrix ![X](https://inst.eecs.berkeley.edu/~ee127a/book/login/eqs/11264033881-130.png): ![X = [x_1,ldots,x_m]](https://inst.eecs.berkeley.edu/~ee127a/book/login/eqs/6993711612918051957-130.png).



Matlab syntax

>> xhat = mean(X,2); % mean of columns of matrix X
>> Xc = X-xhat*ones(1,m); % centered data matrix
>> Sigma = (1/m)*Xc'*Xc; % covariance matrix
>> Sigma = cov(X',1); % built-in command produces the same thingFrom:[https://inst.eecs.berkeley.edu/~ee127a/book/login/exa_sym_cov_matrix.html](https://inst.eecs.berkeley.edu/~ee127a/book/login/exa_sym_cov_matrix.html)




