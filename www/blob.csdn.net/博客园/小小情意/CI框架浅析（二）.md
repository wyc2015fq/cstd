
# CI框架浅析（二） - 小小情意 - 博客园






# [CI框架浅析（二）](https://www.cnblogs.com/xiaoxiaoqingyi/p/6706882.html)
该文延续上篇文章：[CI框架浅析（一）](http://www.cnblogs.com/xiaoxiaoqingyi/p/6654190.html)
在CI框架的核心库中，CodeIgniter.php负责加载所有需要的类库，第一个加载的是公共库 core/Common.php
Common.php 负责加载以下公共方法：
|方法名
|注释|
|is_php($version)
|判断$version是否小于或等于当前安装的PHP版本
|
|is_really_writable($file)
|判断该文件是否可写
|
|load_class($class, $directory = 'libraries', $param = NULL)
|加载类，但只能加载前缀为CI 的类，或前缀为自己配置的 $config['subclass_prefix'
|
|is_loaded
|返回已经加载类的数组
|
|get_config(Array $replace = array())
|加载 application/config/config.php 配置数组
|application/config/ENVIRONMENT/config.php ENVIRONMENT为你开发的环境
|
|config_item($item)
|获取配置项的具体一项值
|
|get_mimes()
|获取config/mimes.php 的元数据数组
|
|is_https()
|判断是否是 https 加密协议
|
|is_cli()
|判断是否是在命令行执行
|
|show_error()
|加载 Exception，在自己的模板输出错误，然后exit
|
|show_404($page = '', $log_error = TRUE)
|加载 Exception，在自己的模板输出404错误
|
|log_message($level, $message)
|输出日志信息(不要把config.php 的log_threshold变量设 置为 4，太卡了，消耗太多的性能)
|
|set_status_header($code = 200, $text = '')
|设置http回应状态头
|
|_error_handler( )
|错误处理，加载Exceptions类，然后用日志输出错误，如果是验证的错误，则exit(1)退出程序。
|
|_exception_handler($exception)
|异常处理，加载Exceptions类，记录异常，然后 exit(1)退出。
|
|_shutdown_handler()
|宕机处理，不同的错误级别做相应的处理，调用_error_handler 方法处理
|
|remove_invisible_characters（）
|移除不可见的字符
|
|html_escape($var, $double_encode = TRUE)
|html 转义
|
|_stringify_attributes($attributes, $js = FALSE)
|将数组等其它类型，转换成字符串类型。
|
|function_usable($function_name)
|方法可用，该方法只是暂时的，后期可能会移除。这方法需要加载Suhosin （一个PHP的补丁或拓展）
|
Common.php中也调用了 Exceptions.php :
|方法名|注释|
|log_exception($severity, $message, $filepath, $line)
|记录异常信息，保存在日志里
|
|show_404($page = '', $log_error = TRUE)
|记录错误的日志，且调用show_error()输出错误页面
|
|show_error($heading, $message, $template = 'error_general', $status_code = 500)
|找到对应的模板，然后输出错误的html页面
|
|show_exception($exception)
|输出异常的html页面
|
|show_php_error($severity, $message, $filepath, $line)
|本地PHP出错处理，输出相应的错误页面。
|

if(file_exists(APPPATH.'config/'.ENVIRONMENT.'/constants.php'))
{require_once(APPPATH.'config/'.ENVIRONMENT.'/constants.php');
}
从代码可以知道：
加载框架的自定义常量，可在applcation/config 创建不同环境目录，如development 与production，在这两个目录下，都创建constants.php，然后创建各环境需要的常量。从其它的代码总结来，可以根据开发的情况，给予配置不同常量，以及config 的几乎所有配置，让用户在开发或者发布的时候，配置不同的信息。





