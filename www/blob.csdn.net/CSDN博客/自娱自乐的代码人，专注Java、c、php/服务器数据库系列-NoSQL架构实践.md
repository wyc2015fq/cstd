# 服务器数据库系列 - NoSQL架构实践 - 自娱自乐的代码人，专注Java、c、php - CSDN博客





2013年04月01日 09:14:36[initphp](https://me.csdn.net/initphp)阅读数：1504








NoSQL架构实践（一）——以NoSQL为辅-zt

怎么样把NoSQL引入到我们的系统架构设计中，需要根据我们系统的业务场景来分析，什么样类型的数据适合存储在NoSQL数据库中，什么样类型的数据必须使用关系数据库存储。明确引入的NoSQL数据库带给系统的作用，它能解决什么问题，以及可能带来的新的问题。下面我们分析几种常见的NoSQL架构。

（一）NoSQL作为镜像

不改变原有的以MySQL作为存储的架构，使用NoSQL作为辅助镜像存储，用NoSQL的优势辅助提升性能。

![](https://img-my.csdn.net/uploads/201304/01/1364779002_2070.jpg)



**图 1 -NoSQL为镜像（代码完成模式 ）**

//写入数据的示例伪代码 

//data为我们要存储的数据对象 

data.title=”title”; 

data.name=”name”; 

data.time=”2009-12-01 10:10:01”; 

data.from=”1”; 

id=DB.Insert(data);//写入MySQL数据库 

NoSQL.Add(id,data);//以写入MySQL产生的自增id为主键写入NoSQL数据库

如果有数据一致性要求，可以像如下的方式使用

//写入数据的示例伪代码 

//data为我们要存储的数据对象 

bool status=false; 

DB.startTransaction();//开始事务 

id=DB.Insert(data);//写入MySQL数据库 

if(id>0){ 

    status=NoSQL.Add(id,data);//以写入MySQL产生的自增id为主键写入NoSQL数据库 

} 

if(id>0 && status==true){ 

    DB.commit();//提交事务 

}else{ 

    DB.rollback();//不成功，进行回滚 

}

上面的代码看起来可能觉得有点麻烦，但是只需要在DB类或者ORM层做一个统一的封装，就能实现重用了，其他代码都不用做任何的修改。

这种架构在原有基于MySQL数据库的架构上增加了一层辅助的NoSQL存储，代码量不大，技术难度小，却在可扩展性和性能上起到了非常大的作用。只需要程序在写入MySQL数据库后，同时写入到NoSQL数据库，让MySQL和NoSQL拥有相同的镜像数据，在某些可以根据主键查询的地方，使用高效的NoSQL数据库查询，这样就节省了MySQL的查询，用NoSQL的高性能来抵挡这些查询。
![](https://img-my.csdn.net/uploads/201304/01/1364779029_1937.jpg)


**图 2 -NoSQL为镜像（同步模式）**

这种不通过程序代码，而是通过MySQL把数据同步到NoSQL中，这种模式是上面一种的变体，是一种对写入透明但是具有更高技术难度一种模式。这种模式适用于现有的比较复杂的老系统，通过修改代码不易实现，可能引起新的问题。同时也适用于需要把数据同步到多种类型的存储中。

MySQL到NoSQL同步的实现可以使用MySQL UDF函数，MySQL binlog的解析来实现。可以利用现有的开源项目来实现，比如：

MySQL memcached UDFs：从通过UDF操作Memcached协议。

国内张宴开源的mysql-udf-http：通过UDF操作http协议。

有了这两个MySQL UDF函数库，我们就能通过MySQL透明的处理Memcached或者Http协议，这样只要有兼容Memcached或者Http协议的NoSQL数据库，那么我们就能通过MySQL去操作以进行同步数据。再结合lib_mysqludf_json，通过UDF和MySQL触发器功能的结合，就可以实现数据的自动同步。

（二）MySQL和NoSQL组合

MySQL中只存储需要查询的小字段，NoSQL存储所有数据。
![](https://img-my.csdn.net/uploads/201304/01/1364779053_1008.jpg)


**图 3 -MySQL和NoSQL组合**

//写入数据的示例伪代码 

//data为我们要存储的数据对象 

data.title=”title”; 

data.name=”name”; 

data.time=”2009-12-01 10:10:01”;

data.from=”1”;

bool status=false; 

DB.startTransaction();//开始事务 

id=DB.Insert(“INSERT INTO table (from) VALUES(data.from)”);//写入MySQL数据库,只写from需要where查询的字段

if(id>0){ 

    status=NoSQL.Add(id,data);//以写入MySQL产生的自增id为主键写入NoSQL数据库 

} 

if(id>0 && status==true){ 

    DB.commit();//提交事务 

}else{ 

    DB.rollback();//不成功，进行回滚 

}

把需要查询的字段，一般都是数字，时间等类型的小字段存储于MySQL中，根据查询建立相应的索引，其他不需要的字段，包括大文本字段都存储在NoSQL中。在查询的时候，我们先从MySQL中查询出数据的主键，然后从NoSQL中直接取出对应的数据即可。

这种架构模式把MySQL和NoSQL的作用进行了融合，各司其职，让MySQL专门负责处理擅长的关系存储，NoSQL作为数据的存储。它有以下优点：

节省MySQL的IO开销。由于MySQL只存储需要查询的小字段，不再负责存储大文本字段，这样就可以节省MySQL存储的空间开销，从而节省MySQL的磁盘IO。我们曾经通过这种优化，把MySQL一个40G的表缩减到几百M。

提高MySQl Query Cache缓存命中率。我们知道query cache缓存失效是表级的，在MySQL表一旦被更新就会失效，经过这种字段的分离，更新的字段如果不是存储在MySQL中，那么对query cache就没有任何影响。而NoSQL的Cache往往都是行级别的，只对更新的记录的缓存失效。

提升MySQL主从同步效率。由于MySQL存储空间的减小，同步的数据记录也减小了，而部分数据的更新落在NoSQL而不是MySQL，这样也减少了MySQL数据需要同步的次数。

提高MySQL数据备份和恢复的速度。由于MySQL数据库存储的数据的减小，很容易看到数据备份和恢复的速度也将极大的提高。

比以前更容易扩展。NoSQL天生就容易扩展。经过这种优化，MySQL性能也得到提高。

比如手机凤凰网就是这种架构[http://www.cnblogs.com/sunli/archive/2010/12/20/imcp.html](http://www.cnblogs.com/sunli/archive/2010/12/20/imcp.html)

总结

以NoSQL为辅的架构还是以MySQL架构的思想为中心，只是在以前的架构上辅助增加了NoSQL来提高其性能和可扩展性。这种架构实现起来比较容易，却能取得不错的效果。如果正想在项目中引入NoSQL，或者你的以MySQL架构的系统目前正出现相关的瓶颈，希望本文可以为你带来帮助。

NoSQL架构实践（二）——以NoSQL为主--ZT

前面一篇《NoSQL架构实践（一）——以NoSQL为辅》主要介绍了以NoSQL为辅助的架构，这种架构实施起来比较简单，易于理解，由于其中也使用了传统的关系数据库，让开发者更容易控制NoSQL带来的风险。接下来我们继续深入下去，换另外一个角度，“以NoSQL为主”来架构系统。

（三）纯NoSQL架构

只使用NoSQL作为数据存储。
![](https://img-my.csdn.net/uploads/201304/01/1364779082_7554.jpg)


**图 4-纯NoSQL架构**

在一些数据结构、查询关系非常简单的系统中，我们可以只使用NoSQL即可以解决存储问题。这样不但可以提高性能，还非常易于扩展。手机凤凰网的前端展示系统就使用了这种方案。

在一些数据库结构经常变化，数据结构不定的系统中，就非常适合使用NoSQL来存储。比如监控系统中的监控信息的存储，可能每种类型的监控信息都不太一样。这样可以避免经常对MySQL进行表结构调整，增加字段带来的性能问题。

这种架构的缺点就是数据直接存储在NoSQL中，不能做关系数据库的复杂查询，如果由于需求变更，需要进行某些查询，可能无法满足，所以采用这种架构的时候需要确认未来是否会进行复杂关系查询以及如何应对。

非常幸运的是，有些NoSQL数据库已经具有部分关系数据库的关系查询特性，他们的功能介于key-value和关系数据库之间，却具有key-value数据库的性能，基本能满足绝大部分web 2.0网站的查询需求。比如：

MongoDB就带有关系查询的功能，能解决常用的关系查询，所以也是一种非常不错的选择。下面是一些MongoDB的资料：
- 《视觉中国的NoSQL之路：从MySQL到MongoDB》
- [《Choosing
 a non-relational database; why we migrated from MySQL to MongoDB》](http://www.google.com.hk/url?q=http%3A%2F%2Fblog.boxedice.com%2F2009%2F07%2F25%2Fchoosing-a-non-relational-database-why-we-migrated-from-mysql-to-mongodb%2F&sa=D&sntz=1&usg=AFQjCNHweqX9Nr13JCZUvohjMa90ZC4ePw)
- 最近的一次Mongo Beijing 开发者聚会也有[一部分资料](http://www.google.com.hk/url?q=http%3A%2F%2Fwww.10gen.com%2Fconferences%2Fmongobeijing2011&sa=D&sntz=1&usg=AFQjCNFF99HNzBXXyvy7rhIgwFf_WCL3GQ)。

虽然Foursquare使用MongoDB的宕机事件的出现使人对MongoDB的自动Shard提出了质疑，但是毫无疑问，MongoDB在NoSQL中，是一个优秀的数据库，其单机性能和功能确实是非常吸引人的。由于上面的例子有详细的介绍，本文就不做MongoDB的使用介绍。

Tokyo Tyrant数据库带有一个名为table的存储类型，可以对存储的数据进行关系查询和检索。一个table库类似于MySQL中的一个表。下面我们看一个小演示：

我们要存储一批用户信息，用户信息包含用户名(name),年龄(age)，email，最后访问时间(lastvisit)，地区(area)。下面为写入的演示代码：

<?php 

$tt = new TokyoTyrantTable ( "127.0.0.1", 1978 ); 

$tt->vanish ();//清空 

$id = $tt->genUid ();//获取一个自增id 

//put方法提供数据写入。 put ( string $key , array $columns )； 

$tt->put ( $id, array ("id" => $id, "name" => "zhangsan", "age" => 27, "email" => "zhangsan@gmail.com", "lastvisit" =>strtotime ( "2011-3-5 12:30:00" ), "area" => "北京" ) );

$id = $tt->genUid (); 

$tt->put ( $id, array ("id" => $id, "name" => "lisi", "age" => 25, "email" => "lisi@126.com", "lastvisit" => strtotime( "2011-3-3 14:40:44" ), "area" => "北京" ) );

$id = $tt->genUid (); 

$tt->put ( $id, array ("id" => $id, "name" => "laowang", "age" => 37, "email" => "laowang@yahoo.com", "lastvisit" =>strtotime ( "2011-3-5 08:30:12" ), "area" => "成都" ) );

$id = $tt->genUid (); 

$tt->put ( $id, array ("id" => $id, "name" => "tom", "age" => 21, "email" => "tom@hotmail.com", "lastvisit" =>strtotime ( "2010-12-10 13:12:13" ), "area" => "天津" ) );

$id = $tt->genUid (); 

$tt->put ( $id, array ("id" => $id, "name" => "jack", "age" => 21, "email" => "jack@gmail.com", "lastvisit" =>strtotime ( "2011-02-24 20:12:55" ), "area" => "天津" ) );

//循环打印数据库的所有数据库 

$it = $tt->getIterator (); 

foreach ( $it as $k => $v ) { 

    print_r ( $v ); 

} 

?>

比如我们需要查询年龄为21岁的所有用户：

<?php 

$tt = new TokyoTyrantTable ( "127.0.0.1", 1978 ); 

$query = $tt->getQuery (); 

//查询年龄为21岁的用户 

$query->addCond ( “age”, TokyoTyrant::RDBQC_NUMEQ, “21” ); 

print_r ( $query->search () ); 

?>

查询所有在2011年3月5日之后登陆的用户：

<?php 

$tt = new TokyoTyrantTable ( "127.0.0.1", 1978 ); 

$query = $tt->getQuery (); 

$query->addCond ( “lastvisit”, TokyoTyrant::RDBQC_NUMGE, strtotime ( "2011-3-5 00:00:00" ) );

print_r ( $query->search () ); 

?>

从上面的示例代码可以看出，使用起来是非常简单的，甚至比SQL语句还要简单。Tokyo Tyrant的表类型存储还提供了给字段建立普通索引和倒排全文索引，大大增强了其检索功能和检索的性能。

所以，完全用NoSQL来构建部分系统，是完全可能的。配合部分带有关系查询功能的NoSQL，在开发上比MySQL数据库更加快速和高效。

（四）以NoSQL为数据源的架构

数据直接写入NoSQL，再通过NoSQL同步协议复制到其他存储。根据应用的逻辑来决定去相应的存储获取数据。
![](https://img-my.csdn.net/uploads/201304/01/1364779113_6238.jpg)


**图 5 -以NoSQL为数据源**

纯NoSQL的架构虽然结构简单，易于开发，但是在应付需求的变更、稳定性和可靠性上，总是给开发人员一种风险难于控制的感觉。为了降低风险，系统的功能不局限在NoSQL的简单功能上，我们可以使用以NoSQL为数据源的架构。

在这种架构中，应用程序只负责把数据直接写入到NoSQL数据库就OK，然后通过NoSQL的复制协议，把NoSQL数据的每次写入，更新，删除操作都复制到MySQL数据库中。同 时，也可以通过复制协议把数据同步复制到全文检索实现强大的检索功能。在海量数据下面，我们也可以根据不同的规则，把数据同步复制到设计好的分表分库的 MySQL中。这种架构：

非常灵活。可以非常方便的在线上系统运行过程中进行数据的调整，比如调整分库分表的规则、要添加一种新的存储类型等等。

操作简单。只需要写入NoSQL数据库源，应用程序就不用管了。需要增加存储类型或者调整存储规则的时候，只需要增加同步的数据存储，调整同步规则即可，无需更改应用程序的代码。

性能高。数据的写入和更新直接操作NoSQL，实现了写的高性能。而通过同步协议，把数据复制到各种适合查询类型的存储中（按照业务逻辑区分不同的存储），能实现查询的高性能，不像以前MySQL一种数据库就全包了。或者就一个表负责跟这个表相关的所有的查询，现在可以把一个表的数据复制到各种存储，让各种存储用自己的长处来对外服务。

易扩展。开发人员只需要关心写入NoSQL数据库。数据的扩展可以方便的在后端由复制协议根据规则来完成。

这种架构需要考虑数据复制的延迟问题，这跟使用MySQL的master-salve模式的延迟问题是一样的，解决方法也一样。

在这种以NoSQL为数据源的架构中，最核心的就是NoSQL数据库的复制功能的实现。而当前的几乎所有的NoSQL都没有提供比较易于使用的复制接口来完成这种架构，对NoSQL进行复制协议的二次开发，需要更高的技术水平，所以这种架构看起来很好，但是却不是非常容易实现的。我的开源项目PHPBuffer中有个实现TokyoTyrant复制的例子，虽然是PHP版本的，但是很容易就可以翻译成其他语言。通过这个例子的代码，可以实现从Tokyo Tyrant实时的复制数据到其他系统中。

总结

以NoSQL为主的架构应该算是对NoSQL的一种深度应用，整个系统的架构以及代码都不是很复杂，但是却需要一定的NoSQL使用经验才行。

NoSQL架构实践（三）——以NoSQL为缓存--ZT

在《NoSQL架构实践》系列的前面两篇文章中，介绍了[《以NoSQL为主》](http://www.infoq.com/cn/news/2011/03/nosql-architecture-practice-2)和[《以NoSQL为辅》](http://www.infoq.com/cn/news/2011/02/nosql-architecture-practice)的架构。由于NoSQL数据库天生具有高性能、易扩展的特点，所以我们常常结合关系数据库，存储一些高性能的、海量的数据。从另外一个角度看，根据NoSQL的高性能特点，它同样适合用于缓存数据。用NoSQL缓存数据可以分为内存模式和磁盘持久化模式。

内存模式

说起内存模式缓存，我们自然就会想起大名鼎鼎的Memcached。在互联网发展过程中，Memcached曾经解救了数据库的大部分压力，做出了巨大的贡献，直到今天，它依然是缓存服务器的首选。Memcached的常见使用方式类似下面的代码：
![](https://img-my.csdn.net/uploads/201304/01/1364779138_6845.jpg)


Memcached提供了相当高的读写性能，一般情况下，都足够应付应用的性能要求。但是基于内存的Memcached缓存的总数据大小受限于内存的大小。

当前如日中天、讨论得异常火热的NoSQL数据库Redis又为我们提供了功能更加强大的内存存储功能。跟Memcached比，Redis的一个key的可以存储多种数据结构Strings、Hashes、Lists、Sets、Sorted sets。Redis不但功能强大，而且它的性能完全超越大名鼎鼎的Memcached。Redis支持List、hashes等多种数据结构的功能，提供了更加易于使用的api和操作性能，比如对缓存的list数据的修改。

同样，其他一些NoSQL数据库也提供了内存存储的功能，所以也适合用来做内存缓存。比如Tokyo Tyrant就提供了内存hash数据库、内存tree数据库功能，内存tree数据可根据key的顺序进行遍历。你可以通过使用其提供的兼容Memcached协议或自定义的协议来使用。

持久化模式

虽然基于内存的缓存服务器具有高性能，低延迟的特点，但是内存成本高、内存数据易失却不容忽视。几十GB内存的服务器，在很多公司看来，还比较奢侈。所以，我们应该根据应用的特点，尽量的提高内存的利用率，降低成本。

大部分互联网应用的特点都是数据访问有热点，也就是说，只有一部分数据是被频繁访问的。如果全部都cache到内存中，无疑是对内存的浪费。

这时，我们可以利用NoSQL来做数据的缓存。其实NoSQL数据库内部也是通过内存缓存来提高性能的，通过一些比较好的算法，把热点数据进行内存cache，非热点数据存储到磁盘以节省内存占用。由于其数据库结构的简单，从磁盘获取一次数 据也比从数据库一次耗时的查询划算很多。用NoSQL数据库做缓存服务器不但具有不错的性能。而且还能够Cache比内存大的数据。

使用NoSQL来做缓存，由于其不受内存大小的限制，我们可以把一些不常访问、不怎么更新的数据也缓存起来。比如论坛、新闻的老数据、数据列表的靠后的页面，虽然用户访问不多，但是搜索引擎爬虫会访问，也可能导致系统负载上升。

如果NoSQL持久化缓存也使用类似基于内存的memcached设置过期时间的方式，那么持久化缓存就失去了意义。所以用NoSQL做缓存的过期策略最好不使用时间过期，而是数据是否被更新过，如果数据没有更新，那么就永久不过期。下面我们用代码(php)演示一种实现这种策略的方法：

场景：新闻站点的评论系统。用户对新闻页面的url进行评论，然后根据url进行查询展示
![](https://img-my.csdn.net/uploads/201304/01/1364779171_9426.jpg)
![](https://img-my.csdn.net/uploads/201304/01/1364779180_1584.jpg)

![](https://img-my.csdn.net/uploads/201304/01/1364779191_4120.jpg)



我把上面代码演示的缓存使用方式称为基于版本的缓存。这种方式同样适用于基于内存的Memcached。它能实现缓存数据的实时性，让用户感觉不到延迟。只要用户一发表评论，该新闻的评论缓存就会失效。用户很少去评论一些过时的新闻，那么缓存就一直存在于NoSQL中，避免了爬虫访问过时新闻的评论数据而冲击数据库。

总结

目前国内的新浪微博已经在大量的使用Redis缓存数据，赶集网也在大量的使用Redis。Redis作为一些List，Hashes等数据结构的缓存，非常适合。

把NoSQL当持久化Cache使用的模式，在很多大数据量、有热点、查询非热点数据比较消耗资源的场景下比较有用。

NoSQL架构实践总结

到这里，关于NoSQL架构实践的三篇文章就结束了。NoSQL架构并不局限于我介绍的三种模式，他们之间也可以进行组合，应该根据你具体的应用场景灵活使用。不管是什么模式，都是为了解决我们的问题而出现的，所以在系统架构的时候，要问下自己，我为什么要用NoSQL；在对NoSQL架构模式选型的时候，要问下自己，我为什么要这么用NoSQL。




