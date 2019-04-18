# 解决oracle自增长sequence失效的问题 - z69183787的专栏 - CSDN博客
2013年12月17日 12:01:57[OkidoGreen](https://me.csdn.net/z69183787)阅读数：2837
如果oracle程序没有按照hibernater设置的sequence自增长。可以试试这样。
在@SequenceGenerator中加入allocationSize = 1就可以了
## @SequenceGenerator
如果使用 [@GeneratedValue](http://www.oracle.com/technology/global/cn/products/ias/toplink/jpa/resources/toplink-jpa-annotations.html#GeneratedValue) 批注指定一个 `SEQUENCE` 类型的主键生成器，则可以使用 `@SequenceGenerator` 批注微调该主键生成器以：
- 
更改分配大小以匹配应用程序要求或数据库性能参数
- 
更改初始值以匹配现有的数据模型（例如，如果基于已经为其分配或保留了一组主键值的现有数据集构建）
- 
使用现有数据模型中预定义的序列
[表 1-42](http://www.oracle.com/technology/global/cn/products/ias/toplink/jpa/resources/toplink-jpa-annotations.html#CHDBBBBA) 列出了此批注的属性``。有关更多详细信息，请参阅 。有关更多详细信息，请参阅 [API](http://java.sun.com/javaee/5/docs/api/javax/persistence/SequenceGenerator.html)。
表 1-42 @SequenceGenerator 属性
|属性|必需|说明|
|----|----|----|
|`name`|![必需](http://www.oracle.com/technology/products/ias/toplink/jpa/resources/required.gif)|`SequenceGenerator` 的名称必须匹配其 `startegy` 设置为 `SEQUENCE` 的 `GeneratedValue` 的名称。|
|`allocationSize`|![可选](http://www.oracle.com/technology/products/ias/toplink/jpa/resources/optional.gif)|默认值： 50.默认情况下，JPA 持续性提供程序使用的分配大小为 50。如果此分配大小与应用程序要求或数据库性能参数不匹配，请将 `allocationSize` 设置为所需的 `int` 值。|
|`initialValue`|![可选](http://www.oracle.com/technology/products/ias/toplink/jpa/resources/optional.gif)|默认值： 0.默认情况下，JPA 持续性提供程序假设持续性提供程序将所有主键值的起始值设置为 0。如果这与现有数据模型不匹配，请将 `initialValue` 设置为所需的 `int` 值。|
|`sequenceName`|![可选](http://www.oracle.com/technology/products/ias/toplink/jpa/resources/optional.gif)|默认值：JPA 持续性提供程序分配它自己创建的序列名。如果要使用事先存在或预定义的序列，请将 `sequenceName` 设置为所需的 `String` 名称。|
