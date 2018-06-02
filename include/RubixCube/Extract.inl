
#define HCODESIZE 256
#define GROUPSIZE 15

struct Node {
  inline UINT Index() const {
    return color >> 16;
  }
  inline int Compare(const Node& n) const {
    return color - n.color;
  }
  inline void Max(UINT& v) const {
    if (count > v) {
      v = count;
    }
  }

  DWORD color;
  UINT count;
};

struct Group {
  DWORD min, max;
  COLOR rgb;
  BYTE noColor;
  UINT facets[54];
};

struct CExtract {
  Node hist[256];
  int hist_count;
  int colors[HCODESIZE];
  Group groups[GROUPSIZE];

  DWORD ColorRGB(COLOR c) const;
  COLOR RColor(DWORD color) const;

  void Insert(DWORD color);
  void Position(DWORD color, UINT ix);
  void CreateGammaCurve(double gamma);
  COLOR GetTextColor(BYTE noColor);
  COLOR GetTextColor(img_t* im, const IRECT* pclip, IRECT r);
  void SetTextId(img_t* im, const IRECT* pclip, int ox, int oy, int m, int n);
  IRECT SetTextMessage(img_t* im, const IRECT* pclip, const char* msg, IRECT rect, int b = 0);

  UINT max;
  int noGroup;
  int m_error;

  DWORD pixels[6][33][33];               // copie des 6 images de faces
  BYTE facets[54];                       // couleurs des facettes du Rubik's Cube
  UINT color1[54], color2[54];
  static DWORD tconv[256], Gamma[256];

  img_t pBitmap[6];     // six images des faces du Rubik's Cube
public:
  CExtract();
  ~CExtract();

  void FacetsErase();
  void DeleteAll();
  void DeleteImages();
  bool AddImage(img_t* pBm);

  int FileRead(const char* pathname);
  BOOL FileWrite(const char* pathname);

  int Analyse();

  IRECT DrawImage(img_t* im, const IRECT* pclip, IPOINT org, int width, bool acq);
  IRECT DrawImage(img_t* im, const IRECT* pclip, IRECT rect, bool acq);
  IRECT DrawIcon(img_t* im, const IRECT* pclip, IRECT rect, const char* fileName);
  void DrawDebugMsg(img_t* im, const IRECT* pclip, IRECT rect);

  int ToString(char* out, int len, bool cor = true) const;

  bool IsReady() const;
  inline bool IsBegin() const {
    return pBitmap[0].tt.data != NULL && pBitmap[5].tt.data == NULL;
  }

  void DrawHist(img_t* im, const IRECT* pclip, IRECT rect, const char* errMsg = "");

  inline UINT GetCount() const {
    return hist_count;
  }
  bool Check();
};

DWORD CExtract::tconv[256] = {DWORD(-1), };
DWORD CExtract::Gamma[256];

CExtract::CExtract()
{
  //pBitmap[0] = pBitmap[1] = pBitmap[2] = pBitmap[3] = pBitmap[4] = pBitmap[5] = NULL;
  memset(pBitmap, 0, sizeof(pBitmap));

  if (tconv[0] == DWORD(-1)) {
    for (int i = 0; i < 256; i++) {
      DWORD j, k, n;

      for (j = 0, k = i, n = 0; j < 8; j++, k <<= 1) {
        n = (n << 3) | (k & 0x00000080);
      }

      tconv[i] = n >> 7;
    }

    CreateGammaCurve(1.0);
  }

  DeleteAll();
}

CExtract::~CExtract()
{
  DeleteImages();
}

static const char colorsId[] = "ORGBWY";
static const char sidesId[] = "DRFULB";  // faces in order of entry

#if 0
static const COLOR defaultColors[6] = {
  // couleurs standards des Rubik's Cubes du commerce
  _RGB(232, 106, 0), // ORANGE   Right (Droite     O orange)
  _RGB(215, 0, 0),   // RED      Left  (Gauche     R rouge)
  _RGB(0, 192, 0),   // GREEN    Back  (Posterieur G vert)
  _RGB(40, 40, 255), // BLUE     Front (Avant      B bleu)
  _RGB(255, 255, 255), // WHITE    Up    (Haut       W blanc)
  _RGB(255, 255, 0)  // YELLOW   Down  (Bas        Y jaune)
};
#endif

