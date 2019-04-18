# Perl类、CGI和DBI编程 - Orisun - 博客园







# [Perl类、CGI和DBI编程](https://www.cnblogs.com/zhangchaoyang/articles/2051164.html)





方法是一种属于类的特殊子例程，要求第一个参数必须是包含或指向对象的引用，并且这个参数是由perl自动隐式赋值的。

House.pm

package House;
sub new{
	my $class=shift;
	my $ref={};
	bless($ref,$class);
	return $ref;
}
sub set_data{
	my $self=shift;
	my $key=shift;
	my $value=shift;
	$self->{$key}=$value;
}
sub get_data{
	my $self=shift;
	my $key=shift;
	$self->{$key};
}
1;


同目录下建立Perl文件useHouse

#!/usr/bin/perl
use 5.010;					##say相当于print外加换行符，但必须申明使用Perl5.1的新特征
require ("House.pm");		##引入外部文件
my $house=new House("Bieshu");
say ref($house);			##ref函数返回类名--House
$house->set_data("name","balihaoting");
say $house->get_data("name");


在Apache服务目录下有个htdocs，用来存放html文件，form.html

<html>
<head><title>CGI Form</title></head>
<bod>
<form action="cgi-bin/form" method=get>
	Name:<br>
	<input type="text" size=25 name="name"><br>
	Phone:<br>
	<input type="text" size=25 name="phone"><br>
	<input type=submit value="Submit">
	<input type=reset value="Clear">
</form>
</body>
</html>


cgi-bin目录下用来存放脚本文件，form

#!/usr/bin/perl
print "Content-type:text/html\n\n";
@key_value=split(/&/,$ENV{QUERY_STRING});
foreach $pair (@key_value){
	$pair=~tr/+/ /;
	($key,$value)=split(/=/,$pair);
	print "\t$key:$value<br>";
}


下面给一个在同一个perl程序中显示页面和处理响应的例子：

#!/usr/bin/perl
use CGI qw(:standard);
print header("text/html;charset=gb2312");
print start_html(-title=>'Function-Oriented Syntax',-bgcolor=>'silver');
print 
	h1("Let's hear from you!"),
	h2("I'm intersted."),
	start_form,
		"what's your name?",textfield('Name'),
		p,
		"what's you occupation?",textfield('Job'),
		p,
		"select a vacation spot.",popup_menu(
			-name=>'Place',
			-value=>['Hawaii','Europe','Mexico','Japan'],
		),
		p,
		"Do you want a green balloon?",
		br,
		checkbox(-name=>"Choice",-label=>'If yes,check this box'),
		p,
		submit("Submit"),
	end_form;
print hr;

if(param()){
	print "Your name is ",em(param('Name')),	
	p,
	"Your occupation is ",em(param('Job')),
	p,
	"Your vacation spot is ",em(param('Place')),
	p;
	if(param('Choice') eq "on"){
		print "You will receive a green balloon shortly!";
	}
	print hr;
}
print end_html;


DBI编程示例：

#!/usr/bin/perl
use DBI qw/:sql_types/;
my $dbh=DBI->connect('dbi:mysql:mysql','root','passwd',
        {
            PrintError=>0,
            RaiseError=>1,
            AutoCommit=>0,
        }
        )or die "Connect failed:$DGI::errstr";
my @rows=(
        ['zcy',3,4],
        ['orisun',34,5],
        ['zsir',4,1]
        );
my $sql=qq{INSERT INTO tb_sample VALUES(?,?,?)};
my $sth=$dbh->prepare($sql);
foreach $param (@rows){
    eval{$sth->bind_param(1,$param->[0],SQL_VARCHAR);
        $sth->bind_param(2,$param->[1],SQL_INTEGER);
        $sth->bind_param(3,$param->[2],SQL_INTEGER);
        $sth->execute() or die "Insert data failed:$DBI::errstr.";
    };
}
if($@){         ##如果eval执行失败，则$@为true
    warn "database error:$DBI::errstr\n";
    $dbh->rollback();
}
else{
    $dbh->commit();

    my $name='zcy';
    $name=$dbh->quote($name);       ##处理引号
    $sth=$dbh->prepare("select * from tb_sample where name=$name");
    $sth->execute();
#   $sth->dump_results();       ##直接全部输出查询结果
    while(my $row=$sth->fetchrow_hashref()){
        print "Name=$row->{name}\t";
        print "Height=$row->{height}\t";
        print "Weight=$row->{weight}\n";
    }
}
$sth->finish();
$dbh->disconnect();













