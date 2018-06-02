/*******************************************************************************
* Piotr's Image&Video Toolbox      Version 3.22
* Copyright 2013 Piotr Dollar.  [pdollar-at-caltech.edu]
* Please email me if you find bugs, or have suggestions or questions!
* Licensed under the Simplified BSD License [see external/bsd.txt]
*******************************************************************************/
//#include <mex.h>
#include "wrappers.inl"
#include <math.h>
//#include <typeinfo>
#include "sse.inl"

#define oT float
#define iT float

// Constants for rgb2luv conversion and lookup table for y-> l conversion
//template<class oT> 
oT* rgb2luv_setup( oT z, oT *mr, oT *mg, oT *mb,
  oT *minu, oT *minv, oT *un, oT *vn )
{ int i;
  oT maxi, y, l;
  static oT lTable[1064]; static bool lInit=false;
  // set constants for conversion
  const oT y0=(oT) ((6.0/29)*(6.0/29)*(6.0/29));
  const oT a= (oT) ((29.0/3)*(29.0/3)*(29.0/3));
  *un=(oT) 0.197833; *vn=(oT) 0.468331;
  mr[0]=(oT) 0.430574*z; mr[1]=(oT) 0.222015*z; mr[2]=(oT) 0.020183*z;
  mg[0]=(oT) 0.341550*z; mg[1]=(oT) 0.706655*z; mg[2]=(oT) 0.129553*z;
  mb[0]=(oT) 0.178325*z; mb[1]=(oT) 0.071330*z; mb[2]=(oT) 0.939180*z;
  maxi =(oT) 1.0/270; *minu=-88*maxi; *minv=-134*maxi;
  // build (padded) lookup table for y->l conversion assuming y in [0,1]
  if( lInit ) return lTable; 
  for( i=0; i<1025; i++) {
    y = (oT) (i/1024.0);
    l = y>y0 ? 116*(oT)pow((double)y,1.0/3.0)-16 : y*a;
    lTable[i] = l*maxi;
  }
  for( i=1025; i<1064; i++) lTable[i]=lTable[i-1];
  lInit = true; return lTable;
}

// Convert from rgb to luv
//template<class iT, class oT> 
void rgb2luv( iT *I, oT *J, int n, oT nrm ) {
  oT minu, minv, un, vn, mr[3], mg[3], mb[3];
  oT *lTable = rgb2luv_setup(nrm,mr,mg,mb,&minu,&minv,&un,&vn);
  oT *L=J, *U=L+n, *V=U+n; iT *R=I, *G=R+n, *B=G+n;
  int i;
  for( i=0; i<n; i++ ) {
    oT r, g, b, x, y, z, l;
    r=(oT)*R++; g=(oT)*G++; b=(oT)*B++;
    x = mr[0]*r + mg[0]*g + mb[0]*b;
    y = mr[1]*r + mg[1]*g + mb[1]*b;
    z = mr[2]*r + mg[2]*g + mb[2]*b;
    l = lTable[(int)(y*1024)];
    *(L++) = l; z = 1/(x + 15*y + 3*z + (oT)1e-35);
    *(U++) = l * (13*4*x*z - 13*un) - minu;
    *(V++) = l * (13*9*y*z - 13*vn) - minv;
  }
}

