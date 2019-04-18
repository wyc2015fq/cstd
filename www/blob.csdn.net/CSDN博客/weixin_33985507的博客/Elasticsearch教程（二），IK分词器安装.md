# Elasticsearch教程（二），IK分词器安装 - weixin_33985507的博客 - CSDN博客
2017年05月19日 16:40:00[weixin_33985507](https://me.csdn.net/weixin_33985507)阅读数：3
elasticsearch-analysis-ik  是一款中文的分词插件，支持自定义词库，也有默认的词库。
# 开始安装。
## 1、下载
下载地址为：[https://github.com/medcl/elasticsearch-analysis-ik](https://github.com/medcl/elasticsearch-analysis-ik)
或者从官网地址搜索：[https://www.elastic.co](https://www.elastic.co/) 搜索`IK`即可。
本博客下面也提供了[elasticsearch-analysis-ik.zip](http://cdn.sojson.com/file/elasticsearch-analysis-ik-master.zip)的下载链接。
2、解压。
把下载的 elasticsearch-analysis-ik.zip解压。
3、[Maven](http://www.sojson.com/tag_maven.html)打包
进入elasticsearch-analysis-ik-master/ 下，打包。注意：打包后的文件在`elasticsearch-analysis-ik-master/target/`目录下
mvn clean package
4.然后在`elasticsearch-2.2.0/plugins`下创建目录 `ik`
cd elasticsearch-2.2.0/plugins
mkdir ik
cd ik
然后将打包后的`elasticsearch-analysis-ik-1.8.0.jar` 放入当前目录下，然后解压。
5、设置（2.0以上可以不设置，我没有设置）。
在[Elasticsearch](http://www.sojson.com/tag_elasticsearch.html) 的`config`下的`elasticsearch.yml`文件中，添加如下代码。
index:  
      analysis:                     
        analyzer:        
          ik:  
              alias: [ik_analyzer]  
              type: org.elasticsearch.index.analysis.IkAnalyzerProvider  
          ik_max_word:  
              type: ik  
              use_smart: false  
          ik_smart:  
              type: ik  
              use_smart: true
