#define BLURDETECT(src, stp)  (-src[stp[0]]-src[stp[1]]-src[stp[2]]-src[stp[3]]-src[stp[4]]-src[stp[5]]+2*src[stp[6]]+2*src[stp[7]]+2*src[stp[8]]-src[stp[9]]-src[stp[10]]+2*src[stp[11]]+2*src[stp[13]]-src[stp[14]]-src[stp[15]]+2*src[stp[16]]+2*src[stp[17]]+2*src[stp[18]]-src[stp[19]]-src[stp[20]]-src[stp[21]]-src[stp[22]]-src[stp[23]]-src[stp[24]])

