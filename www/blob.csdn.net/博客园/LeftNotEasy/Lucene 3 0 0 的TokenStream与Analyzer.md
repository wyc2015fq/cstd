# Lucene 3.0.0 的TokenStream与Analyzer - LeftNotEasy - 博客园







# [Lucene 3.0.0 的TokenStream与Analyzer](https://www.cnblogs.com/LeftNotEasy/archive/2010/01/17/1650139.html)





     如果你看的Lucene相关的书是很老版本的, 比如说2.4或者更早, 那么对于这个版本中的Analyzer可能就不那么容易接受了, 我也是看的<lucene分析与应用>这本书, 比较古老的版本.

     今天读了一下源代码, 大概说说心得, 我从SimpleAnalyzer说起.

     SimpleAnalyzer的作用就是把一段字符串中除了符号和非文字的内容作为分隔, 把句子分成很多的单词. 对于中文也可以用来剔除标点符号


public TokenStream tokenStream(String fieldName, Reader reader) {returnnew LowerCaseTokenizer(reader);}


     SimpleAnalyzer的TokenStream就是调用了LowerCaseTokenizer的构造函数. LowerCaseTokenizer继承自CharTokenizer. 首先来看看CharTokenizer的构造函数吧:


public CharTokenizer(Reader input) {super(input);  offsetAtt = addAttribute(OffsetAttribute.class);  termAtt = addAttribute(TermAttribute.class);}


     除了调用基类的构造函数外(继承自Tokenizer), 另外还有下面两个与Attribute相关的语句, Attribute是Lucene新增的内容(具体是不是3.0新增的就不清楚了), 作用是可以在TokenStream中加入一些用户需要的内容, 比如说单词的词性, 文字等等内容, 这些东西是可以用户定义的, 提供了更多的耦合性, 怎么访问这些内容我上一篇日志也是有说的.

     下面我把CharTokenizer的incrementToken()函数的代码贴出来分析一下


publicfinalboolean incrementToken() throws IOException {  clearAttributes();int length = 0;int start = bufferIndex;char[] buffer = termAtt.termBuffer();while (true) {if (bufferIndex >= dataLen) {      offset += dataLen;      dataLen = input.read(ioBuffer);if (dataLen == -1) {        dataLen = 0;                            // so next offset += dataLen won't decrement offsetif (length > 0)break;elsereturn false;      }      bufferIndex = 0;    }finalchar c = ioBuffer[bufferIndex++];if (isTokenChar(c)) {               // if it's a token charif (length == 0)                 // start of token        start = offset + bufferIndex - 1;elseif (length == buffer.length)        buffer = termAtt.resizeTermBuffer(1+length);      buffer[length++] = normalize(c); // buffer it, normalizedif (length == MAX_WORD_LEN)      // buffer overflow!break;    } elseif (length > 0)             // at non-Letter w/ charsbreak;                           // return 'em  }  termAtt.setTermLength(length);  offsetAtt.setOffset(correctOffset(start), correctOffset(start+length));return true;}








     在一个Tokenizer里面将保留这多个Attribute的实例, 这些实例是循环利用的, 每新得到一个单词, 就改写一下他们的内容, 

> 

1)


if (bufferIndex >= dataLen) {  offset += dataLen;  dataLen = input.read(ioBuffer);if (dataLen == -1) {    dataLen = 0;                            // so next offset += dataLen won't decrement offsetif (length > 0)break;elsereturn false;  }  bufferIndex = 0;}


这段代码是初始化的内容, 得到整个字符串的长度, ioBuffer就是这个字符串

2)


finalchar c = ioBuffer[bufferIndex++];if (isTokenChar(c)) {               // if it's a token charif (length == 0)                 // start of token    start = offset + bufferIndex - 1;elseif (length == buffer.length)    buffer = termAtt.resizeTermBuffer(1+length);  buffer[length++] = normalize(c); // buffer it, normalizedif (length == MAX_WORD_LEN)      // buffer overflow!break;} elseif (length > 0)             // at non-Letter w/ charsbreak;                           // return 'em




这段代码是不停的读取下一个字符, 看看是符号还是合法的字符.

3)


termAtt.setTermLength(length);offsetAtt.setOffset(correctOffset(start), correctOffset(start+length));return true;


最后把单词的termAtt和offsetAtt赋值, 返回












