/*
 * GraphicsGems.h
 * Version 1.0 - Andrew Glassner
 * from "Graphics Gems", Academic Press, 1990
 */

#ifndef GG_H

#define GG_H 1

/*********************/
/* 2d geometry types */
/*********************/

typedef struct Point2Struct { /* 2d point */
  double x, y;
} Point2a;
typedef DPOINT Point2;
typedef Point2 Vector2;

typedef struct IntPoint2Struct {  /* 2d integer point */
  int x, y;
} IntPoint2;

typedef struct Matrix3Struct {  /* 3-by-3 matrix */
  double element[3][3];
} Matrix3;

typedef struct Box2dStruct {    /* 2d box */
  Point2 min, max;
} Box2;


/*********************/
/* 3d geometry types */
/*********************/

typedef struct Point3Struct { /* 3d point */
  double x, y, z;
} Point3;
typedef Point3 Vector3;

typedef struct IntPoint3Struct {  /* 3d integer point */
  int x, y, z;
} IntPoint3;


typedef struct Matrix4Struct {  /* 4-by-4 matrix */
  double element[4][4];
} Matrix4;

typedef struct Box3dStruct {    /* 3d box */
  Point3 min, max;
} Box3;



/***********************/
/* one-argument macros */
/***********************/
#undef ABS
#undef ASSERT
#undef MIN
#undef MAX
#undef SWAP
#undef CLAMP
#undef ROUND
/* absolute value of a */
#define ABS(a)    (((a)<0) ? -(a) : (a))

/* round a to nearest int */
#define ROUND(a)  floor((a)+0.5)

/* take sign of a, either -1, 0, or 1 */
#define ZSGN(a)   (((a)<0) ? -1 : (a)>0 ? 1 : 0)

/* take binary sign of a, either -1, or 1 if >= 0 */
#define SGN(a)    (((a)<0) ? -1 : 1)

/* shout if something that should be true isn't */
#define ASSERT(x)  if (!(x)) fprintf(stderr," Assert failed: x\n");

/* square a */
#define SQR(a)    ((a)*(a))


/***********************/
/* two-argument macros */
/***********************/

/* find minimum of a and b */
#define MIN(a,b)  (((a)<(b))?(a):(b))

/* find maximum of a and b */
#define MAX(a,b)  (((a)>(b))?(a):(b))

/* swap a and b (see Gem by Wyvill) */
#define SWAP(a,b) { a^=b; b^=a; a^=b; }

/* linear interpolation from l (when a=0) to h (when a=1)*/
/* (equal to (a*h)+((1-a)*l) */
#define LERP(a,l,h) ((l)+(((h)-(l))*(a)))

/* clamp the input to the specified range */
#define CLAMP(v,l,h)  ((v)<(l) ? (l) : (v) > (h) ? (h) : v)


/****************************/
/* memory allocation macros */
/****************************/

/* create a new instance of a structure (see Gem by Hultquist) */
#define NEWSTRUCT(x)  (struct x *)(pmalloc((unsigned)sizeof(struct x)))

/* create a new instance of a type */
#define NEWTYPE(x)  (x *)(pmalloc((unsigned)sizeof(x)))


/********************/
/* useful constants */
/********************/

//#define PI    3.141592  /* the venerable pi */
#define PITIMES2  6.283185  /* 2 * pi */
#define PIOVER2   1.570796  /* pi / 2 */
#define E   2.718282  /* the venerable e */
#define SQRT2   1.414214  /* sqrt(2) */
#define SQRT3   1.732051  /* sqrt(3) */
#define GOLDEN    1.618034  /* the golden ratio */
#define DTOR    0.017453  /* convert degrees to radians */
#define RTOD    57.29578  /* convert radians to degrees */


/************/
/* booleans */
/************/

#define TRUE    1
#define FALSE   0
#define ON    1
#define OFF   0
#define boolean int     /* boolean data type */
typedef boolean flag;     /* flag data type */

extern double V2SquaredLength(Vector2* a);
extern double V2Length(Vector2* a);
extern double V2Dot(Vector2* a, Vector2* b);
extern double V2DistanceBetween2Points(Point2* a, Point2*);
extern Vector2* V2Negate(Vector2* v);
extern Vector2* V2Normalize(Vector2* v);
extern Vector2* V2Scale(Vector2* v, double newlen);
extern Vector2* V2Add(Vector2* a, Vector2* b, Vector2* c);
extern Vector2* V2Sub(Vector2* a, Vector2* b, Vector2* c);
extern Vector2* V2Lerp(Vector2* lo, Vector2* hi, double alpha, Vector2* result);
extern Vector2* V2Combine(Vector2* a, Vector2* b, Vector2* result, double ascl, double bscl);
extern Vector2* V2Mul(Vector2* a, Vector2* b, Vector2* result);
extern Vector2* V2MakePerpendicular(Vector2* a, Vector2* ap);
extern Vector2* V2New(double x, double y);
extern Vector2* V2Duplicate(Vector2* a);
extern Point2* V2MulPointByProjMatrix(Point2* pin, Matrix3* m, Point2* pout);
extern Matrix3* V2MatMul(Matrix3* a, Matrix3* b, Matrix3* c);
extern Matrix3* TransposeMatrix3(Matrix3* a, Matrix3* b);

extern double V3SquaredLength(Vector3* a);
extern double V3Length(Vector3* a);
extern double V3Dot(Vector3* a, Vector3* b);
extern double V3DistanceBetween2Points(Point3* a, Point3* b);
extern Vector3* V3Negate(Vector3* v);
extern Vector3* V3Normalize(Vector3* v);
extern Vector3* V3Scale(Vector3* v, double newlen);
extern Vector3* V3Add(Vector3* a, Vector3* b, Vector3* c);
extern Vector3* V3Sub(Vector3* a, Vector3* b, Vector3* c);
extern Vector3* V3Lerp(Vector3* lo, Vector3* hi, double alpha, Vector3* result);
extern Vector3* V3Combine(Vector3* a, Vector3* b, Vector3* result, double ascl, double bscl);
extern Vector3* V3Mul(Vector3* a, Vector3* b, Vector3* result);
extern Vector3* V3Cross(Vector3* a, Vector3* b, Vector3* c);
extern Vector3* V3New(double x, double y, double z);
extern Vector3* V3Duplicate(Vector3* a);
extern Point3* V3MulPointByMatrix(Point3* pin, Matrix3* m, Point3* pout);
extern Point3* V3MulPointByProjMatrix(Point3* pin, Matrix4* m, Point3* pout);
extern Matrix4* V3MatMul(Matrix4* a, Matrix4* b, Matrix4* c);
extern gcd(int u, int v);
extern double RegulaFalsi(double(*f)(), double left, double right);
extern double NewtonRaphson(double(*f)(), double(*df)(), double x);
extern double findroot(double left, double right, double tolerance, double(*f)(), double(*df)());

#endif

