# Lucene 3.0.0 之样例解析(4)-IndexFiles.java - LeftNotEasy - 博客园







# [Lucene 3.0.0 之样例解析(4)-IndexFiles.java](https://www.cnblogs.com/LeftNotEasy/archive/2010/01/09/1643179.html)





**一. TermHashPerField.add()方法**

    这一章继续上面的内容, 上一章谈到TermHashPerField.add()方法就是把一个Term加入到posting表的过程, 那么下面我将从算法的角度来分析这个add()方法:


finalchar[] tokenText = termAtt.termBuffer();;finalint tokenTextLen = termAtt.termLength();


    这两行主要做的事情就是获取当前需要分析的单词的内容以及长度.


int downto = tokenTextLen;int code = 0;while (downto > 0) {char ch = tokenText[--downto];if (ch >= UnicodeUtil.UNI_SUR_LOW_START && ch <= UnicodeUtil.UNI_SUR_LOW_END) {if (0 == downto) {// Unpaired      ch = tokenText[downto] = UnicodeUtil.UNI_REPLACEMENT_CHAR;    } else {finalchar ch2 = tokenText[downto-1];if (ch2 >= UnicodeUtil.UNI_SUR_HIGH_START && ch2 <= UnicodeUtil.UNI_SUR_HIGH_END) {// OK: high followed by low.  This is a valid// surrogate pair.        code = ((code*31) + ch)*31+ch2;        downto--;continue;      } else {// Unpaired        ch = tokenText[downto] = UnicodeUtil.UNI_REPLACEMENT_CHAR;      }                }  } elseif (ch >= UnicodeUtil.UNI_SUR_HIGH_START && (ch <= UnicodeUtil.UNI_SUR_HIGH_END ||                                                      ch == 0xffff)) {// Unpaired or 0xffff    ch = tokenText[downto] = UnicodeUtil.UNI_REPLACEMENT_CHAR;  }  code = (code*31) + ch;}




    这一段的内容就是计算当前单词的Hash值, 从代码中也可以看到, 程序排除了一些无效的utf-16串, 另外hash值计算的函数也还是比较容易看懂, 也就是从单词后往前, 采用类似于31进制的方法计算出一个数.


int hashPos = code & postingsHashMask;




    这个好懂, 就是计算出hash函数所对应的hash表中的位置, 另外, postingsHashMask就是表示当前Hash表的长度.


p = postingsHash[hashPos];if (p != null && !postingEquals(tokenText, tokenTextLen)) {// Conflict: keep searching different locations in// the hash table.finalint inc = ((code>>8)+code)|1;do {    code += inc;    hashPos = code & postingsHashMask;    p = postingsHash[hashPos];  } while (p != null && !postingEquals(tokenText, tokenTextLen));}




    首先获取计算出位置的内容, 看看该p是否为空, 如果不为空, 则表示出现冲突了, 在这段程序中可以理解到, 如果lucene发现冲突, 则计算得到一个inc值, 公式是: inc = ((code >> 8) + code) | 1;

    这个也可以理解为二次hash法吧. 后面的语句有很多是与内存池有关的, 我也没有很清晰的理解, 这里就不多说了, 值得注意的是, Lucene里面大量的内存都是统一分配的, 从Postings 到 Postings里面的内容, 到里面的每一个字符串(Term), 都是采用统一分配的方法, 这样可以有效减小内存的碎片.

**二. 回到DocumentWriter.updateDocument(Document, Analyzer, Term)**


try {// This call is not synchronized and does all the// workfinal DocWriter perDoc = state.consumer.processDocument();// This call is synchronized but fast  finishDocument(state, perDoc);  success = true;}




    执行完了第一句, 多个线程将会在内存中形成Posting表, 下面就该执行:


finishDocument(state, perDoc);




    完成判断是否flush倒排文档的操作.

**下面一节我将谈谈位于IndexWriter.addDocument(Document, Analyzer)中的flush(true, true, true)的大致过程.**