// Convert from rgb to luv using sse
//template<class iT> 
void rgb2luv_sse( iT *I, float *J, int n, float nrm ) {
  enum { k=256 }; float R[k], G[k], B[k];
  int i=0, j, i1, n1; float minu, minv, un, vn, mr[3], mg[3], mb[3];
  float *lTable;
  if( ((size_t)(R)&15||(size_t)(G)&15||(size_t)(B)&15||(size_t)(I)&15||(size_t)(J)&15)
    || n%4>0 ) { rgb2luv(I,J,n,nrm); return; }
  lTable = rgb2luv_setup(nrm,mr,mg,mb,&minu,&minv,&un,&vn);
  while( i<n ) {
    float *R1, *G1, *B1, *J1;
    n1 = i+k; if(n1>n) n1=n; J1=J+i; 
    // convert to floats (and load input into cache)
#if 0
    if( typeid(iT) != typeid(float) ) {
      R1=R; G1=G; B1=B; iT *Ri=I+i, *Gi=Ri+n, *Bi=Gi+n;
      for( i1=0; i1<(n1-i); i1++ ) {
        R1[i1] = (float) *Ri++; G1[i1] = (float) *Gi++; B1[i1] = (float) *Bi++;
      }
    } else 
#endif
    { R1=((float*)I)+i; G1=R1+n; B1=G1+n; }
    // compute RGB -> XYZ
    for( j=0; j<3; j++ ) {
      __m128 _mr, _mg, _mb, *_J=(__m128*) (J1+j*n);
      __m128 *_R=(__m128*) R1, *_G=(__m128*) G1, *_B=(__m128*) B1;
      _mr=SET1f(mr[j]); _mg=SET1f(mg[j]); _mb=SET1f(mb[j]);
      for( i1=i; i1<n1; i1+=4 ) *(_J++) = ADD( ADD(MUL(*(_R++),_mr),
        MUL(*(_G++),_mg)),MUL(*(_B++),_mb));
    }
    { // compute XZY -> LUV (without doing L lookup/normalization)
      __m128 _c15, _c3, _cEps, _c52, _c117, _c1024, _cun, _cvn;
      __m128 *_X, *_Y, *_Z, _x, _y, _z;
      _c15=SET1f(15.0f); _c3=SET1f(3.0f); _cEps=SET1f(1e-35f);
      _c52=SET1f(52.0f); _c117=SET1f(117.0f), _c1024=SET1f(1024.0f);
      _cun=SET1f(13*un); _cvn=SET1f(13*vn);
      _X=(__m128*) J1; _Y=(__m128*) (J1+n); _Z=(__m128*) (J1+2*n);
      for( i1=i; i1<n1; i1+=4 ) {
        _x = *_X; _y=*_Y; _z=*_Z;
        _z = RCP(ADD(_x,ADD(_cEps,ADD(MUL(_c15,_y),MUL(_c3,_z)))));
        *(_X++) = MUL(_c1024,_y);
        *(_Y++) = SUB(MUL(MUL(_c52,_x),_z),_cun);
        *(_Z++) = SUB(MUL(MUL(_c117,_y),_z),_cvn);
      }
    }
    { // perform lookup for L and finalize computation of U and V
      __m128 *_L, *_U, *_V, _l, _cminu, _cminv;
      for( i1=i; i1<n1; i1++ ) J[i1] = lTable[(int)J[i1]];
      _L=(__m128*) J1; _U=(__m128*) (J1+n); _V=(__m128*) (J1+2*n);
      _cminu=SET1f(minu); _cminv=SET1f(minv);
      for( i1=i; i1<n1; i1+=4 ) {
        _l = *(_L++);
        *(_U++) = SUB(MUL(_l,*_U),_cminu);
        *(_V++) = SUB(MUL(_l,*_V),_cminv);
      }
    }
    i = n1;
  }
}

// Convert from rgb to hsv
//template<class iT, class oT> 
void rgb2hsv( iT *I, oT *J, int n, oT nrm ) {
  oT *H=J, *S=H+n, *V=S+n;
  iT *R=I, *G=R+n, *B=G+n;
  int i;
  for(i=0; i<n; i++) {
    const oT r=(oT)*(R++), g=(oT)*(G++), b=(oT)*(B++);
    oT h, s, v, minv, maxv;
    if( r==g && g==b ) {
      *(H++) = 0; *(S++) = 0; *(V++) = r*nrm; continue;
    } else if( r>=g && r>=b ) {
      maxv = r; minv = g<b ? g : b;
      h = (g-b)/(maxv-minv)+6; if(h>=6) h-=6;
    } else if( g>=r && g>=b ) {
      maxv = g; minv = r<b ? r : b;
      h = (b-r)/(maxv-minv)+2;
    } else {
      maxv = b; minv = r<g ? r : g;
      h = (r-g)/(maxv-minv)+4;
    }
    h*=(oT) (1/6.0); s=1-minv/maxv; v=maxv*nrm;
    *(H++) = h; *(S++) = s; *(V++) = v;
  }
}

// Convert from rgb to gray
//template<class iT, class oT> 
void rgb2gray_mex( iT *I, oT *J, int n, oT nrm ) {
  oT *GR=J; iT *R=I, *G=R+n, *B=G+n; int i;
  oT mr=(oT).2989360213*nrm, mg=(oT).5870430745*nrm, mb=(oT).1140209043*nrm;
  for(i=0; i<n; i++) *(GR++)=(oT)*(R++)*mr + (oT)*(G++)*mg + (oT)*(B++)*mb;
}

// Convert from rgb (double) to gray (float)
void rgb2gray_mex_f64( double *I, float *J, int n, float nrm ) {
  float *GR=J; double *R=I, *G=R+n, *B=G+n; int i;
  double mr=.2989360213*nrm, mg=.5870430745*nrm, mb=.1140209043*nrm;
  for(i=0; i<n; i++) *(GR++) = (float) (*(R++)*mr + *(G++)*mg + *(B++)*mb);
}

