# NodeJS入门(3)-文件操作 - 零度的博客专栏 - CSDN博客
2016年11月02日 13:48:13[零度anngle](https://me.csdn.net/zmx729618)阅读数：912
### 小文件拷贝
    var fs = require('fs');
    function copy(src, dst) {//接受源文件地址和目的文件地址
        fs.writeFileSync(dst, fs.readFileSync(src));//从源路径中读取文件内容，然后写入目标路径
    }
    function main(argv) {
        copy(argv[0], argv[1]);
    }
    main(process.argv.slice(2));//`process`是个全局变量，可通过`process.argv`获得命令行参数。由于`argv[0]`固定等于NodeJS程序文件的绝对路径，`argv[1]`固定等于主模块的绝对路径，因此第一个命令参数从`argv[2]`这个位置开始
### 大文件拷贝
    var fs = require('fs');
    function copy(src, dst) {
    fs.createReadStream(src).pipe(fs.createWriteStream(dst));//先创建一个源文件的只读数据流，然后创建一个文件的只写数据源，然后用`pipe`方法把两个数据流连接起来，实现边读边写的操作。
    }
    function main(argv) {
        copy(argv[0], argv[1]);
    }
    main(process.argv.slice(2));
### Buffer(数据块)http://nodejs.org/api/buffer.html
1.JS语言本身只有字符串数据类型，没有二进制数据类型。因此，NodeJS提供了一个与String对等的全局构造函数Buffer来提供对二进制数据的操作。
 典型用法如下：  var bin=new Buffer([0x68,0x65,0x6c,0x6f,0x6f]);//构造一个字符串
 console.log(bin.length);    //输出长度
 bin[0]; //0x68
 var str = bin.toString("UTF-9");//hello,用指定编码将二进制数据转化为字符串；
 var bin = new Buffer('hello', 'utf-8'); // => <Buffer 68 65 6c 6c 6f>，将字符串转换为指定编码下的二进制数据
2.Buffer与字符串有一个重要区别。字符串是只读的，并且对字符串的任何修改得到的都是一个新字符串，原字符串保持不变。至于Buffer，更像是可以做指针操作的C语言数组。例如，可以用[index]方式直接修改某个位置的字节。bin[0] = 0x48;,而.slice方法也不是返回一个新的Buffer，而更像是返回了指向原Buffer中间的某个位置的指针，因此对.slice方法返回的Buffer的修改会作用于原Buffer。
如果想要拷贝一份Buffer，得首先创建一个新的Buffer，并通过.copy方法把原Buffer中的数据复制过去。这个类似于申请一块新的内存，并把已有内存中的数据复制过去。以下是一个例子。Buffer将JS的数据处理能力从字符串扩展到了任意二进制数据
 var bin = new Buffer([ 0x68, 0x65, 0x6c, 0x6c, 0x6f ]);
 var dup = new Buffer(bin.length);
 bin.copy(dup);
 dup[0] = 0x48;
 console.log(bin); // => <Buffer 68 65 6c 6c 6f>
 console.log(dup); // => <Buffer 48 65 65 6c 6f>
### Stream(数据流)[http://nodejs.org/api/stream.html)
1.当内存中无法一次装下需要处理的数据时或者一边读一边处理文件时，就需要用数据流。NodeJS中通过各种Stream来提供对数据流的操作。Stream基于事件机制工作，所有的Stream的实例都继承于NodeJS提供的EventEmitter
2.还是大文件拷贝的例子，创建一个只读数据流
 var rs = fs.createReadStream(pathname);          
 rs.on('data', function (chunk) {//`data`事件会被源源不断的触发，不管`doSomething`函数是否处理得过来。
     doSomething(chunk);
 });
 rs.on('end', function () {
     cleanUp();
 });
继续改造代码：
    var rs = fs.createReadStream(src);
    rs.on('data', function (chunk) {//通过回调添加暂停功能，处理完毕后再继续
        rs.pause();
        doSomething(chunk, function () {
            rs.resume();
        });
    });
    rs.on('end', function () {
        cleanUp();
    });
 一个读写文件的例子：
   var rs = fs.createReadStream(src);
   var ws = fs.createWriteStream(dst);
    rs.on('data', function (chunk) {//如果写的速度太慢，出导致内存溢出
        ws.write(chunk);
    });
    rs.on('end', function () {
        ws.end();
    });
 继续改造代码：
    var rs = fs.createReadStream(src);
    var ws = fs.createWriteStream(dst);
    rs.on('data', function (chunk) {
        if (ws.write(chunk) === false) {//.write可以判断是写入目标了还是放了缓存了
            rs.pause();
        }
    });
    rs.on('end', function () {
        ws.end();
    });
    ws.on('drain', function () {//判断是否已经将缓存中的数据写入标目
        rs.resume();
    });
### File System(文件系统)[http://nodejs.org/api/fs.html)
1.NodeJS通过fs内置模块提供对文件的操作。主要有三类：
        ◦文件属性读写：fs.stat,fs.chmod,fs.chown等
        ◦文件内容的读写：fs.readFile，fs.readdir，fs.writeFile，fs.mkdir等
        ◦底层文件操作：fs.open，fs.read，fs.write，fs.close等
2.fs模块的所有异步API都有对应的同步版本，用于无法使用异步操作时，或者同步操作更方便时的情况。同步API除了方法名的末尾多了Sync外，异常对象与执行结果的传递方式也有相应变化。下面是两个例子： 
 fs.readFile(pathname, function (err, data) {
     if (err) {
         // Deal with error.
     } else {
         // Deal with data.
     }
 });
对应同步版本：  
try {
     var data = fs.readFileSync(pathname);
     // Deal with data.
 } catch (err) {
     // Deal with error.
 }
### Path(路径)[http://nodejs.org/api/path.html)
1.path内置模块用来简化路径相关操作，提高代码可读性。下面是几个常用api。
2.path.normalize将传入的路径转换为标准路径。它在linux下和windows生成的斜杠不同。
   var cache = {};
   function store(key, value) {
       cache[path.normalize(key)] = value;
   }
   store('foo/bar', 1);
   store('foo//baz//../bar', 2);
   console.log(cache);  // => { "foo/bar": 2 }
2.path.join将路径拼接为标准路径。如path.join('fool/','baz/','../bar');//=>"foo/bar"
3.path.extname:根据不同的扩展名作不同操作：path.extname('foo/bar.js'); // => ".js"
遍历目录
1.递归算法：在需要优先考虑性能时，需要把递归算法转换为循环算法，以减少函数调用次数。  
function factorial(n) {
     if (n === 1) {
         return 1;
     } else {
         return n * factorial(n - 1);
     }
 }
2.遍历算法：目录是一个树状结构，在遍历时一般使用深度优先+先序遍历算法。深度优先，意味着到达一个节点后，首先接着遍历子节点而不是邻居节点。先序遍历，意味着首次到达了某节点就算遍历完成，而不是最后一次返回某节点才算数。因此使用这种遍历方式时，下边这棵树的遍历顺序是A > B > D > E > C > F。
           A
          / \
         B   C
        / \   \
       D   E   F
◦同步遍历：
function travel(dir, callback) {
  fs.readdirSync(dir).forEach(function (file) {
      var pathname = path.join(dir, file);
      if (fs.statSync(pathname).isDirectory()) {
          travel(pathname, callback);
      } else {
          callback(pathname);
      }
  });
}
◦异步遍历：
function travel(dir, callback, finish) {
  fs.readdir(dir, function (err, files) {
      (function next(i) {
          if (i < files.length) {
              var pathname = path.join(dir, files[i]);
              fs.stat(pathname, function (err, stats) {
                  if (stats.isDirectory()) {
                      travel(pathname, callback, function () {
                          next(i + 1);
                      });
                  } else {
                      callback(pathname, function () {
                          next(i + 1);
                      });
                  }
              });
          } else {
              finish && finish();
          }
      }(0));
  });
}
### 文本编码
1.常用文本编码有UTF8和GBK两种，并且:UTF8可能还带有BOM。在读取不同编码的文本文件时，需要将文件内容转换为JS使用的UTF8编码字符串后才能正常处理。
2.移除BOM：BOM用于标记一个文本文件使用Unicode编码，其本身是一个Unicode字符（"\uFEFF"），位于文本文件头部。在不同的Unicode编码下，BOM字符对应的二进制字节如下：
     Bytes      Encoding
 ----------------------------
     FE FF       UTF16BE
     FF FE       UTF16LE
     EF BB BF    UTF8
以下代码实现了识别和去除UTF8 BOM的功能。
 function readText(pathname) {
     var bin = fs.readFileSync(pathname);
     if (bin[0] === 0xEF && bin[1] === 0xBB && bin[2] === 0xBF) {
         bin = bin.slice(3);
     }
     return bin.toString('utf-8');
 }
3.GBK编码不在NodeJS自身支持范围内，因此，我们一般借助iconv-lite第三方包来转换编码。下载该NPM后，可下边方式编写一个读取GBK文本文件的函数。
 var iconv = require('iconv-lite');
 function readGBKText(pathname) {
     var bin = fs.readFileSync(pathname);
     return iconv.decode(bin, 'gbk');
 }
4.单字节编码，如果无法预知文件编码，可以使用单字节编码来读取文件，不用关心文件是GBK还是UTF8编码。 
 1. GBK编码源文件内容：var foo = '中文';
 2. 对应字节：
     76 61 72 20 66 6F 6F 20 3D 20 27 D6 D0 CE C4 27 3B
 3. 使用单字节编码读取后得到的内容：
     var foo = '{乱码}{乱码}{乱码}{乱码}';
 4. 替换内容：
     var bar = '{乱码}{乱码}{乱码}{乱码}';
 5. 使用单字节编码保存后对应字节：
     76 61 72 20 62 61 72 20 3D 20 27 D6 D0 CE C4 27 3B
 6. 使用GBK编码读取后得到内容：
     var bar = '中文';
上述例子说明直接操作单字节乱码字符时，背后对应的字节保持不变。
NodeJS自带了一种binary编码可以用来实现这个方法，如下：  
function replace(pathname) {
     var str = fs.readFileSync(pathname, 'binary');
     str = str.replace('foo', 'bar');
     fs.writeFileSync(pathname, str, 'binary');
 }
      .
