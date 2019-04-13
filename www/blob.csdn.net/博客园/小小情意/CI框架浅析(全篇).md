
# CI框架浅析(全篇) - 小小情意 - 博客园






# [CI框架浅析(全篇)](https://www.cnblogs.com/xiaoxiaoqingyi/p/6901654.html)
业余花了点时间看看CodeIgniter框架（简称CI），CI目前的稳定版本是 3.X，4.0版本已经出来了，但还在测试中，所以我分析的还是 3.x 版本。
CI是一个很轻便的框架，整个下载包也就2M多，而且使用起来方便快捷，适用于一些简单的功能开发，以及做app 接口。
该框架整个流程图如下：
![](https://images2015.cnblogs.com/blog/804587/201705/804587-20170524233717450-1541824943.png)
index.php 文件作为前端控制器，初始化运行 CodeIgniter 所需的基本资源；
Router 检查 HTTP 请求，以确定如何处理该请求；
如果存在缓存文件，将直接输出到浏览器，不用走下面正常的系统流程；
在加载应用程序控制器之前，对 HTTP 请求以及任何用户提交的数据进行安全检查；
控制器加载模型、核心类库、辅助函数以及其他所有处理请求所需的资源；
最后一步，渲染视图并发送至浏览器，如果开启了缓存，视图被会先缓存起来用于 后续的请求。
下载框架源码，解压得到如下代码结构：
![](https://images2015.cnblogs.com/blog/804587/201705/804587-20170524234017654-1450857793.png)


主要有三个目录
1、application目录：用于开发者编写相应的配置以及逻辑处理，开发者只需在这个目录下添加自己需要的开发文件。
2、system目录：框架的系统库，里面包括核心库，类库，辅助类库，数据库等，这些文件，开发者最好不要擅自修改，它是整个框架的龙脉。
3、user_guide：用户手册。

**接下来看看源码的请求流程：**
![](https://images2015.cnblogs.com/blog/804587/201705/804587-20170524234143966-63724447.png)

首先假设有一个 URL 请求，入口就是 index.php，该文件定义了几个常量，应用的路径，以及核心库的路径等。
接着引入 核心库system/core下的 CodeIgniter.php文件，该文件初始化核心库system/core里的类库，分别是：
{
  ● benchmark:"Benchmark",
  ● hooks:"Hooks",
  ● config:"Config",
  ● log:"Log",
  ● utf8:"Utf8",
  ● uri:"URI",
  ● router:"Router",
  ● output:"Output",
  ● security:"Security",
  ● input:"Input",
  ● lang:"Lang",
  ● loader:"Loader"}
每个类库的注释在上图已有解释。
同时也加载 应用项目 application/config目录下的配置文件（这些配置文件都是开发者根据自己的需要去添加与配置），
根据判断加载字符串处理库mbstring，添加错误异常预处理方法。在加载的同时，也把钩子部署到了相应位置，如果开发者定义了相应钩子实现方法，就会在相应的位置执行。
在 CodeIgniter.php 初始化核心库的时候定义了五个钩子，分别如下：


pre_system 在系统执行的早期调用，这个时候只有 基准测试类 和 钩子类 被加载了， 还没有执行到路由或其他的流程。
pre_controller 在你的控制器调用之前执行，所有的基础类都已加载，路由和安全检查也已经完成。
post_controller_constructor 在你的控制器实例化之后立即执行，控制器的任何方法都还尚未调用。
post_controller 在你的控制器完全运行结束时执行。
post_system 在最终的页面发送到浏览器之后、在系统的最后期被调用。
然后，实例化 CI_Controller 类：
function &get_instance()
{
return CI_Controller::get_instance();
}
通过路由 router 及 uri 得到请求的 controller控制器、method方法 以及参数，执行该方法。
期间根据开发者在application/config目录下的配置，会加载相应的 librays 类库、 helper辅助函数 及 DB 库。
如果你喜欢MVC的开发模式，也可以添加model类，然后加载 model 模型类，处理相应的业务逻辑。
最后在自己定义的controller控制器处理好的数据结果渲染在html 页面上，展示给用户。


**下面看一下CI框架几个重要部分：**
**控制器**
开发者在 application/controller 目录下添加自己的controller 控制器，但是每个控制器都要继承核心库里的基类 CI_Controller，它已获取到整个框架的核心类库对象，通过它基本可以调用CI框架下的核心方法。
**模型**
模型就是专门用来和数据库打交道的 PHP 类，开发者在 application/models 目录下定义自己的模型类，都要继承 模型基类 CI_Mode。当你在控制下调用模型，只需要下面一行代码就实例化了：
$this->load->model('model_name');
**辅助函数**
当然开发者也可以创建自己的辅助类，文件存放在 application/helpers 目录下，调用的方式与系统
的辅助类一致。
$this->load 就是 Loader.php 文件CI_Load 类实例， 我们来看看 CI_Load 类下的 helper() 函数：
/**
     * Helper Loader
     *
     * @param    string|string[]    $helpers    Helper name(s)
     * @return    object*/publicfunctionhelper($helpers=array())
    {is_array($helpers) OR$helpers=array($helpers);foreach($helpersas&$helper)
        {$filename=basename($helper);$filepath= ($filename===$helper) ? '' :substr($helper, 0,strlen($helper) -strlen($filename));$filename=strtolower(preg_replace('\#(_helper)?(\.php)?$\#i', '',$filename)).'_helper';$helper=$filepath.$filename;if(isset($this->_ci_helpers[$helper]))
            {continue;
            }//Is this a helper extension request?$ext_helper= config_item('subclass_prefix').$filename;$ext_loaded=FALSE;foreach($this->_ci_helper_pathsas$path)
            {if(file_exists($path.'helpers/'.$ext_helper.'.php'))
                {include_once($path.'helpers/'.$ext_helper.'.php');$ext_loaded=TRUE;
                }
            }//If we have loaded extensions - check if the base one is hereif($ext_loaded===TRUE)
            {$base_helper= BASEPATH.'helpers/'.$helper.'.php';if( !file_exists($base_helper))
                {
                    show_error('Unable to load the requested file: helpers/'.$helper.'.php');
                }include_once($base_helper);$this->_ci_helpers[$helper] =TRUE;
                log_message('info', 'Helper loaded: '.$helper);continue;
            }//No extensions found ... try loading regular helpers and/or overridesforeach($this->_ci_helper_pathsas$path)
            {if(file_exists($path.'helpers/'.$helper.'.php'))
                {include_once($path.'helpers/'.$helper.'.php');$this->_ci_helpers[$helper] =TRUE;
                    log_message('info', 'Helper loaded: '.$helper);break;
                }
            }//unable to load the helperif( !isset($this->_ci_helpers[$helper]))
            {
                show_error('Unable to load the requested file: helpers/'.$helper.'.php');
            }
        }return$this;
    }
这段代码主要是 加载（include_once） system/helpers 与 appliation/helpers 目录下的 $name_helper.php 名称文件，自定义的辅助函数文件需要添加 前缀 来与 系统的辅助函数区分开。当执行完加载函数，就能得到 $this->name 实例，然后调用它里面的函数。
所有辅助函数如下：



数组辅助函数
验证码辅助函数
Cookie 辅助函数
日期辅助函数
目录辅助函数
下载辅助函数
邮件辅助函数
文件辅助函数
表单辅助函数
HTML 辅助函数
Inflector 辅助函数
语言辅助函数
数字辅助函数
路径辅助函数
安全辅助函数
表情辅助函数
字符串辅助函数
文本辅助函数
排版辅助函数
URL 辅助函数
[XML 辅助函数](https://codeigniter.org.cn/user_guide/helpers/xml_helper.html)
CI 类库
所有的系统类库都位于 system/libraries/ 目录下，大多数情况下，在使用之前， 你要先在控制器中初始化它，使用下面的方法:
$this->load->library('class_name');
'class_name' 是你想要调用的类库名称，例如，要加载 表单验证类库，你可以这样做:
$this->load->library('form_validation');
一旦类库被载入，你就可以根据该类库的用户指南中介绍的方法去使用它了，这个类似于辅助函数。
同样拓展自己的类库也是在application/libraries 目录下。
一旦加载，你就可以使用小写字母名称来访问你的类，调用方法：
$this->someclass->some_method();
所有的类库如下：

[基准测试类](https://codeigniter.org.cn/user_guide/libraries/benchmark.html)
[缓存驱动器](https://codeigniter.org.cn/user_guide/libraries/caching.html)
[日历类](https://codeigniter.org.cn/user_guide/libraries/calendar.html)
[购物车类](https://codeigniter.org.cn/user_guide/libraries/cart.html)
[配置类](https://codeigniter.org.cn/user_guide/libraries/config.html)
[Email 类](https://codeigniter.org.cn/user_guide/libraries/email.html)
[加密类](https://codeigniter.org.cn/user_guide/libraries/encrypt.html)
[加密类（新版）](https://codeigniter.org.cn/user_guide/libraries/encryption.html)
[文件上传类](https://codeigniter.org.cn/user_guide/libraries/file_uploading.html)
[表单验证类](https://codeigniter.org.cn/user_guide/libraries/form_validation.html)
[FTP 类](https://codeigniter.org.cn/user_guide/libraries/ftp.html)
[图像处理类](https://codeigniter.org.cn/user_guide/libraries/image_lib.html)
[输入类](https://codeigniter.org.cn/user_guide/libraries/input.html)
[Javascript 类](https://codeigniter.org.cn/user_guide/libraries/javascript.html)
[语言类](https://codeigniter.org.cn/user_guide/libraries/language.html)
[加载器类](https://codeigniter.org.cn/user_guide/libraries/loader.html)
[迁移类](https://codeigniter.org.cn/user_guide/libraries/migration.html)
[输出类](https://codeigniter.org.cn/user_guide/libraries/output.html)
[分页类](https://codeigniter.org.cn/user_guide/libraries/pagination.html)
[模板解析类](https://codeigniter.org.cn/user_guide/libraries/parser.html)
[安全类](https://codeigniter.org.cn/user_guide/libraries/security.html)
[Session 类](https://codeigniter.org.cn/user_guide/libraries/sessions.html)
[HTML 表格类](https://codeigniter.org.cn/user_guide/libraries/table.html)
[引用通告类](https://codeigniter.org.cn/user_guide/libraries/trackback.html)
[排版类](https://codeigniter.org.cn/user_guide/libraries/typography.html)
[单元测试类](https://codeigniter.org.cn/user_guide/libraries/unit_testing.html)
[URI 类](https://codeigniter.org.cn/user_guide/libraries/uri.html)
[用户代理类](https://codeigniter.org.cn/user_guide/libraries/user_agent.html)
[XML-RPC 与 XML-RPC 服务器类](https://codeigniter.org.cn/user_guide/libraries/xmlrpc.html)
[Zip 编码类](https://codeigniter.org.cn/user_guide/libraries/zip.html)
### 数据库
CI框架封装了多种数据库驱动与连接方法，让你轻松配置就能连接上，且封装了一些查询构造器与生成查询结果，让代码看起来方便简洁。
你只需 在application/config/database.php 文件下配置你链接的参数：
$db['default'] =array('dsn'    => '',
    'hostname' => 'localhost',
    'username' => '',
    'password' => '',
    'database' => '',
    'dbdriver' => 'mysqli',
    'dbprefix' => '',
    'pconnect' =>FALSE,
    'db_debug' => (ENVIRONMENT !== 'production'),
    'cache_on' =>FALSE,
    'cachedir' => '',
    'char_set' => 'utf8',
    'dbcollat' => 'utf8_general_ci',
    'swap_pre' => '',
    'encrypt' =>FALSE,
    'compress' =>FALSE,
    'stricton' =>FALSE,
    'failover' =>array(),
    'save_queries' =>TRUE);
然后在 Controller 里调用一句 ：$this->load->database(); 就能连接上数据库。
接着，你可以查询你想要的结果：
$this->db->where('name',$name);
$query=$this->db->get('mytable',10,20);
// Executes: SELECT * FROM mytable where name=$nameLIMIT 20, 10
CI框架也提供了数据库的事务处理，如：
$this->db->trans_start();$this->db->query('AN SQL QUERY...');$this->db->query('ANOTHER QUERY...');$this->db->query('AND YET ANOTHER QUERY...');$this->db->trans_complete();

### 提供了简单的查询缓存：
将查询结果对象会被序列化并保存到服务器上的一个文本文件中。 当下次再访问该页面时，会直接使用缓存文件而不用访问数据库了。只有读类型（SELECT）的查询可以被缓存。 这个相对应 Java 的hibernate 数据库映射 就弱化了很多，Java提供了三级的缓存方式，而且在查询数据库的时候，并不会每次都断开，再连接。
以上都是CI框架提供的重要组成部分，也许它可能满足不了你所有的需求，但也提供了一些给你拓展的方式，如在application/core目录下添加你的核心类，这都是CI框架已考虑到的问题。当然它在处理一些繁杂的业务逻辑的时候，还是比较薄弱的，比如说权限使用，模块灵活增删等。
CI框架主要是以轻便，快捷上手为主要的优势，让你去处理一些简单的项目。它介于一个没有框架与一个比较笨重的框架之间，所以一个框架好不好用，还要基于你的需求。
CI框架还提供了一些其它便捷的开发帮助，它有有自己的模板引擎，也有程序分析：
你可以在你的 控制器 方法的任何位置添加一行下面的代码:
$this->output->enable_profiler(TRUE);
**设置基准测试点**
$this->benchmark->mark('code_start');// Some code happens here$this->benchmark->mark('code_end');echo$this->benchmark->elapsed_time('code_start','code_end');
**最后输出分析的信息：**
$sections=array('config'=>TRUE,'queries'=>TRUE);
$this->output->set_profiler_sections($sections);

**下表列出了可用的分析器字段和用来访问这些字段的 key :**
|Key|Description|Default|
|benchmarks|在各个计时点花费的时间以及总时间 TRUE|
|config|CodeIgniter 配置变量 TRUE|
|controller_info|被请求的控制器类和调用的方法 TRUE|
|get|请求中的所有 GET 数据 TRUE|
|http_headers|本次请求的 HTTP 头部 TRUE|
|memory_usage|本次请求消耗的内存（单位字节） TRUE|
|post|请求中的所有 POST 数据 TRUE|
|queries|列出所有执行的数据库查询，以及执行时间 TRUE|
|uri_string|本次请求的 URI TRUE|
|session_data|当前会话中存储的数据 TRUE|
|query_toggle_count|指定显示多少个数据库查询，剩下的则默认折叠起来 25|







以上是我对CI框架大致的了解与分析，我做php的经验不多，这是我第一个尝试去深入了解的php框架，请大家多多指教。









