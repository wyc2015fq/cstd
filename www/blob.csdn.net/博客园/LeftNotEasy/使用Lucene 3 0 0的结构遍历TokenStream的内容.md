# 使用Lucene 3.0.0的结构遍历TokenStream的内容. - LeftNotEasy - 博客园







# [使用Lucene 3.0.0的结构遍历TokenStream的内容.](https://www.cnblogs.com/LeftNotEasy/archive/2010/01/14/1647778.html)





    以前版本的Lucene是用TokenStream.next()来遍历TokenStream的内容, 目前的版本稍微修改了一下, 使用下面的的一段程序可以遍历TokenStream的内容


privatestaticvoid displayTokenStream(TokenStream ts) throws IOException{  TermAttribute termAtt = (TermAttribute)ts.getAttribute(TermAttribute.class);  TypeAttribute typeAtt = (TypeAttribute)ts.getAttribute(TypeAttribute.class);while (ts.incrementToken())  {      System.out.print(termAtt.term());      System.out.print(' ');      System.out.println(typeAtt.type());  }  System.out.println(' ');}













