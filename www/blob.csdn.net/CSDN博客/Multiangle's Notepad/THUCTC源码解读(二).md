
# THUCTC源码解读(二) - Multiangle's Notepad - CSDN博客


2016年05月18日 21:24:45[multiangle](https://me.csdn.net/u014595019)阅读数：2815所属专栏：[自然语言处理](https://blog.csdn.net/column/details/13461.html)



在通过Demo初步了解了THUCTC的用法以后，开始深入探究THUCTC的结构，了解实现方式。只要了解了代码结构，才能了解背后的原理和优化方法，也方便在此基础上做出自己的改进。
---

## THUCTC的主要原理
首先，会将训练文本进行分词处理，然后进行词频统计，通过统计词频和包含该词的文档频率，利用卡方(Chi-Square)检验来选择出特征词，并以此为依据构造文档向量(DocumentVector)，词向量中的每一项均代表一个特征（词），具体的值则为相应的权重，这里一般采用TF-IDF方式，即词的重要性与该文档中该次出现次数成正比，与包含该词的文件频率成反比。在构造完文档向量之后，则将训练和分类的任务交给了现成的库(liblinear或libsvm)。因此，也可以说，THUCTC主要完成的是文档的向量化工作。
---

## Word类
Word类是THUCTC中最基本的类之一，功能很简单，就是存储单词信息，是词典的基本单元
存储的内容有:
id该词在词典（Lexicon）中的id
name该词本身，如”中国”
tf即term frequency，词频，指该词在所有文档中出现次数
df即document frequency ，文频，指出现该词的文档数
---

## Lexicon类
Lexicon,即词典类，顾名思义，处理与词典相关的操作，其基本组成是Word对象。Lexicon类中的**变量**有:
```python
Hashtable<
```
```python
Integer
```
```python
,
```
```python
Word
```
```python
> idHash     //从id到
```
```python
Word
```
```python
的映射，能够根据id获得对应
```
```python
Word
```
```python
对象
Hashtable<
```
```python
String
```
```python
,
```
```python
Word
```
```python
> nameHash    //从name到
```
```python
Word
```
```python
的映射，能够根据单词获得对应
```
```python
Word
```
```python
对象
long numDocs        // 处理过的文档数目
boolean locked      // 当词典被锁定时，locked=true 此时无法向词典中加入新
```
```python
Word
```
```python
Set
```
```python
<
```
```python
Integer
```
```python
> termSet //是一个private项，用于保存一个文档中出现过的单词，用于统计df
```
在来看Lexicon类中的**方法**，主要有
```python
[
```
```python
1
```
```python
]
```
```python
public
```
```python
Lexicon
```
```python
()
[2]
```
```python
public
```
```python
Word
```
```python
getWord
```
```python
(
```
```python
int
```
```python
id )
[3]
```
```python
public
```
```python
Word
```
```python
getWord
```
```python
( String name )
[4]
```
```python
public
```
```python
void
```
```python
addDocument
```
```python
( String [] doc ) 
[5]
```
```python
public
```
```python
Word []
```
```python
convertDocument
```
```python
( String [] doc )
[6]
```
```python
protected
```
```python
Word
```
```python
buildWord
```
```python
( String termString )
[7]
```
```python
public
```
```python
boolean
```
```python
loadFromFile
```
```python
( File f )
[8]
```
```python
public
```
```python
boolean
```
```python
loadFromInputStream
```
```python
(InputStream input)
[9]
```
```python
public
```
```python
Lexicon
```
```python
map
```
```python
( Map<Integer, Integer> translation )
```
我们一个个来看。
### [1] public Lexicon()
```python
public
```
```python
Lexicon
```
```python
() {
    idHash =
```
```python
new
```
```python
Hashtable<Integer, Word>(
```
```python
50000
```
```python
);
    nameHash =
```
```python
new
```
```python
Hashtable<String, Word>(
```
```python
50000
```
```python
);
    locked =
```
```python
false
```
```python
;
    numDocs =
```
```python
0
```
```python
;
  }
```
构建Lexicon对象的时候，
构建空的idHash和nameHash，容量为50000
将locked设为false, 即可以添加新单词
将numDocs设为0，表示未处理过文档
### [2]public Word getWord( int id ), [3] public Word getWord( String name )
```python
public
```
```python
Word
```
```python
getWord
```
```python
(
```
```python
int
```
```python
id ) {
```
```python
return
```
```python
idHash.
```
```python
get
```
```python
( id );
  }
```
```python
public
```
```python
Word
```
```python
getWord
```
```python
( String name ) {
```
```python
return
```
```python
nameHash.
```
```python
get
```
```python
( name );
  }
```
[2]和[3]可以一起来看，**前者是根据id提取出Word，后者根据name提取出Word**
### [4] public void addDocument ( String [] doc )
这个函数用于**处理新文档以更新词典的状态(单词项，tf, df 值等等)**
函数的输入doc需要是一个String数组，即此时的文档是分词完毕以后的结果，数组中的每一个字符串都是一个单词
```python
public
```
```python
void
```
```python
addDocument
```
```python
( String [] doc ) {
    termSet.clear();
```
```python
// 将termSet清空，准备处理新文档
```
```python
for
```
```python
( String token : doc ) {
      Word t = nameHash.
```
```python
get
```
```python
(token);
```
```python
// 对于每个单词，使用nameHash获取对应Word对象
```
```python
if
```
```python
( t ==
```
```python
null
```
```python
) {
```
```python
if
```
```python
( locked )
```
```python
continue
```
```python
;
```
```python
// 若此时词典中无该单词但是locked=true,则跳过，即不添加新词
```
```python
t =
```
```python
new
```
```python
Word();
```
```python
// 若locked=false，则添加新词
```
```python
t.name = token;
        t.id = nameHash.size();
        t.tf =
```
```python
0
```
```python
;       
        t.df =
```
```python
0
```
```python
;
        nameHash.put(t.name, t);
```
```python
//将新词添加进idHash和nameHash
```
```python
idHash.put(t.id, t);
      }
      t.tf +=
```
```python
1
```
```python
;
```
```python
// 该词的term frequency +1
```
```python
if
```
```python
( ! termSet.contains(t.id) ) {
        termSet.add(t.id);
```
```python
// 若该文档之前没出现过这个词，则该词的文档频率+1，否则不管
```
```python
t.df++;
      }
    }
    numDocs ++ ;
```
```python
// numDocs++ ，表示处理过的文档数增加1
```
```python
}
```
### [5] public Word [] convertDocument ( String [] doc )
**将分词后的文档转化成Word数组**。如果出现新词，若locked=true 则更新词典，否则会跳过该新词
```python
public
```
```python
Word []
```
```python
convertDocument
```
```python
( String [] doc ) {
    Word [] terms =
```
```python
new
```
```python
Word[doc.length];
```
```python
// 根据doc的长度来建立Word数组
```
```python
int
```
```python
n =
```
```python
0
```
```python
;
```
```python
for
```
```python
(
```
```python
int
```
```python
i =
```
```python
0
```
```python
; i < doc.length ; i++ ) {
      String token = doc[i];
      Word t = nameHash.
```
```python
get
```
```python
( token );
```
```python
if
```
```python
( t ==
```
```python
null
```
```python
) {
```
```python
if
```
```python
( locked )
```
```python
continue
```
```python
;
        t =
```
```python
new
```
```python
Word ();
        t.name = token;
        t.tf =
```
```python
1
```
```python
;
        t.df =
```
```python
1
```
```python
;
        t.id = nameHash.size();
        nameHash.put(t.name, t);
        idHash.put(t.id, t);
      }
      terms[n++] = t;
```
```python
// 到此为止总体跟[4]addDocument比较相似，
```
```python
//但是要注意当locked=true时，碰到新词是会跳过的
```
```python
//也就是说，有可能会出现Word向量的长度小于doc长度的情况
```
```python
}
```
```python
if
```
```python
( n < terms.length ) {
      Word [] finalterms =
```
```python
new
```
```python
Word[n];
```
```python
//这边就是处理上面提到过的情况
```
```python
for
```
```python
(
```
```python
int
```
```python
i =
```
```python
0
```
```python
; i < n ; i++ ) {
        finalterms[i] = terms[i];
      }
      terms = finalterms;
    }
```
```python
return
```
```python
terms;
}
```
### [6] protected Word buildWord ( String termString )
这个方法是protected类型，不对外开放，是为下面两个方法loadFromInputStream服务的。
**实现了根据一行字符串来新建Word对象的功能**
```python
protected
```
```python
Word
```
```python
buildWord
```
```python
( String termString ) {
```
```python
// 举例；  26163:附赠:114:94
```
```python
Word t =
```
```python
null
```
```python
;
    String [] parts = termString.split(
```
```python
":"
```
```python
);
```
```python
//将字符串根据';'分割成字符串数组
```
```python
if
```
```python
( parts.length ==
```
```python
4
```
```python
) {
      t =
```
```python
new
```
```python
Word();
```
```python
// 各项的值分别是: id,name,tf,df
```
```python
t.id = Integer.parseInt(parts[
```
```python
0
```
```python
]);
      t.name = parts[
```
```python
1
```
```python
].replace(COLON_REPLACER,
```
```python
":"
```
```python
);
      t.tf = Integer.parseInt(parts[
```
```python
2
```
```python
]);
      t.df = Integer.parseInt(parts[
```
```python
3
```
```python
]);
    }
```
```python
return
```
```python
t;
  }
```
### [7] public boolean loadFromFile( File f )
这个函数和后面的loadFromInputStream功能类似，都是用于**从本地文件中载入之前构造好的词典**
这个函数比较简单，先将文件转化成FileInputStream,然后调用[8]loadFromInputStream
```python
public
```
```python
boolean
```
```python
loadFromFile
```
```python
( File f ) {
    FileInputStream fis;
```
```python
try
```
```python
{
      fis =
```
```python
new
```
```python
FileInputStream(f);
```
```python
//FileInputStream能将文件转化成字节流
```
```python
}
```
```python
catch
```
```python
(FileNotFoundException e) {
```
```python
return
```
```python
false
```
```python
;
    }
```
```python
return
```
```python
loadFromInputStream(fis);
  }
```
### [8] public boolean loadFromInputStream(InputStream input)
**用于从字节流中载入之前构造好的词典**
```python
public
```
```python
boolean
```
```python
loadFromInputStream
```
```python
(InputStream input) {
    nameHash.clear();
```
```python
// 先将之前的nameHash和idHash清空
```
```python
idHash.clear();
```
```python
try
```
```python
{
```
```python
//BufferedReader( InputStreamReader( FileInputStream( File ) ) )
```
```python
// FileInputStream 能把文件转化成字节流
```
```python
// InputStreamReader 按照字节读取，一个汉字为2个字节
```
```python
// BufferedReader 可以整行读取，效率更高
```
```python
BufferedReader reader =
```
```python
new
```
```python
BufferedReader(
```
```python
new
```
```python
InputStreamReader( input,
```
```python
"UTF-8"
```
```python
) );
      String termString;
      numDocs = Integer.parseInt(reader.readLine());
```
```python
//文件中的第一行为numDocs
```
```python
while
```
```python
( (termString = reader.readLine()) !=
```
```python
null
```
```python
) {
```
```python
// lexion 中每一行，分别是id, name, tf, df
```
```python
Word t = buildWord( termString );
```
```python
// 调用[6]buildWord方法
```
```python
if
```
```python
( t !=
```
```python
null
```
```python
) {
          nameHash.put( t.name, t);
```
```python
// 对于每一个词，都存入idHash和nameHash
```
```python
idHash.put( t.id, t);
        }
      }
      reader.close();
```
```python
//关闭reader
```
```python
}
```
```python
catch
```
```python
(UnsupportedEncodingException e) {
```
```python
return
```
```python
false
```
```python
;
    }
```
```python
catch
```
```python
(IOException e) {
```
```python
return
```
```python
false
```
```python
;
    }
```
```python
return
```
```python
true
```
```python
;
  }
```
### [9] public Lexicon map( Map《Integer, Integer》 translation )
**用于根据映射表来生成新的词典。**
```python
/**
   * 紧缩词典，利用一个map把原来编号为key的word变为编号为value的word，去掉不在key
   * 中的word
   *
```
```python
@param
```
```python
translation 影射表
   */
```
```python
public
```
```python
Lexicon
```
```python
map
```
```python
( Map<Integer, Integer> translation ) {
```
```python
// 根据映射表生成新的词典
```
```python
// 映射表的左边为该词在现有词典中的id
```
```python
// 映射表的右边为该词在新词典中的id
```
```python
// 注意，新词典的size只与映射表的长度有关，与旧词典的size无关
```
```python
Lexicon newlex =
```
```python
new
```
```python
Lexicon();
    Hashtable<Integer, Word> newIdHash =
```
```python
new
```
```python
Hashtable<Integer, Word>();
    Hashtable<String, Word> newNameHash =
```
```python
new
```
```python
Hashtable<String, Word>();
```
```python
for
```
```python
( Entry<Integer, Integer> e : translation.entrySet()){
      Word w = idHash.get(e.getKey());
      Word nw = (Word) w.clone();
      nw.id = e.getValue();
      newIdHash.put(nw.id, nw);
      newNameHash.put(nw.getName(), nw);
    }
    newlex.idHash = newIdHash;
    newlex.nameHash = newNameHash;
    newlex.numDocs =
```
```python
this
```
```python
.numDocs;
```
```python
return
```
```python
newlex;
  }
```

