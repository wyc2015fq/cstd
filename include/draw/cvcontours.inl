
#ifdef WIN32
#include "cstd.h"
#include "macro.h"
#else
#include "../../include/cstd.h"
#include "../../include/macro.h"
#endif


/* initializes 8-element array for fast access to 3x3 neighborhood of a pixel */
#define  CC_INIT_3X3_DELTAS( deltas, step, nch )         \
  ((deltas)[0] = (nch),  (deltas)[1] = -(step) + (nch),  \
      (deltas)[2] = -(step), (deltas)[3] = -(step) - (nch),  \
      (deltas)[4] = -(nch),  (deltas)[5] =  (step) - (nch),  \
      (deltas)[6] =  (step), (deltas)[7] =  (step) + (nch))

#if 0

CC_IMPL void
cvStartReadChainPoints(CvChain* chain, CvChainPtReader* reader)
{
  int i;

  CC_FUNCNAME("cvStartReadChainPoints");

  __BEGIN__;

  if (!chain || !reader) {
    CC_ERROR(CC_StsNullPtr, "");
  }

  if (chain->elem_size != 1 || chain->header_size < (int) sizeof(CvChain)) {
    CC_ERROR_FROM_STATUS(CC_BADSIZE_ERR);
  }

  cvStartReadSeq((CvSeq*) chain, (CvSeqReader*) reader, 0);
  CC_CHECK();

  reader->pt = chain->origin;

  for (i = 0; i < 8; i++) {
    reader->deltas[ i ][ 0 ] = (char) icvCodeDeltas[ i ].x;
    reader->deltas[ i ][ 1 ] = (char) icvCodeDeltas[ i ].y;
  }

  __END__;
}

/* retrieves next point of the chain curve and updates reader */
CC_IMPL IPOINT
cvReadChainPoint(CvChainPtReader* reader)
{
  char* ptr;
  int code;
  IPOINT pt = { 0, 0 };

  CC_FUNCNAME("cvReadChainPoint");

  __BEGIN__;

  if (!reader) {
    CC_ERROR(CC_StsNullPtr, "");
  }

  pt = reader->pt;

  ptr = reader->ptr;

  if (ptr) {
    code = *ptr++;

    if (ptr >= reader->block_max) {
      cvChangeSeqBlock((CvSeqReader*) reader, 1);
      ptr = reader->ptr;
    }

    reader->ptr = ptr;
    reader->code = (char) code;
    assert((code & ~7) == 0);
    reader->pt.x = pt.x + icvCodeDeltas[ code ].x;
    reader->pt.y = pt.y + icvCodeDeltas[ code ].y;
  }

  __END__;

  return pt;
}

#endif

/* Calculates bounding rectagnle of a point set or retrieves already calculated */
CC_IMPL CvRect
cvBoundingRect(void* ptseq, int ptseq_total, int is_float, int update)
{
  CvRect rect = { 0, 0, 0, 0 };
  int xmin = 0, ymin = 0, xmax = -1, ymax = -1, i;
  //int calculate = update;

  CC_FUNCNAME("cvBoundingRect");

  __BEGIN__;

  if (ptseq_total) {
    int ptseq_len = 0;
#define cvStartReadSeq(_a, _b, _c)  _a##_len=0
#undef CC_READ_SEQ_ELEM
#define CC_READ_SEQ_ELEM(_T, _b, _a)    _b = ((_T*)(_a))[_a##_len++]
    //int  is_float = CC_SEQ_ELTYPE(ptseq) == CC_32FC2;
    cvStartReadSeq(ptseq, &reader, 0);

    if (!is_float) {
      IPOINT pt;
      /* init values */
      CC_READ_SEQ_ELEM(IPOINT, pt, ptseq);
      xmin = xmax = pt.x;
      ymin = ymax = pt.y;

      for (i = 1; i < ptseq_total; i++) {
        CC_READ_SEQ_ELEM(IPOINT, pt, ptseq);

        if (xmin > pt.x) {
          xmin = pt.x;
        }

        if (xmax < pt.x) {
          xmax = pt.x;
        }

        if (ymin > pt.y) {
          ymin = pt.y;
        }

        if (ymax < pt.y) {
          ymax = pt.y;
        }
      }
    }
    else {
      IPOINT pt;
      suf32_t v;
      /* init values */
      CC_READ_SEQ_ELEM(IPOINT, pt, ptseq);
      xmin = xmax = CC_TOGGLE_FLT(pt.x);
      ymin = ymax = CC_TOGGLE_FLT(pt.y);

      for (i = 1; i < ptseq_total; i++) {
        CC_READ_SEQ_ELEM(IPOINT, pt, ptseq);
        pt.x = CC_TOGGLE_FLT(pt.x);
        pt.y = CC_TOGGLE_FLT(pt.y);

        if (xmin > pt.x) {
          xmin = pt.x;
        }

        if (xmax < pt.x) {
          xmax = pt.x;
        }

        if (ymin > pt.y) {
          ymin = pt.y;
        }

        if (ymax < pt.y) {
          ymax = pt.y;
        }
      }

      v.i = CC_TOGGLE_FLT(xmin);
      xmin = FLOOR(v.f);
      v.i = CC_TOGGLE_FLT(ymin);
      ymin = FLOOR(v.f);
      /* because right and bottom sides of
         the bounding rectangle are not inclusive
         (note +1 in width and height calculation below),
         FLOOR is used here instead of cvCeil */
      v.i = CC_TOGGLE_FLT(xmax);
      xmax = FLOOR(v.f);
      v.i = CC_TOGGLE_FLT(ymax);
      ymax = FLOOR(v.f);
    }
  }

  rect.x = xmin;
  rect.y = ymin;
  rect.width = xmax - xmin + 1;
  rect.height = ymax - ymin + 1;

  if (update) {
    ((CvContour*) ptseq) ->rect = rect;
  }

  __END__;

  return rect;
}

/****************************************************************************************\
*                         Raster->Chain Tree (Suzuki algorithms)                         *
\****************************************************************************************/