static const BYTE sortedColors[6] = {3, 2, 1, 0, 5, 4};
static const int addr[33] = {
  2, 3, 4, 5, 6, 16, 25, 26, 27, 28, 29,
  33 + 2, 33 + 3, 33 + 4, 33 + 5, 33 + 6, 33 + 16, 33 + 25, 33 + 26, 33 + 27, 33 + 28, 33 + 29,
  66 + 2, 66 + 3, 66 + 4, 66 + 5, 66 + 6, 66 + 16, 66 + 25, 66 + 26, 66 + 27, 66 + 28, 66 + 29
};

COLOR CExtract::GetTextColor(BYTE noColor)
{
  COLOR rgb = defaultColors[noColor];
  UINT lum = 30 * BYTE(rgb) + 59 * BYTE(rgb >> 8) + 11 * BYTE(rgb >> 16);
  return (lum > 10000) ?  0x00000000 : 0x00FFFFFF ;
}

COLOR CExtract::GetTextColor(img_t* im, const IRECT* pclip, IRECT r)
{
  iRectDeflate2(&r, RCW(&r) / 4, RCH(&r) / 4);
  COLOR rgb0 = imdraw_GetPixel(im, pclip, r.l, r.t);
  COLOR rgb1 = imdraw_GetPixel(im, pclip, r.r, r.t);
  COLOR rgb2 = imdraw_GetPixel(im, pclip, r.l, r.b);
  COLOR rgb3 = imdraw_GetPixel(im, pclip, r.r, r.b);
  UINT lum = 30 * (BYTE(rgb0) + BYTE(rgb1) + BYTE(rgb2) + BYTE(rgb3))
      + 59 * (BYTE(rgb0 >> 8) + BYTE(rgb1 >> 8) + BYTE(rgb2 >> 8) + BYTE(rgb3 >> 8))
      + 11 * (BYTE(rgb0 >> 16) + BYTE(rgb1 >> 16) + BYTE(rgb2 >> 16) + BYTE(rgb3 >> 16));
  return (lum > 40000) ? rgb(0, 0, 0) : rgb(1, 1, 1);
}


static const char* errorMsg[] = {
  NULL,
  "File read error",                                                // 1
  "Image capture error",                                            // 2
  "They need the six images of faces of the Rubik's Cube",          // 3
  "The colors of faces are confused",                               // 4
  "There are more than 6 images in the file",                       // 5
  "Facets are absent or could not be identified",                   // 6
  "Counting the colors of the facets is incorrect",                 // 7
  "There is ambiguity in the identification of the facets",         // 8
  "",                                                               // 9
  "The colors of the faces (center facets) are inconsistent",       // 10
  "The colors of facets are inconsistent"                           // 11
};


