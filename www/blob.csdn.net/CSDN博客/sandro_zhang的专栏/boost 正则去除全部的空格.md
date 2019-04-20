# boost 正则去除全部的空格 - sandro_zhang的专栏 - CSDN博客
2011年09月13日 13:10:57[sandro_zhang](https://me.csdn.net/sandro_zhang)阅读数：1313
string xpsContent = "here is serveral space";
boost::regex exptrim( "\\s" );
string xpsContent_nospace = boost::regex_replace(xpsContent, exptrim, "") ;
            
