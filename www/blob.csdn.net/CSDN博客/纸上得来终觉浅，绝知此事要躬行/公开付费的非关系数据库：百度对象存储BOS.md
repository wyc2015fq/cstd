# 公开付费的非关系数据库：百度对象存储BOS - 纸上得来终觉浅，绝知此事要躬行 - CSDN博客





2015年06月02日 16:02:05[boonya](https://me.csdn.net/boonya)阅读数：4626








      今天收到百度BCS的邮件通知：

![](https://img-blog.csdn.net/20150602153328774?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvYm9vbnlh/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)


     百度对象存储BOS（Baidu Object Storage），提供稳定、安全、高效以及高扩展存储服务，支持单文件最大5TB的文本、多媒体、二进制等任何类型的数据存储。数据多地域跨集群的存储，以实现资源统一利用，降低使用难度，提高工作效率。




# 特性

**灵活管理数据**
- 
创建、查看、删除Bucket
- 用户最多可以创建100个Bucket。
- 用户可以修改、查看Bucket的访问权限。

- 
上传、查看、删除Object 
- 用户可以存储的Object没有数量限制。
- Object支持文本、多媒体、二进制等任何类型的数据。
- 每个Object可包含从0字节到5TB的数据。
- 用户可使用通用的和自定义的元信息机制来定义资源属性。


**大文件分块上传/断点下载**
- 对于超过5GB的超大文件，用户可以使用分块上传机制；超过5MB但不超过5GB的文件可以由用户选择是否需要分块上传。
- 用户将一个超大文件拆分成子Object（又称Part），分别上传这些子Object，上传完成后BOS将自动检查数据，并将子Object组合成一个Object，完成分块上传。
- 分块后的单个Part不能超过5GB，除最后一个Part外，单个Part不能小于5MB。
- 该功能在网络条件较差，或上传文件前无法确定文件大小等场景下具有非常好的表现。

**完善的API和开发工具**
- BOS提供标准的REST接口，可与任何Internet开发工具包搭配使用。
- BOS基于原生的REST API，主要提供了以下三类API：
- Bucket操作
- Object操作
- ACL操作

- BOS提供了多种语言的SDK，Java、Python、PHP等。

**功能完善的管理平台**
- 用户可以通过Web管理平台方便的操作Bucket和Object。
- 用户可以通过Web管理平台中的ACCESSKEY管理功能轻松的创建和管理自己的Access Key ID / Secret Access Key。
- 用户可以通过Web管理平台提供的监控功能，实时监控Bucket的读写情况和占用空间等信息。

**海量数据存储**
- BOS支持任何类型数据的存储、分享及使用。允许用户将整个存储基础设施转移到云端，利用BOS的扩展性和按需付费的优势，以处理不断增长的存储需求。
- BOS支持最大5TB的单文件存储，利用断点上传及下载功能，可以存储多媒体视频、软件包等超大文件。


# 应用场景



**内容存储和分享**

BOS允许企业用户和开发者将整个存储基础设施转移到云端，支持按需使用和付费。BOS支持任何类型数据（文本、图片、音乐、视频、日志、应用程序等）的存储，用户既可以直接访问，也可以把BOS作为应用程序的后端存储，还可以将指定内容共享。

**用于数据分析的存储**

无论是存储用于分析的商业或科学数据，用于调整的图片，还是用于转码的视频，BOS都是企业用户和开发者存储原始数据的理想场所。BOS支持用户使用BMR及云媒体服务进行访问和计算，在BMR和BOS之间的数据传输不会产生任何费用，降低用户使用成本。

**灾备系统**

BOS为灾备系统提供稳定、安全、低成本和高可扩展的存储支持，适用于政府和企事业单位关键数据的定期备份和异常恢复，有效保障核心数据的高安全性和高可用性。

**网盘服务**

BOS为网盘服务提供丰富的数据类型支持，满足海量的数据存储需求。存储容量弹性扩展，存储数据安全稳定，有力支持网盘业务需求的不断增长，有效降低成本，提升用户体验。

# 项目实际使用验证



**直接使用JAR包**

步骤如下：

1.在[官方网站](http://bce.baidu.com/doc/SDKTool/index.html)下载Java SDK压缩工具包。

2.将下载的`bce-java-sdk-version.zip`解压后，复制到工程文件夹中。

3.在Eclipse右键“工程 -> Properties -> Java Build Path -> Add JARs”。

4.添加SDK工具包`lib/bce-java-sdk-version.jar`和第三方依赖工具包`third-party/*.jar`。

其中，`version`为版本号，经过上面几步之后，用户就可以在工程中使用BOS Java SDK。

**运行环境**

Java SDK工具包可在jdk1.6、jdk1.7、jdk8环境下运行。

**版本动态**

本文档针对BOS Java SDK 0.8.2版本，历史更新如下：

*版本 0.8.2*

无针对BOS的更新，用户可以继续使用上一版本而不受影响。

*版本 0.8.1*

更新copyObject中对copy_source的处理方法，当source object为非标准ASCII字符（例如：中文）时使用老版本Java SDK的用户会出现签名无法通过的现象，请及时进行版本更新。

## Java验证代码



### Bucket





```java
package com.boonya.bos.bucket;

import java.util.ArrayList;
import java.util.List;
import com.baidubce.services.bos.BosClient;
import com.baidubce.services.bos.model.BucketSummary;
import com.baidubce.services.bos.model.CannedAccessControlList;
import com.baidubce.services.bos.model.Grant;
import com.baidubce.services.bos.model.Grantee;
import com.baidubce.services.bos.model.Permission;

public class Bucket
{
    /**
     * 创建bucket
     * 
     * @param client
     * @param bucketName
     */
    public void createBucket(BosClient client, String bucketName)
    {
	// 新建一个Bucket
	client.createBucket(bucketName);
    }
    
    /**
     * 获取bucket列表
     * 
     * @param client
     */
    public void listBuckets(BosClient client)
    {
	// 获取用户的Bucket列表
	List<BucketSummary> buckets = client.listBuckets().getBuckets();
	
	// 遍历Bucket
	for (BucketSummary bucket : buckets)
	{
	    System.out.println(bucket.getName());
	}
    }
    
    /**
     * 判断Bucket是否存在
     * 
     * @param client
     * @param bucketName
     */
    public boolean doesBucketExist(BosClient client, String bucketName)
    {
	
	// 获取Bucket的存在信息
	boolean exists = client.doesBucketExist(bucketName);
	
	// 输出结果
	if (exists)
	{
	    System.out.println("Bucket exists");
	} else
	{
	    System.out.println("Bucket not exists");
	}
	return exists;
    }
    
    /**
     * 删除Bucket
     * 
     * @param client
     * @param bucketName
     */
    public void deleteBucket(BosClient client, String bucketName)
    {
	// 删除Bucket
	client.deleteBucket(bucketName);
    }
    
    /**
     * 设置Bucket的访问权限
     * 
     * @param client
     * @param bucketName
     */
    public void setBucketPrivate(BosClient client, String bucketName)
    {
	client.setBucketAcl(bucketName, CannedAccessControlList.Private);
    }
    
    /**
     * 设置指定用户对Bucket的访问权限
     * 
     * @param client
     */
    public void SetBucketAclFromBody(BosClient client)
    {
	List<Grant> grants = new ArrayList<Grant>(); 
	List<Grantee> grantee = new ArrayList<Grantee>();
	List<Permission> permission = new ArrayList<Permission>();
	
	// 授权给特定用户
	grantee.add(new Grantee("UserId_1"));
	grantee.add(new Grantee("UserId_2"));
	// 授权给Everyone
	grantee.add(new Grantee("*"));
	
	// 设置权限
	//Permission中的权限设置包含三个值：READ、WRITE、FULL_CONTROL
	permission.add(Permission.READ);
	permission.add(Permission.WRITE);
	
	grants.add(new Grant().withGrantee(grantee).withPermission(permission));
	//client.setBucketAcl("bucketName", grants);
    }
}
```




### Object



```java
package com.boonya.bos.object;

import java.io.File;
import java.io.FileInputStream;
import java.io.FileNotFoundException;
import java.io.IOException;
import java.io.InputStream;
import java.net.URL;
import java.util.HashMap;
import java.util.Map;
import com.baidubce.services.bos.BosClient;
import com.baidubce.services.bos.model.BosObject;
import com.baidubce.services.bos.model.BosObjectSummary;
import com.baidubce.services.bos.model.CopyObjectRequest;
import com.baidubce.services.bos.model.CopyObjectResponse;
import com.baidubce.services.bos.model.GetObjectRequest;
import com.baidubce.services.bos.model.ListObjectsResponse;
import com.baidubce.services.bos.model.ObjectMetadata;
import com.baidubce.services.bos.model.PutObjectResponse;

public class Object
{
    /**
     * BOS Java SDK本质上是调用后台的HTTP接口，因此BOS服务允许用户自定义Object的Http Header
     * 
     * @param client
     * @param bucketName
     * @param objectKey
     * @param content
     */
    public void setHttpHeader(BosClient client, String bucketName, String objectKey, String content)
    {
	// 初始化上传输入流
	ObjectMetadata meta = new ObjectMetadata();
	
	// 设置ContentLength大小
	meta.setContentLength(1000);
	
	// 设置ContentType
	meta.setContentType("application/json");
	
	client.putObject(bucketName, objectKey, content, meta);
    }
    
    /**
     * 用户自定义元数据
     * 
     * @param client
     * @param bucketName
     * @param objectKey
     * @param content
     */
    public void selfDefinitedObject(BosClient client, String bucketName, String objectKey, String content)
    {
	// 初始化上传输入流
	ObjectMetadata meta = new ObjectMetadata();
	
	// 设置ContentLength大小
	meta.setContentLength(1000);
	// 设置自定义元数据name的值为my-data
	meta.addUserMetadata("name", "my-data");
	
	// 上传Object
	client.putObject(bucketName, objectKey, content, meta);
    }
    
    /**
     * 添加对象到Bucket
     * 
     * @param client
     * @param bucketName
     * @param objectKey
     * @param content
     */
    public void putObject(BosClient client, String bucketName, String objectKey, String content)
    {
	// 以字符串上传Object
	PutObjectResponse putObjectResponseFromString = client.putObject(bucketName, objectKey, content);
	
	// 打印ETag
	System.out.println(putObjectResponseFromString.getETag());
    }
    
    /**
     * BOS一共支持四种形式的Object上传，参考如下代码
     * 
     * @param client
     * @param bucketName
     * @param objectKey
     * @param byte1
     * @param string1
     * @throws FileNotFoundException
     */
    public void PutObject(BosClient client, String bucketName, String objectKey, byte[] byte1, String string1) throws FileNotFoundException
    {
	// 获取指定文件
	File file = new File("/path/to/file.zip");
	// 获取数据流
	InputStream inputStream = new FileInputStream("/path/to/test.zip");
	
	// 以文件形式上传Object
	PutObjectResponse putObjectFromFileResponse = client.putObject(bucketName, objectKey, file);
	// 以数据流形式上传Object
	PutObjectResponse putObjectResponseFromInputStream = client.putObject(bucketName, objectKey, inputStream);
	// 以二进制串上传Object
	PutObjectResponse putObjectResponseFromByte = client.putObject(bucketName, objectKey, byte1);
	// 以字符串上传Object
	PutObjectResponse putObjectResponseFromString = client.putObject(bucketName, objectKey, string1);
	
	// 打印ETag
	System.out.println(putObjectFromFileResponse.getETag());
    }
    
    /**
     * 列出Bucket中的Object
     * 
     * @param client
     * @param bucketName
     */
    public void listObjects(BosClient client, String bucketName)
    {
	
	// 获取指定Bucket下的所有Object信息
	ListObjectsResponse listing = client.listObjects(bucketName);
	
	// 遍历所有Object
	for (BosObjectSummary objectSummary : listing.getContents())
	{
	    System.out.println("ObjectKey: " + objectSummary.getKey());
	}
	
    }
    
    /**
     * 简单的读取Object
     * 
     * @param client
     * @param bucketName
     * @param objectKey
     * @throws IOException
     */
    public void getObject(BosClient client, String bucketName, String objectKey) throws IOException
    {
	
	// 获取Object，返回结果为BosObject对象
	BosObject object = client.getObject(bucketName, objectKey);
	
	// 获取ObjectMeta
	ObjectMetadata meta = object.getObjectMetadata();
	
	// 获取Object的输入流
	InputStream objectContent = object.getObjectContent();
	
	// 处理Object
	// ...
	
	// 关闭流
	objectContent.close();
    }
    
    /**
     * 简单的读取Object
     * 
     * @param client
     * @param bucketName
     * @param objectKey
     * @throws IOException
     */
    public void getObject2(BosClient client, String bucketName, String objectKey)
    {
	// 新建GetObjectRequest
	GetObjectRequest getObjectRequest = new GetObjectRequest(bucketName, objectKey);
	
	// 获取0~100字节范围内的数据
	getObjectRequest.setRange(0, 100);
	
	// 获取Object，返回结果为BosObject对象
	BosObject object = client.getObject(getObjectRequest);
	
    }
    
    /**
     * 下载Object到文件
     * 
     * @param client
     * @param bucketName
     * @param objectKey
     */
    public void downloadObjectToFile(BosClient client, String bucketName, String objectKey)
    {
	// 新建GetObjectRequest
	GetObjectRequest getObjectRequest = new GetObjectRequest(bucketName, objectKey);
	
	// 下载Object到文件
	ObjectMetadata objectMetadata = client.getObject(getObjectRequest, new File("/path/to/file"));
    }
    
    /**
     * 只获取ObjectMetadata
     * 
     * @param client
     * @param bucketName
     * @param objectKey
     */
    public ObjectMetadata getObjectMetadata(BosClient client, String bucketName, String objectKey)
    {
	ObjectMetadata objectMetadata = client.getObjectMetadata(bucketName, objectKey);
	return objectMetadata;
    }
    
    /**
     * 获取下载Object的URL
     * 
     * @param client
     * @param bucketName
     * @param objectKey
     * @param expirationInSeconds
     * @return
     */
    public String generatePresignedUrl(BosClient client, String bucketName, String objectKey, int expirationInSeconds)
    {
	
	URL url = client.generatePresignedUrl(bucketName, objectKey, expirationInSeconds);
	return url.toString();
    }
    
    /**
     * 删除Object
     * 
     * @param client
     * @param bucketName
     * @param objectKey
     */
    public void deleteObject(BosClient client, String bucketName, String objectKey)
    {
	
	// 删除Object
	client.deleteObject(bucketName, objectKey);
    }
    
    /**
     * 拷贝Object
     * 
     * @param client
     * @param srcBucketName
     * @param srcKey
     * @param destBucketName
     * @param destKey
     */
    public void copyObject(BosClient client, String srcBucketName, String srcKey, String destBucketName, String destKey)
    {
	
	// 拷贝Object
	CopyObjectResponse copyObjectResponse = client.copyObject(srcBucketName, srcKey, destBucketName, destKey);
	
	// 打印结果
	System.out.println("ETag: " + copyObjectResponse.getETag() + " LastModified: " + copyObjectResponse.getLastModified());
    }
    
    /**
     * 拷贝Object
     * 
     * @param client
     * @param srcBucketName
     * @param srcKey
     * @param destBucketName
     * @param destKey
     */
    public void copyObject2(BosClient client, String srcBucketName, String srcKey, String destBucketName, String destKey)
    {
	// 创建CopyObjectRequest对象
	CopyObjectRequest copyObjectRequest = new CopyObjectRequest(srcBucketName, srcKey, destBucketName, destKey);
	
	// 设置新的Metadata
	Map<String, String> userMetadata = new HashMap<String, String>();
	
	userMetadata.put("<user-meta-key>", "<user-meta-value>");
	
	ObjectMetadata meta = getObjectMetadata(client, destBucketName, destKey);
	
	meta.setUserMetadata(userMetadata);
	
	copyObjectRequest.setNewObjectMetadata(meta);
	
	// 复制Object
	CopyObjectResponse copyObjectResponse = client.copyObject(copyObjectRequest);
	
	System.out.println("ETag: " + copyObjectResponse.getETag() + " LastModified: " + copyObjectResponse.getLastModified());
    }
}
```







### BosClient

```java
package com.boonya.bos.samle;

import java.io.IOException;
import com.baidubce.auth.DefaultBceCredentials;
import com.baidubce.services.bos.BosClient;
import com.baidubce.services.bos.BosClientConfiguration;
import com.boonya.bos.bucket.Bucket;
import com.boonya.bos.object.Object;

public class Sample
{
    public static void main(String[] args)
    {
	
	/**
	 * ACCESS_KEY_ID对应控制台中的“Access Key ID”
	 */
	String ACCESS_KEY_ID = "02b45c338869400a869aecbe3";
	
	/**
	 * SECRET_ACCESS_KEY对应控制台中的“Access Key Secret”
	 */
	String SECRET_ACCESS_KEY = "4abf9389c29c4b36aaa9241d8";
	
	// 初始化一个BosClient
	BosClientConfiguration config = new BosClientConfiguration();
	config.setCredentials(new DefaultBceCredentials(ACCESS_KEY_ID, SECRET_ACCESS_KEY));
	
	//上面的方式使用默认域名作为BOS的服务地址，如果用户需要自己制定域名，可以通过传入ENDPOINT参数来指定
	/**
	 * 注意：ENDPOINT参数只能用指定的包含Region的域名来进行定义，目前BOS只提供北京一个Region，因此ENDPOINT支持主域名http://bj.bcebos.com和备域名http://bj.baidubos.com，随着Region的增加将会开放其他可以支持的域名。
	 */
	String ENDPOINT = "http://bj.bcebos.com";
	config.setEndpoint(ENDPOINT);
	
	BosClient client = new BosClient(config);
	
	Bucket bucket=new Bucket();
	
	String bucketName="boonyabucket";
	
	if(!bucket.doesBucketExist(client, bucketName))
	{
	    bucket.createBucket(client, bucketName);
	}
	
	
	Object obj=new Object();
	
	obj.putObject(client, bucketName, "boonyakey","hello,bos!");
	
	try
	{
	    obj.getObject(client, bucketName, "boonyakey");
	} catch (IOException e)
	{
	    System.out.println("BOS获取对象，异常："+e.getMessage());
	}
    }
}
```



### 结果



![](https://img-blog.csdn.net/20150602155906875?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvYm9vbnlh/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)

![](https://img-blog.csdn.net/20150602160222174?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvYm9vbnlh/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)


**Bucket感觉就是数据库表（或者说是存储区域），而Object就是数据，跟非关系数据库的存储方式很像，但BOS是一个开放的存储对象平台或者是公用数据库存储集群。**