/*
  Structure that is used for sequental retrieving contours from the image.
  It supports both hierarchical and plane variants of Suzuki algorithm.
*/
typedef struct _CvContourScanner {
  //CvMemStorage *storage1;     /* contains fetched contours */
  //CvMemStorage *storage2;     /* contains approximated contours
  //                               (!=storage1 if approx_method2 != approx_method1) */
  //CvMemStorage *cinfo_storage;        /* contains _CvContourInfo nodes */
  //CvSet *cinfo_set;           /* set of _CvContourInfo nodes */
  //CvMemStoragePos initial_pos;        /* starting storage pos */
  //CvMemStoragePos backup_pos; /* beginning of the latest approx. contour */
  //CvMemStoragePos backup_pos2;        /* ending of the latest approx. contour */
  char* img0;                 /* image origin */
  char* img;                  /* current image row */
  int img_step;               /* image step */
  CvSize img_size;            /* ROI size */
  IPOINT offset;             /* ROI offset: coordinates, added to each contour point */
  IPOINT pt;                 /* current scanner position */
  IPOINT lnbd;               /* position of the last met contour */
  int nbd;                    /* current mark val */
  _CvContourInfo* l_cinfo;    /* information about latest approx. contour */
  _CvContourInfo cinfo_temp;  /* temporary var which is used in simple modes */
  _CvContourInfo frame_info;  /* information about frame */
  //CvSeq frame;                /* frame itself */
  int approx_method1;         /* approx method when tracing */
  int approx_method2;         /* final approx method */
  int mode;                   /* contour scanning mode:
                                     0 - external only
                                     1 - all the contours w/o any hierarchy
                                     2 - connected components (i.e. two-level structure -
                                     external contours and holes) */
  int subst_flag;
  int seq_type1;              /* type of fetched contours */
  int header_size1;           /* hdr size of fetched contours */
  int elem_size1;             /* elem size of fetched contours */
  int seq_type2;              /*                                       */
  int header_size2;           /*        the same for approx. contours  */
  int elem_size2;             /*                                       */
  _CvContourInfo* cinfo_table[ 126 ];
  CONTOURS* contours;
}
_CvContourScanner;

#define _CC_FIND_CONTOURS_FLAGS_EXTERNAL_ONLY    1
#define _CC_FIND_CONTOURS_FLAGS_HIERARCHIC       2

/*
   Initializes scanner structure.
   Prepare image for scanning ( clear borders and convert all pixels to 0-1.
*/
CC_IMPL CvContourScanner
cvStartFindContours(_CvContourScanner* scanner, int height, int width, const unsigned char* img0, int img_step,
    int mode, int method, IPOINT offset, CONTOURS* contours)
{
  int y;
  int step;
  CvSize size;
  unsigned char* img;

  CC_FUNCNAME("cvStartFindContours");

  __BEGIN__;

  if (!scanner) {
    CC_ERROR_FROM_STATUS(CC_OUTOFMEM_ERR);
  }

  if (!contours) {
    CC_ERROR(CC_StsNullPtr, "");
  }

  if (method < 0 || method > CC_CHAIN_APPROX_TC89_KCOS) {
    CC_ERROR_FROM_STATUS(CC_BADRANGE_ERR);
  }

  //if( !CC_IS_MASK_ARR( mat ))
  //    CC_ERROR( CC_StsUnsupportedFormat, "[Start]FindContours support only 8uC1 images" );
  BUFUSEBEGIN(contours->buf, contours->buflen);
  img = BUFMALLOC(unsigned char, height * width);
  BUFUSEEND();
  size = cvSize(width, height);
  step = width;
  MEMCPY2D(height, width, img0, img_step, img, step);

  //if( header_size < (int) (method == CC_CHAIN_CODE ? sizeof( CvChain ) : sizeof( CvContour )))
  //    CC_ERROR_FROM_STATUS( CC_BADSIZE_ERR );

  memset(scanner, 0, sizeof(*scanner));

  scanner->contours = contours;
  //scanner->storage1 = scanner->storage2 = 0;
  scanner->img0 = (char*) img;
  scanner->img = (char*)(img + step);
  scanner->img_step = step;
  scanner->img_size.width = size.width - 1;   /* exclude rightest column */
  scanner->img_size.height = size.height - 1; /* exclude bottomost row */
  scanner->mode = mode;
  scanner->offset = offset;
  scanner->pt.x = scanner->pt.y = 1;
  scanner->lnbd.x = 0;
  scanner->lnbd.y = 1;
  scanner->nbd = 2;
  scanner->mode = (int) mode;
  //scanner->frame_info.contour = 0; //&(scanner->frame);
  scanner->frame_info.is_hole = 1;
  scanner->frame_info.next = 0;
  scanner->frame_info.parent = 0;
  scanner->frame_info.rect = cvRect(0, 0, size.width, size.height);
  scanner->l_cinfo = 0;
  scanner->subst_flag = 0;

  //scanner->frame.flags = CC_SEQ_FLAG_HOLE;

  scanner->approx_method2 = scanner->approx_method1 = method;

  if (method == CC_CHAIN_APPROX_TC89_L1 || method == CC_CHAIN_APPROX_TC89_KCOS) {
    scanner->approx_method1 = CC_CHAIN_CODE;
  }

  if (scanner->approx_method1 == CC_CHAIN_CODE) {
    scanner->seq_type1 = CC_SEQ_CHAIN_CONTOUR;
    //scanner->header_size1 = scanner->approx_method1 == scanner->approx_method2 ?
    //    header_size : sizeof( CvChain );
    scanner->elem_size1 = sizeof(char);
  }
  else {
    scanner->seq_type1 = CC_SEQ_POLYGON;
    //scanner->header_size1 = scanner->approx_method1 == scanner->approx_method2 ?
    //    header_size : sizeof( CvContour );
    scanner->elem_size1 = sizeof(IPOINT);
  }

  //scanner->header_size2 = header_size;

  if (scanner->approx_method2 == CC_CHAIN_CODE) {
    scanner->seq_type2 = scanner->seq_type1;
    scanner->elem_size2 = scanner->elem_size1;
  }
  else {
    scanner->seq_type2 = CC_SEQ_POLYGON;
    scanner->elem_size2 = sizeof(IPOINT);
  }

  scanner->seq_type1 = scanner->approx_method1 == CC_CHAIN_CODE ?
      CC_SEQ_CHAIN_CONTOUR : CC_SEQ_POLYGON;

  scanner->seq_type2 = scanner->approx_method2 == CC_CHAIN_CODE ?
      CC_SEQ_CHAIN_CONTOUR : CC_SEQ_POLYGON;

  contours->flags = scanner->seq_type1;
  //cvSaveMemStoragePos( storage, &(scanner->initial_pos) );

  if (method > CC_CHAIN_APPROX_SIMPLE) {
    //scanner->storage1 = cvCreateChildMemStorage( scanner->storage2 );
  }

  if (mode > CC_RETR_LIST) {
    //scanner->cinfo_storage = cvCreateChildMemStorage( scanner->storage2 );
    //scanner->cinfo_set = cvCreateSet( 0, sizeof( CvSet ), sizeof( _CvContourInfo ),
    //                                  scanner->cinfo_storage );
    //if( scanner->cinfo_storage == 0 || scanner->cinfo_set == 0 )
    //    CC_ERROR_FROM_STATUS( CC_OUTOFMEM_ERR );
  }

  /* make zero borders */
  memset(img, 0, size.width);
  memset(img + step * (size.height - 1), 0, size.width);

  for (y = 1; y < size.height - 1; y++) {
    int pos = y * step;
    img[ pos ] = img[ pos + size.width - 1 ] = 0;
  }

  /* converts all pixels to 0 or 1 */
  //cvThreshold( mat, mat, 0, 1, CC_THRESH_BINARY );
  cvThreshold(height, width, img, step, img, step, 1, 1, CC_THRESH_BINARY);
  //imwrite("./asdf.bmp", 300, 400, img, 400, 1);
  CC_CHECK();

  __END__;

  return scanner;
}

