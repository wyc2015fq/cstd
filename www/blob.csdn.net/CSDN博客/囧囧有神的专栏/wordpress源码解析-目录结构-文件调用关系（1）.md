# wordpress源码解析-目录结构-文件调用关系（1） - 囧囧有神的专栏 - CSDN博客





2012年10月03日 21:20:47[囧囧有神](https://me.csdn.net/liujiyong7)阅读数：44064








         学习开源代码，是一种很快的提升自己的学习方法。Wordpress作为一个开源的博客系统，非常优秀，应用广泛，使用起来简单方便，具有丰富的主题和插件，可以按照自己的需求来任意的进行修改。所以就从wordpress的源码开始自己的php学习之路,希望自己能坚持下去！

         Wordpress版本：3.1.2

         平台：新浪sae平台。[monk.sinaapp.com](http://monk.sinaapp.com)



        Wordpress是一个单入口的文件，所有的前端处理都必须经过index.php,这是通过修改web服务器的rewrite规则来实现的。这种做法的好处是显而易见的，这样URL更好看，不必为每一个url新建一个文件。

        我们看看wp大致的文件调用是什么样子的。

![](https://img-my.csdn.net/uploads/201210/03/1349271036_9698.gif)


        wordpress可以分为3个阶段，一是**初始化阶段**，即初始化常量、环境、加载核心文件等等；二是**内容处理阶段**，即根据用户的请求调用相关函数获取和处理数据，为前端展示准备数据；三是**主题应用阶段**，在这个阶段，需要展示的数据已经准备完毕，需要根据用户的请求加载相应的主题模板，即对主题进行路由。经过这三各阶段，用户请求的页面就可以完全的展现出来了。从上图可以看到wp的初始化阶段相当的繁琐。


         Index.php文件实际上没啥内容. 定义了常量WP_USE_THEMES和加载了文件wp-blog-header.php



```php
<?php
/*index.php*/
//该常量定义为false时，不使用主题，站点会显示为空白，为true时，则正常显示。
define('WP_USE_THEMES', true);
//加载wp-blog-header.php
require('./wp-blog-header.php');
?>
```
       wp--blog-header里面是什么样子呢？





```
<?php
/*wp-blog-header.php */
//wp_did_header变量，相当于一个flag，确保每次刷新时，wp-blog-header.php文件只执行一次。
if ( !isset($wp_did_header) ) {

	$wp_did_header = true;//置为true，确保只执行一次。
	//初始化阶段。加载wp-load.php,
	require_once( dirname(__FILE__) . '/wp-load.php' );
	//内容处理阶段 wp()位于functon.php中
	wp();
	//主题应用阶段。ABSPATH wordpress目录的绝对路径
	require_once( ABSPATH . WPINC . '/template-loader.php' );

}
```
      wp-blog-header任然很简单，但是可以看到wp的三个明显的阶段，初始化阶段、内容处理阶段、主题应用阶段。


     wp-load.php的功能很简单，就是加载wp-config.php. wp-config.php是需要用户自己进行设置的。设置DB设置，密钥，路径，语言等





```php
<?php
/*wp-load.php
 */

// 定义常量 wordpress根目录的绝对路径 
define( 'ABSPATH', dirname(__FILE__) . '/' );
//设置错误报告模式 这个if的分支 差别在E_RECOVERABLE_ERROR ？
if ( defined('E_RECOVERABLE_ERROR') )
	error_reporting(E_CORE_ERROR | E_CORE_WARNING | E_COMPILE_ERROR | E_ERROR | E_WARNING | E_PARSE | E_USER_ERROR | E_USER_WARNING | E_RECOVERABLE_ERROR);
else
	error_reporting(E_CORE_ERROR | E_CORE_WARNING | E_COMPILE_ERROR | E_ERROR | E_WARNING | E_PARSE | E_USER_ERROR | E_USER_WARNING);
//加载wp-config.php
if ( file_exists( ABSPATH . 'wp-config.php') ) {

	/** The config file resides in ABSPATH */
	require_once( ABSPATH . 'wp-config.php' );

} elseif ( file_exists( dirname(ABSPATH) . '/wp-config.php' ) && ! file_exists( dirname(ABSPATH) . '/wp-settings.php' ) ) {

	/** The config file resides one level above ABSPATH but is not part of another install*/
	//如果wp-config.php存在于绝对路径的上一级，而且不是另一个wordpress的一部分
	require_once( dirname(ABSPATH) . '/wp-config.php' );

} else {

	// wp-config.php 不存在时，

	// Set a path for the link to the installer
	if ( strpos($_SERVER['PHP_SELF'], 'wp-admin') !== false )
		$path = '';
	else
		$path = 'wp-admin/';

	// Die with an error message
	require_once( ABSPATH . '/wp-includes/class-wp-error.php' );
	require_once( ABSPATH . '/wp-includes/functions.php' );
	require_once( ABSPATH . '/wp-includes/plugin.php' );
	$text_direction = /*WP_I18N_TEXT_DIRECTION*/'从左到右'/*/WP_I18N_TEXT_DIRECTION*/;
	wp_die(sprintf(/*WP_I18N_NO_CONFIG*/'看起来似乎没有 <code>wp-config.php</code> 文件。我们需要这个文件来让一切开始，可以查看<a href=\'http://codex.wordpress.org/Editing_wp-config.php\'>更多帮助</a>。 那么现在您可以通过这个 Web 界面创建一个 <code>wp-config.php</code> 文件，但并非所有主机都支持，安全的做法是手动创建。</p><p><a href=\'%ssetup-config.php\' class=\'button\'>试试创建一个配置文件</a>'/*/WP_I18N_NO_CONFIG*/, $path), /*WP_I18N_ERROR_TITLE*/'WordPress › 错误'/*/WP_I18N_ERROR_TITLE*/, array('text_direction' => $text_direction));
}
?>
```




来看看wp-config.php是什么样子的。



```php
<?php
/*wp-config.php */

// ** MySQL 设置 - 具体信息来自您正在使用的主机 ** //
/** WordPress 数据库的名称 */
define('DB_NAME', SAE_MYSQL_DB);

/** MySQL 数据库用户名 */
define('DB_USER', SAE_MYSQL_USER);

/** MySQL 数据库密码 */
define('DB_PASSWORD', SAE_MYSQL_PASS);

/** MySQL 主机 */
define('DB_HOST', SAE_MYSQL_HOST_M.':'.SAE_MYSQL_PORT);

/** 创建数据表时默认的文字编码 */
define('DB_CHARSET', 'utf8');

/** 数据库整理类型。如不确定请勿更改 */
define('DB_COLLATE', '');

define('WP_USE_MULTIPLE_DB', true);

$db_list = array(
		'write'=> array(
			array(
				'db_host' => SAE_MYSQL_HOST_M.':'.SAE_MYSQL_PORT,
				'db_user'=> SAE_MYSQL_USER,
				'db_password'=> SAE_MYSQL_PASS,
				'db_name'=> SAE_MYSQL_DB,
				'db_charset'=> 'utf8'
				)
			),
		'read'=> array(
			array(
				'db_host' => SAE_MYSQL_HOST_S.':'.SAE_MYSQL_PORT,
				'db_user'=> SAE_MYSQL_USER,
				'db_password'=> SAE_MYSQL_PASS,
				'db_name'=> SAE_MYSQL_DB,
				'db_charset'=> 'utf8'
				)
			),
		);
$global_db_list = $db_list['write'];

/**#@+
 * 身份密匙设定。
 *
 * 您可以随意写一些字符
 * 或者直接访问 {@link https://api.wordpress.org/secret-key/1.1/salt/ WordPress.org 私钥生成服务}，
 * 任何修改都会导致 cookie 失效，所有用户必须重新登录。
 *
 * @since 2.6.0
 */
define('AUTH_KEY',         hash_hmac('sha1', SAE_ACCESSKEY . 'AUTH_KEY', SAE_SECRETKEY ));
define('SECURE_AUTH_KEY',  hash_hmac('sha1', SAE_ACCESSKEY . 'SECURE_AUTH_KEY', SAE_SECRETKEY ));
define('LOGGED_IN_KEY',    hash_hmac('sha1', SAE_ACCESSKEY . 'LOGGED_IN_KEY', SAE_SECRETKEY ));
define('NONCE_KEY',        hash_hmac('sha1', SAE_ACCESSKEY . 'NONCE_KEY', SAE_SECRETKEY ));
define('AUTH_SALT',        hash_hmac('sha1', SAE_ACCESSKEY . 'AUTH_SALT', SAE_SECRETKEY ));
define('SECURE_AUTH_SALT', hash_hmac('sha1', SAE_ACCESSKEY . 'SECURE_AUTH_SALT', SAE_SECRETKEY ));
define('LOGGED_IN_SALT',   hash_hmac('sha1', SAE_ACCESSKEY . 'LOGGED_IN_SALT', SAE_SECRETKEY ));
define('NONCE_SALT',       hash_hmac('sha1', SAE_ACCESSKEY . 'NONCE_SALT', SAE_SECRETKEY ));

/**#@-*/

/**
 * WordPress 数据表前缀。
 *
 * 如果您有在同一数据库内安装多个 WordPress 的需求，请为每个 WordPress 设置不同的数据表前缀。
 * 前缀名只能为数字、字母加下划线。
 */
$table_prefix  = 'wp_';

/**
 * WordPress 语言设置，默认为英语。
 *
 * 本项设定能够让 WordPress 显示您需要的语言。
 * wp-content/languages 内应放置同名的 .mo 语言文件。
 * 要使用 WordPress 简体中文界面，只需填入 zh_CN。
 */
define ('WPLANG', 'zh_CN');

/**
 * 开发者专用：WordPress 调试模式。
 *
 * 将这个值改为“true”，WordPress 将显示所有开发过程中的提示。
 * 强烈建议插件开发者在开发环境中启用本功能。
 */
define('WP_DEBUG', false);

/* 好了！请不要再继续编辑。请保存该文件。 */

/** WordPress 目录的绝对路径。 */
if ( !defined('ABSPATH') )
	define('ABSPATH', dirname(__FILE__) . '/');

/** 设置 WordPress 变量和包含文件。 这个文件内容很多啊，分析较困难*/
require_once(ABSPATH . 'wp-settings.php');
```





参考：

http://www.cnblogs.com/vicenteforever/articles/1866848.html 







