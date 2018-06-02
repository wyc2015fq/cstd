#include "cstd.h"
// dear imgui, v1.50 WIP
// (demo code)
// Don't remove this file from your project! It is useful reference code that you can execute.
// You can call ImGui_ShowTestWindow() in your code to learn about various features of ImGui.
// Everything in this file will be stripped out by the linker if you don't call ImGui_ShowTestWindow().
#if defined(_MSC_VER) && !defined(_CRT_SECURE_NO_WARNINGS)
#define _CRT_SECURE_NO_WARNINGS
#endif
#ifdef _WIN32
#undef WINVER
#undef _WIN32_WINNT
#undef _WIN32_IE
#define WINVER 0x0500
#define _WIN32_WINNT 0x0501
#define _WIN32_IE 0x0501
#endif
#define vsnprintf _vsnprintf
typedef int intptr_t;
typedef __int64 int64_t;
#include "imgui.h"
#include "imgui.inl"
#include "imgui_draw.inl"
#include <ctype.h> // toupper, isprint
#include <math.h> // sqrtf, powf, cosf, sinf, floorf, ceilf
#include <stdio.h> // vsnprintf, sscanf, printf
#include <stdlib.h> // NULL, malloc, free, qsort, atoi
#if defined(_MSC_VER) && _MSC_VER <= 1500 // MSVC 2008 or earlier
#include <stddef.h> // intptr_t
#else
#include <stdint.h> // intptr_t
#endif
#ifdef _MSC_VER
#pragma warning (disable: 4996) // 'This function or variable may be unsafe': strcpy, strdup, sprintf, vsnprintf, sscanf, fopen
#define snprintf _snprintf
#endif
#ifdef __clang__
#pragma clang diagnostic ignored "-Wold-style-cast" // warning : use of old-style cast // yes, they are more terse.
#pragma clang diagnostic ignored "-Wdeprecated-declarations" // warning : 'xx' is deprecated: The POSIX name for this item.. // for strdup used in demo code (so user can copy & paste the code)
#pragma clang diagnostic ignored "-Wint-to-void-pointer-cast" // warning : cast to 'void *' from smaller integer type 'int'
#pragma clang diagnostic ignored "-Wformat-security" // warning : warning: format string is not a string literal
#pragma clang diagnostic ignored "-Wexit-time-destructors" // warning : declaration requires an exit-time destructor // exit-time destruction order is undefined. if MemFree() leads to users code that has been disabled before exit it might cause problems. ImGui coding style welcomes static/globals.
#pragma clang diagnostic ignored "-Wreserved-id-macro" // warning : macro name is a reserved identifier // 
#elif defined(__GNUC__)
#pragma GCC diagnostic ignored "-Wint-to-pointer-cast" // warning: cast to pointer from integer of different size
#pragma GCC diagnostic ignored "-Wformat-security" // warning : format string is not a string literal (potentially insecure)
#pragma GCC diagnostic ignored "-Wdouble-promotion" // warning: implicit conversion from 'float' to 'double' when passing argument to function
#pragma GCC diagnostic ignored "-Wconversion" // warning: conversion to 'xxxx' from 'xxxx' may alter its value
#endif
// Play it nice with Windows users. Notepad in 2015 still doesn't display text data with Unix-style \n.
#ifdef _WIN32
#define IM_NEWLINE "\r\n"
#else
#define IM_NEWLINE "\n"
#endif
#define IM_ARRAYSIZE(_ARR) ((int)(sizeof(_ARR)/sizeof(*_ARR)))
#define IM_MAX(_A,_B) (((_A) >= (_B)) ? (_A) : (_B))
//-----------------------------------------------------------------------------
// DEMO CODE
//-----------------------------------------------------------------------------
#ifndef IMGUI_DISABLE_TEST_WINDOWS
static void ShowExampleAppConsole(bool* p_open);
static void ShowExampleAppLog(bool* p_open);
static void ShowExampleAppLayout(bool* p_open);
static void ShowExampleAppPropertyEditor(bool* p_open);
static void ShowExampleAppLongText(bool* p_open);
static void ShowExampleAppAutoResize(bool* p_open);
static void ShowExampleAppConstrainedResize(bool* p_open);
static void ShowExampleAppFixedOverlay(bool* p_open);
static void ShowExampleAppManipulatingWindowTitle(bool* p_open);
static void ShowExampleAppCustomRendering(bool* p_open);
static void ShowExampleAppMainMenuBar();
static void ShowExampleMenuFile();
static void ShowHelpMarker(const char* desc)
{
  ImGui_TextDisabled("(?)");
  if (ImGui_IsItemHovered()) {
    ImGui_BeginTooltip();
    ImGui_PushTextWrapPos(450.0f);
    ImGui_TextUnformatted(desc, NULL);
    ImGui_PopTextWrapPos();
    ImGui_EndTooltip();
  }
}
void ImGui_ShowUserGuide()
{
  ImGui_BulletText("Double-click on title bar to collapse window.");
  ImGui_BulletText("Click and drag on lower right corner to resize window.");
  ImGui_BulletText("Click and drag on any empty space to move window.");
  ImGui_BulletText("Mouse Wheel to scroll.");
  if (ImGui_GetIO()->FontAllowUserScaling) {
    ImGui_BulletText("CTRL+Mouse Wheel to zoom window contents.");
  }
  ImGui_BulletText("TAB/SHIFT+TAB to cycle through keyboard editable fields.");
  ImGui_BulletText("CTRL+Click on a slider or drag box to input text.");
  ImGui_BulletText(
      "While editing text:\n"
      "- Hold SHIFT or use mouse to select text\n"
      "- CTRL+Left/Right to word jump\n"
      "- CTRL+A or double-click to select all\n"
      "- CTRL+X,CTRL+C,CTRL+V clipboard\n"
      "- CTRL+Z,CTRL+Y undo/redo\n"
      "- ESCAPE to revert\n"
      "- You can apply arithmetic operators +,*,/ on numerical values.\n"
      " Use +- to subtract.\n");
}
// Demonstrate most ImGui features (big function!)
void ImGui_ShowTestWindow(bool* p_open)
{
  int i;
  // Examples apps
  static bool show_app_main_menu_bar = false;
  static bool show_app_console = false;
  static bool show_app_log = false;
  static bool show_app_layout = false;
  static bool show_app_property_editor = false;
  static bool show_app_long_text = false;
  static bool show_app_auto_resize = false;
  static bool show_app_constrained_resize = false;
  static bool show_app_fixed_overlay = false;
  static bool show_app_manipulating_window_title = false;
  static bool show_app_custom_rendering = false;
  static bool show_app_style_editor = false;
  static bool show_app_metrics = false;
  static bool show_app_about = false;
  if (show_app_main_menu_bar) {
    ShowExampleAppMainMenuBar();
  }
  if (show_app_console) {
    ShowExampleAppConsole(&show_app_console);
  }
  if (show_app_log) {
    ShowExampleAppLog(&show_app_log);
  }
  if (show_app_layout) {
    ShowExampleAppLayout(&show_app_layout);
  }
  if (show_app_property_editor) {
    ShowExampleAppPropertyEditor(&show_app_property_editor);
  }
  if (show_app_long_text) {
    ShowExampleAppLongText(&show_app_long_text);
  }
  if (show_app_auto_resize) {
    ShowExampleAppAutoResize(&show_app_auto_resize);
  }
  if (show_app_constrained_resize) {
    ShowExampleAppConstrainedResize(&show_app_constrained_resize);
  }
  if (show_app_fixed_overlay) {
    ShowExampleAppFixedOverlay(&show_app_fixed_overlay);
  }
  if (show_app_manipulating_window_title) {
    ShowExampleAppManipulatingWindowTitle(&show_app_manipulating_window_title);
  }
  if (show_app_custom_rendering) {
    ShowExampleAppCustomRendering(&show_app_custom_rendering);
  }
  if (show_app_metrics) {
    ImGui_ShowMetricsWindow(&show_app_metrics);
  }
  if (show_app_style_editor) {
    ImGui_Begin("Style Editor", &show_app_style_editor, 0);
    ImGui_ShowStyleEditor(NULL);
    ImGui_End();
  }
  if (show_app_about) {
    ImGui_Begin("About ImGui", &show_app_about, ImGuiWindowFlags_AlwaysAutoResize);
    ImGui_Text("dear imgui, %s", ImGui_GetVersion());
    ImGui_Separator();
    ImGui_Text("By Omar Cornut and all github contributors.");
    ImGui_Text("ImGui is licensed under the MIT License, see LICENSE for more information.");
    ImGui_End();
  }
  static bool no_titlebar = false;
  static bool no_border = true;
  static bool no_resize = false;
  static bool no_move = false;
  static bool no_scrollbar = false;
  static bool no_collapse = false;
  static bool no_menu = false;
  // Demonstrate the various window flags. Typically you would just use the default.
  ImGuiWindowFlags window_flags = 0;
  if (no_titlebar) {
    window_flags |= ImGuiWindowFlags_NoTitleBar;
  }
  if (!no_border) {
    window_flags |= ImGuiWindowFlags_ShowBorders;
  }
  if (no_resize) {
    window_flags |= ImGuiWindowFlags_NoResize;
  }
  if (no_move) {
    window_flags |= ImGuiWindowFlags_NoMove;
  }
  if (no_scrollbar) {
    window_flags |= ImGuiWindowFlags_NoScrollbar;
  }
  if (no_collapse) {
    window_flags |= ImGuiWindowFlags_NoCollapse;
  }
  if (!no_menu) {
    window_flags |= ImGuiWindowFlags_MenuBar;
  }
  ImGui_SetNextWindowSize(fVec2(550, 680), ImGuiSetCond_FirstUseEver);
  if (!ImGui_Begin("ImGui Demo", p_open, window_flags)) {
    // Early out if the window is collapsed, as an optimization.
    ImGui_End();
    return;
  }
  //ImGui_PushItemWidth(ImGui_GetWindowWidth() * 0.65f); // 2/3 of the space for widget and 1/3 for labels
  ImGui_PushItemWidth(-140); // Right align, keep 140 pixels for labels
  ImGui_Text("Dear ImGui says hello.");
  // Menu
  if (ImGui_BeginMenuBar()) {
    if (ImGui_BeginMenu("Menu")) {
      ShowExampleMenuFile();
      ImGui_EndMenu();
    }
    if (ImGui_BeginMenu("Examples")) {
      ImGui_MenuItem("Main menu bar", NULL, &show_app_main_menu_bar);
      ImGui_MenuItem("Console", NULL, &show_app_console);
      ImGui_MenuItem("Log", NULL, &show_app_log);
      ImGui_MenuItem("Simple layout", NULL, &show_app_layout);
      ImGui_MenuItem("Property editor", NULL, &show_app_property_editor);
      ImGui_MenuItem("Long text display", NULL, &show_app_long_text);
      ImGui_MenuItem("Auto-resizing window", NULL, &show_app_auto_resize);
      ImGui_MenuItem("Constrained-resizing window", NULL, &show_app_constrained_resize);
      ImGui_MenuItem("Simple overlay", NULL, &show_app_fixed_overlay);
      ImGui_MenuItem("Manipulating window title", NULL, &show_app_manipulating_window_title);
      ImGui_MenuItem("Custom rendering", NULL, &show_app_custom_rendering);
      ImGui_EndMenu();
    }
    if (ImGui_BeginMenu("Help")) {
      ImGui_MenuItem("Metrics", NULL, &show_app_metrics);
      ImGui_MenuItem("Style Editor", NULL, &show_app_style_editor);
      ImGui_MenuItem("About ImGui", NULL, &show_app_about);
      ImGui_EndMenu();
    }
    ImGui_EndMenuBar();
  }
  ImGui_Spacing();
  if (ImGui_CollapsingHeader("Help")) {
    ImGui_TextWrapped("This window is being created by the ShowTestWindow() function. Please refer to the code for programming reference.\n\nUser Guide:");
    ImGui_ShowUserGuide();
  }
  if (ImGui_CollapsingHeader("Window options")) {
    ImGui_Checkbox("No titlebar", &no_titlebar);
    ImGui_SameLine(150);
    ImGui_Checkbox("No border", &no_border);
    ImGui_SameLine(300);
    ImGui_Checkbox("No resize", &no_resize);
    ImGui_Checkbox("No move", &no_move);
    ImGui_SameLine(150);
    ImGui_Checkbox("No scrollbar", &no_scrollbar);
    ImGui_SameLine(300);
    ImGui_Checkbox("No collapse", &no_collapse);
    ImGui_Checkbox("No menu", &no_menu);
    if (ImGui_TreeNode("Style")) {
      ImGui_ShowStyleEditor(NULL);
      ImGui_TreePop();
    }
    if (ImGui_TreeNode("Logging")) {
      ImGui_TextWrapped("The logging API redirects all text output so you can easily capture the content of a window or a block. Tree nodes can be automatically expanded. You can also call ImGui_LogText() to output directly to the log without a visual output.");
      ImGui_LogButtons();
      ImGui_TreePop();
    }
  }
  if (ImGui_CollapsingHeader("Widgets")) {
    if (ImGui_TreeNode("Trees")) {
      if (ImGui_TreeNode("Basic trees")) {
        for (i = 0; i < 5; i++) {
          if (ImGui_TreeNode((void*)(intptr_t)i, "Child %d", i)) {
            ImGui_Text("blah blah");
            ImGui_SameLine();
            if (ImGui_SmallButton("print")) {
              printf("Child %d pressed", i);
            }
            ImGui_TreePop();
          }
        }
        ImGui_TreePop();
      }
      if (ImGui_TreeNode("Advanced, with Selectable nodes")) {
        ShowHelpMarker("This is a more standard looking tree with selectable nodes.\nClick to select, CTRL+Click to toggle, click on arrows or double-click to open.");
        static bool align_label_with_current_x_position = false;
        ImGui_Checkbox("Align label with current X position)", &align_label_with_current_x_position);
        ImGui_Text("Hello!");
        if (align_label_with_current_x_position) {
          ImGui_Unindent(ImGui_GetTreeNodeToLabelSpacing());
        }
        static int selection_mask = (1 << 2); // Dumb representation of what may be user-side selection state. You may carry selection state inside or outside your objects in whatever format you see fit.
        int node_clicked = -1; // Temporary storage of what node we have clicked to process selection at the end of the loop. May be a pointer to your own node type, etc.
        ImGui_PushStyleVar(ImGuiStyleVar_IndentSpacing, ImGui_GetFontSize() * 3); // Increase spacing to differentiate leaves from expanded contents.
        for (i = 0; i < 6; i++) {
          // Disable the default open on single-click behavior and pass in Selected flag according to our selection state.
          ImGuiTreeNodeFlags node_flags = ImGuiTreeNodeFlags_OpenOnArrow | ImGuiTreeNodeFlags_OpenOnDoubleClick | ((selection_mask & (1 << i)) ? ImGuiTreeNodeFlags_Selected : 0);
          if (i < 3) {
            // Node
            bool node_open = ImGui_TreeNodeEx((void*)(intptr_t)i, node_flags, "Selectable Node %d", i);
            if (ImGui_IsItemClicked()) {
              node_clicked = i;
            }
            if (node_open) {
              ImGui_Text("Blah blah\nBlah Blah");
              ImGui_TreePop();
            }
          }
          else {
            // Leaf: The only reason we have a TreeNode at all is to allow selection of the leaf. Otherwise we can use BulletText() or TreeAdvanceToLabelPos()+Text().
            ImGui_TreeNodeEx((void*)(intptr_t)i, node_flags | ImGuiTreeNodeFlags_Leaf | ImGuiTreeNodeFlags_NoTreePushOnOpen, "Selectable Leaf %d", i);
            if (ImGui_IsItemClicked()) {
              node_clicked = i;
            }
          }
        }
        if (node_clicked != -1) {
          // Update selection state. Process outside of tree loop to avoid visual inconsistencies during the clicking-frame.
          if (ImGui_GetIO()->KeyCtrl) {
            selection_mask ^= (1 << node_clicked); // CTRL+click to toggle
          }
          else { //if (!(selection_mask & (1 << node_clicked))) // Depending on selection behavior you want, this commented bit preserve selection when clicking on item that is part of the selection
            selection_mask = (1 << node_clicked); // Click to single-select
          }
        }
        ImGui_PopStyleVar();
        if (align_label_with_current_x_position) {
          ImGui_Indent(ImGui_GetTreeNodeToLabelSpacing());
        }
        ImGui_TreePop();
      }
      ImGui_TreePop();
    }
    if (ImGui_TreeNode("Collapsing Headers")) {
      static bool closable_group = true;
      if (ImGui_CollapsingHeader("Header")) {
        ImGui_Checkbox("Enable extra group", &closable_group);
        for (i = 0; i < 5; i++) {
          ImGui_Text("Some content %d", i);
        }
      }
      if (ImGui_CollapsingHeader("Header with a close button", &closable_group)) {
        for (i = 0; i < 5; i++) {
          ImGui_Text("More content %d", i);
        }
      }
      ImGui_TreePop();
    }
    if (ImGui_TreeNode("Bullets")) {
      ImGui_BulletText("Bullet point 1");
      ImGui_BulletText("Bullet point 2\nOn multiple lines");
      ImGui_Bullet();
      ImGui_Text("Bullet point 3 (two calls)");
      ImGui_Bullet();
      ImGui_SmallButton("Button");
      ImGui_TreePop();
    }
    if (ImGui_TreeNode("Colored Text")) {
      // Using shortcut. You can use PushStyleColor()/PopStyleColor() for more flexibility.
      ImGui_TextColored(fVec4(1.0f, 0.0f, 1.0f, 1.0f), "Pink");
      ImGui_TextColored(fVec4(1.0f, 1.0f, 0.0f, 1.0f), "Yellow");
      ImGui_TextDisabled("Disabled");
      ImGui_TreePop();
    }
    if (ImGui_TreeNode("Word Wrapping")) {
      // Using shortcut. You can use PushTextWrapPos()/PopTextWrapPos() for more flexibility.
      ImGui_TextWrapped("This text should automatically wrap on the edge of the window. The current implementation for text wrapping follows simple rules suitable for English and possibly other languages.");
      ImGui_Spacing();
      static float wrap_width = 200.0f;
      ImGui_SliderFloat("Wrap width", &wrap_width, -20, 600, "%.0f");
      ImGui_Text("Test paragraph 1:");
      ImVec2 pos = ImGui_GetCursorScreenPos();
      ImDrawList* draw_list = ImGui_GetWindowDrawList();
      ImDrawList_AddRectFilled(draw_list, fVec2(pos.x + wrap_width, pos.y), fVec2(pos.x + wrap_width + 10, pos.y + ImGui_GetTextLineHeight()), 0xFFFF00FF);
      ImGui_PushTextWrapPos(ImGui_GetCursorPos().x + wrap_width);
      ImGui_Text("lazy dog. This paragraph is made to fit within %.0f pixels. The quick brown fox jumps over the lazy dog.", wrap_width);
      ImDrawList_AddRect(draw_list, ImGui_GetItemRectMin(), ImGui_GetItemRectMax(), 0xFF00FFFF);
      ImGui_PopTextWrapPos();
      ImGui_Text("Test paragraph 2:");
      pos = ImGui_GetCursorScreenPos();
      ImDrawList_AddRectFilled(draw_list, fVec2(pos.x + wrap_width, pos.y), fVec2(pos.x + wrap_width + 10, pos.y + ImGui_GetTextLineHeight()), 0xFFFF00FF);
      ImGui_PushTextWrapPos(ImGui_GetCursorPos().x + wrap_width);
      ImGui_Text("aaaaaaaa bbbbbbbb, cccccccc,dddddddd. eeeeeeee ffffffff. gggggggg!hhhhhhhh");
      ImDrawList_AddRect(draw_list, ImGui_GetItemRectMin(), ImGui_GetItemRectMax(), 0xFF00FFFF);
      ImGui_PopTextWrapPos();
      ImGui_TreePop();
    }
    if (ImGui_TreeNode("UTF-8 Text")) {
      // UTF-8 test with Japanese characters
      // (needs a suitable font, try Arial Unicode or M+ fonts http://mplus-fonts.sourceforge.jp/mplus-outline-fonts/index-en.html)
      // Most compiler appears to support UTF-8 in source code (with Visual Studio you need to save your file as 'UTF-8 without signature')
      // However for the sake for maximum portability here we are *not* including raw UTF-8 character in this source file, instead we encode the string with hexadecimal constants.
      // In your own application be reasonable and use UTF-8 in source or retrieve the data from file system!
      // Note that characters values are preserved even if the font cannot be displayed, so you can safely copy & paste garbled characters into another application.
      ImGui_TextWrapped("CJK text will only appears if the font was loaded with the appropriate CJK character ranges. Call io->Font->LoadFromFileTTF() manually to load extra character ranges.");
      ImGui_Text("Hiragana: \xe3\x81\x8b\xe3\x81\x8d\xe3\x81\x8f\xe3\x81\x91\xe3\x81\x93 (kakikukeko)");
      ImGui_Text("Kanjis: \xe6\x97\xa5\xe6\x9c\xac\xe8\xaa\x9e (nihongo)");
      static char buf[32] = "\xe6\x97\xa5\xe6\x9c\xac\xe8\xaa\x9e";
      ImGui_InputText("UTF-8 input", buf, IM_ARRAYSIZE(buf));
      ImGui_TreePop();
    }
    if (ImGui_TreeNode("Images")) {
      ImGui_TextWrapped("Below we are displaying the font texture (which is the only texture we have access to in this demo). Use the 'ImTextureID' type as storage to pass pointers or identifier to your own texture data. Hover the texture for a zoomed view!");
      ImVec2 tex_screen_pos = ImGui_GetCursorScreenPos();
      float tex_w = (float)ImGui_GetIO()->Fonts->TexWidth;
      float tex_h = (float)ImGui_GetIO()->Fonts->TexHeight;
      ImTextureID tex_id = ImGui_GetIO()->Fonts->TexID;
      ImGui_Text("%.0fx%.0f", tex_w, tex_h);
      ImGui_Image(tex_id, fVec2(tex_w, tex_h), fVec2(0, 0), fVec2(1, 1), ImColor(255, 255, 255, 255), ImColor(255, 255, 255, 128));
      if (ImGui_IsItemHovered()) {
        ImGui_BeginTooltip();
        float focus_sz = 32.0f;
        float focus_x = ImGui_GetMousePos().x - tex_screen_pos.x - focus_sz * 0.5f;
        if (focus_x < 0.0f) {
          focus_x = 0.0f;
        }
        else if (focus_x > tex_w - focus_sz) {
          focus_x = tex_w - focus_sz;
        }
        float focus_y = ImGui_GetMousePos().y - tex_screen_pos.y - focus_sz * 0.5f;
        if (focus_y < 0.0f) {
          focus_y = 0.0f;
        }
        else if (focus_y > tex_h - focus_sz) {
          focus_y = tex_h - focus_sz;
        }
        ImGui_Text("Min: (%.2f, %.2f)", focus_x, focus_y);
        ImGui_Text("Max: (%.2f, %.2f)", focus_x + focus_sz, focus_y + focus_sz);
        ImVec2 uv0 = fVec2((focus_x) / tex_w, (focus_y) / tex_h);
        ImVec2 uv1 = fVec2((focus_x + focus_sz) / tex_w, (focus_y + focus_sz) / tex_h);
        ImGui_Image(tex_id, fVec2(128, 128), uv0, uv1, ImColor(255, 255, 255, 255), ImColor(255, 255, 255, 128));
        ImGui_EndTooltip();
      }
      ImGui_TextWrapped("And now some textured buttons..");
      static int pressed_count = 0;
      for (i = 0; i < 8; i++) {
        ImGui_PushID(i);
        int frame_padding = -1 + i; // -1 = uses default padding
        if (ImGui_ImageButton(tex_id, fVec2(32, 32), fVec2(0, 0), fVec2(32.0f / tex_w, 32 / tex_h), frame_padding, ImColor(0, 0, 0, 255))) {
          pressed_count += 1;
        }
        ImGui_PopID();
        ImGui_SameLine();
      }
      ImGui_NewLine();
      ImGui_Text("Pressed %d times.", pressed_count);
      ImGui_TreePop();
    }
    if (ImGui_TreeNode("Selectables")) {
      if (ImGui_TreeNode("Basic")) {
        static bool selected[4] = { false, true, false, false };
        ImGui_Selectable("1. I am selectable", &selected[0]);
        ImGui_Selectable("2. I am selectable", &selected[1]);
        ImGui_Text("3. I am not selectable");
        ImGui_Selectable("4. I am selectable", &selected[2]);
        if (ImGui_Selectable("5. I am double clickable", selected[3], ImGuiSelectableFlags_AllowDoubleClick))
          if (ImGui_IsMouseDoubleClicked(0)) {
            selected[3] = !selected[3];
          }
        ImGui_TreePop();
      }
      if (ImGui_TreeNode("Rendering more text into the same block")) {
        static bool selected[3] = { false, false, false };
        ImGui_Selectable("main.c", &selected[0]);
        ImGui_SameLine(300);
        ImGui_Text(" 2,345 bytes");
        ImGui_Selectable("Hello.cpp", &selected[1]);
        ImGui_SameLine(300);
        ImGui_Text("12,345 bytes");
        ImGui_Selectable("Hello.h", &selected[2]);
        ImGui_SameLine(300);
        ImGui_Text(" 2,345 bytes");
        ImGui_TreePop();
      }
      if (ImGui_TreeNode("In columns")) {
        ImGui_Columns(3, NULL, false);
        static bool selected[16] = { 0 };
        for (i = 0; i < 16; i++) {
          char label[32];
          sprintf(label, "Item %d", i);
          if (ImGui_Selectable(label, &selected[i])) {}
          ImGui_NextColumn();
        }
        ImGui_Columns(1, NULL);
        ImGui_TreePop();
      }
      if (ImGui_TreeNode("Grid")) {
        static bool selected[16] = { true, false, false, false, false, true, false, false, false, false, true, false, false, false, false, true };
        for (i = 0; i < 16; i++) {
          ImGui_PushID(i);
          if (ImGui_Selectable("Sailor", &selected[i], 0, fVec2(50, 50))) {
            int x = i % 4, y = i / 4;
            if (x > 0) {
              selected[i - 1] ^= 1;
            }
            if (x < 3) {
              selected[i + 1] ^= 1;
            }
            if (y > 0) {
              selected[i - 4] ^= 1;
            }
            if (y < 3) {
              selected[i + 4] ^= 1;
            }
          }
          if ((i % 4) < 3) {
            ImGui_SameLine();
          }
          ImGui_PopID();
        }
        ImGui_TreePop();
      }
      ImGui_TreePop();
    }
    if (ImGui_TreeNode("Filtered Text Input")) {
      static char buf1[64] = "";
      ImGui_InputText("default", buf1, 64);
      static char buf2[64] = "";
      ImGui_InputText("decimal", buf2, 64, ImGuiInputTextFlags_CharsDecimal);
      static char buf3[64] = "";
      ImGui_InputText("hexadecimal", buf3, 64, ImGuiInputTextFlags_CharsHexadecimal | ImGuiInputTextFlags_CharsUppercase);
      static char buf4[64] = "";
      ImGui_InputText("uppercase", buf4, 64, ImGuiInputTextFlags_CharsUppercase);
      static char buf5[64] = "";
      ImGui_InputText("no blank", buf5, 64, ImGuiInputTextFlags_CharsNoBlank);
      struct TextFilters {
        static int FilterImGuiLetters(ImGuiTextEditCallbackData* data) {
          if (data->EventChar < 256 && strchr("imgui", (char)data->EventChar)) {
            return 0;
          }
          return 1;
        }
      };
      static char buf6[64] = "";
      ImGui_InputText("\"imgui\" letters", buf6, 64, ImGuiInputTextFlags_CallbackCharFilter, TextFilters::FilterImGuiLetters);
      ImGui_Text("Password input");
      static char bufpass[64] = "password123";
      ImGui_InputText("password", bufpass, 64, ImGuiInputTextFlags_Password | ImGuiInputTextFlags_CharsNoBlank);
      ImGui_SameLine();
      ShowHelpMarker("Display all characters as '*'.\nDisable clipboard cut and copy.\nDisable logging.\n");
      ImGui_InputText("password (clear)", bufpass, 64, ImGuiInputTextFlags_CharsNoBlank);
      ImGui_TreePop();
    }
    if (ImGui_TreeNode("Multi-line Text Input")) {
      static bool read_only = false;
      static char text[1024 * 16] =
          "/*\n"
          " The Pentium F00F bug, shorthand for F0 0F C7 C8,\n"
          " the hexadecimal encoding of one offending instruction,\n"
          " more formally, the invalid operand with locked CMPXCHG8B\n"
          " instruction bug, is a design flaw in the majority of\n"
          " Intel Pentium, Pentium MMX, and Pentium OverDrive\n"
          " processors (all in the P5 microarchitecture).\n"
          "*/\n\n"
          "label:\n"
          "\tlock cmpxchg8b eax\n";
      ImGui_PushStyleVar2(ImGuiStyleVar_FramePadding, fVec2(0, 0));
      ImGui_Checkbox("Read-only", &read_only);
      ImGui_PopStyleVar();
      ImGui_InputTextMultiline("##source", text, IM_ARRAYSIZE(text), fVec2(-1.0f, ImGui_GetTextLineHeight() * 16), ImGuiInputTextFlags_AllowTabInput | (read_only ? ImGuiInputTextFlags_ReadOnly : 0));
      ImGui_TreePop();
    }
    static bool a = false;
    if (ImGui_Button("Button")) {
      printf("Clicked\n");
      a ^= 1;
    }
    if (a) {
      ImGui_SameLine();
      ImGui_Text("Thanks for clicking me!");
    }
    static bool check = true;
    ImGui_Checkbox("checkbox", &check);
    static int e = 0;
    ImGui_RadioButton("radio a", &e, 0);
    ImGui_SameLine();
    ImGui_RadioButton("radio b", &e, 1);
    ImGui_SameLine();
    ImGui_RadioButton("radio c", &e, 2);
    // Color buttons, demonstrate using PushID() to add unique identifier in the ID stack, and changing style->
    for (i = 0; i < 7; i++) {
      if (i > 0) {
        ImGui_SameLine();
      }
      ImGui_PushID(i);
      ImGui_PushStyleColor(ImGuiCol_Button, ImColorHSV(i / 7.0f, 0.6f, 0.6f));
      ImGui_PushStyleColor(ImGuiCol_ButtonHovered, ImColorHSV(i / 7.0f, 0.7f, 0.7f));
      ImGui_PushStyleColor(ImGuiCol_ButtonActive, ImColorHSV(i / 7.0f, 0.8f, 0.8f));
      ImGui_Button("Click");
      ImGui_PopStyleColor(3);
      ImGui_PopID();
    }
    ImGui_Text("Hover over me");
    if (ImGui_IsItemHovered()) {
      ImGui_SetTooltip("I am a tooltip");
    }
    ImGui_SameLine();
    ImGui_Text("- or me");
    if (ImGui_IsItemHovered()) {
      ImGui_BeginTooltip();
      ImGui_Text("I am a fancy tooltip");
      static float arr[] = { 0.6f, 0.1f, 1.0f, 0.5f, 0.92f, 0.1f, 0.2f };
      ImGui_PlotLines("Curve", arr, IM_ARRAYSIZE(arr), 0, NULL);
      ImGui_EndTooltip();
    }
    // Testing IMGUI_ONCE_UPON_A_FRAME macro
    //for (i = 0; i < 5; i++)
    //{
    // IMGUI_ONCE_UPON_A_FRAME
    // {
    // ImGui_Text("This will be displayed only once.");
    // }
    //}
    ImGui_Separator();
    ImGui_LabelText("label", "Value");
    static int item = 1;
    ImGui_Combo("combo", &item, "aaaa\0bbbb\0cccc\0dddd\0eeee\0\0"); // Combo using values packed in a single constant string (for really quick combo)
    const char* items[] = { "AAAA", "BBBB", "CCCC", "DDDD", "EEEE", "FFFF", "GGGG", "HHHH", "IIII", "JJJJ", "KKKK" };
    static int item2 = -1;
    ImGui_Combo("combo scroll", &item2, items, IM_ARRAYSIZE(items)); // Combo using proper array. You can also pass a callback to retrieve array value, no need to create/copy an array just for that.
    {
      static char str0[128] = "Hello, world!";
      static int i0 = 123;
      static float f0 = 0.001f;
      ImGui_InputText("input text", str0, IM_ARRAYSIZE(str0));
      ImGui_SameLine();
      ShowHelpMarker("Hold SHIFT or use mouse to select text.\n" "CTRL+Left/Right to word jump.\n" "CTRL+A or double-click to select all.\n" "CTRL+X,CTRL+C,CTRL+V clipboard.\n" "CTRL+Z,CTRL+Y undo/redo.\n" "ESCAPE to revert.\n");
      ImGui_InputInt("input int", &i0);
      ImGui_SameLine();
      ShowHelpMarker("You can apply arithmetic operators +,*,/ on numerical values.\n e.g. [ 100 ], input \'*2\', result becomes [ 200 ]\nUse +- to subtract.\n");
      ImGui_InputFloat("input float", &f0, 0.01f, 1.0f);
      static float vec4a[4] = { 0.10f, 0.20f, 0.30f, 0.44f };
      ImGui_InputFloat3("input float3", vec4a);
    }
    {
      static int i1 = 50, i2 = 42;
      ImGui_DragInt("drag int", &i1, 1);
      ImGui_SameLine();
      ShowHelpMarker("Click and drag to edit value.\nHold SHIFT/ALT for faster/slower edit.\nDouble-click or CTRL+click to input value.");
      ImGui_DragInt("drag int 0..100", &i2, 1, 0, 100, "%.0f%%");
      static float f1 = 1.00f, f2 = 0.0067f;
      ImGui_DragFloat("drag float", &f1, 0.005f);
      ImGui_DragFloat("drag small float", &f2, 0.0001f, 0.0f, 0.0f, "%.06f ns");
    }
    {
      static int i1 = 0;
      ImGui_SliderInt("slider int", &i1, -1, 3);
      ImGui_SameLine();
      ShowHelpMarker("CTRL+click to input value.");
      static float f1 = 0.123f, f2 = 0.0f;
      ImGui_SliderFloat("slider float", &f1, 0.0f, 1.0f, "ratio = %.3f");
      ImGui_SliderFloat("slider log float", &f2, -10.0f, 10.0f, "%.4f", 3.0f);
      static float angle = 0.0f;
      ImGui_SliderAngle("slider angle", &angle);
    }
    static float col1[3] = { 1.0f, 0.0f, 0.2f };
    static float col2[4] = { 0.4f, 0.7f, 0.0f, 0.5f };
    ImGui_ColorEdit3("color 1", col1);
    ImGui_SameLine();
    ShowHelpMarker("Click on the colored square to change edit mode.\nCTRL+click on individual component to input value.\n");
    ImGui_ColorEdit4("color 2", col2);
    const char* listbox_items[] = { "Apple", "Banana", "Cherry", "Kiwi", "Mango", "Orange", "Pineapple", "Strawberry", "Watermelon" };
    static int listbox_item_current = 1;
    ImGui_ListBox("listbox\n(single select)", &listbox_item_current, listbox_items, IM_ARRAYSIZE(listbox_items), 4);
    //static int listbox_item_current2 = 2;
    //ImGui_PushItemWidth(-1);
    //ImGui_ListBox("##listbox2", &listbox_item_current2, listbox_items, IM_ARRAYSIZE(listbox_items), 4);
    //ImGui_PopItemWidth();
    if (ImGui_TreeNode("Range Widgets")) {
      ImGui_Unindent();
      static float begin = 10, end = 90;
      static int begin_i = 100, end_i = 1000;
      ImGui_DragFloatRange2("range", &begin, &end, 0.25f, 0.0f, 100.0f, "Min: %.1f %%", "Max: %.1f %%");
      ImGui_DragIntRange2("range int (no bounds)", &begin_i, &end_i, 5, 0, 0, "Min: %.0f units", "Max: %.0f units");
      ImGui_Indent();
      ImGui_TreePop();
    }
    if (ImGui_TreeNode("Multi-component Widgets")) {
      ImGui_Unindent();
      static float vec4f[4] = { 0.10f, 0.20f, 0.30f, 0.44f };
      static int vec4i[4] = { 1, 5, 100, 255 };
      ImGui_InputFloat2("input float2", vec4f);
      ImGui_DragFloat2("drag float2", vec4f, 0.01f, 0.0f, 1.0f);
      ImGui_SliderFloat2("slider float2", vec4f, 0.0f, 1.0f);
      ImGui_DragInt2("drag int2", vec4i, 1, 0, 255);
      ImGui_InputInt2("input int2", vec4i);
      ImGui_SliderInt2("slider int2", vec4i, 0, 255);
      ImGui_Spacing();
      ImGui_InputFloat3("input float3", vec4f);
      ImGui_DragFloat3("drag float3", vec4f, 0.01f, 0.0f, 1.0f);
      ImGui_SliderFloat3("slider float3", vec4f, 0.0f, 1.0f);
      ImGui_DragInt3("drag int3", vec4i, 1, 0, 255);
      ImGui_InputInt3("input int3", vec4i);
      ImGui_SliderInt3("slider int3", vec4i, 0, 255);
      ImGui_Spacing();
      ImGui_InputFloat4("input float4", vec4f);
      ImGui_DragFloat4("drag float4", vec4f, 0.01f, 0.0f, 1.0f);
      ImGui_SliderFloat4("slider float4", vec4f, 0.0f, 1.0f);
      ImGui_InputInt4("input int4", vec4i);
      ImGui_DragInt4("drag int4", vec4i, 1, 0, 255);
      ImGui_SliderInt4("slider int4", vec4i, 0, 255);
      ImGui_Indent();
      ImGui_TreePop();
    }
    if (ImGui_TreeNode("Vertical Sliders")) {
      ImGui_Unindent();
      const float spacing = 4;
      ImGui_PushStyleVar2(ImGuiStyleVar_ItemSpacing, fVec2(spacing, spacing));
      static int int_value = 0;
      ImGui_VSliderInt("##int", fVec2(18, 160), &int_value, 0, 5);
      ImGui_SameLine();
      static float values[7] = { 0.0f, 0.60f, 0.35f, 0.9f, 0.70f, 0.20f, 0.0f };
      ImGui_PushID("set1");
      int i;
      for (i = 0; i < 7; i++) {
        if (i > 0) {
          ImGui_SameLine();
        }
        ImGui_PushID(i);
        ImGui_PushStyleColor(ImGuiCol_FrameBg, ImColorHSV(i / 7.0f, 0.5f, 0.5f));
        ImGui_PushStyleColor(ImGuiCol_FrameBgHovered, ImColorHSV(i / 7.0f, 0.6f, 0.5f));
        ImGui_PushStyleColor(ImGuiCol_FrameBgActive, ImColorHSV(i / 7.0f, 0.7f, 0.5f));
        ImGui_PushStyleColor(ImGuiCol_SliderGrab, ImColorHSV(i / 7.0f, 0.9f, 0.9f));
        ImGui_VSliderFloat("##v", fVec2(18, 160), &values[i], 0.0f, 1.0f, "");
        if (ImGui_IsItemActive() || ImGui_IsItemHovered()) {
          ImGui_SetTooltip("%.3f", values[i]);
        }
        ImGui_PopStyleColor(4);
        ImGui_PopID();
      }
      ImGui_PopID();
      ImGui_SameLine();
      ImGui_PushID("set2");
      static float values2[4] = { 0.20f, 0.80f, 0.40f, 0.25f };
      const int rows = 3;
      const ImVec2 small_slider_size = fVec2(18, (160.0f - (rows - 1) * spacing) / rows);
      for (int nx = 0; nx < 4; nx++) {
        if (nx > 0) {
          ImGui_SameLine();
        }
        ImGui_BeginGroup();
        for (int ny = 0; ny < rows; ny++) {
          ImGui_PushID(nx * rows + ny);
          ImGui_VSliderFloat("##v", small_slider_size, &values2[nx], 0.0f, 1.0f, "");
          if (ImGui_IsItemActive() || ImGui_IsItemHovered()) {
            ImGui_SetTooltip("%.3f", values2[nx]);
          }
          ImGui_PopID();
        }
        ImGui_EndGroup();
      }
      ImGui_PopID();
      ImGui_SameLine();
      ImGui_PushID("set3");
      for (i = 0; i < 4; i++) {
        if (i > 0) {
          ImGui_SameLine();
        }
        ImGui_PushID(i);
        ImGui_PushStyleVar(ImGuiStyleVar_GrabMinSize, 40);
        ImGui_VSliderFloat("##v", fVec2(40, 160), &values[i], 0.0f, 1.0f, "%.2f\nsec");
        ImGui_PopStyleVar();
        ImGui_PopID();
      }
      ImGui_PopID();
      ImGui_PopStyleVar();
      ImGui_Indent();
      ImGui_TreePop();
    }
  }
  if (ImGui_CollapsingHeader("Graphs widgets")) {
    static bool animate = true;
    ImGui_Checkbox("Animate", &animate);
    static float arr[] = { 0.6f, 0.1f, 1.0f, 0.5f, 0.92f, 0.1f, 0.2f };
    ImGui_PlotLines("Frame Times", arr, IM_ARRAYSIZE(arr), 0, NULL);
    // Create a dummy array of contiguous float values to plot
    // Tip: If your float aren't contiguous but part of a structure, you can pass a pointer to your first float and the sizeof() of your structure in the Stride parameter.
    static float values[90] = { 0 };
    static int values_offset = 0;
    if (animate) {
      static float refresh_time = ImGui_GetTime(); // Create dummy data at fixed 60 hz rate for the demo
      for (; ImGui_GetTime() > refresh_time + 1.0f / 60.0f; refresh_time += 1.0f / 60.0f) {
        static float phase = 0.0f;
        values[values_offset] = cosf(phase);
        values_offset = (values_offset + 1) % IM_ARRAYSIZE(values);
        phase += 0.10f * values_offset;
      }
    }
    ImGui_PlotLines("Lines", values, IM_ARRAYSIZE(values), values_offset, "avg 0.0", -1.0f, 1.0f, fVec2(0, 80));
    ImGui_PlotHistogram("Histogram", arr, IM_ARRAYSIZE(arr), 0, NULL, 0.0f, 1.0f, fVec2(0, 80));
    // Use functions to generate output
    // FIXME: This is rather awkward because current plot API only pass in indices. We probably want an API passing floats and user provide sample rate/count.
    struct Funcs {
      static float Sin(void*, int i) {
        return sinf(i * 0.1f);
      }
      static float Saw(void*, int i) {
        return (i & 1) ? 1.0f : 0.0f;
      }
    };
    static int func_type = 0, display_count = 70;
    ImGui_Separator();
    ImGui_PushItemWidth(100);
    ImGui_Combo("func", &func_type, "Sin\0Saw\0");
    ImGui_PopItemWidth();
    ImGui_SameLine();
    ImGui_SliderInt("Sample count", &display_count, 1, 400);
    float(*func)(void*, int) = (func_type == 0) ? Funcs::Sin : Funcs::Saw;
    ImGui_PlotLines("Lines", func, NULL, display_count, 0, NULL, -1.0f, 1.0f, fVec2(0, 80));
    ImGui_PlotHistogram("Histogram", func, NULL, display_count, 0, NULL, -1.0f, 1.0f, fVec2(0, 80));
    ImGui_Separator();
    // Animate a simple progress bar
    static float progress = 0.0f, progress_dir = 1.0f;
    if (animate) {
      progress += progress_dir * 0.4f * ImGui_GetIO()->DeltaTime;
      if (progress >= +1.1f) {
        progress = +1.1f;
        progress_dir *= -1.0f;
      }
      if (progress <= -0.1f) {
        progress = -0.1f;
        progress_dir *= -1.0f;
      }
    }
    // Typically we would use fVec2(-1.0f,0.0f) to use all available width, or fVec2(width,0.0f) for a specified width. fVec2(0.0f,0.0f) uses ItemWidth.
    ImVec2 size_arg = fVec2(0.0f, 0.0f);
    ImGui_ProgressBar(progress, &size_arg, NULL);
    ImGui_SameLine(0.0f, ImGui_GetStyle()->ItemInnerSpacing.x);
    ImGui_Text("Progress Bar");
    float progress_saturated = (progress < 0.0f) ? 0.0f : (progress > 1.0f) ? 1.0f : progress;
    char buf[32];
    sprintf(buf, "%d/%d", (int)(progress_saturated * 1753), 1753);
    size_arg = fVec2(0.f, 0.f);
    ImGui_ProgressBar(progress, &size_arg, buf);
  }
  if (ImGui_CollapsingHeader("Layout")) {
    if (ImGui_TreeNode("Child regions")) {
      ImGui_Text("Without border");
      static int line = 50;
      bool goto_line = ImGui_Button("Goto");
      ImGui_SameLine();
      ImGui_PushItemWidth(100);
      goto_line |= ImGui_InputInt("##Line", &line, 0, 0, ImGuiInputTextFlags_EnterReturnsTrue);
      ImGui_PopItemWidth();
      ImGui_BeginChild("Sub1", fVec2(ImGui_GetWindowContentRegionWidth() * 0.5f, 300), false, ImGuiWindowFlags_HorizontalScrollbar);
      for (i = 0; i < 100; i++) {
        ImGui_Text("%04d: scrollable region", i);
        if (goto_line && line == i) {
          ImGui_SetScrollHere();
        }
      }
      if (goto_line && line >= 100) {
        ImGui_SetScrollHere();
      }
      ImGui_EndChild();
      ImGui_SameLine();
      ImGui_BeginChild("Sub2", fVec2(0, 300), true, 0);
      ImGui_PushStyleVar(ImGuiStyleVar_ChildWindowRounding, 5.0f);
      ImGui_Text("With border");
      ImGui_Columns(2, NULL);
      for (i = 0; i < 100; i++) {
        if (i == 50) {
          ImGui_NextColumn();
        }
        char buf[32];
        sprintf(buf, "%08x", i * 5731);
        ImGui_Button(buf, fVec2(-1.0f, 0.0f));
      }
      ImGui_PopStyleVar();
      ImGui_EndChild();
      ImGui_TreePop();
    }
    if (ImGui_TreeNode("Widgets Width")) {
      static float f = 0.0f;
      ImGui_Text("PushItemWidth(100)");
      ImGui_SameLine();
      ShowHelpMarker("Fixed width.");
      ImGui_PushItemWidth(100);
      ImGui_DragFloat("float##1", &f);
      ImGui_PopItemWidth();
      ImGui_Text("PushItemWidth(GetWindowWidth() * 0.5f)");
      ImGui_SameLine();
      ShowHelpMarker("Half of window width.");
      ImGui_PushItemWidth(ImGui_GetWindowWidth() * 0.5f);
      ImGui_DragFloat("float##2", &f);
      ImGui_PopItemWidth();
      ImGui_Text("PushItemWidth(GetContentRegionAvailWidth() * 0.5f)");
      ImGui_SameLine();
      ShowHelpMarker("Half of available width.\n(~ right-cursor_pos)\n(works within a column set)");
      ImGui_PushItemWidth(ImGui_GetContentRegionAvailWidth() * 0.5f);
      ImGui_DragFloat("float##3", &f);
      ImGui_PopItemWidth();
      ImGui_Text("PushItemWidth(-100)");
      ImGui_SameLine();
      ShowHelpMarker("Align to right edge minus 100");
      ImGui_PushItemWidth(-100);
      ImGui_DragFloat("float##4", &f);
      ImGui_PopItemWidth();
      ImGui_Text("PushItemWidth(-1)");
      ImGui_SameLine();
      ShowHelpMarker("Align to right edge");
      ImGui_PushItemWidth(-1);
      ImGui_DragFloat("float##5", &f);
      ImGui_PopItemWidth();
      ImGui_TreePop();
    }
    if (ImGui_TreeNode("Basic Horizontal Layout")) {
      ImGui_TextWrapped("(Use ImGui_SameLine() to keep adding items to the right of the preceding item)");
      // Text
      ImGui_Text("Two items: Hello");
      ImGui_SameLine();
      ImGui_TextColored(fVec4(1, 1, 0, 1), "Sailor");
      // Adjust spacing
      ImGui_Text("More spacing: Hello");
      ImGui_SameLine(0, 20);
      ImGui_TextColored(fVec4(1, 1, 0, 1), "Sailor");
      // Button
      ImGui_AlignFirstTextHeightToWidgets();
      ImGui_Text("Normal buttons");
      ImGui_SameLine();
      ImGui_Button("Banana");
      ImGui_SameLine();
      ImGui_Button("Apple");
      ImGui_SameLine();
      ImGui_Button("Corniflower");
      // Button
      ImGui_Text("Small buttons");
      ImGui_SameLine();
      ImGui_SmallButton("Like this one");
      ImGui_SameLine();
      ImGui_Text("can fit within a text block.");
      // Aligned to arbitrary position. Easy/cheap column.
      ImGui_Text("Aligned");
      ImGui_SameLine(150);
      ImGui_Text("x=150");
      ImGui_SameLine(300);
      ImGui_Text("x=300");
      ImGui_Text("Aligned");
      ImGui_SameLine(150);
      ImGui_SmallButton("x=150");
      ImGui_SameLine(300);
      ImGui_SmallButton("x=300");
      // Checkbox
      static bool c1 = false, c2 = false, c3 = false, c4 = false;
      ImGui_Checkbox("My", &c1);
      ImGui_SameLine();
      ImGui_Checkbox("Tailor", &c2);
      ImGui_SameLine();
      ImGui_Checkbox("Is", &c3);
      ImGui_SameLine();
      ImGui_Checkbox("Rich", &c4);
      // Various
      static float f0 = 1.0f, f1 = 2.0f, f2 = 3.0f;
      ImGui_PushItemWidth(80);
      const char* items[] = { "AAAA", "BBBB", "CCCC", "DDDD" };
      static int item = -1;
      ImGui_Combo("Combo", &item, items, IM_ARRAYSIZE(items));
      ImGui_SameLine();
      ImGui_SliderFloat("X", &f0, 0.0f, 5.0f);
      ImGui_SameLine();
      ImGui_SliderFloat("Y", &f1, 0.0f, 5.0f);
      ImGui_SameLine();
      ImGui_SliderFloat("Z", &f2, 0.0f, 5.0f);
      ImGui_PopItemWidth();
      ImGui_PushItemWidth(80);
      ImGui_Text("Lists:");
      static int selection[4] = { 0, 1, 2, 3 };
      for (i = 0; i < 4; i++) {
        if (i > 0) {
          ImGui_SameLine();
        }
        ImGui_PushID(i);
        ImGui_ListBox("", &selection[i], items, IM_ARRAYSIZE(items));
        ImGui_PopID();
        //if (ImGui_IsItemHovered()) ImGui_SetTooltip("ListBox %d hovered", i);
      }
      ImGui_PopItemWidth();
      // Dummy
      ImVec2 sz = fVec2(30, 30);
      ImGui_Button("A", sz);
      ImGui_SameLine();
      ImGui_Dummy(sz);
      ImGui_SameLine();
      ImGui_Button("B", sz);
      ImGui_TreePop();
    }
    if (ImGui_TreeNode("Groups")) {
      ImGui_TextWrapped("(Using ImGui_BeginGroup()/EndGroup() to layout items. BeginGroup() basically locks the horizontal position. EndGroup() bundles the whole group so that you can use functions such as IsItemHovered() on it.)");
      ImGui_BeginGroup();
      {
        ImGui_BeginGroup();
        ImGui_Button("AAA");
        ImGui_SameLine();
        ImGui_Button("BBB");
        ImGui_SameLine();
        ImGui_BeginGroup();
        ImGui_Button("CCC");
        ImGui_Button("DDD");
        ImGui_EndGroup();
        if (ImGui_IsItemHovered()) {
          ImGui_SetTooltip("Group hovered");
        }
        ImGui_SameLine();
        ImGui_Button("EEE");
        ImGui_EndGroup();
      }
      // Capture the group size and create widgets using the same size
      ImVec2 size = ImGui_GetItemRectSize();
      const float values[5] = { 0.5f, 0.20f, 0.80f, 0.60f, 0.25f };
      ImGui_PlotHistogram("##values", values, IM_ARRAYSIZE(values), 0, NULL, 0.0f, 1.0f, size);
      ImGui_Button("ACTION", fVec2((size.x - ImGui_GetStyle()->ItemSpacing.x) * 0.5f, size.y));
      ImGui_SameLine();
      ImGui_Button("REACTION", fVec2((size.x - ImGui_GetStyle()->ItemSpacing.x) * 0.5f, size.y));
      ImGui_EndGroup();
      ImGui_SameLine();
      ImGui_Button("LEVERAGE\nBUZZWORD", size);
      ImGui_SameLine();
      ImGui_ListBoxHeader("List", size);
      ImGui_Selectable("Selected", true);
      ImGui_Selectable("Not Selected", false);
      ImGui_ListBoxFooter();
      ImGui_TreePop();
    }
    if (ImGui_TreeNode("Text Baseline Alignment")) {
      ImGui_TextWrapped("(This is testing the vertical alignment that occurs on text to keep it at the same baseline as widgets. Lines only composed of text or \"small\" widgets fit in less vertical spaces than lines with normal widgets)");
      ImGui_Text("One\nTwo\nThree");
      ImGui_SameLine();
      ImGui_Text("Hello\nWorld");
      ImGui_SameLine();
      ImGui_Text("Banana");
      ImGui_Text("Banana");
      ImGui_SameLine();
      ImGui_Text("Hello\nWorld");
      ImGui_SameLine();
      ImGui_Text("One\nTwo\nThree");
      ImGui_Button("HOP##1");
      ImGui_SameLine();
      ImGui_Text("Banana");
      ImGui_SameLine();
      ImGui_Text("Hello\nWorld");
      ImGui_SameLine();
      ImGui_Text("Banana");
      ImGui_Button("HOP##2");
      ImGui_SameLine();
      ImGui_Text("Hello\nWorld");
      ImGui_SameLine();
      ImGui_Text("Banana");
      ImGui_Button("TEST##1");
      ImGui_SameLine();
      ImGui_Text("TEST");
      ImGui_SameLine();
      ImGui_SmallButton("TEST##2");
      ImGui_AlignFirstTextHeightToWidgets(); // If your line starts with text, call this to align it to upcoming widgets.
      ImGui_Text("Text aligned to Widget");
      ImGui_SameLine();
      ImGui_Button("Widget##1");
      ImGui_SameLine();
      ImGui_Text("Widget");
      ImGui_SameLine();
      ImGui_SmallButton("Widget##2");
      // Tree
      const float spacing = ImGui_GetStyle()->ItemInnerSpacing.x;
      ImGui_Button("Button##1");
      ImGui_SameLine(0.0f, spacing);
      if (ImGui_TreeNode("Node##1")) {
        for (i = 0; i < 6; i++) {
          ImGui_BulletText("Item %d..", i); // Dummy tree data
        }
        ImGui_TreePop();
      }
      ImGui_AlignFirstTextHeightToWidgets(); // Vertically align text node a bit lower so it'll be vertically centered with upcoming widget. Otherwise you can use SmallButton (smaller fit).
      bool node_open = ImGui_TreeNode("Node##2"); // Common mistake to avoid: if we want to SameLine after TreeNode we need to do it before we add child content.
      ImGui_SameLine(0.0f, spacing);
      ImGui_Button("Button##2");
      if (node_open) {
        for (i = 0; i < 6; i++) {
          ImGui_BulletText("Item %d..", i); // Dummy tree data
        }
        ImGui_TreePop();
      }
      // Bullet
      ImGui_Button("Button##3");
      ImGui_SameLine(0.0f, spacing);
      ImGui_BulletText("Bullet text");
      ImGui_AlignFirstTextHeightToWidgets();
      ImGui_BulletText("Node");
      ImGui_SameLine(0.0f, spacing);
      ImGui_Button("Button##4");
      ImGui_TreePop();
    }
    if (ImGui_TreeNode("Scrolling")) {
      ImGui_TextWrapped("(Use SetScrollHere() or SetScrollFromPosY() to scroll to a given position.)");
      static bool track = true;
      static int track_line = 50, scroll_to_px = 200;
      ImGui_Checkbox("Track", &track);
      ImGui_PushItemWidth(100);
      ImGui_SameLine(130);
      track |= ImGui_DragInt("##line", &track_line, 0.25f, 0, 99, "Line %.0f");
      bool scroll_to = ImGui_Button("Scroll To");
      ImGui_SameLine(130);
      scroll_to |= ImGui_DragInt("##pos_y", &scroll_to_px, 1.00f, 0, 9999, "y = %.0f px");
      ImGui_PopItemWidth();
      if (scroll_to) {
        track = false;
      }
      for (i = 0; i < 5; i++) {
        if (i > 0) {
          ImGui_SameLine();
        }
        ImGui_BeginGroup();
        ImGui_Text("%s", i == 0 ? "Top" : i == 1 ? "25%" : i == 2 ? "Center" : i == 3 ? "75%" : "Bottom");
        ImGui_BeginChildID(ImGui_GetID((void*)(intptr_t)i), fVec2(ImGui_GetWindowWidth() * 0.17f, 200.0f), true, 0);
        if (scroll_to) {
          ImGui_SetScrollFromPosY(ImGui_GetCursorStartPos().y + scroll_to_px, i * 0.25f);
        }
        for (int line = 0; line < 100; line++) {
          if (track && line == track_line) {
            ImGui_TextColored(ImColor(255, 255, 0), "Line %d", line);
            ImGui_SetScrollHere(i * 0.25f); // 0.0f:top, 0.5f:center, 1.0f:bottom
          }
          else {
            ImGui_Text("Line %d", line);
          }
        }
        ImGui_EndChild();
        ImGui_EndGroup();
      }
      ImGui_TreePop();
    }
    if (ImGui_TreeNode("Horizontal Scrolling")) {
      ImGui_Bullet();
      ImGui_TextWrapped("Horizontal scrolling for a window has to be enabled explicitly via the ImGuiWindowFlags_HorizontalScrollbar flag.");
      ImGui_Bullet();
      ImGui_TextWrapped("You may want to explicitly specify content width by calling SetNextWindowContentWidth() before Begin().");
      static int lines = 7;
      ImGui_SliderInt("Lines", &lines, 1, 15);
      ImGui_PushStyleVar(ImGuiStyleVar_FrameRounding, 3.0f);
      ImGui_PushStyleVar2(ImGuiStyleVar_FramePadding, fVec2(2.0f, 1.0f));
      ImGui_BeginChild("scrolling", fVec2(0, ImGui_GetItemsLineHeightWithSpacing() * 7 + 30), true, ImGuiWindowFlags_HorizontalScrollbar);
      for (int line = 0; line < lines; line++) {
        // Display random stuff
        int num_buttons = 10 + ((line & 1) ? line * 9 : line * 3);
        for (int n = 0; n < num_buttons; n++) {
          if (n > 0) {
            ImGui_SameLine();
          }
          ImGui_PushID(n + line * 1000);
          char num_buf[16];
          const char* label = (!(n % 15)) ? "FizzBuzz" : (!(n % 3)) ? "Fizz" : (!(n % 5)) ? "Buzz" : (sprintf(num_buf, "%d", n), num_buf);
          float hue = n * 0.05f;
          ImGui_PushStyleColor(ImGuiCol_Button, ImColorHSV(hue, 0.6f, 0.6f));
          ImGui_PushStyleColor(ImGuiCol_ButtonHovered, ImColorHSV(hue, 0.7f, 0.7f));
          ImGui_PushStyleColor(ImGuiCol_ButtonActive, ImColorHSV(hue, 0.8f, 0.8f));
          ImGui_Button(label, fVec2(40.0f + sinf((float)(line + n)) * 20.0f, 0.0f));
          ImGui_PopStyleColor(3);
          ImGui_PopID();
        }
      }
      ImGui_EndChild();
      ImGui_PopStyleVar(2);
      float scroll_x_delta = 0.0f;
      ImGui_SmallButton("<<");
      if (ImGui_IsItemActive()) {
        scroll_x_delta = -ImGui_GetIO()->DeltaTime * 1000.0f;
      }
      ImGui_SameLine();
      ImGui_Text("Scroll from code");
      ImGui_SameLine();
      ImGui_SmallButton(">>");
      if (ImGui_IsItemActive()) {
        scroll_x_delta = +ImGui_GetIO()->DeltaTime * 1000.0f;
      }
      if (scroll_x_delta != 0.0f) {
        ImGui_BeginChild("scrolling", fVec2(0, 0), false, 0); // Demonstrate a trick: you can use Begin to set yourself in the context of another window (here we are already out of your child window)
        ImGui_SetScrollX(ImGui_GetScrollX() + scroll_x_delta);
        ImGui_End();
      }
      ImGui_TreePop();
    }
    if (ImGui_TreeNode("Clipping")) {
      static ImVec2 size = fVec2(100, 100), offset = fVec2(50, 20);
      ImGui_TextWrapped("On a per-widget basis we are occasionally clipping text CPU-side if it won't fit in its frame. Otherwise we are doing coarser clipping + passing a scissor rectangle to the renderer. The system is designed to try minimizing both execution and CPU/GPU rendering cost.");
      ImGui_DragFloat2("size", (float*)&size, 0.5f, 0.0f, 200.0f, "%.0f");
      ImGui_TextWrapped("(Click and drag)");
      ImVec2 pos = ImGui_GetCursorScreenPos();
      ImVec4 clip_rect = fVec4(pos.x, pos.y, pos.x + size.x, pos.y + size.y);
      ImGui_InvisibleButton("##dummy", size);
      if (ImGui_IsItemActive() && ImGui_IsMouseDragging()) {
        offset.x += ImGui_GetIO()->MouseDelta.x;
        offset.y += ImGui_GetIO()->MouseDelta.y;
      }
      ImDrawList* draw_list = ImGui_GetWindowDrawList();
      ImDrawList_AddRectFilled(draw_list, pos, fVec2(pos.x + size.x, pos.y + size.y), ImColorU32RGBA(90, 90, 120, 255));
      ImDrawList_AddText(draw_list, ImGui_GetFont(), ImGui_GetFontSize() * 2.0f, fVec2(pos.x + offset.x, pos.y + offset.y), ImColorU32RGBA(255, 255, 255, 255), "Line 1 hello\nLine 2 clip me!", NULL, 0.0f, &clip_rect);
      ImGui_TreePop();
    }
  }
  if (ImGui_CollapsingHeader("Popups & Modal windows")) {
    if (ImGui_TreeNode("Popups")) {
      ImGui_TextWrapped("When a popup is active, it inhibits interacting with windows that are behind the popup. Clicking outside the popup closes it.");
      static int selected_fish = -1;
      const char* names[] = { "Bream", "Haddock", "Mackerel", "Pollock", "Tilefish" };
      static bool toggles[] = { true, false, false, false, false };
      if (ImGui_Button("Select..")) {
        ImGui_OpenPopup("select");
      }
      ImGui_SameLine();
      ImGui_Text(selected_fish == -1 ? "<None>" : names[selected_fish]);
      if (ImGui_BeginPopup("select")) {
        ImGui_Text("Aquarium");
        ImGui_Separator();
        for (i = 0; i < IM_ARRAYSIZE(names); i++)
          if (ImGui_Selectable(names[i])) {
            selected_fish = i;
          }
        ImGui_EndPopup();
      }
      if (ImGui_Button("Toggle..")) {
        ImGui_OpenPopup("toggle");
      }
      if (ImGui_BeginPopup("toggle")) {
        for (i = 0; i < IM_ARRAYSIZE(names); i++) {
          ImGui_MenuItem(names[i], "", &toggles[i]);
        }
        if (ImGui_BeginMenu("Sub-menu")) {
          ImGui_MenuItem("Click me");
          ImGui_EndMenu();
        }
        ImGui_Separator();
        ImGui_Text("Tooltip here");
        if (ImGui_IsItemHovered()) {
          ImGui_SetTooltip("I am a tooltip over a popup");
        }
        if (ImGui_Button("Stacked Popup")) {
          ImGui_OpenPopup("another popup");
        }
        if (ImGui_BeginPopup("another popup")) {
          for (i = 0; i < IM_ARRAYSIZE(names); i++) {
            ImGui_MenuItem(names[i], "", &toggles[i]);
          }
          if (ImGui_BeginMenu("Sub-menu")) {
            ImGui_MenuItem("Click me");
            ImGui_EndMenu();
          }
          ImGui_EndPopup();
        }
        ImGui_EndPopup();
      }
      if (ImGui_Button("Popup Menu..")) {
        ImGui_OpenPopup("popup from button");
      }
      if (ImGui_BeginPopup("popup from button")) {
        ShowExampleMenuFile();
        ImGui_EndPopup();
      }
      ImGui_Spacing();
      ImGui_TextWrapped("Below we are testing adding menu items to a regular window. It's rather unusual but should work!");
      ImGui_Separator();
      // NB: As a quirk in this very specific example, we want to differentiate the parent of this menu from the parent of the various popup menus above.
      // To do so we are encloding the items in a PushID()/PopID() block to make them two different menusets. If we don't, opening any popup above and hovering our menu here
      // would open it. This is because once a menu is active, we allow to switch to a sibling menu by just hovering on it, which is the desired behavior for regular menus.
      ImGui_PushID("foo");
      ImGui_MenuItem("Menu item", "CTRL+M");
      if (ImGui_BeginMenu("Menu inside a regular window")) {
        ShowExampleMenuFile();
        ImGui_EndMenu();
      }
      ImGui_PopID();
      ImGui_Separator();
      ImGui_TreePop();
    }
    if (ImGui_TreeNode("Context menus")) {
      static float value = 0.5f;
      ImGui_Text("Value = %.3f (<-- right-click here)", value);
      if (ImGui_BeginPopupContextItem("item context menu")) {
        if (ImGui_Selectable("Set to zero")) {
          value = 0.0f;
        }
        if (ImGui_Selectable("Set to PI")) {
          value = 3.1415f;
        }
        ImGui_EndPopup();
      }
      static ImVec4 color = ImColor(0.8f, 0.5f, 1.0f, 1.0f);
      ImGui_ColorButton(color);
      if (ImGui_BeginPopupContextItem("color context menu")) {
        ImGui_Text("Edit color");
        ImGui_ColorEdit3("##edit", (float*)&color);
        if (ImGui_Button("Close")) {
          ImGui_CloseCurrentPopup();
        }
        ImGui_EndPopup();
      }
      ImGui_SameLine();
      ImGui_Text("(<-- right-click here)");
      ImGui_TreePop();
    }
    if (ImGui_TreeNode("Modals")) {
      ImGui_TextWrapped("Modal windows are like popups but the user cannot close them by clicking outside the window.");
      if (ImGui_Button("Delete..")) {
        ImGui_OpenPopup("Delete?");
      }
      if (ImGui_BeginPopupModal("Delete?", NULL, ImGuiWindowFlags_AlwaysAutoResize)) {
        ImGui_Text("All those beautiful files will be deleted.\nThis operation cannot be undone!\n\n");
        ImGui_Separator();
        //static int dummy_i = 0;
        //ImGui_Combo("Combo", &dummy_i, "Delete\0Delete harder\0");
        static bool dont_ask_me_next_time = false;
        ImGui_PushStyleVar2(ImGuiStyleVar_FramePadding, fVec2(0, 0));
        ImGui_Checkbox("Don't ask me next time", &dont_ask_me_next_time);
        ImGui_PopStyleVar();
        if (ImGui_Button("OK", fVec2(120, 0))) {
          ImGui_CloseCurrentPopup();
        }
        ImGui_SameLine();
        if (ImGui_Button("Cancel", fVec2(120, 0))) {
          ImGui_CloseCurrentPopup();
        }
        ImGui_EndPopup();
      }
      if (ImGui_Button("Stacked modals..")) {
        ImGui_OpenPopup("Stacked 1");
      }
      if (ImGui_BeginPopupModal("Stacked 1")) {
        ImGui_Text("Hello from Stacked The First");
        if (ImGui_Button("Another one..")) {
          ImGui_OpenPopup("Stacked 2");
        }
        if (ImGui_BeginPopupModal("Stacked 2")) {
          ImGui_Text("Hello from Stacked The Second");
          if (ImGui_Button("Close")) {
            ImGui_CloseCurrentPopup();
          }
          ImGui_EndPopup();
        }
        if (ImGui_Button("Close")) {
          ImGui_CloseCurrentPopup();
        }
        ImGui_EndPopup();
      }
      ImGui_TreePop();
    }
  }
  if (ImGui_CollapsingHeader("Columns")) {
    // Basic columns
    if (ImGui_TreeNode("Basic")) {
      ImGui_Text("Without border:");
      ImGui_Columns(3, "mycolumns3", false); // 3-ways, no border
      ImGui_Separator();
      for (int n = 0; n < 14; n++) {
        char label[32];
        sprintf(label, "Item %d", n);
        if (ImGui_Selectable(label)) {}
        //if (ImGui_Button(label, fVec2(-1,0))) {}
        ImGui_NextColumn();
      }
      ImGui_Columns(1, NULL);
      ImGui_Separator();
      ImGui_Text("With border:");
      ImGui_Columns(4, "mycolumns"); // 4-ways, with border
      ImGui_Separator();
      ImGui_Text("ID");
      ImGui_NextColumn();
      ImGui_Text("Name");
      ImGui_NextColumn();
      ImGui_Text("Path");
      ImGui_NextColumn();
      ImGui_Text("Flags");
      ImGui_NextColumn();
      ImGui_Separator();
      const char* names[3] = { "One", "Two", "Three" };
      const char* paths[3] = { "/path/one", "/path/two", "/path/three" };
      static int selected = -1;
      for (i = 0; i < 3; i++) {
        char label[32];
        sprintf(label, "%04d", i);
        if (ImGui_Selectable(label, selected == i, ImGuiSelectableFlags_SpanAllColumns)) {
          selected = i;
        }
        ImGui_NextColumn();
        ImGui_Text(names[i]);
        ImGui_NextColumn();
        ImGui_Text(paths[i]);
        ImGui_NextColumn();
        ImGui_Text("....");
        ImGui_NextColumn();
      }
      ImGui_Columns(1, NULL);
      ImGui_Separator();
      ImGui_TreePop();
    }
    // Scrolling columns
    /*
    if (ImGui_TreeNode("Scrolling"))
    {
    ImGui_BeginChild("##header", fVec2(0, ImGui_GetTextLineHeightWithSpacing()+ImGui_GetStyle()->ItemSpacing.y));
    ImGui_Columns(3);
    ImGui_Text("ID"); ImGui_NextColumn();
    ImGui_Text("Name"); ImGui_NextColumn();
    ImGui_Text("Path"); ImGui_NextColumn();
    ImGui_Columns(1);
    ImGui_Separator();
    ImGui_EndChild();
    ImGui_BeginChild("##scrollingregion", fVec2(0, 60));
    ImGui_Columns(3);
    for (i = 0; i < 10; i++)
    {
    ImGui_Text("%04d", i); ImGui_NextColumn();
    ImGui_Text("Foobar"); ImGui_NextColumn();
    ImGui_Text("/path/foobar/%04d/", i); ImGui_NextColumn();
    }
    ImGui_Columns(1);
    ImGui_EndChild();
    ImGui_TreePop();
    }
    */
    // Create multiple items in a same cell before switching to next column
    if (ImGui_TreeNode("Mixed items")) {
      ImGui_Columns(3, "mixed");
      ImGui_Separator();
      ImGui_Text("Hello");
      ImGui_Button("Banana");
      ImGui_NextColumn();
      ImGui_Text("ImGui");
      ImGui_Button("Apple");
      static float foo = 1.0f;
      ImGui_InputFloat("red", &foo, 0.05f, 0, 3);
      ImGui_Text("An extra line here.");
      ImGui_NextColumn();
      ImGui_Text("Sailor");
      ImGui_Button("Corniflower");
      static float bar = 1.0f;
      ImGui_InputFloat("blue", &bar, 0.05f, 0, 3);
      ImGui_NextColumn();
      if (ImGui_CollapsingHeader("Category A")) {
        ImGui_Text("Blah blah blah");
      }
      ImGui_NextColumn();
      if (ImGui_CollapsingHeader("Category B")) {
        ImGui_Text("Blah blah blah");
      }
      ImGui_NextColumn();
      if (ImGui_CollapsingHeader("Category C")) {
        ImGui_Text("Blah blah blah");
      }
      ImGui_NextColumn();
      ImGui_Columns(1, NULL);
      ImGui_Separator();
      ImGui_TreePop();
    }
    // Word wrapping
    if (ImGui_TreeNode("Word-wrapping")) {
      ImGui_Columns(2, "word-wrapping");
      ImGui_Separator();
      ImGui_TextWrapped("The quick brown fox jumps over the lazy dog.");
      ImGui_TextWrapped("Hello Left");
      ImGui_NextColumn();
      ImGui_TextWrapped("The quick brown fox jumps over the lazy dog.");
      ImGui_TextWrapped("Hello Right");
      ImGui_Columns(1, NULL);
      ImGui_Separator();
      ImGui_TreePop();
    }
    if (ImGui_TreeNode("Borders")) {
      static bool h_borders = true;
      static bool v_borders = true;
      ImGui_Checkbox("horizontal", &h_borders);
      ImGui_SameLine();
      ImGui_Checkbox("vertical", &v_borders);
      ImGui_Columns(4, NULL, v_borders);
      if (h_borders) {
        ImGui_Separator();
      }
      for (i = 0; i < 8; i++) {
        ImGui_Text("%c%c%c", 'a' + i, 'a' + i, 'a' + i);
        ImGui_NextColumn();
      }
      ImGui_Columns(1, NULL);
      if (h_borders) {
        ImGui_Separator();
      }
      ImGui_TreePop();
    }
    bool node_open = ImGui_TreeNode("Tree within single cell");
    ImGui_SameLine();
    ShowHelpMarker("NB: Tree node must be poped before ending the cell.\nThere's no storage of state per-cell.");
    if (node_open) {
      ImGui_Columns(2, "tree items");
      ImGui_Separator();
      if (ImGui_TreeNode("Hello")) {
        ImGui_BulletText("Sailor");
        ImGui_TreePop();
      }
      ImGui_NextColumn();
      if (ImGui_TreeNode("Bonjour")) {
        ImGui_BulletText("Marin");
        ImGui_TreePop();
      }
      ImGui_NextColumn();
      ImGui_Columns(1, NULL);
      ImGui_Separator();
      ImGui_TreePop();
    }
  }
  if (ImGui_CollapsingHeader("Filtering")) {
    static ImGuiTextFilter filter;
    ImGui_Text("Filter usage:\n"
        " \"\" display all lines\n"
        " \"xxx\" display lines containing \"xxx\"\n"
        " \"xxx,yyy\" display lines containing \"xxx\" or \"yyy\"\n"
        " \"-xxx\" hide lines containing \"xxx\"");
    filter.Draw();
    const char* lines[] = { "aaa1.c", "bbb1.c", "ccc1.c", "aaa2.cpp", "bbb2.cpp", "ccc2.cpp", "abc.h", "hello, world" };
    for (i = 0; i < IM_ARRAYSIZE(lines); i++)
      if (filter.PassFilter(lines[i])) {
        ImGui_BulletText("%s", lines[i]);
      }
  }
  if (ImGui_CollapsingHeader("Keyboard, Mouse & Focus")) {
    if (ImGui_TreeNode("Tabbing")) {
      ImGui_Text("Use TAB/SHIFT+TAB to cycle through keyboard editable fields.");
      static char buf[32] = "dummy";
      ImGui_InputText("1", buf, IM_ARRAYSIZE(buf));
      ImGui_InputText("2", buf, IM_ARRAYSIZE(buf));
      ImGui_InputText("3", buf, IM_ARRAYSIZE(buf));
      ImGui_PushAllowKeyboardFocus(false);
      ImGui_InputText("4 (tab skip)", buf, IM_ARRAYSIZE(buf));
      //ImGui_SameLine(); ShowHelperMarker("Use ImGui_PushAllowKeyboardFocus(bool)\nto disable tabbing through certain widgets.");
      ImGui_PopAllowKeyboardFocus();
      ImGui_InputText("5", buf, IM_ARRAYSIZE(buf));
      ImGui_TreePop();
    }
    if (ImGui_TreeNode("Focus from code")) {
      bool focus_1 = ImGui_Button("Focus on 1");
      ImGui_SameLine();
      bool focus_2 = ImGui_Button("Focus on 2");
      ImGui_SameLine();
      bool focus_3 = ImGui_Button("Focus on 3");
      int has_focus = 0;
      static char buf[128] = "click on a button to set focus";
      if (focus_1) {
        ImGui_SetKeyboardFocusHere();
      }
      ImGui_InputText("1", buf, IM_ARRAYSIZE(buf));
      if (ImGui_IsItemActive()) {
        has_focus = 1;
      }
      if (focus_2) {
        ImGui_SetKeyboardFocusHere();
      }
      ImGui_InputText("2", buf, IM_ARRAYSIZE(buf));
      if (ImGui_IsItemActive()) {
        has_focus = 2;
      }
      ImGui_PushAllowKeyboardFocus(false);
      if (focus_3) {
        ImGui_SetKeyboardFocusHere();
      }
      ImGui_InputText("3 (tab skip)", buf, IM_ARRAYSIZE(buf));
      if (ImGui_IsItemActive()) {
        has_focus = 3;
      }
      ImGui_PopAllowKeyboardFocus();
      if (has_focus) {
        ImGui_Text("Item with focus: %d", has_focus);
      }
      else {
        ImGui_Text("Item with focus: <none>");
      }
      ImGui_TextWrapped("Cursor & selection are preserved when refocusing last used item in code.");
      ImGui_TreePop();
    }
    if (ImGui_TreeNode("Dragging")) {
      ImGui_TextWrapped("You can use ImGui_GetItemActiveDragDelta() to query for the dragged amount on any widget.");
      ImGui_Button("Drag Me");
      if (ImGui_IsItemActive()) {
        // Draw a line between the button and the mouse cursor
        ImDrawList* draw_list = ImGui_GetWindowDrawList();
        ImDrawList_PushClipRectFullScreen(draw_list);
        ImDrawList_AddLine(draw_list, ImGui_CalcItemRectClosestPoint(ImGui_GetIO()->MousePos, true, -2.0f), ImGui_GetIO()->MousePos, ImGui_ColorConvertFloat4ToU32(ImGui_GetStyle()->Colors[ImGuiCol_Button]), 4.0f);
        ImDrawList_PopClipRect(draw_list);
        ImVec2 value_raw = ImGui_GetMouseDragDelta(0, 0.0f);
        ImVec2 value_with_lock_threshold = ImGui_GetMouseDragDelta(0);
        ImVec2 mouse_delta = ImGui_GetIO()->MouseDelta;
        ImGui_SameLine();
        ImGui_Text("Raw (%.1f, %.1f), WithLockThresold (%.1f, %.1f), MouseDelta (%.1f, %.1f)", value_raw.x, value_raw.y, value_with_lock_threshold.x, value_with_lock_threshold.y, mouse_delta.x, mouse_delta.y);
      }
      ImGui_TreePop();
    }
    if (ImGui_TreeNode("Keyboard & Mouse State")) {
      int i;
      ImGuiIO* io = ImGui_GetIO();
      ImGui_Text("MousePos: (%g, %g)", io->MousePos.x, io->MousePos.y);
      ImGui_Text("Mouse down:");
      for (i = 0; i < IM_ARRAYSIZE(io->MouseDown); i++) {
        if (io->MouseDownDuration[i] >= 0.0f) {
          ImGui_SameLine();
          ImGui_Text("b%d (%.02f secs)", i, io->MouseDownDuration[i]);
        }
      }
      ImGui_Text("Mouse clicked:");
      for (i = 0; i < IM_ARRAYSIZE(io->MouseDown); i++) {
        if (ImGui_IsMouseClicked(i)) {
          ImGui_SameLine();
          ImGui_Text("b%d", i);
        }
      }
      ImGui_Text("Mouse dbl-clicked:");
      for (i = 0; i < IM_ARRAYSIZE(io->MouseDown); i++) {
        if (ImGui_IsMouseDoubleClicked(i)) {
          ImGui_SameLine();
          ImGui_Text("b%d", i);
        }
      }
      ImGui_Text("Mouse released:");
      for (i = 0; i < IM_ARRAYSIZE(io->MouseDown); i++) {
        if (ImGui_IsMouseReleased(i)) {
          ImGui_SameLine();
          ImGui_Text("b%d", i);
        }
      }
      ImGui_Text("MouseWheel: %.1f", io->MouseWheel);
      ImGui_Text("Keys down:");
      for (i = 0; i < IM_ARRAYSIZE(io->KeysDown); i++) {
        if (io->KeysDownDuration[i] >= 0.0f) {
          ImGui_SameLine();
          ImGui_Text("%d (%.02f secs)", i, io->KeysDownDuration[i]);
        }
      }
      ImGui_Text("Keys pressed:");
      for (i = 0; i < IM_ARRAYSIZE(io->KeysDown); i++) {
        if (ImGui_IsKeyPressed(i)) {
          ImGui_SameLine();
          ImGui_Text("%d", i);
        }
      }
      ImGui_Text("Keys release:");
      for (i = 0; i < IM_ARRAYSIZE(io->KeysDown); i++) {
        if (ImGui_IsKeyReleased(i)) {
          ImGui_SameLine();
          ImGui_Text("%d", i);
        }
      }
      ImGui_Text("KeyMods: %s%s%s%s", io->KeyCtrl ? "CTRL " : "", io->KeyShift ? "SHIFT " : "", io->KeyAlt ? "ALT " : "", io->KeySuper ? "SUPER " : "");
      ImGui_Text("WantCaptureMouse: %s", io->WantCaptureMouse ? "true" : "false");
      ImGui_Text("WantCaptureKeyboard: %s", io->WantCaptureKeyboard ? "true" : "false");
      ImGui_Text("WantTextInput: %s", io->WantTextInput ? "true" : "false");
      ImGui_Button("Hovering me sets the\nkeyboard capture flag");
      if (ImGui_IsItemHovered()) {
        ImGui_CaptureKeyboardFromApp(true);
      }
      ImGui_SameLine();
      ImGui_Button("Holding me clears the\nthe keyboard capture flag");
      if (ImGui_IsItemActive()) {
        ImGui_CaptureKeyboardFromApp(false);
      }
      ImGui_TreePop();
    }
    if (ImGui_TreeNode("Mouse cursors")) {
      ImGui_TextWrapped("Your application can render a different mouse cursor based on what ImGui_GetMouseCursor() returns. You can also set io->MouseDrawCursor to ask ImGui to render the cursor for you in software.");
      ImGui_Checkbox("io->MouseDrawCursor", &ImGui_GetIO()->MouseDrawCursor);
      ImGui_Text("Hover to see mouse cursors:");
      for (i = 0; i < ImGuiMouseCursor_Count_; i++) {
        char label[32];
        sprintf(label, "Mouse cursor %d", i);
        ImGui_Bullet();
        ImGui_Selectable(label, false);
        if (ImGui_IsItemHovered()) {
          ImGui_SetMouseCursor(i);
        }
      }
      ImGui_TreePop();
    }
  }
  ImGui_End();
}
void ImGui_ShowStyleEditor(ImGuiStyle* ref)
{
  int i;
  ImGuiStyle* style = ImGui_GetStyle();
  // You can pass in a reference ImGuiStyle structure to compare to, revert to and save to (else it compares to the default style)
  ImGuiStyle default_style_; // Default style
  ImGuiStyle* default_style = &default_style_;
  if (ImGui_Button("Revert Style")) {
    style = ref ? ref : default_style;
  }
  if (ref) {
    ImGui_SameLine();
    if (ImGui_Button("Save Style")) {
      ref = style;
    }
  }
  ImGui_PushItemWidth(ImGui_GetWindowWidth() * 0.55f);
  if (ImGui_TreeNode("Rendering")) {
    ImGui_Checkbox("Anti-aliased lines", &style->AntiAliasedLines);
    ImGui_Checkbox("Anti-aliased shapes", &style->AntiAliasedShapes);
    ImGui_PushItemWidth(100);
    ImGui_DragFloat("Curve Tessellation Tolerance", &style->CurveTessellationTol, 0.02f, 0.10f, FLT_MAX, NULL, 2.0f);
    if (style->CurveTessellationTol < 0.0f) {
      style->CurveTessellationTol = 0.10f;
    }
    ImGui_DragFloat("Global Alpha", &style->Alpha, 0.005f, 0.20f, 1.0f, "%.2f"); // Not exposing zero here so user doesn't "lose" the UI (zero alpha clips all widgets). But application code could have a toggle to switch between zero and non-zero.
    ImGui_PopItemWidth();
    ImGui_TreePop();
  }
  if (ImGui_TreeNode("Settings")) {
    ImGui_SliderFloat2("WindowPadding", (float*)&style->WindowPadding, 0.0f, 20.0f, "%.0f");
    ImGui_SliderFloat("WindowRounding", &style->WindowRounding, 0.0f, 16.0f, "%.0f");
    ImGui_SliderFloat("ChildWindowRounding", &style->ChildWindowRounding, 0.0f, 16.0f, "%.0f");
    ImGui_SliderFloat2("FramePadding", (float*)&style->FramePadding, 0.0f, 20.0f, "%.0f");
    ImGui_SliderFloat("FrameRounding", &style->FrameRounding, 0.0f, 16.0f, "%.0f");
    ImGui_SliderFloat2("ItemSpacing", (float*)&style->ItemSpacing, 0.0f, 20.0f, "%.0f");
    ImGui_SliderFloat2("ItemInnerSpacing", (float*)&style->ItemInnerSpacing, 0.0f, 20.0f, "%.0f");
    ImGui_SliderFloat2("TouchExtraPadding", (float*)&style->TouchExtraPadding, 0.0f, 10.0f, "%.0f");
    ImGui_SliderFloat("IndentSpacing", &style->IndentSpacing, 0.0f, 30.0f, "%.0f");
    ImGui_SliderFloat("ScrollbarSize", &style->ScrollbarSize, 1.0f, 20.0f, "%.0f");
    ImGui_SliderFloat("ScrollbarRounding", &style->ScrollbarRounding, 0.0f, 16.0f, "%.0f");
    ImGui_SliderFloat("GrabMinSize", &style->GrabMinSize, 1.0f, 20.0f, "%.0f");
    ImGui_SliderFloat("GrabRounding", &style->GrabRounding, 0.0f, 16.0f, "%.0f");
    ImGui_Text("Alignment");
    ImGui_SliderFloat2("WindowTitleAlign", (float*)&style->WindowTitleAlign, 0.0f, 1.0f, "%.2f");
    ImGui_SliderFloat2("ButtonTextAlign", (float*)&style->ButtonTextAlign, 0.0f, 1.0f, "%.2f");
    ImGui_SameLine();
    ShowHelpMarker("Alignment applies when a button is larger than its text content.");
    ImGui_TreePop();
  }
  if (ImGui_TreeNode("Colors")) {
    static int output_dest = 0;
    static bool output_only_modified = false;
    if (ImGui_Button("Copy Colors")) {
      if (output_dest == 0) {
        ImGui_LogToClipboard();
      }
      else {
        ImGui_LogToTTY();
      }
      ImGui_LogText("ImGuiStyle* style = ImGui_GetStyle();" IM_NEWLINE);
      for (i = 0; i < ImGuiCol_COUNT; i++) {
        const ImVec4& col = style->Colors[i];
        const char* name = ImGui_GetStyleColName(i);
        if (!output_only_modified || memcmp(&col, (ref ? &ref->Colors[i] : &default_style->Colors[i]), sizeof(ImVec4)) != 0) {
          ImGui_LogText("style->Colors[ImGuiCol_%s]%*s= fVec4(%.2ff, %.2ff, %.2ff, %.2ff);" IM_NEWLINE, name, 22 - (int)strlen(name), "", col.x, col.y, col.z, col.w);
        }
      }
      ImGui_LogFinish();
    }
    ImGui_SameLine();
    ImGui_PushItemWidth(120);
    ImGui_Combo("##output_type", &output_dest, "To Clipboard\0To TTY\0");
    ImGui_PopItemWidth();
    ImGui_SameLine();
    ImGui_Checkbox("Only Modified Fields", &output_only_modified);
    static ImGuiColorEditMode edit_mode = ImGuiColorEditMode_RGB;
    ImGui_RadioButton("RGB", &edit_mode, ImGuiColorEditMode_RGB);
    ImGui_SameLine();
    ImGui_RadioButton("HSV", &edit_mode, ImGuiColorEditMode_HSV);
    ImGui_SameLine();
    ImGui_RadioButton("HEX", &edit_mode, ImGuiColorEditMode_HEX);
    //ImGui_Text("Tip: Click on colored square to change edit mode.");
    static ImGuiTextFilter filter;
    filter.Draw("Filter colors", 200);
    ImGui_BeginChild("#colors", fVec2(0, 300), true, ImGuiWindowFlags_AlwaysVerticalScrollbar);
    ImGui_PushItemWidth(-160);
    ImGui_ColorEditMode(edit_mode);
    for (i = 0; i < ImGuiCol_COUNT; i++) {
      const char* name = ImGui_GetStyleColName(i);
      if (!filter.PassFilter(name)) {
        continue;
      }
      ImGui_PushID(i);
      ImGui_ColorEdit4(name, (float*)&style->Colors[i], true);
      if (memcmp(&style->Colors[i], (ref ? &ref->Colors[i] : &default_style->Colors[i]), sizeof(ImVec4)) != 0) {
        ImGui_SameLine();
        if (ImGui_Button("Revert")) {
          style->Colors[i] = ref ? ref->Colors[i] : default_style->Colors[i];
        }
        if (ref) {
          ImGui_SameLine();
          if (ImGui_Button("Save")) {
            ref->Colors[i] = style->Colors[i];
          }
        }
      }
      ImGui_PopID();
    }
    ImGui_PopItemWidth();
    ImGui_EndChild();
    ImGui_TreePop();
  }
  if (ImGui_TreeNode("Fonts", "Fonts (%d)", ImGui_GetIO()->Fonts->Fonts.Size)) {
    ImGui_SameLine();
    ShowHelpMarker("Tip: Load fonts with io->Fonts->AddFontFromFileTTF()\nbefore calling io->Fonts->GetTex* functions.");
    ImFontAtlas* atlas = ImGui_GetIO()->Fonts;
    if (ImGui_TreeNode("Atlas texture", "Atlas texture (%dx%d pixels)", atlas->TexWidth, atlas->TexHeight)) {
      ImGui_Image(atlas->TexID, fVec2((float)atlas->TexWidth, (float)atlas->TexHeight), fVec2(0, 0), fVec2(1, 1), ImColor(255, 255, 255, 255), ImColor(255, 255, 255, 128));
      ImGui_TreePop();
    }
    ImGui_PushItemWidth(100);
    for (i = 0; i < atlas->Fonts.Size; i++) {
      ImFont* font = atlas->Fonts[i];
      ImGui_BulletText("Font %d: \'%s\', %.2f px, %d glyphs", i, font->ConfigData ? font->ConfigData[0].Name : "", font->FontSize, font->Glyphs.Size);
      ImGui_TreePush((void*)(intptr_t)i);
      if (i > 0) {
        ImGui_SameLine();
        if (ImGui_SmallButton("Set as default")) {
          atlas->Fonts[i] = atlas->Fonts[0];
          atlas->Fonts[0] = font;
        }
      }
      ImGui_PushFont(font);
      ImGui_Text("The quick brown fox jumps over the lazy dog");
      ImGui_PopFont();
      if (ImGui_TreeNode("Details")) {
        ImGui_DragFloat("Font scale", &font->Scale, 0.005f, 0.3f, 2.0f, "%.1f"); // Scale only this font
        ImGui_SameLine();
        ShowHelpMarker("Note than the default embedded font is NOT meant to be scaled.\n\nFont are currently rendered into bitmaps at a given size at the time of building the atlas. You may oversample them to get some flexibility with scaling. You can also render at multiple sizes and select which one to use at runtime.\n\n(Glimmer of hope: the atlas system should hopefully be rewritten in the future to make scaling more natural and automatic.)");
        ImGui_Text("Ascent: %f, Descent: %f, Height: %f", font->Ascent, font->Descent, font->Ascent - font->Descent);
        ImGui_Text("Fallback character: '%c' (%d)", font->FallbackChar, font->FallbackChar);
        for (int config_i = 0; config_i < font->ConfigDataCount; config_i++) {
          ImFontConfig* cfg = &font->ConfigData[config_i];
          ImGui_BulletText("Input %d: \'%s\'\nOversample: (%d,%d), PixelSnapH: %d", config_i, cfg->Name, cfg->OversampleH, cfg->OversampleV, cfg->PixelSnapH);
        }
        if (ImGui_TreeNode("Glyphs", "Glyphs (%d)", font->Glyphs.Size)) {
          // Display all glyphs of the fonts in separate pages of 256 characters
          const ImFontGlyph* glyph_fallback = font->FallbackGlyph; // Forcefully/dodgily make FindGlyph() return NULL on fallback, which isn't the default behavior.
          font->FallbackGlyph = NULL;
          for (int base = 0; base < 0x10000; base += 256) {
            int count = 0;
            for (int n = 0; n < 256; n++) {
              count += ImFont_FindGlyph(font, (ImWchar)(base + n)) ? 1 : 0;
            }
            if (count > 0 && ImGui_TreeNode((void*)(intptr_t)base, "U+%04X..U+%04X (%d %s)", base, base + 255, count, count > 1 ? "glyphs" : "glyph")) {
              float cell_spacing = style->ItemSpacing.y;
              ImVec2 cell_size = fVec2(font->FontSize * 1, font->FontSize * 1);
              ImVec2 base_pos = ImGui_GetCursorScreenPos();
              ImDrawList* draw_list = ImGui_GetWindowDrawList();
              for (int n = 0; n < 256; n++) {
                ImVec2 cell_p1 = fVec2(base_pos.x + (n % 16) * (cell_size.x + cell_spacing), base_pos.y + (n / 16) * (cell_size.y + cell_spacing));
                ImVec2 cell_p2 = fVec2(cell_p1.x + cell_size.x, cell_p1.y + cell_size.y);
                const ImFontGlyph* glyph = ImFont_FindGlyph(font, (ImWchar)(base + n));;
                ImDrawList_AddRect(draw_list, cell_p1, cell_p2, glyph ? IM_COL32(255, 255, 255, 100) : IM_COL32(255, 255, 255, 50));
                ImFont_RenderChar(font, draw_list, cell_size.x, cell_p1, ImGui_GetColorU32(ImGuiCol_Text), (ImWchar)(base + n)); // We use ImFont::RenderChar as a shortcut because we don't have UTF-8 conversion functions available to generate a string.
                if (glyph && ImGui_IsMouseHoveringRect(cell_p1, cell_p2)) {
                  ImGui_BeginTooltip();
                  ImGui_Text("Codepoint: U+%04X", base + n);
                  ImGui_Separator();
                  ImGui_Text("XAdvance+1: %.1f", glyph->XAdvance);
                  ImGui_Text("Pos: (%.2f,%.2f)->(%.2f,%.2f)", glyph->X0, glyph->Y0, glyph->X1, glyph->Y1);
                  ImGui_Text("UV: (%.3f,%.3f)->(%.3f,%.3f)", glyph->U0, glyph->V0, glyph->U1, glyph->V1);
                  ImGui_EndTooltip();
                }
              }
              ImGui_Dummy(fVec2((cell_size.x + cell_spacing) * 16, (cell_size.y + cell_spacing) * 16));
              ImGui_TreePop();
            }
          }
          font->FallbackGlyph = glyph_fallback;
          ImGui_TreePop();
        }
        ImGui_TreePop();
      }
      ImGui_TreePop();
    }
    static float window_scale = 1.0f;
    ImGui_DragFloat("this window scale", &window_scale, 0.005f, 0.3f, 2.0f, "%.1f"); // scale only this window
    ImGui_DragFloat("global scale", &ImGui_GetIO()->FontGlobalScale, 0.005f, 0.3f, 2.0f, "%.1f"); // scale everything
    ImGui_PopItemWidth();
    ImGui_SetWindowFontScale(window_scale);
    ImGui_TreePop();
  }
  ImGui_PopItemWidth();
}
static void ShowExampleAppMainMenuBar()
{
  if (ImGui_BeginMainMenuBar()) {
    if (ImGui_BeginMenu("File")) {
      ShowExampleMenuFile();
      ImGui_EndMenu();
    }
    if (ImGui_BeginMenu("Edit")) {
      if (ImGui_MenuItem("Undo", "CTRL+Z")) {}
      if (ImGui_MenuItem("Redo", "CTRL+Y", false, false)) {} // Disabled item
      ImGui_Separator();
      if (ImGui_MenuItem("Cut", "CTRL+X")) {}
      if (ImGui_MenuItem("Copy", "CTRL+C")) {}
      if (ImGui_MenuItem("Paste", "CTRL+V")) {}
      ImGui_EndMenu();
    }
    ImGui_EndMainMenuBar();
  }
}
static void ShowExampleMenuFile()
{
  int i;
  ImGui_MenuItem("(dummy menu)", NULL, false, false);
  if (ImGui_MenuItem("New")) {}
  if (ImGui_MenuItem("Open", "Ctrl+O")) {}
  if (ImGui_BeginMenu("Open Recent")) {
    ImGui_MenuItem("fish_hat.c");
    ImGui_MenuItem("fish_hat.inl");
    ImGui_MenuItem("fish_hat.h");
    if (ImGui_BeginMenu("More..")) {
      ImGui_MenuItem("Hello");
      ImGui_MenuItem("Sailor");
      if (ImGui_BeginMenu("Recurse..")) {
        ShowExampleMenuFile();
        ImGui_EndMenu();
      }
      ImGui_EndMenu();
    }
    ImGui_EndMenu();
  }
  if (ImGui_MenuItem("Save", "Ctrl+S")) {}
  if (ImGui_MenuItem("Save As..")) {}
  ImGui_Separator();
  if (ImGui_BeginMenu("Options")) {
    static bool enabled = true;
    ImGui_MenuItem("Enabled", "", &enabled);
    ImGui_BeginChild("child", fVec2(0, 60), true, 0);
    for (i = 0; i < 10; i++) {
      ImGui_Text("Scrolling Text %d", i);
    }
    ImGui_EndChild();
    static float f = 0.5f;
    static int n = 0;
    ImGui_SliderFloat("Value", &f, 0.0f, 1.0f);
    ImGui_InputFloat("Input", &f, 0.1f);
    ImGui_Combo("Combo", &n, "Yes\0No\0Maybe\0\0");
    ImGui_EndMenu();
  }
  if (ImGui_BeginMenu("Colors")) {
    for (i = 0; i < ImGuiCol_COUNT; i++) {
      ImGui_MenuItem(ImGui_GetStyleColName((ImGuiCol)i));
    }
    ImGui_EndMenu();
  }
  if (ImGui_BeginMenu("Disabled", false)) { // Disabled
    IM_ASSERT(0);
  }
  if (ImGui_MenuItem("Checked", NULL, true)) {}
  if (ImGui_MenuItem("Quit", "Alt+F4")) {}
}
static void ShowExampleAppAutoResize(bool* p_open)
{
  int i;
  if (!ImGui_Begin("Example: Auto-resizing window", p_open, ImGuiWindowFlags_AlwaysAutoResize)) {
    ImGui_End();
    return;
  }
  static int lines = 10;
  ImGui_Text("Window will resize every-frame to the size of its content.\nNote that you probably don't want to query the window size to\noutput your content because that would create a feedback loop.");
  ImGui_SliderInt("Number of lines", &lines, 1, 20);
  for (i = 0; i < lines; i++) {
    ImGui_Text("%*sThis is line %d", i * 4, "", i); // Pad with space to extend size horizontally
  }
  ImGui_End();
}
static void ShowExampleAppConstrainedResize(bool* p_open)
{
  int i;
  struct CustomConstraints { // Helper functions to demonstrate programmatic constraints
    static void Square(ImGuiSizeConstraintCallbackData* data) {
      data->DesiredSize = fVec2(IM_MAX(data->DesiredSize.x, data->DesiredSize.y), IM_MAX(data->DesiredSize.x, data->DesiredSize.y));
    }
    static void Step(ImGuiSizeConstraintCallbackData* data) {
      float step = (float)(int)(intptr_t)data->UserData;
      data->DesiredSize = fVec2((int)(data->DesiredSize.x / step + 0.5f) * step, (int)(data->DesiredSize.y / step + 0.5f) * step);
    }
  };
  static int type = 0;
  if (type == 0) {
    ImGui_SetNextWindowSizeConstraints(fVec2(-1, 0), fVec2(-1, FLT_MAX), NULL, NULL); // Vertical only
  }
  if (type == 1) {
    ImGui_SetNextWindowSizeConstraints(fVec2(0, -1), fVec2(FLT_MAX, -1), NULL, NULL); // Horizontal only
  }
  if (type == 2) {
    ImGui_SetNextWindowSizeConstraints(fVec2(100, 100), fVec2(FLT_MAX, FLT_MAX), NULL, NULL); // Width > 100, Height > 100
  }
  if (type == 3) {
    ImGui_SetNextWindowSizeConstraints(fVec2(300, 0), fVec2(400, FLT_MAX), NULL, NULL); // Width 300-400
  }
  if (type == 4) {
    ImGui_SetNextWindowSizeConstraints(fVec2(0, 0), fVec2(FLT_MAX, FLT_MAX), CustomConstraints::Square, NULL); // Always Square
  }
  if (type == 5) {
    ImGui_SetNextWindowSizeConstraints(fVec2(0, 0), fVec2(FLT_MAX, FLT_MAX), CustomConstraints::Step, (void*)100); // Fixed Step
  }
  if (ImGui_Begin("Example: Constrained Resize", p_open, 0)) {
    const char* desc[] = {
      "Resize vertical only",
      "Resize horizontal only",
      "Width > 100, Height > 100",
      "Width 300-400",
      "Custom: Always Square",
      "Custom: Fixed Steps (100)",
    };
    ImGui_Combo("Constraint", &type, desc, IM_ARRAYSIZE(desc));
    if (ImGui_Button("200x200")) {
      ImGui_SetWindowSize(fVec2(200, 200));
    }
    ImGui_SameLine();
    if (ImGui_Button("500x500")) {
      ImGui_SetWindowSize(fVec2(500, 500));
    }
    ImGui_SameLine();
    if (ImGui_Button("800x200")) {
      ImGui_SetWindowSize(fVec2(800, 200));
    }
    for (i = 0; i < 10; i++) {
      ImGui_Text("Hello, sailor! Making this line long enough for the example.");
    }
  }
  ImGui_End();
}
static void ShowExampleAppFixedOverlay(bool* p_open)
{
  ImGui_SetNextWindowPos(fVec2(10, 10), 0);
  if (!ImGui_BeginEx("Example: Fixed Overlay", p_open, fVec2(0, 0), 0.3f, ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoSavedSettings)) {
    ImGui_End();
    return;
  }
  ImGui_Text("Simple overlay\non the top-left side of the screen.");
  ImGui_Separator();
  ImGui_Text("Mouse Position: (%.1f,%.1f)", ImGui_GetIO()->MousePos.x, ImGui_GetIO()->MousePos.y);
  ImGui_End();
}
static void ShowExampleAppManipulatingWindowTitle(bool*)
{
  // By default, Windows are uniquely identified by their title.
  // You can use the "##" and "###" markers to manipulate the display/ID. Read FAQ at the top of this file!
  // Using "##" to display same title but have unique identifier.
  ImGui_SetNextWindowPos(fVec2(100, 100), ImGuiSetCond_FirstUseEver);
  ImGui_Begin("Same title as another window##1", NULL, 0);
  ImGui_Text("This is window 1.\nMy title is the same as window 2, but my identifier is unique.");
  ImGui_End();
  ImGui_SetNextWindowPos(fVec2(100, 200), ImGuiSetCond_FirstUseEver);
  ImGui_Begin("Same title as another window##2", NULL, 0);
  ImGui_Text("This is window 2.\nMy title is the same as window 1, but my identifier is unique.");
  ImGui_End();
  // Using "###" to display a changing title but keep a static identifier "AnimatedTitle"
  char buf[128];
  sprintf(buf, "Animated title %c %d###AnimatedTitle", "|/-\\"[(int)(ImGui_GetTime() / 0.25f) & 3], rand());
  ImGui_SetNextWindowPos(fVec2(100, 300), ImGuiSetCond_FirstUseEver);
  ImGui_Begin(buf, NULL, 0);
  ImGui_Text("This window has a changing title.");
  ImGui_End();
}
static void ShowExampleAppCustomRendering(bool* p_open)
{
  int i;
  ImGui_SetNextWindowSize(fVec2(350, 560), ImGuiSetCond_FirstUseEver);
  if (!ImGui_Begin("Example: Custom rendering", p_open, 0)) {
    ImGui_End();
    return;
  }
  // Tip: If you do a lot of custom rendering, you probably want to use your own geometrical types and benefit of overloaded operators, etc.
  // Define IM_VEC2_CLASS_EXTRA in imconfig.h to create implicit conversions between your types and ImVec2/ImVec4.
  // ImGui defines overloaded operators but they are internal to imgui.cpp and not exposed outside (to avoid messing with your types)
  // In this example we are not using the maths operators!
  ImDrawList* draw_list = ImGui_GetWindowDrawList();
  // Primitives
  ImGui_Text("Primitives");
  static float sz = 36.0f;
  static ImVec4 col = fVec4(1.0f, 1.0f, 0.4f, 1.0f);
  ImGui_DragFloat("Size", &sz, 0.2f, 2.0f, 72.0f, "%.0f");
  ImGui_ColorEdit3("Color", &col.x);
  {
    const ImVec2 p = ImGui_GetCursorScreenPos();
    const ImU32 col32 = ImGui_ColorConvertFloat4ToU32(col);
    float x = p.x + 4.0f, y = p.y + 4.0f, spacing = 8.0f;
    for (int n = 0; n < 2; n++) {
      float thickness = (n == 0) ? 1.0f : 4.0f;
      ImDrawList_AddCircle(draw_list, fVec2(x + sz * 0.5f, y + sz * 0.5f), sz * 0.5f, col32, 20, thickness);
      x += sz + spacing;
      ImDrawList_AddRect(draw_list, fVec2(x, y), fVec2(x + sz, y + sz), col32, 0.0f, ~0, thickness);
      x += sz + spacing;
      ImDrawList_AddRect(draw_list, fVec2(x, y), fVec2(x + sz, y + sz), col32, 10.0f, ~0, thickness);
      x += sz + spacing;
      ImDrawList_AddTriangle(draw_list, fVec2(x + sz * 0.5f, y), fVec2(x + sz, y + sz - 0.5f), fVec2(x, y + sz - 0.5f), col32, thickness);
      x += sz + spacing;
      ImDrawList_AddLine(draw_list, fVec2(x, y), fVec2(x + sz, y), col32, thickness);
      x += sz + spacing;
      ImDrawList_AddLine(draw_list, fVec2(x, y), fVec2(x + sz, y + sz), col32, thickness);
      x += sz + spacing;
      ImDrawList_AddLine(draw_list, fVec2(x, y), fVec2(x, y + sz), col32, thickness);
      x += spacing;
      ImDrawList_AddBezierCurve(draw_list, fVec2(x, y), fVec2(x + sz * 1.3f, y + sz * 0.3f), fVec2(x + sz - sz * 1.3f, y + sz - sz * 0.3f), fVec2(x + sz, y + sz), col32, thickness);
      x = p.x + 4;
      y += sz + spacing;
    }
    ImDrawList_AddCircleFilled(draw_list, fVec2(x + sz * 0.5f, y + sz * 0.5f), sz * 0.5f, col32, 32);
    x += sz + spacing;
    ImDrawList_AddRectFilled(draw_list, fVec2(x, y), fVec2(x + sz, y + sz), col32);
    x += sz + spacing;
    ImDrawList_AddRectFilled(draw_list, fVec2(x, y), fVec2(x + sz, y + sz), col32, 10.0f);
    x += sz + spacing;
    ImDrawList_AddTriangleFilled(draw_list, fVec2(x + sz * 0.5f, y), fVec2(x + sz, y + sz - 0.5f), fVec2(x, y + sz - 0.5f), col32);
    x += sz + spacing;
    ImDrawList_AddRectFilledMultiColor(draw_list, fVec2(x, y), fVec2(x + sz, y + sz), ImColorU32RGB(0, 0, 0), ImColorU32RGB(255, 0, 0), ImColorU32RGB(255, 255, 0), ImColorU32RGB(0, 255, 0));
    ImGui_Dummy(fVec2((sz + spacing) * 8, (sz + spacing) * 3));
  }
  ImGui_Separator();
  {
    static ImVector<ImVec2> points;
    static bool adding_line = false;
    ImGui_Text("Canvas example");
    if (ImGui_Button("Clear")) {
      points.clear();
    }
    if (points.Size >= 2) {
      ImGui_SameLine();
      if (ImGui_Button("Undo")) {
        points.pop_back();
        points.pop_back();
      }
    }
    ImGui_Text("Left-click and drag to add lines,\nRight-click to undo");
    // Here we are using InvisibleButton() as a convenience to 1) advance the cursor and 2) allows us to use IsItemHovered()
    // However you can draw directly and poll mouse/keyboard by yourself. You can manipulate the cursor using GetCursorPos() and SetCursorPos().
    // If you only use the ImDrawList API, you can notify the owner window of its extends by using SetCursorPos(max).
    ImVec2 canvas_pos = ImGui_GetCursorScreenPos(); // ImDrawList API uses screen coordinates!
    ImVec2 canvas_size = ImGui_GetContentRegionAvail(); // Resize canvas to what's available
    if (canvas_size.x < 50.0f) {
      canvas_size.x = 50.0f;
    }
    if (canvas_size.y < 50.0f) {
      canvas_size.y = 50.0f;
    }
    ImDrawList_AddRectFilledMultiColor(draw_list, canvas_pos, fVec2(canvas_pos.x + canvas_size.x, canvas_pos.y + canvas_size.y), ImColorU32RGB(50, 50, 50), ImColorU32RGB(50, 50, 60), ImColorU32RGB(60, 60, 70), ImColorU32RGB(50, 50, 60));
    ImDrawList_AddRect(draw_list, canvas_pos, fVec2(canvas_pos.x + canvas_size.x, canvas_pos.y + canvas_size.y), ImColorU32RGB(255, 255, 255));
    bool adding_preview = false;
    ImGui_InvisibleButton("canvas", canvas_size);
    if (ImGui_IsItemHovered()) {
      ImVec2 mouse_pos_in_canvas = fVec2(ImGui_GetIO()->MousePos.x - canvas_pos.x, ImGui_GetIO()->MousePos.y - canvas_pos.y);
      if (!adding_line && ImGui_IsMouseClicked(0)) {
        points.push_back(mouse_pos_in_canvas);
        adding_line = true;
      }
      if (adding_line) {
        adding_preview = true;
        points.push_back(mouse_pos_in_canvas);
        if (!ImGui_GetIO()->MouseDown[0]) {
          adding_line = adding_preview = false;
        }
      }
      if (ImGui_IsMouseClicked(1) && !points.empty()) {
        adding_line = adding_preview = false;
        points.pop_back();
        points.pop_back();
      }
    }
    ImDrawList_PushClipRect(draw_list, canvas_pos, fVec2(canvas_pos.x + canvas_size.x, canvas_pos.y + canvas_size.y), NULL); // clip lines within the canvas (if we resize it, etc.)
    for (i = 0; i < points.Size - 1; i += 2) {
      ImDrawList_AddLine(draw_list, fVec2(canvas_pos.x + points[i].x, canvas_pos.y + points[i].y), fVec2(canvas_pos.x + points[i + 1].x, canvas_pos.y + points[i + 1].y), 0xFF00FFFF, 2.0f);
    }
    ImDrawList_PopClipRect(draw_list);
    if (adding_preview) {
      points.pop_back();
    }
  }
  ImGui_End();
}
// For the console example, here we are using a more C++ like approach of declaring a class to hold the data and the functions.
struct ExampleAppConsole {
  char InputBuf[256];
  ImVector<char*> Items;
  bool ScrollToBottom;
  ImVector<char*> History;
  int HistoryPos; // -1: new line, 0..History.Size-1 browsing history.
  ImVector<const char*> Commands;
  ExampleAppConsole() {
    ClearLog();
    memset(InputBuf, 0, sizeof(InputBuf));
    HistoryPos = -1;
    Commands.push_back("HELP");
    Commands.push_back("HISTORY");
    Commands.push_back("CLEAR");
    Commands.push_back("CLASSIFY"); // "classify" is here to provide an example of "C"+[tab] completing to "CL" and displaying matches.
    AddLog("Welcome to ImGui!");
  }
  ~ExampleAppConsole() {
    int i;
    ClearLog();
    for (i = 0; i < History.Size; i++) {
      free(History[i]);
    }
  }
  // Portable helpers
  static int Stricmp(const char* str1, const char* str2) {
    int d;
    while ((d = toupper(*str2) - toupper(*str1)) == 0 && *str1) {
      str1++;
      str2++;
    }
    return d;
  }
  static int Strnicmp(const char* str1, const char* str2, int n) {
    int d = 0;
    while (n > 0 && (d = toupper(*str2) - toupper(*str1)) == 0 && *str1) {
      str1++;
      str2++;
      n--;
    }
    return d;
  }
  static char* Strdup(const char* str) {
    size_t len = strlen(str) + 1;
    void* buff = malloc(len);
    return (char*)memcpy(buff, (const void*)str, len);
  }
  void ClearLog() {
    int i;
    for (i = 0; i < Items.Size; i++) {
      free(Items[i]);
    }
    Items.clear();
    ScrollToBottom = true;
  }
  void AddLog(const char* fmt, ...) IM_PRINTFARGS(2) {
    char buf[1024];
    va_list args;
    va_start(args, fmt);
    vsnprintf(buf, IM_ARRAYSIZE(buf), fmt, args);
    buf[IM_ARRAYSIZE(buf) - 1] = 0;
    va_end(args);
    Items.push_back(Strdup(buf));
    ScrollToBottom = true;
  }
  void Draw(const char* title, bool* p_open) {
    int i;
    ImGui_SetNextWindowSize(fVec2(520, 600), ImGuiSetCond_FirstUseEver);
    if (!ImGui_Begin(title, p_open, 0)) {
      ImGui_End();
      return;
    }
    ImGui_TextWrapped("This example implements a console with basic coloring, completion and history. A more elaborate implementation may want to store entries along with extra data such as timestamp, emitter, etc.");
    ImGui_TextWrapped("Enter 'HELP' for help, press TAB to use text completion.");
    // TODO: display items starting from the bottom
    if (ImGui_SmallButton("Add Dummy Text")) {
      AddLog("%d some text", Items.Size);
      AddLog("some more text");
      AddLog("display very important message here!");
    }
    ImGui_SameLine();
    if (ImGui_SmallButton("Add Dummy Error")) {
      AddLog("[error] something went wrong");
    }
    ImGui_SameLine();
    if (ImGui_SmallButton("Clear")) {
      ClearLog();
    }
    ImGui_SameLine();
    if (ImGui_SmallButton("Scroll to bottom")) {
      ScrollToBottom = true;
    }
    //static float t = 0.0f; if (ImGui_GetTime() - t > 0.02f) { t = ImGui_GetTime(); AddLog("Spam %f", t); }
    ImGui_Separator();
    ImGui_PushStyleVar2(ImGuiStyleVar_FramePadding, fVec2(0, 0));
    static ImGuiTextFilter filter;
    filter.Draw("Filter (\"incl,-excl\") (\"error\")", 180);
    ImGui_PopStyleVar();
    ImGui_Separator();
    ImGui_BeginChild("ScrollingRegion", fVec2(0, -ImGui_GetItemsLineHeightWithSpacing()), false, ImGuiWindowFlags_HorizontalScrollbar);
    if (ImGui_BeginPopupContextWindow()) {
      if (ImGui_Selectable("Clear")) {
        ClearLog();
      }
      ImGui_EndPopup();
    }
    // Display every line as a separate entry so we can change their color or add custom widgets. If you only want raw text you can use ImGui_TextUnformatted(log.begin(), log.end());
    // NB- if you have thousands of entries this approach may be too inefficient and may require user-side clipping to only process visible items.
    // You can seek and display only the lines that are visible using the ImGuiListClipper helper, if your elements are evenly spaced and you have cheap random access to the elements.
    // To use the clipper we could replace the 'for (i = 0; i < Items.Size; i++)' loop with:
    // ImGuiListClipper clipper(Items.Size);
    // while (clipper.Step())
    // for (i = clipper.DisplayStart; i < clipper.DisplayEnd; i++)
    // However take note that you can not use this code as is if a filter is active because it breaks the 'cheap random-access' property. We would need random-access on the post-filtered list.
    // A typical application wanting coarse clipping and filtering may want to pre-compute an array of indices that passed the filtering test, recomputing this array when user changes the filter,
    // and appending newly elements as they are inserted. This is left as a task to the user until we can manage to improve this example code!
    // If your items are of variable size you may want to implement code similar to what ImGuiListClipper does. Or split your data into fixed height items to allow random-seeking into your list.
    ImGui_PushStyleVar2(ImGuiStyleVar_ItemSpacing, fVec2(4, 1)); // Tighten spacing
    for (i = 0; i < Items.Size; i++) {
      const char* item = Items[i];
      if (!filter.PassFilter(item)) {
        continue;
      }
      ImVec4 col = fVec4(1.0f, 1.0f, 1.0f, 1.0f); // A better implementation may store a type per-item. For the sample let's just parse the text.
      if (strstr(item, "[error]")) {
        col = ImColor(1.0f, 0.4f, 0.4f, 1.0f);
      }
      else if (strncmp(item, "# ", 2) == 0) {
        col = ImColor(1.0f, 0.78f, 0.58f, 1.0f);
      }
      ImGui_PushStyleColor(ImGuiCol_Text, col);
      ImGui_TextUnformatted(item, NULL);
      ImGui_PopStyleColor();
    }
    if (ScrollToBottom) {
      ImGui_SetScrollHere();
    }
    ScrollToBottom = false;
    ImGui_PopStyleVar();
    ImGui_EndChild();
    ImGui_Separator();
    // Command-line
    if (ImGui_InputText("Input", InputBuf, IM_ARRAYSIZE(InputBuf), ImGuiInputTextFlags_EnterReturnsTrue | ImGuiInputTextFlags_CallbackCompletion | ImGuiInputTextFlags_CallbackHistory, &TextEditCallbackStub, (void*)this)) {
      char* input_end = InputBuf + strlen(InputBuf);
      while (input_end > InputBuf && input_end[-1] == ' ') {
        input_end--;
      } *input_end = 0;
      if (InputBuf[0]) {
        ExecCommand(InputBuf);
      }
      strcpy(InputBuf, "");
    }
    // Demonstrate keeping auto focus on the input box
    if (ImGui_IsItemHovered() || (ImGui_IsRootWindowOrAnyChildFocused() && !ImGui_IsAnyItemActive() && !ImGui_IsMouseClicked(0))) {
      ImGui_SetKeyboardFocusHere(-1); // Auto focus previous widget
    }
    ImGui_End();
  }
  void ExecCommand(const char* command_line) {
    int i;
    AddLog("# %s\n", command_line);
    // Insert into history. First find match and delete it so it can be pushed to the back. This isn't trying to be smart or optimal.
    HistoryPos = -1;
    for (i = History.Size - 1; i >= 0; i--)
      if (Stricmp(History[i], command_line) == 0) {
        free(History[i]);
        History.erase(History.begin() + i);
        break;
      }
    History.push_back(Strdup(command_line));
    // Process command
    if (Stricmp(command_line, "CLEAR") == 0) {
      ClearLog();
    }
    else if (Stricmp(command_line, "HELP") == 0) {
      AddLog("Commands:");
      for (i = 0; i < Commands.Size; i++) {
        AddLog("- %s", Commands[i]);
      }
    }
    else if (Stricmp(command_line, "HISTORY") == 0) {
      for (i = History.Size >= 10 ? History.Size - 10 : 0; i < History.Size; i++) {
        AddLog("%3d: %s\n", i, History[i]);
      }
    }
    else {
      AddLog("Unknown command: '%s'\n", command_line);
    }
  }
  static int TextEditCallbackStub(ImGuiTextEditCallbackData* data) { // In C++11 you are better off using lambdas for this sort of forwarding callbacks
    ExampleAppConsole* console = (ExampleAppConsole*)data->UserData;
    return console->TextEditCallback(data);
  }
  int TextEditCallback(ImGuiTextEditCallbackData* data) {
    int i;
    //AddLog("cursor: %d, selection: %d-%d", data->CursorPos, data->SelectionStart, data->SelectionEnd);
    switch (data->EventFlag) {
    case ImGuiInputTextFlags_CallbackCompletion: {
      // Example of TEXT COMPLETION
      // Locate beginning of current word
      const char* word_end = data->Buf + data->CursorPos;
      const char* word_start = word_end;
      while (word_start > data->Buf) {
        const char c = word_start[-1];
        if (c == ' ' || c == '\t' || c == ',' || c == ';') {
          break;
        }
        word_start--;
      }
      // Build a list of candidates
      ImVector<const char*> candidates;
      for (i = 0; i < Commands.Size; i++)
        if (Strnicmp(Commands[i], word_start, (int)(word_end - word_start)) == 0) {
          candidates.push_back(Commands[i]);
        }
      if (candidates.Size == 0) {
        // No match
        AddLog("No match for \"%.*s\"!\n", (int)(word_end - word_start), word_start);
      }
      else if (candidates.Size == 1) {
        // Single match. Delete the beginning of the word and replace it entirely so we've got nice casing
        data->DeleteChars((int)(word_start - data->Buf), (int)(word_end - word_start));
        data->InsertChars(data->CursorPos, candidates[0]);
        data->InsertChars(data->CursorPos, " ");
      }
      else {
        // Multiple matches. Complete as much as we can, so inputing "C" will complete to "CL" and display "CLEAR" and "CLASSIFY"
        int match_len = (int)(word_end - word_start);
        for (;;) {
          int c = 0;
          bool all_candidates_matches = true;
          for (i = 0; i < candidates.Size && all_candidates_matches; i++)
            if (i == 0) {
              c = toupper(candidates[i][match_len]);
            }
            else if (c != toupper(candidates[i][match_len])) {
              all_candidates_matches = false;
            }
          if (!all_candidates_matches) {
            break;
          }
          match_len++;
        }
        if (match_len > 0) {
          data->DeleteChars((int)(word_start - data->Buf), (int)(word_end - word_start));
          data->InsertChars(data->CursorPos, candidates[0], candidates[0] + match_len);
        }
        // List matches
        AddLog("Possible matches:\n");
        for (i = 0; i < candidates.Size; i++) {
          AddLog("- %s\n", candidates[i]);
        }
      }
      break;
    }
    case ImGuiInputTextFlags_CallbackHistory: {
      // Example of HISTORY
      const int prev_history_pos = HistoryPos;
      if (data->EventKey == ImGuiKey_UpArrow) {
        if (HistoryPos == -1) {
          HistoryPos = History.Size - 1;
        }
        else if (HistoryPos > 0) {
          HistoryPos--;
        }
      }
      else if (data->EventKey == ImGuiKey_DownArrow) {
        if (HistoryPos != -1)
          if (++HistoryPos >= History.Size) {
            HistoryPos = -1;
          }
      }
      // A better implementation would preserve the data on the current input line along with cursor position.
      if (prev_history_pos != HistoryPos) {
        data->CursorPos = data->SelectionStart = data->SelectionEnd = data->BufTextLen = (int)snprintf(data->Buf, (size_t)data->BufSize, "%s", (HistoryPos >= 0) ? History[HistoryPos] : "");
        data->BufDirty = true;
      }
    }
    }
    return 0;
  }
};
static void ShowExampleAppConsole(bool* p_open)
{
  static ExampleAppConsole console;
  console.Draw("Example: Console", p_open);
}
// Usage:
// static ExampleAppLog my_log;
// my_log.AddLog("Hello %d world\n", 123);
// my_log.Draw("title");
struct ExampleAppLog {
  ImGuiTextBuffer Buf;
  ImGuiTextFilter Filter;
  ImVector<int> LineOffsets; // Index to lines offset
  bool ScrollToBottom;
  void Clear() {
    Buf.clear();
    LineOffsets.clear();
  }
  void AddLog(const char* fmt, ...) IM_PRINTFARGS(2) {
    int old_size = Buf.size();
    va_list args;
    va_start(args, fmt);
    Buf.appendv(fmt, args);
    va_end(args);
    for (int new_size = Buf.size(); old_size < new_size; old_size++)
      if (Buf[old_size] == '\n') {
        LineOffsets.push_back(old_size);
      }
    ScrollToBottom = true;
  }
  void Draw(const char* title, bool* p_open = NULL) {
    ImGui_SetNextWindowSize(fVec2(500, 400), ImGuiSetCond_FirstUseEver);
    ImGui_Begin(title, p_open, 0);
    if (ImGui_Button("Clear")) {
      Clear();
    }
    ImGui_SameLine();
    bool copy = ImGui_Button("Copy");
    ImGui_SameLine();
    Filter.Draw("Filter", -100.0f);
    ImGui_Separator();
    ImGui_BeginChild("scrolling", fVec2(0, 0), false, ImGuiWindowFlags_HorizontalScrollbar);
    if (copy) {
      ImGui_LogToClipboard();
    }
    if (Filter.IsActive()) {
      const char* buf_begin = Buf.begin();
      const char* line = buf_begin;
      for (int line_no = 0; line != NULL; line_no++) {
        const char* line_end = (line_no < LineOffsets.Size) ? buf_begin + LineOffsets[line_no] : NULL;
        if (Filter.PassFilter(line, line_end)) {
          ImGui_TextUnformatted(line, line_end);
        }
        line = line_end && line_end[1] ? line_end + 1 : NULL;
      }
    }
    else {
      ImGui_TextUnformatted(Buf.begin(), NULL);
    }
    if (ScrollToBottom) {
      ImGui_SetScrollHere(1.0f);
    }
    ScrollToBottom = false;
    ImGui_EndChild();
    ImGui_End();
  }
};
static void ShowExampleAppLog(bool* p_open)
{
  static ExampleAppLog log;
  // Demo fill
  static float last_time = -1.0f;
  float time = ImGui_GetTime();
  if (time - last_time >= 0.3f) {
    const char* random_words[] = { "system", "info", "warning", "error", "fatal", "notice", "log" };
    log.AddLog("[%s] Hello, time is %.1f, rand() %d\n", random_words[rand() % IM_ARRAYSIZE(random_words)], time, (int)rand());
    last_time = time;
  }
  log.Draw("Example: Log", p_open);
}
static void ShowExampleAppLayout(bool* p_open)
{
  int i;
  ImGui_SetNextWindowSize(fVec2(500, 440), ImGuiSetCond_FirstUseEver);
  if (ImGui_Begin("Example: Layout", p_open, ImGuiWindowFlags_MenuBar)) {
    if (ImGui_BeginMenuBar()) {
      if (ImGui_BeginMenu("File")) {
        if (ImGui_MenuItem("Close")) {
          *p_open = false;
        }
        ImGui_EndMenu();
      }
      ImGui_EndMenuBar();
    }
    // left
    static int selected = 0;
    ImGui_BeginChild("left pane", fVec2(150, 0), true, 0);
    for (i = 0; i < 100; i++) {
      char label[128];
      sprintf(label, "MyObject %d", i);
      if (ImGui_Selectable(label, selected == i)) {
        selected = i;
      }
    }
    ImGui_EndChild();
    ImGui_SameLine();
    // right
    ImGui_BeginGroup();
    ImGui_BeginChild("item view", fVec2(0, -ImGui_GetItemsLineHeightWithSpacing()), false, 0); // Leave room for 1 line below us
    ImGui_Text("MyObject: %d", selected);
    ImGui_Separator();
    ImGui_TextWrapped("Lorem ipsum dolor sit amet, consectetur adipiscing elit, sed do eiusmod tempor incididunt ut labore et dolore magna aliqua. ");
    ImGui_EndChild();
    ImGui_BeginChild("buttons", fVec2(0, 0), false, 0);
    if (ImGui_Button("Revert")) {}
    ImGui_SameLine();
    if (ImGui_Button("Save")) {}
    ImGui_EndChild();
    ImGui_EndGroup();
  }
  ImGui_End();
}
static void ShowExampleAppPropertyEditor(bool* p_open)
{
  ImGui_SetNextWindowSize(fVec2(430, 450), ImGuiSetCond_FirstUseEver);
  if (!ImGui_Begin("Example: Property editor", p_open, 0)) {
    ImGui_End();
    return;
  }
  ShowHelpMarker("This example shows how you may implement a property editor using two columns.\nAll objects/fields data are dummies here.\nRemember that in many simple cases, you can use ImGui_SameLine(xxx) to position\nyour cursor horizontally instead of using the Columns() API.");
  ImGui_PushStyleVar2(ImGuiStyleVar_FramePadding, fVec2(2, 2));
  ImGui_Columns(2, NULL);
  ImGui_Separator();
  struct funcs {
    static void ShowDummyObject(const char* prefix, int uid) {
      int i;
      ImGui_PushID(uid); // Use object uid as identifier. Most commonly you could also use the object pointer as a base ID.
      ImGui_AlignFirstTextHeightToWidgets(); // Text and Tree nodes are less high than regular widgets, here we add vertical spacing to make the tree lines equal high.
      bool node_open = ImGui_TreeNode("Object", "%s_%u", prefix, uid);
      ImGui_NextColumn();
      ImGui_AlignFirstTextHeightToWidgets();
      ImGui_Text("my sailor is rich");
      ImGui_NextColumn();
      if (node_open) {
        static float dummy_members[8] = { 0.0f, 0.0f, 1.0f, 3.1416f, 100.0f, 999.0f };
        for (i = 0; i < 8; i++) {
          ImGui_PushID(i); // Use field index as identifier.
          if (i < 2) {
            ShowDummyObject("Child", 424242);
          }
          else {
            ImGui_AlignFirstTextHeightToWidgets();
            // Here we use a Selectable (instead of Text) to highlight on hover
            //ImGui_Text("Field_%d", i);
            char label[32];
            sprintf(label, "Field_%d", i);
            ImGui_Bullet();
            ImGui_Selectable(label);
            ImGui_NextColumn();
            ImGui_PushItemWidth(-1);
            if (i >= 5) {
              ImGui_InputFloat("##value", &dummy_members[i], 1.0f);
            }
            else {
              ImGui_DragFloat("##value", &dummy_members[i], 0.01f);
            }
            ImGui_PopItemWidth();
            ImGui_NextColumn();
          }
          ImGui_PopID();
        }
        ImGui_TreePop();
      }
      ImGui_PopID();
    }
  };
  // Iterate dummy objects with dummy members (all the same data)
  for (int obj_i = 0; obj_i < 3; obj_i++) {
    funcs::ShowDummyObject("Object", obj_i);
  }
  ImGui_Columns(1, NULL);
  ImGui_Separator();
  ImGui_PopStyleVar();
  ImGui_End();
}
static void ShowExampleAppLongText(bool* p_open)
{
  int i;
  ImGui_SetNextWindowSize(fVec2(520, 600), ImGuiSetCond_FirstUseEver);
  if (!ImGui_Begin("Example: Long text display", p_open, 0)) {
    ImGui_End();
    return;
  }
  static int test_type = 0;
  static ImGuiTextBuffer log;
  static int lines = 0;
  ImGui_Text("Printing unusually long amount of text.");
  ImGui_Combo("Test type", &test_type, "Single call to TextUnformatted()\0Multiple calls to Text(), clipped manually\0Multiple calls to Text(), not clipped\0");
  ImGui_Text("Buffer contents: %d lines, %d bytes", lines, log.size());
  if (ImGui_Button("Clear")) {
    log.clear();
    lines = 0;
  }
  ImGui_SameLine();
  if (ImGui_Button("Add 1000 lines")) {
    for (i = 0; i < 1000; i++) {
      log.append("%i The quick brown fox jumps over the lazy dog\n", lines + i);
    }
    lines += 1000;
  }
  ImGui_BeginChild("Log", fVec2(0, 0), false, 0);
  switch (test_type) {
  case 0:
    // Single call to TextUnformatted() with a big buffer
    ImGui_TextUnformatted(log.begin(), log.end());
    break;
  case 1: {
    // Multiple calls to Text(), manually coarsely clipped - demonstrate how to use the ImGuiListClipper helper.
    ImGui_PushStyleVar2(ImGuiStyleVar_ItemSpacing, fVec2(0, 0));
    ImGuiListClipper clipper;
    ImGuiListClipper_Begin(&clipper, lines);
    while (ImGuiListClipper_Step(&clipper)) {
      for (i = clipper.DisplayStart; i < clipper.DisplayEnd; i++) {
        ImGui_Text("%i The quick brown fox jumps over the lazy dog", i);
      }
    }
    ImGuiListClipper_End(&clipper);
    ImGui_PopStyleVar();
    break;
  }
  case 2:
    // Multiple calls to Text(), not clipped (slow)
    ImGui_PushStyleVar2(ImGuiStyleVar_ItemSpacing, fVec2(0, 0));
    for (i = 0; i < lines; i++) {
      ImGui_Text("%i The quick brown fox jumps over the lazy dog", i);
    }
    ImGui_PopStyleVar();
    break;
  }
  ImGui_EndChild();
  ImGui_End();
}
// End of Demo code
#else
void ImGui_ShowTestWindow(bool*) {}
void ImGui_ShowUserGuide() {}
void ImGui_ShowStyleEditor(ImGuiStyle*) {}
#endif
#ifdef test_imgui_opengl2
#include "test_imgui_opengl2.inl"
#else
#pragma comment(lib, "d3d9.lib")
#include "test_imgui_directx9.inl"
#endif

