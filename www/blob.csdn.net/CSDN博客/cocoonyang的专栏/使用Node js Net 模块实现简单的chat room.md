# 使用Node.js Net 模块实现简单的chat room - cocoonyang的专栏 - CSDN博客





2015年11月27日 06:41:59[cocoonyang](https://me.csdn.net/cocoonyang)阅读数：722
个人分类：[nodejs](https://blog.csdn.net/cocoonyang/article/category/1301193)












参考网络上一些Node.js Net 模块实现chat room的应用，实现了一个简单chat room



```java
var net = require('net');
var rl = require('readline');
 
var sockets = [];
 
/*
 * Cleans the input of carriage return, newline
 */
function cleanInput(data) {
	return data.toString().replace(/(\r\n|\n|\r)/gm,"");
}
 
/*
 * Method executed when data is received from a socket
 */
function receiveData(socket, data) {
	var cleanData = cleanInput(data);

        console.log( cleanData );

	if(cleanData === 'quit') {
		socket.end('Goodbye!\n');
	}
	else {
		for(var i = 0; i<sockets.length; i++) {
			if (sockets[i] !== socket) {
				sockets[i].write(data);
			}
		}
	}
}
 
/*
 * Method executed when a socket ends
 */
function closeSocket(socket) {
	var i = sockets.indexOf(socket);
	if (i != -1) {
		sockets.splice(i, 1);
	}
}
 
/*
 * Callback method executed when a new TCP socket is opened.
 */
function newSocket(socket) {
	sockets.push(socket);
	socket.write('Welcome to the Telnet server!\n');



    var i = rl.createInterface(socket, socket);
    i.on('line', function (line) {
        socket.write(line);
    });


//	socket.on('data', function(data) {
//		receiveData(socket, data);
//	})


var x='';
socket.write('Echo server\r\n');
socket.on('data', function(data) {
         if (data.toString().charCodeAt()==13) {
                console.log(x);
                receiveData(socket, x);
                x='';
            } else {
               x=x+data.toString();
            }
         });


	socket.on('end', function() {
		closeSocket(socket);
	})
}
 
// Create a new server and provide a callback for when a connection occurs
var server = net.createServer(newSocket);
 
// Listen on port 5000
server.listen(5000);
```





参考文献

[1] https://github.com/danscan/stacks







