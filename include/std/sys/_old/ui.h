#ifndef _UI_H_
#define _UI_H_
// Styles for the DoPaintFrame() helper
#define UIFRAME_ROUND 0x00000001
#define UIFRAME_FOCUS 0x00000002
// Styles for the DoPaintArcCaption() helper
#define UIARC_GRIPPER 0x00000001
#define UIARC_EXPANDBUTTON 0x00000002
#define UIARC_COLLAPSEBUTTON 0x00000004
// Flags used for controlling the paint
#define UIS_FOCUSED 0x00000001
#define UIS_SELECTED 0x00000002
#define UIS_DISABLED 0x00000004
#define UIS_HOT 0x00000008
#define UIS_PUSHED 0x00000010
#define UIS_CHECKED 0x00000020
#define UIS_READONLY 0x00000040
#define UIS_CAPTURED 0x00000080
#define UIS_VISIBLE 0x00000100
#define UIS_EXPANDED 0x00000200
#define UIS_HASCHILD 0x00000400
#define UIS_3STATE 0x00000800
//#define MAX max
//#define MIN min
#define SWAP(a,b,t) (t=a,a=b,b=t)
//
//
#ifndef BlendRGB
#define BlendRGB(c1, c2, factor) \
  _RGB( GetRValue(c1) + ((GetRValue(c2) - GetRValue(c1)) * factor / 100L), \
      GetGValue(c1) + ((GetGValue(c2) - GetGValue(c1)) * factor / 100L), \
      GetBValue(c1) + ((GetBValue(c2) - GetBValue(c1)) * factor / 100L) )
#endif
#endif // _UI_H_