int CExtract::Analyse()
{
  int ac[6], noSide, noGr, e = 0, r = 0;
  UINT seuil = 0, noFrame, noColor;
  int pos;
  DWORD test;

  if (pBitmap[5].tt.data == NULL) {
    return 0;
  }

  DeleteAll();

  // classifies different pixels depending on the color
  for (noFrame = 0; noFrame < 6; noFrame++) {
    img_t* pBm = pBitmap + noFrame;

    if (pBm == NULL) {      // error : not enough image
      return m_error = 3;
    }

    int w = pBm->w, h = pBm->h;

    for (int y = 0; y < 33; y++) {
      UINT yy = (y * h) / 33;

      for (int x = 0; x < 33; x++) {
        UINT xx = (x * w) / 33;
        COLOR color;

        color = imdraw_GetPixel(pBm, 0, xx, yy);
        DWORD c = ColorRGB(color);

        if (c > 7) {
          pixels[noFrame][y][x] = c;
          Insert(c);
        }
      }
    }
  }

  // calculate the maximum of each color
  for (max = 0, pos = 0; pos < hist_count; ++pos) {
    hist[pos].Max(max);
  }

  // grouping neighboring colors and identify colors
  noGroup = -1;
  test = 0;

  do {
    DWORD color = 0;

    if (noGroup == -1) {
      seuil = max / 3;
    }
    else {
      seuil = (noGroup < 6) ? (seuil * 5) / 6 : (seuil * 6) / 5;

      if (++r > 10 || seuil == 0) {
        return m_error = 4;  // error : color confused
      }

      noGroup = -1;
    }

    for (pos = 0; pos < hist_count; ++pos) {
      Node& element = hist[pos];

      if (element.count >= seuil) {
        if ((color + 0x0004FFFF) < element.color) {
          // new group
          Group& group = groups[++noGroup];

          if (noGroup >= GROUPSIZE) {
            break;
          }

          color = element.color;
          group.min = color - 0x0000FFFF;
          group.max = color + 0x0000FFFF;

          for (noSide = 0; noSide < 54; noSide++) {
            group.facets[noSide] = 0;
          }
        }
        else {
          color = element.color;
          groups[noGroup].max = color + 0x0000FFFF;
        }
      }
    }

    if (++noGroup > GROUPSIZE || noGroup < 6) {
      continue;
    }

    if (noGroup == 6) {
      for (noGr = 0; noGr < 6; noGr++) {
        Group& group = groups[noGr];
        noColor = sortedColors[noGr];
        group.rgb = defaultColors[noColor];
        group.noColor = BYTE(noColor);
      }

      test = 0x003F;
    }
    else {
      // identify the color groups
      ac[0] = ac[1] = ac[2] = ac[3] = ac[4] = ac[5] = 1;

      for (;;) {
        for (test = 0, noGr = 0; noGr < noGroup; noGr++) {
          Group& group = groups[noGr];

          long score = 0x000FFFFF;
          group.rgb = 0xFFFFFFFF, group.noColor = 0xFF;

          for (noColor = 0; noColor < 6; noColor++) {
            COLOR rgb = defaultColors[noColor];
            long t = ColorRGB(rgb) ^ ((group.min + group.max) / 2);
            t /= ac[noColor];

            if (t < score) {
              score = t;
              test |= (1 << noColor);
              group.rgb = rgb;
              group.noColor = BYTE(noColor);
            }
          }
        }

        if (test != 0x3F) {
          for (noColor = 0; noColor < 6; noColor++)
            if ((test & (1 << noColor)) == 0 && ++ac[noColor] > 5) {
              break;
            }
        }
        else {
          break;
        }
      }
    }

    // complete when all colors are identified
  }
  while (test != 0x3F);

  // classify the pixels according to the position and color
  for (noFrame = 0; noFrame < 6; noFrame++) {
    for (int y = 0; y < 33; y++) {
      UINT ix = noFrame * 9 + (y / 11) * 3;

      for (int x = 0; x < 33; x++) {
        Position(pixels[noFrame][y][x], ix + x / 11);
      }
    }
  }

  // define the color of the facets
  for (noSide = 0; noSide < 54; noSide++) {
    facets[noSide] = 0xFF;
    UINT score = 0, second = 0;

    for (noGr = 0; noGr < noGroup; noGr++) {
      Group& group = groups[noGr];

      if (group.noColor >= 6) {
        continue;
      }

      if (group.facets[noSide] > score) {
        score = group.facets[noSide], facets[noSide] = group.noColor;
      }
    }

    if (score == 0) {
      e++;
    }
    else {
      for (noGr = 0; noGr < noGroup; noGr++) {
        Group& group = groups[noGr];

        if (group.noColor >= 6 || group.noColor == facets[noSide]) {
          continue;
        }

        if (group.facets[noSide] > second) {
          second = group.facets[noSide];
        }
      }

      if (second > score / 2) {
        e |= 0x00010000;
      }
    }

    color1[noSide] = score, color2[noSide] = second;
  }

  for (test = 0, noSide = 4; noSide < 54; noSide += 9)
    if ((noColor = facets[noSide]) < 6) {
      test |= (1 << noColor);
    }

  if (test != 0x3F) {
    return m_error = 6;  // error : some faces are missing
  }

  // rapid test if the colors of the facets are correct
  ac[0] = ac[1] = ac[2] = ac[3] = ac[4] = ac[5] = 0;

  for (noSide = 0; noSide < 54; noSide++)
    if ((noColor = facets[noSide]) < 6) {
      ac[noColor]++;
    }

  for (noColor = 0; noColor < 6; noColor++)
    if (ac[noColor] != 9) {
      return m_error = 7;  // error : bad count of facets colors
    }

  if (e != 0) {
    m_error = 8;  // some others errors !
  }

  return 0;
}

//

void CExtract::DeleteImages()
{
  int i;

  for (i = 0; i < 54; i++) {
    facets[i] = 0xFF;
  }

  for (i = 0; i < 6; i++)
    if (pBitmap[i].tt.data) {
      imfree(pBitmap + i);
    }

  m_error = -1;
}

