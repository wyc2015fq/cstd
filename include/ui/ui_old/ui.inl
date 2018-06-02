
#ifndef _UI_INL_
#define _UI_INL_

#include "cstd.h"
#include "str.h"
#include "draw\imdraw.inl"
#include "draw\imdrawaa.inl"
#include "ui.h"

#include "res.inl"
#include "drawui.inl"
#include "drawuiblue.inl"
#include "drawuivisual.inl"
//#include "ribbontab.inl"
#include "win.inl"

enum {
#define UICTRLINFODEF(name)  dm_##name,
#include "uictrl.txt"
#undef UICTRLINFODEF
};

typedef struct null_ctrl {
  uictrl base;
} null_ctrl;
int draw_null_ctrl(const null_ctrl* s, const event* e)
{
  return 0;
}

int null_ctrl_event(null_ctrl* s, const event* e)
{
  switch (e->msg) {
  case EVENT_INIT:
    UISETCALL(s, draw_null_ctrl, null_ctrl_event);
    break;
  }

  return 0;
}

#include "layout_ctrl.inl"
#include "button_ctrl.inl"
#include "slider_ctrl.inl"
#include "rbox_ctrl.inl"
#include "cbox_ctrl.inl"
#include "polygon_ctrl.inl"
#include "bezier_ctrl.inl"
#include "scale_ctrl.inl"
#include "edit_ctrl.inl"
#include "spline_ctrl.inl"
#include "gamma_ctrl.inl"
#include "progress_ctrl.inl"
#include "scroll_ctrl.inl"
#include "listbox_ctrl.inl"
#include "tree_ctrl.inl"
#include "tab_ctrl.inl"
//#include "buttonst_ctrl.inl"
#include "combobox_ctrl.inl"
//#include "colorpicker_ctrl.inl"
//#include "UITab.inl"
//#include "outlooktab_ctrl.inl"
#include "menu_ctrl.inl"
#include "toolbar_ctrl.inl"
#include "uiwindow.inl"
#include "uipopup.inl"
#include "uimdi.inl"
#include "evalayout_ctrl.inl"
//#include "sudoku_ctrl.inl"

typedef struct uictrlinfo_t {
  char* classname;
  int size;
  event_t event_f;
  draw_t draw_f;
} uictrlinfo_t;
uictrlinfo_t g_uictrlinfo[] = {
#define UICTRLINFODEF(name)  { #name, sizeof(name##_ctrl), (event_t)name##_ctrl_event, (draw_t)draw_##name##_ctrl },
#include "uictrl.txt"
#undef UICTRLINFODEF
};


#include "dm_win2000.inl"
#include "dm_agg.inl"

int ui_delctrls(uiwids* uw, event* e)
{
  int i;
  event et[1] = {0};
  *et = *e;
  et->msg = EVENT_EXIT;

  for (i = 0; i < uw->widlen; ++i) {
    UICALL_EVENT(uw->wid[i], et);
    uw->wid[i] = 0;
  }

  SAFEFREE(uw->wid);
  uw->wid = 0;
  uw->widlen = 0;
  return 0;
}

int ui_setctrl(uiwids* uw, int id, void* ctrl)
{
  int newlen = id + 1;

  if (uw->widlen < newlen) {
    uw->wid = (uictrl**)my_realloc(uw->wid, sizeof(uictrl*)*uw->widlen, sizeof(uictrl*)*newlen);
    uw->widlen = newlen;
  }

  ASSERT(NULL == uw->wid[id] && "¿Ø¼þidÓÐÖØ¸´£¡");
  uw->wid[id] = (uictrl*)ctrl;
  return id;
}

int ui_initctrl(const char* szClass, const void* ctrl, const char* text)
{
  int i;
  uictrlinfo_t* info = 0;
  uictrl* s = (uictrl*)ctrl;
  int infolen = countof(g_uictrlinfo);

  for (i = 0; i < infolen; ++i) {
    if (0 == stricmp(g_uictrlinfo[i].classname, szClass)) {
      info = g_uictrlinfo + i;
      break;
    }
  }

  if (info) {
    event et[1] = {0};
    et->ui = 0;
    et->msg = EVENT_INIT;
    s->event_f = info->event_f;
    s->event_f(s, et);
    et->msg = EVENT_SETATTRIBUTE;

    if (text) {
      str_t str[2] = {0};
      str_t ss = STRtrim(STR1(text), "() ");
      str[0] = STR1("data");
      str_setstr1(str + 1, ss);
      str_replace_chr(str + 1, ';', '\n');
      et->wp = (UINT)(str);
      s->event_f(s, et);
      str_free(str + 1);
    }
  }

  return 0;
}

