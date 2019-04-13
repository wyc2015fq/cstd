
# Node.js环境中同步分行读取文件 - cocoonyang的专栏 - CSDN博客


2016年06月19日 08:54:35[cocoonyang](https://me.csdn.net/cocoonyang)阅读数：3301


In Node.js environment, readline module supports asynchronously reading a file line by line. However, synchronously reading  is not supported.  Inspired by the codes in http://stackoverflow.com/questions/7545147/nodejs-synchronization-read-large-file-line-by-line.   Readlinesyn, a Node.js module, as shown following, provides synchronously reading functionality.

```python
'use strict';
//  
//  Reading file line by line synochronicaly libraries  
//  
//  Author: Chunfeng Yang  
//  All rights reserved.    
//  This software is supplied "AS IS" without any warranties or support.   
//  The author assumes no responsibility or liability for the use of the software, 
//  conveys no license or title under any patent, copyright, or mask work right to the product.  
//  The author reserves the right to make changes in the software without notification.  
// 
//
//  Date: 2016-06-21
//       Remove a bug in open function
//
//  Date: 2016-09-14
//       Open the file path passed as constructor argument
//
//  Date: 2017-05-16
//       The default buffer size is 1024, it is sufficient for many scenarios.
//       In old version, when a line is longer than the buffersize, the next() would return undefined result.   
//       Now, if the line length longer than the buffer size, this version would read next line, until reaching 
//       "\n" character.  
//
const fs = require('fs');
var LineReader = function( path ) {
    this._leftOver = '';
    this._EOF = false;
    this._filename;
    this._fd = 0;
    this._bufferSize = 1024;
    this._buffer = new Buffer(this._bufferSize);
    if( undefined !== path )
    {
        try{
            fs.statSync( path ).isFile() ; 
            this.open( path );
        } 
        catch ( exception )
        {  
            console.log( path, 'is not a file.');
            this._EOF = false;
            return;
        }
    }
}
LineReader.prototype.close = function( ) 
{ 
    var self = this;
    try{
        fs.closeSync(self._fd); 
    }
    catch ( exception )
    {
        console.log( 'closing file failed.');
    }
    self._EOF = true;
    self._fd = 0; 
    return;
}
LineReader.prototype.next = function( ) 
{ 
        var self = this;
        if(0 == self._fd)
        {
             return;
        }
	var _idxStart = 0;
        var idx = 0; 
	while(( self._leftOver.indexOf('\n', _idxStart)) == -1){
            var read;
            try{
                read = fs.readSync( self._fd, self._buffer, 0, self._bufferSize, null)
            } 
            catch ( exception )
            {
                 console.log( 'reading file failed.');
                 self.close(); 
                 return;
            }
            if (read !== 0) {
              self._leftOver  += self._buffer.toString('utf8', 0, read);
            }else {
                 try{
                     fs.closeSync(self._fd);
                  }
                 catch ( exception )
                 {
                    console.log( 'closing file failed.');
                 }
                 self._EOF = true;
                 self._fd = 0; 
                 return;
            }
	}
       if ((idx = self._leftOver.indexOf('\n', _idxStart)) !== -1) {
            var line = self._leftOver.substring(_idxStart, idx);
            _idxStart = idx + 1;
            self._leftOver = self._leftOver.substring(_idxStart);
            _idxStart = 0;
            return line;
        } 
}
LineReader.prototype.open =  function( thePath ) 
{ 
        var self = this;
        self._filename = thePath;
        if(0 !== self._fd)
        {
             self.close();
        }
        try{
            self._fd = fs.openSync( self._filename, 'r'); 
        } 
        catch ( exception )
        {
            console.log( 'open(): ' + self._filename + ' not found.');
            self._EOF = true;
            return;
        } 
        self._EOF = false;
        return;
}
module.exports = LineReader;
```

使用样例


```python
const LineByLine = require('./readlinesyn'); 
var filename = './result.txt';
var liner = new LineByLine();
liner.open( filename ); 
var theline;
while( !liner._EOF )
{
  theline = liner.next();
   console.log( 'READ LINE: ' + theline );
}
liner.close();
```

读取某一文件夹中所有txt文件


```python
var fs = require('fs');
const LineByLine = require('./readlinesyn'); 
var dir = "./abc/";
var writeStream = fs.createWriteStream("./foo.txt" , {'flags': 'a'} );
var liner = new LineByLine();  
var files = fs.readdirSync( dir );
for( index in files )
{
   var fileName = files[index];
   var r = fileName.search(".txt");
   if( -1 != r )
   {
        console.log( fileName );
        liner.open( dir + fileName );   
        var theline;  
        while( !liner._EOF )  
        {  
            theline = liner.next();  
            writeStream.write( String(theline) );
        }  
        liner.close(); 
   }
}
```




