
# intellij idea创建maven项目 - anlcy - 博客园






# [intellij idea创建maven项目](https://www.cnblogs.com/camilla/p/9197932.html)
1.安装好JDK，Tomcat，安装好maven；
2.配置maven全局变量：file-》Other Settings -》Default Settings-》Build，Execution,Deployment-》Build Tools -》Maven-》选择Execute goals recursively,设置Maven home direcory为自己的maven安装目录。库也设置为选择自己建的本地库；选择Maven下的Runner，选择Run in background，VM Options:-Dmaven.multiModuleProjectDirectory=$M2_HOME,选择自己的JRE
3.打开idea，file-》new Project -》maven；
4.需要内置模板就选择Create from archetype选择:maven-archetype-webapp,点击next，不需要则直接下一步，Properties增加项：archetypeCatalog=internal， 点击下一步，逐步设置项目名就可以了，完成后点击“Enable-Auto import”；注意：module name默认和project name 一样的，但是如果这个项目只有这一个module，不改无所谓的，如果有很多的module，那肯定是修改一下。
5.如果无选择内置模板，添加Web项目支持：右击项目：Add Framework Support，选择 “Web Application”；
6.配置项目的JDK和language level,language选择7项，首先打开Project Structure步骤：主界面File——>Project Structure——>Project；
7.把java目录设为源码目录，首先打开Project Structure步骤：主界面File——>Project Structure——>modules，选中src目录,在面目了下新建java文件夹，选择java文件夹，点击Sources，点击apply-》ok；该设置页面的各项目录介绍：
1).Sources 一般用于标注类似 src 这种可编译目录。有时候我们不单单项目的 src 目录要可编译，还有其他一些特别的目录也许我们也要作为可编译的目录，就需要对该目录进行此标注。只有 Sources 这种可编译目录才可以新建 Java 类和包，这一点需要牢记。
2).Tests 一般用于标注可编译的单元测试目录。在规范的 maven 项目结构中，顶级目录是 src，maven 的 src 我们是不会设置为 Sources 的，而是在其子目录 main 目录下的 java 目录，我们会设置为 Sources。而单元测试的目录是 src - test - java，这里的 java 目录我们就会设置为 Tests，表示该目录是作为可编译的单元测试目录。一般这个和后面几个我们都是在 maven项目下进行配置的，但是我这里还是会先说说。从这一点我们也可以看出 IntelliJ IDEA 对 maven 项目的支持是比彻底的。
*3).Resources 一般用于标注资源文件目录。在 maven 项目下，资源目录是单独划分出来的，其目录为：src - main -resources，这里的 resources 目录我们就会设置为 Resources，**表示该目录是作为资源目录。资源目录下的文件是会被编译到输出目录下的。*
*4).Test Resources 一般用于标注单元测试的资源文件目录。在 maven 项目下，单元测试的资源目录是单独划分出来的，其目录为：src - test -resources，这里的 resources 目录**我们就会设置为 Test*
5).Resources，表示该目录是作为单元测试的资源目录。资源目录下的文件是会被编译到输出目录下的。
6).Excluded 一般用于标注排除目录。被排除的目录不会被 IntelliJ IDEA 创建索引，相当于被 IntelliJ IDEA 废弃，该目录下的代码文件是不具备代码检查和智能提示等常规代码功能。
8.为module添加外部依赖的jar包。步骤：主界面File——>Project Structure——>modules-》Dependencies，点击+号添加Tomcat包依赖
9.配置Tomcat：
1).在主页面打开Project Structure，点击图示的按钮或是按ctrl+shift+alt+s快捷键;
2).点击Facets->+号,在弹出的对话框个中选择Web.
3).在弹出的对话框选择刚才创建的Maven项目（MavenDemo），点击ok.
4).点击Deployment Descriptors 对话框右侧的加号，选择web.xml所在的路径，点击Ok。
5).再次点击Web Resource Directories对话框右侧的加号，选择webapp所在的路径，点击Ok。
6).再次回到Project Structure,点击左侧 Artifacts，依次点击+号->Web Application:Exploded->From Modules.
7).在弹出的框中选择刚才创建的Maven项目（MavenDemo），Ok.（1到7步骤为位选择内置模板建的普通项目）；
8).在主页点击图标Edit Configurations，在弹出的对话框中点击+号，找到Tomcat Server ，选择Local.
9).在弹出的对话框中，选择Deployment,点击右侧的+号，选择Artifact。
10).接着回到Server选项卡，配置一下Tomcat的基本参数，name：Tomcat的名字，after launch：设置默认浏览器，设置访问地址和jre：jdk版本，最后点击Ok即可
11).在主页点击debug启动项目。
错误解决方案：
1.启动tomcat时，一直卡在Deploying web application directory这块的解决方案：
找到jdk1.x.x_xx/jre/lib/security/[Java](http://lib.csdn.net/base/java).security文件，在文件中找到securerandom.source这个设置项，将其改为：
securerandom.source=file:/dev/./urandom。





