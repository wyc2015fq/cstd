# 如何创建自己的开源依赖库(如何上传自己的依赖库library到jCenter) - DEVELOPER - CSDN博客





2017年12月01日 16:25:58[学术袁](https://me.csdn.net/u012827205)阅读数：380
所属专栏：[Android-开发](https://blog.csdn.net/column/details/20398.html)









平时自己遇到了问题，或者一些经典的代码无头绪，写不出来的时候，我们很多人都会进入到 github 上去搜索与自己的问题相似的项目。然后搜索到之后，窃喜3秒钟！！简单阅读之后，看一看使用方式，如：![](https://img-blog.csdn.net/20171201162037658?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvanVuaHVhaG91c2U=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)

通过简单的一行代码就能引用我们需要的功能，好神奇！当然这是我刚开始接触到STUDIO时候的感觉。然后，当我们使用久了，必然会有这样的想法，我也需要写自己的功能代码依赖库，然后像这一样“简单粗暴”引用，不用在重复写代码了，多好！而且还能让更多的小伙伴共享~，不失为一件“普天同庆”的大事儿。

### 接下来我就一步一个脚印的分析：
- 在Studio上建Project

![](https://img-blog.csdn.net/20171201163158988?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvanVuaHVhaG91c2U=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)


需要先创建一个Project，然后在这个Project中创建一个Module。上图中，mymodule，就是新建的Module，而app则是建立Project时候自动创建的Module。
- 在码云上新建一个代码仓库

![](https://img-blog.csdn.net/20171201164117241?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvanVuaHVhaG91c2U=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)


在个人主页的左下角，创建公开的项目，如上图。



- 托管到码云

![](https://img-blog.csdn.net/20171201164528871?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvanVuaHVhaG91c2U=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)


在自己刚刚创建的项目中，如上图，复制HTTPS或者SSH的链接。到git上进行提交到码云托管本地IDE上的代码；

![](https://img-blog.csdn.net/20171201165005308?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvanVuaHVhaG91c2U=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)


![](https://img-blog.csdn.net/20171201165015911?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvanVuaHVhaG91c2U=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)


从上面，我们可以看到git命令：从本地ANDROID-STUDIO上的项目，托管到码云平台；从下图的提交注释可以看出，已经提交（push）成功了！

![](https://img-blog.csdn.net/20171201165232805?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvanVuaHVhaG91c2U=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)

- 在JFrog Bintray上操作：创建仓库、获取API-KEY

当然，在[JFrog Bintray](https://bintray.com)上操作需要我们进行注册一个账户；然后 Add New Repository ，并获取API KEY备用；

添加一个远程仓库 Repository，新建成功之后，可进入下一步操作。

![](https://img-blog.csdn.net/20171201170003007?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvanVuaHVhaG91c2U=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)


获取API KEY，在头像下选择 Edit Profile 进入新页面可以考到API KEY然后复制保存备用；

![](https://img-blog.csdn.net/20171201170224631?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvanVuaHVhaG91c2U=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)





![](https://img-blog.csdn.net/20171201170251093?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvanVuaHVhaG91c2U=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)


到此，所有的准备功能已经准备完毕！接下来就要进入到ANDROID STUDIO 中进行代码的一些配置；
- ANDROID STUDIO 代码配置




[https://github.com/bintray/gradle-bintray-plugin 链接](https://github.com/bintray/gradle-bintray-plugin)
[https://github.com/dcendents/android-maven-gradle-plugin 链接](https://github.com/dcendents/android-maven-gradle-plugin)


首先在Project下的 build.gradle 文件中添加代码：



```java
classpath 'com.jfrog.bintray.gradle:gradle-bintray-plugin:1.7'
classpath 'com.github.dcendents:android-maven-gradle-plugin:1.4.1'
```




![](https://img-blog.csdn.net/20171201171141125?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvanVuaHVhaG91c2U=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)


然后在library下的 build.gradle 文件中添加代码：



```java
apply plugin: 'com.github.dcendents.android-maven'
apply plugin: 'com.jfrog.bintray'
```




![](https://img-blog.csdn.net/20171201171224425?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvanVuaHVhaG91c2U=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)


然后，具体详细代码的配置，关于library的build.gradle；



```java
apply plugin: 'com.android.library'

//第一步：上传到jCenter的配置
apply plugin: 'com.github.dcendents.android-maven'
apply plugin: 'com.jfrog.bintray'

android {
    compileSdkVersion 26
    buildToolsVersion "26.0.0"

    defaultConfig {
        minSdkVersion 19
        targetSdkVersion 26
        versionCode 1
        versionName "1.0"

        testInstrumentationRunner "android.support.test.runner.AndroidJUnitRunner"

    }
    buildTypes {
        release {
            minifyEnabled false
            proguardFiles getDefaultProguardFile('proguard-android.txt'), 'proguard-rules.pro'
        }
    }
}

dependencies {
    compile fileTree(dir: 'libs', include: ['*.jar'])
    androidTestCompile('com.android.support.test.espresso:espresso-core:2.2.2', {
        exclude group: 'com.android.support', module: 'support-annotations'
    })
    compile 'com.android.support:appcompat-v7:26.+'
    testCompile 'junit:junit:4.12'


//上传编译的文件到bintray，使用如下的命令：
//gradlew bintrayUpload
/*library版本*/
    version = "v1.0.0"
/*github上的项目主页以及git仓库地址*/
    def siteUrl = 'https://gitee.com/yuan1530702811/BaseOnLibrary'
    def gitUrl = 'https://gitee.com/yuan1530702811/BaseOnLibrary.git'
/*maven group ID 这个gruop很重要，参考 compile 'groupId:artifactId:version'     groupId就是这个，artifactId就是包名，version就是顶部那个*/
    group = "com.ztman.cn"
    install {
        repositories.mavenInstaller {
            // This generates POM.xml with proper parameters
            pom {
                project {
                    packaging 'aar'
                    /*项目描述*/
                    name 'just a test'
                    url siteUrl
                    /*设置证书s*/
                    licenses {
                        license {
                            name 'The Apache Software License, Version 2.0'
                            url 'http://www.apache.org/licenses/LICENSE-2.0.txt'
                        }
                    }
                    developers {
                        developer {
                            id 'yjh'
                            name 'yjh'
                            email 'yuannunhua@gmail.com'
                        }
                    }
                    scm {
                        connection gitUrl
                        developerConnection gitUrl
                        url siteUrl
                    }
                }
            }
        }
    }
    task sourcesJar(type: Jar) {
        from android.sourceSets.main.java.srcDirs
        classifier = 'sources'
    }
    task javadoc(type: Javadoc) {
        source = android.sourceSets.main.java.srcDirs
        classpath += project.files(android.getBootClasspath().join(File.pathSeparator))
        options.encoding = "UTF-8" //添加UTF-8编码方式
    }
    task javadocJar(type: Jar, dependsOn: javadoc) {
        classifier = 'javadoc'
        from javadoc.destinationDir
    }
    artifacts {
        archives javadocJar
        archives sourcesJar
    }
    Properties properties = new Properties()
    properties.load(project.rootProject.file('local.properties').newDataInputStream())
    bintray {
        user = properties.getProperty("bintray.user")
        key = properties.getProperty("bintray.apikey")
        configurations = ['archives']
        pkg {
            repo = "baseOnMvpLibrary"  //对应bintray账号上的仓库名称;发布到Bintray的那个仓库，即你在bintray网站建立的仓库名，若该仓库不存在，会报错。
            name = "baseOnMvpLibrary"    //发布到JCenter仓库下的项目名字
            websiteUrl = siteUrl
            vcsUrl = gitUrl
            licenses = ["Apache-2.0"]
            publish = true
        }
    }
}
```






在local.properties中代码配置：



```java
## This file is automatically generated by Android Studio.
# Do not modify this file -- YOUR CHANGES WILL BE ERASED!
#
# This file must *NOT* be checked into Version Control Systems,
# as it contains information specific to your local configuration.
#
# Location of the SDK. This is only used by Gradle.
# For customization when using a Version Control System, please read the
# header note.
#Fri Dec 01 16:08:45 CST 2017
ndk.dir=C\:\\Users\\YJH\\AppData\\Local\\Android\\Sdk\\ndk-bundle
sdk.dir=C\:\\Users\\YJH\\AppData\\Local\\Android\\Sdk
#bintray上的用户名
bintray.user=yjh
#上面在bintray中，已经copy下来的apikey
bintray.apikey=bb1e83b4e1da047429738dfacc27420fb9e40bbc
```
相比之前多了两行代码：
```java
#bintray上的用户名
bintray.user=yjh
#上面在bintray中，已经copy下来的apikey
bintray.apikey=bb1e83b4e1da047429738dfacc27420fb9e40bbc
```




有一些内容我需要在这里进行指明道破：分别在文件 build.gradle和local.properties 中进行截图说明。如图


![](https://img-blog.csdn.net/20171204093642517?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvanVuaHVhaG91c2U=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)





![](https://img-blog.csdn.net/20171201175034440?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvanVuaHVhaG91c2U=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)





![](https://img-blog.csdn.net/20171201175045934?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvanVuaHVhaG91c2U=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)

- 检测代码并编译上传

在自己的ANDROID STUDIO 的终端 Terminal 中执行命令：

检测>>

![](https://img-blog.csdn.net/20171201175251686?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvanVuaHVhaG91c2U=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)





![](https://img-blog.csdn.net/20171201175236049?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvanVuaHVhaG91c2U=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)


编译上传>>

![](https://img-blog.csdn.net/20171201175332587?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvanVuaHVhaG91c2U=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)





![](https://img-blog.csdn.net/20171201175344879?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvanVuaHVhaG91c2U=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)


当我们在上面的终端中敲入命令，最终的运行结果都是 "BUILD SUCCESSFUL" 时候，说明我们到此的所有操作都是正确的了。这个时候，我们可以看到此时我们之前创建的Repository的名字后面多了一个 单词-package

![](https://img-blog.csdn.net/20171201175836823?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvanVuaHVhaG91c2U=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)


截止到此时，我们已经成功的把library 上传到了 bintray 的 Respository中。但是目前还不能进行 compile 的依赖，因为我们还需要把package 提交到 jCenter。

点击进入，仍需要这样的操作：

![](https://img-blog.csdn.net/20171204091143283?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvanVuaHVhaG91c2U=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)





![](https://img-blog.csdn.net/20171204095237159?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvanVuaHVhaG91c2U=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)


等待审核通过，然后就能 compile 了！！




注意：如果在IDE终端执行命令 gradlew bintrayUpload 出现



```java
FAILURE: Build failed with an exception.

* What went wrong:
Execution failed for task ':mymodule:bintrayUpload'.
> Could not upload to 'https://api.bintray.com/content/yjh/baseOnMvpLibrary/baseOnMvpLibrary/v1.0.0/com/ztman/cn/mymodule/v1.0.0/mymodule-v1.0.0-javadoc.jar'
: HTTP/1.1 409 Conflict [message:Unable to upload files: An art
ifact with the path 'com/ztman/cn/mymodule/v1.0.0/mymodule-v1.0.0-javadoc.jar' already exists]
```




说明 

```java
mymodule-v1.0.0-javadoc.jar
```



文件已存在,意思就是已经上传过一次了，要再次上传版本内容，此时需要我们更新新的版本并上传！