/*
   Final stage of contour processing.
   Three variants possible:
      1. Contour, which was retrieved using border following, is added to
         the contour tree. It is the case when the icvSubstituteContour function
         was not called after retrieving the contour.

      2. New contour, assigned by icvSubstituteContour function, is added to the
         tree. The retrieved contour itself is removed from the storage.
         Here two cases are possible:
            2a. If one deals with plane variant of algorithm
                (hierarchical strucutre is not reconstructed),
                the contour is removed completely.
            2b. In hierarchical case, the header of the contour is not removed.
                It's marked as "link to contour" and h_next pointer of it is set to
                new, substituting contour.

      3. The similar to 2, but when NULL pointer was assigned by
         icvSubstituteContour function. In this case, the function removes
         retrieved contour completely if plane case and
         leaves header if hierarchical (but doesn't mark header as "link").
      ------------------------------------------------------------------------
      The 1st variant can be used to retrieve and store all the contours from the image
      (with optional convertion from chains to contours using some approximation from
      restriced set of methods). Some characteristics of contour can be computed in the
      same pass.

      The usage scheme can look like:

      icvContourScanner scanner;
      CvMemStorage*  contour_storage;
      CvSeq*  first_contour;
      CvStatus  result;

      ...

      icvCreateMemStorage( &contour_storage, block_size/0 );

      ...

      cvStartFindContours
              ( img, contour_storage,
                header_size, approx_method,
                [external_only,]
                &scanner );

      for(;;)
      {
          [CvSeq* contour;]
          result = icvFindNextContour( &scanner, &contour/0 );

          if( result != CC_OK ) break;

          // calculate some characteristics
          ...
      }

      if( result < 0 ) goto error_processing;

      cvEndFindContours( &scanner, &first_contour );
      ...

      -----------------------------------------------------------------

      Second variant is more complex and can be used when someone wants store not
      the retrieved contours but transformed ones. (e.g. approximated with some
      non-default algorithm ).

      The scheme can be the as following:

      icvContourScanner scanner;
      CvMemStorage*  contour_storage;
      CvMemStorage*  temp_storage;
      CvSeq*  first_contour;
      CvStatus  result;

      ...

      icvCreateMemStorage( &contour_storage, block_size/0 );
      icvCreateMemStorage( &temp_storage, block_size/0 );

      ...

      icvStartFindContours8uC1R
              ( <img_params>, temp_storage,
                header_size, approx_method,
                [retrival_mode],
                &scanner );

      for(;;)
      {
          CvSeq* temp_contour;
          CvSeq* new_contour;
          result = icvFindNextContour( scanner, &temp_contour );

          if( result != CC_OK ) break;

          <approximation_function>( temp_contour, contour_storage,
                                    &new_contour, <parameters...> );

          icvSubstituteContour( scanner, new_contour );
          ...
      }

      if( result < 0 ) goto error_processing;

      cvEndFindContours( &scanner, &first_contour );
      ...

      ----------------------------------------------------------------------------
      Third method to retrieve contours may be applied if contours are irrelevant
      themselves but some characteristics of them are used only.
      The usage is similar to second except slightly different internal loop

      for(;;)
      {
          CvSeq* temp_contour;
          result = icvFindNextContour( &scanner, &temp_contour );

          if( result != CC_OK ) break;

          // calculate some characteristics of temp_contour

          icvSubstituteContour( scanner, 0 );
          ...
      }

      new_storage variable is not needed here.

      Two notes.
      1. Second and third method can interleave. I.e. it is possible to
         remain contours that satisfy with some criteria and reject others.
         In hierarchic case the resulting tree is the part of original tree with
         some nodes absent. But in the resulting tree the contour1 is a child
         (may be indirect) of contour2 iff in the original tree the contour1
         is a child (may be indirect) of contour2.
*/
static void
icvEndProcessContour(CvContourScanner scanner)
{
  _CvContourInfo* l_cinfo = scanner->l_cinfo;

  if (l_cinfo) {
    if (scanner->subst_flag) {
      //CvMemStoragePos temp = {0};

      //cvSaveMemStoragePos( scanner->storage2, &temp );

      //if( temp.top == scanner->backup_pos2.top && temp.free_space == scanner->backup_pos2.free_space ) {
      //cvRestoreMemStoragePos( scanner->storage2, &scanner->backup_pos );  }
      scanner->subst_flag = 0;
    }

    //if ( l_cinfo->contour ) {
    //cvInsertNodeIntoTree( l_cinfo->contour, l_cinfo->parent->contour, &(scanner->frame) ); }
    scanner->l_cinfo = 0;
  }
}

/* replaces one contour with another */
CC_IMPL void
cvSubstituteContour(CvContourScanner scanner, CvSeq* new_contour)
{
  _CvContourInfo* l_cinfo;

  CC_FUNCNAME("cvSubstituteContour");

  __BEGIN__;

  if (!scanner) {
    CC_ERROR(CC_StsNullPtr, "");
  }

  l_cinfo = scanner->l_cinfo;

  if (l_cinfo  //&& l_cinfo->contour && l_cinfo->contour != new_contour
     ) {
    //l_cinfo->contour = new_contour;
    scanner->subst_flag = 1;
  }

  __END__;
}

