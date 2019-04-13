
# Node.js 中 fs 模块使用样例 -- 文件读写 - cocoonyang的专栏 - CSDN博客


2016年09月12日 10:47:51[cocoonyang](https://me.csdn.net/cocoonyang)阅读数：2685


Node.js文件 I/O 是由简单封装的标准 POSIX 函数提供的[1]。 fs(File System)是Node.js的内置模块，可直接通过
```python
const fs = require('fs');
```
使用该模块。  fs 模块所有的API都有异步和同步的形式。异步形式始终以完成回调作为它最后一个参数。 传给完成回调的参数取决于具体方法，但第一个参数总是留给异常。 如果操作成功完成，则第一个参数会是 null 或 undefined。当使用同步形式时，任何异常都会被立即抛出。 可以使用 try/catch 来处理异常，或让它们往上冒泡。使用异步API无法保证函数调用顺序. 下列代码中使用fs.rename更改一文件名称，然后使用fs.stat检验改名操作结果。如果使用异步API，fs.stat可能会在调用fs.rename前执行[1].
```python
fs.rename('/tmp/hello', '/tmp/world', function(err){
  if (err) throw err;
  console.log('renamed complete');
});
fs.stat('/tmp/world', function(err, stats){
  if (err) throw err;
  console.log(`stats: ${JSON.stringify(stats)}`);
});
```
正确的做法是使用一系列回调函数来保证API调用顺序
```python
fs.rename('/tmp/hello', '/tmp/world', function(err){
  if (err) throw err;
  fs.stat('/tmp/world', function(err, stats){
    if (err) throw err;
    console.log(`stats: ${JSON.stringify(stats)}`);
  });
});
```

## 文件读写
使用readFile读文件样例[2]
```python
var fs = require('fs'); 
var readFileName = './foo.txt';  
var writeFileName = './target.txt';  
/** Using the readFile API - Asynchronous */
fs.readFile( readFileName, "utf8", function(err, data){
  if ( err ){ throw err;}
  console.log("Reading file asynchronously");
  console.log(data);
});
```
使用readFileSync读文件样例[2]
```python
var fs = require('fs'); 
var readFileName = './foo.txt';  
var writeFileName = './target.txt';  
/** Using the readFile API - Asynchronous */
console.log("Reading file synchronously");
var fileData = fs.readFileSync(  readFileName, "utf8");
console.log(fileData);
```
使用ReadStream读文件样例[2]
```python
var fs = require('fs'); 
var readFileName = './foo.txt';  
var writeFileName = './target.txt';  
/** Reading file using ReadStream API */
//Creating a stream out of the file
var readStreamObject = fs.createReadStream( readFileName, { flags: 'r',
  encoding:"utf8",
  fd: null,
  mode: 0666,
  autoClose: true
});
//Setting up event handlers on the stream object
//readable - this event is fired when data can be read from stream
readStreamObject.on('readable', function(){
  console.log("*** Reading from file using ReadStream");
});

//data - this event is fired when data is available to be read from stream
readStreamObject.on('data', function(data){
  console.log(data);
});
```
使用writeFileSync写文件样例[2]
```python
var fs = require('fs'); 
var readFileName = './foo.txt';  
var writeFileName = './target.txt';  
fs.writeFileSync( writeFileName, "Writing to a file synchronously from node.js", {"encoding":'utf8'});
console.log("*** File written successfully");
//Now reading the same file to confirm data written
fs.readFile(  writeFileName, "utf8", function(err, data){
  if ( err ){ throw err;}
  console.log("*** Reading just written file");
  console.log(data);
});
```
使用writeFile写文件样例[2]
```python
var fs = require('fs'); 
var readFileName = './foo.txt';  
var writeFileName = './target.txt';  
fs.writeFile(  writeFileName, "Writing to a file from node.js", {"encoding":'utf8'}, function(err){
  if ( err ) { throw err; }
  console.log("*** File written successfully");
  //Now reading the same file to confirm data written
  fs.readFile( writeFileName, "utf8", function(err, data){
    if ( err ){ throw err;}
    console.log("*** Reading just written file");
    console.log(data);
  });
});
```
多次对同一文件使用 fs.write 且不等待回调，是不安全的.推荐使用 fs.createWriteStream[1] 。
使用createWriteStream写文件样例[2]
```python
var fs = require('fs'); 
var readFileName = './foo.txt';  
var writeFileName = './target.txt';  

//Create a stream with the required path
var writeStreamObject = fs.createWriteStream( writeFileName );
//write to the stream using the API
writeStreamObject.write("Writing to a file using WriteStream", "utf8");
//Now read the same file to verify that the contents have been successfully written
fs.readFile( writeFileName, "utf8", function(err, data){
  if ( err ){ throw err;}
  console.log("*** Reading the recently written file");
  console.log(data);
});
```

## 文件复制
文件复制样例[3]
```python
var fs = require('fs'),
    path = require('path'),
    out = process.stdout;
var stat
var filePath = './foo.mkv';
var readStream = fs.createReadStream(filePath);
var writeStream = fs.createWriteStream('file.mkv');
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
var totalSize = stat.size;
var passedLength = 0;
var lastSize = 0;
var startTime = Date.now();
readStream.on('data', function(chunk) {
    passedLength += chunk.length;
    if (writeStream.write(chunk) === false) {
        readStream.pause();
    }
});
readStream.on('end', function() {
    writeStream.end();
});
writeStream.on('drain', function() {
    readStream.resume();
});
setTimeout(function show() {
    var percent = Math.ceil((passedLength / totalSize) * 100);
    var size = Math.ceil(passedLength / 1000000);
    var diff = size - lastSize;
    lastSize = size;
    out.clearLine();
    out.cursorTo(0);
    out.write('已完成' + size + 'MB, ' + percent + '%, 速度：' + diff * 2 + 'MB/s');
    if (passedLength < totalSize) {
        setTimeout(show, 500);
    } else {
        var endTime = Date.now();
        console.log();
        console.log('共用时：' + (endTime - startTime) / 1000 + '秒。');
    }
}, 500);
```

### Synchronous Copy
代码样例[7]
```python
copyFileSync = (srcFile, destFile) 
function copyFileSync(srcFile, destFile) {
  var BUF_LENGTH = 64*1024  
  var buff = new Buffer(BUF_LENGTH)  
  var fdr = fs.openSync(srcFile, 'r')  
  var fdw = fs.openSync(destFile, 'w') 
  var bytesRead = 1  
  var pos = 0  
  while( bytesRead > 0 )
  { 
    bytesRead = fs.readSync(fdr, buff, 0, BUF_LENGTH, pos)  
    fs.writeSync(fdw,buff,0,bytesRead)  
    pos += bytesRead  
  }
  fs.closeSync(fdr);  
  fs.closeSync(fdw);  
}
```
Results of testing influences of the buffer size for time consumption shows that there is a lower limitation of the parameter. For one file about 300 MB, the coping processes with various buffer sizes, which over 2 KB, take roughly similar time. If the buffer is lower than 2 KB, however, time computation would increase significantly.

### Asynchronous Copy
stream version in one line [8]
```python
var fs = require('fs');  
  
fs.createReadStream('test.log').pipe(fs.createWriteStream('newLog.log'));
```
stream version with errors handling provided by Mike Schilling [8]
```python
function copyFile(source, target, cb) {  
  var cbCalled = false;  
  
  var rd = fs.createReadStream(source);  
  rd.on("error", function(err) {  
    done(err);  
  });  
  var wr = fs.createWriteStream(target);  
  wr.on("error", function(err) {  
    done(err);  
  });  
  wr.on("close", function(ex) {  
    done();  
  });  
  rd.pipe(wr);  
  
  function done(err) {  
    if (!cbCalled) {  
      cb(err);  
      cbCalled = true;  
    }  
  }  
}
```
stream version with errors handling presented by Jens Hauke [8]
```python
function copyFile(source, target, cb) {  
  var cbCalled = false;  
  
  var rd = fs.createReadStream(source);  
  rd.on("error", done);  
  
  var wr = fs.createWriteStream(target);  
  wr.on("error", done);  
  wr.on("close", function(ex) {  
    done();  
  });  
  rd.pipe(wr);  
  
  function done(err) {  
    if (!cbCalled) {  
      cb(err);  
      cbCalled = true;  
    }  
  }  
}
```

## 大文件读写
大多数情况下读文件的速度总比写文件的速度快，这样便导致大量的数据被积压在内存中，当要读取的文件很大时，甚至会导致因占用内存太多而导致整个 Node.js 进程崩溃。
当读取速度超出我们期望的值时，可以执行pause()先暂停，待时机符合时再执行resume()重新开始[5]。
```python
var util = require('util');
var events = require('events');
var fs = require('fs');
// 一个几百 M 的文本文件
var inputFile = '/Volumes/foo.txt';

function ReadStreamThrottle (stream, speed) {
  this._stream = stream;
  this._readBytes = 0;
  this._speed = speed;
  this._ended = false;
  this._readBytesSecond = 0;
  this._lastTimestamp = Date.now();
  this._paused = false;
  var self = this;
  // 检查速度是否太快
  function isTooFast () {
    var t = (Date.now() - self._lastTimestamp) / 1000;
    var bps = self._readBytesSecond / t;
    return bps > speed;
  }
  // 每隔一段时间检查速度
  function checkSpeed () {
    if (isTooFast()) {
      self.pause();
      // 直到平均速度放缓到预设的值时继续读流
      var tid = setInterval(function () {
        if (!isTooFast()) {
          clearInterval(tid);
          self.resume();
        }
      }, 100);
    } else {
      self.resume();
    }
  }
  stream.on('data', function (chunk) {
    self._readBytes += chunk.length;
    self._readBytesSecond += chunk.length;
    self.emit('data', chunk);
    checkSpeed();
  });
  stream.on('end', function () {
    self._ended = true;
    self.emit('end');
  });
}
util.inherits(ReadStreamThrottle, events.EventEmitter);
ReadStreamThrottle.prototype.pause = function () {
  this._paused = true;
  this._stream.pause();
};
ReadStreamThrottle.prototype.resume = function () {
  this._paused = false;
  this._stream.resume();
};

// 读取文件，限制速度不大于 10MB/S
var MB = 1024 * 1024;
var s = new ReadStreamThrottle(fs.createReadStream(inputFile), MB * 10);
var bytes = 0;
var t = Date.now();
s.on('data', function (c) {
  bytes += c.length;
  var spent = (Date.now() - t) / 1000;
  console.log('read %s bytes, speed: %sMB/S', bytes, (bytes / MB / spent).toFixed(2));
});
s.on('end', function () {
  console.log('end. total %s bytes', bytes);
});
```

## 文件上传
根据"RFC 1867 - Form-based File Upload in HTML"协议，使用Node.js实现文件上传[9]
在浏览器上通过form 表单来上传文件时使用的数据包格式如下：
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
样例代码[9]
```python
const http = require('http');
const fs = require('fs');

//生成分隔数据
var boundaryKey = '----WebKitFormBoundaryjLVkbqXtIi0YGpaB'; 
var currentDir = __dirname + '/';
var sourceFileName = 'BMW5_torsion.bdf'
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

文件上传样例[4]
```python
var urlparse = require('url').parse
  , http = require('http')
  , fs = require('fs');
function upload(url, uploadfile, callback) {
    var urlinfo = urlparse(url);
    var options = {
        method: 'POST',
        host: urlinfo.host,
        path: urlinfo.pathname
    };
    if(urlinfo.port) {
        options.port = urlinfo.port;
    }
    if(urlinfo.search) {
        options.path += urlinfo.search;
    }
    var req = http.request(options, function(res) {
        var chunks = [], length = 0;
        res.on('data', function(chunk) {
            length += chunk.length;
            chunks.push(chunk);
        });
        res.on('end', function() {
            var buffer = new Buffer(length);
            // delay copy
            for(var i = 0, pos = 0, size = chunks.length; i < size; i++) {
                chunks[i].copy(buffer, pos);
                pos += chunks[i].length;
            }
            res.body = buffer;
            callback(null, res);
        });
    });
    var readstream = fs.createReadStream(uploadfile);
    readstream.on('data', function(chunk) {
        console.log('write', chunk.length);
        req.write(chunk);
    });
    readstream.on('end', function() {
        req.end();
    });
};
upload('http://weibo.com/', '/tmp/bigfile.pdf', function(err, res) {
    console.log(res.statusCode, res.headers);
});
```

## 文件下载
大文件下载样例[4]
```python
var urlparse = require('url').parse
var http = require('http')
var fs = require('fs'); 
function download(url, savefile, callback) {
    console.log('download', url, 'to', savefile)
    var urlinfo = urlparse(url);
    var options = {
        method: 'GET',
        host: urlinfo.host,
        path: urlinfo.pathname
    };
    if(urlinfo.port) {
        options.port = urlinfo.port;
    }
    if(urlinfo.search) {
        options.path += urlinfo.search;
    }
    var req = http.request(options, function(res) {
        var writestream = fs.createWriteStream(savefile);
        writestream.on('close', function() {
            callback(null, res);
        });
        res.pipe(writestream);
    });
    req.end();
};
download('http://web.mit.edu/cocosci/Papers/sci_reprint.pdf', '/temp/isomap.pdf', function(err, res) {
    console.log(res.statusCode, res.headers);
});
```

## 大文件的下载以及断点续传
大文件的下载以及断点续传基本原理是，在文件的下载断开以后。客户端继续向服务器端请求的时候，http请求的头文件中会多了一个参数“Range”，来标示当前下载的文件所断开的位置[6]。
```python
function Transfer(req, resp) {
	this.req = req;
	this.resp = resp;
}
/**
 * [@description](/user/description) 计算上次的断点信息
 * [@param](/user/param) {string} Range 请求http头文件中的断点信息，如果没有则为undefined，格式（range: bytes=232323-）
 * [@return](/user/return) {integer} startPos 开始的下载点
 */
Transfer.prototype._calStartPosition = function(Range) {
	var startPos = 0;
	if( typeof Range != 'undefined') {
		var startPosMatch = /^bytes=([0-9]+)-$/.exec(Range);
		startPos = Number(startPosMatch[1]);
	}
	return startPos;
}
/**
 * [@description](/user/description) 配置头文件
 * [@param](/user/param) {object} Config 头文件配置信息（包含了下载的起始位置和文件的大小）
 */
Transfer.prototype._configHeader = function(Config) {
	var startPos = Config.startPos, 
		fileSize = Config.fileSize,
		resp = this.resp;
	// 如果startPos为0，表示文件从0开始下载的，否则则表示是断点下载的。
	if(startPos == 0) {
		resp.setHeader('Accept-Range', 'bytes');
	} else {
		resp.setHeader('Content-Range', 'bytes ' + startPos + '-' + (fileSize - 1) + '/' + fileSize);
	}
	resp.writeHead(206, 'Partial Content', {
		'Content-Type' : 'application/octet-stream',
	});
}
/**
 * [@description](/user/description) 初始化配置信息
 * [@param](/user/param) {string} filePath
 * [@param](/user/param) {function} down 下载开始的回调函数
 */
Transfer.prototype._init = function(filePath, down) {
	var config = {};
	var self = this;
	fs.stat(filePath, function(error, state) {
		if(error)
			throw error;
		config.fileSize = state.size;
		var range = self.req.headers.range;
		config.startPos = self._calStartPosition(range);
		self.config = config;
		self._configHeader(config);
		down();
	});
}
/**
 * [@description](/user/description) 生成大文件文档流，并发送
 * [@param](/user/param) {string} filePath 文件地址
 */
Transfer.prototype.Download = function(filePath) {
	var self = this;
	path.exists(filePath, function(exist) {
		if(exist) {
			self._init(filePath, function() {
				var config = self.config
					resp = self.resp;
				fReadStream = fs.createReadStream(filePath, {
					encoding : 'binary',
					bufferSize : 1024 * 1024,
					start : config.startPos,
					end : config.fileSize
				});
				fReadStream.on('data', function(chunk) {
					resp.write(chunk, 'binary');
				});
				fReadStream.on('end', function() {
					resp.end();
				});
			});
		} else {
			console.log('文件不存在！');
			return;
		}
	});
}

var fs = require('fs')
http = require('http')
path = require('path');
var server = http.createServer(function(req, resp) {
	var transfer = new Transfer(req, resp);
	var filePath = '/Users/xukai/Downloads/love.rmvb';
	transfer.Download(filePath);
});
server.listen('8000');
```

## isFile()
Node.js 中 fs 模块的 isFile() 函数使用样例
```python
var fs = require('fs');
var path = '.\foo.txt';
// 同步版
fs.statSync( path ).isFile() ;

// 异步版
fs.stat( path, function(s){
    callback(s.isFile() );
});
```

参考文献
[1] http://nodejs.cn/api/fs
[2] http://javabeat.net/nodejs-read-write-file/
[3] https://segmentfault.com/a/1190000000519006
[4] http://www.cnblogs.com/fengmk2/archive/2011/08/16/2140460.html
[5] http://morning.work/page/2015-07/read_and_write_big_file_in_nodejs.html
[6] https://cnodejs.org/topic/4f5b47c42373009b5c04e9cb
[7] http://procbits.com/2011/11/15/synchronous-file-copy-in-node-js
[8] http://stackoverflow.com/questions/11293857/fastest-way-to-copy-file-in-node-js
[9] http://www.aichengxu.com/javascript/24609788.htm


