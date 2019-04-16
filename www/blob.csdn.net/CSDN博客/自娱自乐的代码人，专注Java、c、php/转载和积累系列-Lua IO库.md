# 转载和积累系列 - Lua IO库 - 自娱自乐的代码人，专注Java、c、php - CSDN博客





2014年01月13日 17:01:42[initphp](https://me.csdn.net/initphp)阅读数：1063








### io.open (filename [, mode])

按指定的模式打开一个文件，成功则返回文件句柄，失败则返回nil+错误信息

mode:


　　"r": 读模式 (默认);


　　"w": 写模式;


　　"a": 添加模式;


　　"r+": 更新模式，所有之前的数据将被保存


　　"w+": 更新模式，所有之前的数据将被清除


　　"a+": 添加更新模式，所有之前的数据将被保存,只允许在文件尾进行添加


　　"b": 某些系统支持二进制方式





### io.close ([file])  - file:close()

相当于file:close()，关闭默认的输出文件




### io.flush ()  - file:flush()

相当于file:flush(),输出所有缓冲中的内容到默认输出文件




### io.lines ([filename]) - file:lines()

打开指定的文件filename为读模式并返回一个迭代函数,每次调用将获得文件中的一行内容,当到文件尾时，将返回nil,并自动关闭文件




```
for line in io.lines("main.lua") do
    print(line)
end
```




### io.output ([file])

相当于io.input，但操作在默认输出文件上




### io.popen ([prog [, mode]])

开始程序prog于额外的进程,并返回用于prog的文件句柄(并不支持所有的系统平台)




### io.read (...) - file:read(...)

按指定的格式读取一个文件,按每个格式函数将返回一个字串或数字,如果不能正确读取将返回nil,若没有指定格式将指默认按行方式进行读取

格式：


　　"*n": 读取一个数字


　　"*a": 从当前位置读取整个文件,若为文件尾，则返回空字串


　　"*l": [默认]读取下一行的内容,若为文件尾，则返回nil


　　number: 读取指定字节数的字符,若为文件尾，则返回nil;如果number为0则返回空字串,若为文件尾，则返回nil;





### io.tmpfile ()

返回一个临时文件句柄，该文件以更新模式打开，程序结束时自动删除




### io.type (obj)

检测obj是否一个可用的文件句柄

返回：


　　"file"：为一个打开的文件句柄


　　"closed file"：为一个已关闭的文件句柄


　　nil:表示obj不是一个文件句柄




### io.write (...) - file:write(...)

相当于io.output():write，写入文件内容




### file:seek([whence][,offset])

设置和获取当前文件位置,成功则返回最终的文件位置(按字节),失败则返回nil加错误信息


参数


　　whence:


　　"set": 从文件头开始


　　"cur": 从当前位置开始[默认]


　　"end": 从文件尾开始


　　offset:默认为0


　　不带参数file:seek()则返回当前位置,file:seek("set")则定位到文件头,file:seek("end")则定位到文件尾并返回文件大小




### file:setvbuf(mode,[,size])

功能：设置输出文件的缓冲模式


　　参数


　　mode:


　　"no": 没有缓冲，即直接输出


　　"full": 全缓冲，即当缓冲满后才进行输出操作(也可调用flush马上输出)


　　"line": 以行为单位，进行输出(多用于终端设备)


　　最后两种模式,size可以指定缓冲的大小(按字节)，忽略size将自动调整为最佳的大小





### 例子：



```
local file = io.open("read.txt", "a+")
local tt = file:read(13) --读取前面13个字符
print(tt)
file:write("Hello World")
file:close()
```




















