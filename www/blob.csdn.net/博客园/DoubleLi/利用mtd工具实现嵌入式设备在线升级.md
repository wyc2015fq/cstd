# 利用mtd工具实现嵌入式设备在线升级 - DoubleLi - 博客园







版权声明：本文为博主原创文章，未经博主允许不得转载。





目录[(?)](http://blog.csdn.net/gl1987807/article/details/21096023#)[[+]](http://blog.csdn.net/gl1987807/article/details/21096023#)





主要思路是:通过web post方式将升级文件交给CGI程序处理,然后通过mtd工具实现设备在线升级.

## 1.页面部分

<form target="_blank" enctype="multipart/form-data" method="POST" action="./**update.cgi**">

<p>选择升级文件:    

<input type="file" value=""  name="file">

<input type="submit" value="确认" name="savebasesetting">

</p>

</form>



这部分主要是将待升级的文件上传给服务器(交给update.cgi)处理.

注意表单form属性:

1必需的 action 属性规定当提交表单时，向何处发送表单数据。

2 enctype属性规定在发送到服务器之前应该如何对表单数据进行编码。
|**值**|**描述**|
|----|----|
|application/x-www-form-urlencoded|在发送前编码所有字符（默认）|
|multipart/form-data|不对字符编码。|
|text/plain|空格转换为 "+" 加号，但不对特殊字符编码。|

3 method属性规定如何发送表单数据（表单数据发送到 action 属性所规定的页面）。

表单数据可以作为 URL 变量（method="get"）或者 HTTPpost （method="post"）的方式来发送。

**method属性**

浏览器使用method 属性设置的方法将表单中的数据传送给服务器进行处理。共有两种方法：POST 方法和 GET 方法。

如果采用 POST 方法，浏览器将会按照下面两步来发送数据。首先，浏览器将与 action 属性中指定的表单处理服务器建立联系，一旦建立连接之后，浏览器就会按分段传输的方法将数据发送给服务器。

在服务器端，一旦 POST 样式的应用程序开始执行时，就应该从一个标志位置读取参数，而一旦读到参数，在应用程序能够使用这些表单值以前，必须对这些参数进行解码。用户特定的服务器会明确指定应用程序应该如何接受这些参数。

另一种情况是采用 GET 方法，这时浏览器会与表单处理服务器建立连接，然后直接在一个传输步骤中发送所有的表单数据：浏览器会将数据直接附在表单的action URL 之后。这两者之间用问号进行分隔。

一般浏览器通过上述任何一种方法都可以传输表单信息，而有些服务器只接受其中一种方法提供的数据。可以在<form> 标签的 method （方法）属性中指明表单处理服务器要用方法来处理数据，使 POST 还是 GET。

**POST还是 GET？**

如果表单处理服务器既支持 POST 方法又支持 GET 方法，那么你该选择哪种方法呢？下面是有关这方面的一些规律：

·       如果希望获得最佳表单传输性能，可以采用 GET 方法发送只有少数简短字段的小表单。

·       一些服务器操作系统在处理可以立即传递给应用程序的命令行参数时，会限制其数目和长度，在这种情况下，对那些有许多字段或是很长的文本域的表单来说，就应该采用 POST 方法来发送。

·       如果你在编写服务器端的表单处理应用程序方面经验不足，应该选择 GET 方法。如果采用 POST 方法，就要在读取和解码方法做些额外的工作，也许这并不很难，但是也许你不太愿意去处理这些问题。

·       如果安全性是个问题，那么我们建议选用 POST 方法。GET 方法将表单参数直接放在应用程序的 URL 中，这样网络窥探者可以很轻松地捕获它们，还可以从服务器的日志文件中进行摘录。如果参数中包含了信用卡帐号这样的敏感信息，就会在不知不觉中危及用户的安全。而 POST 应用程序就没有安全方面的漏洞，在将参数作为单独的事务传输给服务器进行处理时，至少还可以采用加密的方法。

·       如果想在表单之外调用服务器端的应用程序，而且包括向其传递参数的过程，就要采用 GET 方法，因为该方法允许把表单这样的参数包括进来作为 URL 的一部分。而另一方面，使用 POST 样式的应用程序却希望在 URL 后还能有一个来自浏览器额外的传输过程，其中传输的内容不能作为传统<a> 标签的内容。

## 2 mtd工具介绍:

mtd-util，即mtd的utilities，是mtd相关的很多工具的总称，包括常用的mtdinfo,flash_erase, flash_eraseall,nanddump, nandwrite等，每一个工具，基本上都对应着一个同文件名的C文件。

mtd-util，由mtd官方维护更新，开发这一套工具，目的是为了Linux的MTD层提供一系列工具，方便管理维护mtd分区。mtd工具对应的源码，叫做mtd-utils，随着时间更新，发布了很多版本。

Mtd 工具源码下载: [http://git.infradead.org/](http://git.infradead.org/),

Mtd: 官方网站: [http://git.infradead.org/](http://git.infradead.org/),

## 3 升级前准备:

对flash 进行分区规划,通过uboot将信息传递给kernel:

setenv bootargs 'mem=64Mconsole=ttyAMA0,115200 root=/dev/mtdblock3 rootfstype=cramfsmtdparts=hi_sfc:512k(boot),512k(bootargs),3M(kernel),13M(rootfs),13M(app),2M(config)'





分区的目前是便于管理,在升级时可以单独升级rootfs,或者只升级App.等.



## 4.升级代码

升级代码主要分为4个部分.

### 1主要是获取上传文件的信息,如文件名,文件长度. 函数GetFileInfo()

### 2 主要是将上传的文件保存至内存中.函数: SaveFileToLocal

### 3 主要是对上传的文件进行crc校验: TestCrc,针对的是cramfs文件系统

**4利用mtd工具进行升级:**

**源文件:**

## 5升级注意事项:

1 由于用的是boa web服务器,上传时临时文件会用到目录/tmp,所以必须保证这个目录是可以读写的.由于cramfs是只读文件系统.所以可以采用把/tmp设置为tmpfs文件系统.(相当于内存).

设置/etc/fstab文件

tmpfs           /tmp            tmpfs   size=16m



2 重新分区后,每次启动需要挂载,所以可以在rcS文件中添加

mount -t cramfs  /dev/mtdblock4/opt

mount -t jffs2  /dev/mtdblock5/mnt/nand



3升级时间有时会过长,导致web页面无法正常返回.可以修改boa.conf,及defines.h

#define REQUEST_TIMEOUT                        60