void CExtract::DeleteAll()
{
  int i;

  for (i = 0; i < 256; i++) {
    colors[i] = NULL;
  }

  for (i = 0; i < GROUPSIZE; i++) {
    groups[i].min = groups[i].max = 0;
  }

  for (i = 0; i < 54; i++) {
    facets[i] = 0xFF;
  }

  hist_count = 0;
  max = 0;
}

bool CExtract::AddImage(img_t* pBm)
{
  int i;

  for (i = 0; i < 6; i++)
    if (pBitmap[i].tt.data == NULL) {
      imclone2(pBm, pBitmap + i);

      if (i < 5) {
        return true;
      }
    }

  return false;
}

DWORD CExtract::ColorRGB(COLOR color) const
{
  DWORD r = BYTE(color), g = BYTE(color >> 8), b = BYTE(color >> 16);
  // DWORD R = Gamma[r], G = Gamma[g], B = Gamma[b];
  DWORD R = tconv[r], G = tconv[g], B = tconv[b];
  return (R << 2) | (G << 1) | (B);
}
/*
DWORD CExtract::ColorRGB(COLOR color) const
{
   DWORD R = BYTE(color), G = BYTE(color >> 8), B = BYTE(color >> 16);
   R = Gamma[R], G = Gamma[G], B = Gamma[B];
 // int Y = 30 * R + 59  * G + 11 * B;
   double I = 0.60 * R - 0.28 * G - 0.32 * B;
   double Q = 0.21 * R - 0.52 * G + 0.31 * B;
   double c = atan2(Q, I) / 3.141592654 * 8;
   return (c < 0) 0x01000000 + int(c): int(c);
}*/

COLOR CExtract::RColor(DWORD color) const
{
  DWORD r = 0, g = 0, b = 0;

  for (int i = 0; i < 8; i++, color <<= 3) {
    r = (r << 1) | (color & 0x00800000);
    g = (g << 1) | (color & 0x00400000);
    b = (b << 1) | (color & 0x00200000);
  }

  r >>= 23, g >>= 14, b >>= 5;
  return r | g | b;
}

void CExtract::Insert(DWORD c)
{
  Node element = { c, 1 };
  UINT idx = element.Index();
  int pos;
  int r;

  ASSERT(idx < HCODESIZE);

  if ((pos = colors[idx]) == NULL) {
    // search a valid entry in acces table
    for (UINT i = idx; ++i < HCODESIZE;) {
      if ((pos = colors[i]) == NULL) {
        continue;
      }

      // entry is found
      colors[idx] = hist.InsertBefore(pos, element);
      return;
    }

    // entry not found
    colors[idx] = hist.AddTail(element);
  }
  else if ((r = hist.GetAt(pos).Compare(element)) == 0) {
    // the found entry its a good
    hist.GetAt(pos).count++;
  }
  else if (r > 0) {
    colors[idx] = hist.InsertBefore(pos, element);
  }
  else {
    // search a element in list after the acces entry
    while (hist.GetNext(pos), pos) {
      if ((r = hist.GetAt(pos).Compare(element)) < 0) {
        continue;
      }

      if (r == 0) {
        // ok, found
        hist.GetAt(pos).count++;
      }
      else {
        // position of element in list is out
        hist.InsertBefore(pos, element);
      }

      return;
    }

    // not found in list
    hist.AddTail(element);
  }

  return;
}

void CExtract::Position(DWORD c, UINT ix)
{
  ASSERT(ix < 54);

  for (int noGr = 0; noGr < noGroup; noGr++) {
    Group& group = groups[noGr];

    if (c < group.min) {
      continue;
    }

    if (c <= group.max) {
      group.facets[ix]++;
    }
  }
}

void CExtract::CreateGammaCurve(double gamma)
{
  const double demi = 0.5;

  for (int i = 0; i < 256; ++i) {
    Gamma[i] = tconv[ UINT(demi + pow(double(i) / 255, gamma) * 255) ];
  }
}

// Sortie en ASCII de la couleur des facettes

