# Perl解析当当网图书信息页面 - Orisun - 博客园







# [Perl解析当当网图书信息页面](https://www.cnblogs.com/zhangchaoyang/articles/2053058.html)





功能描述：
- 首先判断html文件是否为当当图书的页面，不是则不处理
- 把图书标题，价格，作者，出版社等信息抽取出来存入文件
- perl程序运行命令：perl  programfile    html_file_list

原代码奉上：

#!/usr/bin/perl
use HTML::Element;
use HTML::TreeBuilder;
use HTML::Parser;

open DATAFH,">>data" || die "open file failed:$!";
select DATAFH;
foreach my $file_name (@ARGV) {
	unless(-e $file_name){
		print "$file_name文件不存在.\n";
		next;
	}
	##首先判断文件编码是不是UTF-8,如果不是要转换成UTF-8,因为我发现perl的HTML模块好像只能处理UTF-8编码的文件，处理GBK文件时会出现中文乱码。而我们从网上下载的页面几乎都是GBK编码。
	system "enca","-L","zh_CN","-x","UTF-8",$file_name;		##用enca比用iconv的好处在于不需要管原文件是什么编码（即使本来就UFTF-8编码），直接转换成UTF-8编码就是了。
    my $root = HTML::TreeBuilder->new;
    $root->parse_file($file_name);
    $title=$root->find_by_tag_name('title');
    $str_title=$title->as_text();
    if($str_title=~qr/图书 - 当当网$/){
    	print "网页标题:$str_title\n";
    	$div_h1=$root->look_down("_tag","div","class","h1_title book_head");
    	$h1=$div_h1->look_down('_tag','h1');
    	print "图书名称:",$h1->as_text(),"\n";
    	$div_info=$root->look_down("_tag","div","class","info book_r");
    	$price_d=$div_info->look_down("_tag","p","class","price_d");
    	print $price_d->as_text(),"\n";
    	$price_m=$div_info->look_down("_tag","p",
    								"class","price_m");
    	foreach my $node_r ($price_m->content_refs_list){
    		next if ref $$node_r;
    		print $$node_r,"\n";
    	}
    	$div_detail=$div_info->look_down("_tag","div","class","book_detailed","name","__Property_pub");
    	@elements=$div_detail->find_by_tag_name('p');
    	foreach(@elements){
			print $_->as_text(),"\n";
		}
		$clear=$div_detail->look_down("_tag","ul","class","clearfix");
		@lis=$clear->content_list();
		@spans=$lis[0]->content_list();
		print $spans[1]->as_text(),"\n";
		@spans=$lis[2]->content_list();
		print $spans[1]->as_text(),"\n";
		print $spans[2]->as_text(),"\n";
		print "********************************************\n";
		$|=1;
    }
    else{
		print STDOUT "$file_name不是当当网图书信息页面。\n";
		next;
	}
    $tree = $root->delete;
}
close DATAFH;


输出文件里的内容：

![](https://pic002.cnblogs.com/images/2011/103496/2011052120351581.png)












