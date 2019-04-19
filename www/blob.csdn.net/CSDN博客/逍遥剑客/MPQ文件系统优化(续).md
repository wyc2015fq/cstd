# MPQ文件系统优化(续) - 逍遥剑客 - CSDN博客
2009年01月07日 23:42:00[xoyojank](https://me.csdn.net/xoyojank)阅读数：2214
天啊, 自己解析(listfile)本身就是一个错误!
因为StormLib在打开MPQ文件时就已经自己解析过了, 没必要多此一举!
而且, 虽然自己缓存一下文件树会加快查找和枚举的速度, 但是, 从字符串构造这个棵树的时间太长了~~~
这就导致了一个很严重的问题: 启动速度慢.......
对于文件树, 游戏没有什么用, 因为游戏读文件时根本不管是什么目录, 你可以把目录也看是文件名的一部分
换句话说, MPQ包里根本就没有目录的概念!
而对于编辑器等需要列举文件/文件夹列表的场合, 直接对所有文件进行枚举再判断字符串即可:
//------------------------------------------------------------------------------ 
Array<String>
MpqArchive::ListFiles(const String& dirPathInMpqArchive, const String& pattern)
{
    Array<String> result;
    String fileMask(dirPathInMpqArchive);
    fileMask.SubstituteChar('/', '//');
    if (fileMask[fileMask.Length() - 1] != '//')
    {
        fileMask.Append("//");
    }
    fileMask.Append(pattern);
    // start to find 
    SFILE_FIND_DATA findData;
    BOOL found = TRUE;
    HANDLE fileHandle = SFileFindFirstFile(this->mpqFileHandle, fileMask.AsCharPtr(), &findData, NULL); 
    while (0 != fileHandle && TRUE == found)
    {
        result.Append(String(findData.cFileName).ExtractFileName());
        found = SFileFindNextFile(fileHandle, &findData);
    }
    return result;
}
下面是我的"单元"测试程序(嘿嘿, 包装一个好听的名字), 运行一下不会超过2秒, DEBUG模式下
//------------------------------------------------------------------------------ 
//  mpqfilesystemtest.cc 
//  (C) 2008 xoyojank 
//------------------------------------------------------------------------------ 
#include "stdneb.h" 
#include "mpqfilesystemtest.h" 
#include "interface/iointerface.h" 
namespace Test
{
__ImplementClass(Test::MpqFileSystemTest, 'MQFT', Test::TestCase);
using namespace IO;
using namespace Interface;
using namespace Util;
//------------------------------------------------------------------------------ 
/**
*/
void
MpqFileSystemTest::Run()
{
    Ptr<IoServer> ioServer = IoServer::Create();
    ioServer->SetZipFileSystemEnabled(false);
    ioServer->RegisterStandardUriSchemes();
    ioServer->MountWoWMpqArchives();
    //n_assert(ioServer->IsFileExsit("wow:Creature/Cat/Cat.m2")); 
    Array<String> catFiles = ioServer->ListFiles("wow:Creature/Cat", "*.*");
    for (IndexT i = 0; i < catFiles.Size(); ++i)
    {
        n_printf("%s/n", catFiles[i].AsCharPtr());
    }
    Array<String> charDirs = ioServer->ListDirectories("wow:Character", "*");
    for (IndexT i = 0; i < charDirs.Size(); ++i)
    {
        n_printf("%s/n", charDirs[i].AsCharPtr());
    }
    URI uri = "wow:Character/BloodElf/HAIR00_00.blp";
    Ptr<Stream> texture = ioServer->CreateStream(uri);
    texture->Open();
    Array<char> buffer(texture->GetSize(), 0, 0);
    texture->Read(&buffer.Front(), texture->GetSize());
    texture->Close();
}
} // namespace Test 
