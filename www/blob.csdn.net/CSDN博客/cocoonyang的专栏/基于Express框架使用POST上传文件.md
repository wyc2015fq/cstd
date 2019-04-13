
# 基于Express框架使用POST上传文件 - cocoonyang的专栏 - CSDN博客


2016年08月25日 09:11:44[cocoonyang](https://me.csdn.net/cocoonyang)阅读数：1871


What
"The Hypertext Transfer Protocol (HTTP) is an application-level  protocol for distributed, collaborative, hypermedia information  systems."[10]
"HTTP is a stateless request/response protocol that operates by exchanging messages across a reliable transport- or session-layer 'connection' "[10]

"A HTTP client is a program that extabilishes a connection to a server for purpose of sending one or more HTTP requests."[10]

"A HTTP server is a program that accepts connections in order to service HTTP requests by sending HTTP responses." [10]

"The terms 'client' and 'server' refer only to roles that these programs perform for a particular connection" [10]

How

"Most HTTP Communication consists of a retrieval request  (GET) for a representation of some resource identified by a URI."
"HTTP relies upon the Uniform Resource Identifier (URI) standard to indicate the target resource and relationships between resources." [10]
"Messages are passed in a format similar to that used by Internet mail and the Multipurpose Internet Mail Extensions (MIME)"
"The Hypertext Transfer Protocol (HTTP) is an application-level  protocol for distributed, collaborative, hypermedia information  systems."[10]
"The HTTP protocol is a request/response protocol. A client sends a request to the server in the form of a request method, URI, and  protocol version, followed by a MIME-like message containing request  modifiers, client information, and possible body content
 over a  connection with a server. The server responds with a status line,  including the message's protocol version and a success or error code, followed by a MIME-like message containing server information, entity  metainformation, and possible entity-body
 content."[10]
"A client sends an HTTP request to a server in the form of a request message, beginning with a request-line that includes a method, URI, and protocol version, followed b header fields containning request modifiers, client information, and reqpresentation metata,
 an empty line to indicate the end of the header section, and finally a message body containing the payload body." [10]

"A server responds to a client’s request by sending one or more HTTP response messages, each beginning with a status line that includes the protocol version, a success or error code, and textual reason phrase , possibly followed by header fields containing
 server information, resource metadata, and representation metadata, an empty line to indicate the end of the header section, and finally a message body containing the payload body. " [10]
An Example[10]:
Client request:
GET /hello.txt HTTP/1.1
 User-Agent: curl/7.16.3 libcurl/7.16.3 OpenSSL/0.9.7l zlib/1.2.3
 Host: www.example.com
 Accept-Language: en, miServer response:HTTP/1.1 200 OK
 Date: Mon, 27 Jul 2009 12:28:53 GMT
 Server: Apache
 Last-Modified: Wed, 22 Jul 2009 19:15:56 GMT
 ETag: "34aa387-d-1568eb00"
 Accept-Ranges: bytes
 Content-Length: 51
 Vary: Accept-Encoding
 Content-Type: text/plain
 Hello World! My payload includes a trailing CRLF.
Request
"A request message from a client to a server includes, within the  first line of that message, the method to be applied to the resource,  the identifier of the resource, and the protocol version in use."[10]

```python
Request       = Request-Line              ; Section 5.1
                        *(( general-header        ; Section 4.5
                         | request-header         ; Section 5.3
                         | entity-header ) CRLF)  ; Section 7.1
                        CRLF
                        [ message-body ]          ; Section 4.3
```
Request-Line(请求行)[10]
"The Request-Line begins with a method token, followed by the Request-URI and the protocol version, and ending with CRLF. The elements are separated by SP characters. No CR or LF is allowed except in the final CRLF sequence."[10]
```python
Request-Line   = Method SP Request-URI SP HTTP-Version CRLF
```
请求行中的Method[10]
The Method  token indicates the method to be performed on the resource identified by the Request-URI. The method is case-sensitive.
```python
Method         = "OPTIONS"                ; Section 9.2
                      | "GET"                    ; Section 9.3
                      | "HEAD"                   ; Section 9.4
                      | "POST"                   ; Section 9.5
                      | "PUT"                    ; Section 9.6
                      | "DELETE"                 ; Section 9.7
                      | "TRACE"                  ; Section 9.8
                      | "CONNECT"                ; Section 9.9
                      | extension-method
       extension-method = token
```

POST[10]
The POST method is used to request that the origin server accept the entity enclosed in the request as a new subordinate of the resource identified by the Request-URI in the Request-Line. POST is designed to allow a uniform method to cover the following
 functions:
```python
- Annotation of existing resources;
      - Posting a message to a bulletin board, newsgroup, mailing list,
        or similar group of articles;
      - Providing a block of data, such as the result of submitting a
        form, to a data-handling process;
      - Extending a database through an append operation.
```
The actual function performed by the POST method is determined by the server and is usually dependent on the Request-URI. The posted entity  is subordinate to that URI in the same way that a file is subordinate  to a directory containing
 it, a news article is subordinate to a  newsgroup to which it is posted, or a record is subordinate to a
database."[10]
" The action performed by the POST method might not result in a resource that can be identified by a URI. In this case, either 200 (OK) or 204 (No Content) is the appropriate response status, depending on whether or not the response includes an entity that 
 describes the result."[10]
"If a resource has been created on the origin server, the response  SHOULD be 201 (Created) and contain an entity which describes the status of the request and refers to the new resource, and a Location  header (see section 14.30)."[10]
"Responses to this method are not cacheable, unless the response  includes appropriate Cache-Control or Expires header fields. However,  the 303 (See Other) response can be used to direct the user agent to  retrieve a cacheable resource."[10]



Form的 Html代码[9]

```python
<p> 
<form  method="post" id="upload" action="/fileupload" enctype="multipart/form-data">  
        <input type="file" name="uploader"/>  
        <input type="submit" value="upload"/>  
   </form> 
</p>
```
[3] When a user clicks the submit button on Web form with this tag, the Web browser should collect data from all input fields and submit to the specified URL. The browser should also follow the following rules:
If "get" is specified as the method, data will be submitted as part of the GET header line of the HTTP request.
If "post" is specified as the method, data will be submitted as the body of the HTTP request.
If "application/x-www-form-urlencoded" is specified as the encryption type, names and values of all input fields will be encoded together based on the URL encoding specification.
If no encryption type is specified, "application/x-www-form-urlencoded" will be used as the default encryption type.
If "multipart/form-data" is specified as the encryption type, data will be submitted in multiple parts with one part for one input field. Parts are separated by a boundary identification string, which should be given as the "boundary" attribute of the "Content-type"
 header line if the HTTP request.
If "application/x-www-form-urlencoded" is specified as the encryption type, only the file name of a FILE type input field will be submitted as the value of the field. The content of the file will not be submitted.
If "multipart/form-data" is specified as the encryption type, only the file name of a FILE type input field will be submitted as the value of the field. The content of the file will not be submitted.
When "multipart/form-data" is specified as the encryption type, the format of the HTTP request body should look like this:

```python
--boundary_identification_string
input_part_1
--boundary_identification_string
input_part_2
--boundary_identification_string
input_part_3
......
--boundary_identification_string--
```
样例[5]：

```python
POST http://www.baidu.com/ HTTP/1.1
Host: www.baidu.com
Content-Length: 495
Content-Type: multipart/form-data; boundary=---------------------------7db2d1bcc50e6e
-----------------------------7db2d1bcc50e6e
Content-Disposition: form-data; name="myText"
hello world
-----------------------------7db2d1bcc50e6e
Content-Disposition: form-data; name="upload1"; filename="C:\file1.txt"
Content-Type: text/plain
This is file1.
-----------------------------7db2d1bcc50e6e
Content-Disposition: form-data; name="upload2"; filename="C:\file2.txt"
Content-Type: text/plain
This is file2, it's longer.
-----------------------------7db2d1bcc50e6e--
```
[5]很显然它们两个选择了不同的数据“模式”作为边界——事实上，浏览器提交两次数据时，使用的边界也可能不会相同，这都没有问题。
选择了边界之后，便会将它放在头部的Content-Type里传递给服务器端，实际需要传递的数据便可以分割为“段”，每段便是“一项”数据。从上面的内容中大家应该都能看出数据传输的规范，因此便不做细谈了。只强调几点：
数据均无需额外编码，直接传递即可，例如您可以看出上面的示例中的“空格”均没有变成加号。至于这里您可以看到清晰地文字内容，是因为我们上传了仅仅包含可视ASCII码的文本文件，如果您上传一个普通的文件，例如图片，捕获到的数据则几乎完全不可读了。
IE和Chrome在filename的选择策略上有所不同，前者是文件的完整路径，而后者则仅仅是文件名。
数据内容以两条横线结尾，并同样以一个换行结束。在网络协议中一般都以连续的CR、LF（即\r、\n，或0x0D、Ox0A）字符作为换行，这与Windows的标准一致。如果您使用其他操作系统，则需要考虑它们的换行符。

处理POST请求的脚本代码

```python
//
var busboy = require('connect-busboy');
// ... 
app.use(busboy());  
// ... 
app.post('/fileupload', function(req, res) {
    var fstream;
    req.pipe(req.busboy);
    req.busboy.on('file', function (fieldname, file, filename) {
        console.log("Uploading: " + filename); 
        var fstream;
        fstream = fs.createWriteStream(__dirname + '/uploads/' + filename);
        fstream.on('error', function(err) {
            console.log(String( err ) );
            file.unpipe();
            fstream.end();
            res.send(filename  + ' uploadding failed.'); 
        });
             
        file.pipe(fstream);
        fstream.on('close', function () {
            res.send(filename  + ' uploadding success.'); 
            // res.redirect('back');  
        });
    });
});
```

浏览器通过form 表单来上传文件时使用的是"RFC 1867 - Form-based File Upload in HTML"协议[4]。使用的数据包格式是：

```python
POSThttp://www.foo.com/ HTTP/1.1
Host: www.foo.com
Content-Length: 199
Content-Type: multipart/form-data; boundary=----WebKitFormBoundarywr3X7sXBYQQ4ZF5G
------WebKitFormBoundarywr3X7sXBYQQ4ZF5G
Content-Disposition: form-data; name="myfile"; filename="upload.txt"
Content-Type: text/plain
hello world
------WebKitFormBoundarywr3X7sXBYQQ4ZF5G--
```
使用Node.js生成上述格式的数据包，即可实现文件上传。使用异步API样例代码如下[1]：

```python
const http = require('http');
const fs = require('fs');

//生成分隔数据
var boundaryKey = '----WebKitFormBoundaryjLVkbqXtIi0YGpaB'; 
var currentDir = __dirname + '/';
var sourceFileName = 'abc.txt'
var filePath = currentDir + sourceFileName;
// checking file
try {  
  stat = fs.statSync(filePath);  
  console.log("File exists.");  
}  
catch (e) {  
    if (e.code == 'ENOENT') {    
      console.log("File does not exist.");  
      //return false;  
    }  
  console.log("Exception fs.statSync (" + filePath + "): " + e);  
  // console.log(filePath + " does not exist.");  
}

// 
var options = {
    hostname: 'localhost',
    port: 80,
    path: '/fileupload',
    method: 'POST'
}

//读取需要上传的文件内容
fs.readFile( filePath, function (err, data) {
    //拼装分隔数据段
    var payload = '--' + boundaryKey + '\r\n' ;
    payload += 'Content-Disposition:form-data; name="myfile"; filename="'+ sourceFileName + '"\r\n' ;
    payload += 'Content-Type:text/plain\r\n\r\n';
    payload += data;
    payload += '\r\n--' + boundaryKey + '--';
    //发送请求
    var req = http.request(options, function (res) {
        res.setEncoding('utf8');
        res.on('data', function (chunk) {
            console.log('body:' + chunk);
        });
    });
    req.on('error', function(e) {
        console.error("error:"+e);
        });
    //把boundary、要发送的数据大小以及数据本身写进请求
    req.setHeader('Content-Type', 'multipart/form-data; boundary='+boundaryKey+'');
    req.setHeader('Content-Length', Buffer.byteLength(payload, 'utf8'));
    req.write(payload);
    req.end();
});
```

使用同步API样例代码如下：
```python
const http = require('http');
const fs = require('fs');

//生成分隔数据
var boundaryKey = '----WebKitFormBdaendyjLVkbqXtIi0YGpaB'; 
var currentDir = __dirname + '/';
var sourceFileName = 'abc.txt'
var filePath = currentDir + sourceFileName;
// checking file
try {  
  stat = fs.statSync(filePath);  
  console.log("File exists.");  
}  
catch (e) {  
    if (e.code == 'ENOENT') {    
      console.log("File does not exist.");  
      //return false;  
    }  
  console.log("Exception fs.statSync (" + filePath + "): " + e);  
  // console.log(filePath + " does not exist.");  
}

// 
var options = {
    hostname: 'localhost',
    port: 80,
    path: '/fileupload',
    method: 'POST'
}
var fileData = fs.readFileSync(  filePath, "utf8");  
var payload = '--' + boundaryKey + '\r\n' ;
    payload += 'Content-Disposition:form-data; name="myfile"; filename="'+ sourceFileName + '"\r\n' ;
    payload += 'Content-Type:text/plain\r\n\r\n';
    payload += fileData;
    payload += '\r\n--' + boundaryKey + '--';
    
//发送请求
var req = http.request(options, function (res) {
        res.setEncoding('utf8');
        res.on('data', function (chunk) {
            console.log('body:' + chunk);
        });
});
req.on('error', function(e) {
        console.error("error:"+e);
});
//把boundary、要发送的数据大小以及数据本身写进请求
req.setHeader('Content-Type', 'multipart/form-data; boundary='+boundaryKey+'');
req.setHeader('Content-Length', Buffer.byteLength(payload, 'utf8'));
req.write(payload);
req.end();
```

nodejs的发送上传文件代码[6]

```python
// modules 
//
const http = require('http');
var path = require('path');
const fs = require('fs');
 
var option = {
    host: "127.0.0.1",
    port: "80",
    path: "/fileupload",
    method: 'post'
};
// 
var boundaryKey = '----WebKitFormBoundaryjLVk4bqXtIi0YGpaB'; 
var formFieldName = "files";
var uploadFilePath = "./abc.txt"; 
var filename = path.basename(uploadFilePath);

// checking file  
try {    
  stat = fs.statSync( uploadFilePath );    
  console.log("File exists.");    
}    
catch (e) {    
    if (e.code == 'ENOENT') {      
      console.log("File does not exist.");    
      //return false;    
    }    
  console.log("Exception fs.statSync (" + uploadFilePath + "): " + e);      
}  

var part = [];
    part.push('--' + boundaryKey); 
    part.push('Content-Type: application/octet-stream');
    part.push('Content-Disposition: form-data; name="' + formFieldName + '"; filename="' + filename + '"');
    part.push('Content-Transfer-Encoding: binary');
    part.push('\r\n');

var request = http.request( option, function(res) {
    // show results
    console.log('STATUS: ' + res.statusCode);
    //console.log('HEADERS: ' + JSON.stringify(res.headers));
    //console.log('Date: ' + JSON.stringify(res.headers)[2] );
    res.setEncoding('utf8');
    res.on('data', function(chunk) {
        console.log('BODY: ' + chunk);
    });
    res.on('end', function(err) {
        console.log("Uploading " + uploadFilePath + ' complete.');
    });
});
    request.on("error", function(e) {
        console.log('upload Error: ' + e.message);
    })
    request.setHeader('Content-Type', 'multipart/form-data; boundary="' + boundaryKey + '"');
    request.write( part.join('\r\n') );

//
//
var readStream = fs.createReadStream(uploadFilePath, {
        bufferSize: 8 * 1024
    });
    readStream.on('end', function(err) {
        if (err) {
            console.error(err);
            return;
        }
        request.write('\r\n--' + boundaryKey + '--');
        request.end();
        // Logging 
        console.log("Sending " + uploadFilePath + ' content complete.');
    });
    readStream.pipe( request, {
        end: true //readStream  
    });
```



参考文献
[1] http://www.aichengxu.com/javascript/24609788.htm
[2] http://stackoverflow.com/questions/5744990/how-to-upload-a-file-from-node-js
[3] http://www.herongyang.com/JSP/File-Upload-RFC-1867-Form-based-File-Upload.html
[4] https://www.w3.org/TR/device-upload
[5] http://kb.cnblogs.com/page/95545/
[6] https://hzxiaosheng.bitbucket.io/work/2014/03/09/download-and-upload-file-with-nodejs.html
[9] http://www.faqs.org/rfcs/rfc1867.html
[10] https://www.rfc-editor.org/rfc/pdfrfc/rfc7230.txt.pdf
http://blog.csdn.net/puncha/article/details/9015317
http://liuxufei.com/blog/jishu/798.html
"The Hypertext Transfer Protocol (HTTP) is an application-level  protocol for distributed, collaborative, hypermedia information  systems."[10]

