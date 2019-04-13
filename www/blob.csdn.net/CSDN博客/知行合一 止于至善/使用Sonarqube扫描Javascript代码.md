
# 使用Sonarqube扫描Javascript代码 - 知行合一 止于至善 - CSDN博客

2018年12月25日 05:49:35[liumiaocn](https://me.csdn.net/liumiaocn)阅读数：1285


使用sonarqube对javascript代码进行扫描，分析代码质量，最简单的方式莫过于使用缺省的sonar-way中的javascript的规则，使用sonar-scanner进行扫描，这篇文章通过最简单的例子，来进行说明。
# 事前准备
## Sonarqube
Sonarqube可以使用docker版本快速搭建，可以参看一下Easypack整理的镜像，具体使用可以参看如下链接，这里不再赘述：
[https://hub.docker.com/r/liumiaocn/sonarqube/](https://hub.docker.com/r/liumiaocn/sonarqube/)
## 环境假定
本文使用到的sonarqube为本机32003可以访问到的服务。版本为5.6.5
## sonar-scanner
sonar-scanner详细信息如下：
`liumiaocn:sonar liumiao$ sonar-scanner -v
INFO: Scanner configuration file: /Users/liumiao/Desktop/sonar/sonar-scanner-3.2.0.1227-macosx/conf/sonar-scanner.properties
INFO: Project root configuration file: /Users/liumiao/sonar/sonar-project.properties
INFO: SonarQube Scanner 3.2.0.1227
INFO: Java 1.8.0_121 Oracle Corporation (64-bit)
INFO: Mac OS X 10.14 x86_64
liumiaocn:sonar liumiao$`
## 项目目录构成
项目文件与目录构成信息如下：
`liumiaocn:sonar liumiao$ tree
.
├── sonar-project.properties
└── src
    └── Person.js
1 directory, 2 files
liumiaocn:sonar liumiao$`
## JavaScript源码
使用如下Person.js的JavaScript源码，详细信息如下
`liumiaocn:sonar liumiao$ cat src/Person.js 
var Person = function(first, last, middle) {
    this.first = first;
    this.middle = middle;
    this.last = last;
};
Person.prototype = {
    whoAreYou : function() {
        return this.first + (this.middle ? ' ' + this.middle: '') + ' ' + this.last;
    }
};
var a = NaN;
if (a === NaN) {  // Noncompliant; always false
  console.log("a is not a number");  // this is dead code
}
if (a !== NaN) { // Noncompliant; always true
  console.log("a is not NaN"); // this statement is not necessarily true
}
for (var i = 0; i < strings.length; i--) {
  console.log("dead code")
}
if (str == null && str.length == 0) {
  console.log("String is empty");
}
liumiaocn:sonar liumiao$`
## sonar-project.properties设定文件
项目设定文件信息详细如下：
`liumiaocn:sonar liumiao$ cat sonar-project.properties 
sonar.projectKey=javascript-prj
sonar.projectName=JavaScript Demo Project
sonar.projectVersion=1.0
sonar.sources=src
sonar.host.url=http://127.0.0.1:32003
sonar.login=admin
sonar.password=admin
liumiaocn:sonar liumiao$`
# 执行sonar-scanner
`liumiaocn:sonar liumiao$ pwd
/Users/liumiao/sonar
liumiaocn:sonar liumiao$ ls
sonar-project.properties src
liumiaocn:sonar liumiao$ sonar-scanner
INFO: Scanner configuration file: /Users/liumiao/Desktop/sonar/sonar-scanner-3.2.0.1227-macosx/conf/sonar-scanner.properties
INFO: Project root configuration file: /Users/liumiao/sonar/sonar-project.properties
INFO: SonarQube Scanner 3.2.0.1227
INFO: Java 1.8.0_121 Oracle Corporation (64-bit)
INFO: Mac OS X 10.14 x86_64
INFO: User cache: /Users/liumiao/.sonar/cache
INFO: SonarQube server 5.6.5
INFO: Default locale: "en_US", source code encoding: "UTF-8" (analysis is platform dependent)
INFO: Load global repositories
INFO: Load global repositories (done) | time=129ms
INFO: User cache: /Users/liumiao/.sonar/cache
INFO: Load plugins index
INFO: Load plugins index (done) | time=3ms
INFO: Process project properties
INFO: Load project repositories
INFO: Load project repositories (done) | time=126ms
INFO: Load quality profiles
INFO: Load quality profiles (done) | time=41ms
INFO: Load active rules
INFO: Load active rules (done) | time=609ms
WARN: SCM provider autodetection failed. No SCM provider claims to support this project. Please use sonar.scm.provider to define SCM of your project.
INFO: Publish mode
INFO: -------------  Scan JavaScript Demo Project
INFO: Load server rules
INFO: Load server rules (done) | time=73ms
INFO: Base dir: /Users/liumiao/sonar
INFO: Working dir: /Users/liumiao/sonar/.scannerwork
INFO: Source paths: src
INFO: Source encoding: UTF-8, default locale: en_US
INFO: Index files
INFO: 1 files indexed
INFO: Quality profile for js: Sonar way
INFO: JaCoCoSensor: JaCoCo report not found : /Users/liumiao/sonar/target/jacoco.exec
INFO: JaCoCoItSensor: JaCoCo IT report not found: /Users/liumiao/sonar/target/jacoco-it.exec
INFO: Sensor Lines Sensor
INFO: Sensor Lines Sensor (done) | time=11ms
INFO: Sensor JavaScriptSquidSensor
INFO: 1 source files to be analyzed
INFO: Sensor JavaScriptSquidSensor (done) | time=200ms
INFO: 1/1 source files have been analyzed
INFO: Sensor SCM Sensor
INFO: No SCM system was detected. You can use the 'sonar.scm.provider' property to explicitly specify it.
INFO: Sensor SCM Sensor (done) | time=0ms
INFO: Sensor org.sonar.plugins.javascript.lcov.UTCoverageSensor
INFO: Sensor org.sonar.plugins.javascript.lcov.UTCoverageSensor (done) | time=0ms
INFO: Sensor org.sonar.plugins.javascript.lcov.ITCoverageSensor
INFO: Sensor org.sonar.plugins.javascript.lcov.ITCoverageSensor (done) | time=0ms
INFO: Sensor Zero Coverage Sensor
INFO: Sensor Zero Coverage Sensor (done) | time=7ms
INFO: Sensor Code Colorizer Sensor
INFO: Sensor Code Colorizer Sensor (done) | time=0ms
INFO: Sensor CPD Block Indexer
INFO: DefaultCpdBlockIndexer is used for js
INFO: Sensor CPD Block Indexer (done) | time=20ms
INFO: Calculating CPD for 1 files
INFO: CPD calculation finished
INFO: Analysis report generated in 53ms, dir size=13 KB
INFO: Analysis reports compressed in 17ms, zip size=6 KB
INFO: Analysis report uploaded in 29ms
INFO: ANALYSIS SUCCESSFUL, you can browse http://127.0.0.1:32003/dashboard/index/javascript-prj
INFO: Note that you will be able to access the updated dashboard once the server has processed the submitted analysis report
INFO: More about the report processing at http://127.0.0.1:32003/api/ce/task?id=AWcNw2JuTV5bsL-6UV7v
INFO: ------------------------------------------------------------------------
INFO: EXECUTION SUCCESS
INFO: ------------------------------------------------------------------------
INFO: Total time: 3.719s
INFO: Final Memory: 19M/278M
INFO: ------------------------------------------------------------------------
liumiaocn:sonar liumiao$`
# 确认结果
代码扫描整体结果
![在这里插入图片描述](https://img-blog.csdnimg.cn/20181225055300741.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L2xpdW1pYW9jbg==,size_16,color_FFFFFF,t_70)
详细代码级别的扫描结果展示
![在这里插入图片描述](https://img-blog.csdnimg.cn/20181225055310882.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L2xpdW1pYW9jbg==,size_16,color_FFFFFF,t_70)

