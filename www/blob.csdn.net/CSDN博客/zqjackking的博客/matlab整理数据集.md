# matlab整理数据集 - zqjackking的博客 - CSDN博客





2017年03月09日 22:19:33[zqjackking](https://me.csdn.net/zqjackking)阅读数：1890








今天用matlab整理了一天的数据集，心好累。 

原始数据是这样的，(152.0, 115.0, 167.0, 135.0), (221.0, 127.0, 240.0, 152.0)。 

我需要把每个数字字符分出来。 

关键代码如下：
```matlab
tline=fgets(fidin); %从文件读行
 s=regexp(tline,'\([^)]*)','match');  %正则表达式，提取出每个括号中的内容。
 %s{1}是一个字符串，为(152.0, 115.0, 167.0, 135.0)
 %s{2}是一个字符串，为(221.0, 127.0, 240.0, 152.0)
 m=numel(s)；%s中字符串的个数，此处等于2
 for i=1:m
    s2=(regexp(s{i},'\d*\.\d*', 'match'));
    %这也是个正则表达式，作用是提取s{i}字符串中的数字部分。
    for j=1:4
        fprintf('%s',s2{j});
        fprintf('\n');
    end
 end
 %打印出来的s2{j}就是分拆出来的数字
152.0
115.0
167.0
135.0
221.0
127.0
240.0
152.0
```

由上可以看出，正则表达式非常强大。 

之后还经历了文件的删除， 

delete([path,filename]); 

以及文件的重命名。 

 eval([‘!rename’ 32 strcat(path,filesname) 32 newname]); 

 需要注意的是，源文件名前面要加路径，而目标文件名前面则不用。以及32和前后面都要有空格。（忍不住吐槽matlab改文件名为什么这么奇怪的写法） 

 最后涉及到的问题就是读写xml文件。xml文件读写有两种方法，一种是matlab自带的函数，即xmlread,xmlwrite，这个用法感觉非常复杂。 

第二种方法是使用工具包。[http://download.csdn.net/search?keywords=xml_io_tools](%E5%B7%A5%E5%85%B7%E5%8C%85%E5%9C%B0%E5%9D%80)

其提供的 xml_read和xml_write函数非常方便。代码如下：
```
annotation = xml_read(strcat(path_label,label_name));
annotation.filename=files_all(i).name;
xml_write([strcat(path_label,label_name)],annotation);
```

即通过xml_read读出来，然后修改节点内容，最后通过xml_write写进去，就OK了。





