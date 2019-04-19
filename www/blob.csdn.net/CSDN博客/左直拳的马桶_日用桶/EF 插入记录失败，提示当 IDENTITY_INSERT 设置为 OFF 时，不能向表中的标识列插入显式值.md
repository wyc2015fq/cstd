# EF:插入记录失败，提示当 IDENTITY_INSERT 设置为 OFF 时，不能向表中的标识列插入显式值 - 左直拳的马桶_日用桶 - CSDN博客
2014年07月03日 18:56:04[左直拳](https://me.csdn.net/leftfist)阅读数：7992
                EF忽然报错：
增加记录时，提示当 IDENTITY_INSERT 设置为 OFF 时，不能向表 '。。。' 中的标识列插入显式值。
真奇怪，添加记录方法一向好地地，从没出过错。要出错，那也是Update时才会，什么“附着不附着”的问题。
从提示信息来看，是向标识列插入了值。那么EF为什么会认不出标识列呢？
原来，是因为同事首先生成了EF代码，然后再在数据库中将某字段改成了标识列，改完以后，没有刷新EF代码，因此EF没有认出该
字段为标识列。
在.edmx文件里，查看其代码，对实体类有如下描述：
```
<EntityType Name="Status_Info">
          <Key>
            <PropertyRef Name="ID" />
          </Key>
          <Property Name="ID" Type="int" StoreGeneratedPattern="Identity" Nullable="false" />
          <Property Name="StatusID" Type="int" />
          <Property Name="Color" Type="nchar" MaxLength="10" />
        </EntityType>
```
是否标识列，一目了然。
教训：**数据库更改，一定要刷新EF代码。**