/*
    marks domain border with +/-<constant> and stores the contour into CvSeq.
        method:
            <0  - chain
            ==0 - direct
            >0  - simple approximation
*/
static CvStatus
icvFetchContour(char* ptr,
    int step,
    IPOINT pt,
    MINIVEC_CvPoint_t* contour,
    int is_hole,
    int _method,
    CvRect* prect)
{
  const char nbd = 2;
  int deltas[ 16 ];
  //CvSeqWriter     writer;
  char* i0 = ptr, *i1, *i3, *i4 = 0;
  int prev_s = -1, s, s_end;
  int method = _method - 1;
  MINIVEC_char_t* contour_code = (MINIVEC_char_t*) contour;

  assert((unsigned) _method <= CC_CHAIN_APPROX_SIMPLE);

  /* initialize local state */
  CC_INIT_3X3_DELTAS(deltas, step, 1);
  memcpy(deltas + 8, deltas, 8 * sizeof(deltas[ 0 ]));

  /* initialize writer */
  cvStartAppendToSeq(contour, &writer);

  if (method < 0) {
    ((CvChain*) contour) ->origin = pt;
  }

  s_end = s = CC_IS_SEQ_HOLE(contour) ? 0 : 4;

  do {
    s = (s - 1) & 7;
    i1 = i0 + deltas[ s ];

    if (*i1 != 0) {
      break;
    }
  }
  while (s != s_end);

  if (s == s_end) {             /* single pixel domain */
    *i0 = (char)(nbd | -128);

    if (method >= 0) {
      CC_WRITE_SEQ_ELEM(pt, contour);
    }
  }
  else {
    i3 = i0;
    prev_s = s ^ 4;

    /* follow border */
    for (;;) {
      s_end = s;

      for (;;) {
        i4 = i3 + deltas[ ++s ];

        if (*i4 != 0) {
          break;
        }
      }

      s &= 7;

      /* check "right" bound */
      if ((unsigned)(s - 1) < (unsigned) s_end) {
        *i3 = (char)(nbd | -128);
      }
      else if (*i3 == 1) {
        *i3 = nbd;
      }

      if (method < 0) {
        char _s = (char) s;
        CC_WRITE_SEQ_ELEM(_s, contour_code);
      }
      else {
        if (s != prev_s || method == 0) {
          CC_WRITE_SEQ_ELEM(pt, contour);
          prev_s = s;
        }

        pt.x += icvCodeDeltas[ s ].x;
        pt.y += icvCodeDeltas[ s ].y;

      }

      if (i4 == i0 && i3 == i1) {
        break;
      }

      i3 = i4;
      s = (s + 4) & 7;
    }                       /* end of border following loop */
  }

  cvEndWriteSeq(contour);

  if (_method != CC_CHAIN_CODE) {
    *prect = cvBoundingRect(contour->data, contour->used, 0, 1);
  }

  assert(  //writer.seq.used == 0 && writer.seq->first == 0 ||
      //writer.seq.used > writer.seq->first->count ||
      //(writer.seq->first->prev == writer.seq->first &&
      // writer.seq->first->next == writer.seq->first)
      1);

  return CC_OK;
}


/*
   trace contour until certain point is met.
   returns 1 if met, 0 else.
*/
static int
icvTraceContour(char* ptr, int step, char* stop_ptr, int is_hole)
{
  int deltas[ 16 ];
  char* i0 = ptr, *i1, *i3, *i4;
  int s, s_end;

  /* initialize local state */
  CC_INIT_3X3_DELTAS(deltas, step, 1);
  memcpy(deltas + 8, deltas, 8 * sizeof(deltas[ 0 ]));

  assert((*i0 & -2) != 0);

  s_end = s = is_hole ? 0 : 4;

  do {
    s = (s - 1) & 7;
    i1 = i0 + deltas[ s ];

    if (*i1 != 0) {
      break;
    }
  }
  while (s != s_end);

  i3 = i0;

  /* check single pixel domain */
  if (s != s_end) {
    /* follow border */
    for (;;) {
      s_end = s;

      for (;;) {
        i4 = i3 + deltas[ ++s ];

        if (*i4 != 0) {
          break;
        }
      }

      if (i3 == stop_ptr || (i4 == i0 && i3 == i1)) {
        break;
      }

      i3 = i4;
      s = (s + 4) & 7;
    }                       /* end of border following loop */
  }

  return i3 == stop_ptr;
}




static CvStatus
icvFetchContourEx(char* ptr,
    int step,
    IPOINT pt,
    MINIVEC_CvPoint_t* contour,
    int is_hole,
    int _method,
    int nbd,
    CvRect* _rect, CvContourScanner ss)
{
  int deltas[ 16 ];
  //CvSeqWriter writer;
  char* i0 = ptr, *i1, *i3, *i4;
  CvRect rect;
  int prev_s = -1, s, s_end;
  int method = _method - 1;
  MINIVEC_char_t* contour_code = (MINIVEC_char_t*) contour;

  assert((unsigned) _method <= CC_CHAIN_APPROX_SIMPLE);
  assert(1 < nbd && nbd < 128);

  /* initialize local state */
  CC_INIT_3X3_DELTAS(deltas, step, 1);
  memcpy(deltas + 8, deltas, 8 * sizeof(deltas[ 0 ]));

  /* initialize writer */
  cvStartAppendToSeq(contour, &writer);

  if (method < 0) {
    ((CvChain*) contour) ->origin = pt;
  }

  rect.x = rect.width = pt.x;
  rect.y = rect.height = pt.y;

  s_end = s = CC_IS_SEQ_HOLE(contour) ? 0 : 4;

  ASSERT(ptr == ss->img0 + pt.y * ss->img_step + pt.x);

  do {
    s = (s - 1) & 7;
    i1 = i0 + deltas[ s ];

    if (*i1 != 0) {
      break;
    }
  }
  while (s != s_end);

  if (s == s_end) {             /* single pixel domain */
    *i0 = (char)(nbd | 0x80);

    if (method >= 0) {
      CC_WRITE_SEQ_ELEM(pt, contour);
    }
  }
  else {
    i3 = i0;

    prev_s = s ^ 4;

    /* follow border */
    for (;;) {
      s_end = s;

      if (i3 != ss->img0 + pt.y * ss->img_step + pt.x) {
        int asdf = 0;
      }

      for (;;) {
        i4 = i3 + deltas[ ++s ];

        //printf("%3d, ", (int)*i4);
        if (*i4 != 0) {
          break;
        }
      }

      if (*i4 != 1) {
        int asdf = 0;
      }

#ifdef _CVCONTOURS_DEBUG

      if (s > 15) {
        s = s_end;

        for (;;) {
          i4 = i3 + deltas[ ++s ];
          printf("%3d, ", (int)*i4);

          if (*i4 != 0) {
            break;
          }
        }
      }

#endif
      s &= 7;
      ASSERT(i4 != i3);
      ASSERT((i4 - ss->img0) % ss->img_step == pt.x + icvCodeDeltas[ s ].x);
      ASSERT((i4 - ss->img0) / ss->img_step == pt.y + icvCodeDeltas[ s ].y);

      /* check "right" bound */
      if ((unsigned)(s - 1) < (unsigned) s_end) {
        *i3 = (char)(nbd | 0x80);
      }
      else if (*i3 == 1) {
        *i3 = (char) nbd;
      }

      if (method < 0) {
        char _s = (char) s;
        CC_WRITE_SEQ_ELEM(_s, contour_code);
      }
      else if (s != prev_s || method == 0) {
#ifdef _CVCONTOURS_DEBUG
        printf("%d, %d\r\n", pt.x, pt.y);

        if (contour->used > 1 &&
            (ABS(pt.x - contour->data[contour->used - 1].x) > 1 ||
                ABS(pt.y - contour->data[contour->used - 1].y) > 1)
           ) {
          int asdf = 0;
          WindowSetPix("asdf", pt, CC_RGB(0, 255, 0));
          cvWaitKey(150);
        }
        else {
          WindowSetPix("asdf", pt, CC_RGB(255, 0, 0));
          cvWaitKey(50);
        }

#endif // _CVCONTOURS_DEBUG
        CC_WRITE_SEQ_ELEM(pt, contour);
      }

      if (s != prev_s) {
        /* update bounds */
        if (pt.x < rect.x) {
          rect.x = pt.x;
        }
        else if (pt.x > rect.width) {
          rect.width = pt.x;
        }

        if (pt.y < rect.y) {
          rect.y = pt.y;
        }
        else if (pt.y > rect.height) {
          rect.height = pt.y;
        }
      }

      prev_s = s;
      pt.x += icvCodeDeltas[ s ].x;
      pt.y += icvCodeDeltas[ s ].y;

      if (i4 == i0 && i3 == i1) {
        break;
      }

      i3 = i4;
      s = (s + 4) & 7;
    }                       /* end of border following loop */
  }

  rect.width -= rect.x - 1;
  rect.height -= rect.y - 1;

  cvEndWriteSeq(contour);

  if (_method != CC_CHAIN_CODE) {
    ((CvContour*) contour) ->rect = rect;
  }

  //assert( writer.seq.used == 0 && writer.seq->first == 0 ||
  //        writer.seq.used > writer.seq->first->count ||
  //        (writer.seq->first->prev == writer.seq->first &&
  //         writer.seq->first->next == writer.seq->first) );

  if (_rect) {
    * _rect = rect;
  }

  return CC_OK;
}
CvStatus
icvApproximateChainTC89(const MINIVEC_char_t* chain,
    IPOINT origin,
    MINIVEC_CvPoint_t* pts,
    int method);

