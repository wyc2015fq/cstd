# 备份LeetCode OJ自己编写的代码 - 在思索中前行！ - CSDN博客





2016年03月27日 22:17:10[_Tham](https://me.csdn.net/txl16211)阅读数：1487







常泡LC的朋友知道LC是不提供代码打包下载的，不像一般的OJ，可是我不备份代码就感觉不舒服…
其实我想说的是…
我自己写了抓取个人提交代码的小工具，放在GitCafe上了…
不知道大家有没有兴趣
[https://gitcafe.com/aaronzhou/
 ... ssion](https://gitcafe.com/aaronzhou/FetchLeetcodeSubmission)

拿Java写的，也打了一个包，本地有JRE环境就能运行，欢迎拍砖






# 抓取 LeetCode OJ 个人提交的代码
- 
Gradle 构建项目
- 
HttpClient、jsoup 抓取、解析网页

## [](https://gitcafe.com/aaronzhou/FetchLeetcodeSubmission#%E4%BD%BF%E7%94%A8%E8%AF%B4%E6%98%8E)使用说明
- 
进入`release`文件夹
- 
修改`init.properties` 中的配置


        #登录账号
	username=your_username

	#登录密码
	password=your_password

	#登录类型，值暂时只能为leetcode或github
	logintype=leetcode

	#抓取的代码在本地的存储目录
        #注意目录路径最后需有一个斜杠/
	dirpath=/path/to/your/own/directory/

	#是否抓取所有的提交代码
	#为假时参考tags集合的取值有选择的抓取
	isfetchall=false

	#抓取与集合中状态匹配的代码
	tags=Accepted,Wrong Answer,Time Limit Exceeded


- 
命令行中运行 
```
java
 -jar FetchLeetcodeSubmission.jar
```
 抓取代码
- 
输出的代码文件，内容依次为
- `本项目信息`
- `题目标题`
- `题目URL`
- `你提交的代码`
- `题目内容`


## [](https://gitcafe.com/aaronzhou/FetchLeetcodeSubmission#%E9%A1%B9%E7%9B%AE%E8%AF%B4%E6%98%8E)项目说明
- 
使用
```
gradle
 release
```
可直接打包最新项目到`release`文件夹


jar {
    from { configurations.compile.collect { it.isDirectory() ? it : zipTree(it) } }
    manifest {
        attributes 'Main-Class': 'cc.aaron67.fetch.leetcode.main.Main'
    }
}

task release(type: Copy) {
	from 'build/libs'
	into 'release'
}

task copyConfig(type: Copy) {
	from 'init.properties'
	into 'release'
}

release.dependsOn 'build', 'copyConfig'







