# OpenSCENARIO——对无人驾驶场景的定义 - 心纯净，行致远 - CSDN博客





2019年04月11日 14:16:31[吉大秦少游](https://me.csdn.net/zhanshen112)阅读数：14








# OpenSCENARIO

**动机**

事实：事实上的标准化已在较低的水平上实现

![](https://img-blog.csdnimg.cn/20190411141259964.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3poYW5zaGVuMTEy,size_16,color_FFFFFF,t_70)

结果：道路文件可以再多种仿真工具之间转换

**动机****-****当前情况**
- 
ADAS功能要求需要在各种各样的环境中进行扩展测试

- 
各种工具的功能集是高度不同的

- 
每一种工具都可以很好地用于其特定目的

- 
场景从一个工具迁移到另外一个工具的代价很高

- 
独立的第三方不能很好地提供与多种仿真工具兼容的场景定义格式

- 
结果：动态内容可能还没有以标准化的方式传输，并且高度依赖于所涉及的工具。


![](https://img-blog.csdnimg.cn/20190411141259979.png)

**动机**

假设：所有的工具都共享相当多的公共功能——>结论：动态内容的描述可能会一致

![](https://img-blog.csdnimg.cn/2019041114130010.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3poYW5zaGVuMTEy,size_16,color_FFFFFF,t_70)
- 
相同的动态内容并不意味着相同的结果


**目标**
- 
提供可转换的XML场景定义以用于各种各样的使用案例

- 
使用OpenDRIVE/OpenCRG作为基本的图层

- 
允许独立的第三方内容提供商

- 
为场景定义的验证提供标准化/开放式工具集

- 
提供支持服务


![](https://img-blog.csdnimg.cn/2019041114130051.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3poYW5zaGVuMTEy,size_16,color_FFFFFF,t_70)

**OpenScenario****倡议**

**定义阶段，第一步**

![](https://img-blog.csdnimg.cn/2019041114130088.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3poYW5zaGVuMTEy,size_16,color_FFFFFF,t_70)

**定义阶段，第二步**

![](https://img-blog.csdnimg.cn/20190411141300116.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3poYW5zaGVuMTEy,size_16,color_FFFFFF,t_70)

**维护阶段**

![](https://img-blog.csdnimg.cn/20190411141300146.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3poYW5zaGVuMTEy,size_16,color_FFFFFF,t_70)

**基本布置**

![](https://img-blog.csdnimg.cn/20190411141300172.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3poYW5zaGVuMTEy,size_16,color_FFFFFF,t_70)

**车辆定义**

![](https://img-blog.csdnimg.cn/20190411141300219.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3poYW5zaGVuMTEy,size_16,color_FFFFFF,t_70)

**驾驶员定义**

![](https://img-blog.csdnimg.cn/20190411141300456.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3poYW5zaGVuMTEy,size_16,color_FFFFFF,t_70)

**操作板定义**

![](https://img-blog.csdnimg.cn/20190411141300281.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3poYW5zaGVuMTEy,size_16,color_FFFFFF,t_70)

**操纵定义**

![](https://img-blog.csdnimg.cn/20190411141300331.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3poYW5zaGVuMTEy,size_16,color_FFFFFF,t_70)

**总结**

•OpenScenario将用作动态场景的开放文件格式

•验证工具和测试数据应该补充在格式规范。

•初始用户需求已收集

•正在审查结构初稿。

•广泛的用户基础和核心团队应为持续发展做出贡献。

•OpenScenario旨在成为事实上的标准

资源：

•http://www.opensecario.org网站

•邮件地址：opensocario@opensocario.org

•http://tracking.vires.com（错误/功能跟踪）



