
#pragma pack(1) //设定为1字节对齐
typedef struct SAMPPAIR {
  short a;
  short b;
}
SAMPPAIR;
#pragma pack()//恢复

typedef struct SAMPPAIR_T {
  SAMPPAIR* pair;    // 样本结构
  int pairlen;
  int pos_num;
  int neg_num;
  //int img_num;
  int men_num;
  int img_per_men;
}
SAMPPAIR_T;

int samppair_free(SAMPPAIR_T* sp)
{
  FREE(sp->pair);
  MEMSET(sp, 0, 1);
  return 0;
}

// 标记样本结构中那些是正负样本和抛弃的样本(正=1,负=-1,抛弃=0)
#define SAMPPAIRFLAG(sp, i)     (   (((sp)->pair[i].b-(sp)->pair[i].a) / (sp)->img_per_men)   ? -1 : 1)

int samppair_init2(SAMPPAIR_T* sp, int n, const int* ids)
{
  int i, j, k = 0, nn = n * (n - 1) / 2;
  sp->pair = MALLOCSET(SAMPPAIR, nn, 0);

  for (i = 0; i < n; ++i) {
    for (j = i + 1; j < n; ++j) {
      if (ids[i] == ids[j]) {
        sp->pair[k].a = i;
        sp->pair[k].b = j;
        ++k;
      }
    }
  }

  sp->pos_num = k;

  for (i = 0; i < n; ++i) {
    for (j = i + 1; j < n; ++j) {
      if (ids[i] != ids[j]) {
        sp->pair[k].a = i;
        sp->pair[k].b = j;
        ++k;
      }
    }
  }

  ASSERT(k == nn);
  sp->pairlen = nn;
  sp->neg_num = nn - sp->pos_num;
  RANDOM_SHUFFLE(sp->neg_num, sp->pair + sp->pos_num, SAMPPAIR);
  return 0;
}

int samppair_init(SAMPPAIR_T* sp, int men_num, int img_per_men)
{
  int i, j;
  int k, all_pairs;
  int sampnum = 0;
  SAMPPAIR* pair = sp->pair;
  SAMPPAIR* rr;

  int img_num = men_num * img_per_men;
  int idxsz = (img_num - 1) * img_num / 2;
  sp->pair = MALLOCSET(SAMPPAIR, idxsz, 0);
  sp->men_num = men_num;
  sp->img_per_men = img_per_men;

  sp->pairlen = idxsz;

  sampnum = 0;

  for (k = 0; k < men_num; ++k) {
    for (i = 0; i < img_per_men; ++i) {
      for (j = i + 1; j < img_per_men; ++j) {
        sp->pair[ sampnum ].a = k * img_per_men + i;
        sp->pair[ sampnum ].b = k * img_per_men + j;
        ++sampnum;
      }
    }
  }

  sp->pos_num = sampnum;

  all_pairs = (men_num) * (men_num - 1) / 2;
  rr = MALLOCSET(SAMPPAIR, all_pairs, 0);
  assert(NULL != rr);

  for (k = 0, i = 0; i < men_num; ++i) {
    for (j = i + 1; j < men_num; ++j) {
      rr[ k ].a = i;
      rr[ k ].b = j;
      ++k;
    }
  }

  all_pairs = k;
  RANDOM_SHUFFLE(all_pairs, rr, SAMPPAIR);

  for (k = 0; k < all_pairs; ++k) {
    int a, b;

    if (all_pairs < 100 || k % (all_pairs / 100) == 2) {
      printf("选择样本%3d/%3d\r", k, all_pairs);
    }

    a = rr[ k ].a * (img_per_men);
    b = rr[ k ].b * (img_per_men);

    // 选择第a个人和第b个人的所有图片对压栈
    for (i = 0; i < img_per_men; ++i) {
      for (j = 0; j < img_per_men; ++j) {
        sp->pair[ sampnum ].a = a + i;
        sp->pair[ sampnum ].b = b + j;
        //sp->pair[ sampnum ].id = -1;
        ++sampnum;
      }
    }
  }

  sp->neg_num = sampnum - sp->pos_num;
  ASSERT(sp->pairlen == sampnum);

#define RANDPAIR_FILENAME "randpair.txt"

  if (0 && !fileok(RANDPAIR_FILENAME)) {
    FILE* pf = fopen(RANDPAIR_FILENAME, "w");

    if (NULL != pf) {
      for (i = 0; i < all_pairs; ++i) {
        fprintf(pf, "%3d %3d\n", rr[ i ].a, rr[ i ].b);
      }

      fclose(pf);
    }
  }

  FREE(rr);
  return 0;
}

