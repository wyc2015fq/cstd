# nodejs中流(stream)的理解 - z69183787的专栏 - CSDN博客
2016年05月20日 15:16:12[OkidoGreen](https://me.csdn.net/z69183787)阅读数：3271
nodejs的`fs`模块并没有提供一个`copy`的方法，但我们可以很容易的实现一个，比如：
```java
var source = fs.readFileSync('/path/to/source', {encoding: 'utf8'});
fs.writeFileSync('/path/to/dest', source);
```
这种方式是把文件内容全部读入内存，然后再写入文件，对于小型的文本文件，这没有多大问题，比如`grunt-file-copy`就是这样实现的。但是对于体积较大的二进制文件，比如音频、视频文件，动辄几个GB大小，如果使用这种方法，很容易使内存“爆仓”。理想的方法应该是读一部分，写一部分，不管文件有多大，只要时间允许，总会处理完成，这里就需要用到流的概念。
![](http://segmentfault.com/img/bVcla6)
如上面高大上的图片所示，我们把文件比作装水的桶，而水就是文件里的内容，我们用一根管子(pipe)连接两个桶使得水从一个桶流入另一个桶，这样就慢慢的实现了大文件的复制过程。
`Stream`在nodejs中是`EventEmitter`的实现，并且有多种实现形式，例如：
- http responses request
- fs read write streams
- zlib streams
- tcp sockets
- child process stdout and stderr
上面的文件复制可以简单实现一下：
```java
```java
var fs = require('fs');
var readStream = fs.createReadStream('/path/to/source');
var writeStream = fs.createWriteStream('/path/to/dest');
readStream.on('data', function(chunk) { // 当有数据流出时，写入数据
    writeStream.write(chunk);
});
readStream.on('end', function() { // 当没有数据时，关闭数据流
    writeStream.end();
});
```
```
上面的写法有一些问题，如果写入的速度跟不上读取的速度，有可能导致数据丢失。正常的情况应该是，写完一段，再读取下一段，如果没有写完的话，就让读取流先暂停，等写完再继续，于是代码可以修改为：
```java
```java
var fs = require('fs');
var readStream = fs.createReadStream('/path/to/source');
var writeStream = fs.createWriteStream('/path/to/dest');
readStream.on('data', function(chunk) { // 当有数据流出时，写入数据
    if (writeStream.write(chunk) === false) { // 如果没有写完，暂停读取流
        readStream.pause();
    }
});
writeStream.on('drain', function() { // 写完后，继续读取
    readStream.resume();
});
readStream.on('end', function() { // 当没有数据时，关闭数据流
    writeStream.end();
});
```
```
或者使用更直接的`pipe`
```
```java
// pipe自动调用了data,end等事件
fs.createReadStream('/path/to/source').pipe(fs.createWriteStream('/path/to/dest'));
```
```
下面是一个更加完整的复制文件的过程
```java
```java
var fs = require('fs'),
    path = require('path'),
    out = process.stdout;
var filePath = '/Users/chen/Movies/Game.of.Thrones.S04E07.1080p.HDTV.x264-BATV.mkv';
var readStream = fs.createReadStream(filePath);
var writeStream = fs.createWriteStream('file.mkv');
var stat = fs.statSync(filePath);
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
```
可以把上面的代码保存为`copy.js`试验一下
我们添加了一个递归的`setTimeout`（或者直接使用setInterval）来做一个旁观者，每500ms观察一次完成进度，并把已完成的大小、百分比和复制速度一并写到控制台上，当复制完成时，计算总的耗费时间，效果如图：
![](http://segmentfault.com/img/bVclbf)
我们复制了一集1080p的权利的游戏第四季第7集，大概3.78G大小，由于使用了SSD，可以看到速度还是非常不错的，哈哈哈~
复制完成后，显示总花费时间
![](http://segmentfault.com/img/bVclbi)
结合nodejs的`readline`， `process.argv`等模块，我们可以添加覆盖提示、强制覆盖、动态指定文件路径等完整的复制方法，有兴趣的可以实现一下，实现完成，可以
```
ln -s /path/to/copy.js /usr/local/bin/mycopy
```
这样就可以使用自己写的`mycopy`命令替代系统的`cp`命令