CString CExtract::ToString(bool cor) const
{
  static const UINT r0[9] = { 0, 1, 2, 3, 4, 5, 6, 7, 8 };
  static const UINT r1[9] = { 2, 5, 8, 1, 4, 7, 0, 3, 6 };
  static const UINT r2[9] = { 8, 7, 6, 5, 4, 3, 2, 1, 0 };
  static const UINT r3[9] = { 6, 3, 0, 7, 4, 1, 8, 5, 2 };
  // correction ordre des facettes liee a la methode de saisie
  static const UINT* rotation[6] = { r2, r3, r3, r3, r0, r0 };
  CString out;

  for (int noSide = 0; noSide < 6; noSide++) {
    const UINT* pRotation = cor ? rotation[noSide] : r0;
    CString tmp;

    tmp.Format(" %c:", sidesId[noSide]);
    out += tmp;

    for (int s = 0; s < 9; s++) {
      UINT noColor = facets[noSide * 9 + pRotation[s]];
      out += (noColor < 6) ? colorsId[noColor] : char('?');
    }
  }

  out.Delete(0);
  return out;
}

// Test if all facets are filled in

bool CExtract::IsReady() const
{
  for (int i = 0; i < 54; i++)
    if (facets[i] >= 6) {
      return false;
    }

  return true;
}

bool CExtract::Check()
{
  POSITION pos = 0;
  Node* pElement = pos ? &hist.GetNext(pos) : NULL;
  Node* pTmp;

  while (pos) {
    pTmp = &hist.GetNext(pos);

    if (pElement->Compare(*pTmp) >= 0) {
      return false;
    }

    pElement = pTmp;
  }

  return true;
}

// place les indications sur sur une facette

void CExtract::SetTextId(img_t* im, const IRECT* pclip,
    int ox, int oy,  // org of the face
    int m,           // height/width of the face
    int n)           // number first facet
{
  int s = m / 3, xx, yy;
  CString text;

  pDC->SetBkMode(TRANSPARENT);

  for (int y = 0; y < 3; y++) {
    yy = oy + (m * y) / 3;

    for (int x = 0; x < 3; x++, n++) {
      BYTE noColor = facets[n];
      IRECT r(0, 0, 0, 0);

      xx = ox + (m * x) / 3;

      text.Format("%d\n%c", n + 1, colorsId[noColor]);
      pDC->DrawText(text, r, DT_CALCRECT);
      r.OffsetRect(xx + (s - r.r) / 2, yy + (s - r.b) / 2);

      COLOR color = GetTextColor(noColor);
      color = pDC->SetTextColor(color);
      pDC->DrawText(text, r, DT_CENTER);
      pDC->SetTextColor(color);
    }
  }
}

// Display a box with text information

IRECT CExtract::SetTextMessage(img_t* im, const IRECT* pclip, const CString& msg, IRECT rect, int b)
{
  CBrush* pOldBrush, brush(_RGB(200, 200, 150));
  int ox, oy, bx = b, by = b;
  IRECT rt(0, 0, 0, 0), rc;

  pDC->DrawText(msg, rt, DT_CALCRECT);

  if (b <= 0) {
    bx = 5, by = 10;
  }

  ox = (rt.Width() > (rect.Width() + 2 * bx)) ?
      ((rect.Width() < 0) ? rect.r - bx - rt.r : rect.l + bx) :
        (rect.r + rect.l - rt.r) / 2;
  oy = (rt.Height() > (rect.Height() + 2 * by)) ? rect.t + by :
      (rect.b + rect.t - rt.b) / 2;
  rt.OffsetRect(ox, oy);
  rc = rt;
  rc.InflateRect(bx, by);

  pDC->SetBkMode(TRANSPARENT);
  pOldBrush = pDC->SelectObject(&brush);
  pDC->Rectangle(rc);
  pDC->DrawText(msg, rt, DT_CENTER);
  pDC->SelectObject(pOldBrush);

  return rc;
}

// View the icon, indicating the direction of rotation

IRECT CExtract::DrawIcon(img_t* im, const IRECT* pclip, IRECT rect, const CString& fileName)
{
  Graphics graphics(pDC->m_hDC);
  USES_CONVERSION;

  img_t image(A2W(FixePath(fileName)));
  int w = image.GetWidth(), h = image.GetHeight();

  // limit the image to the available space
  if (w > rect.Width()) {
    w = rect.Width();
  }

  if (h > rect.Height()) {
    h = rect.Height();
  }

  // center the image in the available space
  int ox = (rect.l + rect.r - w) / 2;
  int oy = (rect.t + rect.b - h) / 2;

  graphics.DrawImage(&image, ox, oy, w, h);

  // returns the rectangle of the image
  return IRECT(ox, oy, ox + w, oy + h);
}

// Display images staircase, normal mode

