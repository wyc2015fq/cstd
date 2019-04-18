# Perl爬虫--爬特定内容的网页 - Orisun - 博客园







# [Perl爬虫--爬特定内容的网页](https://www.cnblogs.com/zhangchaoyang/articles/2053784.html)





[前一篇博文](http://www.cnblogs.com/zhangchaoyang/articles/2053058.html)是对当当网图书详情页面进行分析，提取图书的相关信息。

现在要解决的问题做一个爬虫把当当网图书详情页面下载下来。一般爬虫的基本思路是这样的：给定一个初始URL，沿着网页间的超链接链一直访问下去。但这里要注意几个问题：
- 避免回路出现死循环。使用队列来存储待访问的URL方法简单，网页访问顺序是固定死的；采用来存储待访问的URL，可以很好地用树的层次结构来表示网页的层次结构，对树的遍历可以采用前根遍历，也可以采用层次遍历。
- 大量的页面都是我们不需要的--不是当当网图书详情页面，如何少做一些无用功。
- 访问页面连接超时时间的设定。如果你网速不好，就设长一点，但是再次提醒你访问的大多数网页都是我们不需要的。

#!/usr/bin/perl
use strict;
use warnings;
use LWP 5.64;				##载入新版本的LWP classes
use LWP::Simple;
use LWP::UserAgent;
use LWP::ConnCache;
use HTML::TreeBuilder;
use Unicode::MapUTF8 qw(to_utf8 from_utf8);
use Digest::MD5 qw(md5 md5_hex md5_base64);

open LOGFH,">>log" || die "Open log file faild:$!.";				##记录程序运行过程中的失败或成功信息
open DATAFH,">>data" || die "open data file failed:$!";				##从网页上把图书信息抽取出来后存入data文件

my @urls=();														##根据初始URL和页面间的链接关系建立要搜寻的目标URL群体
my $starturl="http://book.dangdang.com/";
push @urls,$starturl;

my $browser=LWP::UserAgent->new();									##LWP::UserAgent用来请求页面
$browser->agent('Mozilla/4.0 (compatible; MSIE 5.12; Mac_PowerPC)');##伪装一下
$browser->timeout(10);												##request连接超时为10秒
$browser->protocols_allowed(['http','gopher']);						##只接受 http 和 gopher 协议
$browser->conn_cache(LWP::ConnCache->new());

while(scalar @urls>0){
	my $url=shift @urls;
	my $response=$browser->get($url);
	unless ($response->is_success){
		print LOGFH "无法获取$url -- ",$response->status_line,"\n";
		next;
	}
	
	my $html=$response->content;
	$html=to_utf8({-string=>$html,-charset=>'gb2312'});				##把页面编码从gb2312转为utf8
	if(scalar @urls<300){											##控制@urls的容量，避免内存溢出
		while($html=~m/href=\"(.*?)\"/ig){							##本页面上的所有链接都加入目标搜索范围
			push @urls,URI->new_abs($1,$response->base);
		}
	}
	
	unless ($url=~m[http://product.dangdang.com/product.aspx\?product_id=\d{7,8}&ref=book-\d{2}-\u\d?]){
		print LOGFH "$url不是当当图书详情页面.\n";
		next;
	}
	$|=1;						##改娈输出缓存前调用一下$|=1;清空缓存														
		
	select DATAFH;
	my $recordId=md5_base64($url);									##对页面URL计算MD5值，作为图书记录ID
	my $root=HTML::TreeBuilder->new_from_content($html);			##HTML::TreeBuilder用来解析html文本内容
	my $title=$root->find_by_tag_name('title');
	my $str_title=$title->as_text();
	print "记录ID：$recordId\n";
	print "网页地址：$url\n";
	print "网页标题：$str_title\n";
	my $div_h1=$root->look_down("_tag","div","class","h1_title book_head");
	my $h1=$div_h1->look_down('_tag','h1');							##图书标题
	print "图书名称：",$h1->as_text(),"\n";
	my $div_info=$root->look_down("_tag","div","class","info book_r");
	my $price_d=$div_info->look_down("_tag","p","class","price_d");
	print $price_d->as_text(),"\n";									##当当价
	my $price_m=$div_info->look_down("_tag","p",
								"class","price_m");
	foreach my $node_r ($price_m->content_refs_list){
		next if ref $$node_r;
		print $$node_r,"\n";										##定价
	}
	my $div_detail=$div_info->look_down("_tag","div","class","book_detailed","name","__Property_pub");
	my @elements=$div_detail->find_by_tag_name('p');
	foreach(@elements){
		print $_->as_text(),"\n";									##作者、出版社、出版时间
	}
	my $clear=$div_detail->look_down("_tag","ul","class","clearfix");
	my @lis=$clear->content_list();
	my @spans=$lis[0]->content_list();
	print $spans[1]->as_text(),"\n";								##页数
	@spans=$lis[2]->content_list();
	print $spans[1]->as_text(),"\n";								##ISBN
	print $spans[2]->as_text(),"\n";								##包装
	print "********************************************\n";
	$|=1;
	
	my $dp_main=$div_h1->look_up('_tag','div','class','dp_main');
	my $show_book=$dp_main->look_down('_tag','div','class','show book');
	my $book_pic=$show_book->look_down('_tag','div','class','pic');
	my $img=$book_pic->find_by_tag_name('img');
	my $img_url=$img->attr('src');									##获得图书封面的网页URL
	LWP::Simple::getstore($img_url,"/home/orisun/book_img/$recordId.jpg")				
		or die "get picture failed! -- $url";						##访问页面，直接把内容存为文件
	$|=1;
		
	$root->delete;
}
close LOGFH;
close DATAFH;



实验证明上述方法不可行，程序运行了十几分钟，还没有访问到一个有用的页面。

方法二：根据网页URL的特点--http://product.dangdang.com/product.aspx\?product_id=\d{7,8}&ref=book-\d{2}-\u\d?，自己随机生成URL去访问，如果URL不存在则丢弃，重新生成URL。

sub randURL{
	my $proid=200_000_00+int(rand(800_000_00));
	my $r=int(rand(26));
	my @chars=qw/Q W E R T Y U I O P L K J H G F D S A Z X C V B N M/;
	my $c=$chars[$r];
	my $url=sprintf("http:\/\/product.dangdang.com\/product.aspx?product_id=%d&ref=book-0%d-%s%d",$proid,rand(10),$c,rand(10));
}


由于随机数很长，命中一个存在的URL概率很低，实验证明这种方法更行不通。

方法三：对url实现严格限制。这应该是根据当当图书页面具体情况具体分析的结果。

首先把图书首页上匹配http://product.dangdang.com/product.aspx\?product_id=\d{7,8}&ref=book-\d{2}-\u\d?的链接加入初始URL群；进入一个图书详情页面后当当会相关图书推荐，我们再把图书详情页面上的满足http://product.dangdang.com/product.aspx\?product_id=\d{7,8}&ref=book-\d{2}-\u\d?链接加入待搜索URL群；已经解析过的页面UTRL群中删除。

当程序中止（你总有关机的时候）即Ctrl+C--注意不是终止，把待分析的url写入一个文件，下次开始时继续使用上次没有分析完的URL。

现在终于知道为什么Larbin停止时要用Ctrl+C而不能用Ctrl+Z了，如果Ctrl+Z，Larbin也不能从上次中断的地方继续爬取网页，因为在Linux中，STOP和KILL信号程序员是不能捕获进行处理，咱们只能处理SIGINT。

改进后的代码如下：


#!/usr/bin/perl
use strict;
use warnings;
use LWP 5.64;				##载入新版本的LWP classes
use LWP::Simple;
use LWP::UserAgent;
use LWP::ConnCache;
use HTML::TreeBuilder;
use Unicode::MapUTF8 qw(to_utf8 from_utf8);
use Digest::MD5 qw(md5 md5_hex md5_base64);

sub pause_handler;

BEGIN{			
	$SIG{TERM}=$SIG{INT}=\&pause_handler;
}
my @urls=();						##根据初始URL和页面间的链接关系建立要搜寻的目标URL群体
my $response;
my $html;
sub pause_handler{						##程序中止时@urls写入文件
	open ENDFH,">end" || die "Open end file failed:$!";
	for my $i (0..$#urls){
		print ENDFH $urls[$i],"\n";
	}
	$|=1;
	close ENDFH;
	exit(0);
}

my $browser=LWP::UserAgent->new();									##LWP::UserAgent用来请求页面
$browser->agent('Mozilla/4.0 (compatible; MSIE 5.12; Mac_PowerPC)');##伪装一下
$browser->timeout(10);												##request连接超时为10秒
$browser->protocols_allowed(['http','gopher']);						##只接受 http 和 gopher 协议
$browser->conn_cache(LWP::ConnCache->new());
											
open ENDFH,"end" || die "Open end file failed:$!";			##读取上次积累的urls
my $starturl=<ENDFH>;
while(<ENDFH>){
	chomp;
	push @urls,$_;
}
close ENDFH;	

unless (defined($starturl)){									##如果ENDFH为空，则从首页开始积累urls
	$starturl="http://book.dangdang.com/" ;		
	##先把图书首页上的图书详情页面链接加入@urls
	$response=$browser->get($starturl);
	$html=$response->content;
	$html=to_utf8({-string=>$html,-charset=>'gb2312'});			##把页面编码从gb2312转为utf8
	while($html=~m/href=\"(.*?)\"/ig){							##本页面上的所有链接都加入目标搜索范围
		my $new_url=URI->new_abs($1,$response->base);
		push @urls,$new_url if $new_url=~m{http://product.dangdang.com/product.aspx\?product_id=\d{7,8}&ref=book-\d{2}-\u\d?};		##注意$starturl是没有放入@urls的
	}
}

print "已从$starturl开始爬取网页...\n";

while(scalar @urls>0){
	my $url=$urls[0];
	$|=1;
	$response=$browser->get($url);
	unless ($response->is_success){
		print STDOUT "无法获取$url -- ",$response->status_line,"\n";
		shift @urls;				##把当前url从@urls中删除
		next;
	}
	$|=1;
	
	$html=$response->content;
	$html=to_utf8({-string=>$html,-charset=>'gb2312'});				##把页面编码从gb2312转为utf8
	if(scalar @urls<300){											##控制@urls的容量，避免内存溢出
		while($html=~m/href=\"(.*?)\"/ig){							##本页面上的所有链接都加入目标搜索范围
			my $new_url=URI->new_abs($1,$response->base);
			push @urls,$new_url if $new_url=~m{http://product.dangdang.com/product.aspx\?product_id=\d{7,8}&ref=book-\d{2}-\u\d?};
		}
	}													
	
	my $recordId=md5_base64($url);									##对页面URL计算MD5值，作为图书记录ID		
	my $root=HTML::TreeBuilder->new_from_content($html);			##HTML::TreeBuilder用来解析html文本内容
	my $title=$root->find_by_tag_name('title');
	my $str_title=$title->as_text();
	unless ($str_title=~m/ - 图书 - 当当网$/){
		$root->delete;				##把当前url从@urls中删除
		shift @urls;
		next;
	}
	open DATAFH,">$recordId" || die "Open $recordId file failed:$!";
	print DATAFH "记录ID：$recordId\n";
	print DATAFH "网页地址：$url\n";
	print DATAFH "网页标题：$str_title\n";
	my $div_h1=$root->look_down("_tag","div","class","h1_title book_head");
	my $h1=$div_h1->look_down('_tag','h1');							##图书标题
	print DATAFH "图书名称：",$h1->as_text(),"\n";
	my $div_info=$root->look_down("_tag","div","class","info book_r");
	my $price_d=$div_info->look_down("_tag","p","class","price_d");
	print DATAFH $price_d->as_text(),"\n";									##当当价
	my $price_m=$div_info->look_down("_tag","p",
								"class","price_m");
	foreach my $node_r ($price_m->content_refs_list){
		next if ref $$node_r;
		print DATAFH $$node_r,"\n";										##定价
	}
	my $div_detail=$div_info->look_down("_tag","div","class","book_detailed","name","__Property_pub");
	my @elements=$div_detail->find_by_tag_name('p');
	foreach(@elements){
		print DATAFH $_->as_text(),"\n";									##作者、出版社、出版时间
	}
	my $clear=$div_detail->look_down("_tag","ul","class","clearfix");
	my @lis=$clear->content_list();
	my @spans=$lis[0]->content_list();
	print DATAFH $spans[1]->as_text(),"\n";								##页数
	@spans=$lis[2]->content_list();
	print DATAFH $spans[1]->as_text(),"\n";								##ISBN
	print DATAFH $spans[2]->as_text(),"\n";								##包装
	$|=1;
	close DATAFH;
	
	my $dp_main=$div_h1->look_up('_tag','div','class','dp_main');
	my $show_book=$dp_main->look_down('_tag','div','class','show book');
	my $book_pic=$show_book->look_down('_tag','div','class','pic');
	my $img=$book_pic->find_by_tag_name('img');
	my $img_url=$img->attr('src');									##获得图书封面的网页URL
	LWP::Simple::getstore($img_url,"./book_img/$recordId.jpg")				
		or die "get picture failed! -- $url";						##访问页面，直接把内容存为文件		
	$root->delete;
	
	shift @urls;					##对本URL的解析工作完全完成后才把它从@urls中删除
}



其实方法三还有明显的缺陷：
- 首页上的图书详情链接是有限，即初始种群的多样性还不够。
- 当当的图书相关性推荐条目很少，而相关性很大，很容易书A的页面上推荐书B，书B的页面上推荐书A，形成死循环。而且因@urls中本来有很多其他元素，所以这种循环周期较长，不容易被发现。
- 为了增加种群多样性，避免死循环或者延长循环周期我们就需要增大@urls的容量，如果这个容量相当大该如何存储这个数组，用位图散列？要不要保证@urls中元素无重复？












