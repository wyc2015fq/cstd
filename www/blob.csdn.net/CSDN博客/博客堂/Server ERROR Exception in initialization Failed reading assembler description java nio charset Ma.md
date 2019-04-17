# Server ERROR Exception in initialization: Failed reading assembler description: java.nio.charset.Ma - 博客堂 - CSDN博客





2019年03月30日 21:09:25[最小森林](https://me.csdn.net/u012052268)阅读数：190








# 1报错

使用知识图谱开启fuseki的时候报错：

Server ERROR Exception in initialization: the loading of content into file:///D:/AppsPath/apache-jena-fuseki-3.8.0/run/configuration/fuseki_conf.ttl#model_inf was aborted because of Read-only object file

Server ERROR Exception in initialization: Failed reading assembler description: [java.nio.charset.Ma](http://java.nio.charset.Ma)

# 2 原因：

这是由于fuseki版本升级所导致的问题，ja:baseModel <#tdbGraph> ;改为 ja:MemoryModel <#tdbGraph> ; 并同时加载本体和三元组即可顺利运行。

# 3 解决办法1

我找了好久 才找到解决办法（主要参考： [https://blog.csdn.net/qingdujun/article/details/82458785](https://blog.csdn.net/qingdujun/article/details/82458785) 和 [https://zhuanlan.zhihu.com/p/33224431），](https://zhuanlan.zhihu.com/p/33224431%EF%BC%89%EF%BC%8C) 在这里 给出详细步骤， 既可以解决报错，有可以同步加载本体和三元组。（不需要每次自己手动加载）

重写fuseki-3.8.0\run\configuration\fuseki_conf.ttl配置文件：

```python
@prefix :      <http://base/#> .
@prefix tdb:   <http://jena.hpl.hp.com/2008/tdb#> .
@prefix tdb2:  <http://jena.apache.org/2016/tdb#> .
@prefix rdf:   <http://www.w3.org/1999/02/22-rdf-syntax-ns#> .
@prefix ja:    <http://jena.hpl.hp.com/2005/11/Assembler#> .
@prefix rdfs:  <http://www.w3.org/2000/01/rdf-schema#> .
@prefix fuseki: <http://jena.apache.org/fuseki#> .


<#service1> rdf:type fuseki:Service ;
    fuseki:name                       "Hr_HG_QA" ;     
    fuseki:serviceQuery               "sparql", "query" ; 
    fuseki:serviceReadGraphStore      "get" ;
	fuseki:serviceReadWriteGraphStore "data" ;
	fuseki:serviceUpdate              "update" ;
	fuseki:serviceUpload              "upload" ;
	fuseki:dataset                   <#dataset> ;
	.


<#dataset> rdf:type ja:RDFDataset ;
	ja:defaultGraph
	# 这样做可以不抱错。
      [ a ja:MemoryModel ;
      # 加载本体文件
        ja:content [ja:externalContent <file:///D:/Programming/apache-jena-fuseki-3.7.0/run/databases/hr-kg-ontology-en-turtle.ttl> ] ;
        # 不用jena转换，直接加载三元组
        ja:content [ja:externalContent <file:///D:/Programming/d2rq-0.8.1/Hr_HG_QA.nt> ] ;
      ] ;
    .

<#model_inf> a ja:InfModel ;

	#打开OWL推理机
    ja:reasoner [ja:reasonerURL <http://jena.hpl.hp.com/2003/OWLFBRuleReasoner>] ;
	
    #关闭规则推理机，并指定规则文件路径
    #ja:reasoner [ ja:reasonerURL <http://jena.hpl.hp.com/2003/GenericRuleReasoner> ; 
    #              ja:rulesFrom <file:///D:/Programming/apache-jena-fuseki-3.7.0/run/databases/rules.ttl> ] ;
	.
```

这样
- 优点：不要每次都 重新加载。
- 缺点：经过实践，发现丧失了 推理能力。

# 4解决方法二：

删除tdb路径下所有prefixXXX文件，就可以启动fuseki服务。（再次启动会重新生成这些文件，不用担心）
![](http://ww1.sinaimg.cn/large/006qDjsOgy1g1l4iz7uyoj311y0kgdlj.jpg)- 优点： 可以推理
- 但是每次都得自己删除prefixXXX




