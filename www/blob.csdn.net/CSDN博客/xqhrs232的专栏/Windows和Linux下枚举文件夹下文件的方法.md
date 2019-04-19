# Windows和Linux下枚举文件夹下文件的方法 - xqhrs232的专栏 - CSDN博客
2016年05月17日 14:54:17[xqhrs232](https://me.csdn.net/xqhrs232)阅读数：1051
原文地址::[http://blog.csdn.net/wutaozhao/article/details/6044067](http://blog.csdn.net/wutaozhao/article/details/6044067)
相关文章
1、[浅析linux中目录枚举的具体实现](http://blog.chinaunix.net/uid-20564848-id-73754.html) ----[http://blog.chinaunix.net/uid-20564848-id-73754.html](http://blog.chinaunix.net/uid-20564848-id-73754.html)
2、Linux下DIR,dirent,stat等结构体详解----[http://wenku.baidu.com/link?url=_GWxd5QG7rYw3Muste5xTKYLZkXU1wWU6kd2xTU_nMdtHzTGk2vF56eNg71g9XOlWdvh4Izgq97hpVMy9tUmgM3vCgYyljbP0GuR4_a9Qmq](http://wenku.baidu.com/link?url=_GWxd5QG7rYw3Muste5xTKYLZkXU1wWU6kd2xTU_nMdtHzTGk2vF56eNg71g9XOlWdvh4Izgq97hpVMy9tUmgM3vCgYyljbP0GuR4_a9Qmq)
Windows下主要通过FindFirstFile和FindNextFile这两个API实现，每枚举到一个文件，都要判断WIN32_FIND_DATA的dwFileAttributes属性，看一下是目录还是文件，如果是目录，可以递归枚举
**[cpp]**[view
 plain](http://blog.csdn.net/wutaozhao/article/details/6044067#)[copy](http://blog.csdn.net/wutaozhao/article/details/6044067#)
- DWORD IterFiles(string srcPath, string destPath)  
- {  
- DWORD dwStatus = 0;  
- 
-     WIN32_FIND_DATA findFileData;  
-     string filePath = srcPath, srcNewPath = srcPath, destNewPath = destPath;  
-     filePath += "\\*.*";  
- HANDLE hFind = FindFirstFile(filePath.c_str(), &findFileData);  
- if (hFind != INVALID_HANDLE_VALUE)  
-     {  
- do
-         {  
-             srcNewPath = srcPath + "\\" + findFileData.cFileName;  
-             destNewPath = destPath + "\\" + findFileData.cFileName;  
- if (strcmp(findFileData.cFileName, ".") == 0 || strcmp(findFileData.cFileName, "..") == 0)  
-             {  
- continue;  
-             }  
- elseif (findFileData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)  
-             {  
-                 dwStatus = IterFiles(srcNewPath, destNewPath);  
-             }  
- else
-             {/* do something */
-             }  
- if (dwStatus != 0)  
-             {  
- break;  
-             }  
-         } while (FindNextFile(hFind, &findFileData));  
-     }  
- 
- return dwStatus;  
- }  
Linux
**[cpp]**[view
 plain](http://blog.csdn.net/wutaozhao/article/details/6044067#)[copy](http://blog.csdn.net/wutaozhao/article/details/6044067#)
- bool CLinDriverRW::IsDirectory(char *pszName)  
- {  
- struct stat S_stat;  
- 
- //取得文件状态
- if (lstat(pszName, &S_stat) < 0)  
-     {  
- returnfalse;  
-     }  
- 
- if (S_ISDIR(S_stat.st_mode))  
-     {  
- returntrue;  
-     }  
- else
-     {  
- returnfalse;  
-     }  
- }  
**[cpp]**[view
 plain](http://blog.csdn.net/wutaozhao/article/details/6044067#)[copy](http://blog.csdn.net/wutaozhao/article/details/6044067#)
- DWORD IterFiles(string srcPath, string destPath)  
- {  
- int iRet = 0;  
-     DIR *pDir;  
-     string destNewPath;  
- struct dirent *pDirent;  
-     pDir = opendir(srcPath.c_str());  
- if (pDir == NULL)  
-     {  
- return -1;  
-     }  
- while ((pDirent = readdir(pDir)) != NULL)  
-     {  
- // if ((pDirent->d_name == '.') || pDirent->d_name[0] == '..')
- if (strcmp(pDirent->d_name, ".") == 0 || strcmp(pDirent->d_name, "..") == 0)  
-         {  
- continue;  
-         }  
- char szTmpPath[1024] = {0};  
-         sprintf(szTmpPath, "%s/%s", srcPath.c_str(), pDirent->d_name);  
-         destNewPath = destPath + "/" + pDirent->d_name;  
- 
- if (IsDirectory(szTmpPath))  
-         {  
- //如果是文件夹则进行递归
-             iRet = IterFiles(szTmpPath, destNewPath);  
- if (iRet < 0)  
-             {  
- break;  
-             }  
-         }  
- else
-         {/* do something */
-         }  
-     }  
- 
-     closedir(pDir);  
- 
- return iRet;  
- }  
