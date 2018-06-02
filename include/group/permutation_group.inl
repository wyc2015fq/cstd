

typedef struct permutation_t_ permutation_t;
struct permutation_t_ {
  uint hash;
  permutation_t* next;
  uint p[1];
};

CC_INLINE unsigned int permutation_hash(const uint* str, unsigned int len)
{
  unsigned int a = 63689;
  unsigned int hash = 0;
  unsigned int i = 0;
  for (i = 0; i < len; str++, i++) {
    hash = hash * a + (*str);
    a = a * 378551;
  }
  return hash;
}
#define PGROUP_MAXHASHBUFSIZE  256
typedef struct {
  int dim; //维度
  int count; // 元素个数
  permutation_t* buf[PGROUP_MAXHASHBUFSIZE];
} pgroup_t;

permutation_t* pgroup_findadd(pgroup_t* g, const uint* p, int n, bool bAdd) {
  uint hash = permutation_hash(p, n);
  int id = hash%PGROUP_MAXHASHBUFSIZE;
  permutation_t* per = g->buf[id];
  ASSERT(n==g->dim);
  for (; per; per = per->next) {
    if (0==memcmp(p, per->p, sizeof(*p)*n)) {
      return per;
    }
  }
  if (bAdd) {
    permutation_t* newp;
    newp = (permutation_t*)pmalloc(g->dim + sizeof(permutation_t));
    BZERO(newp);
    MEMCPY(newp->p, p, n);
    newp->hash = hash;
    newp->next = g->buf[id];
    g->buf[id] = newp;
  }
  return NULL;
}
