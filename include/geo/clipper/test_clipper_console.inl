
#ifdef _MSC_VER
#define _CRT_SECURE_NO_WARNINGS
#endif

const char* svg_xml_start [] = {
  "<?xml version=\"1.0\" standalone=\"no\"?>\n"
    "<!DOCTYPE svg PUBLIC \"-//W3C//DTD SVG 1.0//EN\"\n"
    "\"http://www.w3.org/TR/2001/REC-SVG-20010904/DTD/svg10.dtd\">\n\n"
    "<svg width=\"",
    "\" height=\"",
    "\" viewBox=\"0 0 ",
    "\" version=\"1.0\" xmlns=\"http://www.w3.org/2000/svg\">\n\n"
};
const char* poly_end [] = {
  "\"\n style=\"fill:",
    "; fill-opacity:",
    "; fill-rule:",
    "; stroke:",
    "; stroke-opacity:",
    "; stroke-width:",
    ";\"/>\n\n"
};

// SVGBuilder class
// a very simple class that creates an SVG image file
class SVGBuilder
{
  static char* ColorToHtml(unsigned clr, char* ss, int len) {
    snprintf(ss, len, "#%06x", (clr & 0xFFFFFF));
    return ss;
  }
  static float GetAlphaAsFrac(unsigned clr) {
    return ((float)(clr >> 24) / 255);
  }
  class StyleInfo
  {
  public:
    PolyFillType pft;
    unsigned brushClr;
    unsigned penClr;
    double penWidth;
    bool showCoords;
    StyleInfo() {
      pft = pftNonZero;
      brushClr = 0xFFFFFFCC;
      penClr = 0xFF000000;
      penWidth = 0.8;
      showCoords = false;
    }
  };
  class PolyInfo
  {
  public:
    Paths paths;
    StyleInfo si;
    PolyInfo(Paths paths, StyleInfo style) {
      this->paths = paths;
      this->si = style;
    }
  };
  typedef std::vector<PolyInfo> PolyInfoList;
private:
  PolyInfoList polyInfos;
public:
  StyleInfo style;
  void AddPaths(Paths& poly) {
    if (poly.size() == 0) {
      return;
    }
    polyInfos.push_back(PolyInfo(poly, style));
  }
  bool SaveToFile(const char* filename, double scale = 1.0, int margin = 10) {
    //calculate the bounding rect ...
    PolyInfoList::size_type i = 0;
    Paths::size_type j, k;
    while (i < polyInfos.size()) {
      j = 0;
      while (j < polyInfos[i].paths.size() &&
          polyInfos[i].paths[j].size() == 0) {
        j++;
      }
      if (j < polyInfos[i].paths.size()) {
        break;
      }
      i++;
    }
    if (i == polyInfos.size()) {
      return false;
    }
    IntRect rec;
    rec.left = polyInfos[i].paths[j][0].X;
    rec.right = rec.left;
    rec.top = polyInfos[i].paths[j][0].Y;
    rec.bottom = rec.top;
    for (; i < polyInfos.size(); ++i) {
      for (j = 0; j < polyInfos[i].paths.size(); ++j) {
        for (k = 0; k < polyInfos[i].paths[j].size(); ++k) {
          IntPoint ip = polyInfos[i].paths[j][k];
          if (ip.X < rec.left) {
            rec.left = ip.X;
          }
          else if (ip.X > rec.right) {
            rec.right = ip.X;
          }
          if (ip.Y < rec.top) {
            rec.top = ip.Y;
          }
          else if (ip.Y > rec.bottom) {
            rec.bottom = ip.Y;
          }
        }
      }
    }
    if (scale == 0) {
      scale = 1.0;
    }
    if (margin < 0) {
      margin = 0;
    }
    rec.left = (cInt)((double)rec.left * scale);
    rec.top = (cInt)((double)rec.top * scale);
    rec.right = (cInt)((double)rec.right * scale);
    rec.bottom = (cInt)((double)rec.bottom * scale);
    cInt offsetX = -rec.left + margin;
    cInt offsetY = -rec.top + margin;
    FILE* file;
    file = fopen(filename, "wb");
    if (!file) {
      return false;
    }
    fprintf(file, svg_xml_start[0]);
    fprintf(file, "%dpx%s", (rec.right - rec.left) + margin * 2, svg_xml_start[1]);
    fprintf(file, "%dpx%s", (rec.bottom - rec.top) + margin * 2, svg_xml_start[2]);
    fprintf(file, "%d %d %s", ((rec.right - rec.left) + margin * 2), ((rec.bottom - rec.top) + margin * 2), svg_xml_start[3]);
    setlocale(LC_NUMERIC, "C");
    for (i = 0; i < polyInfos.size(); ++i) {
      fprintf(file, " <path d=\"");
      for (Paths::size_type j = 0; j < polyInfos[i].paths.size(); ++j) {
        if (polyInfos[i].paths[j].size() < 3) {
          continue;
        }
        fprintf(file, " M %f %f", ((double)polyInfos[i].paths[j][0].X * scale + offsetX),
            ((double)polyInfos[i].paths[j][0].Y * scale + offsetY) );
        for (IPath::size_type k = 1; k < polyInfos[i].paths[j].size(); ++k) {
          IntPoint ip = polyInfos[i].paths[j][k];
          double x = (double)ip.X * scale;
          double y = (double)ip.Y * scale;
          fprintf(file, " L %f %f", (x + offsetX), (y + offsetY) );
        }
        fprintf(file, " z");
      }
      char buf[256];
      fprintf(file, poly_end[0]);
      fprintf(file, "%f", ColorToHtml(polyInfos[i].si.brushClr, buf, countof(buf)));
      fprintf(file, "%s%f", poly_end[1], GetAlphaAsFrac(polyInfos[i].si.brushClr));
      fprintf(file, poly_end[2]);
      fprintf(file, polyInfos[i].si.pft == pftEvenOdd ? "evenodd" : "nonzero");
      fprintf(file, poly_end[3]);
      fprintf(file, ColorToHtml(polyInfos[i].si.penClr, buf, countof(buf)));
      fprintf(file, poly_end[4]);
      fprintf(file, "%f", GetAlphaAsFrac(polyInfos[i].si.penClr));
      fprintf(file, poly_end[5]);
      fprintf(file, "%f", polyInfos[i].si.penWidth);
      fprintf(file, poly_end[6]);
      if (polyInfos[i].si.showCoords) {
        fprintf(file, "<g font-family=\"Verdana\" font-size=\"11\" fill=\"black\">\n\n");
        for (Paths::size_type j = 0; j < polyInfos[i].paths.size(); ++j) {
          if (polyInfos[i].paths[j].size() < 3) {
            continue;
          }
          for (IPath::size_type k = 0; k < polyInfos[i].paths[j].size(); ++k) {
            IntPoint ip = polyInfos[i].paths[j][k];
            fprintf(file, "<text x=\"%d\" y=\"%d\">,</text>\n\n", (int)(ip.X * scale + offsetX),
                (int)(ip.Y * scale + offsetY), ip.X, ip.Y);
          }
        }
        fprintf(file, "</g>\n");
      }
    }
    fprintf(file, "</svg>\n");
    fclose(file);
    setlocale(LC_NUMERIC, "");
    return true;
  }
}; //SVGBuilder

