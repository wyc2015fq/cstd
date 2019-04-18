# 使用Bugzilla，你肯定会遇到的坑。 - qq_42302962的博客 - CSDN博客
2018年06月01日 09:36:32[Linux高级开发](https://me.csdn.net/qq_42302962)阅读数：436
最近和几个朋友一起做用户态协议栈开源项目 [NtyTcp](https://github.com/wangbojing/NtyTcp) ，纯业余爱好，个人情怀。刚刚开始做有好多的Bug，故搭建了一个Bugzilla，用来上传Bug的。
有几个开发朋友想在 [bug.ntytcp.com](http://47.106.79.26:8080/)提交bug，不能注册，因为不能发邮件。至于为什么不能发送邮件？因为阿里云把25端口封了，使用smtp不能发送。
使用465端口发送，发现发不出来，又找不到原因。硬着头皮比Bugzilla的源码看了一下。可恨的是自己并没有写过perl代码。
于是用了两天时间连学带看就把bugzilla的源码看了一遍。然后把bugzilla邮件发送的部分重写了一下。先把bugzilla邮件发送部分贴出来。
bugzilla/mailer.pm
```
`sub``MessageToMTA {`
`my``(``$msg``, ``$send_now``) = (``@_``);`
`my``$method``= Bugzilla->params->{``'mail_delivery_method'``};`
`return``if``$method``eq ``'None'``;`
`if``(Bugzilla->params->{``'use_mailer_queue'``}`
`&& ! ``$send_now`
`&& ! Bugzilla->dbh->bz_in_transaction()`
`) {`
`Bugzilla->job_queue->insert(``'send_mail'``, { msg => ``$msg``});`
`return``;`
`}`
`my``$dbh``= Bugzilla->dbh;`
`my``$email``= ``ref``(``$msg``) ? ``$msg``: Bugzilla::MIME->new(``$msg``);`
`# If we're called from within a transaction, we don't want to send the`
`# email immediately, in case the transaction is rolled back. Instead we`
`# insert it into the mail_staging table, and bz_commit_transaction calls`
`# send_staged_mail() after the transaction is committed.`
`if``(! ``$send_now``&& ``$dbh``->bz_in_transaction()) {`
`# The e-mail string may contain tainted values.`
`my``$string``= ``$email``->as_string;`
`trick_taint(``$string``);`
`my``$sth``= ``$dbh``->prepare(``"INSERT INTO mail_staging (message) VALUES (?)"``);`
`$sth``->bind_param(1, ``$string``, ``$dbh``->BLOB_TYPE);`
`$sth``->execute;`
`return``;`
`}`
`my``$from``= ``$email``->header(``'From'``);`
`my``$hostname``;`
`my``$transport``;`
`if``(``$method``eq ``"Sendmail"``) {`
`if``(ON_WINDOWS) {`
`$transport``= Bugzilla::Sender::Transport::Sendmail->new({ sendmail => SENDMAIL_EXE });`
`}`
`else``{`
`$transport``= Bugzilla::Sender::Transport::Sendmail->new();`
`}`
`}`
`else``{`
`# Sendmail will automatically append our hostname to the From`
`# address, but other mailers won't.`
`my``$urlbase``= Bugzilla->params->{``'urlbase'``};`
`$urlbase``=~ m|//([^:/]+)[:/]?|;`
`$hostname``= $1 || ``'localhost'``;`
`$from``.= ``"\@$hostname"``if``$from``!~ /@/;`
`$email``->header_set(``'From'``, ``$from``);`
`# Sendmail adds a Date: header also, but others may not.`
`if``(!``defined``$email``->header(``'Date'``)) {`
`$email``->header_set(``'Date'``, time2str(``"%a, %d %b %Y %T %z"``, ``time``()));`
`}`
`}`
`if``(``$method``eq ``"SMTP"``) {`
`my``(``$host``, ``$port``) = ``split``(/:/, Bugzilla->params->{``'smtpserver'``}, 2);`
`$transport``= Bugzilla->request_cache->{smtp} //=`
`Email::Sender::Transport::SMTP::Persistent->new({`
`host  => ``$host``,`
`defined``(``$port``) ? (port => ``$port``) : (),`
`sasl_username => Bugzilla->params->{``'smtp_username'``},`
`sasl_password => Bugzilla->params->{``'smtp_password'``},`
`helo => ``$hostname``,`
`ssl => Bugzilla->params->{``'smtp_ssl'``},`
`debug => Bugzilla->params->{``'smtp_debug'``} });`
`}`
`Bugzilla::Hook::process(``'mailer_before_send'``, { email => ``$email``});`
`return``if``$email``->header(``'to'``) eq ``''``;`
`if``(``$method``eq ``"Test"``) {`
`my``$filename``= bz_locations()->{``'datadir'``} . ``'/mailer.testfile'``;`
`open``TESTFILE, ``'>>'``, ``$filename``;`
`# From - <date> is required to be a valid mbox file.`
`print``TESTFILE ``"\n\nFrom - "``. ``$email``->header(``'Date'``) . ``"\n"``. ``$email``->as_string;`
`close``TESTFILE;`
`}`
`else``{`
`# This is useful for Sendmail, so we put it out here.`
`local``$ENV``{PATH} = SENDMAIL_PATH;`
`eval``{ sendmail(``$email``, { transport => ``$transport``}) };`
`if``($@) {`
`ThrowCodeError(``'mail_send_error'``, { msg => $@->message, mail => ``$email``});`
`}`
`}`
`}`
```
使用的sendmail($email, {transport=> $transport}), 发送。由于系统的sendmail没有配置好，发送不出来。
自己写了一版单独用perl发送邮件的。
```
`#!/usr/bin/perl`
`use``Net::SMTP::SSL;`
`use``MIME::Base64;`
`use``MIME::Lite;`
`my``$msg``= MIME::Lite->new(`
`From``=>``'from@host.com'``,`
`To``=>``'to@host.com'``,`
`Subject``=>``'First Email'``,`
`Data``=>``'nihao'``,`
`Type``=>``'text/html'`
`);`
`$account``=``"from@host.com"``;`
`$password``=``"your password"``;`
`$smtp``= Net::SMTP::SSL->new(`
`Host => ``'smtp.host.com'``,`
`Port => 465,`
`Timeout => 120,`
`Debug``=>1`
`); ``# connect to an SMTP server`
`die``"Couldn't open connection: $!"``if``(!``defined``$smtp``);`
`#$smtp->auth($account,$password);`
`$smtp``->datasend(``"AUTH LOGIN\r\n"``);`
`$smtp``->datasend(encode_base64(``'from@host.com'``)); ``# username`
`$smtp``->datasend(encode_base64(``'your password'``)); ``# password`
`$smtp``->mail(``'from@host.com'``);`
`$smtp``->to(``'to@host.com'``);`
`$smtp``->data();`
`$smtp``->datasend(``$msg``->as_string());`
`$smtp``->datasend(``"Blah\r\n\r\n"``);`
`$smtp``->dataend();`
```
相信从代码风格来看，就是第一次写perl   。^_^  。 ^_^
这版是可以发送邮件的。
但是跟bugzilla的mailer.pm 发送差别很大。没办法整合。
于是又换了一种写法。
```
`#!/usr/bin/perl`
`use``strict;`
`use``warnings;`
`use``Email::Sender::Simple ``qw(sendmail)``;`
`use``Email::Sender::Transport::SMTP ();`
`use``Email::Simple ();`
`use``Email::Simple::Creator ();`
`use``Email::Sender::Transport::SMTP::TLS;`
`use``Email::Sender::Transport::SMTP::Persistent;`
`use``Email::Sender::Transport::SMTPS;`
`use``Net::SMTP::SSL;`
`use``MIME::Base64;`
`my``$smtpserver``= ``'smtp.host.com'``;`
`my``$smtpport``= 465;`
`my``$smtpuser``= ``'from@from.com'``;`
`my``$smtppassword``= ``'your password'``;`
`my``$smtpto``= ``'to@host.com'``;`
`my``$hostname``= ``'localhost.localdomain'``;`
`my``$transport``= Email::Sender::Transport::SMTP::Persistent->new({`
`            ``host  => ``$smtpserver``,`
`            ``defined``(``$smtpport``) ? (port => ``$smtpport``) : (),`
`            ``sasl_username => ``$smtpuser``,`
`            ``sasl_password => ``$smtpuser``,`
`            ``helo => ``$hostname``,`
`            ``ssl => 1,`
`            ``debug => 1 });`
`my``$email``= Email::Simple->create(`
`  ``header => [`
`    ``To      => ``$smtpto``,`
`    ``From    => ``$smtpuser``,`
`    ``Subject => ``'Hi!'``,`
`  ``],`
`  ``body => ``"This is my message\n"``,`
`);`
`#try {`
`#eval {sendmail($email, { transport => $transport }) };`
`#}catch {`
`#die "Error sending email: $_";`
`my``$smtp``= Net::SMTP::SSL->new(`
`            ``Host  => ``$smtpserver``,`
`            ``Port => ``$smtpport``,`
`            ``Timeout => 120,`
`            ``Debug => 1`
`        ``);`
`        ``die``"Couldn't open connection: $!"``if``(!``defined``$smtp``);`
`        ``#$smtp->auth(Bugzilla->params->{'smtp_username'}, Bugzilla->params->{'smtp_password'});`
`        ``$smtp``->datasend(``"AUTH LOGIN\n"``);`
`        ``$smtp``->datasend(encode_base64(``$smtpuser``));`
`        ``$smtp``->datasend(encode_base64(``$smtppassword``));`
`        ``$smtp``->mail(``$smtpuser``);`
`        ``$smtp``->to(``$smtpto``);`
`        ``$smtp``->data();`
`        ``$smtp``->datasend(``$email``->as_string());`
`        ``$smtp``->datasend(``"\r\n\r\n"``);`
`        ``$smtp``->dataend();`
`        ``$smtp``->quit();`
```
这个代码的风格就像那么回事了，就写过perl代码的人了 。^_^。^_^
将mailer.pm 的代码整合。
```
`sub``MessageToMTA {`
`    ``my``(``$msg``, ``$send_now``) = (``@_``);`
`    ``my``$method``= Bugzilla->params->{``'mail_delivery_method'``};`
`    ``return``if``$method``eq ``'None'``;`
`    ``if``(Bugzilla->params->{``'use_mailer_queue'``}`
`        ``&& ! ``$send_now`
`        ``&& ! Bugzilla->dbh->bz_in_transaction()`
`    ``) {`
`        ``Bugzilla->job_queue->insert(``'send_mail'``, { msg => ``$msg``});`
`        ``return``;`
`    ``}`
`    ``my``$dbh``= Bugzilla->dbh;`
`    ``my``$email``= ``ref``(``$msg``) ? ``$msg``: Bugzilla::MIME->new(``$msg``);`
`    ``# If we're called from within a transaction, we don't want to send the`
`    ``# email immediately, in case the transaction is rolled back. Instead we`
`    ``# insert it into the mail_staging table, and bz_commit_transaction calls`
`    ``# send_staged_mail() after the transaction is committed.`
`    ``if``(! ``$send_now``&& ``$dbh``->bz_in_transaction()) {`
`        ``# The e-mail string may contain tainted values.`
`        ``my``$string``= ``$email``->as_string;`
`        ``trick_taint(``$string``);`
`        ``my``$sth``= ``$dbh``->prepare(``"INSERT INTO mail_staging (message) VALUES (?)"``);`
`        ``$sth``->bind_param(1, ``$string``, ``$dbh``->BLOB_TYPE);`
`        ``$sth``->execute;`
`        ``return``;`
`    ``}`
`    ``my``$from``= ``$email``->header(``'From'``);`
`    ``my``$hostname``;`
`    ``my``$transport``;`
`    ``if``(``$method``eq ``"Sendmail"``) {`
`        ``if``(ON_WINDOWS) {`
`            ``$transport``= Bugzilla::Sender::Transport::Sendmail->new({ sendmail => SENDMAIL_EXE });`
`        ``}`
`        ``else``{`
`            ``$transport``= Bugzilla::Sender::Transport::Sendmail->new();`
`        ``}`
`    ``}`
`    ``else``{`
`        ``# Sendmail will automatically append our hostname to the From`
`        ``# address, but other mailers won't.`
`        ``my``$urlbase``= Bugzilla->params->{``'urlbase'``};`
`        ``$urlbase``=~ m|//([^:/]+)[:/]?|;`
`        ``$hostname``= $1 || ``'localhost'``;`
`        ``$from``.= ``"\@$hostname"``if``$from``!~ /@/;`
`        ``$email``->header_set(``'From'``, ``$from``);`
`        ``# Sendmail adds a Date: header also, but others may not.`
`        ``if``(!``defined``$email``->header(``'Date'``)) {`
`            ``$email``->header_set(``'Date'``, time2str(``"%a, %d %b %Y %T %z"``, ``time``()));`
`        ``}`
`    ``}`
`    ``my``(``$host``, ``$port``) = ``split``(/:/, Bugzilla->params->{``'smtpserver'``}, 2);`
`    ``if``(``$method``eq ``"SMTP"``) {`
`#        my ($host, $port) = split(/:/, Bugzilla->params->{'smtpserver'}, 2);`
`        ``$transport``= Bugzilla->request_cache->{smtp} //=`
`          ``Email::Sender::Transport::SMTP::Persistent->new({`
`            ``host  => ``$host``,`
`            ``defined``(``$port``) ? (port => ``$port``) : (),`
`            ``sasl_username => Bugzilla->params->{``'smtp_username'``},`
`            ``sasl_password => Bugzilla->params->{``'smtp_password'``},`
`            ``helo => ``$hostname``,`
`            ``ssl => Bugzilla->params->{``'smtp_ssl'``},`
`            ``debug => Bugzilla->params->{``'smtp_debug'``} });`
`    ``}`
`    ``Bugzilla::Hook::process(``'mailer_before_send'``, { email => ``$email``});`
`    ``return``if``$email``->header(``'to'``) eq ``''``;`
`    ``if``(``$method``eq ``"Test"``) {`
`        ``my``$filename``= bz_locations()->{``'datadir'``} . ``'/mailer.testfile'``;`
`        ``open``TESTFILE, ``'>>'``, ``$filename``;`
`        ``# From - <date> is required to be a valid mbox file.`
`        ``print``TESTFILE ``"\n\nFrom - "``. ``$email``->header(``'Date'``) . ``"\n"``. ``$email``->as_string;`
`        ``close``TESTFILE;`
`    ``}`
`    ``else``{`
`        ``# This is useful for Sendmail, so we put it out here.`
`#        local $ENV{PATH} = SENDMAIL_PATH;`
`#        eval { sendmail($email, { transport => $transport }) };`
`#        if ($@) {`
`#            ThrowCodeError('mail_send_error', { msg => $@->message, mail => $email });`
`#        }`
`        ``my``$smtp``= Net::SMTP::SSL->new(`
`            ``Host  => ``$host``,`
`            ``Port => ``$port``,`
`            ``Timeout => 120,`
`            ``Debug => 1`
`        ``);`
`        ``die``"Couldn't open connection: $!"``if``(!``defined``$smtp``);`
`        ``#$smtp->auth(Bugzilla->params->{'smtp_username'}, Bugzilla->params->{'smtp_password'});`
`        ``$smtp``->datasend(``"AUTH LOGIN\n"``);`
`        ``$smtp``->datasend(encode_base64(Bugzilla->params->{``'smtp_username'``}));`
`        ``$smtp``->datasend(encode_base64(Bugzilla->params->{``'smtp_password'``}));`
`        ``$smtp``->mail(Bugzilla->params->{``'smtp_username'``});`
`        ``$smtp``->to(``$email``->header(``'to'``));`
`        ``$smtp``->data();`
`        ``$smtp``->datasend(``$email``->as_string());`
`        ``$smtp``->datasend(``"\r\n\r\n"``);`
`        ``$smtp``->dataend();`
`        ``$smtp``->quit();`
`    ``}`
`}`
```
主要是将邮件发送方式修改了。
```
`        ``local``$ENV``{PATH} = SENDMAIL_PATH;`
`        ``eval``{ sendmail(``$email``, { transport => ``$transport``}) };`
`        ``if``($@) {`
`            ``ThrowCodeError(``'mail_send_error'``, { msg => $@->message, mail => ``$email``});`
`        ``}`
```
换成了
```
`        ``my``$smtp``= Net::SMTP::SSL->new(`
`            ``Host  => ``$host``,`
`            ``Port => ``$port``,`
`            ``Timeout => 120,`
`            ``Debug => 1`
`        ``);`
`        ``die``"Couldn't open connection: $!"``if``(!``defined``$smtp``);`
`        ``#$smtp->auth(Bugzilla->params->{'smtp_username'}, Bugzilla->params->{'smtp_password'});`
`        ``$smtp``->datasend(``"AUTH LOGIN\n"``);`
`        ``$smtp``->datasend(encode_base64(Bugzilla->params->{``'smtp_username'``}));`
`        ``$smtp``->datasend(encode_base64(Bugzilla->params->{``'smtp_password'``}));`
`        ``$smtp``->mail(Bugzilla->params->{``'smtp_username'``});`
`        ``$smtp``->to(``$email``->header(``'to'``));`
`        ``$smtp``->data();`
`        ``$smtp``->datasend(``$email``->as_string());`
`        ``$smtp``->datasend(``"\r\n\r\n"``);`
`        ``$smtp``->dataend();`
`        ``$smtp``->quit();`
```
bugzilla的代码还是写的很人性化的，没有写过perl代码的，一看都能知道个大概。向Bugzilla的作者致敬。
至于邮件发送的流程，大家可以参照rfc822，不要随便相信网上的代码。
