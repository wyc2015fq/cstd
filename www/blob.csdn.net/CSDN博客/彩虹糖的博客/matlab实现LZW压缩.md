# matlab实现LZW压缩 - 彩虹糖的博客 - CSDN博客





2018年09月21日 09:48:27[彩虹糖梦](https://me.csdn.net/caozixuan98724)阅读数：707








LZW压缩（LZW compression）是一种由Abraham Lempel、Jacob Ziv和Terry Welch发明的基于表查寻算法把文件压缩成小文件的无损压缩方法。具体压缩原理参考[LZW压缩算法解析](https://segmentfault.com/a/1190000011425787)，在此不再赘述，参考[LZW算法的个人理解与简单Python实现](https://blog.csdn.net/fly1183989782/article/details/25706119)该文章中的python实现方式写出来matlab代码，为学习matlab基本语法用。

### compress.m

```
function array_after_compress = compress( input_array )
%   该函数可以进行LZW压缩
%   该函数输入为ab序列，输出为压缩后的序列
dict = struct('a',1,'b',2);
prefix = '';
curChar='';
curStr='';
array_after_compress=[];
for curChar = input_array
    curStr = [prefix curChar];
    if isfield(dict,curStr)
        output = -1;
        prefix = curStr;
    else
        output=dict.(prefix);
        prefix = curChar;
        dict.(curStr)= length(fieldnames(dict))+1;
    end
    if output~=-1
        array_after_compress(end+1)=output;
    end
end
output = dict.(prefix);
array_after_compress(end+1)=output;
end
```

### decompress.m

```
function array_after_decompress = decompress( input_array )
curCodeStr = '';
oldCodeStr='';
array_after_decompress=[];
dicSize = 2;
dict(1).id=1;
dict(1).str='a';
dict(2).id=2;
dict(2).str='b';
for curCode = input_array
    [flag, index] = search(dict, curCode);
    if flag
        curCodeStr = dict(index).str;
    else
        curCodeStr = [oldCodeStr oldCodeStr(1)];
    end
    if strcmpi(oldCodeStr,'')~=1
        dicSize = dicSize + 1;
        dict(dicSize).id = dicSize;
        dict(dicSize).str = [oldCodeStr curCodeStr(1)];
    end
    oldCodeStr = curCodeStr;
    array_after_decompress = [array_after_decompress curCodeStr];
end
```

### search.m

```
function [flag, index] = search( input_array, curCode )
flag = 0;
index = -1;
for element = input_array
    if element.id == curCode
        flag=1;
        index = element.id;
        break;
    end
end
```

运行结果如下图所示：

![](https://img-blog.csdn.net/20180921094752684?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L2Nhb3ppeHVhbjk4NzI0/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)



