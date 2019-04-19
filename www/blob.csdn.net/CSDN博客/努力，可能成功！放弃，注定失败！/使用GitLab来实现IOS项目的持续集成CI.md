# 使用GitLab来实现IOS项目的持续集成CI - 努力，可能成功！放弃，注定失败！ - CSDN博客
置顶2016年11月21日 13:37:19[上天眷顾我](https://me.csdn.net/qq_30513483)阅读数：1264
作为程序员，代码是一定要写的，而且要天天写。在好多地方见过这样一种说法：
> 
只会写程序的程序员不是好程序员
当然，我不赞同这种观点，因为有的人他天生就是为程序而生的。但是掌握一些代码之外的理论知识也是一个不错的选择，它能让你的代码质量上一个新的台阶，能极大的提高你的“抠码”效率。
最近新的APP即将上线，但在产品、研发、运营几个环节出了一些问题，也让我静下心来思考一些一个程序员觉得很难直面的问题。这其中和技术相关的问题包括：
- 
APP版本更新较为频繁的问题。
- 
测试覆盖不到位的问题。
APP的开发可能跟以前我所经历的大部分企业级应用软件、IaaS\/PaaS平台、大数据平台相关产品完全的不同，为了响应市场需求，APP的迭代周期要以周为单位（打开手机看看常用 的APP大部分每周一更新）,IOS还受到苹果审核周期的影响，会有更多的不可控因素。为了解决这些问题，目前我们主要做的工作包括几个方面：
- 
解决运营和产品环节的反馈机制问题。
- 
优化APP的打包、测试的方式。
- 
合理的制定版本发布计划。
今天想给技术人员分享的主要就是持续集成和交付/部署（CI/CD）方面的一些基础知识，同时结合我们现在正在开发的APP中遇到的一些问题，和大家共同探讨如何才能更好的优化我们的产品打包、测试过程。
## 持续集成和持续交付/部署 CI/CD
> 
Continuous Integration (CI) is a development practice that requires developers to integrate code into a shared repository several times a day. Each check-in is then verified by an automated build, allowing teams to detect problems early.
By integrating regularly, you can detect errors quickly, and locate them more easily.
上面对CI的定义参考 [thoughtworks](https://www.thoughtworks.com/cn/continuous-integration)，可能把它生硬的翻译过来也不太好理解，我从这些年的产品研发过程中见到过的各种CI工具来定义CI的话，可以做如下定义：
> 
CI是软件（产品）研发生命周期中对代码质量、系统集成的一个持续构进的过程，当作为一个团队开发产品时，每个人都要开发自己的功能模块，最终都需要集成在一起，代码也需要集中托管到同一个地方，通过使用一些自动化的代码打包、测试工具，能够在开发人员每提交一次代码的时候，系统自动对程序进行打包和单元测试，如果出现问题，及时通过邮件等方式通知相关的开发人员。
5年前，我还在从事企业级应用软件的开发，那时根本就没有什么单元测试，更没有自动打包、测试的工具，也就大家开发完功能，提交到SVN服务器，还有一个专业的人在需要打包里把代码down下来在自己机器上打包，然后部署到测试环境跑一把，没什么大问题就上生产环境。现在想想还是真“土”。
这几年，我和我的团队主要是基于openstack开发一款企业级的云管理平台，从一开始的懵懵懂懂，到现在形成比较成熟的基于CI的整个开发流程，也爬过了一个又一个的坑和坎。
目前我们比较成熟的套路是： *git+gerrit+jenkins *
其中git用于代码托管，gerrit主要是是代码的评审，jenkins用于打包和跑单元测试。这样的组合百度一下，会有很多先辈们已经写了很多博客和文章，我这里就不在烂费精力了。今天我想介绍一下的是目前我们的APP开发过程中想采用的gitlab。要不然大家都觉得我写的东西没干货，也不好意思啊。
## GitLab
> 
GitLab是一个集代码管理、测试、代码部署于一体的开源应用程序，它包括一个有非常好的权限控制的Git版本管理系统，代码评审系统，问题跟踪系统，活动反馈系统，wiki，持续集成CI系统等。
*上面的翻译可能有点别扭，没办法，我就这水平了，咱们直接进入正题吧。*
我个人比较喜欢的就是gitlab长的太像github了，使用很方便，同时采用了github的pull request方式来对代码进行审核，还集成了CI（虽然大部分还是要看你写脚本的水平如何）。
我将通过使用一个ios的demo项目来给大家展示如何用gitlab来管理代码、进行代码审核、CI等等。
### 安装GitLab
在Docker盛行的今天，我也就偷偷懒，跑个docker实例给大家演示一下，如果您在打算在生产环境使用gitlab，可以参考官方的安装文档，包括一些集群方案都能找到。用Docker跑gitlab就一个命令：
**这里假设我们的服务器IP是：47.88.21.77 一定要记得根据您的实际情况更改IP地址哦。**
```cpp
```cpp
sudo docker run --detach --hostname gitlab.example.com --env GITLAB_OMNIBUS_CONFIG="external_url 'http://47.88.21.77/'; gitlab_rails['lfs_enabled'] = true;" --publish 443:443 --publish 80:80 --publish 22:22 --name gitlab --restart always --volume /srv/gitlab/config:/etc/gitlab --volume /srv/gitlab/logs:/var/log/gitlab --volume /srv/gitlab/data:/var/opt/gitlab gitlab/gitlab-ce:latest
```
```
服务启动后，浏览器进入：[http://47.88.21.77/](http://47.88.21.77/)
![](http://upload-images.jianshu.io/upload_images/1464256-68faa012bbcfeae7.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/1240)
gitlab_login.png
首次进入可以设置root的密码，注册用户即可进入系统，这里我们演示时使用的用户名是cjzhao，登录后如下图所示：
![](http://upload-images.jianshu.io/upload_images/1464256-8454bd3a54dc1c42.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/1240)
gitlab_firstpage.png
空空如也……
### 使用gitlab评审代码
在这里可以创建组织、项目，基本概念和github一样，这里我们直接创建我们要演示的项目ios_ci_demo。
![](http://upload-images.jianshu.io/upload_images/1464256-f1076b300e19df2a.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/1240)
gitlab_newproject.png
OK，多的不解释了，一看就懂。
![](http://upload-images.jianshu.io/upload_images/1464256-d61fdb54a72ed56a.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/1240)
gitlab_projecthome.png
创建完成后，进入项目首页，看到黄色的提示信息了吧，为了能从远程把代码推到服务器，需要配置一个ssh公钥，点击add an SSH key把你自己的公钥添加完成，就可以把代码上传上去了，这个过程就不截图了哦，看到我的公钥也不是太好……呵呵。
添加完公钥后，回到项目首页，如下图所示：
![](http://upload-images.jianshu.io/upload_images/1464256-7adb93c0e9d4adf9.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/1240)
gitlab_codeuploadmethod.png
我们根据第三种方法把现有的代码上传到gitlab，即执行下面的代码：
```
```
cd existing_folder
git init
git remote add origin git@47.88.21.77:cjzhao/ios_ci_demo.git
git add .
git commit
git push -u origin master
```
```
我们的app很简单，如下图所示:
![](http://upload-images.jianshu.io/upload_images/1464256-8c7941da91c4a188.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/1240)
Simulator_hello.png
很明显， word是错的哦，后面我们将演示另一个用户修改完后提交代码，并做代码审核。
上传完代码后，进入项目首页，如下图所示：
![](http://upload-images.jianshu.io/upload_images/1464256-aa7376bb247fd97a.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/1240)
gitlab_project_home_first.png
再注册一个新用户jack，然后用cjzhao登录后，进入项目设置页面(如下图所示)，把jack增加到项目中，这样他就能提交代码了。
![](http://upload-images.jianshu.io/upload_images/1464256-42459ad6349fba64.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/1240)
gitlab_project_setting_menu.png
添加的过程自己研究吧，很简单，要不编辑会认为我的文章就靠截图凑数了。加完后如下图所示：
![](http://upload-images.jianshu.io/upload_images/1464256-3ef3c3a68b74a67e.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/1240)
gitlab_add_user_to_project.png
OK了，接下来用用户jack修改代码，并提交到git服务器。
jack的操作主要包括以下几个步骤：
```cpp
```cpp
git clone git@47.88.21.77:cjzhao/ios_ci_demo.git
git checkout -b wordbug
//将word改为world
git add .
git commit -m "change word to world"
git push origin wordbug
```
```
这时jack登录系统后，会看到提交的代码，同时系统会提示您创建一个Merge request,如下图所示：
![](http://upload-images.jianshu.io/upload_images/1464256-4fc9c822e3fa460b.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/1240)
gitlab_create_merge_request.png
点击Create Merge Request，填写一些修改内容的描述信息，请求cjzhao将代码合并到master即可提交。
cjzhao登录系统后就能看到jack请求合并的分支，如下图所示：
![](http://upload-images.jianshu.io/upload_images/1464256-87291dfc2a31594f.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/1240)
gitlab_accept_merge_request.png
在完成代码评审后，点击Accept Merge Request即可将代码合并到master。选中Remove source branch同时将wordbug分支删除。
OK，这就完成了多开发人员合作的项目代码评审流程。
### gitlab中的CI
在gitlab中完成持续集成CI包括两个操作：
- 
配置一个Runner(用来编译、测试、打包的服务器节点)。
- 
在项目根目录增加[YAML](https://en.wikipedia.org/wiki/YAML)格式的CI脚本文件`.gitlab-ci.yml`。
#### 配置Runner
我们首先来为我们的项目配置一个Runner，由于我们的项目是ios的，因此需要在安装了macos操作系统和xcode的环境下才能编译、打包我们的APP，因此我们需要将一台mac计算机配置成我们的一个Runner，基本原理就是在Mac上安装一个代理程序gitlab-ci-multi-runner，然后将mac注册到gitlab服务器端，然后这台mac机器就能接收到gitlab服务器下发的CI任务，完成相应的编译、测试、打包等工作，然后将结果反馈给gitlab服务器。
在一台Mac机器上执行如下命令安装gitlab-ci-multi-runner(可能需要翻墙才能装哦):
```
sudo curl --output /usr/local/bin/gitlab-ci-multi-runner https://gitlab-ci-multi-runner-downloads.s3.amazonaws.com/latest/binaries/gitlab-ci-multi-runner-darwin-amd64
sudo chmod +x /usr/local/bin/gitlab-ci-multi-runner
```
进入项目的Runner配置页面，如下图所示：
![](http://upload-images.jianshu.io/upload_images/1464256-2cded617fcb621a0.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/1240)
gitlab_config_runner.png
在Mac机器上执行如下命令，将这台Mac注册到gitlab并绑定到我们示例项目。
```bash
gitlab-ci-multi-runner register
#Please enter the gitlab-ci coordinator URL (e.g. https://gitlab.com/ci):
#输入上图中的URL.
#Please enter the gitlab-ci token for this runner:
#输入上图中的token.
#Please enter the gitlab-ci description for this runner:
#输入一个描述信息，这里我们输入mac_runner
#Please enter the gitlab-ci tags for this runner (comma separated):
#输入一些标签，这里我们输入"mac,xcode7.1"
# Registering runner... succeeded runner=euasz2j9 
#Please enter the executor: docker-ssh+machine, docker, docker-ssh, parallels, shell, ssh, virtualbox, docker+machine:
#这里我们输入shell，因为ios项目的编译、测试、打包我们都采用脚本来执行。
#Runner registered successfully. Feel free to start it, but if it's running already the config should be automatically reloaded!
#注册成功，接下来启动它
gitlab-ci-multi-runner install 
gitlab-ci-multi-runner start
```
现在我们的Mac机器就注册为一个Runner了，查看项目的Runner页面，如下图所示：
![](http://upload-images.jianshu.io/upload_images/1464256-6f191d7b6226941f.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/1240)
gitlab_runner_running.png
我们的Runner就成功注册上了。接下来就可以编写CI的脚本了。
#### 增加CI脚本文件`.gitlab-ci.yml`
在项目根目录创建`.gitlab-ci.yml`文件，内容如下：
```bash
stages:
  - build
  - archive
build_project:
  stage: build
  script: 
    - xctool -project ioscidemo.xcodeproj -scheme ioscidemo clean
    - xctool -project ioscidemo.xcodeproj -scheme ioscidemo test -test-sdk iphonesimulator9.3
archive_project:
  stage: archive
  script: 
    - xctool -project ioscidemo.xcodeproj -scheme ioscidemo archive -archivePath build/ioscidemo
    - xcodebuild -exportArchive -exportFormat ipa -archivePath "build/ioscidemo.xcarchive" -exportPath "build/ioscidemo.ipa"
  only: 
    - master
  artifacts:
    paths:
      - build/ioscidemo.ipa
```
上面脚本使用了[xctool](https://github.com/facebook/xctool)，它是facebook推出的一款替代xcodebuild的app打包和测试工具，它日志输出更加友好，性能高效，我们需要在刚才安装了Runner的Mac机器上先安装它，可以使用如下命令安装：
``brew install xctool``
OK,接下来就是提交我们最新的代码了：
```
```
git add .
git commit -m "add CI cfg file"
git push origin master
```
```
如果没有什么异常的话，CI已经在开始执行编译、测试、打包等工作了。见下图所示：
![](http://upload-images.jianshu.io/upload_images/1464256-d9f5709fc9471826.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/1240)
gitlab_project_build_status.png
看到绿色的`passed`了吧？这就说明编译、测试、打包都成功了（我可是耗费了好长时间才成功的哦，中间总会出现各种各样的问题）。就写到这儿吧，最后再给大家提供一点CD的思路（技术层面的），刚才的过程已经完成了app的打包，我们可以创建一个Releases的分支，在脚本中增加APP上传到itunesconnect的脚本，这样就可以在发布新版本的时候完成APP的整个发布流程。
Apple给我们提供了一套命令行工具altool可以直接把ipa上传到itunesconnect，很方便的，详见：[用 altool 上传您的应用程序二进制文件](http://help.apple.com/itc/apploader/#/apdATD1E53-D1E1A1303-D1E53A1126)
OK，真的结束了，自己去研究吧。
希望本文对您有所帮助，谢谢。
CJ推荐：
[程序员的编辑器-VIM(爱就是爱)](http://www.jianshu.com/p/216811be226b)
[向开源社区贡献您的代码](http://www.jianshu.com/p/901d938368f6)
[在github上写博客](http://www.jianshu.com/p/1260517bbedb)
[DevOps是什么东东？](http://www.jianshu.com/p/ac3eaea47f75)
[js依赖管理工具bower](http://www.jianshu.com/p/8dd992f6ef44)
[JS模块化编程-requirejs](http://www.jianshu.com/p/349921b16415)
文／cjzhao（简书作者）
原文链接：http://www.jianshu.com/p/c840632cef38
著作权归作者所有，转载请联系作者获得授权，并标注“简书作者”。
