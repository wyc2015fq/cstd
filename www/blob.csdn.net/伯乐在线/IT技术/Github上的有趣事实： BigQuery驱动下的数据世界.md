# Github上的有趣事实："BigQuery驱动下的数据世界" - 文章 - 伯乐在线
原文出处： [askfermi（ @贰亲王666 ）](http://blog.askfermi.me/2016/07/01/interesting-github/)
近日看到了Google的BigQuery技术，用类似SQL的语言来做一些数据的分析。而Github也加入了进来。闲来无事，就去体验了一把。https://cloud.google.com/bigquery/public-data/github 这是相关文章的地址。
### BigQuery语法


```
SELECT
  package,
  COUNT(*) count
FROM (
  SELECT
    REGEXP_EXTRACT(line, r' ([a-z0-9\._]*)\.') package,
    id
  FROM (
    SELECT
      SPLIT(content, '\n') line,
      id
    FROM
      [bigquery-public-data:github_repos.sample_contents]
    WHERE
      content CONTAINS 'import'
      AND sample_path LIKE '%.py'
    HAVING
      LEFT(line, 6)='import' )
  GROUP BY
    package,
    id )
GROUP BY
  1
ORDER BY
  count DESC
LIMIT
  40;
```
整体的语法都和SQL非常接近，鉴于点进来的人很多都是冲着“有趣”这两个字来的，就不过多介绍代码了。这段代码的作用是找到github仓库中，使用Python语言的仓库中，前10个被引用的库。
BigQuery的界面也非常有Google风…… ![github_20160702000047.png](https://ooo.0o0.ooo/2016/07/01/577695245abdb.png)
### Most Popular Python Packages
这是通过刚刚的分析做出来的前几名的Python库的列表。 ![a5037d6edd3b9ca5 (2).png](https://ooo.0o0.ooo/2016/07/01/5776947692ff9.png)
### Most Popular Java Packages
![a5037d6edd3b9ca5 (3).png](https://ooo.0o0.ooo/2016/07/01/577698eb17973.png)
### 一些其它有趣的事情
例如大家都在用什么协议来发布作品，有接近150万个仓库使用MIT协议来发布。 ![a5037d6edd3b9ca5 (4).png](https://ooo.0o0.ooo/2016/07/01/57769f3c9b10c.png)
关注tensorflow的人都在关注什么？ ![a5037d6edd3b9ca5 (5).png](https://ooo.0o0.ooo/2016/07/01/5776a076be634.png)
最受欢迎的语言 ![1-_uxwTTOd1oRXjmloZ5Qi0g.png](https://ooo.0o0.ooo/2016/07/01/5776a104a17d5.png)
提交次数和时间的关系 ![1-Ud8ZAz1xoVWK9DfiX6_hBg.png](https://ooo.0o0.ooo/2016/07/01/5776a1049aeb3.png)
