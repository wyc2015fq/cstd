# 用 Jenkins 搭建 iOS/Android 持续集成打包平台 - 文章 - 伯乐在线
本文作者： [伯乐在线](http://blog.jobbole.com) - [debugtalk](http://www.jobbole.com/members/debugtalk) 。未经作者许可，禁止转载！
欢迎加入伯乐在线 [专栏作者](http://blog.jobbole.com/99322)。
## 背景描述
根据项目需求，现要在团队内部搭建一个统一的打包平台，实现对iOS和Android项目的打包。而且为了方便团队内部的测试包分发，希望在打包完成后能生成一个二维码，体验用户（产品、运营、测试等人员）通过手机扫描二维码后就能直接安装测试包。
该需求具有一定的普遍性，基本上所有开发APP的团队都可能会用到，因此我将整个需求实现的过程整理后形成此文，并且真正地做到了`零基础上手，到手即飞、开箱即用`，希望能对大家有所帮助。[**GitHub地址**](https://github.com/debugtalk/JenkinsTemplateForApp)
首先，先给大家展示下平台建设完成后的整体效果：
![](http://debugtalk.com/assets/images/Jenkins_Job_Overview.jpg)![](http://debugtalk.com/assets/images/Jenkins_Job_Build_View.jpg)
该平台主要实现的功能有3点：
- 定期对GitHub仓库进行检测，若有更新则自动执行构建打包；
- 构建成功后根据ipa/apk生成二维码，并可在历史构建列表中展示各个版本的二维码，通过手机扫描二维码可直接安装对应版本；
- 在构建结果页面中展示当次构建的成果物（Artifact，如`.ipa`、`.app`、`.apk`、`info.plist`等文件），供有需要的用户进行下载。
接下来，本文就开始对平台建设的完整实现过程进行详细介绍。
## 安装Jenkins
Jenkins依赖于Java运行环境，因此需要首先安装[Java](http://www.oracle.com/technetwork/java/javase/downloads/index.html)。
安装Jenkins的方式有多种，可以运行对应系统类型的安装包，可以通过docker获取镜像，也可以直接运行`war`包。
我个人倾向于直接运行`war`包的形式，只需下载`jenkins.war`后，运行如下命令即可启动Jenkins。


```
$ nohup java -jar jenkins_located_path/jenkins.war --httpPort=88 &
```
如果不指定`httpPort`，Jenkins的默认端口为8080。
## Jenkins插件
Jenkins有非常多的插件，可以实现各种功能的扩展。
针对搭建的iOS/Android持续集成打包平台，我使用到了如下几个插件。
- GIT plugin
- SSH Credentials Plugin
- Git Changelog Plugin: 获取仓库提交的commit log
- build-name-setter：用于修改Build名称
- description setter plugin：用于在修改Build描述信息，在描述信息中增加显示QRCode（二维码）
- Post-Build Script Plug-in：在编译完成后通过执行脚本实现一些额外功能
- Xcode integration: iOS专用（可选）
- Gradle plugin: Android专用（可选）
安装方式也比较简单，直接在Jenkins的插件管理页面搜索上述插件，点击安装即可。
## 创建项目（Job）
在Jenkins中，构建项目以Job的形式存在，因此需要针对每个项目创建一个Job。有时候，一个项目中可能有多个分支同时在进行开发，为了分别进行构建，也可以针对每个分支创建一个Job。
创建Job的方式有多种，本次只需要创建`Freestyle project`类型的即可。
> 
`Main page` -> `New Item` -> `Freestyle project`
对于一个持续集成打包平台，每次打包都由4步组成：触发构建、拉取代码、执行构建、构建后处理。对应的，在每个Job中也对应了这几项的配置。
## 配置Git代码仓库
要对项目进行构建，配置项目的代码仓库是必不可少的。由于当前我们的项目托管在GitHub私有仓库中，因此在此需要对`Git`进行配置。
在`【Source Code Management】`配置栏目下，如果之前`GIT plugin`安装成功，则会出现`Git`选项。
配置Git代码仓库时，有三项是必须配置的：仓库URL地址（`Repository URL`）、仓库权限校验方式（`Credentials`），以及当前Job需要构建的代码分支（`Branches to build`）。
在配置`Repository URL`时，选择`HTTPS URL`或`SSH URL`均可。不过需要注意的是，`Credentials`要和`Repository URL`对应，也就是说：
- 如果`Repository URL`是`HTTPS URL`形式的，那么`Credentials`就要采用GitHub用户名密码的校验方式；而且，如果在GitHub中开启了`2FA（two-factor authentication）`，那么还需要在GitHub中创建一个`Personal access token`，输入密码时将这个`Personal access token`作为密码进行输入。
- 如果`Repository URL`是`SSH URL`形式的，那么就需要先在Jenkins所在的服务器上创建一个`SSH`秘钥对，并将公钥添加到GitHub的`SSH keys`中，然后在填写`Credentials`时，选择`SSH Username with private key`的校验方式，填入GitHub Username、SSH私钥、以及创建`SSH`秘钥对时设置的`Passphrase`。
如果对Git权限校验的概念还比较模糊，可以参考《深入浅出Git权限校验》。
在配置`Branches to build`时，可以采用多种形式，包括分支名称（`branchName`）、`tagName`、`commitId`等。其中分支名称的形式用的最多，例如，若是构建`master`分支，则填写`refs/heads/master`，若是构建`develop`分支，则填写`refs/heads/develop`。
除了以上关于Git的必填配置项，有时根据项目的实际情况，可能还需要对Jenkins的默认配置项进行修改。
比较常见的一种情况就是对`clone`的配置进行修改。
在Jenkins的默认配置中，`clone`代码时会拉取所有历史版本的代码，而且默认的超时时限只有10分钟。这就造成在某些项目中，由于代码量本身就比较大，历史版本也比较多，再加上网络环境不是特别好，Jenkins根本没法在10分钟之内拉取完所有代码，超时后任务就会被自动终止了（错误状态码143）。
这种问题的解决方式也很简单，无非就是两种思路，要么少拉取点代码（不获取历史版本），要么提高超时时限。对应的配置在`Advanced clone behaviours`中：
- `Shallow clone`：勾选后不获取历史版本；
- `Timeout (in minutes) for clone and fetch operation`：配置后覆盖默认的超时时限。
## 配置构建触发器
代码仓库配置好了，意味着Jenkins具有了访问GitHub代码仓库的权限，可以成功地拉取代码。
那Jenkins什么时候执行构建呢？
这就需要配置构建触发策略，即构建触发器，配置项位于`【Build Triggers】`栏目。
触发器支持多种类型，常用的有：
- 定期进行构建（Build periodically）
- 根据提交进行构建（Build when a change is pushed to GitHub）
- 定期检测代码更新，如有更新则进行构建（Poll SCM）
构建触发器的选择为复合选项，若选择多种类型，则任一类型满足构建条件时就会执行构建工作。如果所有类型都不选择，则该`Jenkins Job`不执行自动构建，但可通过手动点击`【Build Now】`触发构建。
关于定时器（Schedule）的格式，简述如下：
`MINUTE HOUR DOM MONTH DOW`
- MINUTE: Minutes within the hour (0-59)
- HOUR: The hour of the day (0-23)
- DOM: The day of the month (1-31)
- MONTH: The month (1-12)
- DOW: The day of the week (0-7) where 0 and 7 are Sunday.
通常情况下需要指定多个值，这时可以采用如下operator（优先级从上到下）：
- `*`适配所有有效的值，若不指定某一项，则以`*`占位；
- `M-N`适配值域范围，例如7-9代表7/8/9均满足；
- `M-N/X`或`*/X`：以X作为间隔；
- `A,B,C`：枚举多个值。
另外，为了避免多个任务在同一时刻同时触发构建，在指定时间段时可以配合使用`H`字符。添加`H`字符后，Jenkins会在指定时间段内随机选择一个时间点作为起始时刻，然后加上设定的时间间隔，计算得到后续的时间点。直到下一个周期时，Jenkins又会重新随机选择一个时间点作为起始时刻，依次类推。
为了便于理解，列举几个示例：
- `H/15 * * * *`：代表每隔15分钟，并且开始时间不确定，这个小时可能是`:07,:22,:37,:52`，下一个小时就可能是`:03,:18,:33,:48`；
- `H(0-29)/10 * * * *`：代表前半小时内每隔10分钟，并且开始时间不确定，这个小时可能是`:04,:14,:24`，下一个小时就可能是`:09,:19,:29`；
- `H 23 * * 1-5`：工作日每晚23:00至23:59之间的某一时刻；
## 配置构建方式
触发策略配置好之后，Jenkins就会按照设定的策略自动执行构建。但如何执行构建操作，这还需要我们通过配置构建方式来进行设定。
常用的构建方式是根据构建对象的具体类型，安装对应的插件，然后采用相应的构建方式。例如，若是构建`Android`应用，安装`Gradle plugin`之后，就可以选择`Invoke Gradle script`，然后采用`Gradle`进行构建；若是构建`iOS`应用，安装`Xcode integration`插件之后，就可以选择`Xcode`，然后选择`Xcode`进行构建。
该种方式的优势是操作简单，UI可视化，在场景不复杂的情况下可以快速满足需求。不过缺点就是依赖于插件已有的功能，如果场景较复杂时可能单个插件还无法满足需求，需要再安装其它插件。而且，有些插件可能还存在一些问题，例如对某些操作系统版本或XCode版本兼容不佳，出现问题时我们就会比较被动。
我个人更倾向于另外一种方式，就是自己编写打包脚本，在脚本中自定义实现所有的构建功能，然后在`Execute Shell`中执行。这种方式的灵活度更高，各种场景的构建需求都能满足，出现问题后也能自行快速修复。
另外，对于iOS应用的构建，还有一个需要额外关注的点，就是开发者证书的配置。
如果是采用`Xcode integration`插件进行构建，配置会比较复杂，需要在Jenkins中导入开发证书，并填写多个配置项。不过，如果是采用打包脚本进行构建的话，情况就会简单许多。只要在Jenkins所运行的计算机中安装好开发者证书，打包命令在Shell中能正常工作，那么在Jenkins中执行打包脚本也不会有什么问题。
## 构建后处理
完成构建后，生成的编译成果物（ipa/apk）会位于指定的目录中。但是，如果要直接在手机中安装`ipa/apk`文件还比较麻烦，不仅在分发测试包时需要将好几十兆的安装包进行传送，体验用户在安装时也还需要通过数据线将手机与计算机进行连接，然后再使用PP助手或豌豆荚等工具进行安装。
当前比较优雅的一种方式是借助`蒲公英（pgyer）`或`fir.im`等平台，将`ipa/apk`文件上传至平台后由平台生成二维码，然后只需要对二维码链接进行分发，体验用户通过手机扫描二维码后即可实现快速安装，效率得到了极大的提升。
### 上传安装包文件，生成二维码
不管是`蒲公英`还是`fir.im`，都有对应的Jenkins插件，安装插件后可以在`Post-build`中实现对安装包的上传。
除了使用Jenkins插件，`fir.im`还支持命令上传的方式，`蒲公英`还支持`HTTP Post`接口上传的方式。
我个人推荐采用命令或接口上传的方法，并在构建脚本中进行调用。灵活是一方面，更大的好处是如果上传失败后还能进行重试，这在网络环境不是很稳定的情况下极其必要。
Jenkins成功完成安装包上传后，`pgyer/fir.im`平台会生成一个二维码图片，并在响应中将图片的URL链接地址进行返回。
### 展示二维码图片
二维码图片的URL链接有了，那要怎样才能将二维码图片展示在Jenkins项目的历史构建列表中呢？
这里需要用到另外一个插件，`description setter plugin`。安装该插件后，在`【Post-build Actions】`栏目中会多出`description setter`功能，可以实现构建完成后设置当次build的描述信息。这个描述信息不仅会显示在build页面中，同时也会显示在历史构建列表中。
有了这个前提，要将二维码图片展示在历史构建列表中貌似就可以实现了，能直观想到的方式就是采用`HTML`的`img`标签，将`<img url='qr_code_url'>`写入到build描述信息中。
这个方法的思路是正确的，不过这么做以后并不会实现我们预期的效果。
这是因为Jenkins出于安全的考虑，所有描述信息的`Markup Formatter`默认都是采用`Plain text`模式，在这种模式下是不会对build描述信息中的HTML编码进行解析的。
要改变也很容易，`Manage Jenkins` -> `Configure Global Security`，将`Markup Formatter`的设置更改为`Safe HTML`即可。
更改配置后，我们就可以在build描述信息中采用`HTML`的`img`标签插入图片了。
另外还需要补充一个点。如果是使用`蒲公英（pyger）`平台，会发现每次上传安装包后返回的二维码图片是一个短链接，神奇的是这个短连接居然是固定的（对同一个账号而言）。这个短连接总是指向最近生成的二维码图片，但是对于二维码图片的唯一URL地址，平台并没有在响应中进行返回。在这种情况下，我们每次构建完成后保存二维码图片的URL链接就没有意义了。
应对的做法是，每次上传完安装包后，通过返回的二维码图片短链接将二维码图片下载并保存到本地，然后在build描述信息中引用该图片的Jenkins地址即可。
### 收集编译成果物（Artifacts）
每次完成构建后，编译生成的文件较多，但是并不是所有的文件都是我们需要的。
通常情况下，我们可能只需要其中的部分文件，例如`.ipa/.app/.plist/.apk`等，这时我们可以将这部分文件单独收集起来，并在构建页面中展示出来，以便在需要时进行下载。
要实现这样一个功能，需要在`【Post-build Actions】`栏目中新增`Archive the artifacts`，然后在`Files to archive`中通过正则表达式指定成果物文件的路径。
设置完毕后，每次构建完成后，Jenkins会在`Console Output`中采用设定的正则表达式进行搜索匹配，如果能成功匹配到文件，则会将文件收集起来。
## 总结
本文主要是对如何使用Jenkins搭建iOS/Android持续集成打包平台的基础概念和实施流程进行了介绍。对于其中涉及到的执行命令、构建脚本（build.py），以及Jenkins的详细配置，出于篇幅长度和阅读体验的考虑，并没有在文中进行详细展开。
为了实现真正的`开箱即用`，我将Jenkins的配置文件和构建脚本抽离出来形成一套模板，只需要导入到Jenkins中，然后针对具体的项目修改少量配置信息，即可将这一套持续集成打包平台运行起来，实现和文章开头插图中完全相同的功能效果。
详细内容请阅读《关于持续集成打包平台的Jenkins配置和构建脚本实现细节》。
GitHub地址：https://github.com/debugtalk/JenkinsTemplateForApp
