#include <stdio.h>
#include <stdlib.h>
int test_filebuf()
{
  int i=0;
  FILE* fp;
  char msg1[] = "hello,world\n";
  char msg2[256] = {0};
  if ((fp = fopen("no_buf1.txt", "wb")) == NULL) {
    perror("file open fail");
    exit(-1);
  }
  setbuf(fp, NULL);
  fwrite(msg1, 70, 1, fp);
  fclose(fp);
  fp = fopen("no_buf1.txt", "rb");
  fread(msg2, 100, 1, fp);
  printf("%s\n", msg2);
  fclose(fp);
  return 0;
}
int test_filebuf1()
{
  int i;
  FILE* fp;
  char msg1[] = "hello,world\n";
  char msg2[] = "hello/nworld\n";
  char buf[128];
  /*打开（或者创建）一个文件，然后使用setbuf设置为nobuf情况，并检查关闭前流的情况*/
  if ((fp = fopen("no_buf1.txt", "w")) == NULL) {
    perror("file open fail");
    exit(-1);
  }
  setbuf(fp, NULL);
  fwrite(msg1, 7, 1, fp);
  printf("test setbuf(no buf)!check no_buf1.txt\n");//查看 buf 情况
  printf("press enter to continue!\n");
  getchar();
  fclose(fp);//关闭流，因此将回写 buf（如果有 buf 的话）
  /*打开（或者创建）一个文件，然后使用 setvbuf 设置为 nobuf 情况，并检查关闭前流的情况*/
  if ((fp = fopen("no_buf2.txt", "w")) == NULL) {
    perror("file open failure!");
    exit(-1);
  }
  setvbuf(fp, NULL, _IONBF, 0);      //设置为无 buf
  fwrite(msg1, 7, 1, fp);
  printf("test setbuf(no buf)!check no_buf1.txt\n");        //查看 buf 情况
  printf("press enter to continue!\n");
  getchar();
  fclose(fp);//关闭流，因此将回写 buf ( 如果有 buf 的话)
  if ((fp = fopen("1_buf.txt", "w")) == NULL) {
    perror("fail open file");
    exit(-1);
  }
  setvbuf(fp, buf, _IOLBF, sizeof(buf));     //设置为行 buf
  fwrite(msg2, sizeof(msg2), 1, fp);     //写内容
  printf("test setvbuf(line buf)!check 1_buf.txt,because line buf,only data before enter send to file\n");        //查看 buf 情况
  printf("press enter to continue!\n");
  getchar();
  fclose(fp);        //关闭流，因此将回写 buf
  //打开（或者创建）一个文件，然后使用 setvbuf 设置为全 buf情况，并检查关闭前流的情况
  if ((fp = fopen("f_buf.txt", "w")) == NULL) {
    perror("file open failure!");
    exit(-1);
  }
  setvbuf(fp, buf, _IOFBF, sizeof(buf));
  for (i = 0; i < 2; i++) {
    fputs(msg1, fp);
  }
  printf("test setbuf(full buf)! check f_buf.txt\n");
  printf("press enter to continue!\n");
  getchar();
  fclose(fp);//关闭流，因此将回写 buf
  return 0;
}

