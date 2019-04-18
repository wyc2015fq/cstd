# 我的第一个python web开发框架（28）——定制ORM（四） - weixin_33985507的博客 - CSDN博客
2018年08月03日 17:08:00[weixin_33985507](https://me.csdn.net/weixin_33985507)阅读数：2
　　在数据库操作时，新增记录也是必不可少的，接下来我们应用字典的特性来组合sql语句
　　先上产品新增接口代码
```
1 @post('/api/product/')
 2 def callback():
 3     """
 4     新增记录
 5     """
 6     name = web_helper.get_form('name', '产品名称')
 7     code = web_helper.get_form('code', '产品编码')
 8     product_class_id = convert_helper.to_int0(web_helper.get_form('product_class_id', '产品分类'))
 9     standard = web_helper.get_form('standard', '产品规格')
10     quality_guarantee_period = web_helper.get_form('quality_guarantee_period', '保质期')
11     place_of_origin = web_helper.get_form('place_of_origin', '产地')
12     front_cover_img = web_helper.get_form('front_cover_img', '封面图片')
13     content = web_helper.get_form('content', '产品描述', is_check_special_char=False)
14     # 防sql注入攻击处理
15     content = string_helper.filter_str(content, "'")
16     # 防xss攻击处理
17     content = string_helper.clear_xss(content)
18     is_enable = convert_helper.to_int0(web_helper.get_form('is_enable', '是否启用'))
19 
20     # 添加记录（使用returning这个函数能返回指定的字段值，这里要求返回新添加记录的自增id值）
21     sql = """insert into product (name, code, product_class_id, standard, quality_guarantee_period,
22                 place_of_origin, front_cover_img, content, is_enable)
23               values (%s, %s, %s, %s, %s, %s, %s, %s, %s) returning id"""
24     vars = (name, code, product_class_id, standard, quality_guarantee_period, place_of_origin, front_cover_img, content, is_enable)
25     # 写入数据库
26     result = db_helper.write(sql, vars)
27     # 判断是否提交成功
28     if result and result[0].get('id'):
29         return web_helper.return_msg(0, '成功')
30     else:
31         return web_helper.return_msg(-1, "提交失败")
```
　　在21行到24行就是sql语句的拼接，使用这种方法，我们经常会因为多写或少写%s和变量，导致sql执行出错。
　　在python中，我们最常用的就是字典，从新增记录的语句中，可以看到字段与值是一一对应的，那么我们就可以考虑使用字典的方式来进行存储字段与值，在ORM中对其进行处理，然后拼接生成对应的sql语句。
　　操作步骤如下几步：
　　1.将新增记录时的字段名与值，使用字典方式存储起来
　　2.将字典做为参数传给ORM新增记录方法
　　3.在进行下一步之前，大家首先要了解python字符串替的有多种方法，例如：方法一：’Hello %s' % ('world',)  ；方法二：'Hello %(name)s' % {'name': 'world'} ；等，还有很多其他方法这里不一一列举。第一种只能按照指定的顺序进行替换，数量必须一致，不然会出错；而第二种字典类型替换时，字典位置不需要考虑，字典值超出替换内容，会自动忽略，它最大的好处，我觉得是可读性，让这个字符串变的更加容易理解。所以下面我们将会使用字典替换方法。
　　4.新增记录方法接收到参数以后，使用for循环，将字段名提取出来，生成sql插入字段名数组和字典替换数组，即：insert into table_name (字段名,字段名...) values (值,值...)，这里需要将字典中的字段名提取出来组合成“字段名,字段名...”这样的串，值也是一样，为了让组合后的字符串更容易调试与理解，我们将它生成字典替换格式%(name1)s,%(name2)s...，最好再使用字典直接替换（当然你直接将值连接起来也行，你可以根据自己的需要进行调整）。
　　例如，我们需要添加产品名称、产品编码、产品分类id，可以它们用字典存储起来做为参数
```
fields = {
    'name': "'名称'",
    'code': "'201808031245678'",
    'product_class_id': 1,
}
```
　　然后可以通过for循环，将字典参数进行处理，提取出来存储到list中
```
key_list = []
value_list = []
for key in fields.keys():
    key_list.append(key)
    value_list.append('%(' + key + ')s')
```
　　执行后，key_list与value_list的值分别为：
```
key_list = ['name', 'product_class_id', 'code']
value_list = ['%(name)s', '%(product_class_id)s', '%(code)s']
```
　　然后我们设置一个sql字符串拼接字典，将表名、字段名字符串与值字符串存储进去，在存储前使用join方式进行拼接，生成用逗号分隔的字符串
```
parameter = {
    'table_name': self.__table_name,
    'key_list': ','.join(key_list),
    'value_list': ','.join(value_list)
}
```
　　执行后生成的值为：
```
parameter = {'key_list': 'name,product_class_id,code', 'value_list': '%(name)s,%(product_class_id)s,%(code)s', 'table_name': 'product'}
```
　　然后将它们与新增sql合成
```
sql = "insert into %(table_name)s (%(key_list)s) values (%(value_list)s) returning id " % parameter
```
　　执行后sql值为：
```
sql = 'insert into product (name,product_class_id,code) values (%(name)s,%(product_class_id)s,%(code)s) '
```
　　最后将它与最开始提交的字典参数进行合成
```
sql = sql % fields
```
　　生成最终可执行的sql语句
```
sql = "insert into product (name,product_class_id,code) values ('名称',1,'201808031245678') "
```
　　完整代码
```
1     def add_model(self, fields, returning=''):
 2         """新增数据库记录"""
 3         ### 拼接sql语句 ###
 4         # 初始化变量
 5         key_list = []
 6         value_list = []
 7         # 将传入的字典参数进行处理，把字段名生成sql插入字段名数组和字典替换数组
 8         # PS:字符串使用字典替换参数时，格式是%(name)s，这里会生成对应的字串
 9         # 比如：
10         #   传入的字典为： {'id': 1, 'name': '名称'}
11         #   那么生成的key_list为：'id','name'
12         #   而value_list为：'%(id)s,%(name)s'
13         #   最终而value_list为字符串对应名称位置会被替换成相应的值
14         for key in fields.keys():
15             key_list.append(key)
16             value_list.append('%(' + key + ')s')
17         # 设置sql拼接字典，并将数组（lit）使用join方式进行拼接，生成用逗号分隔的字符串
18         parameter = {
19             'table_name': self.__table_name,
20             'pk_name': self.__pk_name,
21             'key_list': ','.join(key_list),
22             'value_list': ','.join(value_list)
23         }
24         # 如果有指定返回参数，则添加
25         if returning:
26             parameter['returning'] = ', ' + returning
27         else:
28             parameter['returning'] = ''
29 
30         # 生成可以使用字典替换的字符串
31         sql = "insert into %(table_name)s (%(key_list)s) values (%(value_list)s) returning %(pk_name)s %(returning)s" % parameter
32         # 将生成好的字符串替字典参数值，生成最终可执行的sql语句
33         sql = sql % fields
34 
35         with db_helper.PgHelper(self.__db, self.__is_output_sql) as db:
36             # 执行sql语句
37             result = db.execute(sql)
38             if result:
39                 db.commit()
40                 return result[0]
41             return {}
```
　　代码大家多debug一下，就可以查看到每一行代码的执行情况以及sql拼接生成情况了。
　　这里的sql语句额外添加了postgresql的returning函数，在默认情况下会返回数据插入成功后的主键值给主程序。我们在进行新增操作时，经常要获取新增记录的自增id，通过return id就可以将它返回给我们，如果你还需要返回其他字段值，可以在这个新增方式的returning参数中添加想要返回的字段名，如果想返回整条记录，输入*号就可以了。
　　写到这里，可以看到查询与执行，都使用类似的with方法，我们可以将它们提取出来重构成独立的函数，为后续的修改和其他查询去除重复代码，改造后代码如下：
```
1 #!/usr/bin/env python
  2 # coding=utf-8
  3 
  4 from common import db_helper
  5 
  6 
  7 class LogicBase():
  8     """逻辑层基础类"""
  9 
 10     def __init__(self, db, is_output_sql, table_name, column_name_list='*', pk_name='id'):
 11         """类初始化"""
 12         # 数据库参数
 13         self.__db = db
 14         # 是否输出执行的Sql语句到日志中
 15         self.__is_output_sql = is_output_sql
 16         # 表名称
 17         self.__table_name = str(table_name).lower()
 18         # 查询的列字段名称，*表示查询全部字段，多于1个字段时用逗号进行分隔，除了字段名外，也可以是表达式
 19         self.__column_name_list = str(column_name_list).lower()
 20         # 主健名称
 21         self.__pk_name = str(pk_name).lower()
 22 
 23     #####################################################################
 24     ### 执行Sql ###
 25 
 26     def get_model(self, wheres):
 27         """通过条件获取一条记录"""
 28         # 如果有条件，则自动添加where
 29         if wheres:
 30             wheres = ' where ' + wheres
 31 
 32         # 合成sql语句
 33         sql = "select %(column_name_list)s from %(table_name)s %(wheres)s" % \
 34               {'column_name_list': self.__column_name_list, 'table_name': self.__table_name, 'wheres': wheres}
 35         # 初化化数据库链接
 36         result = self.select(sql)
 37         if result:
 38             return result[0]
 39         return {}
 40 
 41     def get_model_for_pk(self, pk, wheres=''):
 42         """通过主键值获取数据库记录实体"""
 43         if not pk:
 44             return {}
 45         # 组装查询条件
 46         wheres = '%s = %s' % (self.__pk_name, str(pk))
 47 
 48         return self.get_model(wheres)
 49 
 50     def add_model(self, fields, returning=''):
 51         """新增数据库记录"""
 52         ### 拼接sql语句 ###
 53         # 初始化变量
 54         key_list = []
 55         value_list = []
 56         # 将传入的字典参数进行处理，把字段名生成sql插入字段名数组和字典替换数组
 57         # PS:字符串使用字典替换参数时，格式是%(name)s，这里会生成对应的字串
 58         # 比如：
 59         #   传入的字典为： {'id': 1, 'name': '名称'}
 60         #   那么生成的key_list为：'id','name'
 61         #   而value_list为：'%(id)s,%(name)s'
 62         #   最终而value_list为字符串对应名称位置会被替换成相应的值
 63         for key in fields.keys():
 64             key_list.append(key)
 65             value_list.append('%(' + key + ')s')
 66         # 设置sql拼接字典，并将数组（lit）使用join方式进行拼接，生成用逗号分隔的字符串
 67         parameter = {
 68             'table_name': self.__table_name,
 69             'pk_name': self.__pk_name,
 70             'key_list': ','.join(key_list),
 71             'value_list': ','.join(value_list)
 72         }
 73         # 如果有指定返回参数，则添加
 74         if returning:
 75             parameter['returning'] = ', ' + returning
 76         else:
 77             parameter['returning'] = ''
 78 
 79         # 生成可以使用字典替换的字符串
 80         sql = "insert into %(table_name)s (%(key_list)s) values (%(value_list)s) returning %(pk_name)s %(returning)s" % parameter
 81         # 将生成好的字符串替字典参数值，生成最终可执行的sql语句
 82         sql = sql % fields
 83 
 84         result = self.execute(sql)
 85         if result:
 86             return result[0]
 87         return {}
 88 
 89     def select(self, sql):
 90         """执行sql查询语句（select）"""
 91         with db_helper.PgHelper(self.__db, self.__is_output_sql) as db:
 92             # 执行sql语句
 93             result = db.execute(sql)
 94             if not result:
 95                 result = []
 96         return result
 97 
 98     def execute(self, sql):
 99         """执行sql语句，并提交事务"""
100         with db_helper.PgHelper(self.__db, self.__is_output_sql) as db:
101             # 执行sql语句
102             result = db.execute(sql)
103             if result:
104                 db.commit()
105             else:
106                 result = []
107         return result
108 
109     #####################################################################
```
　　上单元测试代码：
```
#!/usr/bin/evn python
# coding=utf-8
import unittest
from common.string_helper import string
from logic import product_logic
class DbHelperTest(unittest.TestCase):
    """数据库操作包测试类"""
    def setUp(self):
        """初始化测试环境"""
        print('------ini------')
    def tearDown(self):
        """清理测试环境"""
        print('------clear------')
    def test(self):
        ##############################################
        # 只需要看这里，其他代码是测试用例的模板代码 #
        ##############################################
        # 实例化product表操作类ProductLogic
        _product_logic = product_logic.ProductLogic()
        # 执行get_model_for_pk()方法，获取记录实体
        model = _product_logic.get_model_for_pk(2)
        print(model)
        # 测试新增记录
        fields = {
            'name': string('名称'),
            'code': string('201808031245678'),
            'product_class_id': 1,
        }
        result = _product_logic.add_model(fields, '*')
        print(result)
        ##############################################
if __name__ == '__main__':
    unittest.main()
```
　　PS：单元测试中，通过from common.string_helper import string导入了string这个方法，它是用来给字符串添加单撇号字符（'）用的，因为在更新数据库时，字符串类型需要用单撇号括起来的，而Python中直接使用单撇字符只是标识它为字符串而已，需要用\'或双引号+单撇字符+双引号方式，才能得到我们想要的结果，为了方便操作，所以增加了string这个方法，帮我们进行转换。
　　输出结果：
```
------ini------
{'content': '产品描述', 'name': '饼干', 'place_of_origin': '广东广州', 'add_time': datetime.datetime(2018, 7, 25, 23, 10, 4), 'code': '20180212321211', 'is_enable': 1, 'front_cover_img': 'http://xxx.com/xxx.jpg', 'product_class_id': 1, 'quality_guarantee_period': '2018年12月', 'id': 2, 'standard': '500g'}
{'content': '', 'name': '名称', 'place_of_origin': '', 'add_time': datetime.datetime(2018, 8, 3, 16, 51, 3), 'code': '201808031245678', 'is_enable': 0, 'front_cover_img': '', 'product_class_id': 1, 'quality_guarantee_period': '', 'id': 15, 'standard': ''}
------clear------
```
　　前面的接口也可以改造为
```
1 @post('/api/product/')
 2 def callback():
 3     """
 4     新增记录
 5     """
 6     name = web_helper.get_form('name', '产品名称')
 7     code = web_helper.get_form('code', '产品编码')
 8     product_class_id = convert_helper.to_int0(web_helper.get_form('product_class_id', '产品分类'))
 9     standard = web_helper.get_form('standard', '产品规格')
10     quality_guarantee_period = web_helper.get_form('quality_guarantee_period', '保质期')
11     place_of_origin = web_helper.get_form('place_of_origin', '产地')
12     front_cover_img = web_helper.get_form('front_cover_img', '封面图片')
13     content = web_helper.get_form('content', '产品描述', is_check_special_char=False)
14     # 防sql注入攻击处理
15     content = string_helper.filter_str(content, "'")
16     # 防xss攻击处理
17     content = string_helper.clear_xss(content)
18     is_enable = convert_helper.to_int0(web_helper.get_form('is_enable', '是否启用'))
19 
20     # 设置新增参数
21     fields = {
22         'name': string(name),
23         'code': string(code),
24         'product_class_id': product_class_id,
25         'standard': string(standard),
26         'quality_guarantee_period': string(quality_guarantee_period),
27         'place_of_origin': string(place_of_origin),
28         'front_cover_img': string(front_cover_img),
29         'content': string(content),
30         'is_enable': is_enable,
31     }
32     # 实例化product表操作类ProductLogic
33     _product_logic = product_logic.ProductLogic()
34     # 新增记录
35     result = _product_logic.add_model(fields)
36     # 判断是否提交成功
37     if result:
38         return web_helper.return_msg(0, '成功')
39     else:
40         return web_helper.return_msg(-1, "提交失败")
```
　　代码行数比之前是增加了，但可读性好了很多
版权声明：本文原创发表于 [博客园](http://www.cnblogs.com/)，作者为 **[AllEmpty](http://www.cnblogs.com/EmptyFS/)**本文欢迎转载，但未经作者同意必须保留此段声明，且在文章页面明显位置给出原文连接，否则视为侵权。
python开发QQ群：669058475（本群已满）、733466321（可以加2群）    作者博客：http://www.cnblogs.com/EmptyFS/
