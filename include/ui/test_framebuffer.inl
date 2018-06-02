#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <linux/fb.h>
#include <sys/mman.h>
struct fb_fix_screeninfo FixedInfo;
struct fb_var_screeninfo OrigVarInfo;
static int FrameBufferFD = -1;
void* FrameBuffer = (void*) - 1;
void openFBDEV(void)
{
  /* open the framebuffer device */
  FrameBufferFD = open("/dev/fb0", O_RDWR);
  if (FrameBufferFD < 0) {
    fprintf(stderr, "Error opening /dev/fb0\n");
    exit(1);
  }
  /* Get the fixed screen info */
  if (ioctl(FrameBufferFD, FBIOGET_FSCREENINFO, &FixedInfo)) {
    fprintf(stderr, "error: ioctl(FBIOGET_FSCREENINFO) failed\n");
    exit(1);
  }
  /* get the variable screen info */
  if (ioctl(FrameBufferFD, FBIOGET_VSCREENINFO, &OrigVarInfo)) {
    fprintf(stderr, "error: ioctl(FBIOGET_VSCREENINFO) failed\n");
    exit(1);
  }
  if (FixedInfo.visual != FB_VISUAL_TRUECOLOR && FixedInfo.visual != FB_VISUAL_DIRECTCOLOR) {
    fprintf(stderr, "non-TRUE/DIRECT-COLOR visuals (0x%x) not supported by this demo.\n", FixedInfo.visual);
    exit(1);
  }
  /*
  * fbdev says the frame buffer is at offset zero, and the mmio region
  * is immediately after.
  */
  /* mmap the framebuffer into our address space */
  FrameBuffer = (void*) mmap(0, /* start */
      FixedInfo.smem_len, /* bytes */
      PROT_READ | PROT_WRITE, /* prot */
      MAP_SHARED, /* flags */
      FrameBufferFD, /* fd */
      0 /* offset */);
  if (FrameBuffer == (void*) - 1) {
    fprintf(stderr, "error: unable to mmap framebuffer\n");
    exit(1);
  }
}
void closeFBDEV(void)
{
  munmap(FrameBuffer, FixedInfo.smem_len);
  close(FrameBufferFD);
}
int test_framebuffer()
{
  openFBDEV();
  fprintf(stderr, "openFBDEV finish\n");
  memset(FrameBuffer, 128, FixedInfo.smem_len);
  sleep(5);
  closeFBDEV();
  fprintf(stderr, "closeFBDEV finish\n");
  return 0;
}

