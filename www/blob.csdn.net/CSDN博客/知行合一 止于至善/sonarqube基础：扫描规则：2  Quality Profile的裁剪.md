
# sonarqube基础：扫描规则：2: Quality Profile的裁剪 - 知行合一 止于至善 - CSDN博客

2018年11月03日 07:20:51[liumiaocn](https://me.csdn.net/liumiaocn)阅读数：1968所属专栏：[DevOps系列之：持续构建与集成](https://blog.csdn.net/column/details/27057.html)



关于Sonarqube的扫描规则，在上篇文章中介绍了Java的Sonar Way的详细信息，这篇文章来介绍一下如何在Sonar way的基础上生成定制的Quality Profile以及使用相关的Restapi进行生成。
# Quality Profile
从SonarQube中使用back up按钮导出来的规则列表的Quality Profile文件，在对其格式进行整理之后，大体如下所示：
`<?xml version='1.0' encoding='UTF-8'?>
<profile>
    <name>Sonar way</name>
    <language>java</language>
    <rules>
        <rule>
            <repositoryKey>common-java</repositoryKey>
            <key>DuplicatedBlocks</key>
            <priority>MAJOR</priority>
            <parameters/>
        </rule>
        ...省略
    </rules>
</profile>`
# 生成定制的Profile
## 方法1: 使用页面进行操作
这里使用导出来的Sonarway的文件作为输入，只修改Profile的name字段信息，修改前
`<name>Sonar way</name>`将其进行修改，修改为如下内容
`<name>Sonar java profile : all rules by UI operation</name>`生成Profile的方法：
Quality Profiles -> Create -> Restore Profiles
![在这里插入图片描述](https://img-blog.csdnimg.cn/20181102213011770.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L2xpdW1pYW9jbg==,size_16,color_FFFFFF,t_70)
单击Restore，即可生成了修改了名称的Java的Sonar的新的Profile
![在这里插入图片描述](https://img-blog.csdnimg.cn/20181102213218467.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L2xpdW1pYW9jbg==,size_16,color_FFFFFF,t_70)
## 方法2: 使用RestApi生成Profile
### 事前准备
准备如下定制Profile的xml文件，包含3条规则，1个bug，2个Code Smell
`liumiaocn:Desktop liumiao$ cat sonar-java-test-profile.xml 
<?xml version='1.0' encoding='UTF-8'?>
<profile>
    <name>sonar-java-test-profile</name>
    <language>java</language>
    <rules>
        <rule>
            <repositoryKey>common-java</repositoryKey>
            <key>DuplicatedBlocks</key>
            <priority>MAJOR</priority>
            <parameters/>
        </rule>
        <rule>
            <repositoryKey>common-java</repositoryKey>
            <key>InsufficientBranchCoverage</key>
            <priority>MAJOR</priority>
            <parameters>
                <parameter>
                    <key>minimumBranchCoverageRatio</key>
                    <value>65.0</value>
                </parameter>
            </parameters>
        </rule>
        <rule>
            <repositoryKey>squid</repositoryKey>
            <key>AssignmentInSubExpressionCheck</key>
            <priority>MAJOR</priority>
            <parameters/>
        </rule>
    </rules>
</profile>liumiaocn:Desktop liumiao$`
### 执行命令
> curl -X POST -uadmin:admin
> [http://localhost:32003/api/qualityprofiles/restore](http://localhost:32003/api/qualityprofiles/restore)
> --form backup=@sonar-java-test-profile.xml
执行命令说明：
-u指定用户名和密码
POST方式使用/api/qualityprofiles/restore
backup为必须指定内容
生成的profile名称为xml中设定的内容
如果当前profile已经存在则会覆盖生成
### 执行日志与结果确认
为了结果容易确认，这里传给了jq，没有安装的可以忽略
`liumiaocn:Desktop liumiao$ curl -X POST -uadmin:admin http://localhost:32003/api/qualityprofiles/restore --form backup=@sonar-java-test-profile.xml |jq .
...省略
{
  "profile": {
    "key": "java-sonar-java-test-profile-83122",
    "name": "sonar-java-test-profile",
    "language": "java",
    "isDefault": false,
    "isInherited": false,
    "languageName": "Java"
  },
  "ruleSuccesses": 3,
  "ruleFailures": 0
}
liumiaocn:Desktop liumiao$`从这里可以看到，生成了language为Java的name名称为sonar-java-test-profile的Quality Profile，其中成功导入规则（ruleSuccesses）3条。
![在这里插入图片描述](https://img-blog.csdnimg.cn/20181103063242149.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L2xpdW1pYW9jbg==,size_16,color_FFFFFF,t_70)
### 使用restapi查询profile信息
可以查询相关profile的信息，比如C\#语言相关的profile信息
`liumiaocn:Desktop liumiao$ curl http://localhost:32003/api/qualityprofiles/search?language='cs' |jq .
...省略
{
  "profiles": [
    {
      "key": "cs-sonar-way-44636",
      "name": "Sonar way",
      "language": "cs",
      "languageName": "C#",
      "isInherited": false,
      "isDefault": true,
      "activeRuleCount": 101,
      "rulesUpdatedAt": "2018-08-25T01:12:03+0000"
    }
  ]
}
liumiaocn:Desktop liumiao$`
### 删除profile
可以用POST的方式调用/api/qualityprofiles/delete 进行删除，但是注意由于在sonar中使用profileKey来唯一确定，为了避免误删其他语言中同名的profile，在使用profileName进行删除的时候需要结合使用language，不然会得到如下的提示：
`liumiaocn:Desktop liumiao$ curl -X POST http://localhost:32003/api/qualityprofiles/delete -d "profileName=sonar-java-test-profile" -uadmin:admin
{"errors":[{"msg":"Either profileKey or profileName + language must be set"}]}liumiaocn:Desktop liumiao$ 
liumiaocn:Desktop liumiao$`加上language即可删除，当然也可以只指定profileKey来进行删除
`liumiaocn:Desktop liumiao$ curl -X POST http://localhost:32003/api/qualityprofiles/delete -d "profileName=sonar-java-test-profile&language=java" -uadmin:admin
liumiaocn:Desktop liumiao$`
# 总结
使用restapi方式或者直接在页面进行操作，可以生成定制的Quality Profile，可以根据项目具体情况对Quality Profile进行裁剪和加载使用。

[
  ](https://img-blog.csdnimg.cn/20181102213011770.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L2xpdW1pYW9jbg==,size_16,color_FFFFFF,t_70)