# 如何读取MP3的信息 - xqhrs232的专栏 - CSDN博客
2013年05月01日 16:49:01[xqhrs232](https://me.csdn.net/xqhrs232)阅读数：499
个人分类：[C/C++/VC++/VS](https://blog.csdn.net/xqhrs232/article/category/906934)
原文地址::[http://ucg1987.blog.163.com/blog/static/170921407201292743222506/](http://ucg1987.blog.163.com/blog/static/170921407201292743222506/)
相关网帖
1、[C读取MP3头部和尾部信息的代码](http://www.cnblogs.com/aliasmic/archive/2008/09/20/1294915.html)----[http://www.cnblogs.com/aliasmic/archive/2008/09/20/1294915.html](http://www.cnblogs.com/aliasmic/archive/2008/09/20/1294915.html)
每一个MP3文件包含有一个ID3-Tag头,   它用来提供艺术家、标题、专辑、出版年和歌曲流派等信息。这个头总是128字节长并位于MP3文件末尾。 
ID3-Tag结构是这样的:
type   
TID3Tag   =   packed   record   //   128   字节 
      TAGID:   array[0..2]   of   char;//   3   字节:   必须是TAG 
Title:   array[0..29]   of   char;//   30   字节:   歌曲标题 
Artist:   array[0..29]   of   char;//   30   字节:   歌曲的艺术家 
Album:   array[0..29]   of   char;//   30   字节:   歌曲专辑 
Year:   array[0..3]   of   char;//   4   字节:   出版年 
Comment:   array[0..29]   of   char;//   30   字节:   评论 
Genre:byte;//   1   字节:   种类标识 
end;
//为读取ID3-Tag信息并在一个对话框中显示，试试这个函数:   
procedure   TForm1.Button1Click(Sender:TObject);
const
    _mp3file='G:\Mp3\Miscellaneous\ATC   -   Around   The   World.mp3 ';
var
    id3tag:Tid3tag;
    mp3file:Tfilestream;
begin
    mp3file:=Tfilestream.create(_mp3file,fmOpenRead);
try
        mp3file.position:=mp3file.size-128;//   跳到id3-tag 
        mp3file.Read(id3tag,SizeOf(id3tag));
        showmessage('   Title:   '+id3tag.title+#13+ 
'   Artist:   '+id3tag.artist+#13+ 
'   Album:   '+id3tag.album+#13+ 
'   Year:   '+id3tag.year+#13+ 
'   Comment:   '+id3tag.comment+#13+ 
'   Genre-ID:   '+inttostr(id3tag.genre)
);
finally
        mp3file.free;
end;
end;
