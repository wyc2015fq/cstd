# Jersey VS Django-Rest - =朝晖= - 博客园
# [Jersey VS Django-Rest](https://www.cnblogs.com/dhcn/p/7120828.html)
   在对Restful服务框架做对比前，主要先说说Restful设计的三大主要元素：以资源为核心的资源方法、资源状态、关系链接超媒体表述。 辅助的有内容协商、安全、版本化设计等。
   Jersey作为[Java](http://lib.csdn.net/base/java)界比较主流的RestfulWeb服务框架，Django做了[Python](http://lib.csdn.net/base/python)界比较主流Web开发框架，选择在其基础上构建的Django Rest framework，对这两者做一简单的技术选型对比调研(花了两个下午的时间把两者的官方文档读了一遍)。
Jersey主要特性：
1、基于Restful Resource设计的注入式开发模型
2、多部署方式
3、客户端API支持
4、响应式编程支持
5、表述状态支持与响应
6、数据实体的序列化与反序列化设计
7、对于JSON、XML、Multipart三种基本媒体类型的支持
8、过滤器(面向请求与响应)与拦截器(面向数据输入输出流)
9、服务器端和客户端的异步支持
10、对URI安全生成的支持
11、声明式超媒体支持
12、编程式动态生成资源
13、Server-Sent Event支持
14、安全支持特性
15、WADL支持
16、声明式Bean数据验证
17、面向数据视图的实体数据过滤
18、MVC模板
19、应用监视与故障诊断
20、定制注入器与应用生命周期管理
21、Jersey容器自身CDI支持
22、对Spring DI的集成支持
23、Jersey单元[测试](http://lib.csdn.net/base/softwaretest)框架
Django Rest framework的主要特性：
1、Django-Rest的Serializer除了串行和反串行之外还有数据验证的功能，相当于结合了之前的表单数据解析(Json.loads)、Json.dumps.、form.is_valid，而且文档里说它的功能作用主要类似于Djang中的Form.
2、Request和Response做了面向Rest的改造，API View的Response组件内涵内容协商特性来自动确定返回数据的媒体格式等特征。
3、ViewSet和Router的特性将大幅度减少大多数常规API的代码，ViewSet类似于Resource，Router类似于Application Controller。
4、Django-Rest有大量的第三方支持包。
Django Rest framework的特性细节内容展开：
1、面向Restful定制设计的Request
2、面向Restful定制设计的Reponse
3、面向Restful API定制设计的API View
4、内建通用型API View
5、定义式的ViewSet，极大简化面向Model资源的编程
6、Router：集中管理ViewSet，快读生成Restful可用API
7、Parsers：各种媒体格式的解析器
8、Renderers：各种媒体格式生成器
9、Serializers：在[python](http://lib.csdn.net/base/python)数据类型和媒体格式之间的序列化中介
10、Serializer fields：Serializer的字段定义
11、Serializer relations：model关系的序列化
12、Validators：序列化数据验证器
13、Authentication：用户登录验证
14、Permissions：访问权限验证
15、Throttling：基于多重访问数据维度的访问控制
16、Filtering：基于定义配置的数据集动态过滤
17、Pagination：各种各种分页机制支持
18、Versioning：内建的各种版本化机制支持
19、Content negotiation：内建内容协商机制支持
20、Metadata：API元数据表述
21、Format suffixes：URL媒体格式后缀支持
22、Returning URLs：URL反向获得机制
23、Exceptions：各种异常处理支持机制
24、Status codes：资源HTTP状态支持
25、Testing：内建自动化测试机制支持
总体来说：
1、Django-Rest更向上层业务建模靠拢，而Jersey更多是从底层实现封装做起。从设计上讲：前者更偏向业务实现，后者更偏向技术封装。
2、虽然Jersey已经大量使用Java注解等方式简化资源式编码方式，但Django-Rest的开发方式更接近定义式编程方式。所以Django-Rest的编码肯定要比Jersey的少的多。
3、从实现业务服务API的角度讲：Django-Rest的使用方便快捷简易程度应该优于Jersey.

