# Gradle在大型Java项目上的应用 - 零度的博客专栏 - CSDN博客
2015年08月05日 09:53:29[零度anngle](https://me.csdn.net/zmx729618)阅读数：912标签：[gradle](https://so.csdn.net/so/search/s.do?q=gradle&t=blog)
个人分类：[Java](https://blog.csdn.net/zmx729618/article/category/3108443)
在Java构建工具的世界里，先有了Ant，然后有了Maven。Maven的CoC[1]、依赖管理以及项目构建规则重用性等特点，让Maven几乎成为Java构建工具的事实标准。然而，冗余的依赖管理配置、复杂并且难以扩展的构建生命周期，都成为使用Maven的困扰。
[Gradle](http://www.gradle.org/)作为新的构建工具，获得了[2010 Springy](http://www.springsource.org/node/2871)大奖，并入围了2011的[Jax](http://jaxconf.com/)最佳Java技术发明奖。它是基于Groovy语言的构建工具，既保持了Maven的优点，又通过使用Groovy定义的DSL[2]，克服了
 Maven中使用XML繁冗以及不灵活等缺点。在Eugene Dvorkin撰写的文章《[最让人激动的5个Java项目》](http://eugenedvorkin.com/five-exciting-java-projects-you-should-know-about/)中，他是这样介绍Gradle的：
“工程自动化是软件项目成功的必要条件，而且它应该实现起来简单、易用、好玩。构建没有千篇一律的方法，所以Gradle没有死板的强加方法于我们，尽管你会认为查找和描述方法很重要，然而Gradle对于如何描述有着非常好的支持。我不认为工具能够拯救我们，但是Gradle能给你所需要的自由，你可以利用Gradle构建易描述的、可维护的、简洁的、高性能项目”。
在最近半年里，我在使用Gradle作为构建脚本的大型Java项目上工作，更深切体会到Gradle在项目构建过程中是如此的简单、易用。
## 1. 多Module的项目
Hibernate项目负责人Steve Ebersole在Hibernate将构建脚本从Maven换成Gradle时，专门写了一篇文章[《Gradle: why?》](https://community.jboss.org/wiki/Gradlewhy)，文中提到Maven的一个缺点就是：Maven不支持多module的构建。在Micro-Service[3]架构风格流行的今天，在一个项目里面包含多个Module已成为一种趋势。Gradle天然支持多module，并且提供了很多手段来简化构建脚本。在Gradle中，一个模块就是它的一个子项目（subproject），所以，我使用父项目来描述顶级项目，使用子项目来描述顶级项目下面的模块。
### 1.1 配置子项目
在多模块的项目中，Gradle遵循惯例优于配置 （Convention Over Configuration）原则。
在父项目的根目录下寻找settings.gradle文件，在该文件中设置想要包括到项目构建中的子项目。在构建的初始化阶段（Initialization），Gradle会根据settings.gradle 文件来判断有哪些子项目被include到了构建中，并为每一个子项目初始化一个Project对象，在构建脚本中通过project(‘:sub-project-name’)来引用子项目对应的Project对象。
通常，多模块项目的目录结构要求将子模块放在父项目的根目录下，但是如果有特殊的目录结构，可以在settings.gradle文件中配置。
我所在的项目包括：
- 一个描述核心业务的core模块
- 一个遗留的Enterprise Java Bean（enterprise-beans）模块
- 两个提供不同服务的Web项目（cis-war和admin-war）
- 一个通过schema生成jaxb对象的jaxb项目以及一个用来用来打ear包的ear项目
- 一个用于存放项目配置文件相关的config子目录。它不是子模块，所以 config不应该被加到项目的构建中去。
它们都放置在根项目目录下。我们通过如下的settings.gradle来设置项目中的子项目：
include 'core', 'enterprise-beans', 'cis-war', 'admin-war', 'jaxb', 'ear'
我们将需要加入到项目构建中的子项目配置在settings.gradle文件中，而没有加入不需要的config子目录。
### 1.2 共享配置
在大型Java项目中，子项目之间必然具有相同的配置项。我们在编写代码时，要追求代码重用和代码整洁；而在编写Gradle脚本时，同样需要保持代码重用和代码整洁。Gradle 提供了不同的方式使不同的项目能够共享配置。
- allprojects：allprojects是父Project的一个属性，该属性会返回该Project对象以及其所有子项目。在父项目的build.gradle脚本里，可以通过给allprojects传一个包含配置信息的闭包，来配置所有项目（包括父项目）的共同设置。通常可以在这里配置IDE的插件，group和version等信息，比如：
allprojects {
    apply plugin: 'idea'
    }
这样就会给所有的项目（包括当前项目以及其子项目）应用上idea插件。
- subprojects：subprojects和allprojects一样，也是父Project的一个属性，该属性会返回所有子项目。在父项目的build.gradle脚本里，给 subprojects传一个包含配置信息的闭包，可以配置所有子项目共有的设置，比如共同的插件、repositories、依赖版本以及依赖配置：
subprojects {
    apply plugin: 'java'
    repositories {
        mavenCentral()
    }
    ext {
          guavaVersion = ’14.0.1’
          junitVersion = ‘4.10’ 
   } 
    dependencies {
        compile(
                “com.google.guava:guava:${guavaVersion}”
        )
        testCompile(
                “junit:junit:${junitVersion}”
        )
    }
}
这就会给所有子项目设置上java的插件、使用mavenCentral作为 所有子项目的repository以及对Guava[4]和JUnit的项目依赖。此外，这里还在ext里配置依赖包的版本，方便以后升级依赖的版本。
- configure：在项目中，并不是所有的子项目都会具有相同的配置，但是会有部分子项目具有相同的配置，比如在我所在的项目里除了cis-war和admin-war是web项目之外，其他子项目都不是。所以需要给这两个子项目添加war插件。Gradle的configure可以传入子项目数组，并为这些子项目设置相关配置。在我的项目中使用如下的配置：
configure(subprojects.findAll {it.name.contains('war')}) {
    apply plugin: 'war'
    }
configure需要传入一个Project对象的数组，通过查找所有项目名包含war的子项目，并为其设置war插件。
### 1.3 独享配置
在项目中，除了设置共同配置之外， 每个子项目还会有其独有的配置。比如每个子项目具有不同的依赖以及每个子项目特殊的task等。Gradle提供了两种方式来分别为每个子项目设置独有的配置。
- 在父项目的build.gradle文件中通过project(‘:sub-project-name’)来设置对应的子项目的配置。比如在子项目core需要Hibernate的依赖，可以在父项目的build.gradle文件中添加如下的配置：
project(‘:core’) {
      ext{
                   hibernateVersion = ‘4.2.1.Final’
      }
	dependencies { 
    		compile “org.hibernate:hibernate-core:${hibernateVersion}”
}
}
注意这里子项目名字前面有一个冒号（：）。 通过这种方式，指定对应的子项目，并对其进行配置。
- 我们还可以在每个子项目的目录里建立自己的构建脚本。在上例中，可以在子项目core目录下为其建立一个build.gradle文件，并在该构建脚本中配置core子项目所需的所有配置。例如，在该build.gradle文件中添加如下配置：
 ext{
       hibernateVersion = ‘4.2.1.Final’
      }
	dependencies { 
    	compile “org.hibernate:hibernate-core:${hibernateVersion}”
}
根据我对Gradle的使用经验，对于子项目少，配置简单的小型项目，推荐使用第一种方式配置，这样就可以把所有的配置信息放在同一个build.gradle文件里。例如我同事[郑晔](http://dreamhead.blogbus.com/)的开源项目[moco](https://github.com/dreamhead/moco)。它只有两个子项目，因而就使用了第一种方式配置，在项目根目录下的[build.gradle](https://github.com/dreamhead/moco/blob/master/build.gradle)文件中设置项目相关的配置信息。但是，若是对于子项目多，并且配置复杂的大型项目，使用第二种方式对项目进行配置会更好。因为，第二种配置方式将各个项目的配置分别放到单独的build.gradle文件中去，可以方便设置和管理每个子项目的配置信息。
### 1.4 其他共享
在Gradle中，除了上面提到的配置信息共享，还可以共享方法以及Task。可以在根目录的build.gradle文件中添加所有子项目都需要的方法，在子项目的build.gradle文件中调用在父项目build.gradle脚本里定义的方法。例如我定义了这样一个方法，它可以从命令行中获取属性，若没有提供该属性，则使用默认值：
def defaultProperty(propertyName, defaultValue) {
    return hasProperty(propertyName) ? project[propertyName] : defaultValue
}
注意，这段脚本完全就是一段Groovy代码，具有非常好的可读性。
由于在父项目中定义了defaultProperty方法，因而在子项目的build.gradle文件中，也可以调用该方法。
## 2. 环境的配置
为了方便地将应用部署到开发、测试以及产品等不同环境上， Gradle提供了几种不同的方式为不同的环境打包，使得不同的环境可以使用不同的配置文件。此外，它还提供了简单的方法，使得我们能够便捷地初始化数据库 。
### 2.1 Properties配置
要为不同的环境提供不一样的配置信息，Maven选择使用profile，而Gradle则提供了两种方法为构建脚本提供Properties配置：
- 第一种方式是使用传统的properties文件， 然后在使用Gradle时，通过传入不同的参数加载不同的properties文件。例如，我们可以在项目中提供development.properties、test.properties和production.properties。在项目运行时，使用-Pprofile=development来指定加载开发环境的配置。构建脚本中加载properties文件的代码如下：
ext {
    profile = project['profile']
}
def loadProperties(){
    def props = new Properties()
    new File("${rootProject.projectDir}/config/${profile}.properties")
            .withInputStream {
                stream -> props.load(stream)
            }
    props
}
在运行脚本的时候，传入的-Pprofile=development可以指定使用哪个运行环境的配置文件。代码中使用了project['profile']从命令行里读取-P传入的参数，Gradle会去父项目根目录下的config文件夹中需找对应的properties文件。
- 另外一种方式就是使用Groovy的语法，定义可读性更高的配置文件。比如可以在项目中定义config.groovy的配置文件，内容如下：
environments {
    development {
        jdbc {
            url = 'development'
            user = 'xxxx'
            password = 'xxxx'
        }
    }
    test {
        jdbc {
            url = 'test'
            user = 'xxxx'
            password = 'xxxx'
        }
    }
    production {
        jdbc {
            url = 'production'
            user = 'xxxx'
            password = 'xxxx'
        }
    }
}
这里定义了三个环境下的不同数据库配置，在构建脚本中使用如下的代码来加载：
ext {
    profile = project['profile']
}
def loadGroovy(){
    def configFile = file('config.groovy')
    new ConfigSlurper(profile).parse(configFile.toURL()).toProperties()
}
这里在ConfigSlurper的构造函数里传入从命令行里取到的-P的参数。调用loadGroovy方法就可以加载项目根目录下的config.groovy文件，并作为一个Map返回，这样就可以通过jdbc.url来获取url的值。
从可读性以及代码整洁（配置文件也需要代码整洁）而言，我推荐使用第二种方式来配置，因为这种方法具有清晰的结构。如上面的例子，就可以把数据库相关的信息都放在jdbc这个大的节点下，而不用像properties文件这样的扁平结构。但是对于一些已经使用properties文件来为不同环境提供配置信息的遗留项目里，使用properties文件也没有问题。
### 2.2 替换
通过不同的方式加载不同环境的配置后，就需要把它们替换到有占位符的配置文件中去。在配置文件中使用@key@来标注要被替换的位置，比如在config文件夹中有jdbc.properties文件，其内容如下：
jdbc.url=@jdbc.url@
jdbc.user=@jdbc.user@
jdbc.password=@jdbc.password@
在Gradle构建过程中，有一个processResources的Task，可以修改该Task的配置，让其在构建过程中替换资源文件中的占位符：
processResources {
    from(sourceSets.main.resources.srcDirs) {
        filter(org.apache.tools.ant.filters.ReplaceTokens,
                		tokens: loadGroovyConfig()
)
    }
}
上面这种做法用来处理子项目src/main/resources文件夹下的资源文件，所以需要将这段代码放在子项目的独立配置文件里。
在一些复杂的项目中，经常会把配置文件放置到一个目录进行统一管理。比如在我所在的项目，就专门提供了一个config子目录，里面存放了所有的配置信息。在处理这些资源文件时， Gradle默认提供的processResources就不够用了，我们需要在Gradle脚本中定义一个Task去替换这些包含占位符的配置文件，然后让package或者deploy的Task依赖这个Task。该Task的代码如下：
task replace(type: Sync) {
            def configHome = "${project.rootDir}/config"
    from(configHome) {
        include '**/*.properties'
        include '**/*.xml'
        filter org.apache.tools.ant.filters.ReplaceTokens, 
tokens: loadGroovyConfig()
    }
    into "${buildDir}/resources/main"
}
这里定义了一个Sync类型的Task，会将父项目的根目录下的config文件夹的所有properties和xml文件使用从loadGroovyConfig()方法中加载出来的配置替换，并将替换之后的文件放到build文件夹下的resource/main目录中。再让打包的Task依赖这个Task，就会把替换之后的配置文件打到包中。
### 2.3 更复杂的情况
上面介绍了在项目中如何使用Gradle处理 properties和xml文件中具有相同配置，但其中的一些值并不相同的情况 。然而，在有些项目中不同的环境配置之间变化的不仅是值，很有可能整个配置文件都不相同；那么，使用上面替换的处理方式就无法满足要求了。
在我所在的项目中，我们需要依赖一个外部的Web Service。在开发环境上，我们使用了Stub来模拟和Web Service之间的交互，为开发环境提供测试数据，这些数据都放置在一个Spring的配置文件中；而在测试和产品环境上，又要使用对应的测试和产品环境的Web Service。这时，开发、测试与产品环境的配置完全不同。对于这种复杂的情况，Gradle可以在构建过程中为不同的环境指定不同的资源文件夹，在不同的资源文件夹中包含不同的配置文件。
例如，在我们项目的config目录下包含了application文件夹，定义了不同环境所需的不同配置文件，其目录结构如下图所示：
![](http://cdn4.infoqstatic.com/statics_s1_20150804-0226u3/resource/articles/Gradle-application-in-large-Java-projects/zh/resources/40.png)
在构建脚本中，根据从命令行读入的-P参数，使用不同的资源文件夹，其代码如下：
sourceSets {
    main {
        resources {
            srcDir "config/application/spring/${profile}", 
                        "config/application/properties/${profile}"
        }
    }
}
这样在打包的过程中，就可以使用-P传入的参数的资源文件夹下面的properties和xml文件作为项目的配置文件。
### 2.4 初始化数据库
在项目开发过程中，为了方便为不同环境构建相同的数据库及数据，我们通常需创建数据库的表以及插入一些初始化数据。Gradle目前没有提供相关的Task或者Plugin，但是我们可以自己创建Task去运行SQL来初始化各个环境上的数据库。
前面也提到Gradle是Groovy定义的DSL，所以我们可以在Gradle中使用Groovy的代码来执行SQL脚本文件。在Gradle脚本中，使用Groovy加载数据库的Driver之后，就可以使用Groovy提供的Sql类去执行SQL来初始化数据库了。代码如下：
groovy.sql.Sql oracleSql = 
	Sql.newInstance(props.getProperty('database.connection.url'),
                props.getProperty('database.userid'),
                props.getProperty('database.password'),
                props.getProperty('database.connection.driver'))
try {
        new File(script).text.split(";").each {
            logger.info it
            oracleSql.execute(it)
        }
    } catch (Exception e) { }
这段代码会初始化执行SQL的groovy.sql.Sql对象，然后按照分号（;）分割SQL脚本文件里的每一条SQL并执行。对于一些必须运行成功的SQL文件，可以在catch块里通过抛出异常来中止数据库的初始化。需要注意的是需要将数据库的Driver加载到ClassPath里才可以正确地执行。
因为在Gradle中包含了Ant，所以我们除了使用Groovy提供的API来执行SQL之外，还可以使用Ant的[sql任务](http://ant.apache.org/manual/Tasks/sql.html)来执行SQL脚本文件。但若非特殊情况，我并不推荐使用Ant任务，这部分内容与本文无关，这里不再细述 。
## 3. 代码质量
代码质量是软件开发质量的一部分，除了人工代码评审之外，在把代码提交到代码库之前，还应该使用自动检查工具来自动检查代码，来保证项目的代码质量。下面介绍一下Gradle提供的支持代码检查的插件 。
### 3.1 CheckStyle
CheckStyle是SourceForge下的一个项目，提供了一个帮助JAVA开发人员遵守某些编码规范的工具。它能够自动化代码规范检查过程，从而使得开发人员从这项重要却枯燥的任务中解脱出来。Gradle官方提供了[CheckStyle的插件](http://www.gradle.org/docs/current/userguide/checkstyle_plugin.html)，在Gradle的构建脚本中只需要应用该插件：
apply plugin: 'checkstyle'
默认情况下，该插件会找/config/checkstyle/checkstyle.xml作为CheckStyle的配置文件，可以在checkstyle插件的配置阶段（Configuration） 设置CheckStyle的配置文件：
checkstyle{
configFile = file('config/checkstyle/checkstyle-main.xml')
}
还可以通过checkstyle设置CheckStyle插件的其他配置。
### 3.2 FindBugs
FindBugs 是一个静态分析工具，它检查类或者 JAR 文件，将字节码与一组缺陷模式进行对比以发现可能的问题。Gradle使用如下的代码为项目的构建脚本添加FindBugs的插件：
apply plugin: 'findbugs'
同样也可以在FindBugs的配置阶段（Configuration）设置其相关的属性，比如Report的输出目录、检查哪些sourceSet等。
### 3.3 JDepend
在开发Java项目时经常会遇到关于包混乱的问题， JDepend工具可以帮助你在开发过程中随时跟踪每个包的依赖性（引用/被引用），从而设计高维护性的架构，不论是在打包发布还是版本升级都会更加轻松。在构建脚本中加入如下代码即可：
apply plugin: 'jdepend'
### 3.4 PMD
PMD是一种开源分析Java代码错误的工具。与其他分析工具不同的是，PMD通过静态分析获知代码错误，即在不运行Java程序的情况下报告错误。PMD附带了许多可以直接使用的规则，利用这些规则可以找出Java源程序的许多问题。此外，用户还可以自己定义规则，检查Java代码是否符合某些特定的编码规范。在构建脚本中加入如下代码：
apply plugin: 'pmd'
### 3.5 小结
上面提到的几种代码检查插件apply到构建脚本之后，可以运行：
gradle check
来执行代码质量检查。更详细的信息请查阅[Gradle的官方文档](http://www.gradle.org/docs/current/userguide/userguide.html)。运行结束后会在对应的项目目录下的build文件夹下生成report。
对于Gradle没有提供的代码检查工具，我们可以有两种选择：第一就是自己实现一个[Gradle插件](http://www.gradle.org/docs/current/userguide/custom_plugins.html)，第二就是调用Ant任务，让Ant作为一个媒介去调用在Ant中已经有的代码检查工具，比如测试覆盖率的[Cobertura](http://cobertura.sourceforge.net/)。我们的项目使用了Ant来调用Cobertura，但是为了使用方便，我们将它封装为一个Gradle插件，这样就可以在不同的项目里重用。
## 4. 依赖
几乎每个Java项目都会用到开源框架。同时，对于具有多个子模块的项目来说，项目之间也会有所依赖。所以，管理项目中对开源框架和其他模块的依赖是每个项目必须面对的问题。同时，Gradle也使用Repository来管理依赖。
### 4.1 Jar包依赖管理
Maven提出了使用Repository来管理Jar包，Ant也提供了使用Ivy来管理jar包。Gradle提供了对所有这些Respository的支持，可以从[Gradle的官方文档](http://www.gradle.org/docs/current/userguide/dependency_management.html#sec:repositories)上了解更详细的信息。
Gradle沿用Maven的依赖管理方法，通过groupId、name和version到配置的Repository里寻找指定的Jar包。同样，它也提供了和Maven一样的构建生命周期，compile、runtime、testCompile和testRuntime分别对应项目不同阶段的依赖。通过如下方式为构建脚本指定依赖：
dependencies {
    compile group: 'org.hibernate', name: 'hibernate-core', version: '3.6.7.Final'
    testCompile group:'junit', name: 'junit', version '4.11'
}
这里分别指定group、name以及version，但是Gradle提供了一种更简单的方式来指定依赖：
dependencies {
    compile 'org.hibernate:hibernate-core:3.6.7.Final'
    testCompile 'junit:junit:4.11'
}
这样比Maven使用XML来管理依赖简单多了，但是还可以更简单一点。实际上这里的compile和testCompile是Groovy为Gradle提供的方法，可以为其传入多个参数，所以当compile有多个Jar包依赖的时候，可以同时指定到compile里去，代码如下：
compile(
	       'org.hibernate:hibernate-core:3.6.7.Final',
            'org.springframework:spring-context:3.1.4.RELEASE'
)
另外，当在Respository无法找到Jar包时（如数据库的driver），就可以将这些Jar包放在项目的一个子目录中，然后让项目管理依赖。例如，我们可以在项目的根目录下创建一个lib文件夹，用以存放这些Jar包。使用如下代码可以将其添加到项目依赖中：
dependencies {
    compile(
	       'org.hibernate:hibernate-core:3.6.7.Final',
            'org.springframework:spring-context:3.1.4.RELEASE',
	       fileTree(dir: "${rootProject.projectDir}/lib", include: '*.jar')
)
}
### 4.2 子项目之间的依赖
对于多模块的项目，项目中的某些模块需要依赖于其他模块，前面提到在初始化阶段，Gradle为每个模块都创建了一个Project对象，并且可以通过模块的名字引用到该对象。在配置模块之间的依赖时，使用这种方式可以告诉Gradle当前模块依赖了哪些子模块。例如，在我们的项目中，cis-war会依赖core子项目，就可以在cis-war的构建脚本中加上如下代码：
dependencies {
    compile(
	       'org.hibernate:hibernate-core:3.6.7.Final',
             project(':core')
)
}
通过project(':core')来引用core子项目，在构建cis-war时，Gradle会把core加到ClassPath中。
### 4.3 构建脚本的依赖
除了项目需要依赖之外，构建脚本本身也可以有自己的依赖。当使用一个非Gradle官方提供的插件时，就需要在构建脚本里指定其依赖，当然还需要指定该插件的Repository。在Gradle中，使用buildscript块为构建脚本配置依赖。
比如在项目中使用[cucumber-JVM](https://github.com/cucumber/cucumber-jvm)作为项目BDD工具，而Gradle官方没有提供它的插件，好在开源社区有人提供[cucumber的插件](https://github.com/samueltbrown/gradle-cucumber-plugin)。在构建脚本中添加如下代码：
buildscript {
    repositories {
        mavenCentral()
    }
    dependencies {
        classpath "gradle-cucumber-plugin:gradle-cucumber-plugin:0.2"
    }
}
apply plugin: com.excella.gradle.cucumber.CucumberPlugin
## 5. 其他
### 5.1 apply其他Gradle文件
当一个项目很复杂的时候，Gradle脚本也会很复杂，除了将子项目的配置移到对应项目的构建脚本之外，还可以可以按照不同的功能将复杂的构建脚本拆分成小的构建脚本，然后在build.gradle里使用apply from，将这些小的构建脚本引入到整体的构建脚本中去。比如在一个项目中既使用了Jetty，又使用了Cargo插件启动JBoss，就可以把他们分别提到jetty.gradle和jboss.gradle，然后在build.gradle里使用如下的代码将他们引入进来：
apply from: "jetty.gradle"
apply from: "jboss.gradle"
### 5.2 project的目录
在脚本文件中，需要访问项目中的各级目录结构。Gradle为Project对象定义了一些属性指向项目的根目录，方便在脚本中引用。
- rootDir：在子项目的脚本文件中可以通过该属性访问到根项目路径。
- rootProject：在子项目中，可以通过该属性获取父项目的Project对象。
### 5.3 使用Wrapper指定Gradle的版本
为了统一项目中Gradle的版本，可以在构建脚本中通过定义一个wrapper的Task，并在该Task中指定Gradle的版本以及存放Gradle的位置。
task wrapper(type: Wrapper) {
    gradleVersion = '1.0'
    archiveBase = 'PROJECT'
    archivePath = 'gradle/dists'
}
运行gradle wrapper， 就会在根项目目录下创建一个wrapper的文件夹，会包含wrapper的Jar包和properties文件。之后就可以使用gradlew来运行task。第一次使用gradlew执行task的时候，会在项目根目录下的gradle/dists下下载你指定的Gradle版本 。这样在项目构建的时候，就会使用该目录下的Gradle，保证整个团队使用了相同的Gradle版本。
### 5.4 使用gradle.properties文件
Gradle构建脚本会自动找同级目录下的gradle.properties文件，在这个文件中可以定义一些property，以供构建脚本使用。例如，我们要使用的Repository需要提供用户名和密码，就可以将其配置在gradle.properties中。这样，每个团队成员都可以修改该配置文件，却不用上传到代码库中对团队其他成员造成影响。可以使用如下的代码定义：
username=user
password=password
在构建脚本中使用"${username} "就可以访问该文件中定义的相关值。
由于篇幅有限，本文只是我在一个大型Java项目上使用Gradle的部分经验，并未涵盖所有Gradle相关的知识，包括如何编写Gradle插件以及Gradle对其他语言的构建，读者可以通过阅读[Gradle的官方文档](http://www.gradle.org/documentation)（比起其他开源软件，Gradle的另一特点就是文档详细）来了解。另外，Gradle是基于Groovy的构建工具，在使用Gradle的时候也需要了解和使用Groovy。所以，在学习Gradle插件的过程中，也能学会Groovy相关的用法，可谓一举两得。
## 参考文献:
[1] CoC: http://en.wikipedia.org/wiki/Convention_over_configuration
[2] DSL: http://en.wikipedia.org/wiki/Domain-specific_language
[3] Micro Service Architecture: http://yobriefca.se/blog/2013/04/29/micro-service-architecture/
[4] Guava: [https://code.google.com/p/guava-libraries/](https://code.google.com/p/guava-libraries/)