IRECT CExtract::DrawImage(img_t* im, const IRECT* pclip, IRECT rect, bool acq)
{
  Graphics graphics(pDC->m_hDC);
  CString msg;
  IRECT er = rect, ir = rect, mr(0, 0, 0, 0);
  ISIZE size = rect.Size();

  if (m_error > 0) {
    msg.Format("*** Error *** : %s", errorMsg[m_error]);
    pDC->DrawText(msg, er, DT_CALCRECT);
    size.h -= er.Height();
    er.t = rect.b - er.Height(), er.b = rect.b;
  }

  int dx = (size.w - 15) / 4, dy = (size.h - 10) / 3;
  int dim = dx < dy ? dx : dy;
  int wi = dim * 4 + 15, hi = dim * 3 + 10;
  int oix = rect.l + (size.w - wi) / 2;
  int oiy = rect.t - (size.h - hi) / 2;

  if (!acq) {
    ;
  }
  else if (pBitmap[0] == NULL) {

    DrawIcon(pDC, rect, "Avertis.jpg");

  }
  else if (pBitmap[5] == NULL) {
    CString text = "Turn the Rubik's Cube as specified\n"
        "to capture all sides of the Cube.";

    SetTextMessage(pDC, text, IRECT(oix + 2 * dim, oiy, rect.r, oiy + dim), 20);
  }
  else {
    CString text = "Turn the Rubik's Cube again to\n"
        "reset it to its original position.";

    SetTextMessage(pDC, text, IRECT(oix + 2 * dim, oiy, rect.r, oiy + dim), 20);
    DrawIcon(pDC, IRECT(oix + 3 * dim, oiy + dim, oix + 4 * dim, oiy + 2 * dim),
        "Arrow02.jpg");
  }

  for (int i = 0, n = 0; i < 6; i++, n += 9) {
    if (pBitmap[i]) {
      // display images
      graphics.DrawImage(pBitmap[i], oix, oiy, dim, dim);

      if (pBitmap[5]) {
        SetTextId(pDC, oix, oiy, dim, n);
      }

    }
    else if (acq && i > 0) {
      char* pBmn = (i & 1) ? "Arrow01.jpg" : "Arrow02.jpg";

      DrawIcon(pDC, IRECT(oix, oiy, oix + dim, oiy + dim), pBmn);
      break;

    }
    else {
      break;
    }

    if (i & 1) {
      oiy += dim + 5;
    }
    else {
      oix += dim + 5;
    }
  }

  pDC->DrawText(msg, er, 0);

  return rect;
}

// Display inline images (debug mode)

IRECT CExtract::DrawImage(img_t* im, const IRECT* pclip, IPOINT org, int width, bool acq)
{
  int space = (width + 99) / 100, dim = (width - space * 5) / 6;
  int realWidth = 6 * dim + 5 * space, x, y, i, n;
  IRECT rect(0, 0, realWidth, dim);
  Graphics graphics(pDC->m_hDC);
  CString text;

  org.x += (width - realWidth) / 2;
  rect += org;

  for (i = n = 0, x = org.x, y = org.y; i < 6; i++, x += space + dim, n += 9) {
    if (pBitmap[i]) {
      // dislay images
      graphics.DrawImage(pBitmap[i], x, y, dim, dim);

      if (pBitmap[5]) {
        SetTextId(pDC, x, y, dim, n);
      }

    }
    else if (acq && i > 0) {
      char* pBmn = (i & 1) ? "Arrow01.jpg" : "Arrow02.jpg";

      DrawIcon(pDC, IRECT(x, y, x + dim, y + dim), pBmn);
      break;

    }
    else {
      break;
    }
  }

  return rect;
}

void CExtract::DrawDebugMsg(img_t* im, const IRECT* pclip, IRECT rect)
{
  if (pBitmap[0] == NULL) {

    DrawIcon(pDC, rect, "Avertis.jpg");
  }
  else if (pBitmap[5] == NULL) {

    CString text = "Turn the Rubik's Cube as specified\n"
        "to capture all sides of the Cube.";

    SetTextMessage(pDC, text, rect, 20);
  }
}

// Viewing statistics of calculation

