
#if 0
TEFFECT* new_expressionT(int height, int width) {
  TEFFECT* expr = NULL;
  expr = (TEFFECT*)malloc( sizeof(TEFFECT) + (height * width)*4 );
  MEMSET( expr, 0, 1 );
  expr->height = height, expr->width = width;
  expr->fg_h = height, expr->fg_w = width, expr->fg_cn = 4;
  expr->foreground = (unsigned char*)(expr+1);
  return expr;
}

void del_expressionT( TEFFECT** pexpr ) {
  if ( pexpr && *pexpr ) {
    TEFFECT* expr = *pexpr;
    SAFEFREE(expr->starface);
    SAFEFREE( *pexpr );
  }
}

//#include "debug.h"
void config_expressionT( TEFFECT* expr, int type, const TTCHAR* inifile, const TTCHAR* effect_name ) {
  TTCHAR picpath[ 256 ];
  img_t im[1]={0};
      int height, width, tmpstep, cn;
  expr->picidx = 0;
  expr->type |= type;
  if ( ET_EFFECT2 & type ) {
    expr->leye.x = TIniGetInt( inifile, effect_name, _TT( "leyex" ), 0 );
    expr->leye.y = TIniGetInt( inifile, effect_name, _TT( "leyey" ), 0 );
    expr->reye.x = TIniGetInt( inifile, effect_name, _TT( "reyex" ), 0 );
    expr->reye.y = TIniGetInt( inifile, effect_name, _TT( "reyey" ), 0 );
    IniGetString( inifile, effect_name, _TT( "picspec" ), _TT( "" ), expr->picspec, 256 );
  }
#if 0
  if ( ET_EFFECT3 & type ) {
    IniGetString( inifile, effect_name, _TT( "foreground" ), _TT( "" ), picpath, 256 );
    //_trace_dbg(foregroundpic);
    imread( picpath, 4, 1, im );
    height=im->height, width=im->width, cn=im->cn;
    if ( im->data ) {
      int fgstep;
      unsigned char* tmp=0;
      fgstep = expr->fg_w * expr->fg_cn;
      tmpstep = width * 4;
      tmp = MALLOC( unsigned char, height * tmpstep );
      IMRESIZE( height, width, tmp, tmpstep, 4, expr->height, expr->width, expr->foreground, fgstep, 4, INTER_BILINEARITY );
      free_reader( reader );
      SAFEFREE(tmp);
      //imwrite(_TT("girl_gaussian.bmp"), height, width, data, step, channels);
    }
  }
  if ( ET_EFFECT8 & type ) {
    SAFEFREE(expr->starface);
    IniGetString( inifile, effect_name, _TT( "starface" ), _TT( "" ), picpath, 256 );
    reader = new_readerT( picpath, &height, &width, &cn );
    if ( reader ) {
      tmpstep = width * 3;
      expr->starface_height = height, expr->starface_width = width;
      expr->starface = MALLOC( unsigned char, height * tmpstep );
      imread( reader, expr->starface, tmpstep, 3 );
      free_reader( reader );
    }
  }
#endif
}