// Copy and normalize only
//template<class iT, class oT> 
void normalize( iT *I, oT *J, int n, oT nrm ) {
  int i;
  for(i=0; i<n; i++) *(J++)=(oT)*(I++)*nrm;
}

// Convert rgb to various colorspaces
//template<class iT, class oT>
oT* rgbConvert_impl( iT *I, int n, int d, int flag, oT nrm ) {
  oT *J = (oT*) wrMalloc(n*(flag==0 ? (d==1?1:d/3) : d)*sizeof(oT));
  int i, n1;
  oT thr;
  bool useSse;
  n1 =d*(n<1000?n/10:100); thr = (oT)(1.001);
  if(flag>1 && nrm==1) for(i=0; i<n1; i++) if(I[i]>thr)
    wrError("For floats all values in I must be smaller than 1.");
  //useSse = n%4==0 && typeid(oT)==typeid(float);
  useSse = n%4==0;
  if( flag==2 && useSse )
    for(i=0; i<d/3; i++) rgb2luv_sse(I+i*n*3,(float*)(J+i*n*3),n,(float)nrm);
  else if( (flag==0 && d==1) || flag==1 ) normalize(I,J,n*d,nrm);
  else if( flag==0 ) for(i=0; i<d/3; i++) rgb2gray_mex(I+i*n*3,J+i*n*1,n,nrm);
  else if( flag==2 ) for(i=0; i<d/3; i++) rgb2luv(I+i*n*3,J+i*n*3,n,nrm);
  else if( flag==3 ) for(i=0; i<d/3; i++) rgb2hsv(I+i*n*3,J+i*n*3,n,nrm);
  else wrError("Unknown flag.");
  return J;
}

// J = rgbConvertMex(I,flag,single); see rgbConvert_impl.m for usage details
#ifdef MATLAB_MEX_FILE
void rgbConvertMex(int nl, mxArray *pl, int nr, const mxArray *pr) {
  const int *dims; int i, nDims, n, d, dims1[3]; void *I; void *J; int flag;
  bool single; mxClassID idIn, idOut;

  // Error checking
  if( nr!=3 ) mexErrMsgTxt("Three inputs expected.");
  if( nl>1 ) mexErrMsgTxt("One output expected.");
  dims = (const int*) mxGetDimensions(pr); n=dims[0]*dims[1];
  nDims = mxGetNumberOfDimensions(pr);
  d = 1; for( i=2; i<nDims; i++ ) d*=dims[i];

  // extract input arguments
  I = mxGetPr(pr);
  flag = (int) mxGetScalar(pr+1);
  single = (bool) (mxGetScalar(pr+2)>0);
  idIn = mxGetClassID(pr);

  // call rgbConvert_impl() based on type of input and output array
  if(!((d==1 && flag==0) || flag==1 || (d/3)*3==d))
    mexErrMsgTxt("I must have third dimension d==1 or (d/3)*3==d.");
  if( idIn == mxSINGLE_CLASS && !single )
    J = (void*) rgbConvert_impl( (float*) I, n, d, flag, 1.0 );
  else if( idIn == mxSINGLE_CLASS && single )
    J = (void*) rgbConvert_impl( (float*) I, n, d, flag, 1.0f );
#if 0
  else if( idIn == mxDOUBLE_CLASS && !single )
    J = (void*) rgbConvert_impl( (double*) I, n, d, flag, 1.0 );
  else if( idIn == mxDOUBLE_CLASS && single )
    J = (void*) rgbConvert_impl( (double*) I, n, d, flag, 1.0f );
  else if( idIn == mxUINT8_CLASS && !single )
    J = (void*) rgbConvert_impl( (unsigned char*) I, n, d, flag, 1.0/255 );
  else if( idIn == mxUINT8_CLASS && single )
    J = (void*) rgbConvert_impl( (unsigned char*) I, n, d, flag, 1.0f/255 );
#endif
  else
    mexErrMsgTxt("Unsupported image type.");

  // create and set output array
  dims1[0]=dims[0]; dims1[1]=dims[1]; dims1[2]=(flag==0 ? (d==1?1:d/3) : d);
  idOut = single ? mxSINGLE_CLASS : mxDOUBLE_CLASS;
  mxSetNumericMatrix(pl, 0,0,idOut,mxREAL);
  mxSetData(pl,J); mxSetDimensions(pl,(const mwSize*) dims1,3);
}
#endif

#undef oT
#undef iT