void CExtract::DrawHist(img_t* im, const IRECT* pclip, const IRECT& rect, const CString& errMsg)
{
  IRECT r = rect, r0, r1(0, 0, 0, 0);
  CString msg;

  pDC->SetBkMode(TRANSPARENT);

  if (pBitmap[5]) {
    int y0 = rect.t, y1, y2, y3 = rect.b, w, h1, h2;
    UINT maxColor;

    ASSERT(Check());

    msg.Format("Number samples %d, max count %d, %d colors groups",
        hist.GetCount(), max, noGroup);

    if (m_error > 0) {
      msg += CString("\n*** error *** ") + errorMsg[m_error];
    }
    else if (!errMsg.IsEmpty()) {
      msg += CString("\n") + errMsg;
    }

    pDC->DrawText(msg, r, DT_CALCRECT);
    pDC->DrawText(msg, r, 0);
    y1 = r.b + 2, y2 = y1 + (y3 - y1) / 2;
    h1 = y2 - y1 - 15;
    h2 = y3 - y2;

    int b = y1 + h1, c = b + 30, org;
    int noSide, noColor, noGr;
    CBrush* pOldBrush, brush(_RGB(200, 200, 150));

    msg = "Count\nof pixels\nbased on\ncolor";
    r0 = SetTextMessage(pDC, msg, IRECT(rect.l, r.b, rect.l, r.b + h1 + 20));
    org = r0.r + 5, w = rect.Width() - org;

    if (max > 0) {
      POSITION pos = 0;

      while (pos) {
        Node& element = hist.GetNext(pos);
        UINT x = org + ((element.color >> 8) * w) / 0x00FFFF;
        UINT y = b - (element.count * h1) / max;
        CPen pen(PS_SOLID, 1, RColor(element.color));

        pDC->MoveTo(x, b), pDC->LineTo(x, y);

        CPen* pPen = pDC->SelectObject(&pen);
        pDC->MoveTo(x, b), pDC->LineTo(x, b + 8);
        pDC->SelectObject(pPen);
      }

      pDC->MoveTo(org, b), pDC->LineTo(rect.r, b);

      for (noGr = 0; noGr < noGroup; noGr++) {
        Group& group = groups[noGr];
        UINT x0 = org + ((group.min >> 8) * w) / 0x00FFFF;
        UINT x1 = org + ((group.max >> 8) * w) / 0x00FFFF;

        pDC->MoveTo(x0 - 1, b + 10), pDC->LineTo(x1 + 1, b + 10);
        pDC->MoveTo(x0 - 1, b + 11), pDC->LineTo(x1 + 1, b + 11);

        if (group.noColor < 6) {
          msg = colorsId[group.noColor];
          r.SetRect(x0, b - h1 / 2, 0, 0);
          pDC->DrawText(msg, r, DT_CALCRECT);
          pDC->DrawText(msg, r, 0);
        }
      }

      for (noColor = 0; noColor < 6; noColor++) {
        COLOR rgb = defaultColors[noColor];
        DWORD color = ColorRGB(rgb);
        UINT X0 = org + ((color >> 8) * w) / 0x00FFFF;
        POINT points[3] = { {X0, b + 14}, {X0 - 3, b + 20}, {X0 + 3, b + 20}};
        CBrush brush(rgb);

        pOldBrush = pDC->SelectObject(&brush);
        pDC->Polygon(points, 3);
        pDC->SelectObject(pOldBrush);
      }

      msg = "Count\nof pixels\ndepending\non the\nlocation";
      r1 = SetTextMessage(pDC, msg, IRECT(rect.r + 1, c, rect.r, c));
      w = r1.l - rect.l - 5;

      for (noSide = 0, maxColor = 0; noSide < 54; noSide++)
        if (color1[noSide] > maxColor) {
          maxColor = color1[noSide];
        }

      pDC->DrawText(CString("XX"), r, DT_CALCRECT);
      int ht = r.Height();

      for (noSide = 0; noSide < 54; noSide++) {
        int x0 = rect.l + (noSide * w) / 54;
        int x1 = rect.l + ((noSide + 1) * w) / 54;
        int x2 = (x0 + x1) / 2;
        BYTE noColor = facets[noSide];
        CBrush brush(defaultColors[noColor]);
        pOldBrush = pDC->SelectObject(&brush);

        msg = (noColor < 6) ? colorsId[noColor] : '?';
        r.SetRect(x0, c, x0, c);

        pDC->DrawText(msg, r, DT_CALCRECT);
        r.l = x0, r.r = x1;
        pDC->DrawText(msg, r, DT_CENTER);

        int y0 = c + 2 * ht + 2, y1 = rect.b - y0;
        int y2 = y0 + (y1 * color1[noSide]) / maxColor;
        int y3 = y0 + (y1 * color2[noSide]) / maxColor;

        pDC->Rectangle(x0, y0, x1, y2);
        pDC->MoveTo(x2, y0), pDC->LineTo(x2, y3);

        pDC->SelectObject(pOldBrush);
      }

      int y0 = r.b + 2, n;

      for (noSide = 4, n = 0; noSide < 54; noSide += 9, n++) {
        noColor = facets[noSide];
        int x0 = rect.l + ((noSide - 4) * w) / 54;
        int x1 = rect.l + ((noSide + 5) * w) / 54;

        r.SetRect(x0, y0, x1, y0 + ht);
        CBrush brush(defaultColors[noColor]);
        pOldBrush = pDC->SelectObject(&brush);
        COLOR color = pDC->SetTextColor(GetTextColor(noColor));
        pDC->Rectangle(r);
        pDC->DrawText(CString(sidesId[n]), r, DT_CENTER);
        pDC->SetTextColor(color);
        pDC->SelectObject(pOldBrush);
      }
    }
  }
}

