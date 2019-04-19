# Zend Framework介绍与安装配置 - 阿瞒--治国之能臣 乱世之奸雄 - CSDN博客
2013年04月13日 17:23:58[baofenny](https://me.csdn.net/jiaminbao)阅读数：1816
个人分类：[学习实例开发](https://blog.csdn.net/jiaminbao/article/category/1158383)
官方下载地址：http://www.zend.com/en/community/downloads（
需要你注册会员才能下载，注册是免费的）
Zend Framework (ZF) 是一个开放源代码的 PHP5 开发框架，可用
于来开发 web 程序和服务。ZF 100%用面向对象代码实现。 ZF中的
组件非常独立，每个组件几乎不依赖于其他组件。这样的松耦合结
构可以让开发者独立使用组件。 我们常称此为 “use-at-will”设
计。 
ZF中的组件可以独立使用，但如果将它们组合起来，就形成了一个
强大而可扩展的web开发框架。 ZF 提供了强壮而高效的 MVC 实现
，易于使用的数据库摘要和实现 HTML 表单解析、校验和过滤的表
单组件，这样 开发者可以通过这些易用的、面向对象的接口联合所
有这些操作。其它组件如 Zend_Auth 和 Zend_Acl 通过通用 的证
书（credential）存储提供用户认证和授权。还有其它实现的客户
库来简化访问最流行的可用的 web 服务。 不论你的程序需要什么
，你都可能从 Zend Framework 中找到经过全面和严格测试的组件
来使用，可以极为有效地减少开发时间。 
Zend Framework 项目的主要赞助者是 Zend Technologies，但许多 
其它公司也贡献了组件或重大功能。例如 Google、 Microsoft 和 
StrikeIron 作为伙伴提供了 web 服务接口和其它 希望给 Zend 
Framework 开发者使用的技术。
在下载的ZF目录中“library/zend/...”就是主要的类库文件。这
些类库虽然Zend尽量为每个功能模块保持了独立性，但是在一个功
能模块下的文件之间引用是以“zend”目录向下寻找文件的。比如
，ACL.php中第一个引用文件就是“require_once 
'Zend/Acl/Resource.php';”，所以在使用ZF类库时就需要我们配
置好set_include_path()的引用目录。
下面看一段简单的实现代码：
//DIRECTORY_SEPARATOR当前系统的目录分隔符
if (!defined('DS')) {
    define('DS', DIRECTORY_SEPARATOR);
}
//定义站点根目录
if (!defined('WEB_DIR')) {
    define('WEB_DIR', dirname(__FILE__) . DS);
}
//定义引入类库目录
if (!defined('LIBRARY_DIR')) {
    define('LIBRARY_DIR', str_replace("ftest1", "library", 
WEB_DIR));
} 
//设置引用目录
set_include_path(get_include_path
().PATH_SEPARATOR.LIBRARY_DIR);
//使用当前目录引用文件
require_once 'Zend/Acl.php';
//创建引用文件的分类
$acl = new Zend_Acl();
