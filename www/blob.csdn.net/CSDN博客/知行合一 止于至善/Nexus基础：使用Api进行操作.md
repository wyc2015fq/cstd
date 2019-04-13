
# Nexus基础：使用Api进行操作 - 知行合一 止于至善 - CSDN博客

2018年11月05日 07:31:18[liumiaocn](https://me.csdn.net/liumiaocn)阅读数：3410所属专栏：[DevOps系列之：镜像私库](https://blog.csdn.net/column/details/26478.html)



Nexus提供了RestApi，但是一部分Api仍然需要结合Groovy等进行操作，在3.3及其以后的版本上进行了强化，但是和普通的RestApi相比仍有一些“使用上的特性”。这篇文章以创建创建raw的repository为例，来确认一下相关的使用方法。
# 前提准备
准备可运行的Nexus，这里的版本为3.2.1
![在这里插入图片描述](https://img-blog.csdnimg.cn/20181105060106213.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L2xpdW1pYW9jbg==,size_16,color_FFFFFF,t_70)
# Api机制
这篇文章介绍的是Nexus3推出之初的Api的设计思路，主要是通过结合groovy脚本来完成相关的功能，简单的来说需要用户自行提供groovy脚本，nexus提供上传和运行脚本的接口，虽然有些粗糙，但是大部分功能毕竟留出了一个方式供用户去使用，强大的groovy脚本中至于需要实现什么样的功能，那就看用户自己的需要了。在一定程度上来说，算是一个能够创造API的API
# 使用步骤
## 步骤1: 以json文件为载体，创建可运行的groovy脚本
格式大体如下：
`{
  "name": "脚本名称",
  "type": "groovy",
  "content": "groovy语句"
}`注意事项：
注意双引号和单引号的使用
脚本的名称会成为后续API的一部分，请注意命名
## 步骤2: 上传json文件
使用如下方式和接口即可上传相应包含groovy操作的json文件
接口：/service/siesta/rest/v1/script
方法：POST
文件：将步骤1中准备的文件作为POST的内容传入
## 步骤3: 执行groovy文件
使用如接口即可执行在步骤2中上传的groovy脚本
接口：/service/siesta/rest/v1/script/脚本名称/run
方法：POST
此处需要注意脚本名称也会是执行接口中的一部分，命名变得灰常重要。
# 使用示例
这里使用上述方式来演示一下如何在Nexus中创建一个Raw类型的repository。
## 步骤1: 以json文件为载体，创建可运行的groovy脚本
因为创建repository需要指定blob，如果没有指定则会使用default，但是如果项目众多，将会导致default非常巨大，在前面介绍nexus的使用实践中也给出过对于不同的repository创建自己的blob，所以这里创建如下的json文件：
`liumiaocn:~ liumiao$ cat rawrepotest1.json 
{
  "name": "rawrepotest1",
  "type": "groovy",
  "content": "def rawStore = blobStore.createFileBlobStore('rawrepotest1', 'raw');repository.createRawHosted('rawrepotest1', rawStore.name);"
}
liumiaocn:~ liumiao$`建议：因为Nexus的这种使用方式本身就不是非常完整的方式，建议使用的过程中诸如rawrepotest1全部一致，这样blob和repository的关系也比较清楚，无非多上传几个大小可以忽略不计的json文件而已。
## 步骤2: 上传json文件
`liumiaocn:~ liumiao$ curl -X POST -u admin:admin123 --header "Content-Type: application/json" http://localhost:32004/service/siesta/rest/v1/script -d @rawrepotest1.json
liumiaocn:~ liumiao$`
## 步骤3: 执行groovy文件
`liumiaocn:~ liumiao$ curl -X POST -u admin:admin123 --header "Content-Type: text/plain" http://localhost:32004/service/siesta/rest/v1/script/rawrepotest1/run
{
  "name" : "rawrepotest1",
  "result" : "RepositoryImpl$$EnhancerByGuice$$13e8178d{type=hosted, format=raw, name='rawrepotest1'}"
}liumiaocn:~ liumiao$`
# 结果确认
## blob确认
已经生成相应的blob：rawrepotest1
![在这里插入图片描述](https://img-blog.csdnimg.cn/20181105072707883.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L2xpdW1pYW9jbg==,size_16,color_FFFFFF,t_70)
## repository确认
已经生成相应的raw方式的repository：rawrepotest1
![在这里插入图片描述](https://img-blog.csdnimg.cn/20181105072833431.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L2xpdW1pYW9jbg==,size_16,color_FFFFFF,t_70)
# 总结
Nexus中结合groovy能够实现Api方式的集成，但是这种方式在使用上还有诸多的不变，多次执行是否出错等都成为groovy脚本确认的事项，相关功能是否会强化可能还需要看nexus后续版本的支持情况，和groovy的结合方式会不会是功能过度时期的对应方式还需进一步观察。
# 参考文章
[https://technology.amis.nl/2016/10/27/sonatype-nexus-3-0-using-the-new-groovy-api/](https://technology.amis.nl/2016/10/27/sonatype-nexus-3-0-using-the-new-groovy-api/)
[https://github.com/sonatype/nexus-book-examples/tree/nexus-3.x/scripting/complex-script](https://github.com/sonatype/nexus-book-examples/tree/nexus-3.x/scripting/complex-script)
[https://github.com/sonatype/nexus-book-examples/blob/nexus-3.x/scripting/complex-script/rawRepositories.groovy](https://github.com/sonatype/nexus-book-examples/blob/nexus-3.x/scripting/complex-script/rawRepositories.groovy)
[https://groups.google.com/a/glists.sonatype.com/forum/\#!topic/nexus-users/fB245hjTLNg](https://groups.google.com/a/glists.sonatype.com/forum/#!topic/nexus-users/fB245hjTLNg)

