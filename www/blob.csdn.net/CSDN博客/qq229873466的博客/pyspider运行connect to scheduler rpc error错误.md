# pyspider运行connect to scheduler rpc error错误 - qq229873466的博客 - CSDN博客

2017年03月12日 21:27:26[qq229873466](https://me.csdn.net/qq229873466)阅读数：815


运行[pyspider](http://www.pyspider.cn)遇到connect to scheduler rpc error错误，单独运行pyspider scheduler 出现No nodule named xmlrpc_server错误．这个问题很多网友都遇到了．其实解决问题很简单，重装一下six库就可以．

修复命令：

pip install -U six

参考：[运行pyspider scheduler 出现No nodule named xmlrpc_server](http://www.pyspider.cn/fq/No-nodule-named-xmlrpc-server-11.html)


