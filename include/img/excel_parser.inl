
#include "../DirectUI/mycell.h"
#if 0
#endif

UINT StyleE2BorderStyle(STRTREE* mm, int begpos, int ind, Border* b)
{
  int m2, m1;
  m1 = XmlFindSubNode(mm, begpos + 1, ind, XML_MARK_ATTRIBUTENAME, "ss:LineStyle", 12);

  if (m1 > 0 && mm->node[m1 + 1].type == XML_ATTRIBUTEVALUE) {
    m2 = id_str_find_id(EXlLineStyleSP, countof(EXlLineStyleSP), mm->node[m1 + 1].name.s, mm->node[m1 + 1].name.e-mm->node[m1 + 1].name.s);

    if (m2 >= 0) {
      b->LineStyle  = EXlLineStyleSP[m2].i;
    }
  }

  m1 = XmlFindSubNode(mm, begpos + 1, ind, XML_MARK_ATTRIBUTENAME, "ss:Weight", 9);

  if (m1 > 0 && mm->node[m1 + 1].type == XML_ATTRIBUTEVALUE) {
    b->Weight = atoi(mm->node[m1 + 1].name.s);
  }

  m1 = XmlFindSubNode(mm, begpos + 1, ind, XML_MARK_ATTRIBUTENAME, "ss:Color", 8);

  if (m1 > 0 && mm->node[m1 + 1].type == XML_ATTRIBUTEVALUE) {
    b->Color = RGBFromHtmlString(mm->node[m1 + 1].name.s);
  }

  return 0;
}

#define XMLFINDSUBNODE_GETINT(M, K, I, N, O)    {int m1 = XMLFINDSUBNODE((M), K, I, XML_MARK_ATTRIBUTENAME, N); if (m1>0 && (M)->node[m1+1].type==XML_ATTRIBUTEVALUE) { O = atoi((M)->node[m1+1].name.s); } }
#define XMLFINDSUBNODE_GETFLOAT(M, K, I, N, O)    {int m1 = XMLFINDSUBNODE((M), K, I, XML_MARK_ATTRIBUTENAME, N); if (m1>0 && (M)->node[m1+1].type==XML_ATTRIBUTEVALUE) { O = (float)atof((M)->node[m1+1].name.s); } }

