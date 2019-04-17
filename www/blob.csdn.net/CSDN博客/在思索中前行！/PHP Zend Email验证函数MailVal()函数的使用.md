# PHP  Zend Email验证函数MailVal()函数的使用 - 在思索中前行！ - CSDN博客





2014年05月15日 21:37:31[_Tham](https://me.csdn.net/txl16211)阅读数：2514








# PHP Email验证

- 
<?php

- 
/************************************************************************

- 
*此功能检查格式的电子邮件地址。有5个级别的

- 


- 
   *检查：

- 



- 
   * 1 -基本格式检查。确保：

- 


- 
   *有一个@符号的东西在左边和右边的东西

- 


- 
   *为了正确的@符号，但至少有一个点，与一些左翼和右翼。

- 


- 
   *要的权利，最后点或者2个或3个字母，或特殊情况“阿尔帕”

- 


- 
   * 2 -以上，再加上信的权利，最后点是：

- 


- 
   *通信，网络，组织，教育，大学，人事局，诠释，阿尔帕或一两个字母的国家代码

- 


- 
   * 3 -以上，再加上努力，以检查是否存在的MX （邮件交换）记录

- 


- 
   *域名。

- 


- 
   * 4 -上述情况，加上尝试连接到邮件服务器

- 


- 
   * 5 -以上，再加上检查，看看是否有回应的邮件服务器。第三

- 


- 
   *论点这一功能是可选的，并提出了多少次循环，而

- 


- 
   *等待响应的邮件服务器。默认值为15000 。实际的等待

- 


- 
   *时间，当然，依赖于这种事情的速度，您的服务器。

- 


- 
   *第1级是防弹：如果该地址没有这个水平，这是坏的。 2级仍然是

- 


- 
   *非常稳固，但不太确定：有可能是有效的顶级忽视写作时

- 


- 
   *此功能，或新的可以增加。 3级甚至更小一些：有

- 


- 
   *一些东西，可以防止找到一个MX记录的一个有效的地址

- 


- 
   *在任何特定时间。第4和第5更是某些仍在。归根结底，只有绝对

- 


- 
   *积极的方式来测试的电子邮件地址是发送给它。

- 



- 
   *该函数返回0为一个有效的地址，或在哪一级失败，一个

- 


- 
   *地址无效。

- 


- 
   *

- 
* This function checks the format of an email address. There are five levels of

- 
   * checking:

- 
   *

- 
   * 1 – Basic format checking. Ensures that:

- 
   *      There is an @ sign with something on the left and something on the right

- 
   *      To the right of the @ sign, there's at least one dot, with something to the left and right.

- 
   *      To the right of the last dot is either 2 or 3 letters, or the special case "arpa"

- 
   * 2 – The above, plus the letters to the right of the last dot are:

- 
   *      com, net, org, edu, mil, gov, int, arpa or one of the two-letter country codes

- 
   * 3 – The above, plus attempts to check if there is an MX (Mail eXchange) record for the

- 
   *      domain name.

- 
   * 4 – The above, plus attempt to connect to the mail server

- 
   * 5 – The above, plus check to see if there is a response from the mail server. The third

- 
   *      argument to this function is optional, and sets the number of times to loop while

- 
   *      waiting for a response from the mail server. The default is 15000. The actual waiting

- 
   *      time, of course, depends on such things as the speed of your server.

- 
   *

- 
   * Level 1 is bulletproof: if the address fails this level, it's bad. Level 2 is still

- 
   * pretty solid, but less certain: there could be valid TLDs overlooked when writing

- 
   * this function, or new ones could be added. Level 3 is even less certain: there are

- 
   * a number of things that could prevent finding an MX record for a valid address

- 
   * at any given time. 4 and 5 are even less certain still. Ultimately, the only absolutely

- 
   * positive way to test an email address is to send something to it.

- 
   *

- 
   * The function returns 0 for a valid address, or the level at which it failed, for an

- 
   * invalid address.

- 
   *

- 
   ************************************************************************/

- 


- 

```php
function MailVal($Addr, $Level, $Timeout = 15000) {
//   Valid Top-Level Domains
    $gTLDs = "com:net:org:edu:gov:mil:int:arpa:";
    $CCs   = "ad:ae:af:ag:ai:al:am:an:ao:aq:ar:as:at:au:aw:az:ba:bb:bd:be:bf:".
             "bg:bh:bi:bj:bm:bn:bo:br:bs:bt:bv:bw:by:bz:ca:cc:cf:cd:cg:ch:ci:".
             "ck:cl:cm:cn:co:cr:cs:cu:cv:cx:cy:cz:de:dj:dk:dm:do:dz:ec:ee:eg:".
             "eh:er:es:et:fi:fj:fk:fm:fo:fr:fx:ga:gb:gd:ge:gf:gh:gi:gl:gm:gn:".
             "gp:gq:gr:gs:gt:gu:gw:gy:hk:hm:hn:hr:ht:hu:id:ie:il:in:io:iq:ir:".
             "is:it:jm:jo:jp:ke:kg:kh:ki:km:kn:kp:kr:kw:ky:kz:la:lb:lc:li:lk:".
             "lr:ls:lt:lu:lv:ly:ma:mc:md:mg:mh:mk:ml:mm:mn:mo:mp:mq:mr:ms:mt:".
             "mu:mv:mw:mx:my:mz:na:nc:ne:nf:ng:ni:nl:no:np:nr:nt:nu:nz:om:pa:".
             "pe:pf:pg:ph:pk:pl:pm:pn:pr:pt:pw:py:qa:re:ro:ru:rw:sa:sb:sc:sd:".
             "se:sg:sh:si:sj:sk:sl:sm:sn:so:sr:st:su:sv:sy:sz:tc:td:tf:tg:th:".
             "tj:tk:tm:tn:to:tp:tr:tt:tv:tw:tz:ua:ug:uk:um:us:uy:uz:va:vc:ve:".
             "vg:vi:vn:vu:wf:ws:ye:yt:yu:za:zm:zr:zw:";
 
//   The countries can have their own 'TLDs', e.g. mydomain.com.au
    $cTLDs = "com:net:org:edu:gov:mil:co:ne:or:ed:go:mi:";
 
    $fail = 0;
 
//   Shift the address to lowercase to simplify checking
    $Addr = strtolower($Addr);
 
//   Split the Address into user and domain parts
    $UD = explode("@", $Addr);
     if (sizeof($UD) != 2 || !$UD[0]) $fail = 1;
 
//   Split the domain part into its Levels
    $Levels = explode(".", $UD[1]); $sLevels = sizeof($Levels);
     if ($sLevels < 2) $fail = 1;
 
//   Get the TLD, strip off trailing ] } ) > and check the length
    $tld = $Levels[$sLevels-1];
    $tld = ereg_replace("[>)}]$|]$", "", $tld);
     if (strlen($tld) < 2 || strlen($tld) > 3 && $tld != "arpa") $fail = 1;
 
    $Level–;
 
//   If the string after the last dot isn't in the generic TLDs or country codes, it's invalid.
    if ($Level && !$fail) {
    $Level–;
     if (!ereg($tld.":", $gTLDs) && !ereg($tld.":", $CCs)) $fail = 2;
     }
 
//   If it's a country code, check for a country TLD; add on the domain name.
    if ($Level && !$fail) {
    $cd = $sLevels - 2; $domain = $Levels[$cd].".".$tld;
     if (ereg($Levels[$cd].":", $cTLDs)) { $cd–; $domain = $Levels[$cd].".".$domain; }
     }
 
//   See if there's an MX record for the domain
    if ($Level && !$fail) {
    $Level–;
     if (!getmxrr($domain, $mxhosts, $weight)) $fail = 3;
     }
 
//   Attempt to connect to port 25 on an MX host
    if ($Level && !$fail) {
    $Level–;
     while (!$sh && list($nul, $mxhost) = each($mxhosts))
      $sh = fsockopen($mxhost, 25);
     if (!$sh) $fail = 4;
     }
 
//   See if anyone answers
    if ($Level && !$fail) {
    $Level–;
    set_socket_blocking($sh, false);
    $out = ""; $t = 0;
     while ($t++ < $Timeout && !$out)
      $out = fgets($sh, 256);
     if (!ereg("^220", $out)) $fail = 5;
     }
 
     if ($sh) fclose($sh);
 
     return $fail;
   } //MailVal
?></span>
```



注释：[MX记录 （Mail Exchange）邮件路由记录](http://baike.baidu.com/view/30676.htm)

用户可以将该域名下的邮件服务器指向到自己的mail server上，然后即可自行操作控制所有的邮箱设置。您只需在线填写您服务器的主机名或主机IP地址，即可将您域名下的邮件全部转到您自己设定相应的邮件服务器上。
　　这个大家都明白了吗？就是将你的域名中邮件服务器分开，将它设置到其它的IP去！
　　比如同样是 cnMonkey_com ，如果你设置A记录是指向123.12.123.123，而MX记录你设置是指向222.22.222.222，那么你的DNS服务器接收到别人的邮件路 由请求时就将会将它的请求解释到222.22.222.222上去！而别人访问你的网页的时候仍然是访问123.12.123.123。　
　　可以使用nslookup -qt=mx test_com来解析mx的指向。例子中以test_com为例。



