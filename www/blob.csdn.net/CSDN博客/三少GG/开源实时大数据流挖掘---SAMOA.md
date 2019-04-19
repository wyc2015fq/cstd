# 开源实时大数据流挖掘---SAMOA - 三少GG - CSDN博客
2013年12月10日 17:01:10[三少GG](https://me.csdn.net/scut1135)阅读数：1733
官方非常赞介绍性 ppt: **[http://yahoo.github.io/samoa/](http://yahoo.github.io/samoa/)**
非官方优秀博客：**[http://www.otnira.com/category/projects/](http://www.otnira.com/category/projects/)**
## [Introducing SAMOA, an open source platform for mining big data streams.](http://yahooeng.tumblr.com/post/65453012905/introducing-samoa-an-open-source-platform-for-mining)
[](https://github.com/yahoo/samoa)[https://github.com/yahoo/samoa](https://github.com/yahoo/samoa)
Machine learning and data mining are well established techniques in the world of IT and especially among web companies and startups. Spam detection, personalization and recommendations are just a few of the applications
 made possible by mining the huge quantity of data available nowadays. However, “big data” is not only about Volume, but also about Velocity (and Variety, [3V of big
 data](http://www.gartner.com/newsroom/id/1731916)).
The usual pipeline for modeling data (what “data scientists” do) involves taking a sample from production data, cleaning and preprocessing it to make it usable, training a model for the task at hand and finally
 deploying it to production. The final output of this process is a pipeline that needs to run periodically (and be maintained) in order to keep the model up to date. Hadoop and its ecosystem (e.g., [Mahout](http://mahout.apache.org/))
 have proven to be an extremely successful platform to support this process at web scale.
However, no solution is perfect and big data is ["data whose characteristics forces us to look beyond the traditional
 methods that are prevalent at the time"](http://queue.acm.org/detail.cfm?id=1563874). The current challenge is to move towards analyzing data as soon as it arrives into the system, nearly in real-time.
For example, models for mail spam detection get outdated with time and need to be retrained with new data. New data (i.e., spam reports) comes in continuously and the model starts being outdated the moment it is
 deployed: all the new data is sitting without creating any value until the next model update. On the contrary, incorporating new data as soon as it arrives is what the “Velocity” in big data is about. In this case, Hadoop is not the ideal tool to cope with
 streams of fast changing data.
Distributed stream processing engines are emerging as the platform of choice to handle this use case. Examples of these platforms are [Storm](http://storm-project.net/), [S4](http://incubator.apache.org/s4),
 and recently [Samza](http://samza.incubator.apache.org/). These platforms join the scalability of distributed processing with the fast response of stream processing. Yahoo has already adopted Storm as a key
 technology for low-latency big data processing.
Alas, currently there is no common solution for mining big data streams, that is, for doing machine learning on streams on a distributed environment.
## Enter SAMOA
SAMOA (Scalable Advanced Massive Online Analysis) is a framework for mining big data streams. As most of the big data ecosystem, it is written in Java. It features a pluggable architecture that allows it to run
 on several distributed stream processing engines such as Storm and S4. SAMOA includes distributed algorithms for the most common machine learning tasks such as classification and clustering. For a simple analogy, you can think of SAMOA as Mahout for streaming.
![image](https://lh3.googleusercontent.com/Hcu0M3ZxMKLbDExSo_zg9BbEwmwml6cQB259cjArLrUsgGpKPI3P4qK6HkbEumv6C0kBq0IL9zwLfqBwiOU8gkrxjoHYOxZV5jFUwU1-iWUU5a3W9D4-sMuGYw)
SAMOA is both a platform and a library. As a platform, it allows the algorithm developer to abstract from the underlying execution engine, and therefore reuse their code to run on different engines. It also allows
 to easily write plug-in modules to port SAMOA to different execution engines.
As a library, SAMOA contains state-of-the-art implementations of algorithms for distributed machine learning on streams. The first alpha release allows classification and clustering.
For classification, we implemented a Vertical Hoeffding Tree (VHT), a distributed streaming version of decision trees tailored for sparse data (e.g., text). For clustering, we included a distributed algorithm based
 on CluStream. The library also includes meta-algorithms such as bagging.
![image](https://lh6.googleusercontent.com/TXoF3zWPusZLHVW9fswh1aJIKRfoawi18aTjydDPNmgaCjhNpSWVWsUAMyW_Xaa4nzHipCWvcWEoNWY3_O2dboEPJHh3gATgRTk5-O9s1tfiKYHhGlKN2WpbqA)
## HOW DOES IT WORK?
An algorithm in SAMOA is represented by a series of nodes communicating via messages along streams that connect pairs of nodes (a graph). Borrowing the terminology from Storm, this is called a Topology. Each node
 in the Topology is a Processor that sends messages to a Stream. The user code that implements the algorithm resides inside a Processor. Figure 3 shows an example of a Processor joining two stream from two source Processors. Here is a code snippet to build
 such a topology in SAMOA.
```
TopologyBuilder builder;
Processor sourceOne = new SourceProcessor();
builder.addProcessor(sourceOne);
Stream streamOne = builder.createStream(sourceOne);
Processor sourceTwo = new SourceProcessor();
builder.addProcessor(sourceTwo);
Stream streamTwo = builder.createStream(sourceTwo);
Processor join = new JoinProcessor();
builder.addProcessor(join).connectInputShuffle(streamOne).connectInputKey(streamTwo);
```
![image](https://lh3.googleusercontent.com/fxrQTkPx2VDpIZfWHytCuUbVVZePgwSxBNeAnpIksQngOgDW22UyqwythMGe0208x2VlrV1-xsaDdt9qORk9NWWPv108812OBtuMFo6UtJvYCeCAuKSHu96fQA)
## SWEET! HOW DO I GET STARTED?
1. Download SAMOA
```
git clone git@github.com:yahoo/samoa.git
cd samoa
mvn -Pstorm package
```
2. Download the Forest CoverType dataset.
```
wget "http://downloads.sourceforge.net/project/moa-datastream/Datasets/Classification/covtypeNorm.arff.zip"
unzip covtypeNorm.arff.zip
```
Forest CoverType contains the forest cover type for 30 x 30 meter cells obtained from US Forest Service (USFS) Region 2 Resource Information System (RIS) data. It contains 581,012 instances and 54 attributes, and
 it has been used in several papers on data stream classification.
3. Download a simple logging library.
`wget "http://repo1.maven.org/maven2/org/slf4j/slf4j-simple/1.7.2/slf4j-simple-1.7.2.jar"`
4. Run an Example. Classifying the CoverType dataset with the VerticalHoeffdingTree in local mode.
`java -cp slf4j-simple-1.7.2.jar:target/SAMOA-Storm-0.0.1.jar com.yahoo.labs.samoa.DoTask "PrequentialEvaluation -l classifiers.trees.VerticalHoeffdingTree -s (ArffFileStream -f covtypeNorm.arff) -f 100000"`
The output will be a sequence of the evaluation metrics for accuracy, taken every 100,000 instances.
To run the example on Storm, please refer to the [instructions on the wiki](https://github.com/yahoo/samoa/wiki/Executing-SAMOA-with-Apache-Storm).
## I WANT TO KNOW MORE!
For more information about SAMOA, see the [README](https://github.com/yahoo/samoa) and the [wiki](https://github.com/yahoo/samoa/wiki) on
 github, or post a question on the [mailing list](https://groups.google.com/forum/#!forum/samoa-user).
SAMOA is licensed under an Apache Software License v2.0. You are welcome to contribute to the project! SAMOA accepts contributions under an Apache style contributor license agreement.
Good luck! We hope you find SAMOA useful. We will continue developing the framework by adding new algorithms and platforms.
*Gianmarco De Francisci Morales ([gdfm@yahoo-inc.com](mailto:gdfm@yahoo-inc.com)) and Albert Bifet ([abifet@yahoo.com](mailto:abifet@yahoo-inc.com)) @ Yahoo Labs Barcelona*
- [big data](http://yahooeng.tumblr.com/tagged/big-data)
- [streaming](http://yahooeng.tumblr.com/tagged/streaming)
- [machine learning](http://yahooeng.tumblr.com/tagged/machine-learning)
- [data mining](http://yahooeng.tumblr.com/tagged/data-mining)
- [samoa](http://yahooeng.tumblr.com/tagged/samoa)
- By ![](http://25.media.tumblr.com/avatar_60c1bcd4cab0_16.png) gdfm
- [](http://www.tumblr.com/reblog/65453012905/le4JYq2B)
- 
- [OCT 29TH, 2013](http://yahooeng.tumblr.com/post/65453012905/introducing-samoa-an-open-source-platform-for-mining)
- [28](http://yahooeng.tumblr.com/post/65453012905/introducing-samoa-an-open-source-platform-for-mining)
