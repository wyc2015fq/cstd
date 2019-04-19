# DjangoRestFramework实践笔记 - =朝晖= - 博客园
# [DjangoRestFramework实践笔记](https://www.cnblogs.com/dhcn/p/7124896.html)
1、Restful服务的实现方式一共三种：function based view，class based view，viewset+router，这三种实现方式的封装重度依序升高，越往后越适合典型CURD服务，因为我们的产品是互联网产品，服务接口大多数是业务操作，所以主要以function based view为主，偶尔使用class based view。
2、序列化器Serializer的使用，序列化器在输入处理中类似Form，实际是反序列化的作用。同时对输出数据的格式有所定制，真正的序列化定制。当然序列化要想完整使用，就要用modal序列化器，而这个序列化器要直接使用需要是RDB存储，这在如今这个NoSQL盛行的时代，有所约束，不过也算好了。
3、基本配置
- REST_FRAMEWORK = {  
- 'DEFAULT_PARSER_CLASSES': (  
- 'rest_framework.parsers.JSONParser',#主要使用JSon解析器  
-     ),  
- 
- 'DEFAULT_AUTHENTICATION_CLASSES': (  
- 'rest_framework.authentication.TokenAuthentication',#for Restful服务验证  
- 'rest_framework.authentication.SessionAuthentication',#for 自动生成文档页面的验证  
-     ),  
- 'DEFAULT_PERMISSION_CLASSES': (  
- 'rest_framework.permissions.IsAuthenticated',#所有view的默认认证  
-     )  
- 
- }  
4、form文件上传问题，默认使用的JSon解析器可能会对multipart的data body有干扰，导致request.Files解析直接失败，给客户端返回415答复，这个时候需要手动给相关view修改解析器，比如如下函数装饰器配置：
- @parser_classes((MultiPartParser,))  
5、引用到user的序列化器对user外键字段的输出要格外小心，这个地方有可能连user的密码字段都输出掉，所以引用user的外键，一定要从序列化器的depth设置或者序列化器关系字段定义等方式上，让user的输出符合你的要求。
6、Field的定制，Field的定制化改写方式是改写to_representation或to_internal_value，前者负责把变量转化成字符串，后者负责把字符串转化成变量，我们这个项目中的FileField为了输出完整路径，改写了to_representation方法：
- class TFileField(serializers.FileField):  
- def to_representation(self, obj):  
- 
- if not obj:  
- return None  
- 
- if obj.name.find("/static/") == 0:  
- return MEDIA_DOMAIN + obj.name  
- else:  
- return MEDIA_DOMAIN + "/static/" + obj.url  
7、Serializer的定制化改写，我说一个我的改写场景，某对象的聚合对象的属性需要和主对象属性并列，这个采用官方Serializer的特性只能做到一个外键ID属性并列，其他属性都不可以，于是可以改写Serializer的to_representation如下：
- def to_representation(self, instance):  
- 
-     ret = super(ContactSerializer,self).to_representation(instance)  
-     ret["username"] = instance.user.username  
- return ret  
8、调用序列化对象的save时报错：
- ValueError: "<Topic: Topic object>" needs to have a value for field "topic" before this many-to-many relationship can be used.  
这个错误在form的save操作时也会出，错误原因，一个新对象在未存储前，不能对他的ManyToMany的关系做操作，原因也很简单，ManyToMany需要新对象的ID，方法就是在序列化器的存储前，先对对象本身save一下，这样新对象有了ID，操作就Ok了。
8、应用加载不完全错误：错误提示：
- AttributeError: type object 'Token' has no attribute 'objects'  
错误原因，Token所在的应用'rest_framework.authtoken'没有加在INSTALLED_APPS配置里面，加上即可解决问题。

