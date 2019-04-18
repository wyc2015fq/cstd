# ALS推荐算法—训练并保存—加载并测试 - wsp_1138886114的博客 - CSDN博客





2018年09月21日 21:50:08[SongpingWang](https://me.csdn.net/wsp_1138886114)阅读数：331











### 文章目录
- [一、读取数据—清洗数据训练并保存](#_2)
- [二、加载模型—预测](#_54)



数据集下载地址：[https://download.csdn.net/download/wsp_1138886114/10681947](https://download.csdn.net/download/wsp_1138886114/10681947)


##### 一、读取数据—清洗数据训练并保存

```python
import os
from pyspark import SparkContext,SparkConf
from pyspark.mllib.recommendation import ALS,Rating

def create_spark_context():
    os.environ['JAVA_HOME'] = 'C:/Java/jdk1.8.0_91'
    os.environ['HADOOP_HOME'] = 'C:/Java/hadoop-2.6.0-cdh5.7.6'
    os.environ['SPARK_HOME'] = 'C:/Java/spark-2.2.0-bin-2.6.0-cdh5.7.6'

    spark_conf = SparkConf()\
        .setAppName('Python_Spark_WordCount')\
        .setMaster('local[4]') \
        .set("spark.driver.extraJavaOptions", "-Xss4096k")

    spark_context = SparkContext(conf=spark_conf) # 获取SparkContext实例对象,
    spark_context.setLogLevel('WARN')             # 设置日志级别
    return spark_context

def prepare_data(spark_context):
    # ------------1.读取评分数据并解析 -------------
    raw_user_data = spark_context.textFile("../ml-100k/u.data")
    raw_ratings = raw_user_data.map(lambda line: line.split("\t")[:3])
    ratings_rdd = raw_ratings.map(lambda x: Rating(int(x[0]), int(x[1]), float(x[2])))

    # ------------2.数据初步统计 ----------------
    num_ratings = ratings_rdd.count()
    num_users = ratings_rdd.map(lambda x: x[0]).distinct().count()
    num_movies = ratings_rdd.map(lambda x: x[1]).distinct().count()
    print("总共: ratings: " + str(num_ratings) + ", User: " + str(num_users) + ", Moive: " + str(num_movies))
    return ratings_rdd

def save_mode(spark_context,model):
    try:
        model.save(spark_context, "../datas/als-model")
    except Exception:
        print ("保存模型出错") 
        
if __name__ =="__main__":
    sc = create_spark_context()

    print("==================数据准备阶段===================")
    rating_rdd = prepare_data(sc)
    print("==================模型训练阶段===================")
    #开始使用ALS算法：rank=5",iterations = 5, lambda = 0.1
    als_model = ALS.train(rating_rdd,5,iterations=5,lambda_=0.1)
    print( als_model)
    print("==================模型保存阶段===================")
    save_mode(sc,als_model)
    sc.stop()
```

##### 二、加载模型—预测

```python
from pyspark import SparkContext, SparkConf
from pyspark.mllib.recommendation import MatrixFactorizationModel
import os
import sys

def create_spark_context():
    os.environ['JAVA_HOME'] = 'C:/Java/jdk1.8.0_91'
    os.environ['HADOOP_HOME'] = 'C:/Java/hadoop-2.6.0-cdh5.7.6'
    os.environ['SPARK_HOME'] = 'C:/Java/spark-2.2.0-bin-2.6.0-cdh5.7.6'
    spark_conf = SparkConf()\
        .setAppName('Python_Spark_WordCount')\
        .setMaster('local[4]') \
        .set("spark.driver.extraJavaOptions", "-Xss4096k")

    spark_context = SparkContext(conf=spark_conf) # 获取SparkContext实例对象,
    spark_context.setLogLevel('WARN')             # 设置日志级别
    return spark_context

def prepare_data(spark_context):
    item_rdd = sc.textFile("../ml-100k/u.item")   # 读取 u.item 电影信息数据
    movie_title = item_rdd \                      # 创建 电影名称 与 电影ID  映射的字典
        .map(lambda line: line.split("|")) \
        .map(lambda a: (float(a[0]), a[1]))

    movie_title_dict = movie_title.collectAsMap() # 将RDD转换字典
    return movie_title_dict

def load_model(spark_context):                    # 加载模型
    try:
        model = MatrixFactorizationModel.load(spark_context, '../datas/als-model')
        print (model)
        return model
    except Exception:
            print ("加载模型出错")

if __name__ =="__main__": 
    # 运行前点击 pycharm 菜单栏 run->Edit configuration->Script parameters 输入： --U 198（随意user_id）
    if len(sys.argv)!=3:
        print("请输入两个参数：--U user_id,--M movie_id")

def recommend_movies(als, movies, user_id):
    rmd_movies = als.recommendProducts(user_id, 10)
    print('推荐的电影为：{}'.format(rmd_movies))
    for rmd in rmd_movies:
        print("为用户{}推荐的电影为：{}".format(rmd[0], movies[rmd[1]]))
    return rmd_movies

def recommend_users(als, movies, movie_id):      # 为每个电影推荐10个用户
    rmd_users = als.recommendUsers(movie_id, 10)
    print('针对电影ID：{0},电影名:{1},推荐是个用户为:'.format(movie_id, movies[movie_id]))
    for rmd in rmd_users:
        print("推荐用户ID：{},推荐评分：{}".format(rmd[0], rmd[2]))


def recommend(als_model, movie_dic):
    if sys.argv[1] == '--U':                     # 推荐电影给用户
        recommend_movies(als_model, movie_dic, int(sys.argv[2]))
    if sys.argv[1] == '--M':                     # 推荐用户给电影
        recommend_users(als_model, movie_dic, int(sys.argv[2]))

if __name__ == "__main__":
    """
    1.数据准备
    2.加载模型
    3.预测推荐
    """
    # 由于推荐的方式有两种，一个是依据用户的推荐，一个是基于商品的推荐
    if len(sys.argv) != 3:
        print("请输入2个参数, 要么是: --U user_id,  要么是: --M movie_id")
        exit(-1)
    sc = create_spark_context()

    # 数据准备，就是加载电影数据信息，转换字典
    print('============= 数据准备 =============')
    movie_title_dic = prepare_data(sc)
    print('============= 加载模型 =============')
    als_load_model = load_model(sc)
    print('============= 预测推荐 =============')
    recommend(als_load_model, movie_title_dic)
```



