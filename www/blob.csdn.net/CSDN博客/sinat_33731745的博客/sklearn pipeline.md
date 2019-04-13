
# sklearn pipeline - sinat_33731745的博客 - CSDN博客

2017年10月12日 20:19:33[Tao-Tao-Tao](https://me.csdn.net/sinat_33731745)阅读数：153标签：[sklearn																](https://so.csdn.net/so/search/s.do?q=sklearn&t=blog)[pipeline																](https://so.csdn.net/so/search/s.do?q=pipeline&t=blog)[
							](https://so.csdn.net/so/search/s.do?q=sklearn&t=blog)个人分类：[sklearn																](https://blog.csdn.net/sinat_33731745/article/category/7221454)



管道：pipeline = Pipeline([('tfidf', TfidfVectorizer(ngram_range=(1, 3), analyzer='char', min_df=2, max_df=0.85)), ('lrc', OneVsRestClassifier(LogisticRegression( solver='liblinear'))])
参数传递：
param_grid = {'lrc__estimator__C':C ,'lrc__estimator__max_iter':max_iter}
第一种为正常传递，在模型名和参数之间用双下划线连接，第二种为模型嵌套，通过estimator进行连接，用双下划线连接。

