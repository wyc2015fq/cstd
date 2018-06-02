#include <stdio.h>
#include <stdlib.h>
#include <stdio.h>
#include <errno.h>
#include <unistd.h>
#include <sys/mount.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <linux/fs.h>
#include <sys/ioctl.h>
#include <errno.h>
int test_mount() {
  const char* dev = "/dev/sdb1";
  const char* dir = "/media/yw/64ae8210-50dd-4c4e-b0d9-f1012cf6832a";
  char buf[256];
  FILE* pf;
  int ret=0;
  dev = "/dev/sdc1", dir = "/media/yw/0086ddf3-ad18-4192-b03d-96096a38a3e2";
  dev = "/dev/sdd1", dir = "/media/yw/64ae8210-50dd-4c4e-b0d9-f1012cf6832a1";
  snprintf(buf, 256, "%s/%s", dir, "aaa.txt");
  remove(buf);
  pf = fopen(buf, "wb");
  if (pf) {
    printf("ok\n");
    fclose(pf);
  } else {
    printf("fail 1\n");
  }
  usleep(100);
  ret = umount(dir);
  printf("umount ret %d errno=%d\n", ret, errno);
  mkdir(dir, S_IRWXU | S_IRWXG | S_IROTH | S_IXOTH);
  ret = mount(dev, dir, "ext3", 0, NULL);
  ret = rmdir(dir);
  printf("mount ret %d errno=%d\n", ret, errno);
  pf = fopen(buf, "rb");
  if (pf) {
    printf("ok\n");
    fclose(pf);
  } else {
    printf("fail 1\n");
  }
  //remove(buf);
  return 0;
}
int test_read_dev()
{
  int fd = 0;
  int sizes = 0;
  char buf[1000] = {0};
  char buf2[1000] = {0};
  int line, j;
  fd = open("/dev/sdb1", O_RDWR);
  if (fd != -1) {
    ioctl(fd, BLKSSZGET, &sizes);
    printf("sector size=%d\n", sizes);
    lseek(fd, 0*sizes, SEEK_SET);
    read(fd, buf, sizes);
    if (1) {
      printf("buf\n");
      for (line = 0; line < 32; line++) {
        for (j = 0; j < 16; j++) {
          printf("%02x ", (unsigned char)buf[line * 16 + j]);
        }
        printf("\n");
      }
    }
    if (1) {
      for (j = 0; j < sizes; ++j) {
        buf2[j] = j;
      }
      lseek(fd, 0, SEEK_SET);
      write(fd, buf2, sizes);
      lseek(fd, 0, SEEK_SET);
      read(fd, buf2, sizes);
      if (1) {
        printf("buf2\n");
        for (line = 0; line < 32; line++) {
          for (j = 0; j < 16; j++) {
            printf("%02x ", (unsigned char)buf2[line * 16 + j]);
          }
          printf("\n");
        }
      }
      lseek(fd, 0, SEEK_SET);
      write(fd, buf, sizes);
    }
    close(fd);
  }
  return (EXIT_SUCCESS);
}

