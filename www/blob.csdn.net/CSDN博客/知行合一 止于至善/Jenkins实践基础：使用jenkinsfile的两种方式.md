
# Jenkins实践基础：使用jenkinsfile的两种方式 - 知行合一 止于至善 - CSDN博客

2019年01月12日 12:02:09[liumiaocn](https://me.csdn.net/liumiaocn)阅读数：1949所属专栏：[DevOps系列之：持续构建与集成](https://blog.csdn.net/column/details/27057.html)



![在这里插入图片描述](https://img-blog.csdn.net/20160809065608330)
jenkinsfile以代码的形式将持续集成中的流水线构成进行保存，更是可以结合scm进行保存，这篇文章介绍一下如何使用jenkinsfile的两种方式。
[
](https://img-blog.csdn.net/20160809065608330)
# 前提准备
[
](https://img-blog.csdn.net/20160809065608330)使用可以创建pipeline方式的jenkins服务即可，如果没有相关的环境，可参看如下文章进行创建：
[
](https://img-blog.csdn.net/20160809065608330)[https://blog.csdn.net/liumiaocn/article/details/86098125](https://blog.csdn.net/liumiaocn/article/details/86098125)
[
](https://img-blog.csdn.net/20160809065608330)
# 插件安装
[
](https://img-blog.csdn.net/20160809065608330)流水线方式搭建，可安装如下插件
[
](https://img-blog.csdn.net/20160809065608330)pipeline-model-definition
build-pipeline-plugin
buildgraph-view
workflow-aggregator
[
](https://img-blog.csdn.net/20160809065608330)也可以直接安装blueocean
[
](https://img-blog.csdn.net/20160809065608330)blueocean
[
](https://img-blog.csdn.net/20160809065608330)
# pipeline代码示例
[
](https://img-blog.csdn.net/20160809065608330)`node {
    stage('build'){
     echo 'build';
    }
    
    stage('test'){
     echo 'test';
    }
    
    stage('deploy'){
     echo 'deploy';
    }
}`[
](https://img-blog.csdn.net/20160809065608330)
# 方式1: 通过jenkins图形界面进行jenkinsfile编辑
[
](https://img-blog.csdn.net/20160809065608330)点击Jenkins 选择New Item，如下图方式创建Pipeline方式的job，本文中job名称为：build2deployfromjenkins
![在这里插入图片描述](https://img-blog.csdnimg.cn/20190112082301627.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L2xpdW1pYW9jbg==,size_16,color_FFFFFF,t_70)
将sample用的jenkinsfile拷贝到下图中Advanced Project Options中，选择Pipeline script，然后将内容拷贝到Scripts中，点击Save进行保存
![在这里插入图片描述](https://img-blog.csdnimg.cn/20190112082347953.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L2xpdW1pYW9jbg==,size_16,color_FFFFFF,t_70)
点击Build Now进行构建，下图为构建结果的显示
![在这里插入图片描述](https://img-blog.csdnimg.cn/20190112082451666.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L2xpdW1pYW9jbg==,size_16,color_FFFFFF,t_70)
# 方式2: 结合版本管理工具进行构建
点击Jenkins 选择New Item，如下图方式创建Pipeline方式的job，本文中job名称为：build2deployfromscm
![在这里插入图片描述](https://img-blog.csdnimg.cn/20190112112918416.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L2xpdW1pYW9jbg==,size_16,color_FFFFFF,t_70)
以sample的jenkinsfile为基础，稍作拷贝用于区别上个示例。选择Advanced Project Options中的Pipeline script from scm，做如下设定：
![在这里插入图片描述](https://img-blog.csdnimg.cn/20190112120057822.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L2xpdW1pYW9jbg==,size_16,color_FFFFFF,t_70)
scm相关设定准备
`[root@platform jenkins]# git clone http://192.168.163.118:32001/root/jenkinsfileprj.git
Cloning into 'jenkinsfileprj'...
Username for 'http://192.168.163.118:32001': root
Password for 'http://root@192.168.163.118:32001': 
warning: You appear to have cloned an empty repository.
[root@platform jenkins]# cd jenkinsfileprj/
[root@platform jenkinsfileprj]# touch Jenkinsfile
[root@platform jenkinsfileprj]# vi Jenkinsfile
[root@platform jenkinsfileprj]# ls
Jenkinsfile
[root@platform jenkinsfileprj]# cat Jenkinsfile 
node {
    stage('build-using-scm'){
     echo 'build';
    }
    
    stage('test-using-scm'){
     echo 'test';
    }
    
    stage('deploy-using-scm'){
     echo 'deploy';
    }
}
[root@platform jenkinsfileprj]# git add Jenkinsfile 
[root@platform jenkinsfileprj]# git commit -m "for jenkinsfile scm test"
[master (root-commit) 9d55ff6] for jenkinsfile scm test
 1 file changed, 13 insertions(+)
 create mode 100644 Jenkinsfile
[root@platform jenkinsfileprj]# git push origin master
Username for 'http://192.168.163.118:32001': root
Password for 'http://root@192.168.163.118:32001': 
Counting objects: 3, done.
Compressing objects: 100% (2/2), done.
Writing objects: 100% (3/3), 300 bytes | 0 bytes/s, done.
Total 3 (delta 0), reused 0 (delta 0)
To http://192.168.163.118:32001/root/jenkinsfileprj.git
 * [new branch]      master -> master
[root@platform jenkinsfileprj]#`![在这里插入图片描述](https://img-blog.csdnimg.cn/20190112115806428.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L2xpdW1pYW9jbg==,size_16,color_FFFFFF,t_70)
点击Build Now进行构建，下图为构建结果的显示
![在这里插入图片描述](https://img-blog.csdnimg.cn/20190112120138269.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L2xpdW1pYW9jbg==,size_16,color_FFFFFF,t_70)
# 总结
这篇文章介绍了在jenkins中使用jenkinsfile的两种常用方法，从Infrastructure as code观点，方式2更值得在实际项目中推荐，所有的操作都以自动化和代码的形式出现，可以更好地保证环境的一致性

[
](https://img-blog.csdnimg.cn/20190112115806428.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L2xpdW1pYW9jbg==,size_16,color_FFFFFF,t_70)
