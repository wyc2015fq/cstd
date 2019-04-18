# [PDO]①0--quote()方法防止SQL注入 - weixin_33985507的博客 - CSDN博客
2017年09月04日 10:17:00[weixin_33985507](https://me.csdn.net/weixin_33985507)阅读数：9
# test.php
```
<<!doctype html>
<html lang="en">
<head>
    <meta charset="UTF-8">
    <meta name="viewport"
          content="width=device-width, user-scalable=no, initial-scale=1.0, maximum-scale=1.0, minimum-scale=1.0">
    <meta http-equiv="X-UA-Compatible" content="ie=edge">
    <title>Document</title>
</head>
<body>
<form action="doAction.php" method="post">
    用户名:<input type="text" name="username"/><br/>
    密码:<input type="password" name="password"/><br/>
    <input type="submit" value="登陆">
</form>
</body>
</html>
```
# doAction.php
```
<?php
header('content-type:text/html;charset=utf-8');
$username =& $_POST['username'];
$password = $_POST['password'];
try {
    $pdo = new PDO('mysql:host=localhost;dbname=test', 'root', '');
    //sql注入 'or 1=1 #
    //$sql = "select * from user where username='{$username}' and password = '{$password}'";
    $username = $pdo->quote($username);
    //通过quote() 返回带引号的字符串 过滤字符串中的特殊字符
    //防止Sql注入
    $sql = "select * from user where username={$username} and password='{$password}'";
    //select * from user where username='\'or 1=1 \'' and password='king2'
    //echo $sql;
    $stmt = $pdo->query($sql);
    echo $stmt->rowCount();//0 1...对于select 返回结果集中记录的条数
    //对于INSERT UPDATE DELETE 返回受影响的记录的条数
} catch (PDOException $e) {
    echo $e->getMessage();
}
?>
```
![2953340-05915139d9757aff.png](https://upload-images.jianshu.io/upload_images/2953340-05915139d9757aff.png)
Paste_Image.png
![2953340-84f934752918ea9a.png](https://upload-images.jianshu.io/upload_images/2953340-84f934752918ea9a.png)
Paste_Image.png
