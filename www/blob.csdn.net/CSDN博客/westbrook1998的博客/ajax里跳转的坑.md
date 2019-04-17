# ajax里跳转的坑 - westbrook1998的博客 - CSDN博客





2019年01月01日 14:50:36[westbrook1998](https://me.csdn.net/westbrook1998)阅读数：22








ajax登录后在回调的success里一直不能跳转，然后找了很久发现都不对，最后发现是`localhost:8080/show`这里写错了，应该是`http://localhost:8080`，前一天也是这里写错了导致跨域报错搞了半天



