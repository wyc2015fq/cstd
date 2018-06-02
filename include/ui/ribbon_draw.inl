// Quick Access and Caption Bar
int DrawCaptionBarBackground(img_t* im, IRECT r)
{
  COLOR colors[] = {
    ColorTable_Caption1, ColorTable_Caption2,
    ColorTable_Caption3, ColorTable_Caption4,
    ColorTable_Caption5, ColorTable_Caption6,
    ColorTable_Caption7, ColorTable_Caption7
  };
  int i;
  int hh[4] = {4, 4, RCH(&r) - 9, 1};
  for (i = 0; i < countof(hh); i++) {
    IRECT grect = iRECT(r.l, r.t, r.r, r.t + hh[i]);
    //grect.b += 2; grect.t -= 1;
    imdraw_rect_gradient(im, &r, grect, colors[i * 2 + 0], colors[i * 2 + 1], 0);
    r.t += hh[i];
    //g.FillRectangle(b, rects[i]);
    //using(LinearGradientBrush b = new LinearGradientBrush(grect, colors[i, 0], colors[i, 1], 90)) {}
  }
  return 0;
}
int DrawRibbonButton(img_t* im, IRECT rc, ribbon_button_t* b)
{
#if 0
  // Button
  if (b->Enabled) {
    if (b->Style == RibbonButtonStyle_Normal) {
      if (b->Pressed && b->SizeMode != RibbonElementSizeMode_DropDown) {
        DrawButtonPressed(e.Graphics, b);
      }
      else if (b.Selected) {
        if (b.Checked) {
          DrawButtonPressed(e.Graphics, b);
        }
        else {
          DrawButtonSelected(e.Graphics, b);
        }
      }
      else if (b.Checked) {
        DrawButtonChecked(e.Graphics, b);
      }
      else if (b is RibbonOrbOptionButton) {
        DrawOrbOptionButton(e.Graphics, b.Bounds);
      }
      else {
        //No background
      }
    }
    else {
      if (b.DropDownPressed && b.SizeMode != RibbonElementSizeMode_DropDown) {
        DrawButtonPressed(e.Graphics, b);
        DrawSplitButtonDropDownSelected(e, b);
      }
      else if (b.Pressed && b.SizeMode != RibbonElementSizeMode_DropDown) {
        DrawButtonPressed(e.Graphics, b);
        DrawSplitButtonSelected(e, b);
      }
      else if (b.DropDownSelected) {
        DrawButtonSelected(e.Graphics, b);
        DrawSplitButtonDropDownSelected(e, b);
      }
      else if (b.Selected) {
        DrawButtonSelected(e.Graphics, b);
        DrawSplitButtonSelected(e, b);
      }
      else if (b.Checked) {
        DrawButtonChecked(e.Graphics, b);
      }
      else {
        DrawSplitButton(e, b);
      }
    }
  }
  if (b.Style != RibbonButtonStyle_Normal && !(b.Style == RibbonButtonStyle_DropDown && b.SizeMode == RibbonElementSizeMode_Large)) {
    if (b.Style == RibbonButtonStyle_DropDown) {
      DrawButtonDropDownArrow(e.Graphics, b, b.OnGetDropDownBounds(b.SizeMode, b.Bounds));
    }
    else {
      DrawButtonDropDownArrow(e.Graphics, b, b.DropDownBounds);
    }
  }
#endif
  return 0;
}
int OnRenderRibbonItem(img_t* im, IRECT rc)
{
#if 0
  if (e.Item is RibbonItemGroup) {
    // Group
    DrawItemGroup(e, e.Item as RibbonItemGroup);
  }
  else if (e.Item is RibbonButtonList) {
    // ButtonList
    DrawButtonList(e.Graphics, e.Item as RibbonButtonList);
  }
  else if (e.Item is RibbonSeparator) {
    // Separator
    DrawSeparator(e.Graphics, e.Item as RibbonSeparator);
  }
  else if (e.Item is RibbonTextBox) {
    // TextBox
    RibbonTextBox t = e.Item as RibbonTextBox;
    if (t.Enabled) {
      if (t != null && (t.Selected || (t.Editing))) {
        DrawTextBoxSelected(e.Graphics, t.TextBoxBounds);
      }
      else {
        DrawTextBoxUnselected(e.Graphics, t.TextBoxBounds);
      }
    }
    else {
      DrawTextBoxDisabled(e.Graphics, t.TextBoxBounds);
    }
    if (t is RibbonComboBox) {
      DrawComboxDropDown(e.Graphics, t as RibbonComboBox);
    }
  }
#endif
  return 0;
}
/// Paints the Ribbon on the specified device
int ribbon_draw(ribbon_t* s, img_t* im, IRECT rc)
{
  IRECT captionBar;
  //Caption Background
  imdraw_fillrect(im, &rc, rc, ColorTable_RibbonBackground);
  //Caption Bar
  captionBar = iRECT(rc.l, rc.t, rc.r, rc.t + CaptionBarHeight);
  DrawCaptionBarBackground(im, captionBar);
  //if (s->hit)
  {
    DrawButtonPressed(im, rc, 3);
  }
  //DrawCaptionBarText(s->CaptionTextBounds, e);
  imdraw_text(im, &rc, captionBar, s->CaptionText->s, s->CaptionText->l, g_scr->font, ColorTable_FormBorder, 0, TT_CENTER | TT_VCENTER);
#if 0
  //Caption Buttons
  if (s->CaptionButtonsVisible) {
    MinimizeButton.OnPaint(this, new RibbonElementPaintEventArgs(clip, im, RibbonElementSizeMode_Medium));
    MaximizeRestoreButton.OnPaint(this, new RibbonElementPaintEventArgs(clip, im, RibbonElementSizeMode_Medium));
    CloseButton.OnPaint(this, new RibbonElementPaintEventArgs(clip, im, RibbonElementSizeMode_Medium));
  }
  //Orb
  Renderer.OnRenderRibbonOrb(new RibbonRenderEventArgs(this, im, clip));
  //QuickAcess toolbar
  QuickAcessToolbar.OnPaint(this, new RibbonElementPaintEventArgs(clip, im, RibbonElementSizeMode_Compact));
  //Render Tabs
  foreach(RibbonTab tab in Tabs) {
    tab.OnPaint(this, new RibbonElementPaintEventArgs(tab.TabBounds, im, RibbonElementSizeMode_None, this));
  }
#endif
  //test_asdf(im, rc);
  return 0;
}

