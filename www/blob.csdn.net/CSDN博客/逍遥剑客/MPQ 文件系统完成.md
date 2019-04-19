# MPQ 文件系统完成 - 逍遥剑客 - CSDN博客
2008年12月28日 02:55:00[xoyojank](https://me.csdn.net/xoyojank)阅读数：3492
基于StormLib, 参考N3的ZipFileSystem实现了一个MpqFileSystem
有一点要注意, 文件路径里不能用'/', 都要用'//' @_@
mpq文件包里的文件是不保存文件名(或目录名)的, 也就是说打包后的文件是不可逆的. 但是在每个文件包的根目录下都有一个(listfile)文件, 里面包含了文件包中的文件名列表
(为什么listfile文件最后有一段空字符串呢???)
没有去实现写入的功能, 虽然StormLib可以添加文件, 不过对于游戏只需要读就可以了
mpq有一点比zip有优势, 那就是可以进行Seek...这样就支持流式的文件加载了.
而且对于wav可以采用特殊的压缩算法来增大压缩率. 不过我对wav没好印象的说...
用XACT可以播放xWMA, 用DirectShow可以播放WMV(我晕, 我怎么又跟着M$混了. 没办法啊, 谁叫fmod之类的商用收费来着)
嗯...写完记得要测试.....(BOSS说这才是专业的一种表现-_-)
//------------------------------------------------------------------------------ 
/**
*/
void
MpqFileSystemTest::Run()
{
    Ptr<IoServer> ioServer = IoServer::Create();
    Ptr<MpqFileSystem> mpqFileSystem = MpqFileSystem::Create();
    Ptr<MpqArchive> patch2 = mpqFileSystem->Mount("file:///e:/World of Warcraft/Data/patch-2.MPQ");
    n_assert(patch2.isvalid());
    const Array<String>& fileList = patch2->GetFileList();
    n_assert(!fileList.IsEmpty());
    for (IndexT i = 0; i < fileList.Size(); ++i)
    {
        n_printf("%s/n", fileList[i].AsCharPtr());
    }
    n_assert(patch2->IsFileExsit("Creature/Ogre/Ogre.M2"));
    Array<String> spellFiles = patch2->ListFiles("Spells", "*.*");
    for (IndexT i = 0; i < spellFiles.Size(); ++i)
    {
        n_printf("%s/n", spellFiles[i].AsCharPtr());
    }
    Array<String> creatureDirs = patch2->ListDirectories("Creature", "*");
    for (IndexT i = 0; i < creatureDirs.Size(); ++i)
    {
        n_printf("%s/n", creatureDirs[i].AsCharPtr());
    }
    URI uri = "file:///e:/World of Warcraft/Data/Shaders/Vertex/snowpoint.bls";
    Ptr<MpqArchive> archive = mpqFileSystem->FindMpqArchiveWithFile(uri);
    n_assert(archive == patch2);
    Ptr<MpqFileStream> snowpoint = MpqFileStream::Create();
    uri = "mpq:///e:/World of Warcraft/Data/patch-2.MPQ?file=Shaders/Vertex/snowpoint.bls";
    snowpoint->SetURI(uri);
    snowpoint->Open();
    Array<char> buffer(snowpoint->GetSize(), 0, 0);
    snowpoint->Read(&buffer.Front(), snowpoint->GetSize());
    snowpoint->Close();
}
![](https://p-blog.csdn.net/images/p_blog_csdn_net/xoyojank/EntryImages/20081228/N3_MPQ_TEST_0.JPG)
![](https://p-blog.csdn.net/images/p_blog_csdn_net/xoyojank/EntryImages/20081228/N3_MPQ_TEST_1.JPG)
嗯, 为了比较方便, 把文件名都转换成小写了....不过linux下不是大小写区分的么, 那linux程序员写的打包程序会不会也区别文件名的大小写, HOHO~
