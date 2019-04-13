
# Rundeck基础：3：基础概念：操作Project - 知行合一 止于至善 - CSDN博客

2019年04月10日 07:18:13[liumiaocn](https://me.csdn.net/liumiaocn)阅读数：38


![在这里插入图片描述](https://img-blog.csdnimg.cn/20190409102518442.jpg?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9saXVtaWFvY24uYmxvZy5jc2RuLm5ldA==,size_16,color_FFFFFF,t_70)
Rundeck是一个基于Java和Grails的开源的运维自动化工具，提供了Web管理界面进行操作，同时提供命令行工具和WebAPI的访问控制方式。在这篇文章中，介绍一下基本概念和操作Project的方法。
[
](https://img-blog.csdnimg.cn/20190409102518442.jpg?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9saXVtaWFvY24uYmxvZy5jc2RuLm5ldA==,size_16,color_FFFFFF,t_70)
# 基本概念
[
](https://img-blog.csdnimg.cn/20190409102518442.jpg?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9saXVtaWFvY24uYmxvZy5jc2RuLm5ldA==,size_16,color_FFFFFF,t_70)在Rundeck中需要了解如下几个最为常见的基本概面和术语，其在Rundeck中是什么含义
[
](https://img-blog.csdnimg.cn/20190409102518442.jpg?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9saXVtaWFvY24uYmxvZy5jc2RuLm5ldA==,size_16,color_FFFFFF,t_70)|概念|说明|
|---|---|
|Project|Project是进行Job管理的场所，也是Rundeck使用时的具体实例，Rundeck可以运行多个Project|
|Jobs|Job是相关操作的步骤与设定选项以及执行Job的Node所组成，而在实际的场景中，很多运维的例行操作都可以在Rundeck中以job的方式进行定义。|
|Nodes|物理机器或者网络可访问的虚拟设备，是Rundeck中管理的资源类型，在Rundeck中将Job和Node进行了连接，使得整体可以进行管理。|
|Commands|相较于Job，Command是可以在Node上进行单次执行的可执行的命令，通过Rundeck在指定的Node上进行此命令的执行。|
|Executions|运维的操作在Rundeck中抽象成Job和Command，其每次执行都类似与实际运维作业中的例行操作或者一次性的手工操作，每次操作在Rundeck中就以Execution的形式存在，通过对Execution，可以更好的确认Job或者Command执行的状态/和结果。|
|Plugins|Rundeck很多功能都是通过插件的方式来进行设计实现的，Rundeck本身也有很多社区类型或者商业类型的插件提供给用户使用。|
# Project的创建
[
](https://img-blog.csdnimg.cn/20190409102518442.jpg?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9saXVtaWFvY24uYmxvZy5jc2RuLm5ldA==,size_16,color_FFFFFF,t_70)Rundeck启动之后，需要创建一个Project才能开始使用，接下来在创建的同时对相关的概念予以介绍。
[
](https://img-blog.csdnimg.cn/20190409102518442.jpg?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9saXVtaWFvY24uYmxvZy5jc2RuLm5ldA==,size_16,color_FFFFFF,t_70)
## 创建的三种方式
[
](https://img-blog.csdnimg.cn/20190409102518442.jpg?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9saXVtaWFvY24uYmxvZy5jc2RuLm5ldA==,size_16,color_FFFFFF,t_70)CLI方式
[
](https://img-blog.csdnimg.cn/20190409102518442.jpg?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9saXVtaWFvY24uYmxvZy5jc2RuLm5ldA==,size_16,color_FFFFFF,t_70)
> 创建示例：rd projects create -p Project名称
[
](https://img-blog.csdnimg.cn/20190409102518442.jpg?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9saXVtaWFvY24uYmxvZy5jc2RuLm5ldA==,size_16,color_FFFFFF,t_70)RestAPI方式
[
](https://img-blog.csdnimg.cn/20190409102518442.jpg?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9saXVtaWFvY24uYmxvZy5jc2RuLm5ldA==,size_16,color_FFFFFF,t_70)
```python
POST
```
```python
/
```
```python
api
```
```python
/
```
```python
13
```
```python
/
```
```python
projects
Content
```
```python
-
```
```python
Type
```
```python
:
```
```python
application
```
```python
/
```
```python
json
```
```python
{
```
```python
"name"
```
```python
:
```
```python
"MyProject"
```
```python
,
```
```python
"config"
```
```python
:
```
```python
{
```
```python
"project.label"
```
```python
:
```
```python
"My Project"
```
```python
}
```
```python
}
```
[
](https://img-blog.csdnimg.cn/20190409102518442.jpg?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9saXVtaWFvY24uYmxvZy5jc2RuLm5ldA==,size_16,color_FFFFFF,t_70)图形界面方式
通过图形界面进行输入与选择，完成Project的创建。后续在集成中将进一步展开，此处使用推行界面方式进行Project的创建。
![在这里插入图片描述](https://img-blog.csdnimg.cn/20190409162112804.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9saXVtaWFvY24uYmxvZy5jc2RuLm5ldA==,size_16,color_FFFFFF,t_70)
![在这里插入图片描述](https://img-blog.csdnimg.cn/20190409162126833.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9saXVtaWFvY24uYmxvZy5jc2RuLm5ldA==,size_16,color_FFFFFF,t_70)
[
](https://img-blog.csdnimg.cn/20190409162112804.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9saXVtaWFvY24uYmxvZy5jc2RuLm5ldA==,size_16,color_FFFFFF,t_70)设定选项
[
](https://img-blog.csdnimg.cn/20190409162112804.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9saXVtaWFvY24uYmxvZy5jc2RuLm5ldA==,size_16,color_FFFFFF,t_70)Project Name
Project的唯一标志符，可以包含数字和字母但不能包含空格和其他特殊字符。
Label
由于Project Name有标志符的限制，Lable在这里起到Project展示名称的作用，比如可以包含空格，这样的Project Name看起来会更自然一点。
Description
关于项目的简单描述，一般来说时间个简单的句子用于解释项目的目的，如果有大量的介绍性的内容需要写，则可以考虑使用创建project的README来实现。
Execution Mode
在执行模式中可以在Project级别对Job执行和编排进行设定
设定项目设定说明|
|---|---|
Disable Execution选中则会关闭执行Job和Command的权限|
Disable Schedule选中则会关闭Job编排的权限和功能|
User Interface
用于设定Project的辅助配置功能，设定项目说明如下所示
设定项目设定说明|
|---|---|
Job Group Expansion LevelJob页面中，Job Group是否折叠显示。取值范围：缺省为1，表示显示1个。设定为0则表示全部折叠，-1为全部打开。|
Display the Readme在列表等页面是否显示Project的Readme页面|
Display the MOTD在列表等页面是否显示Project的Readme页面|
Default Node Executor
用于设定Node的执行方式，通过此项设定来提供到远端Node上执行脚本的能力，在Linux机器上，缺省方式为SSH。除去SSH还有其他多种方式比如通过指定的脚本或者Ansible等
Default File Copier
用于设定文件的拷贝方式，通过此项设定来提供将文件传输到远端Node上的能力，在Linux机器上，缺省方式为SCP。除去SCP还有其他多种方式比如通过指定脚本或者Ansible等
设定示例
在本文的示例中，将只设定Project的基础信息，其余保持缺省设定。
设定项目设定值|
|---|---|
Project Namerundeck-test-project|
LableRundeck Test Project|
DescriptionThis project is to introduce Rundeck function|
![在这里插入图片描述](https://img-blog.csdnimg.cn/20190409164617782.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9saXVtaWFvY24uYmxvZy5jc2RuLm5ldA==,size_16,color_FFFFFF,t_70)
当然保持缺省状态实际是有前提的，比如SSH，相关的keypair，Rundeck已经事前做好了，在实际的环境中，这里需要使用相关的证书予以替代。
[
](https://img-blog.csdnimg.cn/20190409164617782.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9saXVtaWFvY24uYmxvZy5jc2RuLm5ldA==,size_16,color_FFFFFF,t_70)liumiaocn:~ liumiao$ dockerexec-it rundeck sh
sh-4.3$ls/home/rundeck/.ssh/id_rsa
/home/rundeck/.ssh/id_rsa
sh-4.3$cd/home/rundeck/.ssh/
sh-4.3$lsid_rsa	id_rsa.pub
sh-4.3$[
](https://img-blog.csdnimg.cn/20190409164617782.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9saXVtaWFvY24uYmxvZy5jc2RuLm5ldA==,size_16,color_FFFFFF,t_70)创建确认
[
](https://img-blog.csdnimg.cn/20190409164617782.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9saXVtaWFvY24uYmxvZy5jc2RuLm5ldA==,size_16,color_FFFFFF,t_70)创建完成后可以看到Rundeck的整体功能菜单以及关于新建的Project的信息，相关的操作则可以进一步的进行了。
![在这里插入图片描述](https://img-blog.csdnimg.cn/20190409164823421.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9saXVtaWFvY24uYmxvZy5jc2RuLm5ldA==,size_16,color_FFFFFF,t_70)
点击保存，根据提示接下来就应该对Node，也就是Rundeck操作管控的资源进行设定了。
![在这里插入图片描述](https://img-blog.csdnimg.cn/20190409165228281.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9saXVtaWFvY24uYmxvZy5jc2RuLm5ldA==,size_16,color_FFFFFF,t_70)
其他内容
[https://liumiaocn.blog.csdn.net/article/details/89135773](https://liumiaocn.blog.csdn.net/article/details/89135773)
参考文章
[https://docs.rundeck.com/docs.html](https://docs.rundeck.com/docs.html)
[https://docs.rundeck.com/docs/manual/getting-started.html\#project-setup](https://docs.rundeck.com/docs/manual/getting-started.html#project-setup)
[https://docs.rundeck.com/docs/administration/projects/create-new-project.html\#graphical-interface](https://docs.rundeck.com/docs/administration/projects/create-new-project.html#graphical-interface)

