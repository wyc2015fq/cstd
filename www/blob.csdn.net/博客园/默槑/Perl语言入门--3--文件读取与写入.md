# Perl语言入门--3--文件读取与写入 - 默槑 - 博客园







# [Perl语言入门--3--文件读取与写入](https://www.cnblogs.com/modaidai/p/6690839.html)





现有文件test.txt,内容为："123\n456"


1,打开文本test.txt

      #!/usr/bin/perl

      open d,"test.txt";

    d 为文件句柄，指向打开的文件



2，逐行读取文本test.txt

      #!/usr/bin/perl

      open d,"< test.txt";

      while(<d>){

          print $_;

       }

       close d;

      结果：123

               456

     或者：print <d>;

     结果：同上

     <> 为取行操作符



3，向test111.txt中写入内容

   #!/usr/bin/perl

    open d,">test111.txt";

    print f "hello,world\nsee you\n";

    close d;

    如果test111.txt原本有内容，则原内容将会被擦除，test111.txt的内容为

                    hello,world 

                    see you

4,向test111.txt中追加内容

    #!/usr/bin/perl

      open d,">>test111.txt";

      print d "thank you\n";

      close d;

      test111.txt的内容为 

              hello,world

              see you

              thank you



   print 相当于写命令，别丢了语句结尾的 “;"













