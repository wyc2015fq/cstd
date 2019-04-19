# Apache Beam初探 - SHINING的博客 - CSDN博客
2017年01月17日 10:10:30[snsn1984](https://me.csdn.net/snsn1984)阅读数：2206
# Apache Beam
#### Apache Beam provides an advanced unified programming model, allowing you to implement batch and streaming data processing jobs that can run on any execution engine.
Apache Beam is:
- UNIFIED - Use a single programming model for both batch and streaming use cases.
- PORTABLE - Execute pipelines on multiple execution environments, including Apache Apex, Apache Flink, Apache Spark, and Google Cloud Dataflow.
- EXTENSIBLE - Write and share new SDKs, IO connectors, and transformation libraries.
原文地址：[https://beam.apache.org/](https://https//beam.apache.org/)
从定义上看，Apache Beam是一个增强型的统一编程模型，这个模型可以支持实现batch和streaming 数据处理工作，并且可以运行在任何执行引擎上。所以总结Apache Beam的特点为：
1、可以针对batch and streaming use cases使用一个编程模型；
2、可以在多个执行环境上执行pipeline，包括了当前比较流行的Apache Apex,Apache Flink,Apache Spark,和Google Cloud Dataflow。
3、可扩展，可以编写和共享新的SDKs,IO connectors,和transformation 库。
分析看来，Apache Beam是提供了一个统一的编程模型，或者说是为客户提供了一个统一的接口，让用户可以不再关心batch和steaming data的不同处理，让用户可以不再关心具体的执行环境或者执行引擎。
这个图更能说明Apache Beam做了什么：
![](https://img-blog.csdn.net/20170117100458676?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvc25zbjE5ODQ=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)
如果Apache Beam真能发展壮大，那么针对大数据处理，无疑是提供了一个很好的解决方案，让用户省去了很多麻烦事。目前sdk仅仅能支持Java版本。
![](https://img-blog.csdn.net/20170117100932104?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvc25zbjE5ODQ=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)
