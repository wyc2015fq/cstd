# java中List与Map的使用 - Big Smile - CSDN博客
2018年01月07日 21:36:54[王啸tr1912](https://me.csdn.net/tr1912)阅读数：690
所属专栏：[java学习](https://blog.csdn.net/column/details/26775.html)
最近在捣鼓SKU算法（初步），然后用到了一堆list与map或者map与list的各种嵌套，所以借此机会来总结一些list和map的一些特性，以及一些使用的注意事项。
# 一、map和list的区别
        首先呢，我们先简要的说一下map与List的一些区别地方，首先呢，map就是类似于key-value形式的数据结构集合，key值可以唯一的找到一个对应的value值，然后value值可以是任何类型，或者是对象类型，因为有了这个特点，我们也可以在map中放入List，从而行成一个靠value寻找List的形式。
        而List呢，是某一种类型对象的集合，可以是各种基于对象类型的对象，比如String，或者是某个实体，或者是Map都是可以的。给人的感觉是这个样子：
![](https://img-blog.csdn.net/20180107220544946?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvdHIxOTEy/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
        因为有这种特性，所以我们在组装数据的时候就有n多种可能性了：
![](https://img-blog.csdn.net/20180107220733608?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvdHIxOTEy/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
![](https://img-blog.csdn.net/20180107221110423?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvdHIxOTEy/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
# 二、实际应用
        然而我们在实际应用的时候考虑的问题会有很多，比如第一就是我们需要如何去拿数据来用，我们的数据中哪个部分是静态的，哪个部分又是动态不确定的，这都是很重要的。
        首先我们来看，List和map虽然都是可以延伸很多个组或者对象的数据结构，但是这里面有很本质的区别，那就是map中取数据可以用key来指定取哪个，但是lsit则必须指定序号来取，并且有排序的功能。如此一来，在我们的数据数量也好，组分也好是固定的情况下的话，我们一般适用的是map来存储，这样我们可以直接找到并使用我们想要找的数据。例如：性别，是否存在等特性的时候经常使用这样的数据结构来存储，以达到可以随时使用的目的。
        但是类似于班级的人等不确定数量的元素，一般我们用List结构来存储，因为我们可以按照不同的班级创建容量不同的学生集合，按照循环来调用学生List<Object>。如果是要一个年级的学生的话，则需要map和list配合来使用了，map中标志为年级中的每个班，而班中存储的是List，为班里面的学生。Map<String,List<Object>>
        那么什么时候用到List中套map呢？这种情况就是List中想要存储类型或者表现形式不同的数据的时候，可以借助map来实现，比如一个map是Map<String,int>,一个是Map<String,double>用Map把类型都统一了就可以放在List中了，或者是map中是集合，但是map的数量不确定的时候。
       那么什么时候用List嵌套List<List<Object>>呢？这样的时候就是有两种因素都不确定的时候，比如商品的属性数量还有属性中的项目个数都不确定的时候就可以使用这种结构。
声明：我们使用的是jsp页面进行数据显示的，所以查询出来的数据需要按key取值，或者是循环显示，所以就有了这样的一个思考路线和数据结构的铺垫，希望大家可以借鉴。
