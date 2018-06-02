
p00 = pWirk21->ft0.p0;
p01 = pWirk21->ft1.p0;

p10 = p00 + pWirk21->ft0.w;
p20 = p00 + pWirk21->ft0.h;
p11 = p01 + pWirk21->ft1.w;
p21 = p01 + pWirk21->ft1.h;

p30 = p10 + pWirk21->ft0.h;
p31 = p11 + pWirk21->ft1.h;
#if 0
rt0 = ( tmpSamp[ p00 ] - tmpSamp[ p10 ] + tmpSamp[ p30 ] - tmpSamp[ p20 ] ) * pWirk21->ft0.dd;
rt1 = ( tmpSamp[ p01 ] - tmpSamp[ p11 ] + tmpSamp[ p31 ] - tmpSamp[ p21 ] ) * pWirk21->ft1.dd;
haarval = ( invvar2 * ( rt1 + rt0 ) );
#else
//ASSERT(pWirk21->ft0.dd==-8192);printf("%d\n", pWirk21->ft1.dd/8192);
rt0 = ( tmpSamp[ p00 ] - tmpSamp[ p10 ] + tmpSamp[ p30 ] - tmpSamp[ p20 ] );
rt1 = ( tmpSamp[ p01 ] - tmpSamp[ p11 ] + tmpSamp[ p31 ] - tmpSamp[ p21 ] ) * pWirk21->ft1.dd;
haarval = ( invvar2 * ( rt1 - rt0 ) );
#endif
// 移位代替除法，有误差
#if 0
id = ( ( ( ( ( haarval ) ) - ( ih21 ) ->minval ) * ( ih21 ) ->binval ) >> ( _SHIFT_2 ) ) & ( LUT_BIN_1 );
#else
id = ( ( ( haarval - ih21->minval ) * ( ih21->binval )  ) >> ( _SHIFT_2 ) );
id = MAX(id, 0);
id = MIN(id, 31);
#endif
iscore += ( ih21 ) ->val[ id ];
