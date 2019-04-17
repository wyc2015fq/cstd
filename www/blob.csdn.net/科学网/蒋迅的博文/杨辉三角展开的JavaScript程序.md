# 科学网—杨辉三角展开的JavaScript程序 - 蒋迅的博文




# 杨辉三角展开的JavaScript程序                           

已有 8219 次阅读2012-1-25 13:11|个人分类:[谈数学](http://blog.sciencenet.cn/home.php?mod=space&uid=420554&do=blog&classid=130689&view=me)|系统分类:[教学心得](http://blog.sciencenet.cn/home.php?mod=space&do=blog&view=all&uid=420554&catid=9)|关键词:杨辉三角,,程序,,JavaScript|[程序](misc.php?mod=tag&id=15652), [杨辉三角](misc.php?mod=tag&id=137391)



作者：蒋迅

![](http://image.sciencenet.cn/album/201201/05/2226557nfnzc6yfeyew7sb.jpg)

上面是一位美国学生的作业。这个学生按照字面的意思“展开”(expand) 代数式(a + b)n，真的有才。其实老师的意思是要学生得出象 (*a* + *b*)2 = *a*2 + 2*ab* + *b*2 这样的等式。但是题目过於抽象，学生不知所措。其实应该先从具体的自然数 *n* = 2, 3, ... 开始。

![](http://image.sciencenet.cn/album/201201/06/092916qn9btj55tq3b79s9.gif)![](file:///C:/Users/micino/Documents/public_html/Album00/Math/images/p_PascalsTriangle.gif)
Source: [Dauger Research](http://daugerresearch.com/vault/parallelpascalstriangle.shtml)
这个数学公式在中国被称作“[杨辉三角](http://zh.wikipedia.org/wiki/%E6%9D%A8%E8%BE%89%E4%B8%89%E8%A7%92)”。之所以称为杨辉三角是因为宋代数学家[杨辉](http://zh.wikipedia.org/wiki/%E6%9D%A8%E8%BE%89)在《详解九章算术》里讨论这种形式的数表，并说明此表引自贾宪的《释锁算术》。英文的名字是“[Pascal Triangle](http://en.wikipedia.org/wiki/Pascal_triangle)”，取自法国数学家[布莱士·帕斯卡](http://zh.wikipedia.org/wiki/%E5%B8%83%E8%8E%B1%E5%A3%AB%C2%B7%E5%B8%95%E6%96%AF%E5%8D%A1)([Blaise Pascal](http://en.wikipedia.org/wiki/Blaise_Pascal)) 的名子。实际上，这个三角形的发现比杨辉和帕斯卡都要早。我以前写过一个Javascript程序帮助学生做杨辉三角的展开。现在贡献出来，给老师们参考。不过，这里不让运行自己的Javascript程序，所以我只能给出原程序，请读者自己下载试用。


<SCRIPT type="text/javascript">

<!--

var my_n;

var noIFrame = false;


function reLoadFrame(StrPoly,StrTable){

  with(winTrig.document) {

    close();

    open();

    write('<html>n');

    write('<head>n');

    write('<title>Expansion of Perfect nth Power and Pascal's Triangle</title>n');

    write('</head>n');

    write('<body>n');

    write(StrPoly + 'n');

    write('<p>n');

    write(StrTable + 'n');

    write('</body>n');

    write('</html>n');

    close();

  }

}


function InitFrame() {

  with(winTrig.document) {

    close();

    open();

    writeln('<html>');

    writeln('<head>');

    writeln('<title>Expansion of Perfect nth Power and Pascal's Triangle</title>');

    writeln('</head>');

    writeln('<body>');

    writeln("Please input a positive integer.");

    writeln('</body>');

    writeln('</html>');

    close();

  }

}


function doOnLoad() {

  if(noIFrame) {

    if(top.location == self.location || self.location != parent.frames[0].location) {

      with(top.document) {

        close();

        open();

        writeln('<html>');

        writeln('<head>');

        writeln('<title>Expansion of Perfect nth Power and Pascal's Triangle</title>');

        writeln('</head>');

        writeln('<frameset rows="350,*">');

        writeln('<frame name="pmain" src="'+ self.location + '" scrolling="no">');

        writeln('<frame name="graph" src="about:blank">');

        writeln('</frameset>');

        writeln('</html>');

        close();

      }

      return;

    }

    winTrig = parent.frames[1];

  }

  else

    winTrig = window["EmbdFrame"];

  InitFrame();

  document.forms[0].reset();

  document.forms[0].btnStart.disabled = true;

  document.forms[0].expd.disabled = false;

}


function expandit() {

  my_n = document.forms[0].user_power.value;

  if (isNumber(my_n)) {

    // coefficients of polynomial expansion.

    c = new Array(my_n+1);

    c[0] = new Array(1);

    c[0][0] = 1;

    for (i=1; i<= my_n; i++) {

      c[i] = new Array(i+1);

      c[i][0] = 1;

      c[i][i] = 1;

      for (j=1; j<i; j++) {

        c[i][j] = c[i-1][j-1] + c[i-1][j];

      }

    }


    // construct the polynomial expansion.

    var txtPoly = "(x + y)" + my_n + " = n";

    if (my_n == 1) { txtPoly += "x + y."}

    else {

      if (my_n == 2) { txtPoly += 
"x2 + 2xy +
 y2."}

      else {

        txtPoly += 'x' + my_n + 'n';

        for(i=1; i<my_n; i++){

          j = my_n - i;

          coef = c[my_n][i];

          // x^j*y^i

          txtPoly += ' + ' + coef + 'x' + j
 + 'y' + i + 'n';

          if (i%5 == 0) txtPoly += '
             ';

        }

        txtPoly += ' + y' + my_n + '.n';

        // filter out power 1.

        txtPoly = txtPoly.replace(/<sup>1</sup>/g,'');

      }

    }


    var txtTable = "The corresponding Pascal's Triangle:n<p>";

    txtTable += "<table border=0>";

    for (i = 0; i <= my_n; i++) {

      txtTable += "<tr>";

      for (j = 0; j < my_n - i; j++) {

        txtTable += "<td>";

        txtTable += "   "

        txtTable += "</td>n";

      }

      for (j = my_n - i; j < 2*my_n + 1; j++) {

        k = j - my_n + i;

        txtTable += "<td align='center'>";

        if (k%2 == 0 && k <= 2*i) {

          txtTable += c[i][k/2];

        }

        else {

          txtTable += "   "

        }

        txtTable += "</td>n";

      }

      txtTable += "</tr>n";

    }

    txtTable += "</table>";

    reLoadFrame(txtPoly,txtTable);

  }

}


// data validation. Could be replaced by regular expression.

function isNumber(inputVal) {

  inputStr = "" + inputVal;

  for (var i = 0; i < inputStr.length; i++) {

    var oneChar = inputStr.charAt(i);

    if (oneChar < "0" || oneChar > "9") {

      return false;

    }

    return true;

  }

}


function myReload() {

  document.forms[0].user_power.value = 3;

  doOnLoad();

  javascript:location.reload();

}

// -->

</SCRIPT>

<FORM>

<h2 align="center">Expansion of Perfect nth Power and Pascal's Triangle</h2>


This script will generate the expansion of the perfect 
nth power (x + 
y)n.


<p>

<form name="formular">

请选择一个正整数: <input name="user_power" size=4 maxlength=2 value=3>




<input type=button name="btnStart" value="开始" onClick="doOnLoad()">


<input type=button name="expd" value="展开" onClick="expandit()" DISABLED="true">


<!-- <input type=reset value="重来" onClick="myReload()"> -->

</form>


<P> </P>


<IFRAME ID="IFrameExpd" Name="EmbdFrame" FRAMEBORDER=0 SRC="" WIDTH="100%" height="500">

<SCRIPT type="text/javascript">

<!--

  noIFrame = true;

// -->

</SCRIPT>

</IFRAME>


笔者在这个程序里有一件事情没有做：假如有人输入一个任意的“n”，我们应该得到一个一般性的展开式。做到这点不难，有兴趣的读者不妨自己补上。

下面再给几个相关图形：

![](http://image.sciencenet.cn/album/201201/06/0941226yiyh7gv17g8zyv7.jpg)
Source: [](http://milan.milanovic.org/math/english/fibo/fibo4.html)Radoslav Jovanovic

![](http://image.sciencenet.cn/album/201201/06/094129qoli0irrki6yiol7.png)
Source: [](http://www.texample.net/tikz/examples/pascals-triangle-and-sierpinski-triangle/)TeXample.net

![](http://image.sciencenet.cn/album/201201/06/094126dk87fgds6q8az8r6.gif)
Source: [](http://jwilson.coe.uga.edu/emat6680/parsons/mvp6690/essay1/sierpinski.html)Marianne Parsons

![](http://image.sciencenet.cn/album/201201/06/094126ytti82s0z5rsleww.jpg)
Source: [](http://mu6.com/catalan_numbers_growth.html)Dirk Laureyssens

转载本文请联系原作者获取授权，同时请注明本文来自蒋迅科学网博客。
链接地址：[](http://blog.sciencenet.cn/blog-420554-531567.html)http://blog.sciencenet.cn/blog-420554-531567.html 

上一篇：[](blog-420554-530391.html)反对过度审查互联网！
下一篇：[](blog-420554-531778.html)蒋式瑆卷照




