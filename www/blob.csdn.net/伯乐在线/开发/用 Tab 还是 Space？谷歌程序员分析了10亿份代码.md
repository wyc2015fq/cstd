# 用 Tab 还是 Space？谷歌程序员分析了10亿份代码 - 文章 - 伯乐在线
本文由 [伯乐在线](http://blog.jobbole.com) - [小米云豆粥](http://www.jobbole.com/members/buntamarui) 翻译，[黄利民](http://www.jobbole.com/members/huanglimin) 校稿。未经许可，禁止转载！
英文出处：[Felipe Hoffa](https://medium.com/@hoffa/400-000-github-repositories-1-billion-files-14-terabytes-of-code-spaces-or-tabs-7cfe0b5dd7fd#.s0dzkag2f)。欢迎加入[翻译组](https://github.com/jobbole/translation-project)。
【伯乐在线导读】：用 Tab 还是 Space？事关程序员的信仰之争。[2015 年程序员 Uku Pattak 分析了 GitHub 上多种语言的热门项目（star 数量高）中使用 Tab 和空格的对比情况](http://blog.jobbole.com/96174/)。2016 年，谷歌程序员 Felipe Hoffa 分析了 10 亿份代码。
分析结果可先看下图：
![](http://ww4.sinaimg.cn/large/7cc829d3gw1f7hfunzewxj20gg0att9k.jpg)
### 40万个repo, 10亿份代码文件, 14TB 代码
**规则：**
- **数据来源**：已[存储在 BigQuery 上的 GIthub 文档](https://medium.com/google-cloud/github-on-bigquery-analyze-all-the-code-b3576fd2b150)
- **Star 很重要**：只选择了2016年1月到5月之间加星量（star）在前 40 万个的项目
- **排除小文件**：代码文件至少得有 10 行代码，开始的字符是：空格或者制表符
- **去除冗余**：不管它们存在于多少个项目中，相同的程序文件只算一次
- **一个文件一票**：有些代码文件中混用了制表符和空格。如果该文件制表符用的多，那它就算制表符一票。反之亦然。
- **只考虑使用广泛的编程语言**：我只选取了有下面这些后缀的文件：(.java, .h, .js, .c, .php, .html, .cs, .json, .py, .cpp, .xml, .rb, .cc, .go)
**数据**
![](http://ww2.sinaimg.cn/large/7cc829d3gw1f7hfuorpcxj20ba08s76c.jpg)
**实现方法**
我使用了已有的 [bigquery-public-data:github_repos.sample_files] 表。这个表列出了排名最前的 40 万个项目。从中提取了含有被广泛使用的语言后缀的文件。


```
SELECT a.id id, size, content, binary, copies, sample_repo_name , sample_path
FROM (
  SELECT id, FIRST(path) sample_path, FIRST(repo_name) sample_repo_name 
  FROM [bigquery-public-data:github_repos.sample_files] 
  WHERE REGEXP_EXTRACT(path, r'.([^.]*)$') IN ('java','h','js','c','php','html','cs','json','py','cpp','xml','rb','cc','go')
  GROUP BY id
) a
JOIN [bigquery-public-data:github_repos.contents] b
ON a.id=b.id
```


```
864.6s elapsed, 1.60 TB processed
```
由于我要联结分别有 1.97 亿行和 0.7 亿行的两个表（一共有 1.6T 数据量），这个过程花费了不少时间。我把结果放在这里了[[fh-bigquery:github_extracts.contents_top_repos_top_langs](https://bigquery.cloud.google.com/table/fh-bigquery:github_extracts.contents_top_repos_top_langs)]，你们就不用重新跑这个过程了。
在 [contents] 表格中，包含了去冗余之后的所有文件。下面的命令行用来检查文件数量和大小。


```
SELECT SUM(copies) total_files, SUM(copies*size) total_size
FROM [fh-bigquery:github_extracts.contents_top_repos_top_langs]
```
![](http://ww3.sinaimg.cn/large/7cc829d3gw1f7hfupg0jrj20m807f3zy.jpg)
*10 亿文件，14TB代码*
然后开始按照前面提到的规则排序：


```
SELECT ext, tabs, spaces, countext, LOG((spaces+1)/(tabs+1)) lratio
FROM (
  SELECT REGEXP_EXTRACT(sample_path, r'.([^.]*)$') ext, 
         SUM(best='tab') tabs, SUM(best='space') spaces, 
         COUNT(*) countext
  FROM (
    SELECT sample_path, sample_repo_name, IF(SUM(line=' ')>SUM(line='t'), 'space', 'tab') WITHIN RECORD best,
           COUNT(line) WITHIN RECORD c
    FROM (
      SELECT LEFT(SPLIT(content, 'n'), 1) line, sample_path, sample_repo_name 
      FROM [fh-bigquery:github_extracts.contents_top_repos_top_langs]
      HAVING REGEXP_MATCH(line, r'[ t]')
    )
    HAVING c>10 # at least 10 lines that start with space or tab
  )
  GROUP BY ext
)
ORDER BY countext DESC
LIMIT 100
```


```
16.0s elapsed, 133 GB processed
```
分析 133G 代码只有 16 秒？我爱 BigQuery。
### 最后 Felipe Hoffa 还贴上一个视频
> 
在热播剧《硅谷》第三季中，不仅有 Tab 和 Space 之争，也有 Vim 和 Emacs……结果导致一对程序员做不成情侣了
视频亮点在结尾 :)
> 
**打赏支持我翻译更多好文章，谢谢！**
[打赏译者](#rewardbox)
#### 打赏支持我翻译更多好文章，谢谢！
任选一种支付方式
![](http://jbcdn2.b0.upaiyun.com/2016/07/2ec965670e006b5d53c221a7cdcf0108.jpg)![](http://jbcdn2.b0.upaiyun.com/2016/07/a5e36df34a1aea088f06fb503c7ce93d.jpg)
