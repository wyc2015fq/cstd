# 删除C程序中的注释 - xiahouzuoxin - CSDN博客





2014年02月15日 17:44:48[xiahouzuoxin](https://me.csdn.net/xiahouzuoxin)阅读数：4729








搞神马软件著作权，什么，还得把所有注释去了，代码之间不能有空行。。。。

好吧，删到手麻还不知道删干净没有，突然想到，干嘛不用程序呢？




也不东藏西捏的，直接给出matlab的代码，matlab中直接可用：



```
% 删除C文件中的注释内容，删除注释后删除空行
% xiahouzuoxin
% 2014.02.15

% 空格 ASCII=32   TAB ASCII=9   换行ASCII=10
% 测试OK，能删除：
% (1) C源文件中/**/和//格式的注释
% (2) 非文件首尾的空行
% (3) 行尾过多的填充空格和TAB字符

clc;
clear;

[fname,pname]=uigetfile(...
    {'*.c';'*.h';'*.*'},...
    'Input C File');
fid = fopen(fullfile(pname,fname), 'r');
c_codes = fread(fid, inf);

% 删除注释
is_notes = 0;
i = 1;
while 1
    % 终止程序
    if i>=length(c_codes)
        break;
    end    
    
    if is_notes == 0 && c_codes(i)==47 % '/'
        if c_codes(i+1)==42 % '/*'
            is_notes = 1; 
            c_codes(i) = 32; % 空格
            c_codes(i+1) = 32; % 空格
            i = i + 2;
            continue;
        elseif c_codes(i+1)==47 % '//'
            is_notes = 2;
            c_codes(i) = 32; % 空格
            c_codes(i+1) = 32; % 空格
            i = i + 2;
            continue;
        end          
    elseif is_notes == 1 && c_codes(i)==42 % '*'
        if c_codes(i+1)==47 % '*'
            is_notes = 0;
            c_codes(i) = 32; % 空格
            c_codes(i+1) = 32; % 空格
            i = i + 2;
            continue;
        end   
    elseif is_notes == 2 && c_codes(i)==10 % '换行符'
            is_notes = 0;
            i = i + 1;
            continue;
    end

    if is_notes ~= 0
        c_codes(i) = 32; % 空格 
    else
        % do nothing
    end
    i = i + 1;
end

% 删除空行
start_addr = 0;
is_null = 0;  % 初始化为0
end_while = 0;
i = 1;
while ~end_while
    if c_codes(i)==10  % 换行
        if is_null == 1  % 两个换行符之间全部为空格或TAB
            c_codes(start_addr:i-1) = [];
            i = start_addr;  % 删除后原向量长度将减小，要更新索引
        end
        start_addr = i;
        is_null = 1;
    elseif c_codes(i)~=32 && c_codes(i)~=9 % 不是空格和TAB
        is_null = 0;
    end
    
    i = i + 1;
    if i>=length(c_codes)
        end_while  =1; 
    end
end

% 删除多余空格，防止由于注释导致的单行代码过长
start_addr = 0;
del_space = 0;  % 初始化为0
end_while = 0;
i = 1;
while ~end_while
    if (c_codes(i)==32 || c_codes(i)==9) && del_space == 0  % 是第一个空格
        del_space = 1;
        start_addr = i;
    elseif c_codes(i)==10  % 换行
        if del_space == 1  % 两个换行符之间全部为空格
             c_codes(start_addr:i-1) = [];
             i = start_addr;  % 删除后原向量长度将减小，要更新索引
        end
        del_space = 0;
    elseif c_codes(i)~=32 && c_codes(i)~=9  % 不是空格和TAB及换行
        del_space = 0;    
    end
    
    i = i + 1;
    if i>=length(c_codes)
        end_while  =1; 
    end
end

% 写到新文件中
fid_new = fopen('new.c', 'w+');
fwrite(fid_new, c_codes);
fclose(fid_new);

fprintf('Process successfull.\n');

% 关闭源文件
fclose(fid);
```


唠嗑几小下：


（1）根据换行符查找//注释的尾端

（2）先删除文档中所有注释后再进行空行处理，注释的处理是：将注释部分转换成空格

（3）我使用matlab而不用其它语言的原因有2，其一是我还算比较熟悉matlab，可视化及数据操作比C语言要强很多，其二，呃，惭愧，除了C语言和matlab外还不会用任何其它第三方计算机语言。。。




后来又试了试各种.c文件，发现上面的代码总是没法删除部分空行，想起awk脚本似乎删除空行很容易，于是又增加了一个文件名为rm_spaceline.sh的awk脚本（先使用matlab去注释）：



```
#########################################################################
# FileName : rm_spaceline.sh
# Author   : xiahouzuoxin @163.com
# Date     : 2014/10/19 11:59:14
# NOtes    : remove sapce line of all files
#########################################################################
# 
#!/bin/bash
for name in `find $1`
do
if [ ! -d $name ];then
  if [ $name == "./rm_spaceline.sh" ];then
    echo "$name"
  else
    awk NF $name >> new.c
    echo "$name"
  fi
fi
done
```