int
cvFindNextContour(CvContourScanner scanner)
{
  char* img0;
  char* img;
  int step;
  int width, height;
  int x, y;
  int prev, ret = 0;
  IPOINT lnbd;
  int nbd;
  int mode;
  CvStatus result = (CvStatus) 1;
  //MINIVEC_CvPoint_t* seq = &( scanner->contours->pts );

  CC_FUNCNAME("cvFindNextContour");

  __BEGIN__;

  if (!scanner) {
    CC_ERROR(CC_StsNullPtr, "");
  }

  icvEndProcessContour(scanner);

  /* initialize local state */
  img0 = scanner->img0;
  img = scanner->img;
  step = scanner->img_step;
  x = scanner->pt.x;
  y = scanner->pt.y;
  width = scanner->img_size.width;
  height = scanner->img_size.height;
  mode = scanner->mode;
  lnbd = scanner->lnbd;
  nbd = scanner->nbd;

  prev = img[ x - 1 ];

  for (; y < height; y++, img += step) {
    for (; x < width; x++) {
      _CvContourInfo _l_cinfo = {0}, _par_info = {0};
      _CvContourInfo* par_info = &_par_info;
      _CvContourInfo* l_cinfo = &_l_cinfo;
      //CvSeq *seq = 0;
      int is_hole = 0;
      IPOINT origin;

      int p = img[ x ];

      if (p == prev) {
        continue;
      }

      if (!(prev == 0 && p == 1)) {
        /* if not external contour */
        /* check hole */
        if (p != 0 || prev < 1) {
          goto resume_scan;
        }

        if (prev & -2) {
          lnbd.x = x - 1;
        }

        is_hole = 1;
      }

      if (mode == 0 && (is_hole || img0[ lnbd.y * step + lnbd.x ] > 0)) {
        goto resume_scan;
      }

      origin.y = y;
      origin.x = x - is_hole;

      /* find contour parent */
      if (mode <= 1 || (!is_hole && mode == 2) || lnbd.x <= 0) {
        par_info = &(scanner->frame_info);
      }
      else {
        int lval = img0[ lnbd.y * step + lnbd.x ] & 0x7f;
        _CvContourInfo* cur = scanner->cinfo_table[ lval - 2 ];

        assert(lval >= 2);

        /* find the first bounding contour */
        while (cur) {
          if ((unsigned)(lnbd.x - cur->rect.x) < (unsigned) cur->rect.width &&
              (unsigned)(lnbd.y - cur->rect.y) < (unsigned) cur->rect.height) {
            if (par_info) {
              if (icvTraceContour(scanner->img0 + par_info->origin.y * step + par_info->origin.x,
                  step, img + lnbd.x, par_info->is_hole) > 0) {
                break;
              }
            }

            par_info = cur;
          }

          cur = cur->next;
        }

        assert(par_info != 0);

        /* if current contour is a hole and previous contour is a hole or
           current contour is external and previous contour is external then
           the parent of the contour is the parent of the previous contour else
           the parent is the previous contour itself. */
        if (par_info->is_hole == is_hole) {
          par_info = par_info->parent;

          /* every contour must have a parent
             (at least, the frame of the image) */
          if (!par_info) {
            par_info = &(scanner->frame_info);
          }
        }

        /* hole flag of the parent must differ from the flag of the contour */
        assert(par_info->is_hole != is_hole);

        /*if ( par_info->contour == 0 )          removed contour */
        if (par_info->ptlen == 0) {         /* removed contour */
          goto resume_scan;
        }
      }

      memset(&l_cinfo->rect, 0, sizeof(l_cinfo->rect));
      lnbd.x = x - is_hole;

      //cvSaveMemStoragePos( scanner->storage2, &(scanner->backup_pos) );

      //seq = cvCreateSeq( scanner->seq_type1, scanner->header_size1,
      //                   scanner->elem_size1, scanner->storage1 );
      //seq->flags |= is_hole ? CC_SEQ_FLAG_HOLE : 0;

      /* initialize header */
      if (mode <= 1) {
        int ptpos = scanner->contours->pts.used;
        l_cinfo = &(scanner->cinfo_temp);
        result = icvFetchContour(img + x - is_hole, step,
            iPOINT(origin.x + scanner->offset.x,
                origin.y + scanner->offset.y),
            &(scanner->contours->pts), is_hole,
            scanner->approx_method1, &(l_cinfo->rect));
        l_cinfo->ptlen = scanner->contours->pts.used - ptpos;

        if (result < 0) {
          goto exit_func;
        }
      }
      else {
        union {
          _CvContourInfo* ci;
          CvSetElem* se;
        } v;
        int ptpos = scanner->contours->pts.used;
        v.ci = l_cinfo;
        //cvSetAdd( scanner->cinfo_set, 0, &v.se );
        l_cinfo = v.ci;

        result = icvFetchContourEx(img + x - is_hole, step,
            iPOINT(origin.x + scanner->offset.x,
                origin.y + scanner->offset.y),
            &(scanner->contours->pts), is_hole,
            scanner->approx_method1,
            nbd, &(l_cinfo->rect), scanner);

        if (result < 0) {
          goto exit_func;
        }

        l_cinfo->rect.x -= scanner->offset.x;
        l_cinfo->rect.y -= scanner->offset.y;
        l_cinfo->ptlen = scanner->contours->pts.used - ptpos;

        l_cinfo->next = scanner->cinfo_table[ nbd - 2 ];
        scanner->cinfo_table[ nbd - 2 ] = l_cinfo;

        /* change nbd */
        nbd = (nbd + 1) & 127;
        nbd += nbd == 0 ? 3 : 0;
      }

      l_cinfo->is_hole = is_hole;
      //l_cinfo->contour = ( void* ) seq;
      l_cinfo->origin = origin;
      l_cinfo->parent = par_info;

      if (scanner->approx_method1 != scanner->approx_method2) {
        //result = icvApproximateChainTC89( (void *) seq, origin,
        //  &(l_cinfo->contour), scanner->approx_method2 );
        if (result < 0) {
          goto exit_func;
        }

        //cvClearMemStorage( scanner->storage1 );
      }

      //l_cinfo->contour->v_prev = l_cinfo->parent->contour;

      //if( par_info->contour == 0 )
      if (l_cinfo->rect.width == 0) {
        //l_cinfo->contour = 0;
        //if( scanner->storage1 == scanner->storage2 ) {cvRestoreMemStoragePos( scanner->storage1, &(scanner->backup_pos) );
        //} else {cvClearMemStorage( scanner->storage1 ); }
        p = img[ x ];
        goto resume_scan;
      }

      MINIVEC_ADD(&(scanner->contours->cinfo), *l_cinfo);
      //cvSaveMemStoragePos( scanner->storage2, &(scanner->backup_pos2) );
      scanner->l_cinfo = l_cinfo;
      scanner->pt.x = x + 1;
      scanner->pt.y = y;
      scanner->lnbd = lnbd;
      scanner->img = (char*) img;
      scanner->nbd = nbd;
      //contour = l_cinfo->contour;

      result = CC_OK;
      goto exit_func;
resume_scan:
      prev = p;

      /* update lnbd */
      if (prev & -2) {
        lnbd.x = x;
      }
    }                       /* end of loop on x */

    lnbd.x = 0;
    lnbd.y = y + 1;
    x = 1;
    prev = 0;
  }                           /* end of loop on y */

exit_func:

  ret = 1;

  if (result != 0) {
    ret = 0;
  }

  if (result < 0) {
    CC_ERROR_FROM_STATUS(result);
  }

  __END__;

  return ret;
}


