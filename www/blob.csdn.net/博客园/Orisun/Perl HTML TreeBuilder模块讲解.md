# Perl  HTML::TreeBuilder模块讲解 - Orisun - 博客园







# [Perl  HTML::TreeBuilder模块讲解](https://www.cnblogs.com/zhangchaoyang/articles/2052842.html)





HTML::TreeBuilder继承自HTML::Parser和HTML::Element，所以下面讲的很多方法实际上源于HTML::Parser和HTML::Element。

拿下面这个HTML文件开刀：

<html lang='i-klingon'>
     <head><title>Pati Pata</title></head>
     <body>
       <h1 lang='la'>职工</h1>
       <p lang='es-MX' align='center'>
         Foo bar baz <cite>Quux</cite>.
       </p>
       <p>Hooboy.</p>
     </body>
  </html>


perl代码：

#!/usr/bin/perl
use HTML::Element;
use HTML::TreeBuilder;

foreach my $file_name (@ARGV) {
    my $tree = HTML::TreeBuilder->new;
    $tree->parse_file($file_name);
	$head=$tree->find_by_tag_name('head');　　　　　　##找到head节点
	print $head->attr('_parent')->as_text();　　　　　　　　##_parent属性的值是html节点
	print "\n";
    $tree = $tree->delete;
}


 首先根据一个文件创建了一棵HTML结构解析树$tree，注意$tree本身也是HTML::Element的一个实例，因为HTML::TreeBuilder继承自

HTML::Element。

调用<html>节点的as_text输出：Pati Pata职工 Foo bar baz Quux. Hooboy.

如果把find_by_tag_name('head')改成find('head')方法，效果一样。

如果把$head->attr('_parent')改成$head->look_up('lang','i-klingon')效果一样。同样你应该look_down()怎么用了。



#!/usr/bin/perl
use HTML::Element;
use HTML::TreeBuilder;
use HTML::Parser;

foreach my $file_name (@ARGV) {
    my $root = HTML::TreeBuilder->new;
    $root->parse_file($file_name);
	$body=$root->find_by_tag_name('body');
	$p=$body->find_by_attribute('lang','es-MX');　　##在当前节点及其子节点下寻找lang属性为es-MX的节点
	print $p->as_text();
	print "\n";
    $tree = $root->delete;
}
输出： Foo bar baz Quux. 
find_by_attribute不能向上查找，试图通过find_by_attribute('lang','i-klingon')来找到<html>节点是行不通的。
$p=$body->look_down('lang','es-MX');效果是一样的。

还可以这样来寻找<p>节点：

$p=$body->look_down('_tag','p',
						'align','center',
						sub{$_[0]->attr('lang')=~qr/es/}
						);




@childern=$root->content_list;　　　　　　##返回content列表。即返回包含head和body的列表
    foreach(@childern){
		print $_->as_text();
		print "\n";
	}


输出：

Pati Pata

职工 Foo bar baz Quux. Hooboy.

在标量环境下$root->content_list返回list的长度，在其他情况下如果把一个list返回给一个标量，则是返回list中的第一个元素。



foreach my $node_r ($p->content_refs_list){
		next if ref $$node_r;
		print $$node_r;
		print "\n";
	}


输出两行：

  Foo bar baz

 . 

之所以没有输出Quux就是因为ref $$node_r返回true了。

$p->dump()输出结果为：

<p align="center" lang="es-MX"> @0.1.1 

 " Foo bar baz " 

 <cite> @0.1.1.1    

"Quux"  

". "

dump(*FH)   dump到一个文件中。

print $p->as_HTML();


输出：<p align="center" lang="es-MX"> Foo bar baz <cite>Quux</cite>. 注意没有输出结尾标签</p>。

print $p->as_text(skip_dels=>1,extra_chars=>'\xA0');

“del"节点下的content text将被忽略，同时text string中多个连续的空格将被合并为一个空格。

$p->as_XML();

输出：<p align="center" lang="es-MX"> Foo bar baz <cite>Quux</cite>. </p>。除了比as_HTML多一个</p>外，还多一个换行符。

print $p->as_Lisp_form();
输出：
("_tag" "p" "align" "center" "lang" "es-MX" "_content" (
  " Foo bar baz "
  ("_tag" "cite" "_content" (
    "Quux"))
  ". "))



上面这个方法能让我们对一个节点的属性和属性的值看得非常清楚。












