
# spark概念：container和Executor - leofionn的博客 - CSDN博客


2018年06月05日 21:16:25[leofionn](https://me.csdn.net/qq_36142114)阅读数：1522


![](https://img-blog.csdn.net/20180605214550942?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3FxXzM2MTQyMTE0/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)![](https://img-blog.csdn.net/20180605210846573)



![](https://img-blog.csdn.net/20180605211726812?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3FxXzM2MTQyMTE0/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)


AM：
![](https://img-blog.csdn.net/20180605210943237)
Executor
![](https://img-blog.csdn.net/20180605211008168)
![](https://img-blog.csdn.net/20180605211021354)
AM启动一个container  -> executorlauncher
executorlauncher启动一个container  -> CoarseGraineExecutorBackend

相关链接，具体内容：
http://spark.apache.org/docs/2.2.0/running-on-yarn.html
http://spark.apache.org/docs/2.2.0/cluster-overview.html
http://spark.apache.org/docs/2.2.0/running-on-yarn.html