/*
   The function add to tree the last retrieved/substituted contour,
   releases temp_storage, restores state of dst_storage (if needed), and
   returns pointer to root of the contour tree */
CC_IMPL int
cvEndFindContours(_CvContourScanner* scanner)
{

  CC_FUNCNAME("cvFindNextContour");

  __BEGIN__;

  if (scanner) {
    icvEndProcessContour(scanner);
    //SAFEFREE(scanner->img0);
    //if( scanner->storage1 != scanner->storage2 )
    //cvReleaseMemStorage( &(scanner->storage1) );
    //if( scanner->cinfo_storage )
    //cvReleaseMemStorage( &(scanner->cinfo_storage) );
    //first = scanner->frame.v_next;
  }

  __END__;

  return 0;
}


#define ICC_SINGLE                  0
#define ICC_CONNECTING_ABOVE        1
#define ICC_CONNECTING_BELOW        -1
#define ICC_IS_COMPONENT_POINT(val) ((val) != 0)

//#define CC_GET_WRITTEN_ELEM( writer ) ((writer).ptr - (writer).seq->elem_size)

typedef struct CvLinkedRunPoint {
  struct CvLinkedRunPoint* link;
  struct CvLinkedRunPoint* next;
  IPOINT pt;
}
CvLinkedRunPoint;

