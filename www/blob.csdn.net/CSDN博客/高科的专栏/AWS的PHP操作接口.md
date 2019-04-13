
# AWS的PHP操作接口 - 高科的专栏 - CSDN博客

2018年08月28日 19:48:11[高科](https://me.csdn.net/pbymw8iwm)阅读数：438


demo.php
![复制代码](https://common.cnblogs.com/images/copycode.gif)
<?php
/* 
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */
define('AWS_KEY', 'input your key');
define('AWS_SECRET_KEY', 'input your secret key');
$HOST = 'input your endpoint';
// require the amazon sdk for php library
require_once dirname(__FILE__).'/sdk.class.php';
// Instantiate the S3 class and point it at the desired host
$s3 = new AmazonS3(array(
        'key' => AWS_KEY,
        'secret' => AWS_SECRET_KEY,
));
$s3->set_hostname($HOST);
$s3->allow_hostname_override(false);
// Set the S3 class to use objects.dreamhost.com/bucket
// instead of bucket.objects.dreamhost.com
$s3->enable_path_style();

$bucketname="test_wx";
\#=========================create_bucket====================
echo str_repeat("=", 30)."create_bucket".str_repeat("=", 30)."\n";
$ret=$s3->create_bucket($bucketname, AmazonS3::REGION_US_E1);
echo print_r($ret,1),"\n";
\#=========================list_buckets====================
echo str_repeat("=", 30)."list_buckets".str_repeat("=", 30)."\n";
$ListResponse = $s3->list_buckets();
$Buckets = $ListResponse->body->Buckets->Bucket;
foreach ($Buckets as $Bucket) {
        echo $Bucket->Name . "\t" . $Bucket->CreationDate . "\n";
}

\#=========================create_object====================
echo str_repeat("=", 30)."create_object:".$bucketname.str_repeat("=", 30)."\n";
$s3->create_object($bucketname, 'hello.txt', array(
        'body' => "Hello World!",
        'acl'=>AmazonS3::ACL_PUBLIC,\#对对象进行权限分配
));
echo "create file hello.txt and assign public authority to it \n";
$ret=$s3->create_object($bucketname, 'upload.jpg', array(
        'fileUpload' => dirname(__FILE__)."/j01.png",
        'acl'=>AmazonS3::ACL_PUBLIC,\#对对象进行权限分配
));
echo "return url:".$ret->header['_info']['url'],"\n";
echo "upload file j01.png and assign public authority to it \n";
\#=========================set_object_acl  对对象进行权限更改====================
echo str_repeat("=", 30)."set_object_acl:".$bucketname.str_repeat("=", 30)."\n";
$s3->set_object_acl($bucketname,"hello.txt", AmazonS3::ACL_PRIVATE);
echo "change file hello.txt authority  \n";
\#=========================list_objects====================
$ObjectsListResponse = $s3->list_objects($bucketname);
$Objects = $ObjectsListResponse->body->Contents;
echo str_repeat("=", 30)."list_objects:".$bucketname.str_repeat("=", 30)."\n";
foreach ($Objects as $Object) {
        echo $Object->Key . "\t" . $Object->Size . "\t" . $Object->LastModified . "\n";
}
\#=========================get_object_url  获取url====================
echo str_repeat("=", 30)."get_object_url:".$bucketname.str_repeat("=", 30)."\n";
$secret_url = $s3->get_object_url($bucketname, 'j02.jpg', '1 hour');
echo $secret_url . "\n";

\#=========================DOWNLOAD AN OBJECT====================
\#This downloads the object upload.jpg and saves it in d:/
$FileHandle = fopen('d:/upload.jpg', 'w+');
$s3->get_object($bucketname, 'upload.jpg', array(
        'fileDownload' => $FileHandle,
));
\#=========================delete_object====================
echo str_repeat("=", 30)."delete_object".str_repeat("=", 30)."\n";
$s3->delete_object($bucketname, 'hello.txt');
\#=========================delete_bucket====================
echo str_repeat("=", 30)."delete_bucket".str_repeat("=", 30)."\n";
$ret=$s3->delete_bucket($bucketname, 1); \#This will delete the bucket even if it is not empty.
echo print_r($ret,1),"\n";![复制代码](https://common.cnblogs.com/images/copycode.gif)
sdk.class.php library下载地址:
https://github.com/amazonwebservices/aws-sdk-for-php
参考地址：
http://docs.ceph.com/docs/master/radosgw/s3/php/\#change-an-object-s-acl


