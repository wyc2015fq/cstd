// Copyright (C) 2002-2012 Nikolaus Gebhardt
// This file is part of the "Irrlicht Engine".
// For conditions of distribution and use, see copyright notice in irrlicht.h
#ifndef __I_CURSOR_CONTROL_H_INCLUDED__
#define __I_CURSOR_CONTROL_H_INCLUDED__
struct IGUISpriteBank;
//! Default icons for cursors
ECURSOR_ICON getActiveIcon(ICursorControl* s) {
  return ECI_NORMAL;
}
ECURSOR_ICON addIcon(ICursorControl* s, const SCursorSprite* icon) {
  return ECI_NORMAL;
}
void changeIcon(ICursorControl* s, ECURSOR_ICON iconId, const SCursorSprite& sprite) {}
dimension2di getSupportedIconSize() const {
  return dimension2di(0, 0);
}
void setPlatformBehavior(ICursorControl* s, ECURSOR_PLATFORM_BEHAVIOR behavior) {}
ECURSOR_PLATFORM_BEHAVIOR getPlatformBehavior(ICursorControl* s) {
  return ECPB_NONE;
}

#endif

