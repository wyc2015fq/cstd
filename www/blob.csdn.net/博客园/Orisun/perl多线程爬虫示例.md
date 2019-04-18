# perl多线程爬虫示例 - Orisun - 博客园







# [perl多线程爬虫示例](https://www.cnblogs.com/zhangchaoyang/articles/2059168.html)





代码是从网上找的，注释是我加的

#!/usr/bin/perl
use strict;
use warnings;

use threads;
use threads::shared;
use Thread::Queue;
use Thread::Semaphore;

use Bloom::Filter;
use URI;
use URI::URL;
use Web::Scraper;
use LWP::Simple;

use LWP::UserAgent;
use HTTP::Cookies;

#use HTTP::Cookies::Guess;

use String::Diff;
use String::Diff qw(diff_fully diff diff_merge diff_regexp);
use URI::Split qw(uri_split uri_join);

my $fid : shared;#下载的页面以递增的数字命名
share($fid);	#多线程共享该变量
$fid=0;

#crawling with signed cookie
my $cookie_jar = '.mozilla/firefox/bg146ia6.default/cookies.sqlite';

my $tmp_ua = LWP::UserAgent->new;    #UserAgent用来发送网页访问请求
$tmp_ua->timeout(15);                ##连接超时时间设为15秒
$tmp_ua->protocols_allowed( [ 'http', 'https' ] ); ##只允许http和https协议
$tmp_ua->agent(
"Mozilla/4.0 (compatible; MSIE 6.0; Windows NT 5.1; SV1; .NET CLR 2.0.50727;.NET CLR 3.0.04506.30; .NET CLR 3.0.4506.2152; .NET CLR 3.5.30729)"
  )
  ; ##用来在header中告诉服务器你用的是什么"浏览器"，设置文件头的User-Agent

$tmp_ua->cookie_jar(HTTP::Cookies->new('file'=>"$ENV{'HOME'}/$cookie_jar",'autosave'=>1));
# 设置cookie，在运行过程中必须执行两个方法，extract_cookies($request) 和 add_cookie_header($response)。在运行的过程中实际用到了HTTP::Cookies模块。如：
# $ua->cookie_jar({ file => "$ENV{HOME}/.cookies.txt" });
# 等价于
# require HTTP::Cookies;
# $ua->cookie_jar(HTTP::Cookies->new(file => "$ENV{HOME}/.cookies.txt"));

push @{$tmp_ua->requests_redirectable}, 'POST';#告诉LWP在POST请求发送后如果发生重新定向就自动跟随

my $max_threads = 5;

my $base_url = $ARGV[0] || 'http://www.cnblogs.com/zhangchaoyang/';
my $host = URI::URL->new($base_url)->host;
print "Host Name: $host.\n";

my $queue = Thread::Queue->new( );		#线程队列，每个线程负责去处理一个url

my $semaphore = Thread::Semaphore->new( $max_threads );
my $mutex = Thread::Semaphore->new( 1 );


#my ($sec,$min,$hour,$mday,$mon,$year,$wday,$yday,$isdst) = localtime;
#my $logfile = "crawler".($year+1900).($mon+1).$mday.$hour.$min.$sec.".log";
#open(BANLOG,">>$logfile") or die("can't open logfile:$!\n");

# Bloom::Filter使用更少的内存采用一种基于概率的算法来进行存在性测试。
my $filter = shared_clone( Bloom::Filter->new(capacity => 1000000, error_rate => 0.001) );

$queue->enqueue( $base_url );		#放入线程队列的URL就要被线程所处理
$filter->add( $base_url );			#放入filter中好判断该URL是否已经存在

my @tmp_url = ();					#@tmp_url存在处理过的url
push(@tmp_url,$base_url);

while( 1 )
{
    # join所有可以被join的线程
    #my $joined = 0;
    foreach ( threads->list(threads::joinable) )
    {
        #$joined ++;
        $_->join( );
    }
    #print $joined, " joinedn";

    # if there are no url need process.
    my $item = $queue->pending();#返回队列中url的个数
    # 线程队列为空
    if( $item == 0 )
    {
        my $active = threads->list(threads::running);
        # 已经没有active线程了，结束所有的工作
        if( $active == 0 )
        {
            print "All done!\n";
            last;
        }
        # 如果还有活动线程，那么主线程sleep，等待处理URL的子线程结束
        else
        {
            #print "[MAIN] 0 URL, but $active active threadn";
            sleep 1;
            next;
        }
    }

    # 线程队列不为空，信号量减1,占用一个线程来处理url
    #print "[MAIN] $item URLn";
    $semaphore->down;
    #print "[MAIN]Create thread.n";
    threads->create( \&ProcessUrl );
}

# join all threads which can be joined
foreach ( threads->list() )
{
    $_->join( );
}

