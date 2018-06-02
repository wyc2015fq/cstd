
//321
//4 0
//567
#define IMGPATH  "F:\\code\\opencv教程源代码\\例4-3\\pic1.bmp"

_CvContourInfo ccc[100];
_CvContourInfo ppp[100000];
char buf[130000];


int test_contours()
{
  img_t* src;
  // 第一条命令行参数确定了图像的文件名。
  const TCHAR* picname = 0; //myGetOpenFileName(0);
  CONTOURS con = {0};

  if (NULL == picname) {
    picname = IMGPATH;
  }

  if ((src = imread2(picname, 1)) != 0) {
    img_t* dst;
    //cvThreshold( src->h * src->w, src->data, src->data, 1, 0, 255);
    imshow("Source", src);
    MINIVEC_INIT((&con.cinfo), ccc, 0, 100);
    MINIVEC_INIT((&con.pts), ppp, 0, 100000);
    con.buf = buf, con.buflen = 130000;
    //MINIVEC_ALLOC( ( &con.cinfo ), src->h );
    //MINIVEC_ALLOC( ( &con.pts ), src->h * src->w );
    dst = imread2(picname, 3);
    con.method = CC_LINK_RUNS;
    con.method = CC_CHAIN_CODE;
    con.method = CC_CHAIN_APPROX_NONE;
    con.method = CC_CHAIN_APPROX_SIMPLE;
    con.method = CC_CHAIN_APPROX_TC89_L1;
    con.method = CC_CHAIN_APPROX_TC89_KCOS;
    con.method = CC_CHAIN_APPROX_SIMPLE;
    con.mode = CC_RETR_EXTERNAL;
    con.mode = CC_RETR_CCOMP;
    con.height = src->h, con.width = src->w;
    con.img_step = src->w * src->c;
    con.img = src->data, con.offset = iPOINT(0, 0);
    cvFindContours(&con);

    {
      int i, ptpos;
      //CvBox2D32f box;
      IPOINT* PointArray;
      CvPoint2D32f PointArray2D32f[10000];
      CONTOURS con0 = con;
      con0.cinfo.used = 1;
      con0.cinfo.size = 1;

      /* 用1替代 CC_FILLED  所指示的轮廓外形 */
      for (i = 0, ptpos = 0; i < (int) con.cinfo.used; ++i) {
        CvScalar color = CC_RGB(rand() & 255, rand() & 255, rand() & 255);
        con0.cinfo.data = con.cinfo.data + i;
        con0.pts.data = con.pts.data + ptpos;
        con0.pts.used = con.cinfo.data[ i ].ptlen;
        con0.pts.size = con.cinfo.data[ i ].ptlen;
        ptpos += con.cinfo.data[ i ].ptlen;
        PointArray = con0.pts.data;

        for (i = 0; i < (int)con0.pts.used; i++) {
          PointArray2D32f[i].x = (float)PointArray[i].x;
          PointArray2D32f[i].y = (float)PointArray[i].y;
        }

        // Fits ellipse to current contour.
        //cvFitEllipse(PointArray2D32f, count, &box);

        /* 用1替代 CC_FILLED  所指示的轮廓外形 */
        cvDrawContours(dst->h, dst->w, dst->data, dst->w * dst->c, dst->c,
            &con0, color, color, -1, CC_FILLED, 8, iPOINT(0, 0));
      }

      cvNamedWindow("Components", 1);
      imshow("Components", dst);
      //imshow( "Components", src );
      cvWaitKey(0);
    }
    //MINIVEC_FREE( ( &con.cinfo ) );
    //MINIVEC_FREE( ( &con.pts ) );
    imfree(dst);
    imfree(src);
  }

  return 0;
}