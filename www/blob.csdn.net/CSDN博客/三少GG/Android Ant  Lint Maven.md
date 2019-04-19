# Android Ant|| Lint||Maven - 三少GG - CSDN博客
2013年01月23日 13:21:40[三少GG](https://me.csdn.net/scut1135)阅读数：1359
个人分类：[Android 常规](https://blog.csdn.net/scut1135/article/category/1057214)
### ### [使用ANT打包Android应用](http://blog.csdn.net/liuhe688/article/details/6679879)
分类： [ANDROID](http://blog.csdn.net/liuhe688/article/category/790276)2011-08-15
 20:246296人阅读[评论](http://blog.csdn.net/liuhe688/article/details/6679879#comments)(15)[收藏]()[举报](http://blog.csdn.net/liuhe688/article/details/6679879#report)
大家好，今天来分享一下如何使用ANT打包Android应用。
通常我们习惯用eclipse来开发Android程序，它会自动帮我们打包当前的应用程序。如果在Navigator视图下，我们可以看到以下几个文件：
![](http://hi.csdn.net/attachment/201108/12/0_1313145421Dibi.gif)
在上图中，com包放置的是我们的class文件，classes.dex是class文件经过转换后的可以在dalvik上跑的精简类文件，resources.ap_是经过打包的资源文件，ant.apk就是最终的打包文件。
使用ANT来对应用打包，一般会经过以下几个步骤：
1.用aapt命令生成R.java文件
2.用aidl命令生成相应java文件
3.用javac命令编译java源文件生成class文件
4.用dx.bat将class文件转换成classes.dex文件
5.用aapt命令生成资源包文件resources.ap_
6.用apkbuilder.bat打包资源和classes.dex文件,生成unsigned.apk
7.用jarsinger命令对apk认证,生成signed.apk
为了便于理解和记忆，下面来用一张流程图来说明以上的几个过程：
![](http://hi.csdn.net/attachment/201108/15/0_1313405397MZjr.gif)
以上就是整体的流程，下面我们就对其每个部分进行做出详细讲解，把每一个步骤都弄清楚了。
我们需要先熟悉一下每一个步骤所使用到的命令：
1.aapt（Android Asset Packaging Tool）命令，根据资源文件生成R.java文件
![](http://hi.csdn.net/attachment/201108/15/0_13134058081BHb.gif)
参数说明：
-f  强制覆盖已存在的文件。
-m  在-J指定的位置下自动生成相应的包的目录。
-J  指定R.java文件生成的目录。
-S  指定资源目录。
-M  指定清单文件。
-I  引入类库。
注意，我们当前所在的位置是ant项目根目录，所以必要时需要输入很多关于命令的路径，以下示例也是一样。
2.aidl（Android Interface Definition Language）命令，根据.aidl定义文件生成java文件
![](http://hi.csdn.net/attachment/201108/15/0_13134061921Z5f.gif)
上面的示例所在位置为com/scott/ant下，根据包中的Person.aidl文件，在gen对应的目录中生成Person.java文件，示例中只是处理单一文件，下文中会讲述如何处理目录中的多个aidl文件。
3.javac（Java Compiler）命令，根据源文件生成对应的class文件
![](http://hi.csdn.net/attachment/201108/15/0_13134066001bbQ.gif)
参数说明：
-d <目录>      指定存放生成的类文件的位置
-bootclasspath <路径>     覆盖引导类文件的位置
示例中并没有考虑到引用类路径下面的类库，复杂的情况会在稍后遇到的。
4.dx命令，将class文件转换成.dex文件
![](http://hi.csdn.net/attachment/201108/15/0_1313406943p02O.gif)
以上示例是将bin目录下的class文件转换成classes.dex文件，输出到bin目录，我们也许会用到第三方类库，等一会就会看到。
5.aapt将资源文件打包
![](http://hi.csdn.net/attachment/201108/15/0_1313407283eOe5.gif)
参数说明：
-f 强制覆盖
-M 指定Manifest文件
-S 指定资源目录
-A 指定资产目录
-I 指定引入的类库
-F 指定要生成的包
6.apkbuilder命令，根据classes.dex文件和resources.ap_生成为签证的apk包
![](http://hi.csdn.net/attachment/201108/15/0_1313407805oL03.gif)
参数说明：
-rf 参照源文件的目录的结构
7.jarsigner命令，对上面生成的apk包进行签证
![](http://hi.csdn.net/attachment/201108/15/0_1313407979xnzc.gif)
在签证的过程中，需要使用到证书文件，需要注意的是最后的release是证书的别名，关于如何创建证书，请看下图：
![](http://hi.csdn.net/attachment/201108/15/0_1313409172BdDm.gif)
当然也可以在eclipse里使用ADT提供的图形界面完成以上步骤，选中项目，点击右键，“Android Tools=>Export Signed Application Package”，然后再其中的Keystore selection环节选择“Create new keystore”，然后按照提示填写信息就可以了。
以上是我们使用到的命令，接下来我们就该来分析一下ANT所必须的build.xml：
首先我们需要定义大量的变量属性，用来表示使用到的路径、目录等，如下：
**[html]**[view
 plain](http://blog.csdn.net/liuhe688/article/details/6679879#)[copy](http://blog.csdn.net/liuhe688/article/details/6679879#)
- <projectname="ant"default="release">
- <!-- ANT环境变量 -->
- <propertyenvironment="env"/>
- <!-- 应用名称 -->
- <propertyname="appName"value="${ant.project.name}"/>
- <!-- SDK目录(获取操作系统环境变量ANDROID_SDK_HOME的值) -->
- <propertyname="sdk-folder"value="${env.ANDROID_SDK_HOME}"/>
- <!-- SDK指定平台目录 -->
- <propertyname="sdk-platform-folder"value="${sdk-folder}/platforms/android-8"/>
- <!-- SDK中tools目录 -->
- <propertyname="sdk-tools"value="${sdk-folder}/tools"/>
- <!-- SDK指定平台中tools目录 -->
- <propertyname="sdk-platform-tools"value="${sdk-platform-folder}/tools"/>
- 
- <!-- 使用到的命令(当前系统为windows,如果系统为linux,可将.bat文件替换成相对应的命令) -->
- <propertyname="aapt"value="${sdk-platform-tools}/aapt"/>
- <propertyname="aidl"value="${sdk-platform-tools}/aidl"/>
- <propertyname="dx"value="${sdk-platform-tools}/dx.bat"/>
- <propertyname="apkbuilder"value="${sdk-tools}/apkbuilder.bat"/>
- <propertyname="jarsigner"value="${env.JAVA_HOME}/bin/jarsigner"/>
- 
- <!-- 编译需要的jar; 如果项目使用到地图服务则需要maps.jar -->
- <propertyname="android-jar"value="${sdk-platform-folder}/android.jar"/>
- <propertyname="android-maps-jar"value="${sdk-folder}/add-ons/addon_google_apis_google_inc_8/libs/maps.jar"/>
- 
- <!-- 编译aidl文件所需的预处理框架文件framework.aidl -->
- <propertyname="framework-aidl"value="${sdk-platform-folder}/framework.aidl"/>
- 
- <!-- 生成R文件的相对目录 -->
- <propertyname="outdir-gen"value="gen"/>
- <!-- 编译后的文件放置目录 -->
- <propertyname="outdir-bin"value="bin"/>
- 
- <!-- 清单文件 -->
- <propertyname="manifest-xml"value="AndroidManifest.xml"/>
- <!-- 源文件目录 -->
- <propertyname="resource-dir"value="res"/>
- <propertyname="asset-dir"value="assets"/>
- <!-- java源文件目录 -->
- <propertyname="srcdir"value="src"/>
- <propertyname="srcdir-ospath"value="${basedir}/${srcdir}"/>
- <!-- 外部类库所在目录 -->
- <propertyname="external-lib"value="lib"/>
- <propertyname="external-lib-ospath"value="${basedir}/${external-lib}"/>
- 
- <!-- 生成class目录 -->
- <propertyname="outdir-classes"value="${outdir-bin}"/>
- <propertyname="outdir-classes-ospath"value="${basedir}/${outdir-classes}"/>
- 
- <!-- classes.dex相关变量 -->
- <propertyname="dex-file"value="classes.dex"/>
- <propertyname="dex-path"value="${outdir-bin}/${dex-file}"/>
- <propertyname="dex-ospath"value="${basedir}/${dex-path}"/>
- 
- <!-- 经过aapt生成的资源包文件 -->
- <propertyname="resources-package"value="${outdir-bin}/resources.ap_"/>
- <propertyname="resources-package-ospath"value="${basedir}/${resources-package}"/>
- 
- <!-- 未认证apk包 -->
- <propertyname="out-unsigned-package"value="${outdir-bin}/${appName}-unsigned.apk"/>
- <propertyname="out-unsigned-package-ospath"value="${basedir}/${out-unsigned-package}"/>
- 
- <!-- 证书文件 -->
- <propertyname="keystore-file"value="${basedir}/release.keystore"/>
- 
- <!-- 已认证apk包 -->
- <propertyname="out-signed-package"value="${outdir-bin}/${appName}.apk"/>
- <propertyname="out-signed-package-ospath"value="${basedir}/${out-signed-package}"/>
-         ...  
- </project>
然后，我们分步骤来进行，首先是初始化：
**[html]**[view
 plain](http://blog.csdn.net/liuhe688/article/details/6679879#)[copy](http://blog.csdn.net/liuhe688/article/details/6679879#)
- <!-- 初始化工作 -->
- <targetname="init">
- <echo>Initializing all output directories...</echo>
- <deletedir="${outdir-bin}"/>
- <mkdirdir="${outdir-bin}"/>
- <mkdirdir="${outdir-classes}"/>
- </target>
其次是生成R.java文件：
**[html]**[view
 plain](http://blog.csdn.net/liuhe688/article/details/6679879#)[copy](http://blog.csdn.net/liuhe688/article/details/6679879#)
- <!-- 根据工程中的资源文件生成R.java文件  -->
- <targetname="gen-R"depends="init">
- <echo>Generating R.java from the resources...</echo>
- <execexecutable="${aapt}"failonerror="true">
- <argvalue="package"/>
- <argvalue="-f"/>
- <argvalue="-m"/>
- <argvalue="-J"/>
- <argvalue="${outdir-gen}"/>
- <argvalue="-S"/>
- <argvalue="${resource-dir}"/>
- <argvalue="-M"/>
- <argvalue="${manifest-xml}"/>
- <argvalue="-I"/>
- <argvalue="${android-jar}"/>
- </exec>
- </target>
接着是aidl生成java源文件：
**[html]**[view
 plain](http://blog.csdn.net/liuhe688/article/details/6679879#)[copy](http://blog.csdn.net/liuhe688/article/details/6679879#)
- <!-- 编译aidl文件 -->
- <targetname="aidl"depends="gen-R">
- <echo>Compiling .aidl into java files...</echo>
- <applyexecutable="${aidl}"failonerror="true">
- <!-- 指定预处理文件 -->
- <argvalue="-p${framework-aidl}"/>
- <!-- aidl声明的目录 -->
- <argvalue="-I${srcdir}"/>
- <!-- 目标文件目录 -->
- <argvalue="-o${outdir-gen}"/>
- <!-- 指定哪些文件需要编译 -->
- <filesetdir="${srcdir}">
- <includename="**/*.aidl"/>
- </fileset>
- </apply>
- </target>
我们指定了一个framework.aidl，里面定义了很多android内置对象，然后我们指定了aidl所在目录和输出目录，组后指定编译后缀为aidl的文件。
接下来是将源文件编译成class文件：
**[html]**[view
 plain](http://blog.csdn.net/liuhe688/article/details/6679879#)[copy](http://blog.csdn.net/liuhe688/article/details/6679879#)
- <!-- 将工程中的java源文件编译成class文件 -->
- <targetname="compile"depends="aidl">
- <echo>Compiling java source code...</echo>
- <javacencoding="utf-8"target="1.5"srcdir="."destdir="${outdir-classes}"bootclasspath="${android-jar}">
- <classpath>
- <filesetdir="${external-lib}"includes="*.jar"/>
- <filelist>
- <filename="${android-maps-jar}"/>
- </filelist>
- </classpath>
- </javac>
- </target>
如果使用到了第三方类库，我们可以在classpath标签下配置。
接着是将class文件转换成classes.dex：
**[html]**[view
 plain](http://blog.csdn.net/liuhe688/article/details/6679879#)[copy](http://blog.csdn.net/liuhe688/article/details/6679879#)
- <!-- 将.class文件转化成.dex文件 -->
- <targetname="dex"depends="compile">
- <echo>Converting compiled files and external libraries into a .dex file...</echo>
- <execexecutable="${dx}"failonerror="true">
- <argvalue="--dex"/>
- <!-- 输出文件 -->
- <argvalue="--output=${dex-ospath}"/>
- <!-- 要生成.dex文件的源classes和libraries -->
- <argvalue="${outdir-classes-ospath}"/>
- <argvalue="${external-lib-ospath}"/>
- </exec>
- </target>
就像上面的代码一样，如果使用到第三方类库，可以在最后一参数的形式追加进去。
然后是将资源文件打包：
**[html]**[view
 plain](http://blog.csdn.net/liuhe688/article/details/6679879#)[copy](http://blog.csdn.net/liuhe688/article/details/6679879#)
- <!-- 将资源文件放进输出目录 -->
- <targetname="package-res-and-assets">
- <echo>Packaging resources and assets...</echo>
- <execexecutable="${aapt}"failonerror="true">
- <argvalue="package"/>
- <argvalue="-f"/>
- <argvalue="-M"/>
- <argvalue="${manifest-xml}"/>
- <argvalue="-S"/>
- <argvalue="${resource-dir}"/>
- <argvalue="-A"/>
- <argvalue="${asset-dir}"/>
- <argvalue="-I"/>
- <argvalue="${android-jar}"/>
- <argvalue="-F"/>
- <argvalue="${resources-package}"/>
- </exec>
- </target>
接着是打包成未签证的apk包：
**[html]**[view
 plain](http://blog.csdn.net/liuhe688/article/details/6679879#)[copy](http://blog.csdn.net/liuhe688/article/details/6679879#)
- <!-- 打包成未签证的apk -->
- <targetname="package"depends="dex, package-res-and-assets">
- <echo>Packaging unsigned apk for release...</echo>
- <execexecutable="${apkbuilder}"failonerror="true">
- <argvalue="${out-unsigned-package-ospath}"/>
- <argvalue="-u"/>
- <argvalue="-z"/>
- <argvalue="${resources-package-ospath}"/>
- <argvalue="-f"/>
- <argvalue="${dex-ospath}"/>
- <argvalue="-rf"/>
- <argvalue="${srcdir-ospath}"/>
- </exec>
- <echo>It will need to be signed with jarsigner before being published.</echo>
- </target>
然后是对apk签证：
**[html]**[view
 plain](http://blog.csdn.net/liuhe688/article/details/6679879#)[copy](http://blog.csdn.net/liuhe688/article/details/6679879#)
- <!-- 对apk进行签证 -->
- <targetname="jarsigner"depends="package">
- <echo>Packaging signed apk for release...</echo>
- <execexecutable="${jarsigner}"failonerror="true">
- <argvalue="-keystore"/>
- <argvalue="${keystore-file}"/>
- <argvalue="-storepass"/>
- <argvalue="123456"/>
- <argvalue="-keypass"/>
- <argvalue="123456"/>
- <argvalue="-signedjar"/>
- <argvalue="${out-signed-package-ospath}"/>
- <argvalue="${out-unsigned-package-ospath}"/>
- <!-- 不要忘了证书的别名 -->
- <argvalue="release"/>
- </exec>
- </target>
最后发布：
**[html]**[view
 plain](http://blog.csdn.net/liuhe688/article/details/6679879#)[copy](http://blog.csdn.net/liuhe688/article/details/6679879#)
- <!-- 发布 -->
- <targetname="release"depends="jarsigner">
- <!-- 删除未签证apk -->
- <deletefile="${out-unsigned-package-ospath}"/>
- <echo>APK is released. path:${out-signed-package-ospath}</echo>
- </target>
这样就完成了build.xml的编辑，eclipse继承了ANT，所以我们可以在eclipse中直接运行，也可以在代码中调用。
首先我们需要下载ANT，然后配置相应的环境变量信息，最后我们这样调用：
**[java]**[view
 plain](http://blog.csdn.net/liuhe688/article/details/6679879#)[copy](http://blog.csdn.net/liuhe688/article/details/6679879#)
- Process p = Runtime.getRuntime().exec("ant.bat -buildfile d:/workspace/ant/build.xml");  
- InputStream is = p.getInputStream();  
- BufferedReader br = new BufferedReader(new InputStreamReader(is));  
- String line = null;  
- while ((line = br.readLine()) != null) {  
-     System.out.println(line);  
- }  
- System.out.println("SUCCESS.");  
就先讲到这里吧，谢谢大家。
http://tools.android.com/tips/lint
### Android Lint
## ![](http://tools.android.com/_/rsrc/1321373723841/tips/lint/lint-html-small.png)Android Lint is a new tool introduced in ADT 16 (and Tools 16) which scans Android project sources for potential bugs. It is available both as a command line tool, as well as integrated with Eclipse (described below), and IntelliJ ([details](http://blogs.jetbrains.com/idea/2012/02/integration-with-android-lint-tool-in-intellij-idea-111/)). The architecture is deliberately IDE independent so it will hopefully be integrated with other IDEs, with other build tools and with continuous integration systems as well.
## Here are some examples of the types of errors that it looks for:
- Missing translations (and unused translations)
- Layout performance problems (all the issues the old `layoutopt` tool used to find, and more)
- Unused resources
- Inconsistent array sizes (when arrays are defined in multiple configurations)
- Accessibility and internationalization problems (hardcoded strings, missing contentDescription, etc)
- Icon problems (like missing densities, duplicate icons, wrong sizes, etc)
- Usability problems (like not specifying an input type on a text field)
- Manifest errors
and many more.

你是否早已厌倦了日复一日的手工构建工作？你是否对各个项目风格迥异的构建系统感到恐惧？Maven——这一Java社区事实标准的项目管理工具，能帮你从琐碎的手工劳动中解脱出来，帮你规范整个组织的构建系统。不仅如此，它还有依赖管理、自动生成项目站点等超酷的特性。已经有无数的开源项目使用它来构建项目，促进团队交流。每天都由数以万计的开发者在访问中央仓库以获取他们需要的依赖。
由国内的Maven权威亲自执笔，《Maven实战》告诉你Maven的本质是什么，深入阐述Maven的依赖管理、仓库、生命周期、插件等核心概念。你还能看到实际的案例演变，以及诸如多模块组织、建立Nexus私服、使用Hudson进行持续集成等企业开发的最佳实践。