// Read or write of the TIFF file

int CExtract::FileRead(const CString& pathname)
{
  static IStream*  pIStream = NULL;
  GUID pageGuid = FrameDimensionPage;
  UINT count, frameCount;
  INT width, height;
  USES_CONVERSION;

  DeleteImages();

  img_t image(A2W(pathname));
  count = image.GetFrameDimensionsCount();

  if (count == 0) {
    return m_error = 1;
  }

  GUID* pDimensionIDs = (GUID*)malloc(sizeof(GUID) * count);
  width = image.GetWidth(), height = image.GetHeight();

  // Get the list of frame dimensions from the Image object.
  image.GetFrameDimensionsList(pDimensionIDs, count);

  // Get the number of frames in the first dimension.
  frameCount = image.GetFrameCount(&pDimensionIDs[0]);

  if (frameCount > 6) {
    frameCount = 6, m_error = 5;
  }
  else if (frameCount < 6) {
    m_error = 3;
  }

  // Display the GUID of the first (and only) frame dimension.
  WCHAR strGuid[39];
  StringFromGUID2(pDimensionIDs[0], strGuid, 39);
  free(pDimensionIDs);
  CLSID  encoderClsid;
  GetEncoderClsid(L"image/bmp", &encoderClsid);

  for (UINT fc = 0; fc < frameCount; fc++) {

    image.SelectActiveFrame(&pageGuid, fc);
    ::CreateStreamOnHGlobal(NULL, TRUE, &pIStream);
    image.Save(pIStream, &encoderClsid, NULL);

    pBitmap[fc] = img_t::FromStream(pIStream);

    if (pIStream) {
      pIStream->Release();
    }
  }

  return 0;
}

BOOL CExtract::FileWrite(const CString& pathname)
{
  EncoderParameters encoderParameters;
  Image* pImage = pBitmap[0];
  ULONG parameterValue;
  USES_CONVERSION;
  Status stat;
  int i;

  if (pImage == NULL) {
    return FALSE;
  }

  // An EncoderParameters object has an array of EncoderParameter
  // objects. In this case, there is only one EncoderParameter
  // object in the array.
  encoderParameters.Count = 1;

  // Initialize the one EncoderParameter object.
  encoderParameters.Parameter[0].Guid = EncoderSaveFlag;
  encoderParameters.Parameter[0].Type = EncoderParameterValueTypeLong;
  encoderParameters.Parameter[0].NumberOfValues = 1;
  encoderParameters.Parameter[0].Value = &parameterValue;

  // Get the CLSID of the TIFF encoder.
  CLSID encoderClsid;
  GetEncoderClsid(L"image/tiff", &encoderClsid);

  // Save the first page (frame).
  parameterValue = EncoderValueMultiFrame;
  stat = pImage->Save(A2W(pathname), &encoderClsid, &encoderParameters);

  if (stat != Ok) {
    return FALSE;
  }

  for (i = 1; i < 6; i++) {
    if (pBitmap[i]) {
      // Save the second page (frame).
      parameterValue = EncoderValueFrameDimensionPage;
      stat = pImage->SaveAdd(pBitmap[i], &encoderParameters);

      if (stat != Ok) {
        return FALSE;
      }
    }
  }

  // Close the multiframe file.
  parameterValue = EncoderValueFlush;
  stat = pImage->SaveAdd(&encoderParameters);

  if (stat == Ok) {
    return TRUE;
  }

  return FALSE;
}
