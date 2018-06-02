
static const unsigned char lbpmap[ 6 ][ 256 ] = {
                                   {
#include "./txt/mapping_3.txt"

                                   },
                                   {
#include "./txt/mapping_9.txt"
                                   },
                                   {
#include "./txt/mapping_15.txt"
                                   },
                                   {
#include "./txt/mapping_21.txt"
                                   },
                                   {
#include "./txt/mapping_27.txt"
                                   },
                                   {
#include "./txt/mapping_33.txt"
                                   },
                                 };
//static const unsigned char* lbpmap59bin = lbpmap[0];

static const unsigned char lbp_block_100x100_Feature_List_3[] = {
#include "./txt/Feature_List_3.txt"
                                    };

static const unsigned char lbp_block_100x100_Feature_List_9[] = {
#include "./txt/Feature_List_9.txt"
                                    };

static const unsigned char lbp_block_100x100_Feature_List_15[] = {
#include "./txt/Feature_List_15.txt"
                                     };

static const unsigned char lbp_block_100x100_Feature_List_21[] = {
#include "./txt/Feature_List_21.txt"
                                     };

static const unsigned char lbp_block_100x100_Feature_List_27[] = {
#include "./txt/Feature_List_27.txt"
                                     };

static const unsigned char lbp_block_100x100_Feature_List_33[] = {
#include "./txt/Feature_List_33.txt"
                                     };

static const int lbp_Feature_idyw_chi_2443[] = {
#include "./txt/exclude1/idyw6.txt"
      //#include "./txt/exclude1/idyw_abs6.txt"
      //#include "./txt/exclude1/idyw6_500.txt"
      //#include "./txt/exclude1/idyw_abs6_2499.txt"
    };

static const int lbp_Feature_idyw_abs_2165[] = {
      //#include "./txt/exclude1/idyw6.txt"
      //#include "./txt/exclude1/idyw_abs6.txt"
      //#include "./txt/exclude1/idyw6_500.txt"
#include "./txt/exclude1/idyw_abs6_2165.txt"
    };

static const int lbp_Feature_idyw_abs_3[] = {
  //#include "./txt/exclude1/idyw_abs_1670.txt"
#if FEAT_SIZE3==1332
#include "./txt/exclude1/idyw_abs_1332.txt"
#endif
#if FEAT_SIZE3==2093
#include "./txt/exclude1/idyw_abs_2093.txt"
#endif
#if FEAT_SIZE3==2566
#include "./txt/exclude1/idyw_abs_2566.txt"
#endif
};

static const int lbp_Feature_idyw_sorted[] = {
#include "./txt/exclude1/idyw6_sorted.txt"
                                       };

//static double lbp_Feature_weightyw[] = {
//#include "./txt/exclude1\weightyw6.txt"
//                                       };

typedef struct lbp_Feature_List_T {
  const unsigned char* ptr;
  int len;
  int bin;
}
lbp_Feature_List_T;

static lbp_Feature_List_T lbp_block_100x100_Feature_List[] =//
  {
    { lbp_block_100x100_Feature_List_3, sizeof( lbp_block_100x100_Feature_List_3 ) / ( sizeof( lbp_block_100x100_Feature_List_3[ 0 ] ) * 4 ), 59 },
    { lbp_block_100x100_Feature_List_9, sizeof( lbp_block_100x100_Feature_List_9 ) / ( sizeof( lbp_block_100x100_Feature_List_9[ 0 ] ) * 4 ), 64 },
    { lbp_block_100x100_Feature_List_15, sizeof( lbp_block_100x100_Feature_List_15 ) / ( sizeof( lbp_block_100x100_Feature_List_15[ 0 ] ) * 4 ), 64 },
    { lbp_block_100x100_Feature_List_21, sizeof( lbp_block_100x100_Feature_List_21 ) / ( sizeof( lbp_block_100x100_Feature_List_21[ 0 ] ) * 4 ), 64 },
    { lbp_block_100x100_Feature_List_27, sizeof( lbp_block_100x100_Feature_List_27 ) / ( sizeof( lbp_block_100x100_Feature_List_27[ 0 ] ) * 4 ), 64 },
    { lbp_block_100x100_Feature_List_33, sizeof( lbp_block_100x100_Feature_List_33 ) / ( sizeof( lbp_block_100x100_Feature_List_33[ 0 ] ) * 4 ), 64 }
  };

