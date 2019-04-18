# MapReduce算法设计--Think in Hadoop - Orisun - 博客园







# [MapReduce算法设计--Think in Hadoop](https://www.cnblogs.com/zhangchaoyang/articles/2623905.html)





本文介绍几种MapReduce算法设计的技巧，全部内容翻译自《[Data-Intensive Text Processing with MapReduce](http://lintool.github.com/MapReduceAlgorithms/MapReduce-book-final.pdf)》。

### Local Aggregation

说到Local Aggregation，你可能会想不就是Combiner吗。实际上在mapper中进行combining比使用真正的combiner高效得多。首先combiner只是作为MapReduce的可选优化方案（就像inline对于C++编译器是一种可选优化方案一样），不一定会被执行。其次在mapper中进行combining可以减少很多的I/O操作，提高效率，毕竟mapper的每个结果都个结果都要被写入磁盘，我们当然希望写入磁盘的数据越少越好。

回想一下WordCount的例子，在mapper中Emit(word,1)，在reducer中对相同的键值Emit(word,sum)，而combiner采用和reducer一样的例程。

现在我们要在mapper中就把combiner的事情给做了。

class Mapper
    method setup
        H := new AssociativeArray
    method map(docid a,doc d)
        foreach term in doc
            H{term} := H{term}+1
    method cleanup
        foreach term in H
            Emit(term,value of H{term})


但是上述方法在可扩展性上会遇到瓶颈：我们必须在内存中维持一个关联数组H，当mapper接收到的数据分片很大，里面有很多word时，H就会很大，甚至超出内存。解决办法有两种：
- 设定一个值K，mapper每读入K条键值对，就把H中的内容全部Emit，然后把H清空接着读入键值对。类似于先读满一个buffer，再把它flush掉，然后buffer重复利用。
- 在mapper中监控内存的使用率，当达到一定阈值时，就把H的内容flush。

### 使用Local Aggregation设计算法的正确性

有一个计算平均值的例子。输入文件中的每一行记录着（string,count）表示一个单词出现的次数，在所有的记录中相同的string会出现多次，现在我们要计算每一个string的平均count。

求和是一种“分配型”的聚合，即

![](http://www.forkosh.com/mathtex.cgi?%20\dpi{150}sum(a,b,c,d,e)=sum(sum(a,b),sum(c,d,e)))

但求平均不是分配型的，即

![](http://www.forkosh.com/mathtex.cgi?%20\dpi{150}mean(a,b,c,d,e)\ne{mean(mean(a,b),mean(c,d,e))})

一种直接的想法是：

class Mapper
	method map(string word,integer count)
		Emit(word,count)

class Reducer
	method reduce(string word,integer-list [c1,c2,...])
		sum := 0
		cnt := 0
		foreach integer c in [c1,c2,...]
			sum := sum+c
			cnt := cnt+1
		avg := sum/cnt
		Emit(word,avg)


现在我们想使用combiner进行local aggregation。在上面的代码中，很容易地可以将reducer的部分工作放到combiner中来完成。

class Mapper
	method map(string word,integer count)
		Emit(word,count)
		
class Combiner
	method reduce(string word,integer-list [c1,c2,...])
		sum := 0
		cnt := 0
		foreach integer c in [c1,c2,...]
			sum := sum+c
			cnt := cnt+1
		Emit(word,pair(sum,cnt))

class Reducer
	method reduce(string word,pairs[(s1,c1),(s2,c2),...])
		sum := 0
		cnt := 0
		foreach pair(s,c) in pairs[(s1,c1),(s2,c2),...]
			sum := sum+s
			cnt := cnt+c
		avg := sum/cnt
		Emit(word,avg)


很遗憾上面的代码是错误的，不符合MapReduce的规范，由于Combiner只是可选的优化方案，所以MapReduce要求在没有Combiner的情况下程序仍能正确的运行。上述代码mapper的输出键值对和reducer的输入键值对类型不匹配，导致程序无法运行。还好，我们只需要把mapper稍作修改就可以了。

class Mapper
	method map(string word,integer count)
		Emit(word,pair(count,1))


进一步，现在我们想把local aggregation从combiner中转移到mapper中，与wordcount例子中使用的技巧一样，使用关联数组！

class Mapper
	method setup
		S := new AssociativeArray
		C := new AssociativeArray
	method map(string word,integer count)
		S{word} := S{word}+count
		C{word} := C{word}+1
	method cleanup
		foreach word in S
			Emit(word,pair(S{word},C{word}))


### Pairs and Stripes

mapper和reducer之间能够交换的数据仅仅是一个键值对，很多时候我们为了使这一个键值对中包含更多的信息，就把键或值设计得足够复杂，比如让它是一个pair或AssociativeArray。在上文中我们已经看到这种应用实例了。

在N-Gram模型中，我们要计算一个方阵A，其规模是N×N，N是语料库中单词的个数。A[i][j]表示单词j在单词i的领域中出现的次数。

Pairs方式：

class Mapper
	method map(docid a,doc d)
		foreach term w in doc
			foreach term u in Neighbour(w)
				Emit(pair(w,u),1)
				
class Reducer
	method reduce(pair p,counts [c1,c2,...])
		s := 0
		foreach count c in [c1,c2,...]
			s := s+c
		Emit(p,s)


Stripes方式：

class Mapper
	method map(docid a,doc d)
		foreach term w in doc
			H := new AssociativeArray
			foreach term u in Neighbour(w)
				H{u} := H{u}+1
			Emit(w,H)
				
class Reducer
	method reduce(term w,stripes [H1,H2,...])
		F := new AssociativeArray
		foreach stripe H in [H1,H2,...]
			sum(F,H)
		Emit(w,F)


上述pairs和stripe方式都可以很轻松的写一个combiner来提高效率，并且在stripes方式中combiner发挥的作用会相对大一些。

注意在Stripes方式中，由于要为doc中的每一个term建立一个AssociativeArray，维持这么多的关联数组可能会导致内存溢出。解决方案在本文的第一小节已经提到了。

stripe方式要比pairs方式快得多，因为pairs方式Emit操作（即写磁盘操作）太频繁了。

### Computing Relative Frequencies

还说N-Gram这个问题，刚才我们只是计算了单词u在单词w的领域内出现的次数，但是个次数多并不能说明u和w的关系就强烈，因为可能是w太平凡了，导致w出现的次数很多，进而导致w和u相伴出现的次数也很多。另外有一对单词p和q，它们相伴出现的次数比较少并不能说明它们的关系不强烈，可能是因为在语料库p总共出现的次数也不多。所以我们应该计算“相对频率”才是合理的：

![](https://pic002.cnblogs.com/images/2012/103496/2012080516451623.png)

实际上就是对已有矩阵的每一行求和，再让该行的每个元素除以这个和。

使用上面的Stripes方式代码可以很容易地扩展为计算相对频率的代码，因为在redcuer中Emit的每一个键值对刚好对应矩阵的一行数据，计算矩阵的行和非常方便。

现在我们要在pairs方式中计算相对频率该怎么做呢？因为mapper输入的键值对是(pair(w,u),1)，要计算行和就必须使所相同的w都到一个reducer中，但默认情况下Hadoop只能保证相同的pair(w,u)映射到同一个reducer当中--对于复杂的键类型，MapReduce中要把它转换成bytes再进行Hash。因此我们需要重写Partitioner，仅对pair的左值（即w）进行散列，这就可以保证相同的w都映射到了同一个reducer。但是注意，映射到同一个reducer的pair的左值可能不只一种。MapReduce保证映射到同一个reducer的键值对都是按键排序好的，为了使在同一个reducer中，相同的左值是连续在一起的，我们还需要自定义的pair的比较方式：对pair进行比较时，只比较左值。

class Pair
	method compareTo(Pair obj)
		rect := this.left.compareTo(obj.left)
		if(rect == 0)
			if(this.right == "*")
				return -1
			else if(obj.right == "*")
				return 1
		return rect

class Mapper
	method map(docid a,doc d)
		foreach term w in doc
			foreach term u in Neighbour(w)
				Emit(Pair(w,u),1)
				Emit(Pair(w,*),1)
				
class Partitioner
	method getPartition(Pair key)
		return key.getLeft().hashCode()
				
class Reducer
	method setup
		prev := NULL
		rowsum := 0
	method reduce(pair p,counts [c1,c2,...])
		foreach count c in [c1,c2,...]
			s := s+c
		if(p.left != prev)
			assert(p.right == "*")
			prev := p.left
			rowsum := s
		else
			Emit(p,s/rowsum)


上面的代码中，我们使mapper多输出了一项Emit(Pair(w,*),1)，它的作用是使在reducer中不需要遍历所有具有相同左值的pair(w,u)就能提前计算出行和--仅依靠pair(w,*)就可计算出行和，这样在第一次遇到pair(w,u)时就可以计算出它的相对频率。为了使reducer最先遇到pair(w,*)，我们定义在对pair排序时，在左值相同的情况下，右值为“*"者较小。

在一个reducer中出现的键值对的顺序是这样子的：

![](https://pic002.cnblogs.com/images/2012/103496/2012080517400963.png)

同样，仍然可以使用combiner进行优化，把local aggregate放在mapper中更好。

说到这里顺便扯一句，一般程序员在计算样本方差时会采用这个公式：

![](https://pic002.cnblogs.com/images/2012/103496/2012080518244372.png)

这个公式要求2趟扫描样本，第1趟扫描是为了计算出X的平均值，第2趟扫描是按照公式来计算。而统计学专业的学生知道计算方差还有另外一个公式：

![](https://pic002.cnblogs.com/images/2012/103496/2012080518324489.png)

只需要1趟扫描就可以把ΣXi2和ΣXi都算出来，直接代入公式就可算出方差。

### Secondary Sorting

Hadoop只能保证同一个reducer中的健值对是按键排好序的，在Goole在MapReduce中提供一个可选的函数--在按键排序之后按value排序。考虑这么一种情形：mapper输出的键值对形如

key　　　value

t1　　　　(m1,r1988)

t1　　　　(m2,r1987)

t2　　　　(m1,r1990)

key已经排好序了，t1<t2。我们希望key相同的时候按value中的第2列排序，即希望的结果为

key　　　value

t1　　　　(m2,r1987)

t1　　　　(m1,r1988)

t2　　　　(m1,r1990)

Hadoop是不对value进行排序的，所以我们的解决方法是：把需要排序的valueu部分放到key里面去，即

key　　　　　　value

(t1,r1987)　　　m2

(t1,r1988)　　　m1

(t2,r1990)　　　m1

当然这个时候需要自定义partitioner仅对pair的左值进行散列，且pair的比较规则是先比较左值，左值相等时再比较右值。