int CreateWidget(UI_engine* ui, uictrl** w, STRTREE* pmm, int pos)
{
  //hwnd = Wnd_CreateEx(szClass, szWindow, dwStyle, dwExStyle, hParent);
  int i, ni, id = -1;
  char szClass[256];
  int infolen = countof(g_uictrlinfo);
  uictrlinfo_t* info = 0;

  ni = XmlFindNode(pmm, pos, XML_MARK_ATTRIBUTENAME, "id", 2);

  if (ni == 0) {
    return 0;
  }

  STRncpy(szClass, 256, pmm->node[pos].name);

  for (i = 0; i < infolen; ++i) {
    if (0 == stricmp(g_uictrlinfo[i].classname, szClass)) {
      info = g_uictrlinfo + i;
      break;
    }
  }

  if (info) {
    event et[1] = {0};
    str_t s[32] = {0};
    int ni2, i, n = XmlGetAttributes(pmm, pos, s);
    w[0] = (uictrl*)malloc(info->size);
    et->ui = ui;
    memset(w[0], 0, info->size);
    et->msg = EVENT_INIT;
    w[0]->event_f = info->event_f;
    w[0]->event_f(w[0], et);
    id = atoi(pmm->node[ni + 1].name.s);
    et->msg = EVENT_SETATTRIBUTE;

    for (i = 0; i < n; i += 2) {
      et->wp = (UINT)(s + i);
      w[0]->event_f(w[0], et);
    }

    ni2 = XmlFindFirstNode(pmm, pos, XML_MARK_TEXT);

    if (ni2 > 0) {
      s[0] = STR1("data");
      s[1] = pmm->node[ni2].name;
      et->wp = (UINT)(s);
      w[0]->event_f(w[0], et);
    }
  }

  return id;
}

int CreateFromXml(UI_engine* ui, uiwids* uw, STRTREE* pmm, str_t ctrlsname)
{
  int j, k = 0, elemnum = 0, elempos, el = ctrlsname.l, eind;
  //CreateFontXml(ui, pmm, fontsname, fontsname.l);
  elempos = XmlFindElemWithAttribute(pmm, "name", 4, ctrlsname.s, el);
  assert(elempos >= 0);
  eind = pmm->node[elempos].ind;

  if (elempos < 0) {
    return 0;
  }

  ++elempos;
  elemnum = 0;

  for (j = elempos; j < pmm->len && (pmm->node[j].ind > eind); ++j) {
    if (XML_ELEMENTNAME == pmm->node[j].type) {
      uictrl* wi[1] = {0};
      int id, ni;
      ni = XmlFindNode(pmm, j, XML_MARK_ATTRIBUTENAME, "id", 2);

      if (ni > 0) {
        id = atoi(pmm->node[ni + 1].name.s);
        elemnum = MAX(id, elemnum);
      }
    }
  }

  ++elemnum;
  uw->wid = (uictrl**)malloc(elemnum * sizeof(uictrl*));
  memset(uw->wid, 0, elemnum * sizeof(uictrl*));
  uw->widlen = elemnum;

  for (j = elempos; j < pmm->len && (pmm->node[j].ind > eind); ++j) {
    if (XML_ELEMENTNAME == pmm->node[j].type) {
      uictrl* wi[1] = {0};
      int id = CreateWidget(ui, wi, pmm, j);

      if (id >= 0) {
        uw->wid[id] = *wi;
      }
    }
  }

  return uw->wid != NULL;
}

int ui_loadxml(UI_engine* ui, uiwids* uw, const char* xmlfn)
{
  STRTREE xml[1] = {0};
  int ret = 0, ni1, ni2, elempos;
  str_t ctrlsname = {0}, fontsname = {0};

  xml_load(xml, xmlfn);
  elempos = XmlFindElemWithAttribute(xml, "name", 4, "main", 4);
  ni1 = XmlFindNode(xml, elempos, XML_MARK_ATTRIBUTENAME, "fonts", 5);
  ni2 = XmlFindNode(xml, elempos, XML_MARK_ATTRIBUTENAME, "ctrls", 5);

  if (ni2 >= 0) {
    ctrlsname = STRtrim(xml->node[ni2 + 1].name, "\"");
    ret = CreateFromXml(ui, uw, xml, ctrlsname);
  }

  xml_free(xml);
  return ret;
}

int UI_set(UI_engine* ui, const char* name, void* subctrl, HWND hParent)
{
  UI_setImpl(ui, name, subctrl, hParent);
  ui->res->dm_draw_f = dm_win2000;
  return 0;
}

int test_ui_init(int row, int col)
{
  static int inited = 0;
  test_ui_t* g = g_test_ui;
  g_test_ui->row = MAX(row + 1, g_test_ui->row);
  g_test_ui->col = MAX(col + 1, g_test_ui->col);
  matrix_rect(g->base.rc, g->row, g->col, 4, 4, g->num, g->rc);

  if (0 == inited) {
    inited = 1;
    UISETCALL(g, test_ui_paint_proc, test_ui_frame_proc);
    UI_set(g_test_ui->ui, "test_showims", &g_test_ui->base, 0);
  }

  return row * g_test_ui->col + col;
}
int showims(int row, int col, int h, int w, const unsigned char* A, int al, int ai)
{
  int i = test_ui_init(row, col);
  img_t im[1] = {0};
  IMINIT(im, h, w, A, al, ai, 1);
  imto32(im, g_test_ui->m_img + i, 0);
  return 0;
}
int test_showims()
{
  uchar img[100 * 100] = {0};
  memset(img, 255, 50 * 100);
  showims(0, 0, 100, 100, img, 100, 1);
  showims(0, 1, 100, 100, img, 100, 1);
  showims(1, 0, 100, 100, img, 100, 1);
  showims(1, 1, 100, 100, img, 100, 1);
  waitkey(g_test_ui->ui, -1);
  return 0;
}

#endif // _UI_INL_