static unsigned char lbp_block_50x100_Feature_List_3[] = {
#include "./txt/block_50x100_Feature_List_3.txt"
                                    };

static unsigned char lbp_block_50x100_Feature_List_9[] = {
#include "./txt/block_50x100_Feature_List_9.txt"
                                    };

static unsigned char lbp_block_50x100_Feature_List_15[] = {
#include "./txt/block_50x100_Feature_List_15.txt"
                                     };

static unsigned char lbp_block_50x100_Feature_List_21[] = {
#include "./txt/block_50x100_Feature_List_21.txt"
                                     };

static unsigned char lbp_block_50x100_Feature_List_27[] = {
#include "./txt/block_50x100_Feature_List_27.txt"
                                     };

static unsigned char lbp_block_50x100_Feature_List_33[] = {
#include "./txt/block_50x100_Feature_List_33.txt"
                                     };

static lbp_Feature_List_T lbp_block_50x100_Feature_List[] =//
{
  { lbp_block_50x100_Feature_List_3, sizeof( lbp_block_50x100_Feature_List_3 ) / ( sizeof( lbp_block_50x100_Feature_List_3[ 0 ] ) * 4 ), 59 },
  { lbp_block_50x100_Feature_List_9, sizeof( lbp_block_50x100_Feature_List_9 ) / ( sizeof( lbp_block_50x100_Feature_List_9[ 0 ] ) * 4 ), 64 },
  { lbp_block_50x100_Feature_List_15, sizeof( lbp_block_50x100_Feature_List_15 ) / ( sizeof( lbp_block_50x100_Feature_List_15[ 0 ] ) * 4 ), 64 },
  { lbp_block_50x100_Feature_List_21, sizeof( lbp_block_50x100_Feature_List_21 ) / ( sizeof( lbp_block_50x100_Feature_List_21[ 0 ] ) * 4 ), 64 },
  { lbp_block_50x100_Feature_List_27, sizeof( lbp_block_50x100_Feature_List_27 ) / ( sizeof( lbp_block_50x100_Feature_List_27[ 0 ] ) * 4 ), 64 },
  { lbp_block_50x100_Feature_List_33, sizeof( lbp_block_50x100_Feature_List_33 ) / ( sizeof( lbp_block_50x100_Feature_List_33[ 0 ] ) * 4 ), 64 }
};

#define FN (sizeof(lbp_block_100x100_Feature_List)/sizeof(lbp_block_100x100_Feature_List[0]))
#define MINFN  (0)

const int* get_lbp_Feature_idyw(int select_type, int* pfeaturelen) {
  const int* lbp_Feature_idyw = NULL;
  int featurelen;
  if (1==select_type) {
    lbp_Feature_idyw = lbp_Feature_idyw_chi_2443;
    featurelen = countof(lbp_Feature_idyw_chi_2443)*sizeof(FEAT_T);
  } else if (2==select_type) {
    lbp_Feature_idyw = lbp_Feature_idyw_abs_2165;
    featurelen = countof(lbp_Feature_idyw_abs_2165)*sizeof(FEAT_T);
  } else if (3==select_type) {
    lbp_Feature_idyw = lbp_Feature_idyw_abs_3;
    featurelen = countof(lbp_Feature_idyw_abs_3)*sizeof(FEAT_T);
  }
  if (pfeaturelen) {
    *pfeaturelen = featurelen;
  }
  return lbp_Feature_idyw;
}


lbp_Feature_List_T* get_lbp_Feature_List(int select_type) {
  lbp_Feature_List_T* pp=0;
  if (2==select_type) {
    pp=lbp_block_100x100_Feature_List;
  } else if (3==select_type) {
    pp=lbp_block_50x100_Feature_List;
  }
  return pp;
}

// 总特征数目
int init_size(int select_type) {
  int k, i;
  int n=0;
  lbp_Feature_List_T* pp=get_lbp_Feature_List(select_type);
  if (2==select_type) {
    n=countof(lbp_block_100x100_Feature_List);
    pp=lbp_block_100x100_Feature_List;
  } else if (3==select_type) {
    n=countof(lbp_block_50x100_Feature_List);
    pp=lbp_block_50x100_Feature_List;
  }

  for ( k = 0, i = 0; i < n; ++i ) {
    k += pp[ i ].len * pp[ i ].bin;
  }
  return k;
}
