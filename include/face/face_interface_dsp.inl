void* dspmalloc( size_t n ) {
  size_t * p;
  //n = (n+3)&~3;
  DBG( "dspmalloc. len = %d\n", n );
  p = ( size_t * ) Memory_contigAlloc( n + sizeof( size_t ), Memory_DEFAULTALIGNMENT );
  if ( p == NULL ) {
    DBG( "Failed to allocate contiguous memory block. len = %d\n", n );
    //cleanup(THREAD_FAILURE);
  } else {
    //DBG( "Contiguous buffer allocated at physical address %#lx, len = %d\n", Memory_getPhysicalAddress( p ), n );
  }
  return p;
}
void dspfree( void* p, size_t n ) {
  Memory_contigFree( p, n );
}
void* dsp_malloc( size_t n ) {
  size_t * p;
  //n = (n+3)&~3;
    DBG( "Memory_contigAlloc. len = %d\n", n );
  p = ( size_t * ) Memory_contigAlloc( n + sizeof( size_t ), Memory_DEFAULTALIGNMENT );
  if ( p == NULL ) {
    DBG( "Failed to allocate contiguous memory block. len = %d\n", n );
    //cleanup(THREAD_FAILURE);
  } else {
  *p = n;
    //DBG( "Contiguous buffer allocated at physical address %#lx, len = %d\n", Memory_getPhysicalAddress( p ), n );
  }
  return p+1;
}
void dsp_free( void* buf ) {
  if ( buf ) {
          int buflen;
          size_t * p = (size_t*)buf;
          --p;
          buflen = p[0]+sizeof(size_t);
          Memory_contigFree( p, buflen );
  }
}
void* dsp_memclone( void* p, size_t n ) {
  void * a;
  a = dsp_malloc( n );
        if (a) {
  memcpy( a, p, n );
        } else {
                DBG("memclone fail\n");
        }
  return a;
}
Engine_Handle hEngine=0;
VIDENC_Handle hEncode=0;
void iris_recog_close() {
    if (hEncode) {
        //VIDDEC_delete(hEncode);
    }
    if (hEngine) {
        Engine_close(hEngine);
    }
}
int ceruntime_init() {
  DBG("CERuntime_init\n");
  CERuntime_init();
  return 0;
}
void* engine_open() {
  DBG("Engine_open\n");
  hEngine = Engine_open( ENGINE_NAME, NULL, NULL );
  if ( hEngine == NULL ) {
    ERR( "Failed to open codec engine %s\n", ENGINE_NAME );
    iris_recog_close();
    return 0;
  }
  return hEngine;
}
void iris_recog_open() {
  VIDENC_Params params;
  ceruntime_init();
  hEngine = (Engine_Handle)engine_open();
  if ( hEngine == NULL ) {return ;}
  DBG( "Codec Engine opened in video thread\n" );
  /* Allocate and initialize video encoder on the engine */

  params.size = sizeof( VIDENC_Params );
  params.encodingPreset = XDM_DEFAULT;
  params.maxFrameRate = 25000;
  params.dataEndianness = XDM_BYTE;
  params.maxInterFrameInterval = 0;
  params.inputChromaFormat = XDM_YUV_422ILE;
  params.inputContentType = IVIDEO_PROGRESSIVE;
  params.maxHeight = D1_HEIGHT;
  params.maxWidth = D1_WIDTH;

  DBG("VIDENC_create\n");
  /* Create encoder instance */
  hEncode = VIDENC_create( hEngine, "facerecog", &params );

  if ( hEncode == NULL ) {
    ERR( "Can't open encode algorithm.\n" );
    iris_recog_close();
    return ;
  }

  DBG( "face_recog_open ok\n" );
  return ;
}
int iris_recog_process( void* p0) {
  XDAS_Int8* BufArray[ 20 ];
  XDAS_Int32 BufSizeArray[ 20 ], status;
  XDM_BufDesc inBufDesc, outBufDesc;
  VIDENC_InArgs inArgs;
  VIDENC_OutArgs outArgs;
  int i;
  printf("iris_recog_process\n");
        i=0;
        #define SETBUF(_P)  (BufArray[ i ] = ( XDAS_Int8* ) ( NULL==_P ? (void*)(BufArray[0]) : (void*)(GETBUFPTR(size_t, _P, -1)) ), BufSizeArray[ i ] = GETBUFVAL(size_t, _P, -1)+sizeof(size_t), ++i)

        SETBUF(p0);
  inBufDesc.numBufs = 0, inBufDesc.bufSizes = BufSizeArray, inBufDesc.bufs = BufArray;
  outBufDesc.numBufs = i, outBufDesc.bufSizes = BufSizeArray, outBufDesc.bufs = BufArray;
  inArgs.size = sizeof( VIDENC_InArgs );
  outArgs.size = sizeof( VIDENC_OutArgs );
  status = VIDENC_process( hEncode, &inBufDesc, &outBufDesc, &inArgs, &outArgs );
  return status;
}