int add_expressionT( TEFFECT* expr, unsigned char* img, int al, int ai ) {
  XRECT face[ 100 ];
  XRECT eyes[ 100 ];
  int face_num, i, j, type = expr->type;
  struct RGB_t tms = {
                       0, 0, 0
                     };
  int aa = 9;//(int)time(NULL);
  int h = expr->height, w = expr->width;
  srand(aa);
  //RAND_INIT;
  if ( ET_EFFECT8 & type && expr->starface && 3==ai) {
    starface1(
      expr->starface_height, expr->starface_width,
      expr->starface, expr->starface_width*ai,
      h, w, img, al, ai);
  }
  if ( ET_EFFECT1 & type || ET_EFFECT2 & type ) {
    unsigned char * gry = img;
    int gl = al;
    if ( 3 == ai ) {
      gry = MALLOC( unsigned char, h * w );
      gl = w;
      IMTRANS( h, w, img, al, ai, gry, gl, 1, BGR2GRAY );
    }
    //IMTRANS(h, w, img, al, ai, gry, w, 1, BGR2GRAY);
    //SKIP_FILTEREX(h, w, img, al, ai, buf, al, ai, 1, 1, DILATION3X3_FT1, unsigned char);
    {
      //const void* cas = CPM_GetCascade( VIS_FACE_CASCADE );
      const void* cas = vis_nesting_face0701;
      //utime_start(s);
      //imwrite("a.bmp", h, w, gry, gl, 1);
      face_num = CPM_FaceDetect( cas, h, w, gry, gl, 0, 0,
                                 0.25, 1.f, 100.f, 1.25f, 0, 0.8f, 1, face, 100 );
      //printf("ºÄÊ± %10.9f Ãë, face_num=%3d\n", utime_elapsed(s), face_num);
    }
    
    if ( !face_num )
      expr->picidx = 0;
    for ( i = 0; i < face_num; ++i ) {
      int eyes_num = 0;
#if 1
      const void* lcas = VisAllEye_nesting;
      const void* rcas = VisAllEye_nesting;
#else
      const void* lcas = NIR_LEYE_GLASS_CASCADE;
      const void* rcas = NIR_REYE_GLASS_CASCADE;
#endif
      //DrawRectangle2( h, w, img, al, ai, face[ i ], CC_RGB( 255, 0, 0 ), 3 );
      
      eyes_num = CPM_EyesDetect( 0, lcas, rcas, 0, 0, 0, 0, gry, gl, 5, face + i, eyes, 100 );
                                 
      for ( j = 1; j < 3; ++j ) {
        //DrawRectangle2( h, w, img, al, ai, eyes[ j ], CC_RGB( 255, 0, 0 ), 1 );
      }
      if ( !eyes[ 1 ].cx ) {
        eyes[ 1 ].cy = face[ i ].cx / 16;
        eyes[ 1 ].cx = face[ i ].cx / 8;
        eyes[ 1 ].y = face[ i ].y + face[ i ].cx / 4 - eyes[ 1 ].cy / 2;
        eyes[ 1 ].x = face[ i ].x + face[ i ].cx / 4 - eyes[ 1 ].cx / 2;
      }
      if ( !eyes[ 2 ].cx ) {
        eyes[ 2 ].cy = face[ i ].cx / 16;
        eyes[ 2 ].cx = face[ i ].cx / 8;
        eyes[ 2 ].y = face[ i ].y + face[ i ].cx / 4 - eyes[ 1 ].cy / 2;
        eyes[ 2 ].x = face[ i ].x + face[ i ].cx / 4 - eyes[ 1 ].cx / 2 + face[ i ].cx / 2;
      }
      if ( ET_EFFECT1 & type ) {
        int xx, yy;
        for ( j = 1; j < 3; ++j ) {
          xx = eyes[ j ].x + eyes[ j ].cx / 2;
          yy = eyes[ j ].y + eyes[ j ].cy / 2;
          //Spherize_trans(h,w,( unsigned char* )img, al, ai, xx, yy, eyes[j].cx, 0.5);
          sphere( h, w, img, al, ai, img, al, ai, xx, yy, eyes[ j ].cx, expr->eyes_sphere );
        }
        xx = face[ i ].x + face[ i ].cx / 2;
        yy = face[ i ].y + face[ i ].cy / 2;
        sphere( h, w, img, al, ai, img, al, ai, xx, yy, ( int ) ( face[ i ].cx / 1.5 ), expr->face_sphere );
      }
      if ( ET_EFFECT2 & type ) {
        TTCHAR picpath[ 256 ];
        _sntprintfT( picpath, 256, expr->picspec, ++( expr->picidx ) );
#if 0
        
        if ( expr->picspec[ 0 ] ) {
          int height, width, channels = 4;
          real lx = ( real ) ( eyes[ 1 ].x + eyes[ 1 ].cx / 2 ) / w;
          real ly = ( real ) ( eyes[ 1 ].y + eyes[ 1 ].cy / 2 ) / h;
          real rx = ( real ) ( eyes[ 2 ].x + eyes[ 2 ].cx / 2 ) / w;
          real ry = ( real ) ( eyes[ 2 ].y + eyes[ 2 ].cy / 2 ) / h;
          struct fmtimgreader* reader = new_readerT( picpath, &height, &width, 0 );
          // _tprintf(_TT("%s asdf\n"), picpath);
          if ( !reader ) {
            expr->picidx = 0;
            _sntprintfT( picpath, 256, expr->picspec, ++( expr->picidx ) );
            reader = new_readerT( picpath, &height, &width, 0 );
          }
          if ( reader ) {
            int step = width * channels;
            unsigned char* data = MALLOC(unsigned char, height * step );
            unsigned char* stdimg = MALLOC(unsigned char, h * w * channels );
            //_tprintf(_TT("%s\n"), picpath);
            memset( data, 0, height * step );
            memset( stdimg, 0, h * w * channels );
            imread( reader, data, step, channels );
            free_reader( reader );
            CPM_FaceStd1( height, width, data, step, channels, h, w, stdimg, w * channels, channels,
                         expr->leye, expr->reye, lx, ly, rx, ry );
            //imwrite(_TT("girl_gaussian.bmp"), height, width, data, step, channels);
            //imwrite(_TT("stdimg.bmp"), h, w, stdimg, w*channels, channels);
            //COPY_MARK(h, w, (struct RGB_t*)stdimg, w, (struct RGB_t*)img, w, tms);
            imcopy( h, w, stdimg, w * channels, 4, img, w * ai, ai, ai, 1 );
            SAFEFREE( data );
            SAFEFREE( stdimg );
          }
        }
      }
#endif
      
    }
    if ( 3 == ai ) {
      SAFEFREE( gry );
    }
  }
  
  if ( ( ET_EFFECT4 & type ) && expr->splash_radius ) {
    glass_splash( h, w, img, al, ai, img, al, ai, expr->splash_radius, expr->splash_radius );
  }
  
  if ( ( ET_EFFECT5 & type ) && expr->mosaic_size ) {
    skip_mosaic( h, w, img, al, ai, img, al, ai, expr->mosaic_size, expr->mosaic_size );
  }
  
  if ( ( ET_EFFECT6 & type ) && expr->windy_step ) {
    skip_windy( h, w, img, al, ai, img, al, ai, expr->windy_step, 1 );
  }
  
  if ( ( ET_EFFECT7 & type ) && expr->stripe_swing && expr->stripe_frequency ) {
    skip_stripe( h, w, img, al, ai, img, al, ai, expr->stripe_swing, expr->stripe_frequency );
  }
  
  if ( ET_EFFECT3 & type && expr->foreground ) {
    imcopy( expr->fg_h, expr->fg_w, expr->foreground, expr->fg_w * expr->fg_cn, expr->fg_cn, img, w * ai, ai, ai, 1 );
  }
  return 0;
}

#endif