sub ProcessUrl
{
    my $scraper = scraper
    {
            process '//a', 'links[]' => '@href';#根据XPath表达式寻找所有的标签a,把href属性存到散列的value中
    };

    my $res;
    my $link;

    while( my $url = $queue->dequeue_nb() )
    {
        eval#eval BLOCK，BLOCK只会被解析一次，并且在编译时进行代码语法检查。
       {
       		print "开始下载",URI->new($url)->as_string,"\t\$fid=$fid\n";
       		LWP::Simple::getstore(URI->new($url)->as_string,"$ENV{'HOME'}/master/cnblog/cn$fid") or print "Can't download the web page.";
       		$fid+=1;
            $scraper->user_agent($tmp_ua);#设置$scraper的user_agent
            $res = $scraper->scrape( URI->new($url) )->{'links'};#把URI传给scrape函数。scrape函数返回一个数组引用，因为links是数组
        };
        if( $@ )# 当BLOCK中有语法错误、运行时错误遇到 die 语句， eval 将返回 undef 。错误码被保存在 $@ 中。
        {
            warn "$@\n";
            next;
        }
        next if (! defined $res );#如果HTML文档中没有发现a标签

        #print "there are ".scalar(threads->list(threads::running))." threads, ", $queue->pending(), " urls need process.n";

        foreach( @{$res} )
        {
            # $_ => URI->new("http://example.com/")     所以要调用sa_string来获取"http://example.com/"
            $link = $_->as_string;
            $link = URI::URL->new($link, $url);
			#$u1 = URI::URL->new($str, $base);
			#$u2 = $u1->abs;

            # not http and not https?
            next if( $link->scheme ne 'http' && $link->scheme ne 'https' );
			#The three forms of URI reference syntax are summarized as follows:
			#<scheme>:<scheme-specific-part>#<fragment>
			#<scheme>://<authority><path>?<query>#<fragment>
			#<path>?<query>#<fragment>
			#可以通过URL::Split把名个部分分离出来
            # another domain?
            # next if( $link->host ne $host );

            #search for the sub domain
            next if(!($link->host =~ /$host/));

            $link = $link->abs->as_string;#获得绝对路径

            if( $link =~ /(.*?)#(.*)/ )#去除书签锚点，即#以后的内容
            {
                $link = $1;
            }

            next if( $link =~ /rss|.(jpg|png|bmp|mp3|wma|wmv|gz|zip|rar|iso|pdf)$/i );#这些文件格式我们不抓取


            #print "test:$link\n";

            #EscapeUrl,skip query form values
            my $tmp_link = &EscapeUrl($link);#$tmp_link中已经把查询参数的值去掉了
            #print "Escape:".$tmp_link."\n";

            $mutex->down(); #互质体减1,进入线程临界资源区
            my $tmp_mark = 0;

            #print "test start:$link\n";
            if( ! $filter->check($tmp_link) )	#如果$tmp_link不在$filter中
            {
                #print "Test filter ok:$tmp_link\n";

                #DiffUrl,diff $link from queue with number

                foreach(@tmp_url)
                {
                    #print "Test Queue:".$tmpurl."\n";
                    #print "test-1:$_\ntest-2:$tmp_link\n";
                    if(&DiffUrl($_,$link))#如果发现@tmp_url中的url和当前页面中的一个链接url仅是在某些数字上不同（很可能是查询参数值不同），则跳过该链接，即跳到else里面去。
                    {
                        $tmp_mark = 2;
                        last;
                    }
                }

                if( $tmp_mark != 2 )
                {
                    $queue->enqueue($link);		#把页面上的链接$link交给线程进行处理
                    #print "add queue:$link\n";
                    $filter->add($tmp_link);#$tmp_link放入$filter
                    #print "add filter:$tmp_link\n";
                    #print BANLOG $filter->key_count(), " ", $link, "\n";
                    #print $filter->key_count(), " ", $link, "\n";

                    push(@tmp_url,$link);#把$link放入已处理的url数组@tmp_url
                }
                else
                {
                    #print "pass:$link\n";#$link被忽略
                }
            }
            #print "pass:$link\n";

            $mutex->up();#互斥信号量加1
            undef $link;
        }
        undef $res;#清除创建的一些object，否则在while循环中这些object越积越多
	}
    undef $scraper;
    $semaphore->up( );##普通信号量加1
}

#close(BANLOG);
print "ALL DONE.\n";

#把URL尾部的request参数置为空
#比如http://category.dangdang.com/?ref=www-0-C&name=orisun-zhang#ref=www-0-C被处理为http://category.dangdang.com/?ref=&name=
sub EscapeUrl
{
    my $urlold = shift;
    my ($scheme,$auth,$path,$query,$frag) = uri_split($urlold);#把一个url的各部分分离出来
    my $urlnew = uri_join($scheme,$auth,$path);

    my $u = URI->new($urlold);
    my @tmp_array = $u->query_form();
    my $tmp = '';
    my $i = 0;
    for($i=0;$i<@tmp_array;$i+=2)#把request参数的值去掉
    {
        $tmp .=$tmp_array[$i]."=&";
    }
    if(@tmp_array != 0)
    {
        $tmp =~ s/&$//;
        $urlnew .= "?".$tmp;
    }
    undef $u;#清除子例程中创建的object
    #print $urlnew."\n";
    return $urlnew;
}

sub DiffUrl
{
    my $urlold = shift;
    my $urlnew = shift;
    my $urloldx = &EscapeUrl($urlold); 
    my $urlnewx = &EscapeUrl($urlnew);

    my($old,$new) = String::Diff::diff($urloldx,$urlnewx);
    #my($old,$new) = String::Diff::diff($urlold,$urlnew);
    if (($old =~ m/(\[\d+\])/i) && ($new =~ m/{\d+}/i))			#如果两个url仅是在某些数字上不同
    #if ($new =~ m/{\d+}/i)
    {
        #print "test num success.\n";
        return 1;
    }
    else
    {
        #print "test num failed.\n";
        return 0;
    }
}