// Miscellaneous function ...
bool SaveToFile(const char* filename, Paths& ppg, double scale = 1.0, unsigned decimal_places = 0)
{
  FILE* ofs = fopen(filename, "wb");
  if (!ofs) {
    return false;
  }
  if (decimal_places > 8) {
    decimal_places = 8;
  }
  for (size_t i = 0; i < ppg.size(); ++i) {
    for (size_t j = 0; j < ppg[i].size(); ++j) {
      fprintf(ofs, "%f, %f,\n", ppg[i][j].X / scale, ppg[i][j].Y / scale);
    }
    fprintf(ofs, "\n");
  }
  fclose(ofs);
  return true;
}
bool LoadFromFile(Paths& ppg, const char* filename, double scale)
{
  //file format assumes:
  // 1. path coordinates (x,y) are comma separated (+/- spaces) and
  // each coordinate is on a separate line
  // 2. each path is separated by one or more blank lines
  ppg.clear();
  FILE* ifs = fopen(filename, "rb");
  if (!ifs) {
    return false;
  }
  char line[1024];
  IPath pg;
  while (fgets(line, countof(line), ifs)>0) {
    double X = 0.0, Y = 0.0;
    sscanf(line, "%f, %f", &X, &Y);
    pg.push_back(IntPoint((cInt)(X * scale), (cInt)(Y * scale)));
  }
  if (pg.size() > 0) {
    ppg.push_back(pg);
  }
  fclose(ifs);
  return true;
}
void MakeRandomPoly(int edgeCount, int width, int height, Paths& poly)
{
  poly.resize(1);
  poly[0].resize(edgeCount);
  for (int i = 0; i < edgeCount; i++) {
    poly[0][i].X = rand() % width;
    poly[0][i].Y = rand() % height;
  }
}
bool ASCII_icompare(const char* str1, const char* str2)
{
  //case insensitive compare for ASCII chars only
  while (*str1) {
    if (toupper(*str1) != toupper(*str2)) {
      return false;
    }
    str1++;
    str2++;
  }
  return (!*str2);
}
// Main entry point ...
int test_clipper_console(int argc, char* argv[])
{
  if (1 || (argc > 1 && (strcmp(argv[1], "-b") == 0 || strcmp(argv[1], "--benchmark") == 0))) {
    //do a benchmark test that creates a subject and a clip polygon both with
    //100 vertices randomly placed in a 400 * 400 space. Then perform an
    //intersection operation based on even-odd filling. Repeat all this X times.
    int loop_cnt = 1000;
    char* dummy;
    if (argc > 2) {
      loop_cnt = strtol(argv[2], &dummy, 10);
    }
    if (loop_cnt == 0) {
      loop_cnt = 1000;
    }
    printf("\nPerforming %d random intersection operations ... ", loop_cnt);
    srand((int)time(0));
    int error_cnt = 0;
    Paths subject, clip, solution;
    Clipper clpr;
    time_t time_start = clock();
    for (int i = 0; i < loop_cnt; i++) {
      MakeRandomPoly(100, 400, 400, subject);
      MakeRandomPoly(100, 400, 400, clip);
      clpr.Clear();
      clpr.AddPaths(subject, ptSubject, true);
      clpr.AddPaths(clip, ptClip, true);
      if (!clpr.Execute(ctIntersection, solution, pftEvenOdd, pftEvenOdd)) {
        error_cnt++;
      }
    }
    double time_elapsed = double(clock() - time_start) / CLOCKS_PER_SEC;
    printf("\nFinished in %f secs with %d errors.\n\n", time_elapsed, error_cnt);
    //let's save the very last result ...
    SaveToFile("Subject.txt", subject);
    SaveToFile("Clip.txt", clip);
    SaveToFile("Solution.txt", solution);
    //and see the final clipping op as an image too ...
    SVGBuilder svg;
    svg.style.penWidth = 0.8;
    svg.style.pft = pftEvenOdd;
    svg.style.brushClr = 0x1200009C;
    svg.style.penClr = 0xCCD3D3DA;
    svg.AddPaths(subject);
    svg.style.brushClr = 0x129C0000;
    svg.style.penClr = 0xCCFFA07A;
    svg.AddPaths(clip);
    svg.style.brushClr = 0x6080ff9C;
    svg.style.penClr = 0xFF003300;
    svg.style.pft = pftNonZero;
    svg.AddPaths(solution);
    svg.SaveToFile("solution.svg");
    return 0;
  }
  if (argc < 3) {
    const char* s_usage = "\nUsage:\n"
         " clipper_console_demo S_FILE C_FILE CT [S_FILL C_FILL] [PRECISION] [SVG_SCALE]\n"
         "or\n"
         " clipper_console_demo --benchmark [LOOP_COUNT]\n\n"
         "Legend: [optional parameters in square braces]; {comments in curly braces}\n\n"
         "Parameters:\n"
         " S_FILE & C_FILE are the subject and clip input files (see format below)\n"
         " CT: cliptype, either INTERSECTION or UNION or DIFFERENCE or XOR\n"
         " SUBJECT_FILL & CLIP_FILL: either EVENODD or NONZERO. Default: NONZERO\n"
         " PRECISION (in decimal places) for input data. Default = 0\n"
         " SVG_SCALE: scale of the output svg image. Default = 1.0\n"
         " LOOP_COUNT is the number of random clipping operations. Default = 1000\n\n"
         "\nFile format for input and output files:\n"
         " X, Y[,] {first vertex of first path}\n"
         " X, Y[,] {next vertex of first path}\n"
         " {etc.}\n"
         " X, Y[,] {last vertex of first path}\n"
         " {blank line(s) between paths}\n"
         " X, Y[,] {first vertex of second path}\n"
         " X, Y[,] {next vertex of second path}\n"
         " {etc.}\n\n"
         "Examples:\n"
         " clipper_console_demo \"subj.txt\" \"clip.txt\" INTERSECTION EVENODD EVENODD\n"
         " clipper_console_demo --benchmark 1000\n";
    printf(s_usage);
    return 1;
  }
  int scale_log10 = 0;
  char* dummy;
  if (argc > 6) {
    scale_log10 = strtol(argv[6], &dummy, 10);
  }
  double scale = pow(double(10), scale_log10);
  double svg_scale = 1.0;
  if (argc > 7) {
    svg_scale = strtod(argv[7], &dummy);
  }
  svg_scale /= scale;
  Paths subject, clip;
  if (!LoadFromFile(subject, argv[1], scale)) {
    printf("\nCan't open the file %s or the file format is invalid.\n", argv[1]);
    return 1;
  }
  if (!LoadFromFile(clip, argv[2], scale)) {
    printf("\nCan't open the file %s or the file format is invalid.\n", argv[2]);
    return 1;
  }
  ClipType clipType = ctIntersection;
  const char* sClipType[] = {"INTERSECTION", "UNION", "DIFFERENCE", "XOR"};
  if (argc > 3) {
    if (ASCII_icompare(argv[3], "XOR")) {
      clipType = ctXor;
    }
    else if (ASCII_icompare(argv[3], "UNION")) {
      clipType = ctUnion;
    }
    else if (ASCII_icompare(argv[3], "DIFFERENCE")) {
      clipType = ctDifference;
    }
    else {
      clipType = ctIntersection;
    }
  }
  PolyFillType subj_pft = pftNonZero, clip_pft = pftNonZero;
  if (argc > 5) {
    if (ASCII_icompare(argv[4], "EVENODD")) {
      subj_pft = pftEvenOdd;
    }
    if (ASCII_icompare(argv[5], "EVENODD")) {
      clip_pft = pftEvenOdd;
    }
  }
  Clipper c;
  c.AddPaths(subject, ptSubject, true);
  c.AddPaths(clip, ptClip, true);
  Paths solution;
  if (!c.Execute(clipType, solution, subj_pft, clip_pft)) {
    printf("%s failed!\n\n", sClipType[clipType]);
    return 1;
  }
  printf("\nFinished!\n\n");
  SaveToFile("solution.txt", solution, scale);
  //let's see the result too ...
  SVGBuilder svg;
  svg.style.penWidth = 0.8;
  svg.style.brushClr = 0x1200009C;
  svg.style.penClr = 0xCCD3D3DA;
  svg.style.pft = subj_pft;
  svg.AddPaths(subject);
  svg.style.brushClr = 0x129C0000;
  svg.style.penClr = 0xCCFFA07A;
  svg.style.pft = clip_pft;
  svg.AddPaths(clip);
  svg.style.brushClr = 0x6080ff9C;
  svg.style.penClr = 0xFF003300;
  svg.style.pft = pftNonZero;
  svg.AddPaths(solution);
  svg.SaveToFile("solution.svg", svg_scale);
  //finally, show the svg image in the default viewing application
  system("solution.svg");
  return 0;
}