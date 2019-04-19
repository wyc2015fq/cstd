# Cache中的MESI协议基本知识介绍 - 文章 - 伯乐在线
![mesi_t-1](http://jbcdn2.b0.upaiyun.com/2016/10/92e051d2d66f8a58acc3af151bc0f915.png)
Read消息：read消息中包括了所需要读取的cache line的物理地址。
Read相应消息：该消息中包含了 read消息中所请求的数据。该数据有可能是由内存或者其它cpu中所获得。例如：其它cpu中含有所需要的处于“modified”模式的数据。 cache必须回应一个read相应消息。
invalidate消息：invalidate消息包含需要进行invalidate的cache line的物理地址。 所有的cache必须要将相应的数据从其cache和相应消息中删除。
invalidate应答：当一个cpu接受到一个invalidate消息后，在其完成将相应的数据从其cache中删除后必须给予相应的invalidate应答消息。
read无效：其消息中包含了所需要读取的cache line的物理地址。于此同时，将其相直接关联的cache中的数据删除。 因此，其包含了read和invalidate两重意思。 一个 read invalidate消息需要：一个read相应以及一些列的 invalidate 应答作为其回应。
writeback: 该消息中包含了需要写回到内存的：地址和数据。该消息允许cache强制将其cache line置位modified状态，当其需要较为其它数据腾出相应的空间时。
下面我们就对上述的四种状态给出一个直观的介绍。
（0）初始状态， 初始状态如上述的图二所示。
（1）shared状态，该状态下Cache Line中的数据与Memory中的数据一致，且其数据也存在于其它的CPU Cache Line中。
![mesi_modified-1](http://jbcdn2.b0.upaiyun.com/2016/10/b7d46452088c16bc019bb66ba6c54c95.png)
（2）Modified状态，在该状态下Cache Line中的数据与Memory数据一致，但数据只是存在与本CPU的Cache Line中。
![smp-1-1](http://jbcdn2.b0.upaiyun.com/2016/10/1db7bb116aa70dd4664bf2d474346823.png)
下面我们就来介绍一下MESI转换图中的各个状态并给出详细的分析和介绍。
## #2. MESI状态转换详解.
- a: 当一个cache line处于M状态，表明该cache line中的数据，因此当将cache line中 的数据回写到内存时候，就由M状态变为 E，表明数据有效，且已经回写到内存中， 但由于为及时的与其它cpu进行数据同步，故而只有在本cpu中存在着该数据。其中 蓝色的点代表修改后的数据。
![mesi_a-1](http://jbcdn2.b0.upaiyun.com/2016/10/4fb661fe782a369223b44e7dfeee6e97.png)
- b: 当cpu想一个处于独享状态的cache line中写入数据时候，由于该cache line处于 独享状态（其表明：该cache line中的数据只在本cpu的cache line中存在），因 此，当处于独享状态下时候，我们无需将数据做其他额外的工作，直接修改数据 即可。
![mesi_b-1](http://jbcdn2.b0.upaiyun.com/2016/10/02ef92b0baab87934ca70f7514201942.png)
- c: 此种状态下，由Modified到invalidate表明，将数据从相应的cache line中删除。 将本地的数据删除。即执行清空数据操作。即：转换由类似于e的状态，到初始 状态0. 各个cpu在接收到read invalidate消息后，各个cpu必须回复read res和 invalidate ACk消息。 发送read invalidate消息是通知所有cpu将其各自本地的 数据给清除掉，在完成本地数据的清除后，发送一个invalidate ack消息，告诉 发起read invalidate消息的主cpu，其它cpu已经完成该项操作。
- d: cpu对于一个未在cache line中的数据进行原子的读写操作。此时cache line会由 原来的invaldate 状态，转为Modified状态。首先发送一个read invalidate消息 而后接收到 read相应消息，而该消息中包含有相应的包含所请求的数据。当cpu接收 完所有的 invalidate应答相应后，则完成本次的数据读取。
- e: cpu在一个已有数据的cache line上执行原子的读写操作，由于原数据已存在于 cache line中，因此如果对该cache line进行读写操作，首先要使得原有数据由 该cache line中删除，因此发送一个invalidate消息。由于数据同样也存在于其它 cpu的cache line中，因此必须等到所有的cpu均返回 invalidate 应答，方可以认 本次读写操作结束。
![mesi_e-1](http://jbcdn2.b0.upaiyun.com/2016/10/0a34633143985d0f6fb3e09146ac114f.png)
- f: 当处于Modified状态时候，一个其它cpu发起读数据操作。此时，拥有数据的cpu需要 将该数据发生到其它cpu的cache line中并将数据更新到内存中。该中状态转换，通常是 由一个read消息所发起，当该cpu接收到一个read消息时候，那么该cpu将回复一个read相应 且该相应消息中包含着所请求的数据。
- g: 当其他cpu需要从cache line中读取数据时，其是通过其它cpu cache或者内存中 读取相应的数据。无论何种情况，该cpu保存了一份read-only拷贝。本次转换由 cpu接收到一个read消息后发起该请求read 请求。 该cpu收到该消息后，会发送 一个包含所请求数据的 read 响应消息。
- h: 当该cpu意识到不仅需要将某些数据写入到该cpu的cache line中时候，其将会发送一个invalidate消息。cpu只有等到所有的cpu都已发送invalidate ack消息后，方可认为完成本次操作，否则的话会导致该cpu 无法知道其他cpu中的数据是否被删除。或者，其他所有的cpu将该cache line从他们的caches中通过使用 writeback消息将其从cache中删除（为了未来可能的数据腾出空间）。因此，该cpu中的数据变为独享数据。此时的状态，cpu0，可以发生数据写入操作，此时会发送一个Invalidate消息给其它所有的cpu，例如：cpu1，cpu1将其cache line中数据删除，并返回invalidate ack消息。 此时，由于其它的cpu中的数据均被删除，故而cpu0中的数据为独享状态。
![mesi_h-1](http://jbcdn2.b0.upaiyun.com/2016/10/03eb65eb53c848e6fc5b0be3697cb691.png)
- i: 当其它某些cpu试图在由本cpu cache line中所拥有的数据上执行一个原子的读写操作时候。该 cpu将该数据从本cache line中删除（使用 invalidate消息）。该项转移由该cpu接受到一个read invalidate消息开始，以该cpu响应一个 read 响应消息以及一个invalidate ack消息。因为在mesi 协议中对于一个invalidate消息必须要有一个invalidate ack消息与之相应对于。最后的状态回到（0） 状态。
- j: 当cpu保存一个未在其cache line中的数据项时候，其会发送一个read invalidate消 息给其它cpu，且该cpu等待，直到其接收一个 read响应消息和所有cpu返回的invalidate ack消息。该cache line将由（b)状态转为修改状态，当完成实际的数据存储后。
- k: cpu读取一个数据，而该数据并不在该cpu的cache line中。cpu将发送一个read消息 且当读取到read 响应消息后，完成本次操作。
- l: 当某些cpu执行对在其cache line中的数据存储命令时，但是该其cpu cache line中 的数据是read only状态，因为该cache line中的数据是在其它cpu中，而该cpu cache line中的数据只是其它cpu cache line中的数据的一份拷贝。该次转为由接收到一个invalidate消息开始并返回一个invalidate ack消息。
正是由于多核架构以及指令的乱序执行所带来的数据在不同核直接的数据同步问题，需要我们在使用存在着这样的一种现象：当我们使用内存中某个数据的时候，如果此时执行该指令的核中的数据Cache中的数据与内存中的数据一致的话，此时执行并无问题。但是当在后续的执行过程，由于乱序以及多核，此时该指令运行在另外的一个核之上，而该核中Cache的数据并同步，此时系统将取出旧数据进行执行。这样便导致了程序运行出错误的结果，为了防止这该种情况的发生，在使用某个变量之前，强制的要求各个核进行数据的同步，当数据同步完成后，方可进行后续的执行。因此，引入了内存屏障的概念。当然为了性能的考虑，我们又将内存屏障细分为：读写两种内存屏障操作。
