
#include "cstd.h"
#include "cfile.h"
#include "str.h"
#include "str/scanf.inl"
#include "pdf.inl"
#include "test_tif.inl"

#define PDFHDAD    "%PDF-"
#define STARTXREF  "startxref"
#define XREF  "xref"
#define STATIC_STRLEN(xx)  (sizeof(xx)-1)

#define PDF_NUL   0x00 // NULL
#define PDF_HT    0x09 // HORIZONTAL
#define PDF_LF    0x0A // LINE FEED
#define PDF_FF    0x0C // FORM FEED
#define PDF_CR    0x0D // CARRIAGE RETURN
#define PDF_SP    0x20 // SPACE

int get_startxref(pdf_t* pdf, str_t* bf, int pos)
{
  int j;
  int ofs, len, nobj = 0;
  obj_t trailer[1] = {0};

  {
    int pos1 = STRfind(bf->s, bf->l, "trailer", -1, pos);

    if (pos1 < 0) {
      ASSERT(0 && "trailer\n");
    }

    pos1 += strlen("trailer");
    pos1 = str_skip(bf->s, bf->l, pos1, " \r\n");
    pdf_get_obj_sub(pdf, bf, pos1, trailer);
  }

  if (memcmp(bf->s + pos, XREF, STATIC_STRLEN(XREF))) {
    ASSERT(0 && "XREF\n");
  }

  pos += STATIC_STRLEN(XREF) + 1;

  if (pdf->Size < trailer->v[Size]) {
    pdf->Size = trailer->v[Size];
    pdf->xref = (xrefentry_t*)realloc(pdf->xref, sizeof(xrefentry_t) * pdf->Size);
  }

  for (; 1;) {
    ofs = str2i(bf->s, bf->l, pos, " ", &pos);
    len = str2i(bf->s, bf->l, pos, " ", &pos);

    for (j = 0; j < len; ++j) {
      pdf->xref[ofs + j].ofs = str2i(bf->s, bf->l, pos, " ", &pos);
      pdf->xref[ofs + j].gen = str2i(bf->s, bf->l, pos + 1, " ", &pos);
      pdf->xref[ofs + j].type = bf->s[pos++];

      while (PDF_LF != bf->s[pos]) {
        ++pos;
      }

      ++pos;
    }

    nobj = MAX(nobj, ofs + len);
    pos = str_skip(bf->s, bf->l, pos, "\r\n");

    if (0 == memcmp(bf->s + pos, "trailer", 7)) {
      break;
    }
  }

  for (j = 0; j < nobj; ++j) {
    if ('n' == pdf->xref[j].type) {
      pdf_get_obj(pdf, bf, j);
    }
  }

  return trailer->v[Prev];
}

int test_pdf()
{
  str_t bf[1] = {0};
  pdf_t pdf[1] = {0};
  int pos;
  _chdir("E:/pub/bin/pdf");

  //test_tif();
  //str_load("hello.pdf", bf);
  str_load("38万希特勒战俘在美国.pdf", bf);

  if (memcmp(bf->s, PDFHDAD, STATIC_STRLEN(PDFHDAD))) {
    ASSERT(0 && "不是PDF\n");
  }

  pos = bf->l;
  pos = str_rfind_str(*bf, STR1("startxref"), pos);
  pos += STATIC_STRLEN(STARTXREF) + 1;
  pos = str2i(bf->s, bf->l, pos, " ", 0);

  if (pos < 0) {
    ASSERT(0 && "没有startxref");
  }

  for (; pos = get_startxref(pdf, bf, pos););

  str_free(bf);
  return 0;
}

