# Oracle CHAR，VARCHAR，VARCHAR2,nvarchar类型的区别与使用 - z69183787的专栏 - CSDN博客
2014年08月08日 11:10:56[OkidoGreen](https://me.csdn.net/z69183787)阅读数：1125
这篇对oracle几种字符类型总结的比较好，收藏了。
1．CHAR的长度是固定的，而VARCHAR2的长度是可以变化的， 比如，存储字符串“abc"，对于CHAR (20)，表示你存储的字符将占20个字节(包括17个空字符)，而同样的VARCHAR2 (20)则只占用3个字节的长度，20只是最大值，当你存储的字符小于20时，按实际长度存储。
　　2．CHAR的效率比VARCHAR2的效率稍高。
　　3．目前VARCHAR是VARCHAR2的同义词。工业标准的VARCHAR类型可以存储空字符串，但是oracle不这样做，尽管它保留以后这样做的权利。Oracle自己开发了一个数据类型VARCHAR2，这个类型不是一个标准的VARCHAR，它将在数据库中varchar列可以存储空字符串的特性改为存储NULL值。如果你想有向后兼容的能力，Oracle建议使用VARCHAR2而不是VARCHAR。
一 varchar，varchar2，nvarchar，nvarchar2 
四个类型都属于变长字符类型， varchar和varchar2的区别在与后者把所有字符都占两字节**（好像都是英文1个 中文2个）**，前者只对汉字和全角等字符占两字节，都是非 Unicode 字符数据，可以定义的最大长度为4000字节。 nvarchar和nvarchar2的区别和上面一样，   与上面区别在于是根据Unicode   标准所进行的定义的类型（长度定义的是字符数，而不是字节数），通常用于支持多国语言类似系统的定义，可以定义的最大长度为4000字符、8000字节。 
1.char 
char的长度是固定的，比如说，你定义了char(20),即使你你插入abc，不足二十个字节，数据库也会在abc后面自动加上17个空格，以补足二十个字节； 
char是区分中英文的，中文在char中占两个字节，而英文占一个，所以char(20)你只能存20个字母或10个汉字。 
char适用于长度比较固定的，一般不含中文的情况 
2.varchar/varchar2 
varchar是长度不固定的，比如说，你定义了varchar(20),当你插入abc，则在数据库中只占3个字节。 
varchar同样区分中英文，这点同char。 
varchar2基本上等同于varchar，它是oracle自己定义的一个非工业标准varchar，不同在于，varchar2用null代替varchar的空字符串 
varchar/varchar2适用于长度不固定的，一般不含中文的情况 
3.nvarchar/nvarchar2 
nvarchar和nvarchar2是长度不固定的，不区分中英文，其长度定义的是字符数
nvarchar不区分中英文，比如说：你定义了nvarchar(20),你可以存入20个英文字母/汉字或中英文组合，这个20定义的是字符数而不是字节数 
nvarchar2基本上等同于nvarchar，不同在于nvarchar2中存的英文字母也占两个字节 
nvarchar/nvarchar2适用于存放中文
1.NULL值(空值)。 
     a. char列的NULL值占用存储空间。char类型可以定义空字符串
     b. varcahr列的NULL值不占用存储空间。varchar类型定义的空字符串按照null进行处理
c. 插入同样数量的NULL值，varchar列的插入效率明显高出char列。 
2.插入数据 
    无论插入数据涉及的列是否建立索引，char的效率都明显低于varchar。 
3. 更新数据 
     如果更新的列上未建立索引，则char的效率低于varchar，差异不大；建立索引的话，效率较高。 
4. 修改结构 
     a. 无论增加或是删除的列的类型是char还是varchar，操作都能较快的完成，而且效率上没有什么差异。 
     b. 对于增加列的宽度而言，char与varchar有非常明显的效率差异，修改varcahr列基本上不花费时间，而修改char列需要花费很长的时间。 
5.数据检索 
     无论是否通过索引，varchar类型的数据检索略优于char的扫描。 
选择char还是选择varchar的建议 
    1.适宜于char的情况： 
     a. 列中的各行数据长度基本一致，长度变化不超过50字节； 
     b. 数据变更频繁，数据检索的需求较少。 
     c. 列的长度不会变化，修改char类型列的宽度的代价比较大。 
     d. 列中不会出现大量的NULL值。 
     e. 列上不需要建立过多的索引，过多的索引对char列的数据变更影响较大。 
    2.适宜于varchar的情况; 
     a. 列中的各行数据的长度差异比较大。 
     b. 列中数据的更新非常少，但查询非常频繁。 
     c. 列中经常没有数据，为NULL值或为空值
占用长度的比较：
1.CHAR(size)和VARCHAR(size)的区别 
    CHAR为定长的字段，最大长度为2K字节； 
    VARCHAR为可变长的字段，最大长度为4K字节； 
2.CHAR(size)和NCHAR(size)的区别 
    CHAR如果存放字母数字占1个字节，存放GBK编码的汉字存放2个字节，存放UTF-8编码的汉字占用3个字节； 
    NCHAR根据所选字符集来定义存放字符的占用字节数，一般都为2个字节存放一个字符(不管字符或者汉字) 
3.VARCHAR(size)和VARCHAR2(size)的区别 
  后者把所有字符都占两字节，前者只对汉字和全角等字符占两字节，如果存放的字符为中英文结合的数据，那么存放的字符个数不同，varchar多于varchar2；占用的最大长度为4K字节；推荐使用VARCHAR2； 
4.VARCHAR2(size)和NVARCHAR2(size)的区别 
   后者把所有字符都占两字节，前者只对汉字和全角等字符占两字节；（如果数据库字符集长度是2，则NVARCHAR2最大为2K）
由于定义长度是字符数，nvarchar和nvarchar2定义相同的长度，如果数据既有中文又有英文和数字，则nvarchar占用的存储空间小于nvarchar2
转载请说明出处：[http://www.cnblogs.com/ItisFriday/archive/2010/09/24/1832971.html](http://www.cnblogs.com/ItisFriday/archive/2010/09/24/1832971.html)
