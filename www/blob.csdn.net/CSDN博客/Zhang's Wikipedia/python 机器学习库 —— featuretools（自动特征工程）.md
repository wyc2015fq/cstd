
# python 机器学习库 —— featuretools（自动特征工程） - Zhang's Wikipedia - CSDN博客


2018年04月03日 22:48:16[Inside_Zhang](https://me.csdn.net/lanchunhui)阅读数：1796


文档：[https://docs.featuretools.com/\#minute-quick-start](https://docs.featuretools.com/#minute-quick-start)
所谓自动特征工程，即是将人工特征工程的过程自动化。以 featuretools 为代表的自动特征工程在整个机器学习的端到端实践中扮演的角色如下图所示：

![这里写图片描述](https://img-blog.csdn.net/20180403222901677?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L2xhbmNodW5odWk=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)[](https://img-blog.csdn.net/20180403222901677?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L2xhbmNodW5odWk=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)
[](https://img-blog.csdn.net/20180403222901677?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L2xhbmNodW5odWk=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)
## 1. demo
[](https://img-blog.csdn.net/20180403222901677?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L2xhbmNodW5odWk=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)导入包：import featuretools as ft
加载数据：data = ft.demo.load_mock_customer()，data 为 dict 类型
data.keys() ⇒ dict_keys([‘transactions’, ‘products’, ‘customers’, ‘sessions’])
顾客发生了多次购买会话（session），每次会话产生了多次交易（transaction）
data[‘customers’] ⇒ DataFrame（Pandas）
整理数据：
customers_df=data['customers']sessions_df=data['sessions']transactions_df=data['transactions']
构建数据集
entities = {...:"customers": (customers_df,"customer_id"),...:"sessions": (sessions_df,"session_id","session_start"),...:"transactions": (transactions_df,"transaction_id","transaction_time")...: }
指定关系：父实体与子实体的关系，通过如下四元组来定义：
(parent_entity, parent_variable, child_entity, child_variable)接下来定义如下的关系：
relationships = [("sessions","session_id","transactions","session_id"),
                 ("customers","customer_id","sessions","customer_id")]
2. DFS：Deep Feature Synthesis，深度特征合成
feature_matrix_customers, features_defs = ft.dfs(entities=entities, relationships=relationships, target_entity="customers"）
[](https://img-blog.csdn.net/20180403222901677?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L2xhbmNodW5odWk=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)
