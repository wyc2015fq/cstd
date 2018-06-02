// ¸¡µã°æ±¾
static int lutpass_0( const FIX_FLOAT* A1, const void* ih0, const void* stage0, int irl,
              const fHaarWork* pW0, double invvar, double* score ) {
  const fHaar* ih2;
  const fHaar* ih = (const fHaar* )ih0;
  const fStage* stage = (const fStage* )stage0;
  const iHaarWork3* pW = (const iHaarWork3* )pW0;
  int ispassed = 0;
  int f = 0, l = irl;
  int pp = 0, qq;
  double vel, val;
  if ( !CAISNESTING(ih, stage) ) {
    for ( ; f != l; ++f ) {
      qq = pp + ( stage + f ) ->len;
      vel = 0.f;
      ++pp;
      for ( ; pp != qq; ++pp ) {
      ih2=ih + pp;
        vel += LUT_VAL( HAAR_FEAT_VAL3( A1, invvar, ( pW + pp ) ), ih2 );
      }
      
      if ( vel < ( stage + f ) ->thd ) {
        break;
      }
    }
  } else {
    qq = pp + ( stage + f ) ->len;
    vel = 0.f;
    //logset("¸¡µã.log");
    //logclear();
#define PRINTVAL() logprintf("id=%3d val=%f vel=%f idf=%f min=%f bin=%f\n", id, ih2->val[ id ], val, ( ( val - ih2->minval ) * ih2->binval ), ih2->minval, ih2->binval );
    //logprintf("invvar=%f\n", invvar);
    for ( ; pp != qq; ++pp ) {
      double rt0, rt1;
      int id;
      ih2=ih + pp;
      rt0 = HAAR_RECT_VAL3( A1, ( pW + pp ) ->ft0 );
      rt1 = HAAR_RECT_VAL3( A1, ( pW + pp ) ->ft1 );
      val = invvar * ( rt0 + rt1 );
      id = ( int ) ( ( val - ih2  ->minval ) * ih2->binval );// & ( LUT_BIN_1 );
      id = MAX(id, 0);
      id = MIN(id, 31);
      vel += ih2 ->val[ id ];
      //PRINTVAL();
    }
    
    //logprintf("lv=%3d len=%3d thd=%f sco=%f\n", f, ( stage + f ) ->len, ( stage + f ) ->thd, vel);
    if ( vel >= ( stage + f ) ->thd ) {
      int id;
      for ( ++f; f != l; ++f ) {
        int iii=0;
        qq = pp + ( stage + f ) ->len;
        ih2=ih + pp;
        //vel = LUT_VAL( vel, ih2 );
        val=vel;
        id = ( int ) ( ( val - ih2->minval ) * ih2->binval );
        id = MIN(id, 31);
        vel = ih2 ->val[ id ];
        //PRINTVAL();
        ++pp;
        for ( ; pp != qq; ++pp ) {
          ++iii;
          ih2=ih + pp;
          val = HAAR_FEAT_VAL3( A1, invvar, ( pW + pp ) );
          id = ( int ) ( ( val - ih2->minval ) * ih2->binval );
          id = MAX(id, 0);
          id = MIN(id, 31);
          vel += ih2 ->val[ id ];
          //vel += LUT_VAL( val, ih2 );
          //PRINTVAL();
        }
        
        //logprintf("lv=%3d len=%3d thd=%f sco=%f\n", f, ( stage + f ) ->len, ( stage + f ) ->thd, vel);
        if ( vel < ( stage + f ) ->thd ) {
          break;
        }
      }
    }
  }
#undef PRINTVAL
  ispassed = l == f;
  *score = vel;
  return ispassed;
}
