# ztree “Uncaught TypeError: Object # has no method 'toLowerCase'” - z69183787的专栏 - CSDN博客
2013年07月02日 10:52:25[OkidoGreen](https://me.csdn.net/z69183787)阅读数：1979
今天上头说ztree.js 这个控件似乎有点问题，树展开了不能缩回去，
我感觉应该是js 的问题，打开浏览器，开启调试，发现如下错误
![image](http://images.cnblogs.com/cnblogs_com/jicheng1014/201208/201208281740089258.png)
google后无解，一步一步排查，发现将下列代码删除后正常
```
<dl>
                    <dt>节点名称</dt>
                    <dd>
                        <span id="nodeName"></span>
                    </dd>
                    <dt>节点状态</dt>
                    <dd>
                        <span id="nodeStatus"></span>
                    </dd>
                </dl>
```
慢慢找原因，最后发现是这个id 的命名似乎与jquery 有冲突   
“nodeName”
换成"theNodeName" 后，ztree.js 就可以顺利collapse了
