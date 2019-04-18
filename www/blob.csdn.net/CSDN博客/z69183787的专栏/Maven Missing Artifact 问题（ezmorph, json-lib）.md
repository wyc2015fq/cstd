# Maven Missing Artifact 问题（ezmorph, json-lib） - z69183787的专栏 - CSDN博客
2015年07月16日 18:26:46[OkidoGreen](https://me.csdn.net/z69183787)阅读数：1598
最近要在项目中处理JSON格式的数据，所以要引用相关的包，其中有两个
**[html]**[view
 plain](http://blog.csdn.net/zhouyingge1104/article/details/35994379#)[copy](http://blog.csdn.net/zhouyingge1104/article/details/35994379#)
- <dependency>
- <groupId>ezmorph</groupId>
- <artifactId>ezmorph</artifactId>
- <version>1.0.4</version>
- </dependency>
- <dependency>
- <groupId>json-lib</groupId>
- <artifactId>json-lib</artifactId>
- <version>2.4-jdk15</version>
- </dependency>
但是两个denpence都报错：Missing artifact。。。
解决方法：是groupId的问题，两个groupId应该是net.sf.ezmorph和net.sf.json-lib，
**[html]**[view
 plain](http://blog.csdn.net/zhouyingge1104/article/details/35994379#)[copy](http://blog.csdn.net/zhouyingge1104/article/details/35994379#)![在CODE上查看代码片](https://code.csdn.net/assets/CODE_ico.png)[](https://code.csdn.net/snippets/411574/fork)
- <dependency>
- <groupId>net.sf.ezmorph</groupId>
- <artifactId>ezmorph</artifactId>
- <version>1.0.4</version>
- </dependency>
但是这样修改之后第一个ezmorph没问题了，json-lib还是报错。
新的解决方法：json-lib提供了两个jdk版本的实现， json-lib-jdk13.jar和json-lib-jdk15.jar，
把第二个改成：
**[html]**[view
 plain](http://blog.csdn.net/zhouyingge1104/article/details/35994379#)[copy](http://blog.csdn.net/zhouyingge1104/article/details/35994379#)![在CODE上查看代码片](https://code.csdn.net/assets/CODE_ico.png)[](https://code.csdn.net/snippets/411574/fork)
- <dependency>
- <groupId>net.sf.json-lib</groupId>
- <artifactId>json-lib</artifactId>
- <classifier>jdk15</classifier>
- <version>2.4</version>
- </dependency>
就OK了。
