# Recipes for Running Spark Streaming Applications in Production - BlueSky - CSDN博客
2015年11月27日 23:42:47[ASIA_kobe](https://me.csdn.net/ASIA_kobe)阅读数：341
个人分类：[Spark Learning](https://blog.csdn.net/ASIA_kobe/article/category/5912555)
转自：https://spark-summit.org/2015/events/recipes-for-running-spark-streaming-applications-in-production/
[Tathagata Das](https://spark-summit.org/2015/speakers/tathagata-das/) (Databricks)
- Tuesday, June 16
- 2:00 PM – 2:30 PM
- Grand Ballroom B
[Slides PDF](http://www.slideshare.net/SparkSummit/recipes-for-running-spark-streaming-apploications-in-production-tathagata-daspptx)[Video](https://www.youtube.com/watch?v=d5UJonrruHk&list=PL-x35fyliRwgfhffEpywn4q23ykotgQJ6&index=4)
Spark Streaming extends the core Apache Spark to perform large-scale stream processing. It is being rapidly adopted by companies spread across various business verticals – ad monitoring, real-time analysis of machine data, anomaly detections, etc. This interest
 is due to its simple, high-level programming model, and its seamless integration with SQL querying (Spark SQL), machine learning algorithms (MLlib), etc. However, for building a real-time streaming analytics pipeline, its not sufficient to be able to easily
 express your business logic. Running the platform with high uptimes and continuously monitoring it has a lot of operational challenges. Fortunately, Spark Streaming makes all that easy as well. In this talk, I am going to elaborate about various operational
 aspects of a Spark Streaming application at different stages of deployment – prototyping, testing, monitoring continuous operation, upgrading. In short, all the recipes that takes you from “hello-world” to large scale production in no time.
