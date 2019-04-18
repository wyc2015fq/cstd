# 推荐系统：MovivLens20M数据集解析 - wishchinYang的专栏 - CSDN博客
2017年09月22日 10:59:02[wishchin](https://me.csdn.net/wishchin)阅读数：1861
个人分类：[推荐/Rank系统																[ReinforceLearning](https://blog.csdn.net/wishchin/article/category/5783969)](https://blog.csdn.net/wishchin/article/category/7149402)
          MovieLens 是历史最悠久的推荐系统。它由美国 Minnesota 大学计算机科学与工程学院的 GroupLens 项目组创办，是一个非商业性质的、以研究为目的的实验性站点。MovieLens 主要使用 Collaborative Filtering 和 Association Rules 相结合的技术，向用户推荐他们感兴趣的电影。 
   参考资料：[movieLens-百度百科](https://baike.baidu.com/item/MovieLens/1705896?fr=aladdin)、[电影数据集总结](http://write.blog.csdn.net/%E7%94%B5%E5%BD%B1%E6%95%B0%E6%8D%AE%E9%9B%86%E6%80%BB%E7%BB%93%EF%BC%9ANetflix%E3%80%81MovieLens%E3%80%81LDOS-CoMoDa%E3%80%81AdomMovie)
数据集地址：[https://grouplens.org/datasets/movielens/](https://grouplens.org/datasets/movielens/)
          This dataset (ml-20m) describes 5-star rating and free-text tagging activity from [MovieLens](http://movielens.org), a movie recommendation service. It contains 20000263 ratings and 465564 tag applications across 27278 movies. These data were created
 by 138493 users between January 09, 1995 and March 31, 2015. This dataset was generated on March 31, 2015, and updated on October 17, 2016 to update links.csv and add genome-* files.
        Users were selected at random for inclusion. All selected users had rated at least 20 movies.
No demographic information is included. Each user is represented by an id, and no other information is provided.
        The data are contained in six files, `genome-scores.csv`, `genome-tags.csv`, `links.csv`, `movies.csv`, `ratings.csv` and `tags.csv`. More details about the contents and use of all these files follows.
         This and other GroupLens data sets are publicly available for download at <http://grouplens.org/datasets/>.。
   此数据集描述了5星之内的电影不受限制的标记，用于给出用户推荐。数据集包含了138493个用户对27278个电影的20000263个评分和465564个标签。此评价收集于1995年1月到2015年3月之间，并在2016年10月17日更新为csv格式。
   用户为随机选取，每个选取的用户至少评分20个电影。没有人口统计信息。每个用户只给出一个ID，且不涉及其他私人信息。
## **数据格式**
      movieLens20M使用了CSV格式存储数据列表，代替了10M和1M、100K的DAT格式，可以直接可视化分析。
文件列表：genome_scores.csv、genome-tags.csv、links.csv、movies.csv、ratings.csv、tags.csv。
      movies.csv：MovieId+title+geners。以此表示电影ID、电影名称、电影流派/种类。其中电影流派具有多个标签，即可以表示电影的多个属性。
用以生成电影属性矩阵。
      Rating.csv：userId+movieId+rating+timestamp。分别表示用户ID、电影ID、评分，以及截至时间戳。给出了用户对电影的评分列表。
用以生成用户-电影评分矩阵。
      Trgs.csv: userId+ movieId+tag+timestamp。分别表示用户ID、电影ID、用户对电影的标签、时间戳。给出了用户对电影的标签列表。
用以生成用户-电影标签矩阵。？？
      Links.csv:moviesId+imdeId+tmdbId。IMDB为互联网电影资料库。tMDB为电影数据集。给出了电影ID和两个数据标记ID的对应关系。
      genome_tags.csv：电影标签 DNA标记，唯一标识符。
      genome_scores.csv: movieId+tagId+relevance。分别表示电影ID、电影标签ID、官方标签相关性。给出了电影的官方标签。
用以生成电影的标签相关性矩阵。
