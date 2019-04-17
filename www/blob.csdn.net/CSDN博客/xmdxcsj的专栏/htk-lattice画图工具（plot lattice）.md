# htk - lattice画图工具（plot lattice） - xmdxcsj的专栏 - CSDN博客





2016年06月27日 20:40:14[xmucas](https://me.csdn.net/xmdxcsj)阅读数：1455








对于生成的lattice文件，需要根据节点和边手动画出来lattice网络结构，非常不方便，找到一种方法可以把lattice转化为pdf的网络图形式。 

一、安装工具 

1.基于perl的cpan安装工具Graph-ReadWrite
```
sudo perl -MCPAN -e shell
install Graph
install XML::Parser
install XML::Writer
install IO::File
install Graph::ReadWrite
```

2.安装dot 

yum install graphviz-devel* 

二、转化脚本 

1.lattice -> dot 

perl脚本如下
```
use Graph;
use Graph::Reader::HTK;
use Graph::Writer::Dot;

my ($graph, $reader, $writer);
my ($testfile);
my $FILE;
my $ingraph;

$testfile = $ARGV[0];
$genfile  = $ARGV[1];
$reader = Graph::Reader::HTK->new();
$writer = Graph::Writer::Dot->new();
$ingraph = $reader->read_graph($testfile);
$writer->write_graph($ingraph, $genfile);
```

2.dot->pdf 

shell命令如下：
`dot -Tpdf $dot_file > $pdf_file`
3.转化以后的结果 
![这里写图片描述](https://img-blog.csdn.net/20160627203915551)





