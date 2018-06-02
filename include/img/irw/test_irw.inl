
#include "irw.inl"
//#include "emf.inl"

//isio_close
int test_irw()
{
  stream_t s[1] = {0};
  FILE* f = NULL;
  INFOSTR info[1];
  char* fn;
  memset(info, 0, sizeof(*info));
  sys_chdir("E:/pub/bin/imgio/");
  fn = "yaleB11_P00_Ambient.pgm";
  fn = "0135.ani";
  //info->irwpfun = myirwpfun;
  if (f = fopen(fn, "rb")) {
    fstream_init(s, f);
    //cur_get_image_info(s, info);
    //pnm_load_image(s, info);
    ani_load_image(s, info);
    {
      img_t im[1] = {0};
      int linesize = _calcu_scanline_size(info->width, info->bitcount);
      PixFmt fmt = bpp2PixFmt(info->bitcount);
      cvShowImagePal("asdf", info->height, info->width, info->p_bit_data, linesize, info->bitcount/8, fmt, info->palette);
      cvWaitKey(-1);
    }
    fclose(f);
  }
  return 0;
}

