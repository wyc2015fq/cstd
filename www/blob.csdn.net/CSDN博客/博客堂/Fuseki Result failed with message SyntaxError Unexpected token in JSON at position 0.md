# Fuseki Result: failed with message SyntaxError: Unexpected token  in JSON at position 0 - 博客堂 - CSDN博客





2019年03月23日 20:51:59[最小森林](https://me.csdn.net/u012052268)阅读数：40








Fuseki: Not able to upload owl file

Fuseki无法上传三元组，报错

Result: failed with message “SyntaxError: Unexpected token < in JSON at position 0”
![在这里插入图片描述](https://user-images.githubusercontent.com/18755135/31388116-3fc72998-adcd-11e7-98ac-d040524c3401.png)

解决办法：

修改文件：run/configuration/fuseki_conf.ttl

在service1配置中添加一句话，

添加前是这样的：

```python
<#service1> rdf:type fuseki:Service ;
    fuseki:name                       "Hr_HG_QA" ;     
    fuseki:serviceQuery               "sparql", "query" ; 
    fuseki:serviceReadGraphStore      "get" ;
	fuseki:serviceUpdate              "update" ;
	fuseki:serviceUpload              "upload" ;
	fuseki:dataset                   <#dataset> ;
	.
```

添加一句：

```
fuseki:serviceReadWriteGraphStore "data" ;
```

添加后是这样的：

```python
<#service1> rdf:type fuseki:Service ;
    fuseki:name                       "Hr_HG_QA" ;     
    fuseki:serviceQuery               "sparql", "query" ; 
    fuseki:serviceReadGraphStore      "get" ;
    # 这下面加一句话：
	fuseki:serviceReadWriteGraphStore "data" ;
	fuseki:serviceUpdate              "update" ;
	fuseki:serviceUpload              "upload" ;
	fuseki:dataset                   <#dataset> ;
	.
```

即可解决可以上传。




