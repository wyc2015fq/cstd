# Jenkins Pipeline 项目持续集成交互实践路径 - 纸上得来终觉浅，绝知此事要躬行 - CSDN博客





2017年09月12日 10:38:41[boonya](https://me.csdn.net/boonya)阅读数：20284








## Jenkins Pipleline插件介绍

![](https://img-blog.csdn.net/20170912103559404)


Jenkins 2.x的精髓是Pipeline as Code，是帮助Jenkins实现CI到CD转变的重要角色。什么是Pipeline，简单来说，就是一套运行于Jenkins上的工作流框架，将原本独立运行于单个或者多个节点的任务连接起来，实现单个任务难以完成的复杂发布流程。Pipeline的实现方式是一套Groovy DSL，任何发布流程都可以表述为一段Groovy脚本，并且Jenkins支持从代码库直接读取脚本，从而实现了Pipeline as Code的理念。




**Pipeline的几个基本概念：  **
- Stage: 阶段，一个Pipeline可以划分为若干个Stage，每个Stage代表一组操作。注意，Stage是一个逻辑分组的概念，可以跨多个Node。
- Node: 节点，一个Node就是一个Jenkins节点，或者是Master，或者是Agent，是执行Step的具体运行期环境。
- Step: 步骤，Step是最基本的操作单元，小到创建一个目录，大到构建一个Docker镜像，由各类Jenkins Plugin提供。

**Jenkins Pipleline插件：**

[https://wiki.jenkins.io/display/JENKINS/Pipeline+Plugin](https://wiki.jenkins.io/display/JENKINS/Pipeline+Plugin)
使用时，Jenkins需要安装Pipeline和Maven插件。



## Jenkins Pipeline语法参考
以下是完整的Jenkins Pipeline语法参考卡。当然，当您添加插件或插件更新时，新的流水线脚本元素将在您的环境中可用。Pipeline代码段生成器和UI将自动添加这些和任何相关的帮助文本，以便您知道如何使用它们！




### Basics



![](https://www.cloudbees.com/sites/default/files/basics.png)

### Advanced

![](https://www.cloudbees.com/sites/default/files/advanced.png)

### File System

![](https://www.cloudbees.com/sites/default/files/file-system.png)

### Flow Control

![](https://www.cloudbees.com/sites/default/files/flow-control.png)

### Docker

![](https://www.cloudbees.com/sites/default/files/docker-1.png)
![](https://www.cloudbees.com/sites/default/files/docker-2.png)


## Jenkins Pipleline 脚本示例

参考地址：[https://www.cloudbees.com/blog/using-pipeline-plugin-accelerate-continuous-delivery-part-3](https://www.cloudbees.com/blog/using-pipeline-plugin-accelerate-continuous-delivery-part-3)




```
stage 'build'
node {
     git 'https://github.com/cloudbees/todo-api.git'
     withEnv(["PATH+MAVEN=${tool 'm3'}/bin"]) {
          sh "mvn -B –Dmaven.test.failure.ignore=true clean package"
     }
     stash excludes: 'target/', includes: '**', name: 'source'
}
stage 'test'
parallel 'integration': { 
     node {
          unstash 'source'
          withEnv(["PATH+MAVEN=${tool 'm3'}/bin"]) {
               sh "mvn clean verify"
          }
     }
}, 'quality': {
     node {
          unstash 'source'
          withEnv(["PATH+MAVEN=${tool 'm3'}/bin"]) {
               sh "mvn sonar:sonar"
          }
     }
}
stage 'approve'
timeout(time: 7, unit: 'DAYS') {
     input message: 'Do you want to deploy?', submitter: 'ops'
}
stage name:'deploy', concurrency: 1
node {
     unstash 'source'
     withEnv(["PATH+MAVEN=${tool 'm3'}/bin"]) {
          sh "mvn cargo:deploy"
     }
}
```
注:m3表示Jenkins配置的maven名称。


## Jenkins Pipeline Docker脚本示例

[Docker Pipeline Plugin (示例)](https://go.cloudbees.com/docs/cloudbees-documentation/cje-user-guide/index.html#docker-workflow)


Docker Pipeline插件公开了一个Docker全局变量，它为普通的Docker操作提供DSL，只需要在运行步骤的执行程序上使用一个Docker客户端（在您的节点步骤中使用一个标签来定位启用Docker的代理）。

默认情况下，Docker全局变量连接到本地Docker守护程序。 您可以使用docker.withServer步骤连接到远程Docker主机。 图像步骤为特定的Docker图像提供句柄，并允许执行其他与图像相关的其他步骤，包括image.inside步骤。 内部步骤将启动指定的容器并在该容器中运行一个步骤：

示例1：




```
docker.image('maven:3.3.3-jdk8').inside('-v ~/.m2/repo:/m2repo') {
     sh 'mvn -Dmaven.repo.local=/m2repo clean package'
}
```



示例2(参考：[http://www.youruncloud.com/blog/127.html](http://www.youruncloud.com/blog/127.html))：



```
node{  
  // 代码检出
  stage('get Code') {
    git credentialsId: 'git-credentials-id', url: 'http://192.168.19.250/ufleet/uflow.git'
  }
  
    // 镜像中进行单元测试
  stage('unit testing'){ 
    // 启动golnag:1.7并在golang内编译代码
    docker.image('golang:1.7').inside {
      sh './script/unittest.sh'
    }
  }
  
  // 镜像中代码构建
  stage('Build'){    

    def confFilePath = 'conf/app.conf'
    def config = readFile confFilePath

    writeFile file: confFilePath, text: config
    
    // 启动golnag:1.7并在golang内编译代码
    docker.image('golang:1.7').inside {
      sh './script/build.sh'
    }
  }
  
  // 编译镜像并push到仓库
  def imagesName = '192.168.18.250:5002/ufleet/uflow:v0.9.1.${BUILD_NUMBER}'  
  stage('Image Build And Push'){
    docker.withRegistry('http://192.168.18.250:5002', 'registry-credentials-id') {
      docker.build(imagesName).push()
    }
  }
  
  // 启动刚运行的容器
  stage('deploy iamegs'){    
    // 需要删除旧版本的容器，否则会导致端口占用而无法启动。
    try{
      sh 'docker rm -f cicdDemo'
    }catch(e){
        // err message
    }
    docker.image(imagesName).run('-p 9091:80 --name cicdDemo') 
  }
}
```



更多使用方法请参考Jenkins Pipeline docker语法。

另外可以借助Pipeline Syntax来生成基本的脚本：

![](https://img-blog.csdn.net/20170912103807164)





## Jenkins Pipeline war 包部署示例

Pipeline通过增强stage原语来提供此功能。例如，一个阶段可以具有一个定义的并发级别，以指示在任何时候只有一个线程应该在该阶段中运行。这实现了运行部署的期望状态，就像运行一样快。
参考：[https://jenkins.io/doc/book/pipeline-as-code/](https://jenkins.io/doc/book/pipeline-as-code/)



```
stage name: 'Production', concurrency: 1
 node {
     unarchive mapping: ['target/x.war' : 'x.war']
     deploy 'target/x.war', 'production'
     echo 'Deployed to http://localhost:8888/production/'
 }
```



**本博参考文章：**

[初试Jenkins2.0 Pipeline持续集成](http://blog.csdn.net/aixiaoyang168/article/details/72818804)


[持续集成 Jenkins 2.7 pipeline 功能使用介绍](https://testerhome.com/topics/5044)

[Pipeline as Code Introduction (包含war包部署介绍)](https://jenkins.io/doc/book/pipeline-as-code/)


[Jenkins2 Pipeline: deploy on tomcat](https://stackoverflow.com/questions/37787805/jenkins2-pipeline-deploy-on-tomcat)


**更多阅读Jenkinsfile参考：**

[Pipeline](https://jenkins.io/doc/book/pipeline/)


[Getting Started with Pipeline](https://jenkins.io/doc/book/pipeline/getting-started/)

[Using a Jenkinsfile](https://jenkins.io/doc/book/pipeline/jenkinsfile/#using-a-jenkinsfile)








