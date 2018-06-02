#include "cstd.h"
#include "img/imgio.inl"
typedef struct capimgseq {
  int pos;
  bool loop;
  vstr_t sv[1];
}
capimgseq;

static int capimgseq_close(capdev* s0)
{
  capimgseq* s = (capimgseq*)(s0->x);
  
  if (s) {
    vstr_free(s->sv);
    free(s);
    s0->x = NULL;
  }
  return 0;
}

static int capimgseq_getframe(struct cap_t* s0, img_t* im, int flag)
{
  capimgseq* s = (capimgseq*)(s0->x);
  if (s->sv->n>0) {
    const char* name = NULL;
    if (s->loop) {
      name = s->sv->v[(s->pos)%s->sv->n].s;
    } else {
      if (s->pos>=s->sv->n) {
        return 0;
      }
      name = s->sv->v[(s->pos)%s->sv->n].s;
    }
    s->pos++;
    if (NULL==name) {
      return 0;
    }
    return imread(name, 3, 1, im)!=NULL;
  }
  return 0;
}

static int capimgseq_open(capdev* s0, const char* filelist, bool loop)
{
  capimgseq* s = NULL;
  vstr_t sv[1] = {0};
  if (vstr_load(filelist, sv)) {
    if (sv->n>0) {
      s = (capimgseq*)malloc(sizeof(capimgseq));
      assert(s != NULL);
      memset(s, 0, sizeof(capimgseq));
      s->loop = loop;
      *s->sv = *sv;
      s0->x = s;
      s0->close = capimgseq_close;
      s0->getframe = capimgseq_getframe;
    }
  }

  return s!=0;
}