static int
icvFindContoursInInterval(int height, int width, const unsigned char* img, int img_step, CONTOURS* Contours)
{
  int count = 0;
  //CvMemStorage* storage00 = 0;
  //CvMemStorage* storage01 = 0;
  //CvSeq* first = 0;
  //CvSeq* prev = 0;

  int i, j, k, n;

  const uchar* src_data = img;
  //int  img_step = 0;
  CvSize img_size = cvSize(width, height);

  int connect_flag;
  int lower_total;
  int upper_total;
  int all_total;

  typedef MINIVEC(CvLinkedRunPoint) MINIVEC_CvLinkedRunPoint_t;
  typedef MINIVEC(CvLinkedRunPoint*) MINIVEC_CvLinkedRunPointptr_t;

  MINIVEC_CvLinkedRunPoint_t runs0;
  MINIVEC_CvLinkedRunPoint_t* runs;
  MINIVEC_CvLinkedRunPointptr_t runs_ext0;
  MINIVEC_CvLinkedRunPointptr_t runs_int0;
  MINIVEC_CvLinkedRunPointptr_t* runs_ext;
  MINIVEC_CvLinkedRunPointptr_t* runs_int;
  CvLinkedRunPoint tmp;
  CvLinkedRunPoint* tmp_prev;
  CvLinkedRunPoint* upper_line = 0;
  CvLinkedRunPoint* lower_line = 0;
  CvLinkedRunPoint* last_elem;

  CvLinkedRunPoint* upper_run = 0;
  CvLinkedRunPoint* lower_run = 0;
  CvLinkedRunPoint* prev_point = 0;

  //CvSeqWriter  writer_ext;
  //CvSeqWriter  writer_int;
  //CvSeqWriter  writer;
  //CvSeqReader  reader;

  //CvLinkedRunPoint** external_contours;
  //CvLinkedRunPoint** internal_contours;

  CC_FUNCNAME("icvFindContoursInInterval");

  __BEGIN__;

  if (!Contours) {
    CC_ERROR(CC_StsNullPtr, "NULL double CvSeq pointer");
  }

  //if( contourHeaderSize < (int)sizeof(CvContour))
  //    CC_ERROR( CC_StsBadSize, "Contour header size must be >= sizeof(CvContour)" );

  //CC_CALL( storage00 = cvCreateChildMemStorage(storage));
  //CC_CALL( storage01 = cvCreateChildMemStorage(storage));

  {
    //CvMat stub, *mat;
    //CC_CALL( mat = cvGetMat( src, &stub ));
    //if( !CC_IS_MASK_ARR(mat))
    //    CC_ERROR( CC_StsBadArg, "Input array must be 8uC1 or 8sC1" );
    //src_data = mat->data.ptr;
    //img_step = mat_step;
    //img_size = cvGetMatSize( mat );
  }

  Contours->flags = CC_SEQ_ELTYPE_POINT | CC_SEQ_POLYLINE | CC_SEQ_FLAG_CLOSED;
  // Create temporary sequences
  //runs = cvCreateSeq(0, sizeof(CvSeq), sizeof(CvLinkedRunPoint), storage00 );
  runs = &runs0;
  runs_ext = &runs_ext0;
  runs_int = &runs_int0;
  //MINIVEC_INIT(_V, BUFMALLOC((_N)*sizeof(*((_V)->data))), 0, _N);
#if 0
  MINIVEC_ALLOC(runs, img_size.height * img_size.width);
  MINIVEC_ALLOC(runs_ext, img_size.height * img_size.width);
  MINIVEC_ALLOC(runs_int, img_size.height * img_size.width);
#else
  MINIVEC_INIT(runs, 0, 0, img_size.height * img_size.width);
  MINIVEC_INIT(runs_ext, 0, 0, img_size.height * img_size.width);
  MINIVEC_INIT(runs_int, 0, 0, img_size.height * img_size.width);
  BUFUSEBEGIN(Contours->buf, Contours->buflen);
  BUFMALLOC2(runs->data, runs->size);
  BUFMALLOC2(runs_ext->data, runs_ext->size);
  BUFMALLOC2(runs_int->data, runs_int->size);
  BUFUSEEND();
#endif
  cvStartAppendToSeq(runs, &writer);

  cvStartWriteSeq(runs_ext, sizeof(CvSeq), sizeof(CvLinkedRunPoint*), storage01, &writer_ext);
  cvStartWriteSeq(runs_int, sizeof(CvSeq), sizeof(CvLinkedRunPoint*), storage01, &writer_int);

  tmp_prev = &(tmp);
  tmp_prev->next = 0;
  tmp_prev->link = 0;

  // First line. None of runs is binded
  tmp.pt.y = 0;
  i = 0;
  CC_WRITE_SEQ_ELEM(tmp, runs);
  upper_line = (CvLinkedRunPoint*) CC_GET_WRITTEN_ELEM(runs);

  tmp_prev = upper_line;

  for (j = 0; j < img_size.width;) {
    for (; j < img_size.width && !ICC_IS_COMPONENT_POINT(src_data[ j ]); j++)
      ;

    if (j == img_size.width) {
      break;
    }

    tmp.pt.x = j;
    CC_WRITE_SEQ_ELEM(tmp, runs);
    tmp_prev->next = (CvLinkedRunPoint*) CC_GET_WRITTEN_ELEM(runs);
    tmp_prev = tmp_prev->next;

    for (; j < img_size.width && ICC_IS_COMPONENT_POINT(src_data[ j ]); j++)
      ;

    tmp.pt.x = j - 1;
    CC_WRITE_SEQ_ELEM(tmp, runs);
    tmp_prev->next = (CvLinkedRunPoint*) CC_GET_WRITTEN_ELEM(runs);
    tmp_prev->link = tmp_prev->next;
    // First point of contour
    CC_WRITE_SEQ_ELEM(tmp_prev, runs_ext);
    tmp_prev = tmp_prev->next;
  }

#define cvFlushSeqWriter(_X)
  cvFlushSeqWriter(&writer);
  upper_line = upper_line->next;
  upper_total = runs->used - 1;
  last_elem = tmp_prev;
  tmp_prev->next = 0;

  for (i = 1; i < img_size.height; i++) {
    //------// Find runs in next line
    src_data += img_step;
    tmp.pt.y = i;
    all_total = runs->used;

    for (j = 0; j < img_size.width;) {
      for (; j < img_size.width && !ICC_IS_COMPONENT_POINT(src_data[ j ]); j++)
        ;

      if (j == img_size.width) {
        break;
      }

      tmp.pt.x = j;
      CC_WRITE_SEQ_ELEM(tmp, runs);
      tmp_prev->next = (CvLinkedRunPoint*) CC_GET_WRITTEN_ELEM(runs);
      tmp_prev = tmp_prev->next;

      for (; j < img_size.width && ICC_IS_COMPONENT_POINT(src_data[ j ]); j++)
        ;

      tmp.pt.x = j - 1;
      CC_WRITE_SEQ_ELEM(tmp, runs);
      tmp_prev = tmp_prev->next = (CvLinkedRunPoint*) CC_GET_WRITTEN_ELEM(runs);
    } //j

    cvFlushSeqWriter(&writer);
    lower_line = last_elem->next;
    lower_total = runs->used - all_total;
    last_elem = tmp_prev;
    tmp_prev->next = 0;
    //------//
    //------// Find links between runs of lower_line and upper_line
    upper_run = upper_line;
    lower_run = lower_line;
    connect_flag = ICC_SINGLE;

    for (k = 0, n = 0; k < upper_total / 2 && n < lower_total / 2;) {
      switch (connect_flag) {
      case ICC_SINGLE:
        if (upper_run->next->pt.x < lower_run->next->pt.x) {
          if (upper_run->next->pt.x >= lower_run->pt.x - 1) {
            lower_run->link = upper_run;
            connect_flag = ICC_CONNECTING_ABOVE;
            prev_point = upper_run->next;
          }
          else {
            upper_run->next->link = upper_run;
          }

          k++;
          upper_run = upper_run->next->next;
        }
        else {
          if (upper_run->pt.x <= lower_run->next->pt.x + 1) {
            lower_run->link = upper_run;
            connect_flag = ICC_CONNECTING_BELOW;
            prev_point = lower_run->next;
          }
          else {
            lower_run->link = lower_run->next;
            // First point of contour
            CC_WRITE_SEQ_ELEM(lower_run, runs_ext);
          }

          n++;
          lower_run = lower_run->next->next;
        }

        break;

      case ICC_CONNECTING_ABOVE:
        if (upper_run->pt.x > lower_run->next->pt.x + 1) {
          prev_point->link = lower_run->next;
          connect_flag = ICC_SINGLE;
          n++;
          lower_run = lower_run->next->next;
        }
        else {
          prev_point->link = upper_run;

          if (upper_run->next->pt.x < lower_run->next->pt.x) {
            k++;
            prev_point = upper_run->next;
            upper_run = upper_run->next->next;
          }
          else {
            connect_flag = ICC_CONNECTING_BELOW;
            prev_point = lower_run->next;
            n++;
            lower_run = lower_run->next->next;
          }
        }

        break;

      case ICC_CONNECTING_BELOW:
        if (lower_run->pt.x > upper_run->next->pt.x + 1) {
          upper_run->next->link = prev_point;
          connect_flag = ICC_SINGLE;
          k++;
          upper_run = upper_run->next->next;
        }
        else {
          // First point of contour
          CC_WRITE_SEQ_ELEM(lower_run, runs_int);

          lower_run->link = prev_point;

          if (lower_run->next->pt.x < upper_run->next->pt.x) {
            n++;
            prev_point = lower_run->next;
            lower_run = lower_run->next->next;
          }
          else {
            connect_flag = ICC_CONNECTING_ABOVE;
            k++;
            prev_point = upper_run->next;
            upper_run = upper_run->next->next;
          }
        }

        break;
      }
    } // k, n

    for (; n < lower_total / 2; n++) {
      if (connect_flag != ICC_SINGLE) {
        prev_point->link = lower_run->next;
        connect_flag = ICC_SINGLE;
        lower_run = lower_run->next->next;
        continue;
      }

      lower_run->link = lower_run->next;

      //First point of contour
      CC_WRITE_SEQ_ELEM(lower_run, runs_ext);

      lower_run = lower_run->next->next;
    }

    for (; k < upper_total / 2; k++) {
      if (connect_flag != ICC_SINGLE) {
        upper_run->next->link = prev_point;
        connect_flag = ICC_SINGLE;
        upper_run = upper_run->next->next;
        continue;
      }

      upper_run->next->link = upper_run;
      upper_run = upper_run->next->next;
    }

    upper_line = lower_line;
    upper_total = lower_total;
  } //i

  upper_run = upper_line;

  //the last line of image
  for (k = 0; k < upper_total / 2; k++) {
    upper_run->next->link = upper_run;
    upper_run = upper_run->next->next;
  }

  //------//
  //------//Find end read contours
  //external_contours = ( runs_ext );
  //internal_contours = ( runs_int );

  for (k = 0; k < 2; k++) {
    MINIVEC_CvLinkedRunPointptr_t* contours = k == 0 ? runs_ext : runs_int;
    int contours_index = 0;
#undef cvStartReadSeq
#define cvStartReadSeq cvStartAppendToSeq

    cvStartReadSeq(contours, &reader);

    for (j = 0; j < (int) contours->used; j++, count++) {
      CvLinkedRunPoint* p_temp;
      CvLinkedRunPoint* p00;
      CvLinkedRunPoint* p01;
      _CvContourInfo cinfo = {0};
      MINIVEC_CvPoint_t* contour = &(Contours->pts);

      (p00 = contours->data[ contours_index++ ]);
      p01 = p00;

      if (!p00->link) {
        continue;
      }

      cvStartWriteSeq(contour,   //CC_SEQ_ELTYPE_POINT | CC_SEQ_POLYLINE | CC_SEQ_FLAG_CLOSED,
          contourHeaderSize, sizeof(IPOINT), storage, &writer);

      do {
        cinfo.ptlen++;
        CC_WRITE_SEQ_ELEM(p00->pt, contour);
        p_temp = p00;
        p00 = p00->link;
        p_temp->link = 0;
      }
      while (p00 != p01);

      //contour = cvEndWriteSeq( &writer );
      //cvBoundingRect( contour, 1 );
      cinfo.rect = cvBoundingRect(contour, contour->used, 0, 1);
      MINIVEC_ADD(&Contours->cinfo, cinfo);

      if (k != 0) {
        // ÊÇ¸ö¶´ is_hole=1                contour->flags |= CC_SEQ_FLAG_HOLE;
      }

      //if( !first ) { prev = first = contour; } else { contour->h_prev = prev; prev = prev->h_next = contour; }
    }
  }

  __END__;

#if 0
  MINIVEC_FREE(runs);
  MINIVEC_FREE(runs_ext);
  MINIVEC_FREE(runs_int);
#endif
  //if( !first )        count = -1;

  //if( result )        *result = first;

  //cvReleaseMemStorage(&storage00);
  //cvReleaseMemStorage(&storage01);

  return count;
}

