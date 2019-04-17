# wordpress源码解析-数据库表结构（2） - 囧囧有神的专栏 - CSDN博客





2012年10月05日 23:20:43[囧囧有神](https://me.csdn.net/liujiyong7)阅读数：18128








如果是一个普通的用户，不需要了解wordpress数据库的结构。但是，如果你正在写一个插件，你应该会对wordpress如何处理它的数据和关系感兴趣。如果你已经尝试使用已经存在的wordpress api 去访问你需要的数据，但不直接访问数据库的情况下，这是不可能的，WordPress的提供WPDB的类，使这项任务变得简单。

WordPress数据库的11个数据表分别是：


|**表名（点击表名查看详细介绍）**|**描述**|
|----|----|
|wp_commentmeta|文章评论额外信息表|
|wp_comments|文章评论信息表|
|wp_links|链接信息表|
|wp_options|基本配置信息表，通常通过get_option来操作，该表通常作为插件存储数据的一个地方。|
|wp_postmeta|文章额外数据表，例如文章浏览次数，文章的自定义字段等都存储在这里。|
|wp_posts|文章信息表，包括了日志、附件、页面等等信息。是WordPress最重要的一个数据表。|
|wp_terms|文章分类、链接分类、标签的信息表。|
|wp_term_relationships|分类与文章信息表（wp_posts）、链接表(wp_links)的关联表。|
|wp_term_taxonomy|分类信息表，区分wp_terms信息的分类类型，有category、link_category和tag三种分类类型。|
|wp_usermeta|用户额外信息表|
|wp_users|用户基本信息表。存放系统所有用户基本信息。|


WordPress**数据库表之间的关系是什么呢？**

要搞明白这个问题以及对WordPress数据表之间的关系有更加深刻的理解，让我们先来想想WordPress博客系统的功能，默认安装的WordPress2.6.2版本博客系统涉及的数据信息主要包括了用户信息、分类信息、链接信息、文章信息、文章评论信息、基本配置信息这6类信息。

用户信息：系统中所有注册用户的帐号信息。

分类信息：包括了文章分类、链接分类、标签这3中分类信息。

链接信息：就是博客系统中的友情链接信息。

文章信息：博客系统中的日志、日志产生的附件、页面等信息。

文章评论信息：对具体日志或者附件的评论信息。

基本配置信息：系统中的基本配置信息，例如博客名称，博客地址等等。

WordPress数据库的11个数据表就是为了存储以上6类数据信息而设计，这些表之间的总体关系结构如下图所示：

![](https://img-my.csdn.net/uploads/201210/05/1349445394_7840.png)




从上图可以看出WordPress博客系统6类数据信息分别所涉及的数据表及关系如下表所示：


|**信息类型**|**涉及数据表及关联关系**|
|----|----|
|用户信息|数据表：wp_users、wp_usermeta，关联关系：wp_users.ID->wp_usermeta.user_id|
|分类信息|数据表：wp_terms、wp_term_taxonomy关联关系：wp_terms.term_id->wp_term_taxonomy.term_id|
|链接信息|数据表：wp_links、wp_term_relationships、wp_terms、wp_term_taxonomy、 wp_users、wp_usermeta关联关系：一，确定链接所属分类 （1）wp_links.link_id->wp_term_relationships.object_id， （2）wp_term_relationships.term_taxonomy_id->wp_term_taxonomy.term_taxonomy_id（该关系还要取决与wp_term_taxonomy表中的taxonomy分类类型为“link_category”） （3）wp_terms.term_id->wp_term_taxonom.term_id二、确定链接所有者 （4）wp_links.link_owner->wp_users.ID（5）wp_users.ID->wp_usermeta.user_id|
|文章信息|数据表：wp_posts、wp_postmeta、wp_comments、wp_term_relationships、wp_terms、 wp_term_taxonomy、wp_users、wp_usermeta关联关系：一、确定文章信息 （1）wp_posts.ID->wp_postsmeta.post_id二、确定文章评论 （2）wp_posts.ID->wp_comments.comment_post_id三、确定文章评论的作者 （3）wp_comments.comment_author->wp_users.ID（4）wp_users.ID->wp_usermeta.user_id四、确定文章所属分类（5）wp_posts.ID->wp_term_relationships.object_id，（6）wp_term_relationships.term_taxonomy_id->wp_term_taxonomy.term_taxonomy_id（该关系还要取决与wp_term_taxonomy表中的taxonomy分类类型为“category”或者“tag”）（7）wp_terms->term_id->wp_term_taxonomy五、确定文章作者（8）wp_posts.author->wp_users.ID;（9）wp_users.ID->wp_usermeta.user_id|
|文章评论信息|数据表：wp_comments、wp_posts、wp_users、wp_usermeta关联关系：一、确定评论的文章 （1）wp_comments.comment_post_id->wp_posts.ID二、确定评论的作者 （2）wp_comments.comment_author->wp_users.ID（3）wp_users.ID->wp_usermeta.user_id|
|基本配置信息|数据表：wp_options没有关联关系|

Wp-config.php中关于数据库的设置。



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


wp-db.php 包含一个wpdb的类，是数据库操作API 。产生一个$wpdb的全局变量，是wpdb类的一个对象。挺长的，贴在文章里太臃肿了。







参考：




[http://wenku.baidu.com/view/6448da5bbe23482fb4da4c94.html?from_page=view&from_mod=download](http://wenku.baidu.com/view/6448da5bbe23482fb4da4c94.html?from_page=view&from_mod=download)

[http://codex.wordpress.org/Database_Description](http://codex.wordpress.org/Database_Description)

[http://codex.wordpress.org/Function_Reference/wpdb_Class](http://codex.wordpress.org/Database_Description)

[http://www.jakc.net/post/272](http://www.jakc.net/post/272)