int loadXmlExcel(Workbook* b, LPCTSTR xmlfn)
{
  STRTREE mm = {0};
  int i, j, k, m, n = 0;
  int m2, m1;
  mm.mode = XML_MARK_ALL;
  mm.s = LoadAsni(xmlfn, &mm.l);
  ParseXml(&mm, 0);
  mm.node = (STRNODE*)pmalloc(mm.maxlen * sizeof(STRNODE));
  mm.pos = 0;
  //mm.doecho = 0;
  ParseXml(&mm, 0);
  j = XmlFindSubNode(&mm, 0, 0, XML_MARK_ELEMENT, "Workbook", 8);

  if (j < 0) {
    return 0;
  }

  j = XmlFindSubNode(&mm, j + 1, 1, XML_MARK_ELEMENT, "Styles", 6);
  b->stylelen = XmlCountSubNode(&mm, j + 1, 2, XML_MARK_ELEMENT, "Style", 5);
  b->styleStore_ = MALLOC(StyleDesc, b->stylelen);
  memset(b->styleStore_, 0, sizeof(StyleDesc)*b->stylelen);

  for (i = 0, k = j; i < b->stylelen; ++i) {
    StyleDesc* s = b->styleStore_ + i;
    k = XmlFindSubNode(&mm, k + 1, 2, XML_MARK_ELEMENT, "Style", 5);
    m = XmlFindSubNode(&mm, k + 1, 3, XML_MARK_ATTRIBUTENAME, "ss:ID", 5);

    if (m > 0 && mm.node[m + 1].type == XML_ATTRIBUTEVALUE) {
      if (0 == mystrcmp("Default", 7, mm.node[m + 1].name.s, mm.node[m + 1].name.e-mm.node[m + 1].name.s)) {
        s->ID = 0;
      }
      else {
        //ASSERT('m'==mm.node[m+1].name.s[0]);
        s->ID = atoi(mm.node[m + 1].name.s + 1);
      }
    }

    //Alignment
    m = XmlFindSubNode(&mm, k + 1, 3, XML_MARK_ELEMENT, "Alignment", 9);

    if (m > 0) {
      m1 = XmlFindSubNode(&mm, m + 1, 4, XML_MARK_ATTRIBUTENAME, "ss:Horizontal", 13);

      if (m1 > 0 && mm.node[m1 + 1].type == XML_ATTRIBUTEVALUE) {
        m2 = id_str_find_id(EXlHAlignSP, countof(EXlHAlignSP), mm.node[m1 + 1].name.s, mm.node[m1 + 1].name.e-mm.node[m1 + 1].name.s);

        if (m2 >= 0) {
          s->hAlign = EXlHAlignSP[m2].i;
        }
      }

      m1 = XmlFindSubNode(&mm, m + 1, 4, XML_MARK_ATTRIBUTENAME, "ss:Vertical", 11);

      if (m1 > 0 && mm.node[m1 + 1].type == XML_ATTRIBUTEVALUE) {
        m2 = id_str_find_id(EXlVAlignSP, countof(EXlVAlignSP), mm.node[m1 + 1].name.s, mm.node[m1 + 1].name.e-mm.node[m1 + 1].name.s);
        s->vAlign = (m2 >= 0) ? EXlVAlignSP[m2].i : 0;
      }

      m1 = XmlFindSubNode(&mm, m + 1, 4, XML_MARK_ATTRIBUTENAME, "ss:WrapText", 11);

      if (m1 > 0 && mm.node[m1 + 1].type == XML_ATTRIBUTEVALUE) {
        s->bWrapText = mm.node[m1 + 1].name.s[0] == '0' ? FALSE : TRUE;
      }
    }

    //Font
    m = XmlFindSubNode(&mm, k + 1, 3, XML_MARK_ELEMENT, "Font", 4);

    if (m > 0) {
      m1 = XmlFindSubNode(&mm, m + 1, 4, XML_MARK_ATTRIBUTENAME, "ss:FontName", 11);

      if (m1 > 0 && mm.node[m1 + 1].type == XML_ATTRIBUTEVALUE) {
        STRncpy(s->logFont.lfFaceName, LF_FACESIZE, mm.node[m1 + 1].name);
      }

      m1 = XmlFindSubNode(&mm, m + 1, 4, XML_MARK_ATTRIBUTENAME, "ss:Bold", 7);

      if (m1 > 0 && mm.node[m1 + 1].type == XML_ATTRIBUTEVALUE) {
        s->logFont.lfWeight = mm.node[m1 + 1].name.s[0] == '1' ? FW_BOLD : FW_NORMAL;
      }

      XMLFINDSUBNODE_GETINT(&mm, m + 1, 4, "x:CharSet", s->logFont.lfCharSet);
      XMLFINDSUBNODE_GETINT(&mm, m + 1, 4, "ss:Size", s->logFont.lfHeight);
      XMLFINDSUBNODE_GETINT(&mm, m + 1, 4, "ss:StrikeThrough", s->logFont.lfStrikeOut);
      XMLFINDSUBNODE_GETINT(&mm, m + 1, 4, "ss:Italic", s->logFont.lfItalic);
      m1 = XmlFindSubNode(&mm, m + 1, 4, XML_MARK_ATTRIBUTENAME, "ss:Color", 8);

      if (m1 > 0 && mm.node[m1 + 1].type == XML_ATTRIBUTEVALUE) {
        s->crText = RGBFromHtmlString(mm.node[m1 + 1].name.s);
      }
    }

    //Borders
    m = XmlFindSubNode(&mm, k + 1, 3, XML_MARK_ELEMENT, "Borders", 7);

    if (m > 0) {
      struct borderstr {
        Border* b;
        char* s;
      } aa[] = {
        &s->borders.Left, "Left",
        &s->borders.Top, "Top",
        &s->borders.Right, "Right",
        &s->borders.Bottom, "Bottom",
        &s->borders.DiagonalRight, "DiagonalRight",
        &s->borders.DiagonalLeft, "DiagonalLeft"
      };

      for (; 1;) {
        m = XmlFindSubNode(&mm, m + 1, 4, XML_MARK_ELEMENT, "Border", 6);

        if (m1 < 0) {
          break;
        }

        m1 = XmlFindSubNode(&mm, m + 1, 5, XML_MARK_ATTRIBUTENAME, "ss:Position", 11);

        if (m1 > 0 && mm.node[m1 + 1].type == XML_ATTRIBUTEVALUE) {
          int m3;

          for (m3 = 0; m3 < countof(aa); ++m3) {
            if (0 == mystrcmp(aa[m3].s, (int)strlen(aa[m3].s), mm.node[m1 + 1].name.s, mm.node[m1 + 1].name.e-mm.node[m1 + 1].name.s)) {
              StyleE2BorderStyle(&mm, m1, 5, aa[m3].b);
              break;
            }
          }
        }
      }
    }

    //<Interior ss:Color="#FFFF99" ss:Pattern="Solid"/>
    m = XmlFindSubNode(&mm, k + 1, 3, XML_MARK_ELEMENT, "Interior", 8);

    if (m > 0) {
      m1 = XmlFindSubNode(&mm, m + 1, 4, XML_MARK_ATTRIBUTENAME, "ss:Color", 8);

      if (m1 > 0 && mm.node[m1 + 1].type == XML_ATTRIBUTEVALUE) {
        s->crBack = RGBFromHtmlString(mm.node[m1 + 1].name.s);
      }
    }
  }

  k = j;
  b->sheetlen = 1;

  for (; 1;) {
    Worksheet* s = b->vecWorksheet_;
    k = XmlFindSubNode(&mm, k + 1, 1, XML_MARK_ELEMENT, "Worksheet", 9);

    if (k < 0) {
      break;
    }

    m1 = XMLFINDSUBNODE(&mm, k + 1, 2, XML_MARK_ATTRIBUTENAME, "ss:Name");

    if (m1 > 0 && mm.node[m1 + 1].type == XML_ATTRIBUTEVALUE) {
      STRncpy(s->szName_, 32, mm.node[m1 + 1].name);
    }

    k = XmlFindSubNode(&mm, k + 1, 2, XML_MARK_ELEMENT, "Table", 5);

    if (k > 0) {
      XMLFINDSUBNODE_GETINT(&mm, k + 1, 3, "ss:ExpandedColumnCount", s->ExpandedColumnCount);
      XMLFINDSUBNODE_GETINT(&mm, k + 1, 3, "ss:ExpandedRowCount", s->ExpandedRowCount);
      XMLFINDSUBNODE_GETINT(&mm, k + 1, 3, "x:FullColumns", s->FullColumns);
      XMLFINDSUBNODE_GETINT(&mm, k + 1, 3, "x:FullRows", s->FullRows);
      XMLFINDSUBNODE_GETINT(&mm, k + 1, 3, "ss:DefaultColumnWidth", s->DefaultColumnWidth);
      XMLFINDSUBNODE_GETINT(&mm, k + 1, 3, "ss:DefaultRowHeight", s->DefaultRowHeight);
      m1 = XMLFINDSUBNODE((&mm), k + 1, 3, XML_MARK_ATTRIBUTENAME, "ss:StyleID");

      if (m1 > 0 && (&mm)->node[m1 + 1].type == XML_ATTRIBUTEVALUE) {
        s->StyleID = atoi((&mm)->node[m1 + 1].name.s + 1);
      }
    }

    s->chlen = XmlCountSubNode(&mm, k + 1, 3, XML_MARK_ELEMENT, "Column", 6);
    s->ch = MALLOC(ColHeader, s->chlen);

    for (i = 0, m = k; i < s->chlen; ++i) {
      ColHeader* c = s->ch + i;
      m = XmlFindSubNode(&mm, m + 1, 3, XML_MARK_ELEMENT, "Column", 6);
      m1 = XMLFINDSUBNODE((&mm), m + 1, 4, XML_MARK_ATTRIBUTENAME, "ss:StyleID");

      if (m1 > 0 && (&mm)->node[m1 + 1].type == XML_ATTRIBUTEVALUE) {
        c->StyleID = atoi((&mm)->node[m1 + 1].name.s + 1);
      }

      XMLFINDSUBNODE_GETFLOAT(&mm, m + 1, 4, "ss:Width", c->Width);
      XMLFINDSUBNODE_GETINT(&mm, m + 1, 4, "ss:Index", c->Index);
    }

    s->rhlen = XmlCountSubNode(&mm, k + 1, 3, XML_MARK_ELEMENT, "Row", 3);
    s->rh = MALLOC(RowHeader, s->rhlen);
    memset(s->rh, 0, sizeof(RowHeader)*s->rhlen);

    for (i = 0, m = k; i < s->rhlen; ++i) {
      RowHeader* c = s->rh + i;
      int i1, k1;
      m = XmlFindSubNode(&mm, m + 1, 3, XML_MARK_ELEMENT, "Row", 3);
      m1 = XMLFINDSUBNODE((&mm), m + 1, 4, XML_MARK_ATTRIBUTENAME, "ss:StyleID");

      if (m1 > 0 && (&mm)->node[m1 + 1].type == XML_ATTRIBUTEVALUE) {
        c->StyleID = atoi((&mm)->node[m1 + 1].name.s + 1);
      }

      XMLFINDSUBNODE_GETFLOAT(&mm, m + 1, 4, "ss:Height", c->Height);
      XMLFINDSUBNODE_GETINT(&mm, m + 1, 4, "ss:Index", c->Index);
      c->celen = XmlCountSubNode(&mm, m + 1, 4, XML_MARK_ELEMENT, "Cell", 4);
      c->ce = MALLOC(CellStore, c->celen);

      for (i1 = 0, k1 = m; i1 < c->celen; ++i1) {
        CellStore* e = c->ce + i1;
        k1 = XmlFindSubNode(&mm, k1 + 1, 4, XML_MARK_ELEMENT, "Cell", 4);
        XMLFINDSUBNODE_GETINT(&mm, k1 + 1, 5, "ss:Index", e->Index);
        XMLFINDSUBNODE_GETINT(&mm, k1 + 1, 5, "ss:MergeAcross", e->MergeAcross);
        m1 = XMLFINDSUBNODE((&mm), k1 + 1, 5, XML_MARK_ATTRIBUTENAME, "ss:StyleID");

        if (m1 > 0 && (&mm)->node[m1 + 1].type == XML_ATTRIBUTEVALUE) {
          e->StyleID = atoi((&mm)->node[m1 + 1].name.s + 1);
        }

        m1 = XMLFINDSUBNODE((&mm), k1 + 1, 5, XML_MARK_ELEMENT, "Data");
        m1 = XmlFindFirstNode((&mm), m1, XML_MARK_TEXT);

        if (m1 > 0) {
          e->data = (&mm)->node[m1].name;
          XMLPRINTNODE((&mm)->node[m1], " ");
        }
      }
    }
  }

  FreeXml(&mm);
  return 0;
}