/*F///////////////////////////////////////////////////////////////////////////////////////
//    Name: cvFindContours
//    Purpose:
//      Finds all the contours on the bi-level image.
//    Context:
//    Parameters:
//      img  - source image.
//             Non-zero pixels are considered as 1-pixels
//             and zero pixels as 0-pixels.
//      step - full width of source image in bytes.
//      size - width and height of the image in pixels
//      storage - pointer to storage where will the output contours be placed.
//      header_size - header size of resulting contours
//      mode - mode of contour retrieval.
//      method - method of approximation that is applied to contours
//      first_contour - pointer to first contour pointer
//    Returns:
//      CC_OK or error code
//    Notes:
//F*/
CC_IMPL int
cvFindContours(CONTOURS* Contours)
{
  //CvContourScanner scanner = 0;
  int ret = 0;
  int count = -1;

  if (!Contours) {
    CC_ERROR(CC_StsNullPtr, "NULL double CvSeq pointer");
  }

  if (Contours->method == CC_LINK_RUNS) {
    if (Contours->offset.x != 0 || Contours->offset.y != 0)
      CC_ERROR(CC_StsOutOfRange,
          "Nonzero offset is not supported in CC_LINK_RUNS yet");

    count = icvFindContoursInInterval(Contours->height, Contours->width, Contours->img, Contours->img_step, Contours);
  }
  else {
    _CvContourScanner scanner = {0};
    CC_CALL(cvStartFindContours(&scanner, Contours->height, Contours->width, Contours->img, Contours->img_step,
        Contours->mode, Contours->method, Contours->offset, Contours));

    do {
      count++;
      //cvNamedWindow("asdf", 0);
      //cvShowImage("asdf", Contours->height, Contours->width, Contours->img, Contours->width, 1);
      //cvWaitKey(-1);
      ret = cvFindNextContour(&scanner);
    }
    while (ret != 0);

    cvEndFindContours(&scanner);
  }

  return count;
}

#undef cvStartReadSeq
/* End of file. */
