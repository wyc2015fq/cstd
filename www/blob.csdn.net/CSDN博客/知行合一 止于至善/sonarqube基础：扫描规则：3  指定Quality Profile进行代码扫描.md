
# sonarqube基础：扫描规则：3: 指定Quality Profile进行代码扫描 - 知行合一 止于至善 - CSDN博客

2018年11月04日 05:42:09[liumiaocn](https://me.csdn.net/liumiaocn)阅读数：2387所属专栏：[DevOps系列之：持续构建与集成](https://blog.csdn.net/column/details/27057.html)



这篇文章以前面创建的Spring boot2的web应用为例，使用自定义的Quality Profile来进行质量的检查。
# 事前准备
关于spring boot+maven+jacoco+sonarqube+junit等相关基础，请参看 spring快速入门系列中的整理，此处不再赘述。
spring快速入门：[https://blog.csdn.net/column/details/13907.html](https://blog.csdn.net/column/details/13907.html)
# Quality Profile准备
根据前一篇文章的整理，请自行准备自己的Quality Profile，比如这里根据sonarqube的三种分类方式整理如下三种Quality Profile：
|Quality Profile|说明|
|---|---|
|java-sonarway-bug.xml|缺陷相关的规则|
|java-sonarway-vulnerability.xml|脆弱性相关的规则|
|java-sonarway-codesmell.xml|代码改善相关的规则|
## 命令执行
执行如下命令，生成响应的Quality Profile，当然使用UI进行直接操作也没有问题。
> curl -X POST -uadmin:admin
> [http://localhost:32003/api/qualityprofiles/restore](http://localhost:32003/api/qualityprofiles/restore)
> --form backup=@java-sonarway-bug.xml

> curl -X POST -uadmin:admin
> [http://localhost:32003/api/qualityprofiles/restore](http://localhost:32003/api/qualityprofiles/restore)
> --form backup=@java-sonarway-codesmell.xml

> curl -X POST -uadmin:admin
> [http://localhost:32003/api/qualityprofiles/restore](http://localhost:32003/api/qualityprofiles/restore)
> --form backup=@java-sonarway-vulnerability.xml

> 结果确认

> 执行中需确认规则是否都成功导入，ruleFailures的个数应该为0

> liumiaocn:java liumiao$ curl -X POST -uadmin:admin http://localhost:32003/api/qualityprofiles/restore --form backup=@java-sonarway-bug.xml
{"profile":{"key":"java-java-sonarway-bug-65847","name":"java-sonarway-bug","language":"java","isDefault":false,"isInherited":false,"languageName":"Java"},"ruleSuccesses":75,"ruleFailures":0}liumiaocn:java liumiao$ 
liumiaocn:java liumiao$ curl -X POST -uadmin:admin http://localhost:32003/api/qualityprofiles/restore --form backup=@java-sonarway-codesmell.xml
{"profile":{"key":"java-java-sonarway-codesmell-25578","name":"java-sonarway-codesmell","language":"java","isDefault":false,"isInherited":false,"languageName":"Java"},"ruleSuccesses":159,"ruleFailures":0}liumiaocn:java liumiao$ 
liumiaocn:java liumiao$ curl -X POST -uadmin:admin http://localhost:32003/api/qualityprofiles/restore --form backup=@java-sonarway-vulnerability.xml
{"profile":{"key":"java-java-sonarway-vulnerability-91184","name":"java-sonarway-vulnerability","language":"java","isDefault":false,"isInherited":false,"languageName":"Java"},"ruleSuccesses":20,"ruleFailures":0}liumiaocn:java liumiao$ 
liumiaocn:java liumiao$
> 执行后从Sonarqube的管理页面也能看到成功导入的状况
![在这里插入图片描述](https://img-blog.csdnimg.cn/20181103111113359.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L2xpdW1pYW9jbg==,size_16,color_FFFFFF,t_70)
> 指定Quality Profile

> 指定方式

> 缺省方式下，使用的是Sonar way进行扫描，如果指定-Dsonar.profile则可指定相应的Quality Profile进行扫描。

> 死而不僵的sonar.profile

> 需要说明的是，此项功能在4.2之后已经是一个deprecated的状态了，直到目前的版本7仍然是此种状态。官方给出的解释是：通过Web界面你可以很容易的进行定制你自己需要的Profile，而且认为这种事情应该在server端做。

> 由于此项功能尚在被广泛使用，并且目前也没提供除了手动操作之外的其他解决方案，相当于直接废弃一项功能，大概是这个原因虽然从4开始deprecated了，但是一直到版本7都是可以使用的。这里使用LTS版本5.6.5进行验证。

> 我个人的结论是：直接无视，先继续使用，因为基本也不影响使用，后面说不定就自己想通了，到时可能会有对应的替代方案出来也说不定。

> 执行profile

> 使用缺陷和脆弱性Profile

> 因为此处使用的例子中只有2个Code Smell，所以使用这里只检查缺陷和脆弱性的Profile是无法检出任何问题的。

> 使用缺陷Profile检查：mvn sonar:sonar -Dsonar.host.url=http://localhost:32003 -Dsonar.profile=java-sonarway-bug

> 使用安全Profile检查：mvn sonar:sonar -Dsonar.host.url=http://localhost:32003 -Dsonar.profile=java-sonarway-vulnerability

> 使用这两个profile，都是0检出
![在这里插入图片描述](https://img-blog.csdnimg.cn/20181103114524424.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L2xpdW1pYW9jbg==,size_16,color_FFFFFF,t_70)
> 使用Code Smell的Profile

> 使用Code Smell的Profile检查：mvn sonar:sonar -Dsonar.host.url=http://localhost:32003 -Dsonar.profile=java-sonarway-codesmell

> 结果确认如下，可以看到检出两个Code Smell的问题。
![在这里插入图片描述](https://img-blog.csdnimg.cn/20181103114818888.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L2xpdW1pYW9jbg==,size_16,color_FFFFFF,t_70)
> 总结

> 使用sonar.profile则可使用maven方式或者sonar-scanner方式使用企业自行裁剪的Quality Profile，虽然这里只能裁剪，但是也已经能够满足不少实际的需求了。


