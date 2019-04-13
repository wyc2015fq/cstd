
# 前端Javascript代码质量扫描解决方案：Karma + Jasmine + lcov + Sonarqube - 知行合一 止于至善 - CSDN博客

2018年12月27日 21:58:24[liumiaocn](https://me.csdn.net/liumiaocn)阅读数：1127


代码质量扫描的流行工具sonarqube，java系的应用开发框架可以使用Maven/Gradle作为构建工具，JUnit作为单体覆盖率测试工具，使用Jacoco可视化提供支撑。而Javascript则可以使用Karma+Jasmine+lcov+Sonarqube进行类似的质量扫描。
# 环境准备&说明
## node版本
事前安装node，本文示例中使用如下版本
`liumiaocn:~ liumiao$ npm -v
5.5.1
liumiaocn:~ liumiao$ node -v
v8.9.1
liumiaocn:~ liumiao$`
## karma安装与介绍
### 安装karma
使用npm install -g karma进行karma的安装
`liumiaocn:~ liumiao$ npm set registry="https://registry.npm.taobao.org/"
liumiaocn:~ liumiao$ npm install -g karma
... 
+ karma@3.1.4
added 328 packages in 13.582s
liumiaocn:~ liumiao$`
### 版本确认
`liumiaocn:~ liumiao$ karma --version
Karma version: 3.1.4
liumiaocn:~ liumiao$`
### karma简介
Karma是google为AngularJS开发的测试工具，当然后面更名的Angular同样在使用。除了Angular的应用之外，普通的Javascript程序也可以使用Karma。Karma在使用的时候通过Karma start启动服务器端应用（缺省使用9876端口），而客户端使用浏览器与之建立连接。服务器端应用会监视源码或测试代码的更新，而无需手动刷新浏览器，通过类似的改进，使用karma可以快速的在不同的浏览器中进行自动化单元测试。
### 安装浏览器启动器插件
karma在启动服务器的时候，可以同时通过浏览器启动器插件启动浏览器，而这些浏览器和服务器应用建立连接。
|浏览器|浏览器启动器名称|
|---|---|
|FIREFOX|karma-firefox-launcher|
|CHROME|karma-chrome-launcher|
安装启动器插件(chrome)：npm install -g karma-chrome-launcher
`liumiaocn:~ liumiao$ npm install -g karma-chrome-launcher
+ karma-chrome-launcher@2.2.0
added 5 packages in 1.546s
liumiaocn:~ liumiao$`浏览器可执行文件目录的设定：
通过设定 浏览器名称_BIN来进行环境变量的设定
|浏览器|环境变量|
|---|---|
|FIREFOX浏览器|FIREFOX_BIN|
|CHROME浏览器|CHROME_BIN|
### 安装测试框架适配器
Karma支持Jasmine/Mocha等多种测试框架，比如安装Jasmine适配器：
npm install -g karma-jasmine
`liumiaocn:~ liumiao$ npm install -g karma-jasmine
...省略
+ karma-jasmine@2.0.1
added 2 packages in 1.2s
liumiaocn:~ liumiao$`基于BDD（行为驱动开发）的Jasmine是一个JS的测试框架框架。使用Jasmine可以快速地进行测试代码的开发。
### 安装测试报告插件
通过不同报告插件，比如JUnit格式和HTML格式：
|格式|插件名称|
|---|---|
|JUnit格式|karma-junit-reporter|
|HTML格式|karma-html-reporter|
npm install -g karma-html-reporter
npm install -g karma-junit-reporter
`liumiaocn:~ liumiao$ npm install -g karma-html-reporter
...省略
+ karma-html-reporter@0.2.7
added 3 packages in 1.455s
liumiaocn:~ liumiao$ npm install -g karma-junit-reporter
...省略
+ karma-junit-reporter@1.2.0
added 3 packages in 1.349s
liumiaocn:~ liumiao$`
### 安装karma-coverage
`liumiaocn:~ liumiao$ npm install -g karma-coverage
+ karma-coverage@1.1.2
added 95 packages in 9.97s
liumiaocn:~ liumiao$`
# 创建karma例程
## 用karma init生成karma.conf.js
使用karma init创建简单示例用的测试的设定文件karma.conf.js
`liumiaocn:~ liumiao$ mkdir karma
liumiaocn:~ liumiao$ cd karma
liumiaocn:karma liumiao$ karma init
Which testing framework do you want to use ?
Press tab to list possible options. Enter to move to the next question.
> jasmine
Do you want to use Require.js ?
This will add Require.js plugin.
Press tab to list possible options. Enter to move to the next question.
> no
Do you want to capture any browsers automatically ?
Press tab to list possible options. Enter empty string to move to the next question.
> Chrome
> 
What is the location of your source and test files ?
You can use glob patterns, eg. "js/*.js" or "test/**/*Spec.js".
Enter empty string to move to the next question.
> 
Should any of the files included by the previous patterns be excluded ?
You can use glob patterns, eg. "**/*.swp".
Enter empty string to move to the next question.
> 
Do you want Karma to watch all the files and run the tests on change ?
Press tab to list possible options.
> yes

Config file generated at "/Users/liumiao/karma/karma.conf.js".
liumiaocn:karma liumiao$`karma init只有一个作用，就是生成karma的设定文件，此文件缺省名称为karma.conf.js，可以通过`karma init 指定文件名称`的方式来修改设定文件名称。
具体相关的设定说明：
|步骤|提示|说明|示例设定值|
|---|---|---|---|
|设定项1|Which testing framework do you want to use ?|选择测试框架，比如mocha或者jasmine等|jasmine|
|设定项2|Do you want to use Require.js ?|是否需要引入Require.js|no:不需要|
|设定项3|Do you want to capture any browsers automatically ?|选择测试所使用的浏览器，如果选择多个将会在多个浏览器中进行测试|Chrome|
|设定项4|What is the location of your source and test files ?|支持正则表达式的源码和测试代码的目录设定|未设定（后将手动设定）|
|设定项5|Should any of the files included by the previous patterns be excluded ?|Step 4中需要排除在外的文件（支持正则）|未设定|
|设定项6|Do you want Karma to watch all the files and run the tests on change ?|是否观察文件变化|yes：观察文件变化|
## 构建前端单体测试示例
结合上述生成的文件，整体创建如下结构的组成：
`liumiaocn:karma liumiao$ ls
karma.conf.js            sonar-project.properties src                      test
liumiaocn:karma liumiao$ tree
.
├── karma.conf.js
├── sonar-project.properties
├── src
│   └── cal.js
└── test
    └── cal.spec.js
2 directories, 4 files
liumiaocn:karma liumiao$`详细说明
|文件名|说明|
|---|---|
|karma.conf.js|karma的工程设定文件|
|src/cal.js|javascript源码，一个加减乘除的函数|
|test/cal.spec.js|javascript测试代码，用于测试cal.js|
|sonar-project.properties|sonar-scanner工程设定文件|
## 代码说明：src/cal.js
用于实现加减乘除的函数：
函数名称：cal
参数1: op1（操作数1）
参数2: op2（操作数2）
参数3: optype：类型（加减乘除）
返回值：返回运算的结果
`liumiaocn:karma liumiao$ cat src/cal.js 
function cal(op1,op2,optype){
    if ( optype === 'add' ) {
      return op1 + op2;
    } else if ( optype === 'sub' ) {
      return op1 - op2;
    } else if ( optype === 'mul' ) {
      return op1 * op2;
    } else if ( optype === 'div' ) {
      return op1 / op2;
    }
}
liumiaocn:karma liumiao$`
## 代码说明：test/cal.spec.js
在说明测试代码之前，首先理解一下使用Jasmine进行开发的一些基础概念和Jasmine中的实现：
测试用例集：Test Suite：以函数describe进行封装和实现
测试用例：Test Case：以函数it进行分装和实现，在Jasmine中称为Specs
断言：Asset：以函数expect进行分装和实现，进行计算值和期望值之间是否一致的确认
`liumiaocn:karma liumiao$ cat test/cal.spec.js 
describe('cal test suit',function(){
    it('test cal add',function(){
        var ret = cal(40,2,'add');
        expect(ret).toEqual(42);
    });
    it('test cal mul',function(){
        var ret = cal(21,2,'mul');
        expect(ret).toEqual(42);
    });
});
liumiaocn:karma liumiao$`上面的测试代码可以清楚地看到测试了加法和乘法两个分支。这是一个非常见的的测试实际使用起来会复杂地多，比如常见的describe的嵌套等，由于篇幅的关系，本文只是介绍前端javascript的一个通用的代码质量扫描和测试覆盖率结合的整体的方式，重点在于各个流程的贯通。
### 设定karma.conf.js
前文使用karma init生成了karma.conf.js文件，结合测试的javascript源码和测试代码，将karma.conf.js文件进行设定。
使用karma init中的设定项4(What is the location of your source and test files ?)中设定的内容，可设定源码和测试代码的目录设定，这里做如下设定：
`files: [
      'src/*.js',
      'test/*.spec.js'
    ],`同时设定一下preprocessors，在此处定义的方法，会在文件被浏览器运行前执行。
`preprocessors: {
      'src/*.js' : ['coverage'],
      'test/*.js' : ['coverage']
    },`在Javascript的单体测试中，lcov就像java中的jacoco一样，用于提供测试用例执行之后的相关数据记录，根据这个文件karam可以生成相关的测试覆盖率报告，同样sonarqube也是同样会利用lcov的文件。在karma.conf.js中的coverageReporter进行如下设定，则可在根目录的reports/coverage下生成相关的测试结果文件。
`coverageReporter: {
      type : 'lcov',
      dir : 'reports',
      subdir : 'coverage'
    },`将coverage加入reporters中
`reporters: ['progress','coverage'],`这样就完成了Karma的设定，如此，Karma + Jasmine + lcov的合体就能够正常动作了。
# karma动作确认
使用karma start可以执行上述测试用例并能生成结果文件。 执行方式可以为
> karma start
或者
> karma start karma.conf.js
设定参数
|参数|说明|
|---|---|
|–port|指定服务器运行的端口|
|–auto-watch|自动监控源码文件并基于变更运行|
|–detached|从服务器断开|
|–no-auto-watch|不监控源码文件|
|–log-level|日志级别：disable|
|–colors|报表和日志展示的颜色设定|
|–no-colors|报表和日志展示时不使用颜色|
|–reporters|reporter列表(包括: dots, progress, junit, growl, coverage).|
|–browsers|浏览器列表 (示例： --browsers Chrome,ChromeCanary,Firefox).|
|–capture-timeout|超时时间 [单位：ms].|
|–single-run|运行后推出的方式|
|–no-single-run|不实用single-run方式|
|–fail-on-empty-test-suite|空测试用例集失败|
|–no-fail-on-empty-test-suite|空测试用例集继续执行|
|–fail-on-failing-test-suite|测试用例集失败退出|
|–no-fail-on-failing-test-suite|测试用例集失败不退出|
比如single-run  都是很有用的Option，可以使得karma的服务器端应用执行之后直接退出，可根据具体场景进行使用。
`liumiaocn:karma liumiao$ ls
karma.conf.js            sonar-project.properties src                      test
liumiaocn:karma liumiao$ karma start --single-run
27 12 2018 21:40:08.621:INFO [karma-server]: Karma v3.1.4 server started at http://0.0.0.0:9876/
27 12 2018 21:40:08.623:INFO [launcher]: Launching browsers Chrome with concurrency unlimited
27 12 2018 21:40:08.628:INFO [launcher]: Starting browser Chrome
27 12 2018 21:40:09.568:INFO [Chrome 71.0.3578 (Mac OS X 10.14.0)]: Connected on socket 98YmcgcevgWq06DWAAAA with id 98376372
Chrome 71.0.3578 (Mac OS X 10.14.0): Executed 2 of 2 SUCCESS (0.003 secs / 0.003 secs)
TOTAL: 2 SUCCESS
liumiaocn:karma liumiao$`因为是single-run方式，可以看到一闪而过的chrome浏览器，并在控制台上看见以上信息，提示执行了两个测试用例而且全部成功，然后来确认一下结果。
`liumiaocn:karma liumiao$ ls
karma.conf.js            reports                  sonar-project.properties src                      test
liumiaocn:karma liumiao$ tree reports
reports
└── coverage
    ├── lcov-report
    │   ├── base.css
    │   ├── index.html
    │   ├── prettify.css
    │   ├── prettify.js
    │   ├── sort-arrow-sprite.png
    │   ├── sorter.js
    │   ├── src
    │   │   ├── cal.js.html
    │   │   └── index.html
    │   └── test
    │       ├── cal.spec.js.html
    │       └── index.html
    └── lcov.info
4 directories, 11 files
liumiaocn:karma liumiao$`可以看到生成了上述提到的结果目录以及lcov.info文件， 接下来通过reports/coverage/lcov-report/index.html来确认结果
`liumiaocn:lcov-report liumiao$ pwd
/Users/liumiao/easypack/karma/reports/coverage/lcov-report
liumiaocn:lcov-report liumiao$ ls
base.css              prettify.css          sort-arrow-sprite.png src
index.html            prettify.js           sorter.js             test
liumiaocn:lcov-report liumiao$`整体的文件的测试覆盖状况的整体信息
![在这里插入图片描述](https://img-blog.csdnimg.cn/20181227214510784.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L2xpdW1pYW9jbg==,size_16,color_FFFFFF,t_70)
查看源码文件src/cal.js的测试覆盖整体状况
![在这里插入图片描述](https://img-blog.csdnimg.cn/20181227214520981.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L2xpdW1pYW9jbg==,size_16,color_FFFFFF,t_70)
查看源码文件src/cal.js的代码测试覆盖详细
![在这里插入图片描述](https://img-blog.csdnimg.cn/20181227214529904.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L2xpdW1pYW9jbg==,size_16,color_FFFFFF,t_70)
+ Sonarqube
如果结合sonarqube进行结果的整体确认，可以使用sonar-scanner，而相关的设定信息如下所示：
liumiaocn:karma liumiao$ cat sonar-project.properties 
sonar.projectKey=karma-demo
sonar.projectName=Karma Demo Project
sonar.projectVersion=1.0
sonar.sources=src
sonar.host.url=http://127.0.0.1:32003
sonar.login=admin
sonar.password=admin
sonar.javascript.lcov.reportPath=reports/coverage/lcov.info
liumiaocn:karma liumiao$其他的sonar的设定，因为在sonarquebe的使用方式中多次做过详细介绍，此处不再赘述。而与前端测试的覆盖率展示相关的仅有sonar.javascript.lcov.reportPath而已，sonar.javascript.lcov.reportPath是用与设定lcov的相关文件路径，而在上述文件中也看到过此文件的生成。
执行方式
通过调用sonar-scanner即可（sonar-scanner只需要下载下来，然后将可执行文件放到PATH中即可，所以此处也不再赘述）。
liumiaocn:karma liumiao$ ls
karma.conf.js            reports                  sonar-project.properties src                      test
liumiaocn:karma liumiao$ sonar-scanner
INFO: Scanner configuration file: /Users/liumiao/Desktop/sonar/sonar-scanner-3.2.0.1227-macosx/conf/sonar-scanner.properties
INFO: Project root configuration file: /Users/liumiao/easypack/karma/sonar-project.properties
INFO: SonarQube Scanner 3.2.0.1227
INFO: Java 1.8.0_121 Oracle Corporation (64-bit)
INFO: Mac OS X 10.14 x86_64
INFO: User cache: /Users/liumiao/.sonar/cache
INFO: SonarQube server 5.6.5
INFO: Default locale: "en_US", source code encoding: "UTF-8" (analysis is platform dependent)
INFO: Load global repositories
INFO: Load global repositories (done) | time=127ms
INFO: User cache: /Users/liumiao/.sonar/cache
INFO: Load plugins index
INFO: Load plugins index (done) | time=3ms
INFO: Process project properties
INFO: Load project repositories
INFO: Load project repositories (done) | time=94ms
INFO: Load quality profiles
INFO: Load quality profiles (done) | time=56ms
INFO: Load active rules
INFO: Load active rules (done) | time=325ms
INFO: Publish mode
INFO: -------------  Scan Karma Demo Project
INFO: Load server rules
INFO: Load server rules (done) | time=64ms
INFO: Base dir: /Users/liumiao/easypack/karma
INFO: Working dir: /Users/liumiao/easypack/karma/.scannerwork
INFO: Source paths: src
INFO: Source encoding: UTF-8, default locale: en_US
INFO: Index files
INFO: 1 files indexed
INFO: Quality profile for js: Sonar way
INFO: JaCoCoSensor: JaCoCo report not found : /Users/liumiao/easypack/karma/target/jacoco.exec
INFO: JaCoCoItSensor: JaCoCo IT report not found: /Users/liumiao/easypack/karma/target/jacoco-it.exec
INFO: Sensor Lines Sensor
INFO: Sensor Lines Sensor (done) | time=10ms
INFO: Sensor JavaScriptSquidSensor
INFO: 1 source files to be analyzed
INFO: Sensor JavaScriptSquidSensor (done) | time=127ms
INFO: 1/1 source files have been analyzed
INFO: Sensor SCM Sensor
INFO: SCM provider for this project is: git
INFO: 1 files to be analyzed
INFO: 1/1 files analyzed
INFO: Sensor SCM Sensor (done) | time=218ms
INFO: Sensor org.sonar.plugins.javascript.lcov.UTCoverageSensor
INFO: Analysing /Users/liumiao/easypack/karma/reports/coverage/lcov.info
WARN: Could not resolve 1 file paths in lcov.info, first unresolved path: /Users/liumiao/easypack/karma/test/cal.spec.js
INFO: Sensor org.sonar.plugins.javascript.lcov.UTCoverageSensor (done) | time=13ms
INFO: Sensor org.sonar.plugins.javascript.lcov.ITCoverageSensor
INFO: Sensor org.sonar.plugins.javascript.lcov.ITCoverageSensor (done) | time=0ms
INFO: Sensor Zero Coverage Sensor
INFO: Sensor Zero Coverage Sensor (done) | time=6ms
INFO: Sensor Code Colorizer Sensor
INFO: Sensor Code Colorizer Sensor (done) | time=1ms
INFO: Sensor CPD Block Indexer
INFO: DefaultCpdBlockIndexer is used for js
INFO: Sensor CPD Block Indexer (done) | time=18ms
INFO: Calculating CPD for 1 files
INFO: CPD calculation finished
INFO: Analysis report generated in 55ms, dir size=12 KB
INFO: Analysis reports compressed in 15ms, zip size=5 KB
INFO: Analysis report uploaded in 31ms
INFO: ANALYSIS SUCCESSFUL, you can browse http://127.0.0.1:32003/dashboard/index/karma-demo
INFO: Note that you will be able to access the updated dashboard once the server has processed the submitted analysis report
INFO: More about the report processing at http://127.0.0.1:32003/api/ce/task?id=AWcYZ67mTV5bsL-6UV8A
INFO: ------------------------------------------------------------------------
INFO: EXECUTION SUCCESS
INFO: ------------------------------------------------------------------------
INFO: Total time: 3.544s
INFO: Final Memory: 21M/279M
INFO: ------------------------------------------------------------------------
liumiaocn:karma liumiao$结果确认：整体扫描结果与测试覆盖率
![在这里插入图片描述](https://img-blog.csdnimg.cn/20181227215258638.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L2xpdW1pYW9jbg==,size_16,color_FFFFFF,t_70)
结果确认：代码粒度测试覆盖详细
![在这里插入图片描述](https://img-blog.csdnimg.cn/20181227215347347.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L2xpdW1pYW9jbg==,size_16,color_FFFFFF,t_70)
需要注意的是代码行数的计算和覆盖率的细节，两个工具略有不同，虽然影响不大，但是这是需要意识到的一个小细节。
源码
示例用的整体代码和生成的文件上传至github的easypack中，目录如下：
[https://github.com/liumiaocn/easypack/tree/master/karma](https://github.com/liumiaocn/easypack/tree/master/karma)
总结
这样结合karma和sonarqube，结合一个简单的javascript示例，重点介绍了前端应用的代码扫描和测试覆盖率确认的简单方式。

