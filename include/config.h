
#define CPU_ONLY


#define countof(arr) sizeof(arr)/sizeof(arr[0])
#define MIN(a, b)  ((a) < (b) ? (a) : (b))
#define MAX(a, b)  ((a) > (b) ? (a) : (b))
#define BOUND(x, a, b)   ((x)<(a) ? (a) : ((x)>(b) ? (b) :(x)) )
#define ASSERT(x)  assert(x)
#define CMP(a, b)  ((a)>(b)) - ((a)<(b))
#define IFRET(ret, comp)  if (ret = comp) {return ret;}
#define IFCMPRET(ret, a, b)  if (ret = CMP(a, b)) {return ret;}

 