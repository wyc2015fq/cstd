# Django单元测试简明实践 - =朝晖= - 博客园
# [Django单元测试简明实践](https://www.cnblogs.com/dhcn/p/7124678.html)
1、准备数据模式，Django空库[测试](http://lib.csdn.net/base/softwaretest)需要所有相关数据模式必须在Django中定义，说白了，model不能有managed=Fasle,为了解决这个问题，你必须得有一个managed全部为True的Schema代码版本。
2、准备初始化数据，当然使用fixtures固定数据集特性，这个数据集最好现存库里面查询出要用的数据，然后用Django序列化器生成所需的格式即可。
3、如permission，contenttype这类系统生成的数据，不需要做fixture，但是其中如果有你自己自定义插进去的数据，就需要做fixture，而且做的时候，不要把系统自动添加的数据做上，否则会导致测试中的系统自动添加行为失败。
4、自建permission数据的准备问题：因为permission引用的contentype数据和生产库的实际数据是有可能有区别的，主要是ID顺序不同，所以，permission的contenttypeID需要手动查找合适的contenttype，然后修改到fixtures里面
5、文本比对一定要从正确输出的HTML截取字符串，从浏览器的某些界面下copy的字符串文本比对会出错误，当然除了文本比对，django testcase还提供了其他几种结构化数据的比对方式。
6、像问题4中的数据，还有那种有及时性要求的数据，可以在Test代码中手动往数据里添加，然后再做相关的测试操作。
7、使用StaticLiveServerTestCase的时候，出现以下错误：
- Hint: Look at the output of 'django-admin sqlflush'. That's the SQL this command wasn't able to run.  
- The full error: cannot truncate a table referenced in a foreign key constraint  
- DETAIL:  Table "users" references "analyst_analyst".  
- HINT:  Truncate table "users" at the same time, or use TRUNCATE ... CASCADE.  
通过查看TestCase源码，在类中定义非空的available_apps列表配置，就可以解决这个问题。添加的时候，要把能用到的都添加上，否则可能会报错，TestCase没有这个问题
8、我使用的是WebDriver里面的ChromeDriver,除了pip安装selenium以外，还需要到 https://sites.google.com/a/chromium.org/chromedriver/downloads 页面下载最新的chromedriver，解压以后是个可执行文件，把那个文件扔到随便那个系统PATH目录下即可。
9、在使用StaticLiveServerTestCase的时候，又报以下错误：
- No translation files found for default language zh-cn  
解决方法：在settings里面添加配置：
- LOCALE_PATHS = [BASE_DIR]  
然后在这个项目根目录BASE_DIR中添加/zh-CN/LC_MESSAGES/django.po

