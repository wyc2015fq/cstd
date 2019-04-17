# Deploy to Maven Central Repository - 纸上得来终觉浅，绝知此事要躬行 - CSDN博客





2015年04月09日 09:41:32[boonya](https://me.csdn.net/boonya)阅读数：1223标签：[maven																[source code																[idea																[nexus																[sonatype](https://so.csdn.net/so/search/s.do?q=sonatype&t=blog)
个人分类：[Wise reading](https://blog.csdn.net/boonya/article/category/1628867)






Thanks for Lubos Krnac sharing this article.

**He had described himself like below:**

**![Author, Lubos Krnac](http://www.sonatype.org/nexus/content/uploads/2015/01/LubosKrnac-590x350.jpg)**

I am a Java/JavaScript developer. My religion is to constantly
 improve my development skills according to best practices. I strongly believe that TDD drives better design and nicely decoupled code. Past experience includes C++, Assembler and C#. You can find my blog at http://lkrnac.net/ .


This Article is From :[http://www.sonatype.org/nexus/2015/01/08/deploy-to-maven-central-repository/](http://www.sonatype.org/nexus/2015/01/08/deploy-to-maven-central-repository/)


Do you need to make your Java library publicly accessible? Is your project hosted on GitHub? Do you like idea of “all-in-one deploy to [Maven Central
 Repository](http://central.sonatype.org/)” button? I am going to show how to set it up with usage of [maven-release-plugin](http://maven.apache.org/maven-release/maven-release-plugin/). Source code is hosted
 on GitHub, so SSH access to source control will be also described.


There are various steps needed to set up environment for your project. I followed a lot of steps from [official
 Sonatype guide](http://central.sonatype.org/pages/ossrh-guide.html), therefore I will refer to it when needed. But this guide also describes a lot of techniques that are not needed for configuring a “deploy to Maven Central repository” button.

# Consideration


My first approach was to create a Jenkins job that would represent the “Deploy to Maven Central” button. This idea is much cleaner for me  than pushing from my development machine. [I
 tried hard](https://lkrnac.ci.cloudbees.com/job/patere-deploy/) but couldn’t make [Jenkins SSH Agent plugin](https://wiki.jenkins-ci.org/display/JENKINS/SSH+Agent+Plugin) register my SSH identity when the maven-release-plugin
 is pushing a version update to GitHub. Therefore, I created a very simple Linux Bash script that involves two steps.

# 1. Create Sonatype JIRA ticket for registration of your groupId


Sonatype drives one of the biggest Nexus repositories. It is needed to deploy Java artifacts before it can be synced into the Maven Central repository. In order to release the Java library into a Sonatype Nexus repository, a  JIRA ticket is needed to create
 your user account and setup permissions to publish your own projects. The Sonatype guide includes description on how to create it.


Pick up the Maven groupId reflecting your top level domain. It is typically the same as the main Java package (e.g. *com.google*, not *com.google.guava*). This is because you don’t want to create a JIRA ticket for each project/library under your
 domain/main package/groupId.  [Here is more reading about Java package naming conventions.](http://docs.oracle.com/javase/tutorial/java/package/namingpkgs.html)


This step is only necessary a first. Once your account is set up, you can do as many releases and deployments of different projects under that groupId as you want.

# 2. Configure your pom.xml


In the past a parent pom was the suggested use case. However the single inheritance of Maven project caused issues for many users and it is now best practice to manage all configuration in your own pom.


Luckily this setup is [well documented](http://central.sonatype.org/pages/apache-maven.html)and can easily be added to your project pom or for reuse across multiple projects to your organization pom.



# 3. Configure GPG signing of Maven artifact

This is needed because of security policy of Maven Central repository. Include various sub-steps:
- Installing GnuPG tool
- Generate a key pair
- Distribute your public key
- Set up signing of artifact in **settings.xml** (your local repository configuration).

Working with GPG is documented in more detail in the [Central Repository documentation as well](http://central.sonatype.org/pages/working-with-pgp-signatures.html).




```xml
<
```

```xml
profiles
```

```xml
>
```



```xml

```

```xml
<
```

```xml
profile
```

```xml
>
```



```xml

```

```xml
<
```

```xml
id
```

```xml
>ossrh</
```

```xml
id
```

```xml
>
```



```xml

```

```xml
<
```

```xml
properties
```

```xml
>
```



```xml

```

```xml
<
```

```xml
gpg.keyname
```

```xml
>F21879F3</
```

```xml
gpg.keyname
```

```xml
>
```



```xml

```

```xml
<
```

```xml
gpg.passphrase
```

```xml
>*********</
```

```xml
gpg.passphrase
```

```xml
>
```



```xml

```

```xml
<
```

```xml
gpg.defaultKeyring
```

```xml
>false</
```

```xml
gpg.defaultKeyring
```

```xml
>
```



```xml

```

```xml
<
```

```xml
gpg.useagent
```

```xml
>true</
```

```xml
gpg.useagent
```

```xml
>
```



```xml

```

```xml
<
```

```xml
gpg.lockMode
```

```xml
>never</
```

```xml
gpg.lockMode
```

```xml
>
```



```xml

```

```xml
<
```

```xml
gpg.homedir
```

```xml
>/home/lkrnac/.gnupg</
```

```xml
gpg.homedir
```

```xml
>
```



```xml

```

```xml
<
```

```xml
gpg.publicKeyring
```

```xml
>/home/lkrnac/.gnupg/pubring.gpg</
```

```xml
gpg.publicKeyring
```

```xml
>
```



```xml

```

```xml
<
```

```xml
gpg.secretKeyring
```

```xml
>/home/lkrnac/.gnupg/secring.gpg</
```

```xml
gpg.secretKeyring
```

```xml
>
```



```xml

```

```xml
</
```

```xml
properties
```

```xml
>
```



```xml

```

```xml
</
```

```xml
profile
```

```xml
>
```



```xml
</
```

```xml
profiles
```

```xml
>
```




F21879F3 is my public GPG key ID. **gpg –list-keys** command would list it for you. Passphase is password for your private GPG key. **.gnupg** folder is typically generated in your home directory.


# 4. Set up your credentials for Sonatype Nexus repository

Needed for push to Sonatype Nexus repository. Same as Sonatype JIRA credentials. Again in **settings.xml**:


```xml
<
```

```xml
servers
```

```xml
>
```



```xml

```

```xml
<
```

```xml
server
```

```xml
>
```



```xml

```

```xml
<
```

```xml
id
```

```xml
>ossrh</
```

```xml
id
```

```xml
>
```



```xml

```

```xml
<
```

```xml
username
```

```xml
>lkrnac</
```

```xml
username
```

```xml
>
```



```xml

```

```xml
<
```

```xml
password
```

```xml
>*************</
```

```xml
password
```

```xml
>
```



```xml

```

```xml
</
```

```xml
server
```

```xml
>
```



```xml
</
```

```xml
servers
```

```xml
>
```





### 5. Fill pom.xml mandatory sections

### 6. Set up SSH access to GitHub

Follow these sub-steps:
- [Generate new SSH key](https://help.github.com/articles/generating-ssh-keys#step-2-generate-a-new-ssh-key)
- [Add public SSH key
 to GitHub](https://help.github.com/articles/generating-ssh-keys#step-3-add-your-ssh-key-to-github)

### 7. Create “Deploy to Maven Central” button

Maven-release-plugin needs to push/check-in version update to source control system and tag the revision. In my case the code is hosted on GitHub. Therefore I need to register my SSH identity (generated in previous
 step) before invoking maven-release-plugin’s goals. This is done by bash script **maven-central-deploy.sh** located alongside **pom.xml**:


```bash
!
```

```bash
/bin/bash
```



```bash
#
 Deploy maven artefact in current directory into Maven central repository
```



```bash
#
 using maven-release-plugin goals
```





```bash
read
```

```bash
-p
```

```bash
"Really
 deploy to maven central repository  (yes/no)? "
```





```bash
if
```

```bash
(
 [
```

```bash
"$REPLY"
```

```bash
==
```

```bash
"yes"
```

```bash
]
 )
```

```bash
then
```



```bash

```

```bash
ssh
```

```bash
-add
 ~/.
```

```bash
ssh
```

```bash
/lubos
```

```bash
.krnac
```



```bash

```

```bash
ssh
```

```bash
-add
 -l
```



```bash

```

```bash
mvn
 release:clean release:prepare release:perform -B -e |
```

```bash
tee
```

```bash
maven-central-deploy.log
```



```bash

```

```bash
ssh
```

```bash
-add
 -D
```



```bash
else
```



```bash

```

```bash
echo
```

```bash
'Exit
 without deploy'
```



```bash
fi
```







Make the script executable by: 
```bash
chmod
```

```bash
+x
 maven-central-deploy.sh
```




#  8. Push the “Deploy to Maven Central” button

Just run: 
```bash
.
```

```bash
/maven-central-deploy
```

```bash
.sh
```


confirm and enter SSH private key passphase if required.

# 9. Release artifact via Sonatype Nexus repository

When the artifact is pushed to Sonatype Nexus repository like this, you need to [release
 it manually](http://central.sonatype.org/pages/releasing-the-deployment.html). You can also setup [automatic releases with the usage of the Nexus Staging plugin](http://central.sonatype.org/pages/apache-maven.html). Finally you need to add comments to the Sonatype JIRA issue you created
 at the beginning and wait until somebody reviews your artifact and sets up the sync to Maven Central. Of course the JIRA part would need to be done only once.

### Link

Example project configured this way is [hosted on GitHub](https://github.com/lkrnac/patere). (It’s tiny testing library).








The following two tabs change content below.](https://so.csdn.net/so/search/s.do?q=nexus&t=blog)](https://so.csdn.net/so/search/s.do?q=idea&t=blog)](https://so.csdn.net/so/search/s.do?q=source code&t=blog)](https://so.csdn.net/so/search/s.do?q=maven&t=blog)




