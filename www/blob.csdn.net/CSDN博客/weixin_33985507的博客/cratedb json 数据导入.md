# cratedb json 数据导入 - weixin_33985507的博客 - CSDN博客
2018年08月06日 13:21:00[weixin_33985507](https://me.csdn.net/weixin_33985507)阅读数：12
基本环境的搭建，可以参考相关文档，或者直接使用docker
## 安装
`docker run -d -p 4200:4200 crate`
## 导出mongodb数据（可选，同时使用工具进行数据类型转换）
```
mongoexport --db <dbname> --collection <data> --out out.json
curl https://raw.githubusercontent.com/crate/crate-utils/master/migrations/mongodb/convert_json.py -o convert_json.py
python convert_json.py -i mongodb-colletion.json -o appdemo.json
```
## 导出数据
> 
copy from
- 参考数据
```
demoinfo.json
{"id": 1, "quote": "Don't panic"}
{"id": 2, "quote": "Ford, you're turning into a penguin. Stop it."}
```
- 创建表结构
```
create table appdemofirst (
     id int,
     quote string
 ) clustered into 5 shards with (number_of_replicas = 0);
```
- 导入数据
> 
注意数据需要copy 到容器中 使用docker cp 命令
`COPY  appdemofirst FROM 'file:///data/demoinfo.json'`
- 导入结果 
![](https://images2018.cnblogs.com/blog/562987/201808/562987-20180806132118742-908808700.png)
- 数据查询
`select * from appdemofirst limit 100;`
![](https://images2018.cnblogs.com/blog/562987/201808/562987-20180806132119626-1185034559.png)
## 参考资料
[https://crate.io/docs/crate/reference/en/latest/sql/statements/copy-from.html](https://crate.io/docs/crate/reference/en/latest/sql/statements/copy-from.html)
[https://crate.io/docs/crate/guide/en/latest/best-practices/migrating-from-mongodb.html](https://crate.io/docs/crate/guide/en/latest/best-practices/migrating-from-mongodb.html)

