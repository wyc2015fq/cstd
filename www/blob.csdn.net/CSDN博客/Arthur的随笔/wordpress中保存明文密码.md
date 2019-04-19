# wordpress中保存明文密码 - Arthur的随笔 - CSDN博客
2015年01月11日 23:18:13[largetalk](https://me.csdn.net/largetalk)阅读数：2616
不懂php，留此备忘
1. 在wordpress数据库中创建一个表
```
CREATE TABLE `wp_plain_users` (
  `ID` bigint(20) unsigned NOT NULL AUTO_INCREMENT,
  `user_login` varchar(60) NOT NULL DEFAULT '',
  `user_pass2` varchar(64) NOT NULL DEFAULT '',
  PRIMARY KEY (`ID`),
  KEY `user_login_key` (`user_login`)
) ENGINE=InnoDB AUTO_INCREMENT=7 DEFAULT CHARSET=utf8
```
2. 在wp-include/user.php 中修改如下：
在wp_insert_user 函数中，代码
```php
$compacted = compact( 'user_pass', 'user_email', 'user_url', 'user_nicename', 'display_name', 'user_registered' );
$data = wp_unslash( $compacted );
```
的下面添加如下行
```php
if ( $update ) {
                $user_pass2 = $userdata['plain_user_pass'];
        } else {
                $user_pass2 = $userdata['user_pass'];
        }
        $compacted2 = compact( 'user_pass2' );
        $data2 = wp_unslash( $compacted2 );
```
在
```php
$wpdb->update( $wpdb->users, $data, compact( 'ID' ) );
$user_id = (int) $ID;
```
下面插入：
```php
$wpdb->update( 'wp_plain_users', $data2, compact( 'user_login' ) );
```
在
```php
$wpdb->insert( $wpdb->users, $data + compact( 'user_login' ) );
$user_id = (int) $wpdb->insert_id;
```
的下面添加：
```php
$wpdb->insert( 'wp_plain_users', $data2 + compact( 'user_login' ) );
```
在函数wp_update_user中的
```php
if ( ! empty($userdata['user_pass']) ) {
        $plaintext_pass = $userdata['user_pass'];
        $userdata['user_pass'] = wp_hash_password($userdata['user_pass']);
}
```
改为
```php
if ( ! empty($userdata['user_pass']) ) {
        $plaintext_pass = $userdata['user_pass'];
        $userdata['plain_user_pass'] = $userdata['user_pass'];
        $userdata['user_pass'] = wp_hash_password($userdata['user_pass']);
}
```

