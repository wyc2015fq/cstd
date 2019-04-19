# Real Time Detection of Outliers in Sensor Data using Spark Streaming - BlueSky - CSDN博客
2016年03月03日 14:59:29[ASIA_kobe](https://me.csdn.net/ASIA_kobe)阅读数：540

From:https://pkghosh.wordpress.com/2015/02/19/real-time-detection-of-outliers-in-sensor-data-using-spark-streaming/
As far as analytic of sensor generated data is concerned, in Internet of Things ([IoT](http://en.wikipedia.org/wiki/Internet_of_Things))
 and in a connected everything world, it’s mostly about real time analytic of time series data. In this post, I will be addressing an use case involving
 detecting outliers in sensor generated data with Spark Streaming. Outliers are data points that deviate significantly from most of the data. The implementation is part of my
 new open source project [ruscello](https://github.com/pranab/ruscello),
 implemented in Scala. The project is focused on real time analytic of sensor data with various IoT use cases in mind.
The specific use we will be addressing in this post has to with temperature data from temperature controlled shipping containers.
## Temperature Controlled Shipping Containers
Consider some product being shipped in temperature controlled containers. The customer has a Service Level Agreement (SLA) with the transportation company,
 which defines how the temperature is maintained within a predefined range. Here are some examples of SLA, that have been implemented
- Mean temperature within a time window has to be below predefined upper limit or above some predefined lower threshold.
- Some minimum percentage of the data within a time window has to be below some upper threshold or above some lower threshold
The second SLA is more tolerant of small signal to noise ratio data. All data points that cause the SLA to
 be violated are outliers. In Machine Learning parlance the problem we are solving is supervised
 outlier detection. It’s supervised because we are specifying the outlier conditions explicitly through the SLA.
In time series analysis literature, this problem is also known level shift detection. There are other techniques for detecting level shift, e.g., computing
 histogram for sliding window data and detecting any notable change in histogram mode.
The transportation company is responsible for detecting all the violation incidents of SLAand reporting them to the customer. The customer may claim damages
 when SLA is violated.
## IoT and Big Data Analytic
According Cisco, there will 21 billion connected devices by 2018. All these devices will be spewing data creating a data deluge. But all the data
 may not end up in a Big Data platform.
Although Big Data analytic will play a significant role in IoT, we must be cognizant of the fact that with IoT,
 some of the data processing may happen at the edge of the [sensor
 network](http://en.wikipedia.org/wiki/Wireless_sensor_network). It’s been called fog computation. There may be hub nodes in a sensor network which may collect and aggregate data from a set of sensors.
The data arriving at the Big Data platform may not always be the raw sensor data. It may be data aggregated and preprocessed at the network edge. For
 example, inside a sensor network a hub may aggregate data from multiple sensors and based on computation may decided to activate some actuator. The hub may send data related to the decision it made to a Big Data platform, instead of the raw data. Big Data
 platform will still have a big role in IoT, however some of the computation will happen at the edge.
If the sensor raw data needs to be stored in a NOSQL database, it will always be sent to back end Big Data platform. If the insight being sought through
 analytic needs a global context e.g., other sensor data or some metadata then also all the data need to be collected and processed in a Big Data platform.
## Quick Tour of Spark Streaming
[Spark Streaming](https://spark.apache.org/streaming/) architecture is fundamentally different from
 other real time stream processing systems. It has the following notable characteristics.
- Messages are processed in micro batches, where the stream is essentially a sequence of  RDDs. A batch size is defined by a batch interval set by the user.
- It may not sound very intuitive, but Spark Streaming latency has been found to be lower than Storm, especially for small messages.
- RDDs from the stream are processed like normal spark offline RDD processing.
- Recovery from node failure is handled through RDD lineage. Recovery is parallelized and fast
- Effectively handles stragglers through speculative execution
- Spark unifies batch and real time processing in one system, which is a big plus for many projects.
As you can tell, Spark Streaming is a powerful platform for real time streamprocessing.The fact that it leverages many features
 of the underlying core engine for batch processing is very clever.
## Outlier Detection in Streaming Temperature Sensor Data
The sensor data generation in simulated through a python script. The temperature stays at the desired level except for some random noise superimposed. At some random time, the temperature shifts to an upper or lower level, again with some random noise superimposed.
 It stays there for a while and then goes back to the normal level. The sensor data has three fields as below
- Sensor ID
- Time stamp
- Temperature
The data is piped to socket server. In Spark Streaming, the data is ingested by socket text stream receivers. The data is partitioned by sensor ID and processed
 through windows. .Kafka and HDFS is also
 supported as data stream source, although they are not fully tested yet.
With Kafka, the data stream ingestion by Spark receivers could be parallelized by having multiple input DStreams,
 if the sensor data in Kafka is partitioned by the sensorID.
The project ruscello is essentially a Spark shell around [hoidla](https://github.com/pranab/hoidla),
 which is a  java library for streaming algorithms. Windowing functions from hoidla is used. [Windowing
 in hoidla](https://pkghosh.wordpress.com/2014/10/05/tracking-web-site-bounce-rate-in-real-time/)offers the following powerful features.
- Size bound window or time bound window.
- Various window population techniques, including simple sliding widow with every data point added to the window and more complex techniques e.g., reservoir sampling
- Various statistical parameter calculations on windowed data e.g., standard deviation, entropy etc
- A simple predicate logic based expression evaluator  e.g mean(window) < 60 and stdDev(window) < 1.2. Currently only disjunction of conjunctives are supported.
The condition expression is disjunction of a set of set of conjunctive expressions. Each conjunctive expression is conjunction of a set of predicates. Predicates are either relational or functional.
Since we are dealing with time series data, we should be using time bound window. However, I made the assumption that data samples arrive at regular intervals
 and the variability in sampling period is negligible. Accordingly I have used size bound window. Knowing the sampling rate and the desired time span of the window, we can easily find the window size in terms of number of data points.
The window object is held inside a state object, leveraging the state management feature of Spark Streaming. Because of partitioning based on sensorID,
 there is a separate state object for each sensor. I could not use Spark’s native window function, because it did not meet my requirements.
As each data point arrives, it’s added to the window and the SLA condition
 expression is evaluated. If the evaluation returns true, then the tuple (timestamp, mean) is appended to a list inside the state object.
## SLA Breach Output
Finally Spark returns a stream of RDDs, where each RDD is
 comprised of (sensorID, state object). We query the state object for number of violations and return the number of violations for each sensor.
 We see the number of violations increasing with time. Here is some sample output
device:U4W8U4L3 num violations:102
device:HCEJRWFP num violations:194
device:U4W8U4L3 num violations:102
device:HCEJRWFP num violations:194
device:U4W8U4L3 num violations:247
device:HCEJRWFP num violations:411
We could also produce a more detailed output containing the time stamp and mean temperature reading for each violation of each sensor.
## Summing Up
Spark Streaming is a powerful real time stream processing platform. We have shown it’s usefulness using IoT sensor data analytic as an example. IoT related
 data  is expected to have explosive growth and Spark Streaming can play a critical role in analysis of such data.
If you are interested in running this use case, here is a [tutorial
 document](https://github.com/pranab/ruscello/blob/master/spark/resource/level_shift_detector_tutorial.txt). You will find other associated configuration file, shell script etc in the same github directory.
