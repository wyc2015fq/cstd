
# Clojure 学习入门（19）—— 数组 - 阳光岛主 - CSDN博客

2014年04月27日 22:30:31[阳光岛主](https://me.csdn.net/sunboy_2050)阅读数：8694


**1、创建数组**
### 1.1 从集合创建数组 into-array
into-array
> (into-array aseq)

> (into-array type aseq)
**示例：**
user=>(into-array [1 2 3])
\#<Integer[] [Ljava.lang.Integer;@4b0ea9ba>
user=>(seq (into-array [1 2 3]))
(1 2 3)
返回包含aset中所有元素的数组。数组的组件类型为type（如果指定了type），或aset中第一个元素的类型（如果aset不为空），或Object。aset中所有元素的类型必须兼容数组的组件类型。type可以使用基本类型的类对象，例如Integer/TYPE等。
Added in Clojure version 1.0
[Source](https://github.com/clojure/clojure/blob/c6756a8bab137128c8119add29a25b0a88509900/src/clj/clojure/core.clj#L3086)

(into-array aseq)
|1|(into-array [])    ; \#<Object[]> - []|
|2|(into-array [|1|2|3|])    ; \#<Long[]> - [|1|,|2|,|3|]，注：into-array返回的不是原生数组，而是引用数组|

**示例：**
user=>(into-array [])
\#<Object[] [Ljava.lang.Object;@26ae533a>
user=>(into-array Integer/TYPE [1 2 3])
\#<int[] [I@2cba5bdb>
user=>(into-array Long/TYPE [1 2 3])
\#<long[] [J@373c0b53>
user=>(seq (into-array Long/TYPE [1 2 3]))
(1 2 3)
(into-array type aseq)
|1|(into-array Integer/TYPE [|1|2|3|])     ; \#<|int|[]> - [|1|,|2|,|3|]|
|2|; 等效于(|int|-array [|1|2|3|])|



### 1.2 创建空数组 make-array
make-array
> (make-array type len)

> (make-array type dim & more-dims)
创建并返回一个指定类型、指定尺寸的数组。需要注意是类对象是必需的。类对象可以通过导入或完全限定名称获得。type可以使用基本类型的类对象，例如Integer/TYPE等。
Added in Clojure version 1.0
[Source](https://github.com/clojure/clojure/blob/c6756a8bab137128c8119add29a25b0a88509900/src/clj/clojure/core.clj#L3595)

(make-array type len)
|1|(make-array Integer/TYPE|10|)     ; \#<|int|[]> - [|0|,|0|,|0|,|0|,|0|,|0|,|0|,|0|,|0|,|0|]|
|2|; 等效于(|int|-array|10|)|

(make-array type dim & more-dims)
|1|(make-array Integer/TYPE|2|10|)     ;二维数组 \#<|int|[][]> - [[I|@4b8bc0fb|, [I|@442ac57c|]|
|2|(make-array Integer/TYPE|2|2|10|)     ;三维数组 \#<|int|[][][]> - [[[I|@17f7b44f|, [[I|@75ebad4|]|



### 1.3 创建基本类型数组 int-array
int-array
> (int-array size-or-seq)

> (int-array size init-val-or-seq)
创建一个int数组
Added in Clojure version 1.0
[Source](https://github.com/clojure/clojure/blob/c6756a8bab137128c8119add29a25b0a88509900/src/clj/clojure/core.clj#L4776)
**示例：**
user=>(int-array 10)
\#<int[] [I@6490832e>
user=>(seq (int-array 10))
(0 0 0 0 0 0 0 0 0 0)
user=>(double-array 10 1.24)
\#<double[] [D@2f87c55c>
user=>(seq (double-array 10 1.24))
(1.24 1.24 1.24 1.24 1.24 1.24 1.24 1.24 1.24 1.24)
同系列的全部方法
byte-array
short-array
int-array
long-array
float-array
double-array
char-array
boolean-array
object-array*
object-array只有(object-array size-or-seq)用法
(int-array size)
|1|(|int|-array|3|)     ; \#<|int|[]> - [|0|,|0|,|0|]|
|2|; 相当于(make-array Integer/TYPE|3|)|

(int-array seq)
|1|(|int|-array [|1|2|3|])     ; \#<|int|[]> - [|1|,|2|,|3|]|
|2|; 相当于(into-array Integer/TYPE [|1|2|3|])|

(int-array size init-val)
|1|(|int|-array|3|5|)     ; \#<|int|[]> - [|5|,|5|,|5|]|

(int-array size seq)
|1|(|int|-array|5|[|1|2|3|])     ; \#<|int|[]> - [|1|,|2|,|3|,|0|,|0|]|
|2|(|int|-array|5|[|1|2|3|4|5|6|7|])     ; \#<|int|[]> - [|1|,|2|,|3|,|4|,|5|]|



## 2、 访问和修改数组
### 2.1 访问数组的值 aget
aget
> (aget array idx)

> (aget array idx & idxs)
返回指定索引的值。可用于所有类型的Java数组。（等效于array[idx]）
Added in Clojure version 1.0
[Source](https://github.com/clojure/clojure/blob/c6756a8bab137128c8119add29a25b0a88509900/src/clj/clojure/core.clj#L3521)
|1|(let [arrs (into-array|
|2|[(|int|-array [|1|2|3|])|
|3|(|int|-array [|11|12|13|14|15|])])]|
|4|; \#<|int|[][]> [[|1|,|2|,|3|], [|11|,|12|,|13|,|14|,|15|]]|
|5|(println (aget arrs|0|2|))|
|6|;|3|
|7|(println (aget arrs|1|4|)))|
|8|;|15|


### 2.2 修改数组的值 aset
aset
> (aset array idx val)

> (aset array idx idx2 & idxv)
用指定的元素替代数组中索引位置上的元素。可用于引用类型的Java数组。返回val。
Added in Clojure version 1.0
[Source](https://github.com/clojure/clojure/blob/c6756a8bab137128c8119add29a25b0a88509900/src/clj/clojure/core.clj#L3532)
(aset array idx val)
|1|(let [arr (object-array [|"A"|"B"|"C"|"D"|"E"|])|
|2|; [A, B, C, D, E]|
|3|val (aset arr|2|"F"|)]|
|4|(println val))|
|5|; arr: [A, B, F, D, E]|
|6|; val: F|

示例：

```python
(let [arr (object-array ["A" "B" "C" "D" "E"])
      val (aget arr 2)
      val2 (aset arr 2 "F")
      ]
      (println val)  ; C
      (println val2) ; F
  )
```
(aset array idx idx2 & idxv)
|01|// Java|
|02|public|static|Object[][][] arr3d = {|
|03|{|
|04|{|
|05|"A"|,|"B"|,|"C"|,|"D"|,|"E"|
|06|},|
|07|{|
|08|"1"|,|"2"|,|"3"|,|"4"|,|"5"|
|09|}|
|10|},|
|11|{|
|12|{|
|13|'a'|,|'b'|,|'c'|,|'d'|,|'e'|
|14|},|
|15|{|
|16|11|,|12|,|13|,|14|,|15|
|17|}|
|18|}|
|19|};|// 用Clojure初始化多维数组实在是太蛋疼了|
|20|
|21|; clojure|
|22|(let [val1 (aset arr3d|0|0|0|"val1"|)|
|23|val2 (aset arr3d|0|1|0|"val2"|)|
|24|val3 (aset arr3d|1|0|0|"val3"|)|
|25|val4 (aset arr3d|1|1|0|"val4"|)]|
|26|(println (java.util.Arrays/toString (aget arr3d|0|0|)))|
|27|(println (java.util.Arrays/toString (aget arr3d|0|1|)))|
|28|(println (java.util.Arrays/toString (aget arr3d|1|0|)))|
|29|(println (java.util.Arrays/toString (aget arr3d|1|1|))))|
|30|; [val1, B, C, D, E]|
|31|; [val2,|2|,|3|,|4|,|5|]|
|32|; [val3, b, c, d, e]|
|33|; [val4,|12|,|13|,|14|,|15|]|

aset-int
> (aset-int array idx val)

> (aset-int array idx idx2 & idxv)
用指定的元素替代数组中索引位置上的元素。可用于int类型的Java数组。返回val。
Added in Clojure version 1.0
[Source](https://github.com/clojure/clojure/blob/c6756a8bab137128c8119add29a25b0a88509900/src/clj/clojure/core.clj#L3555)
同系列全部方法
aset-byte
aset-short
aset-int
aset-long
aset-float
aset-double
aset-char
aset-boolean
aset
### 2.3 计算数组长度 alength
alength
> (alength array)
返回Java数组的长度。可用于所有类型的数组。
Added in Clojure version 1.0
[Source](https://github.com/clojure/clojure/blob/c6756a8bab137128c8119add29a25b0a88509900/src/clj/clojure/core.clj#L3507)
|1|(alength (|char|-array|11|\a))     ;|11|
|2|; 等效于(count (|char|-array|11|\a))|


### 2.4 使用Clojure序列库
#### 2.4.1 通用版序列库
|1|; clojure.lang.LazySeq|
|2|; clojure.lang.LazySeq|


#### 2.4.2 数组专用序列库 amap, areduce
amap宏
> (amap a idx ret expr)
用表达式expr映射数组a。使用一个索引idx，和一个返回值ret，并将ret初始化为数组a的克隆，然后将ret的每一个元素应用于expr表达式，（用表达式的返回值替换ret当前值）最后返回新的ret数组。
Added in Clojure version 1.0
[Source](https://github.com/clojure/clojure/blob/c6756a8bab137128c8119add29a25b0a88509900/src/clj/clojure/core.clj#L4693)
|1|; 为数组的每个值加|2|
|2|(let [arr (|int|-array [|1|2|3|])|
|3|ret (amap arr idx ret (|do|
|4|(println|"="|(vec ret))|
|5|(+|2|(aget ret idx))))]|
|6|(println (vec ret)))|

**示例：**

```python
(let [arr (int-array [1 2 3])
      ret (amap arr idx ret (do (println "=" (vec ret))
                (+ 2 (aget ret idx)))
                )]
  (println (vec ret))
  )
;= [1 2 3]
;= [3 2 3]
;= [3 4 3]
;[3 4 5]
```
areduce宏
> (areduce a idx ret init expr)
用表达式expr归纳数组a。使用一个索引idx，和一个返回值ret，将ret初始化为init，将ret设置为expr表达式每一步的计算结果，最后返回ret。
Added in Clojure version 1.0
[Source](https://github.com/clojure/clojure/blob/c6756a8bab137128c8119add29a25b0a88509900/src/clj/clojure/core.clj#L4709)
|1|; 计算数组[|1|,|2|, ..,|100|]每个元素的和|
|2|(let [arr (|int|-array (range|1|101|))|
|3|ret (areduce arr idx ret|0|(+ ret (aget arr idx)))]|
|4|(println ret))|
|5|;|5050|



## 3、 其他
### 3.1 类型转换 ints
bytes`Casts to bytes[]` Added in Clojure version 1.1
shorts`Casts to shorts[]` Added in Clojure version 1.1
ints`Casts to int[]` Added in Clojure version 1.0
longs`Casts to long[]` Added in Clojure version 1.0
floats`Casts to float[]` Added in Clojure version 1.0
doubles`Casts to double[]` Added in Clojure version 1.0
chars`Casts to chars[]` Added in Clojure version 1.1
booleans`Casts to boolean[]` Added in Clojure version 1.1
引用类型可以用to-array
### 3.2 数组克隆 aclone
aclone
> (aclone array)
返回Java数组的拷贝。可用于已知类型的数组。
Added in Clojure version 1.0
[Source](https://github.com/clojure/clojure/blob/c6756a8bab137128c8119add29a25b0a88509900/src/clj/clojure/core.clj#L3514)

## 4、总结
生活中一怕没有选择，二怕选择过多。创建数组的方法太多也会使思维混乱。一般而言对于引用类型的对象，使用集合容器远优于使用数组。所以使用数组的场景就只有处理基本类型数据（并且对性能有一定要求）的情况了，因此个人推荐优先使用int-array等方法创建数组。
花了一天时间，翻了好几本书，几乎找遍了与数组相关的所有方法，希望没有遗漏的。Clojure数组专题到此为止，之后会继续以专题的形式总结其他的API。
========2013/11/4
Clojure操作多维数组的aset, aget性能简直让人无法直视，用来写算法还是算了吧。
========2013/12/8
直接使用into-array函数返回的不是原生数组，而是一个引用数组。如果要用into-array创建原生数组，必须指定类型（第一个参数）。之前没注意这个，在《The Joy Of Clojure》中看到。

**附录：**
user=> (find-doc "-array")

```python
user=> (find-doc "-array")
-------------------------
clojure.core/boolean-array
([size-or-seq] [size init-val-or-seq])
  Creates an array of booleans
-------------------------
clojure.core/byte-array
([size-or-seq] [size init-val-or-seq])
  Creates an array of bytes
-------------------------
clojure.core/char-array
([size-or-seq] [size init-val-or-seq])
  Creates an array of chars
-------------------------
clojure.core/double-array
([size-or-seq] [size init-val-or-seq])
  Creates an array of doubles
-------------------------
clojure.core/float-array
([size-or-seq] [size init-val-or-seq])
  Creates an array of floats
-------------------------
clojure.core/int-array
([size-or-seq] [size init-val-or-seq])
  Creates an array of ints
-------------------------
clojure.core/into-array
([aseq] [type aseq])
  Returns an array with components set to the values in aseq. The array's
  component type is type if provided, or the type of the first value in
  aseq if present, or Object. All values in aseq must be compatible with
  the component type. Class objects for the primitive types can be obtained
  using, e.g., Integer/TYPE.
-------------------------
clojure.core/long-array
([size-or-seq] [size init-val-or-seq])
  Creates an array of longs
-------------------------
clojure.core/make-array
([type len] [type dim & more-dims])
  Creates and returns an array of instances of the specified class of
  the specified dimension(s).  Note that a class object is required.
  Class objects can be obtained by using their imported or
  fully-qualified name.  Class objects for the primitive types can be
  obtained using, e.g., Integer/TYPE.
-------------------------
clojure.core/short-array
([size-or-seq] [size init-val-or-seq])
  Creates an array of shorts
-------------------------
clojure.core/to-array
([coll])
  Returns an array of Objects containing the contents of coll, which
  can be any Collection.  Maps to java.util.Collection.toArray().
-------------------------
clojure.core/to-array-2d
([coll])
  Returns a (potentially-ragged) 2-dimensional array of Objects
  containing the contents of coll, which can be any Collection of any
  Collection.
nil
```

**参考推荐：**
[Clojure API之数组操作](http://my.oschina.net/u/580483/blog/159989#OSC_h2_1)



