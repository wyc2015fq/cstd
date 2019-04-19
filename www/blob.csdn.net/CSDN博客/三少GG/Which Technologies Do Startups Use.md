# Which Technologies Do Startups Use? - 三少GG - CSDN博客
2014年10月07日 20:59:14[三少GG](https://me.csdn.net/scut1135)阅读数：743
个人分类：[云计算/大数据](https://blog.csdn.net/scut1135/article/category/1449467)

# [Coding VC](http://codingvc.com/)
## Startups and startup investing from the perspective of a software engineer turned VC.
[« Back to blog](http://codingvc.com/)
## [Which Technologies Do Startups Use? An Exploration of AngelList Data.](http://codingvc.com/which-technologies-do-startups-use-an-exploration-of-angellist-data)
There's a lot of hype surrounding new programming languages, databases, and the like. I've always been curious about which technologies are actually in use, and whether great startups use different technologies
 than not-so-great startups.
Fortunately, AngelList offers some self-reported data about technology usage. For example, you can see that[Robinhood](https://angel.co/robinhood) uses Python,
 Django, and iOS while [Secret](https://angel.co/secret) uses Java, Go, Python, JavaScript, HTML5, CSS, iOS, and Android.
Additionally, AngelList calculates a Signal score for each startup. While it's not 100% clear what this represents, it seems to be some combination of company quality and popularity. For example, [these](https://angel.co/transportation) are
 all of the startups in the Transportation sector, sorted by their Signal scores.
In the interest of openness, there are lots of caveats for this dataset:
- It's not clear if AngelList Signal is actually correlated with company quality (although it seems to be).
- Many companies don't report the technologies that they use.
- The lists of technologies that are self-reported are not necessarily exhaustive.
- etc.
Limitations aside, I calculated the number of startups with low, medium, and high Signal scores using each of ~75 different technologies, and this post summarizes the results. Whenever I refer to okay/good/great
 companies, the intended interpretation is companies with low/medium/high AngelList Signal scores.
(Note: you can click on each chart to see a higher resolution version.)
# Interpreting the Charts
In each chart, blue represents 'okay' startups, red represents 'good' startups, and orange represents 'great' startups. Within each color, the bars show relative frequencies of technology mentions. For example,
 let's say we're looking at technologies A and B. If 'okay' companies use B 3x as often as A, 'good' companies use A and B equally often, and 'great' companies use A twice as often as B, then the chart would look like this:
![](https://phaven-prod.s3.amazonaws.com/files/image_part/asset/1212300/wwoDiHODFCicgImQ92zA2DhoH3U/medium_sample.png)
(Note: the ratios of the lengths of blue/orange/red bars are 1:3, 1:1, and 2:1.)
# Programming Languages
![](https://phaven-prod.s3.amazonaws.com/files/image_part/asset/1212244/eHHq7f9S69z9dfa7YX0tBJqfo1Q/medium_proglang.png)
Observations:
- For great companies, the most popular languages are Javascript, Ruby, Python, and Java.
- For okay companies, the most popular languages are Javascript, Ruby, PHP, and Java.
- The likelihood that PHP is being used is strongly anti-correlated with company quality.
- The better the company, the more likely it is to be using modern and/or functional programming languages (i.e. Go, Scala, Haskell, Erlang, Clojure).
# Front-end Tech
![](https://phaven-prod.s3.amazonaws.com/files/image_part/asset/1212242/UGFacR1L9y4N9kDQD4My37iPTPs/medium_frontend.png)
This chart shows a combination of web frameworks (Rails, Django), Javascript Libraries, and HTML/CSS.
Observations:
- Ruby on Rails is super popular.
- HTML5 is dominating HTML.
- CSS is still dominating CSS3.
- The better the company, the less likely it is to use Bootstrap. 
# Databases/Storage/Caching
![](https://phaven-prod.s3.amazonaws.com/files/image_part/asset/1212277/Rm0eDWBucVoqH2sdhuRRrtt8RaA/medium_db.png)
Observations:
- MySQL, Mongo, and Postgres dominate the database side.
- Redis is much more popular than memcached.
- The better the company, the less likely it is to build on top of Microsoft's products (SQL Server).
# Mobile
![](https://phaven-prod.s3.amazonaws.com/files/image_part/asset/1212278/yMjdQXA6mapwjh8zc1yeIjKy1AM/medium_mobile.png)
Observations:
- Developing for iOS is slightly more popular than developing for Android
- The gap widens as company quality increases.
- Windows Mobile (which is not present in the chart) is 30x-50x less popular than iOS and Android among good/great companies.
# Infrastructure/Hosting
![](https://phaven-prod.s3.amazonaws.com/files/image_part/asset/1212279/bxA-CymneGT-KO7txmNzFleaV7k/medium_paas.png)
Observations:
- AWS and Heroku dominate.
- The better the company, the more likely it is to use IaaS (e.g. AWS) instead of PaaS (e.g. Parse)
- The better the company, the less likely it is to build on top of Microsoft's products (Azure).
# DevOps Tools

![](https://phaven-prod.s3.amazonaws.com/files/image_part/asset/1212280/D2i7Bc_5XjhJvilqp_SIKkCkrZA/medium_deploy.png)
(Note: the sample size here was small because DevOps tools were rarely mentioned on AngelList profiles)
# Search
![](https://phaven-prod.s3.amazonaws.com/files/image_part/asset/1212281/1Yh_LH57o-fGcFn78DcUvTg1Sh8/medium_search.png)
Observations:
- Elasticsearch dominates this category.
# API Integrations
![](https://phaven-prod.s3.amazonaws.com/files/image_part/asset/1212282/KJnHDs75AKVALv6WwA67bdwpg08/medium_APIs.png)
(Note: the sample size here was small because APIs were rarely mentioned on AngelList profiles -- especially for good/great companies.)
# Advanced Tech
![](https://phaven-prod.s3.amazonaws.com/files/image_part/asset/1212283/wBzqwQusSvH0UiIS5Taavr2FsHc/medium_technologies.png)
Observations:
- I was surprised that there's no clear correlation between quality of company and usage of sophisticated technologies like machine learning or computer vision.
# Big Data Software
![](https://phaven-prod.s3.amazonaws.com/files/image_part/asset/1212308/XA759GEnocO0o6Y2XN1Uf9OMoRw/medium_hadoop.png)
(Note: small sample size)
*If you liked this post, please consider sharing it or subscribing to future posts.*

