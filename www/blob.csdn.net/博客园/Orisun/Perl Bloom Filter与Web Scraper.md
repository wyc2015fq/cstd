# Perl Bloom::Filter与Web::Scraper - Orisun - 博客园







# [Perl Bloom::Filter与Web::Scraper](https://www.cnblogs.com/zhangchaoyang/articles/2058200.html)





安装Bloom::Filter之前要先安装Digest::SHA1

#!/usr/bin/perl
use Bloom::Filter;
my $bf=Bloom::Filter->new(capacity => 10,error_rate => .001);
my @keys=qw/a s d f g h j k l z/;
$bf->add(@keys);
while(<>){
    chomp;
    print "Found $_\n" if $bf->check($_);
}

$bf=Bloom::Filter->new(capacity => 10,error_rate => .001);

创建一个Bloom::Filter，其最大容量为10,判断某一个元素是否在容器中的最大出错概率为0.001。

$bf->add(@keys); 向容器中添加元素。

$bf->check($_)

判断某一个元素是否在容器中。

<<===========================================================================>>

快速掠一下下面的代码，初次看是很难看懂。

use URI;
use Web::Scraper;

# First, create your scraper block
my $tweets = scraper {
     # Parse all LIs with the class "status", store them into a resulting
     # array 'tweets'.  We embed another scraper for each tweet.
     process "li.status", "tweets[]" => scraper {
         # And, in that array, pull in the elementy with the class
         # "entry-content", "entry-date" and the link
         process ".entry-content", body => 'TEXT';
         process ".entry-date", when => 'TEXT';
         process 'a[rel="bookmark"]', link => '@href';
     };
 };

 my $res = $tweets->scrape( URI->new("http://twitter.com/miyagawa") );

 # The result has the populated tweets array
 for my $tweet (@{$res->{tweets}}) {
     print "$tweet->{body} $tweet->{when} (link: $tweet->{link})\n";
 }

The structure would resemble this (visually)
 {
   tweets => [
     { body => $body, when => $date, link => $uri },
     { body => $body, when => $date, link => $uri },
   ]
 }

Web::Scaper是一组用来提取HTML文档中元素内容的工具集，它能够理解HTML和CSS选择器以及XPath表达式。

$res = $scraper->scrape(URI->new($uri));
$res = $scraper->scrape($html_content);  
$res = $scraper->scrape(\$html_content);  
$res = $scraper->scrape($http_response);  
$res = $scraper->scrape($html_element);

如果你传入的参数是URI或HTTP response，那Web::Scaper自动去寻找Content-Type header和META标签以判断文件编码。否则你压根先把HTML内容解码为Unicode后再传给scape函数。

当你把HTML内容作为参数传给scrape函数时，你还要考虑一个问题：HTML文档中出现 相对路径怎么办？所以这个时候你可以把base url一并作为参数传进去。

$res = $scraper->scrape($html_content, "http://example.com/foo");

process函数用来根据给定的HTML或CSS选择器或XPath表达式来匹配元素，把text或attributes抽取到result中。

scraper { process "tag.class",key => 'TEXT';

　　　　process '//tag[contains(@foo, "bar")]', key2 => '@attr';

};

它有两个参数，当第一个参数以"//"或"id("开头时作为XPath对待；否则作为HTML或CSS选择器对待。

# <span class="date">2008/12/21</span>
# date => "2008/12/21"
process ".date", date => 'TEXT';　　　　# CSS的class是date

# <div class="body"><a href="http://example.com/">foo</a></div>
# link => URI->new("http://example.com/")
process ".body > a", link => '@href';　　　　# href是个属性

# <div class="body"><a href="http://example.com/">foo</a></div>
# link => URI->new("http://example.com/"), text => "foo"
process ".body > a", link => '@href', text => 'TEXT';　　# TEXT指标签内真正的内容

# <ul><li>foo</li><li>bar</li></ul>
# list => [ "foo", "bar" ]
process "li", "list[]" => "TEXT";　　　　# []返回数组

# <ul><li id="1">foo</li><li id="2">bar</li></ul>
# list => [ { id => "1", text => "foo" }, { id => "2", text => "bar" } ];
process "li", "list[]" => { id => '@id', text => "TEXT" };　　　　#返回散列数组












