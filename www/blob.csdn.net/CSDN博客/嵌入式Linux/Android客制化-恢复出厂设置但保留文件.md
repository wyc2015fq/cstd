
# Android客制化-恢复出厂设置但保留文件 - 嵌入式Linux - CSDN博客

2018年11月01日 14:24:16[写代码的篮球球痴](https://me.csdn.net/weiqifa0)阅读数：65


很久没有记录了，持之以恒做一件事，需要一定的毅力呐！
最近遇到了一个需求，要求恢复出厂设置保留内置sd卡下某个目录的文件。思来想去，从驱动那边备份校准信号文件得到了一些思路。因为带通话设置的装置需要进行校准，我们会将校准的文件保存在/data下。具体做法呢，在执行恢复出厂设置时，将此文件copy到tmp分区，然后在恢复完成时，再次copy回/data分区。因为我们是备份文件夹，所以我们需要对copy函数进行修改。下面贴出部分代码。
```python
diff --git a/bootable/recovery/recovery.cpp b/bootable/recovery/recovery.cpp
index 598840c..3693cf1 100644
--- a/bootable/recovery/recovery.cpp
+++ b/bootable/recovery/recovery.cpp
@@ -111,6 +111,8 @@ static const char *OTA_FLAG_FILE = "/cache/recovery/last_ota_flag";
 #ifdef STK_BACKUP_OFFSET
 //要备份的文件夹 
+static const char *BACKUP_APK_PATH = "/system/third";
 //临时存储路径
+static const char *TEMP_APK_BACKUP_PATH = "/tmp/third_app";
 #endif
 // Number of lines per page when displaying a file on screen
@@ -286,6 +288,116 @@ static int stk_copy_file(const char *src,const char *dstFilePath)
        printf("stk_copy_file end src=%s,dst=%s\n",src,dstFilePath);
     return 0;
 }
//拷贝文件夹的主体函数
+void error_quit(const char *msg)
+{
+    perror(msg);
+       //printf("something is error %s \n",msg);
+}
+
+void change_path(const char *path)
+{
+    printf("Leave %s Successed . . .\n",getcwd(NULL,0));
+
+    if(chdir(path)==-1){
+               error_quit(path);
+        printf("chdir(path)==-1 %s \n",path);
+               return;
+       }
+
+    printf("Entry %s Successed . . .\n",getcwd(NULL,0));
+}
+
+
+void _copy_file(const char *old_path,const char *new_path)
+{
+    FILE *in,*out;
+    size_t len;
+    char buf[64];
+    char *p=getcwd(NULL,0);
+
+    if((in=fopen(old_path,"rb"))==NULL){
+        error_quit(old_path);
+       printf("(in=fopen(old_path %s \n",old_path);
+       return;}
+    change_path(new_path);
+
+    if((out=fopen(old_path,"wb"))==NULL){
+        error_quit(old_path);
+       printf("out=fopen(old_path %s \n",old_path);
+       return;}
+
+    while(!feof(in))
+    {
+        bzero(buf,sizeof(buf));
+
+        len=fread(&buf,1,sizeof(buf)-1,in);
+        fwrite(&buf,len,1,out);
+    }
+
+    fclose(in);
+    fclose(out);
+
+    change_path(p);
+}
+
+char *get_rel_path(const char *dir,const char *path)
+{
+    char *rel_path;
+    unsigned long d_len,p_len;
+
+    d_len=strlen(dir);
+    p_len=strlen(path);
+    bzero(rel_path,d_len+p_len+2);
+
+    strncpy(rel_path,path,p_len);
+    strncat(rel_path,"/",sizeof(char));
+    strncat(rel_path,dir,d_len);
+
+    return rel_path;
+}
+
+void copy_dir(const char *old_path,const char *new_path)
+{
+    DIR *dir;
+    struct stat buf;
+    struct dirent *dirp;
+    char *p=getcwd(NULL,0);
+
+    if((dir=opendir(old_path))==NULL){
+        error_quit(old_path);
+               printf("dir=opendir(old_path) %s \n",old_path);
+               return;
+       }
+    if(mkdir(new_path,0777)==-1){
+        error_quit(new_path);
+       printf("mkdir(new_path %s \n",new_path);
+       return;
+       }
+
+    change_path(old_path);
+
+    while((dirp=readdir(dir)))
+    {
+        if(strcmp(dirp->d_name,".")==0 || strcmp(dirp->d_name,"..")==0)
+            continue;
+        if(stat(dirp->d_name,&buf)==-1){
+            error_quit("stat");
+               printf("stat(dirp->d_name %s \n",stat);
+               return;
+               }
+        if(S_ISDIR(buf.st_mode))
+        {
+            copy_dir(dirp->d_name,get_rel_path(dirp->d_name,new_path));
+            continue;
+        }
+
+        _copy_file(dirp->d_name,new_path);
+    }
+
+    closedir(dir);
+    change_path(p);
+}
 #endif
 // command line args come from, in decreasing precedence:
 //   - the actual command line
@@ -1326,6 +1438,7 @@ main(int argc, char **argv) {
     }
     #ifdef STK_BACKUP_OFFSET
     //在开始进入恢复出厂时，进行一次拷贝
+    copy_dir(BACKUP_APK_PATH,TEMP_APK_BACKUP_PATH);
     #endif
     device->StartRecovery();
     property_get("UserVolumeLabel", gVolume_label, "");
@@ -1567,8 +1680,11 @@ main(int argc, char **argv) {
     finish_recovery(send_intent);
        #ifdef STK_BACKUP_OFFSET
     //在恢复出厂完成时，我们再次将其拷贝回来
+    copy_dir(TEMP_APK_BACKUP_PATH,BACKUP_APK_PATH);
+    //赋予权限操作
+    chmod(BACKUP_APK_PATH, 0666);
+    chown(BACKUP_APK_PATH, 1000, 1000); // system system
     #endif
 #ifdef FOTA_UPDATE_SUPPORT
     if (perform_fota == 1) {
```
我们只要重点关注在finish_recovery(send_intent);与device->StartRecovery();这两个函数身上即可。我测试过直接从/mnt/sdcard下拷贝，这样是不行的。因为在执行恢复出厂设置时，这个分区此时还未挂载，我通过代码让其先挂载，但是无效。当然也可能在恢复出厂设置时，我们所要备份的文件夹并不存在，所以贴出代码片中的一些判空都进行了return。否则会无法开机，串口log一直重复打印log。
如果各位有其他办法，欢迎留言。有不对之处，欢迎指出。


