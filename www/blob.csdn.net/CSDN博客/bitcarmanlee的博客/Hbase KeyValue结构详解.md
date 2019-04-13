
# Hbase KeyValue结构详解 - bitcarmanlee的博客 - CSDN博客


2018年01月05日 12:18:33[bitcarmanlee](https://me.csdn.net/bitcarmanlee)阅读数：2218



## 1综述
Hbase是面向列的存储结构，而实际存储单元里存储的都是KeyValue结构。在看Hbase的API中，发现Hbase的API中就有KeyValue类，这个KeyValue类就是Hbase中数据存储的基本类型。那么这个KeyValue里到底包含哪些内容了？为了方便理解，特意为大家来剖析一下KeyValue类的结构。
## 2.KeyValue类源码
首先为了从大体上了解KeyValue结构，我们可以先看看KeyValue类中源码上的关于此类的注释。不得不说，老外这点做得很好，基本会用言简意赅的语言来概括或者描述此部分代码。
```python
/**
 * An HBase
```
```python
Key
```
```python
/Value. This
```
```python
is
```
```python
the fundamental HBase Type.  
 * ...
 * KeyValue wraps a
```
```python
byte
```
```python
array
```
```python
and
```
```python
takes offsets
```
```python
and
```
```python
lengths
```
```python
into
```
```python
passed array at
```
```python
where
```
```python
to
```
```python
start
 * interpreting the content
```
```python
as
```
```python
KeyValue. The KeyValue format inside a
```
```python
byte
```
```python
array
```
```python
is
```
```python
:
 * <code>&lt;keylength> &lt;valuelength> &lt;
```
```python
key
```
```python
> &lt;value></code>
```
```python
Key
```
```python
is
```
```python
further decomposed
```
```python
as
```
```python
:
 * <code>&lt;rowlength> &lt;row> &lt;columnfamilylength> &lt;columnfamily> &lt;columnqualifier>
 * &lt;timestamp> &lt;keytype></code>
```
这段注释就很关键信息量也很大：
1.第一句话就告诉了我们，KeyValue是hbase中的基础类，很重要。
2.后面那部分的信息就很关键了，其实就告诉了我们KeyValue中的结构：
KeyValue的整体结构为：
keylength  valuelength key value
而Key的结构为：
rowlength   row  columnfamilylength  columnfamily    columnqualifier     timestamp   keytype
![这里写图片描述](https://img-blog.csdn.net/20180105104700561?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvYml0Y2FybWFubGVl/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
如果用一张图来形象描述，如上图所示，可以清楚表示KeyValue内部的存储结构！
根据注释里的原文，我们还可以得知：HBase的KeyValue内部维护着一个字节数组，然后通过不同的偏移量来获取不同的部分。
## 3.进一步细节
在KeyValue中，其中的KeyLength为4B：
```python
/** Size of the key length field in bytes*/
```
```python
public
```
```python
static
```
```python
final
```
```python
int
```
```python
KEY_LENGTH_SIZE = Bytes.SIZEOF_INT;
```
ValueLength标识Value在字节数组中所占的长度，为4B：
```python
public
```
```python
static
```
```python
final
```
```python
int
```
```python
ROW_OFFSET =
    Bytes.SIZEOF_INT
```
```python
/*keylength*/
```
```python
+
    Bytes.SIZEOF_INT
```
```python
/*valuelength*/
```
```python
;
```
Column Family Length:储列簇Column Family的长度，为1B
```python
/** Size of the family length field in bytes */
```
```python
public
```
```python
static
```
```python
final
```
```python
int
```
```python
FAMILY_LENGTH_SIZE = Bytes.SIZEOF_BYTE;
```
Row Length：存储rowkey的长度，为2B
```python
/** Size of the row length field in bytes */
```
```python
public
```
```python
static
```
```python
final
```
```python
int
```
```python
ROW_LENGTH_SIZE = Bytes.SIZEOF_SHORT;
```
TimeStamp是Long型，肯定就是占8B了：
```python
/** Size of the timestamp field in bytes */
```
```python
public
```
```python
static
```
```python
final
```
```python
int
```
```python
TIMESTAMP_SIZE = Bytes.SIZEOF_LONG;
```
KeyType为1B：
```python
/** Size of the key type field in bytes */
```
```python
public
```
```python
static
```
```python
final
```
```python
int
```
```python
TYPE_SIZE = Bytes.SIZEOF_BYTE;
```
从ColumnQualifier开始内容前面不在带有长度了。TimeStamp和KeyType因为所占的长度是固定的，所以不用包含长度信息。而Qualifier的长度信息，则用以下的方式可以得出：
```python
// Size of the length shorts and bytes in key.
```
```python
public
```
```python
static
```
```python
final
```
```python
int
```
```python
KEY_INFRASTRUCTURE_SIZE = ROW_LENGTH_SIZE
      + FAMILY_LENGTH_SIZE + TIMESTAMP_TYPE_SIZE;
```
```python
/**
   *
```
```python
@return
```
```python
Qualifier length
   */
```
```python
@Override
```
```python
public
```
```python
int
```
```python
getQualifierLength
```
```python
() {
```
```python
return
```
```python
getQualifierLength(getRowLength(),getFamilyLength());
  }
```
```python
/**
   *
```
```python
@return
```
```python
Qualifier length
   */
```
```python
private
```
```python
int
```
```python
getQualifierLength
```
```python
(
```
```python
int
```
```python
rlength,
```
```python
int
```
```python
flength) {
```
```python
return
```
```python
getKeyLength() - (
```
```python
int
```
```python
) getKeyDataStructureSize(rlength, flength,
```
```python
0
```
```python
);
  }
```
## 4.KeyValue的实现
在Hbase中，所有数据都是以Byte的形式存在的。在KeyValue中，使用的是byte数组来存储实际内容。
```python
///
```
```python
/
```
```python
// KeyValue core instance fields.
```
```python
private
```
```python
byte
```
```python
[] bytes =
```
```python
null
```
```python
;
```
```python
// an immutable byte array that contains the KV
```
```python
private
```
```python
int
```
```python
offset =
```
```python
0
```
```python
;
```
```python
// offset into bytes buffer KV starts at
```
```python
private
```
```python
int
```
```python
length =
```
```python
0
```
```python
;
```
```python
// length of the KV starting from offset.
```
其中，bytes数组用来存储KeyValue中的实际内容，offset表示KeyValue在数组bytes中的起始位置，length则是KeyValue在数组bytes中自起始位置offset后的长度。
KeyValue提供了一系列的Offset方法在数组中定位各个字段的的起始位置，如getValueOffset，getRowOffset等。也提供了一系列的length方法来获取KeyValue中各个字段的大小。

