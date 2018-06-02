
#include "img/imgio.inl"
#include "ui/window.inl"

typedef struct obj_t {
  int v[128];
  double MediaBox_[4];
  char* stream;
} obj_t;

typedef struct xrefentry_t {
  int id;
  int ofs;  /* file offset / objstm object number */
  int gen;  /* generation / objstm index */
  int stmofs; /* on-disk stream */
  int type; /* 0=unset (f)ree i(n)use (o)bjstm */
  obj_t o[1]; /* stored/cached object */
} xrefentry_t;

enum {
#define PDFTYPEDEF(a)  a,
#include "pdftypedef.txt"
#undef PDFTYPEDEF
};

typedef struct pdf_t {
  xrefentry_t* xref;
  int Size;
} pdf_t;

int pdf_get_obj(pdf_t* pdf, str_t* bf, int j);

int pdf_nexttok(str_t* bf, int pos, int* ppos)
{
  static const char* toks = "|"
#define PDFTYPEDEF(a)  #a "|"
#include "pdftypedef.txt"
#undef PDFTYPEDEF
      ;
  str_t s0 = str2s(bf->s, bf->l, pos + 1, "/ ()[]<>\t\r\n", ppos);
  int k = STRsplitfind(toks, -1, s0.s, s0.l, 0, 0);
  //ASSERT(k >= 0);
  return k;
}
int pdf_get_int(pdf_t* pdf, str_t* bf, int pos, int* ppos)
{
  int x = str2i(bf->s, bf->l, pos, " ", &pos);
  str_skip(bf->s, bf->l, pos, " ");

  if (isdigit(bf->s[pos])) {
    int x1 = str2i(bf->s, bf->l, pos, " ", &pos);
    str_skip(bf->s, bf->l, pos, " ");

    if ('R' == bf->s[pos]) {
      ++pos;
      pdf_get_obj(pdf, bf, x);
      x = pdf->xref[x].o->v[Length];
    }
  }

  *ppos = pos;
  return x;
}
int pdf_get_obj_sub(pdf_t* pdf, str_t* bf, int pos, obj_t* o)
{
  pos = str_skip(bf->s, bf->l, pos, " ");
  if (memcmp(bf->s + pos, "<<", 2)) {
    //ASSERT(0 && "XREF\n");
    return 0;
  }

  pos += 2;
  pos = str_skip(bf->s, bf->l, pos, " ");

  for (; pos < bf->l && ('>' != bf->s[pos] && '>' != bf->s[pos]);) {
    if ('/' == bf->s[pos]) {
      int tok = pdf_nexttok(bf, pos, &pos);

      switch (tok) {
      case Type:
      case Subtype:
      case ColorSpace:
      case Filter:
      case PageLayout:
      case PageMode:
        o->v[tok] = pdf_nexttok(bf, pos, &pos);
        ASSERT(o->v[tok] > 0);
        break;

      case MediaBox:
        o->v[MediaBox] = 1;
        str_vscanf(bf->s + pos, bf->l - pos, 0, "[%I64f %I64f %I64f %I64f]", (va_list)o->MediaBox_, 0);
        break;

      case ID:
      case FontBBox:
      case ProcSet:
      case OpenAction:
        pos = str_find_chr(*bf, pos, ']');
        pos++;
        break;
        
      case Encoding:
      //case BaseFont:
      case CreationDate:
      case FontName:
      case Name:
        str2s(bf->s, bf->l, pos + 1, "/ ()[]", &pos);
        break;
        
      case BaseFont:
        str2s(bf->s, bf->l, pos + 1, "/ ()[]", &pos);
        break;
        
      case Kids:
        o->v[tok] = 1;
        str2s(bf->s, bf->l, pos + 1, "/ ()[]", &pos);
        break;
        
      case Nums:
        o->v[tok] = 1;
        str2s(bf->s, bf->l, pos + 1, "[]", &pos);
        ++pos;
        break;
        
      case XObject:
      case Style:
      case CIDSystemInfo:
        pos = str_skip(bf->s, bf->l, pos, "\r ");
        if ('<' == bf->s[pos] && '<' == bf->s[pos + 1]) {
          pos = STRfind(bf->s, bf->l, ">>", 2, pos);
          ASSERT(pos > 0);
          pos += 2;
        }

        break;

      case PageLabels:
      case Resources:
        pos = pdf_get_obj_sub(pdf, bf, pos, o);
        break;
      case Length:
        o->v[tok] = pdf_get_int(pdf, bf, pos, &pos);
        break;

      case Size:
      case Ascent:
      case Prev:
      case Height:
      case Width:
      case Metadata:
      case BitsPerComponent:
      case XHeight:
      case StemH:
      case MissingWidth:
      case Leading:
      case MaxWidth:
      case AvgWidth:
        o->v[tok] = pdf_get_int(pdf, bf, pos, &pos);
        break;
        
      case Info:
      case Root:
      case Next:
        o->v[tok] = str2i(bf->s, bf->l, pos, " ", &pos);
        break;
        
      default:
        break;
      }

      //} else if ('<'==bf->s[pos]) {
      //  pos = pdf_get_obj_sub(bf, pos, o);
    }
    else {
      ++pos;
    }
  }

  pos = str_skip(bf->s, bf->l, pos, "\r\n");
  ASSERT('>' == bf->s[pos] && pos + 10 < bf->l && '>' == bf->s[pos + 1]);
  pos += 2;
  return pos;
}
int get_obj_impl(pdf_t* pdf, str_t* bf, int pos, int j)
{
  obj_t* o = pdf->xref[j].o;
  pos = pdf_get_obj_sub(pdf, bf, pos, o);

  if (pos <= 0) {
    return 0;
  }

  pos = str_skip(bf->s, bf->l, pos, "\r\n");

  if (0 == memcmp(bf->s + pos, "stream", 6)) {
    int len = o->v[Length];
    pos += 6;
    pos = str_skip(bf->s, bf->l, pos, "\r\n\0");
    o->stream = (char*)realloc(o->stream, len);
    memcpy(o->stream, bf->s + pos, len);
    pos += len;
    pos = STRfind(bf->s, bf->l, "endstream", -1, pos);
    ASSERT(0 == memcmp(bf->s + pos, "endstream", 9));
    pos += 9;

    if (o->v[Type] == XObject && o->v[Subtype] == Image) {
      img_t im[1] = {0};
      printf("%04d_%dx%d %d\n", j, o->v[Height], o->v[Width], len);
      if (1) {
        char buf[256];
        _snprintf(buf, 256, "./out/%04d_%dx%d.jpg", j, o->v[Height], o->v[Width]);
        savefile(buf, o->stream, len);
      }
      if (0) {
        cvShowMat("asdfasdf", "%c", 100, 8, o->stream, 8, 1, -1); cvWaitKey(-1);
        //imread("aa.jpg", 3, 1, im);
        imload_mem(o->stream, len, 3, im);
        imshow(im);
        cvWaitKey(-1);
      }
    }
  }

  //pos = str_skip(bf->s, bf->l, pos, "\r\n");
  //ASSERT(0 == memcmp(bf->s + pos, "endobj", 6));
  pos = STRfind(bf->s, bf->l, "endobj", 2, pos);
  return pos;
}
int pdf_get_obj(pdf_t* pdf, str_t* bf, int j)
{
  xrefentry_t* e = pdf->xref + j;
  int pos = e->ofs, pos_end=0;
  obj_t* o = e->o;
  int k = 0, gen;
  if (1187==j) {
    int adsf=0;
  }
  e->id = str2i(bf->s, bf->l, pos, " ", &pos);
  gen = str2i(bf->s, bf->l, pos, " ", &pos);

  if (memcmp(bf->s + pos, "obj", 3)) {
    ASSERT(0 && "XREF\n");
  }

  pos += 3;
  pos = str_skip(bf->s, bf->l, pos, "\r\n");
  pos_end = STRfind(bf->s, bf->l, "endobj", 6, pos);
  if (pos_end>pos) {
    if ('<' == bf->s[pos] && '<' == bf->s[pos + 1]) {
      pos = get_obj_impl(pdf, bf, pos, j);
    }
    else if ('0' <= bf->s[pos] && bf->s[pos] <= '9') {
      o->v[Length] = str2i(bf->s, bf->l, pos, " ", &pos);
      pos = str_skip(bf->s, bf->l, pos, "\r\n");
      ASSERT(0 == memcmp(bf->s + pos, "endobj", 6));
    }
    else {
      ASSERT(0);
    }
  }

  return pos;
}


