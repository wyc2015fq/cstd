
CDiagramEntity::CDiagramEntity()
{
  Clear();
  str_setstr2(m_type, _T("basic"));

}

void CDiagramEntity::Clear()
{

  m_rc = iRECT(0, 0, 0, 0);
  m_selected = FALSE;

}

BOOL CDiagramEntity::FromString(const str_t& str)
{

  BOOL result = FALSE;
  str_t data[1] = {0};
  str_t header[1] = {0};
  GetHeaderFromString(str, header, data);

  if (0 == str_cmp(*header, *m_type)) {
    if (GetDefaultFromString(data)) {
      result = TRUE;
    }
  }

  str_free(data);
  str_free(header);
  return result;

}

int CDiagramEntity::GetHeaderFromString(str_t str, str_t* header, str_t* data)
{

  vstr_t main = {0};

  strv_setstr(&main, str, ':', " \t");

  if (main.n == 2) {
    str_setstr1(header, main.v[0]);
    str_setstr1(data, main.v[1]);
  }

  return 0;

}

BOOL CDiagramEntity::GetDefaultFromString(str_t* str)
{
  BOOL result = FALSE;
  str_t data;
  vstr_t tok[1] = {0};
  int size;
  data = STRtrim(*str, ";");
  strv_setstr(tok, data, ',', " \t");

  size = tok->n;

  if (size >= 7) {
    int left;
    int top;
    int right;
    int bottom;
    str_t str1 = {0};
    int group;
    int count = 0;

    left = atoi(tok->v[count++].s);
    top = atoi(tok->v[count++].s);
    right = atoi(tok->v[count++].s);
    bottom = atoi(tok->v[count++].s);
    str1 = tok->v[count++];
    str_setstr1(m_title, str1);
    str1 = tok->v[count++];
    str_setstr1(m_name, str1);
    str1 = tok->v[count++];
    group = atoi(tok->v[count++].s);

    m_rc = iRECT(left, top, right, bottom);

    // Rebuild rest of string
    str->l = 0;

    for (int t = count ; t < size ; t++) {
      str1 = tok->v[t];

      str_cat(str, str1);

      if (t < size - 1) {
        str_cat(str, STR1(","));
      }
    }

    result = TRUE;
  }

  strv_free(tok);
  return result;

}

BOOL CDiagramEntity::LoadFromString(str_t str, str_t* data)
{

  BOOL result = FALSE;
  str_t header[1] = {0};
  GetHeaderFromString(str, header, data);

  if (0 == str_cmp(*header, *m_type))
    if (GetDefaultFromString(data)) {
      result = TRUE;
    }

  return result;

}

int CDiagramEntity::GetDefaultGetString(str_t* str) const
{
  str_format(str, _T("%s:%f,%f,%f,%f,%s,%s,%i"), m_type->s, m_rc.left, m_rc.top, m_rc.right, m_rc.bottom, m_title->s, m_name->s, 0);
  return 0;
}

void CDiagramEntity::MoveRect(int x, int y)
{
  m_rc = iRECT(m_rc.left + x, m_rc.top + y, m_rc.right + x, m_rc.bottom + y);
}

BOOL CDiagramEntity::BodyInRect(RECT rect) const
{

  BOOL result = FALSE;
  RECT rectEntity = m_rc;
  RECT rectIntersect;

  NormalizeRect(&rect);
  NormalizeRect(&rectEntity);

  IntersectRect(&rectIntersect, &rect, &rectEntity);

  if (!IsRectEmpty(&rectIntersect)) {
    result = TRUE;
  }

  return result;

}

int CDiagramEntity::GetHitCode(POINT point) const
{

  RECT rect = m_rc;
  return GetHitCode(point, rect);

}

void CDiagramEntity::DrawObject(HDDC dc)
{

  Draw(dc, m_rc);


}

void CDiagramEntity::Draw(HDDC dc, RECT rect)
{
  DrawRect(dc, rect, _RGB(255, 255, 255), _RGB(0, 0, 0));
}

str_t CDiagramEntity::Export(UINT /*format*/) const
{

  return STR1("");

}

int CDiagramEntity::GetHitCode(const POINT& point, const RECT& rect) const
{

  int result = DEHT_NONE;

  if (PtInRect(&rect, point)) {
    result = DEHT_BODY;
  }

  return result;

}

