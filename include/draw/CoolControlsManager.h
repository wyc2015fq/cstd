
//////////////////////////////////////////////////////////////////////
// CCoolControlsManager struct

enum ButtonStatesEnum {
  bsNormal,
  bsHover,
  bsChecked,
};

enum DrawStatesEnum {
  dsNormal,
  dsHover,
  dsAlternate,
  dsFocus = 0x04,
  dsHoverMask = 0x05,
  dsDisabled = 0x08,
};

enum OrientationsEnum {
  tabTop,
  tabLeft,
  tabRight,
  tabBottom,
};
struct CCoolControlsManager {
  int m_nState;     // Current control state
  int m_nStyle;

#include "CoolControlsManager.inl"
};

