#ifndef _CTRL_DRAW_INL_
#define _CTRL_DRAW_INL_

typedef struct {
  float Alpha; // Global alpha applies to everything in ImGui
  IPOINT WindowPadding; // Padding within a window
  IPOINT WindowMinSize; // Minimum window size
  int WindowRounding; // Radius of window corners rounding. Set to 0.0f to have rectangular windows
  ImVec2 WindowTitleAlign; // Alignment for title bar text. Defaults to (0.0f,0.5f) for left-aligned,vertically centered.
  int ChildWindowRounding; // Radius of child window corners rounding. Set to 0.0f to have rectangular windows
  IPOINT FramePadding; // Padding within a framed rectangle (used by most widgets)
  int FrameRounding; // Radius of frame corners rounding. Set to 0.0f to have rectangular frame (used by most widgets).
  IPOINT ItemSpacing; // Horizontal and vertical spacing between widgets/lines
  IPOINT ItemInnerSpacing; // Horizontal and vertical spacing between within elements of a composed widget (e.g. a slider and its label)
  IPOINT TouchExtraPadding; // Expand reactive bounding box for touch-based system where touch position is not accurate enough. Unfortunately we don't sort widgets so priority on overlap will always be given to the first widget. So don't grow this too much!
  int IndentSpacing; // Horizontal indentation when e.g. entering a tree node. Generally == (FontSize + FramePadding.x*2).
  int ColumnsMinSpacing; // Minimum horizontal spacing between two columns
  int ScrollbarSize; // Width of the vertical scrollbar, Height of the horizontal scrollbar
  int ScrollbarRounding; // Radius of grab corners for scrollbar
  int GrabMinSize; // Minimum width/height of a grab box for slider/scrollbar.
  int GrabRounding; // Radius of grabs corners rounding. Set to 0.0f to have rectangular slider grabs.
  ImVec2 ButtonTextAlign; // Alignment of button text when button is larger than text. Defaults to (0.5f,0.5f) for horizontally+vertically centered.
  IPOINT DisplayWindowPadding; // Window positions are clamped to be visible within the display area by at least this amount. Only covers regular windows.
  IPOINT DisplaySafeAreaPadding; // If you cannot see the edge of your screen (e.g. on a TV) increase the safe area padding. Covers popups/tooltips as well regular windows.
  bool AntiAliasedLines; // Enable anti-aliasing on lines/borders. Disable if you are really tight on CPU/GPU.
  bool AntiAliasedShapes; // Enable anti-aliasing on filled shapes (rounded rectangles, circles, etc.)
  float CurveTessellationTol; // Tessellation tolerance. Decrease for highly tessellated curves (higher quality, more polygons), increase to reduce quality.
  ImColor Colors[ImGuiCol_COUNT];
  int Flags;
} ImGuiStyle;
static ImGuiStyle* GetImGuiStyle()
{
  static ImGuiStyle g_def_style[1] = {0};
  if (!g_def_style->Colors[0]) {
    ImGuiStyle* g = g_def_style;
    ImColor* Colors = g->Colors;
    g->Alpha = 1.0f; // Global alpha applies to everything in ImGui
    g->WindowPadding = iPOINT(8, 8); // Padding within a window
    g->WindowMinSize = iPOINT(32, 32); // Minimum window size
    g->WindowRounding = 9; // Radius of window corners rounding. Set to 0.0f to have rectangular windows
    g->WindowTitleAlign = fVec2(0.0f, 0.5f); // Alignment for title bar text
    g->ChildWindowRounding = 0.0f; // Radius of child window corners rounding. Set to 0.0f to have rectangular child windows
    g->FramePadding = iPOINT(4, 3); // Padding within a framed rectangle (used by most widgets)
    g->FrameRounding = 0.0f; // Radius of frame corners rounding. Set to 0.0f to have rectangular frames (used by most widgets).
    g->ItemSpacing = iPOINT(8, 4); // Horizontal and vertical spacing between widgets/lines
    g->ItemInnerSpacing = iPOINT(4, 4); // Horizontal and vertical spacing between within elements of a composed widget (e.g. a slider and its label)
    g->TouchExtraPadding = iPOINT(0, 0); // Expand reactive bounding box for touch-based system where touch position is not accurate enough. Unfortunately we don't sort widgets so priority on overlap will always be given to the first widget. So don't grow this too much!
    g->IndentSpacing = 21; // Horizontal spacing when e.g. entering a tree node. Generally == (FontSize + FramePadding.x*2).
    g->ColumnsMinSpacing = 6; // Minimum horizontal spacing between two columns
    g->ScrollbarSize = 16; // Width of the vertical scrollbar, Height of the horizontal scrollbar
    g->ScrollbarRounding = 9; // Radius of grab corners rounding for scrollbar
    g->GrabMinSize = 10; // Minimum width/height of a grab box for slider/scrollbar
    g->GrabRounding = 0; // Radius of grabs corners rounding. Set to 0.0f to have rectangular slider grabs.
    g->ButtonTextAlign = fVec2(0.5f, 0.5f); // Alignment of button text when button is larger than text.
    g->DisplayWindowPadding = iPOINT(22, 22); // Window positions are clamped to be visible within the display area by at least this amount. Only covers regular windows.
    g->DisplaySafeAreaPadding = iPOINT(4, 4); // If you cannot see the edge of your screen (e.g. on a TV) increase the safe area padding. Covers popups/tooltips as well regular windows.
    g->AntiAliasedLines = true; // Enable anti-aliasing on lines/borders. Disable if you are really short on CPU/GPU.
    g->AntiAliasedShapes = true; // Enable anti-aliasing on filled shapes (rounded rectangles, circles, etc.)
    g->CurveTessellationTol = 1.25f; // Tessellation tolerance. Decrease for highly tessellated curves (higher quality, more polygons), increase to reduce quality.
#define COLORDEF(id, r, g, b, a) Colors[id] = _rgba(r, g, b, a);
    COLORDEF_DEF(COLORDEF)
#undef COLORDEF
  }
  return g_def_style;
}
#endif // _CTRL_DRAW_INL_
