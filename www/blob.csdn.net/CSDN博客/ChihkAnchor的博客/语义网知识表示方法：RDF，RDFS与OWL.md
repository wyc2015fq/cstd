# 语义网知识表示方法：RDF，RDFS与OWL - ChihkAnchor的博客 - CSDN博客





2019年02月25日 22:55:58[Chihk-Anchor](https://me.csdn.net/weixin_40871455)阅读数：201








RDF，RDFS与OWL都是基于语义网的知识表示框架，还有一种知识表示框架称为分布式表示，即把知识表示为低维稠密向量，例如TransE模型

对于初学者来讲，这个概念很容易和语义网（Semantic Web）相混淆。语义网络指Semantic Network，语义网指Semantic Web。

语义网络是由Quillian于上世纪60年代提出的知识表达模式，其用相互连接的节点和边来表示知识。节点表示对象、概念，边表示节点之间的关系。

语义网络的优点： 

1. 容易理解和展示。 

2. 相关概念容易聚类。

语义网络的缺点： 

1. 节点和边的值没有标准，完全是由用户自己定义。 

2. 多源数据融合比较困难，因为没有标准。 

3. 无法区分概念节点和对象节点。 

4. 无法对节点和边的标签进行定义。

### W3C推荐的语义网（Semantic Web）技术标准栈

![](https://img-blog.csdnimg.cn/20190225104350576.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3dlaXhpbl80MDg3MTQ1NQ==,size_16,color_FFFFFF,t_70)



### RDF表现形式

RDF(Resource Description Framework)，即资源描述框架，其本质是一个数据模型（Data Model）。

描述什么资源，用什么手段描述这些资源，这就需要我们明白R、D、F分别是什么意思，看下图：

![](https://img-blog.csdnimg.cn/2019022511104990.png)

它提供了一个统一的标准，用于描述实体/资源。简单来说，就是表示事物的一种方法和手段。RDF形式上表示为SPO三元组，有时候也称为一条语句，知识图谱中我们也称其为一条知识：![](https://img-blog.csdnimg.cn/20190225111305687.png)

RDF由节点和边组成，节点表示实体/资源、属性，边则表示了实体和实体之间的关系以及实体和属性的关系。RDF的提出解决了语义网络的缺点1和缺点2，在节点和边的取值上做了约束，制定了统一标准，为多源数据的融合提供了便利。

### RDF序列化方法

RDF的表示形式和类型有了，那我们如何创建RDF数据集，将其序列化（Serialization）呢？换句话说，就是我们怎么存储和传输RDF数据。目前，RDF序列化的方式主要有：RDF/XML，N-Triples，Turtle，RDFa，JSON-LD等几种。

RDF/XML，顾名思义，就是用XML的格式来表示RDF数据。之所以提出这个方法，是因为XML的技术比较成熟，有许多现成的工具来存储和解析XML。然而，对于RDF来说，XML的格式太冗长，也不便于阅读，通常我们不会使用这种方式来处理RDF数据。

N-Triples，即用多个三元组来表示RDF数据集，是最直观的表示方法。在文件中，每一行表示一个三元组，方便机器解析和处理。开放领域知识图谱DBpedia通常是用这种格式来发布数据的。

Turtle, 应该是使用得最多的一种RDF序列化方式了。它比RDF/XML紧凑，且可读性比N-Triples好。

RDFa, 即“The Resource Description Framework in Attributes”，是HTML5的一个扩展，在不改变任何显示效果的情况下，让网站构建者能够在页面中标记实体，像人物、地点、时间、评论等等。也就是说，将RDF数据嵌入到网页中，搜索引擎能够更好的解析非结构化页面，获取一些有用的结构化信息。读者可以去这个页面感受一下RDFa，其直观展示了普通用户看到的页面，浏览器看到的页面和搜索引擎解析出来的结构化信息。

JSON-LD，即“JSON for Linking Data”，用键值对的方式来存储RDF数据。



下面，给出罗纳尔多知识图谱其N-Triples和Turtle的具体表示。

```
Example1 N-Triples:

<http://www.kg.com/person/1> <http://www.kg.com/ontology/chineseName> "罗纳尔多·路易斯·纳萨里奥·德·利马"^^string.
<http://www.kg.com/person/1> <http://www.kg.com/ontology/career> "足球运动员"^^string.
<http://www.kg.com/person/1> <http://www.kg.com/ontology/fullName> "Ronaldo Luís Nazário de Lima"^^string.
<http://www.kg.com/person/1> <http://www.kg.com/ontology/birthDate> "1976-09-18"^^date.
<http://www.kg.com/person/1> <http://www.kg.com/ontology/height> "180"^^int.
<http://www.kg.com/person/1> <http://www.kg.com/ontology/weight> "98"^^int.
<http://www.kg.com/person/1> <http://www.kg.com/ontology/nationality> "巴西"^^string.
<http://www.kg.com/person/1> <http://www.kg.com/ontology/hasBirthPlace> <http://www.kg.com/place/10086>.
<http://www.kg.com/place/10086> <http://www.kg.com/ontology/address> "里约热内卢"^^string.
<http://www.kg.com/place/10086> <http://www.kg.com/ontology/coordinate> "-22.908333, -43.196389"^^string.
```

用Turtle表示的时候我们会加上前缀（Prefix）对RDF的IRI进行缩写。

```
Example2 Turtle:

@prefix person: <http://www.kg.com/person/> .
@prefix place: <http://www.kg.com/place/> .
@prefix : <http://www.kg.com/ontology/> .

person:1 :chineseName "罗纳尔多·路易斯·纳萨里奥·德·利马"^^string.
person:1 :career "足球运动员"^^string.
person:1 :fullName "Ronaldo Luís Nazário de Lima"^^string.
person:1 :birthDate "1976-09-18"^^date.
person:1 :height "180"^^int. 
person:1 :weight "98"^^int.
person:1 :nationality "巴西"^^string. 
person:1 :hasBirthPlace place:10086.
place:10086 :address "里约热内卢"^^string.
place:10086 :address "-22.908333, -43.196389"^^string.
```

同一个实体拥有多个属性（数据属性）或关系（对象属性），我们可以只用一个subject来表示，使其更紧凑。我们可以将上面的Turtle改为：

```
Example3 Turtle:

@prefix person: <http://www.kg.com/person/> .
@prefix place: <http://www.kg.com/place/> .
@prefix : <http://www.kg.com/ontology/> .

person:1 :chineseName "罗纳尔多·路易斯·纳萨里奥·德·利马"^^string;
         :career "足球运动员"^^string;
         :fullName "Ronaldo Luís Nazário de Lima"^^string;
         :birthDate "1976-09-18"^^date;
         :height "180"^^int;
         :weight "98"^^int;
         :nationality "巴西"^^string; 
         :hasBirthPlace place:10086.
place:10086 :address "里约热内卢"^^string;
            :address "-22.908333, -43.196389"^^string.
```

即，将一个实体用一个句子表示（这里的句子指的是一个英文句号“.”）而不是多个句子，属性间用分号隔开。

### RDF的表达能力

RDF的表达能力有限，无法区分类和对象，也无法定义和描述类的关系/属性。RDF是对具体事物的描述，缺乏抽象能力，无法对同一个类别的事物进行定义和描述。就以罗纳尔多这个知识图为例，RDF能够表达罗纳尔多和里约热内卢这两个实体具有哪些属性，以及它们之间的关系。但如果我们想定义罗纳尔多是人，里约热内卢是地点，并且人具有哪些属性，地点具有哪些属性，人和地点之间存在哪些关系，这个时候RDF就表示无能为力了。不论是在智能的概念上，还是在现实的应用当中，这种泛化抽象能力都是相当重要的；同时，这也是知识图谱本身十分强调的。RDFS和OWL这两种技术或者说模式语言/本体语言（schema/ontology language）解决了RDF表达能力有限的困境。

### RDFS

RDFS/OWL本质上是一些预定义词汇（vocabulary）构成的集合，用于对RDF进行类似的类定义及其属性的定义。

![](https://img-blog.csdnimg.cn/20190225114850129.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3dlaXhpbl80MDg3MTQ1NQ==,size_16,color_FFFFFF,t_70)

### RDFS序列化方式

和RDF没什么不同，其实在表现形式上，它们就是RDF。其常用的方式主要是RDF/XML，Turtle。另外，通常我们用小写开头的单词或词组来表示属性，大写开头的表示类。数据属性（data property，实体和literal字面量的关系）通常由名词组成，而对象数据（object property，实体和实体之间的关系）通常由动词（has，is之类的）加名词组成。剩下的部分符合驼峰命名法。

还是以罗纳尔多知识图为例，我们在概念、抽象层面对RDF数据进行定义。下面的RDFS定义了人和地点这两个类，及每个类包含的属性。

```
@prefix rdfs: <http://www.w3.org/2000/01/rdf-schema#> .
@prefix rdf: <http://www.w3.org/1999/02/22-rdf-syntax-ns#> .
@prefix : <http://www.kg.com/ontology/> .

### 这里我们用词汇rdfs:Class定义了“人”和“地点”这两个类。
:Person rdf:type rdfs:Class.
:Place rdf:type rdfs:Class.

### rdfs当中不区分数据属性和对象属性，词汇rdf:Property定义了属性，即RDF的“边”。
:chineseName rdf:type rdf:Property;
        rdfs:domain :Person;
        rdfs:range xsd:string .

:career rdf:type rdf:Property;
        rdfs:domain :Person;
        rdfs:range xsd:string .

:fullName rdf:type rdf:Property;
        rdfs:domain :Person;
        rdfs:range xsd:string .

:birthDate rdf:type rdf:Property;
        rdfs:domain :Person;
        rdfs:range xsd:date .

:height rdf:type rdf:Property;
        rdfs:domain :Person;
        rdfs:range xsd:int .

:weight rdf:type rdf:Property;
        rdfs:domain :Person;
        rdfs:range xsd:int .

:nationality rdf:type rdf:Property;
        rdfs:domain :Person;
        rdfs:range xsd:string .

:hasBirthPlace rdf:type rdf:Property;
        rdfs:domain :Person;
        rdfs:range :Place .

:address rdf:type rdf:Property;
        rdfs:domain :Place;
        rdfs:range xsd:string .

:coordinate rdf:type rdf:Property;
        rdfs:domain :Place;
        rdfs:range xsd:string .
```

我们这里只介绍RDFS几个比较重要，常用的词汇： 

1. rdfs:Class. 用于定义类。 

2. rdfs:domain. 用于表示该属性属于哪个类别，即主语的取值。 

3. rdfs:range. 用于描述该属性的取值类型，即宾语的取值。 

4. rdfs:subClassOf. 用于描述该类的父类。比如，我们可以定义一个运动员类，声明该类是人的子类。 

5. rdfs:subProperty. 用于描述该属性的父属性。比如，我们可以定义一个名称属性，声明中文名称和全名是名称的子类。

其实rdf:Property和rdf:type也是RDFS的词汇，因为RDFS本质上就是RDF词汇的一个扩展。RDFS其他的词汇及其用法请参考W3C官方文档。[https://www.w3.org/TR/rdf-schema/](https://www.w3.org/TR/rdf-schema/)

## OWL（Web Ontology Language）

RDFS本质上是RDF词汇的一个扩展。前面我们知道，通过RDF(S)可以表达一些简单的语义，但在更复杂的场景下，RDF(S)语义表达能力显得太弱，还缺少诸多常用的特征。包括对局部值域的属性定义，类、属性、个体的等价性，不相交类的定义，基数约束，关于属性特征的描述等。因此W3C提出了OWL语言扩展RDF(S)，作为语义网上表示本体的推荐语言。我们也可以把OWL当做是RDFS的一个扩展，其添加了额外的预定义词汇。

OWL，即“Web Ontology Language”，语义网技术栈的核心之一。OWL有两个主要的功能： 

1. 提供快速、灵活的数据建模能力。 

2. 高效的自动推理。

我们先谈如何利用OWL进行数据建模。用OWL对罗纳尔多知识图进行语义层的描述：

```
@prefix rdfs: <http://www.w3.org/2000/01/rdf-schema#> .
@prefix rdf: <http://www.w3.org/1999/02/22-rdf-syntax-ns#> .
@prefix : <http://www.kg.com/ontology/> .
@prefix owl: <http://www.w3.org/2002/07/owl#> .

### 这里我们用词汇owl:Class定义了“人”和“地点”这两个类。
:Person rdf:type owl:Class.
:Place rdf:type owl:Class.

### owl区分数据属性和对象属性（对象属性表示实体和实体之间的关系）。
### 词汇owl:DatatypeProperty定义了数据属性，owl:ObjectProperty定义了对象属性。
:chineseName rdf:type owl:DatatypeProperty;
        rdfs:domain :Person;
        rdfs:range xsd:string .

:career rdf:type owl:DatatypeProperty;
        rdfs:domain :Person;
        rdfs:range xsd:string .

:fullName rdf:type owl:DatatypeProperty;
        rdfs:domain :Person;
        rdfs:range xsd:string .

:birthDate rdf:type owl:DatatypeProperty;
        rdfs:domain :Person;
        rdfs:range xsd:date .

:height rdf:type owl:DatatypeProperty;
        rdfs:domain :Person;
        rdfs:range xsd:int .

:weight rdf:type owl:DatatypeProperty;
        rdfs:domain :Person;
        rdfs:range xsd:int .

:nationality rdf:type owl:DatatypeProperty;
        rdfs:domain :Person;
        rdfs:range xsd:string .

:hasBirthPlace rdf:type owl:ObjectProperty;
        rdfs:domain :Person;
        rdfs:range :Place .

:address rdf:type owl:DatatypeProperty;
        rdfs:domain :Place;
        rdfs:range xsd:string .

:coordinate rdf:type owl:DatatypeProperty;
        rdfs:domain :Place;
        rdfs:range xsd:string .
```

罗纳尔多这个例子不能展现OWL丰富的表达能力，我们这里简单介绍一下常用的词汇：

描述属性特征的词汇 

1. owl:TransitiveProperty. 表示该属性具有传递性质。例如，我们定义“位于”是具有传递性的属性，若A位于B，B位于C，那么A肯定位于C。 

2. owl:SymmetricProperty. 表示该属性具有对称性。例如，我们定义“认识”是具有对称性的属性，若A认识B，那么B肯定认A。 

3. owl:FunctionalProperty. 表示该属性取值的唯一性。 例如，我们定义“母亲”是具有唯一性的属性，若A的母亲是B，在其他地方我们得知A的母亲是C，那么B和C指的是同一个人。 

4. owl:inverseOf. 定义某个属性的相反关系。例如，定义“父母”的相反关系是“子女”，若A是B的父母，那么B肯定是A的子女。

本体映射词汇（Ontology Mapping） 

1. owl:equivalentClass. 表示某个类和另一个类是相同的。 

2. owl:equivalentProperty. 表示某个属性和另一个属性是相同的。 

3. owl:sameAs. 表示两个实体是同一个实体。

本体映射主要用在融合多个独立的Ontology（Schema）。举个例子，张三自己构建了一个本体结构，其中定义了Person这样一个类来表示人；李四则在自己构建的本体中定义Human这个类来表示人。当我们融合这两个本体的时候，就可以用到OWL的本体映射词汇。如果没有OWL，我们将无法融合知识图谱。

```
http://www.zhangsan.com/ontology/Person rdf:type owl:Class . 
http://www.lisi.com/ontology/Human rdf:type owl:Class . 
http://www.zhangsan.com/ontology/Person owl:equivalentClass http://www.lisi.com/ontology/Human .
```

更多的OWL词汇和特性请参考[W3C官网文档](https://www.w3.org/TR/2004/REC-owl-features-20040210/)。

知识图谱是由本体（Ontology）作为Schema层，和RDF数据模型兼容的结构化数据集。知识图谱的推理主要分为两类：**基于本体**的推理和**基于规则**的推理。

我们这里谈的是基于本体的推理。读者应该发现，上面所介绍的属性特征词汇其实就创造了对RDF数据进行推理的前提。此时，我们加入支持OWL推理的推理机（reasoner），就能够执行基于本体的推理了。RDFS同样支持推理，由于缺乏丰富的表达能力，推理能力也不强。举个例子，我们用RDFS定义人和动物两个类，另外，定义人是动物的一个子类。此时推理机能够推断出一个实体若是人，那么它也是动物。OWL当然支持这种基本的推理，除此之外，凭借其强大的表达能力，我们能进行更有实际意义的推理。想象一个场景，我们有一个庞大数据库存储人物的亲属关系。里面很多关系都是单向的，比如，其只保存了A的父亲（母亲）是B，但B的子女字段里面没有A，如果在只有单个关系，数据量不多的情况下，我们尚能人工的去补全这种关系。如果在关系种类上百，人物上亿的情况下，我们如何处理？当进行关系修改，添加，删除等操作的时候，该怎么处理？这种场景想想就会让人崩溃。如果我们用inversOf来表示hasParent和hasChild互为逆关系

目前，OWL的最新版本是OWL 2，在兼容OWL的基础上添加了新的功能，有兴趣的读者可以查阅W3C文档。另外，OWL 2包含了三个标准，或者三种配置（Profile），它们是OWL 2完整标准（OWL 2/Full）的一个子集。读者目前不用考虑它们之间的差别，只有当我们要用到OWL自动推理功能的时候才需要考虑到底使用哪一种配置。且在大多数情况下，我们需要知道哪种配置才是最合适的。下面简单说说它们使用的场景：

OWL 2/EL 使用场景：本体结构中有大量相互链接的类和属性，设计者想用自动推理机得到里面复杂的关系。适合实例较多的情况，如医学领域。

OWL 2/QL 使用场景：有大量的实例数据。OWL 2 QL本体可以被改写为SQL查询，适用于使用OBDA（ontology based data access）的方式来访问关系数据库。也就是说我们不用显式地把关系数据库中的数据转为RDF，而是通过映射的方式，将数据库转为虚拟RDF图进行访问。

OWL 2/RL 使用场景：需要结合基于规则的推理引擎（rule-based reasoning engine）的场合。

再了解一下知识图谱相关的名词：
- 
**Knowledge Base**：通常翻译为“**知识库**”。知识库是人工智能的经典概念之一。最早是作为专家系统（Expert System）的组成部分，用于支持推理。知识库中的知识有很多种不同的形式，例如本体知识、关联性知识、规则库、案例知识等。相比于知识库的概念，知识图谱更加侧重关联性知识的构建，如三元组。

- 
**The Semantic Web ：**通常翻译为“**语义网**”或“**语义互联网**”，是Web之父Tim Berners Lee于1998年提出的。语义互联网的核心内涵是：Web不仅仅要通过超链接把文本页面链接起来，还应该把事物链接起来，使得搜索引擎可以直接对事物进行搜索，而不仅仅是对网页进行搜索。谷歌知识图谱是语义互联网这一理念的商业化实现。也可以把语义互联网看做是一个基于互联网共同构建的全球知识库。

- 
**Linked Data：**通常翻译为“**链接数据**”。是Tim Berners Lee于2006年提出，是为了强调语义互联网的目的是要建立数据之间的链接，而非仅仅是把结构化的数据发布到网上。他为建立数据之间的链接制定了四个原则。从理念上讲，链接数据最接近于知识图谱的概念。但很多商业知识图谱的具体实现并不一定完全遵循Tim所提出的那四个原则。

- 
**Semantic Net/ Semantic Network**：通常翻译为“**语义网络**”或“**语义网**”，这个翻译通常被与Semantic Web的翻译混淆起来，为了以示区别，这里采用“语义网络”的翻译。语义网络最早是1960年由认知科学家Allan M. Collins作为知识表示的一种方法提出。WordNet是最典型的语义网络。相比起知识图谱，早期的语义网络更加侧重描述概念以及概念之间的关系，而知识图谱更加强调数据或事物之间的链接。

- 
**Ontology：**通常翻译为“**本体**”。本体本身是个哲学名词。在上个世纪80年代，人工智能研究人员将这一概念引入了计算机领域。Tom Gruber把本体定义为“概念和关系的形式化描述”。通俗点讲，本体相似于数据库中的Schema，主要用来定义类和关系，以及类层次和关系层次等。OWL是最常用的本体描述语言。本体通常被用来为知识图谱定义Schema。




