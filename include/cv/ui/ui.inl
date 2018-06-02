
#define waitKey WaitKey
#define setMouseCallback SetMouseCB
int SetMouseCB(const char* window_name, MouseCB on_mouse, void* param);
#define namedWindow(name) NamedWindow(name, 1)
#define cstr_empty(str)   (!(str) || !*(str))
#define cstr_size(str)   strlen(str)
typedef const img_t* InputArray;

// Compatibility macros to allow compilation with either OpenCV 2.x or OpenCV 3.x
#if (CV_MAJOR_VERSION < 3)
#define CVUI_ANTIALISED CC_AA
#else
#define CVUI_ANTIALISED CC_LINE_AA
#endif
#define CVUI_FILLED -1

// If we are not in a Windows-based environment, replace Windows-specific functions with
// their POSIX equivalents.
#if !defined(_MSC_VER)
#define _vsnprintf vsprintf
#define _snprintf sprintf
#endif

// Define a few platform-dependent macros
#ifdef _MSC_VER
#define _CVUI_COMPILE_MESSAGE(x) message(x)
#elif __GNUC__
#define _CVUI_COMPILE_MESSAGE(x) message x
#endif

// Check for Unix stuff
#ifdef __GNUC__
// just to remove the warning under gcc that is introduced by the cvui_VERSION variable below
// (needed for those who compile with -Werror (make warning as errors)
#pragma GCC diagnostic ignored "-Wunused-variable"
#endif

// Lib version
static const char *cvui_VERSION = "2.5.0-BETA";

enum CVUI_TYPE {
 CVUI_ROW = 0,
 CVUI_COLUMN = 1,
 CVUI_DOWN = 2,
 CVUI_CLICK = 3,
 CVUI_OVER = 4,
 CVUI_OUT = 5,
 CVUI_UP = 6,
 CVUI_IS_DOWN = 7,
};

// Constants regarding mouse buttons
const int cvui_LEFT_BUTTON = 0;
const int cvui_MIDDLE_BUTTON = 1;
const int cvui_RIGHT_BUTTON = 2;

// Constants regarding components
const unsigned int cvui_TRACKBAR_HIDE_SEGMENT_LABELS = 1;
const unsigned int cvui_TRACKBAR_HIDE_STEP_SCALE = 2;
const unsigned int cvui_TRACKBAR_DISCRETE = 4;
const unsigned int cvui_TRACKBAR_HIDE_MIN_MAX_LABELS = 8;
const unsigned int cvui_TRACKBAR_HIDE_VALUE_LABEL = 16;
const unsigned int cvui_TRACKBAR_HIDE_LABELS = 32;

#define MAX_NAME  32

// Describes the block structure used by the lib to handle `begin*()` and `end*()` calls.
typedef struct {
  img_t* where;			// where the block should be rendered to.
  CRect rect;			// the size and position of the block.
  CRect fill;			// the filled area occuppied by the block as it gets modified by its inner components.
  CPoint anchor;		// the point where the next component of the block should be rendered.
  int padding;			// padding among components within this block.
  enum CVUI_TYPE type;				// type of the block, e.g. CVUI_ROW or CVUI_COLUMN.
} cvui_block_t;

// Describes a component label, including info about a shortcut.
// If a label cRectContains "Re&start", then:
// - hasShortcut will be true
// - shortcut will be 's'
// - textBeforeShortcut will be "Re"
// - textAfterShortcut will be "tart"
typedef struct {
  bool hasShortcut;
  char shortcut;
  char textBeforeShortcut[MAX_NAME];
  char textAfterShortcut[MAX_NAME];
} cvui_label_t;

// Describe a mouse button
typedef struct {
  bool justReleased;          // if the mouse button was released, i.e. click event.
  bool justPressed;           // if the mouse button was just pressed, i.e. true for a frame when a button is down.
  bool pressed;               // if the mouse button is pressed or not.
} cvui_mouse_btn_t;

// Describe the information of the mouse cursor
typedef struct {
  cvui_mouse_btn_t buttons[3]; // status of each button. Use cvui_{RIGHT,LEFT,MIDDLE}_BUTTON to access the buttons.
  cvui_mouse_btn_t anyButton;  // represent the behavior of all mouse buttons combined
  CPoint position;          // x and y coordinates of the mouse at the moment.
} cvui_mouse_t;

// Describes a (window) context.
typedef struct {
  char windowName[MAX_NAME];       // name of the window related to this context.
  cvui_mouse_t mouse;          // the mouse cursor related to this context.
} cvui_context_t;


static char gDefaultContext[MAX_NAME];
static char gCurrentContext[MAX_NAME];
static struct ContextMap {
  char key[MAX_NAME];
  cvui_context_t context[1];
} gContexts[1]; // indexed by the window name.
cvui_context_t* findContext(const char* key, int add = 0) {
#if 0
  int i;
  for (i=0; i<100 && gContexts[i].key[0]; ++i) {
    if (0==strcmp(key, gContexts[i].key)) {
      return gContexts[i].context;
    }
  }
  if (add) {
    for (i=0; i<100 && gContexts[i].key[0]; ++i);
    strncpy(gContexts[i].key, key, MAX_NAME);
    return gContexts[i].context;
  }
#endif
  return gContexts[0].context;
}
static char gBuffer[1024];
static int gLastKeyPressed; // TODO: collect it per window
static int gDelayWaitKey;
static cvui_block_t gScreen;

struct TrackbarParams {
		long double min;
    long double max;
    long double step;
    int segments;
    unsigned int options;
    char labelFormat[MAX_NAME];
    
    inline TrackbarParams()
      : min(0.)
      , max(25.)
      , step(1.)
      , segments(0)
      , options(0)
    {strncpy(labelFormat, "%.0Lf", MAX_NAME);}
};

static cvui_block_t gStack[100]; // TODO: make it dynamic?
static int gStackCount = -1;
static const int gTrackbarMarginX = 14;

bool isMouseButton(cvui_mouse_btn_t& theButton, int theQuery) {
		bool aRet = false;
    
    switch (theQuery) {
    case CVUI_CLICK:
    case CVUI_UP:
      aRet = theButton.justReleased; break;
    case CVUI_DOWN:
      aRet = theButton.justPressed; break;
    case CVUI_IS_DOWN:
      aRet = theButton.pressed; break;
    }
    
    return aRet;
}

void resetMouseButton(cvui_mouse_btn_t& theButton) {
		theButton.justPressed = false;
    theButton.justReleased = false;
    theButton.pressed = false;
}

void init(const char* theWindowName, int theDelayWaitKey) {
		strncpy(gDefaultContext, theWindowName, MAX_NAME);
    strncpy(gCurrentContext, theWindowName, MAX_NAME);
    gDelayWaitKey = theDelayWaitKey;
    gLastKeyPressed = -1;
}

void error(int theId, char* theMessage) {
		printf("[CVUI] Fatal error (code %d): %s\n", theId, theMessage);
    waitKey(100000);
    exit(-1);
}

cvui_context_t* getContext(const char* theWindowName = "") {
		if (!cstr_empty(theWindowName)) {
      // Get context in particular
      return findContext(theWindowName);
      
    } else if (!cstr_empty(gCurrentContext)) {
      // No window provided, return currently active context.
      return findContext(gCurrentContext);
      
    } else if (!cstr_empty(gDefaultContext)) {
      // We have no active context, so let's use the default one.
      return findContext(gCurrentContext);
      
    } else {
      // Apparently we have no window at all! <o>
      // This should not happen. Probably cvui_init() was never called.
      //error(5, "Unable to read context. Did you forget to call cvui_init()?");
      return findContext("first"); // return to make the compiler happy.
    }
}

bool bitsetHas(unsigned int theBitset, unsigned int theValue) {
		return (theBitset & theValue) != 0;
}

void block_updateLayoutFlow(cvui_block_t& theBlock, CSize theSize) {
		int aValue;
    
    if (theBlock.type == CVUI_ROW) {
      aValue = theSize.width + theBlock.padding;
      
      theBlock.anchor.x += aValue;
      theBlock.fill.width += aValue;
      theBlock.fill.height = MAX(theSize.height, theBlock.fill.height);
      
    }
    else if (theBlock.type == CVUI_COLUMN) {
      aValue = theSize.height + theBlock.padding;
      
      theBlock.anchor.y += aValue;
      theBlock.fill.height += aValue;
      theBlock.fill.width = MAX(theSize.width, theBlock.fill.width);
    }
}

bool blockStackEmpty() {
		return gStackCount == -1;
}

cvui_block_t& topBlock() {
		if (gStackCount < 0) {
      error(3, "You are using a function that should be enclosed by begin*() and end*(), but you probably forgot to call begin*().");
    }
    
    return gStack[gStackCount];
}

cvui_block_t& pushBlock() {
		return gStack[++gStackCount];
}

cvui_block_t& popBlock() {
		// Check if there is anything to be popped out from the stack.
		if (gStackCount < 0) {
      error(1, "Mismatch in the number of begin*()/end*() calls. You are calling one more than the other.");
    }
    
    return gStack[gStackCount--];
}

void cvui_begin(enum CVUI_TYPE theType, img_t* theWhere, int theX, int theY, int theWidth, int theHeight, int thePadding) {
		cvui_block_t& aBlock = pushBlock();
    
    cvCloneMat(aBlock.where, theWhere);
    
    aBlock.rect.x = theX;
    aBlock.rect.y = theY;
    aBlock.rect.width = theWidth;
    aBlock.rect.height = theHeight;
    
    aBlock.fill = aBlock.rect;
    aBlock.fill.width = 0;
    aBlock.fill.height = 0;
    
    aBlock.anchor.x = theX;
    aBlock.anchor.y = theY;
    
    aBlock.padding = thePadding;
    aBlock.type = theType;
}

void cvui_end(enum CVUI_TYPE theType) {
		cvui_block_t& aBlock = popBlock();
    
    if (aBlock.type != theType) {
      error(4, "Calling wrong type of end*(). E.g. endColumn() instead of endRow(). Check if your begin*() calls are matched with their appropriate end*() calls.");
    }
    
    // If we still have blocks in the stack, we must update
    // the current top with the dimensions that were filled by
    // the newly popped block.
    
    if (!blockStackEmpty()) {
      cvui_block_t& aTop = topBlock();
      CSize aSize;
      
      // If the block has rect.width < 0 or rect.heigth < 0, it means the
      // user don't want to calculate the block's width/height. It's up to
      // us do to the math. In that case, we use the block's fill rect to find
      // out the occupied space. If the block's width/height is greater than
      // zero, then the user is very specific about the desired size. In that
      // case, we use the provided width/height, no matter what the fill rect
      // actually is.
      aSize.width = aBlock.rect.width < 0 ? aBlock.fill.width : aBlock.rect.width;
      aSize.height = aBlock.rect.height < 0 ? aBlock.fill.height : aBlock.rect.height;
      
      block_updateLayoutFlow(aTop, aSize);
    }
}

// Find the min and max values of a vector
void findMinMax(const double* theValues, int aSize, double *theMin, double *theMax) {
		int i;
    double aMin = theValues[0], aMax = theValues[0];
    
    for (i = 0; i < aSize; i++) {
      if (theValues[i] < aMin) {
        aMin = theValues[i];
      }
      
      if (theValues[i] > aMax) {
        aMax = theValues[i];
      }
    }
    
    *theMin = aMin;
    *theMax = aMax;
}

cvui_label_t createLabel(const char* theLabel) {
		cvui_label_t aLabel;
    char* aBefore = aLabel.textBeforeShortcut;
    char* aAfter = aLabel.textAfterShortcut;
    
    aLabel.hasShortcut = false;
    aLabel.shortcut = 0;
    
    for (size_t i = 0; i < cstr_size(theLabel); i++) {
      char c = theLabel[i];
      if ((c == '&') && (i < cstr_size(theLabel) - 1)) {
        aLabel.hasShortcut = true;
        aLabel.shortcut = theLabel[i + 1];
        ++i;
      }
      else if (!aLabel.hasShortcut) {
        *aBefore++ = c;
      }
      else {
        *aAfter++ = c;
      }
    }
    *aBefore = 0;
    *aAfter = 0;
    
    return aLabel;
}

CScalar hexToScalar(unsigned int theColor) {
		int aAlpha = (theColor >> 24) & 0xff;
    int aRed = (theColor >> 16) & 0xff;
    int aGreen = (theColor >> 8) & 0xff;
    int aBlue = theColor & 0xff;
    
    return cScalar(aBlue, aGreen, aRed, aAlpha);
}

void resetRenderingBuffer(cvui_block_t& theScreen) {
		theScreen.rect.x = 0;
    theScreen.rect.y = 0;
    theScreen.rect.width = 0;
    theScreen.rect.height = 0;
    
    theScreen.fill = theScreen.rect;
    theScreen.fill.width = 0;
    theScreen.fill.height = 0;
    
    theScreen.anchor.x = 0;
    theScreen.anchor.y = 0;
    
    theScreen.padding = 0;
}


inline double clamp01(double value)
{
		value = value > 1. ? 1. : value;
    value = value < 0. ? 0. : value;
    return value;
}

inline void trackbarForceValuesAsMultiplesOfSmallStep(const TrackbarParams & theParams, long double *theValue)
{
		if (bitsetHas(theParams.options, cvui_TRACKBAR_DISCRETE) && theParams.step != 0.) {
      long double k = (*theValue - theParams.min) / theParams.step;
      k = (long double)cRound((double)k);
      *theValue = theParams.min + theParams.step * k;
    }
}

inline long double trackbarXPixelToValue(const TrackbarParams & theParams, CRect & theBounding, int thePixelX)
{
		long double ratio = (thePixelX - (long double)(theBounding.x + gTrackbarMarginX)) / (long double)(theBounding.width - 2 * gTrackbarMarginX);
    ratio = clamp01(ratio);
    long double value = theParams.min + ratio * (theParams.max - theParams.min);
    return value;
}

inline int block_trackbarValueToXPixel(const TrackbarParams & theParams, CRect & theBounding, long double theValue)
{
		long double aRatio = (theValue - theParams.min) / (theParams.max - theParams.min);
    aRatio = clamp01(aRatio);
    long double thePixelsX = (long double)theBounding.x + gTrackbarMarginX + aRatio * (long double)(theBounding.width - 2 * gTrackbarMarginX);
    return (int)thePixelsX;
}

int cvui_iarea(int theX, int theY, int theWidth, int theHeight) {
		cvui_mouse_t& aMouse = getContext()->mouse;
    
    // By default, return that the mouse is out of the interaction area.
    int aRet = CVUI_OUT;
    
    // Check if the mouse is over the interaction area.
    bool aMouseIsOver = cRectContains(cRect(theX, theY, theWidth, theHeight), aMouse.position);
    
    if (aMouseIsOver) {
      if (aMouse.anyButton.pressed) {
        aRet = CVUI_DOWN;
      } else {
        aRet = CVUI_OVER;
      }
    }
    
    // Tell if the button was clicked or not
    if (aMouseIsOver && aMouse.anyButton.justReleased) {
      aRet = CVUI_CLICK;
    }
    
    return aRet;
}

void block_image(cvui_block_t& theBlock, CRect theRect, const img_t* theImage) {
  img_t im[1];
		cvCopyMat(cvGetSubRect(theBlock.where, im, theRect), theImage);
}

void image(cvui_block_t& theBlock, int theX, int theY, img_t* theImage) {
		CRect aRect = cRect(theX, theY, theImage->cols, theImage->rows);
    
    // TODO: check for render outside the frame area
    block_image(theBlock, aRect, theImage);
    
    // Update the layout flow according to image size
    CSize aSize = cSize(theImage->cols, theImage->rows);
    block_updateLayoutFlow(theBlock, aSize);
}

bool block_button(cvui_block_t& theBlock, int theX, int theY, img_t* theIdle, img_t* theOver, img_t* theDown, bool theUpdateLayout) {
		CRect aRect = cRect(theX, theY, theIdle->cols, theIdle->rows);
    int aStatus = cvui_iarea(theX, theY, aRect.width, aRect.height);
    
    switch (aStatus) {
    case CVUI_OUT: block_image(theBlock, aRect, theIdle); break;
    case CVUI_OVER: block_image(theBlock, aRect, theOver); break;
    case CVUI_DOWN: block_image(theBlock, aRect, theDown); break;
    }
    
    // Update the layout flow according to button size
    // if we were told to update.
    if (theUpdateLayout) {
      CSize aSize = cSize(aRect.width, aRect.height);
      block_updateLayoutFlow(theBlock, aSize);
    }
    
    // Return true if the button was clicked
    return aStatus == CVUI_CLICK;
}

void block_button(cvui_block_t& theBlock, int theState, CRect& theShape, const char* theLabel) {
		// Outline
		rectangle(theBlock.where, theShape, cScalar(0x29, 0x29, 0x29));
    
    // Border
    theShape.x++; theShape.y++; theShape.width -= 2; theShape.height -= 2;
    rectangle(theBlock.where, theShape, cScalar(0x4A, 0x4A, 0x4A));
    
    // Inside
    theShape.x++; theShape.y++; theShape.width -= 2; theShape.height -= 2;
    rectangle(theBlock.where, theShape, theState == CVUI_OUT ? cScalar(0x42, 0x42, 0x42) : (theState == CVUI_OVER ? cScalar(0x52, 0x52, 0x52) : cScalar(0x32, 0x32, 0x32)), CVUI_FILLED);
}

void block_text(cvui_block_t& theBlock, const char* theText, CPoint& thePos, double theFontScale, unsigned int theColor = 0x00FF00) {
		putText(theBlock.where, theText, -1, thePos, CC_FONT_HERSHEY_SIMPLEX, theFontScale, hexToScalar(theColor), 1, CVUI_ANTIALISED);
}

int block_putText(cvui_block_t& theBlock, int theState, CScalar aColor, const char* theText, CPoint thePosition) {
		double aFontSize = theState == CVUI_DOWN ? 0.39 : 0.4;
    CSize aSize;
    
    if (theText != "") {
      ::putText(theBlock.where, theText, -1, thePosition, CC_FONT_HERSHEY_SIMPLEX, aFontSize, aColor, 1, CVUI_ANTIALISED);
      aSize = getTextSize(theText, -1, CC_FONT_HERSHEY_SIMPLEX, aFontSize, 1, NULL);
    }
    
    return aSize.width;
}

int block_putTextCentered(cvui_block_t& theBlock, const CPoint & position, const char* text) {
		double aFontScale = 0.3;
    
    CSize size = getTextSize(text, -1, CC_FONT_HERSHEY_SIMPLEX, aFontScale, 1, NULL);
    CPoint positionDecentered = cPoint(position.x - size.width / 2, position.y);
    ::putText(theBlock.where, text, positionDecentered, CC_FONT_HERSHEY_SIMPLEX, aFontScale, cScalar(0xCE, 0xCE, 0xCE), 1, CVUI_ANTIALISED);
    
    return size.width;
};

void block_buttonLabel(cvui_block_t& theBlock, int theState, CRect theRect, const char* theLabel, CSize& theTextSize) {
		CPoint aPos = cPoint(theRect.x + theRect.width / 2 - theTextSize.width / 2, theRect.y + theRect.height / 2 + theTextSize.height / 2);
    CScalar aColor = cScalar(0xCE, 0xCE, 0xCE);
    
    cvui_label_t aLabel = createLabel(theLabel);
    
    if (!aLabel.hasShortcut) {
      block_putText(theBlock, theState, aColor, theLabel, aPos);
    }
    else {
      int aWidth = block_putText(theBlock, theState, aColor, aLabel.textBeforeShortcut, aPos);
      int aStart = aPos.x + aWidth;
      aPos.x += aWidth;
      
      char aShortcut[4] = {0};
      *aShortcut = (aLabel.shortcut);
      
      aWidth = block_putText(theBlock, theState, aColor, aShortcut, aPos);
      int aEnd = aStart + aWidth;
      aPos.x += aWidth;
      
      block_putText(theBlock, theState, aColor, aLabel.textAfterShortcut, aPos);
      line(theBlock.where, cPoint(aStart, aPos.y + 3), cPoint(aEnd, aPos.y + 3), aColor, 1, CVUI_ANTIALISED);
    }
}

void block_counter(cvui_block_t& theBlock, CRect& theShape, const char* theValue) {
		rectangle(theBlock.where, theShape, cScalar(0x29, 0x29, 0x29), CVUI_FILLED); // fill
    rectangle(theBlock.where, theShape, cScalar(0x45, 0x45, 0x45)); // border
    
    CSize aTextSize = getTextSize(theValue, CC_FONT_HERSHEY_SIMPLEX, 0.4, 1, NULL);
    
    CPoint aPos = cPoint(theShape.x + theShape.width / 2 - aTextSize.width / 2, theShape.y + aTextSize.height / 2 + theShape.height / 2);
    ::putText(theBlock.where, theValue, aPos, CC_FONT_HERSHEY_SIMPLEX, 0.4, cScalar(0xCE, 0xCE, 0xCE), 1, CVUI_ANTIALISED);
}

void block_rect(cvui_block_t& theBlock, CRect& thePos, unsigned int theBorderColor, unsigned int theFillingColor = 0xff000000) {
		CScalar aBorder = hexToScalar(theBorderColor);
    CScalar aFilling = hexToScalar(theFillingColor);
    
    bool aHasFilling = aFilling[3] != 0xff;
    
    if (aHasFilling) {
      rectangle(theBlock.where, thePos, aFilling, CVUI_FILLED, CVUI_ANTIALISED);
    }
    
    // Render the border
    rectangle(theBlock.where, thePos, aBorder, 1, CVUI_ANTIALISED);
}

bool block_button(cvui_block_t& theBlock, int theX, int theY, int theWidth, int theHeight, const char* theLabel, bool theUpdateLayout) {
		// Calculate the space that the label will fill
		CSize aTextSize = getTextSize(theLabel, CC_FONT_HERSHEY_SIMPLEX, 0.4, 1, NULL);
    
    // Make the button bit enough to house the label
    CRect aRect = cRect(theX, theY, theWidth, theHeight);
    
    // Render the button according to mouse interaction, e.g. CVUI_OVER, CVUI_DOWN, CVUI_OUT.
    int aStatus = cvui_iarea(theX, theY, aRect.width, aRect.height);
    block_button(theBlock, aStatus, aRect, theLabel);
    block_buttonLabel(theBlock, aStatus, aRect, theLabel, aTextSize);
    
    // Update the layout flow according to button size
    // if we were told to update.
    if (theUpdateLayout) {
      CSize aSize = cSize(theWidth, theHeight);
      block_updateLayoutFlow(theBlock, aSize);
    }
    
    bool aWasShortcutPressed = false;
    
    //Handle keyboard shortcuts
    if (gLastKeyPressed != -1) {
      // TODO: replace with something like strpos(). I think it has better performance.
      cvui_label_t aLabel = createLabel(theLabel);
      if (aLabel.hasShortcut && (tolower(aLabel.shortcut) == tolower((char)gLastKeyPressed))) {
        aWasShortcutPressed = true;
      }
    }
    
    // Return true if the button was clicked
    return aStatus == CVUI_CLICK || aWasShortcutPressed;
}

bool block_button(cvui_block_t& theBlock, int theX, int theY, const char* theLabel) {
		// Calculate the space that the label will fill
		CSize aTextSize = getTextSize(theLabel, CC_FONT_HERSHEY_SIMPLEX, 0.4, 1, NULL);
    
    // Create a button based on the size of the text
    return block_button(theBlock, theX, theY, aTextSize.width + 30, aTextSize.height + 18, theLabel, true);
}

void block_checkbox(cvui_block_t& theBlock, int theState, CRect& theShape) {
		// Outline
		rectangle(theBlock.where, theShape, theState == CVUI_OUT ? cScalar(0x63, 0x63, 0x63) : cScalar(0x80, 0x80, 0x80));
    
    // Border
    theShape.x++; theShape.y++; theShape.width -= 2; theShape.height -= 2;
    rectangle(theBlock.where, theShape, cScalar(0x17, 0x17, 0x17));
    
    // Inside
    theShape.x++; theShape.y++; theShape.width -= 2; theShape.height -= 2;
    rectangle(theBlock.where, theShape, cScalar(0x29, 0x29, 0x29), CVUI_FILLED);
}

void block_checkboxLabel(cvui_block_t& theBlock, CRect& theRect, const char* theLabel, CSize& theTextSize, unsigned int theColor = 0x00FF00) {
		CPoint aPos = cPoint(theRect.x + theRect.width + 6, theRect.y + theTextSize.height + theRect.height / 2 - theTextSize.height / 2 - 1);
    block_text(theBlock, theLabel, aPos, 0.4, theColor);
}

void block_checkboxCheck(cvui_block_t& theBlock, CRect& theShape) {
		theShape.x++; theShape.y++; theShape.width -= 2; theShape.height -= 2;
    rectangle(theBlock.where, theShape, cScalar(0xFF, 0xBF, 0x75), CVUI_FILLED);
}

bool block_checkbox(cvui_block_t& theBlock, int theX, int theY, const char* theLabel, bool *theState, unsigned int theColor = 0x00FF00) {
		cvui_mouse_t& aMouse = getContext()->mouse;
    CRect aRect = cRect(theX, theY, 15, 15);
    CSize aTextSize = getTextSize(theLabel, CC_FONT_HERSHEY_SIMPLEX, 0.4, 1, NULL);
    CRect aHitArea = cRect(theX, theY, aRect.width + aTextSize.width + 6, aRect.height);
    bool aMouseIsOver = cRectContains(aHitArea, aMouse.position);
    
    if (aMouseIsOver) {
      block_checkbox(theBlock, CVUI_OVER, aRect);
      
      if (aMouse.anyButton.justReleased) {
        *theState = !(*theState);
      }
    } else {
      block_checkbox(theBlock, CVUI_OUT, aRect);
    }
    
    block_checkboxLabel(theBlock, aRect, theLabel, aTextSize, theColor);
    
    if (*theState) {
      block_checkboxCheck(theBlock, aRect);
    }
    
    // Update the layout flow
    CSize aSize = cSize(aHitArea.width, aHitArea.height);
    block_updateLayoutFlow(theBlock, aSize);
    
    return *theState;
}

void block_text(cvui_block_t& theBlock, int theX, int theY, const char* theText, double theFontScale, unsigned int theColor, bool theUpdateLayout) {
		CSize aTextSize = getTextSize(theText, CC_FONT_HERSHEY_SIMPLEX, theFontScale, 1, NULL);
    CPoint aPos = cPoint(theX, theY + aTextSize.height);
    
    block_text(theBlock, theText, aPos, theFontScale, theColor);
    
    if (theUpdateLayout) {
      // Add an extra pixel to the height to overcome OpenCV font size problems.
      aTextSize.height += 1;
      
      block_updateLayoutFlow(theBlock, aTextSize);
    }
}

int block_counter(cvui_block_t& theBlock, int theX, int theY, int *theValue, int theStep = 1, const char *theFormat = "%d") {
		CRect aContentArea = cRect (theX + 22, theY, 48, 22);
    
    if (block_button(theBlock, theX, theY, 22, 22, "-", false)) {
      *theValue -= theStep;
    }
    
    _snprintf(gBuffer, countof(gBuffer), theFormat, *theValue);
    block_counter(theBlock, aContentArea, gBuffer);
    
    if (block_button(theBlock, aContentArea.x + aContentArea.width, theY, 22, 22, "+", false)) {
      *theValue += theStep;
    }
    
    // Update the layout flow
    CSize aSize = cSize(22 * 2 + aContentArea.width, aContentArea.height);
    block_updateLayoutFlow(theBlock, aSize);
    
    return *theValue;
}

double block_counter(cvui_block_t& theBlock, int theX, int theY, double *theValue, double theStep = 0.5, const char *theFormat = "%.2f") {
		CRect aContentArea = cRect(theX + 22, theY, 48, 22);
    
    if (block_button(theBlock, theX, theY, 22, 22, "-", false)) {
      *theValue -= theStep;
    }
    
    _snprintf(gBuffer, countof(gBuffer), theFormat, *theValue);
    block_counter(theBlock, aContentArea, gBuffer);
    
    if (block_button(theBlock, aContentArea.x + aContentArea.width, theY, 22, 22, "+", false)) {
      *theValue += theStep;
    }
    
    // Update the layout flow
    CSize aSize = cSize(22 * 2 + aContentArea.width, aContentArea.height);
    block_updateLayoutFlow(theBlock, aSize);
    
    return *theValue;
}

void block_trackbarPath(cvui_block_t& theBlock, int theState, CRect& theShape, double theValue, const TrackbarParams &theParams, CRect& theWorkingArea) {
		int aBarHeight = 7;
    CPoint aBarTopLeft = cPoint(theWorkingArea.x, theWorkingArea.y + theWorkingArea.height / 2);
    CRect aRect = cRectPS(aBarTopLeft, cSize(theWorkingArea.width, aBarHeight));
    
    int aBorderColor = theState == CVUI_OVER ? 0x4e4e4e : 0x3e3e3e;
    
    block_rect(theBlock, aRect, aBorderColor, 0x292929);
    ::line(theBlock.where, cPoint(aRect.x + 1, aRect.y + aBarHeight - 2), cPoint(aRect.x + aRect.width - 2, aRect.y + aBarHeight - 2), cScalar(0x0e, 0x0e, 0x0e));
}

void block_trackbarSteps(cvui_block_t& theBlock, int theState, CRect& theShape, double theValue, const TrackbarParams &theParams, CRect& theWorkingArea) {
		CPoint aBarTopLeft = cPoint(theWorkingArea.x, theWorkingArea.y + theWorkingArea.height / 2);
    CScalar aColor = cScalar(0x51, 0x51, 0x51);
    
    bool aDiscrete = bitsetHas(theParams.options, cvui_TRACKBAR_DISCRETE);
    long double aFixedStep = aDiscrete ? theParams.step : (theParams.max - theParams.min) / 20;
    
    // TODO: check min, max and step to prevent infinite loop.
    for (long double aValue = theParams.min; aValue <= theParams.max; aValue += aFixedStep) {
      int aPixelX = block_trackbarValueToXPixel(theParams, theShape, aValue);
      CPoint aPoint1 = cPoint(aPixelX, aBarTopLeft.y);
      CPoint aPoint2 = cPoint(aPixelX, aBarTopLeft.y - 3);
      line(theBlock.where, aPoint1, aPoint2, aColor);
    }
}

void block_trackbarSegmentLabel(cvui_block_t& theBlock, CRect& theShape, const TrackbarParams &theParams, long double theValue, CRect& theWorkingArea, bool theShowLabel) {
		CScalar aColor = cScalar(0x51, 0x51, 0x51);
    CPoint aBarTopLeft = cPoint(theWorkingArea.x, theWorkingArea.y + theWorkingArea.height / 2);
    
    int aPixelX = block_trackbarValueToXPixel(theParams, theShape, theValue);
    
    CPoint aPoint1 = cPoint(aPixelX, aBarTopLeft.y);
    CPoint aPoint2 = cPoint(aPixelX, aBarTopLeft.y - 8);
    line(theBlock.where, aPoint1, aPoint2, aColor);
    
    if (theShowLabel)
    {
      _snprintf(gBuffer, countof(gBuffer), theParams.labelFormat, theValue);
      CPoint aTextPos = cPoint(aPixelX, aBarTopLeft.y - 11);
      block_putTextCentered(theBlock, aTextPos, gBuffer);
    }
}

void block_trackbarSegments(cvui_block_t& theBlock, int theState, CRect& theShape, double theValue, const TrackbarParams &theParams, CRect& theWorkingArea) {
		int aSegments = theParams.segments < 1 ? 1 : theParams.segments;
    long double aSegmentLength = (long double)(theParams.max - theParams.min) / (long double)aSegments;
    
    bool aHasMinMaxLabels = bitsetHas(theParams.options, cvui_TRACKBAR_HIDE_MIN_MAX_LABELS) == false;
    
    // Render the min value label
    block_trackbarSegmentLabel(theBlock, theShape, theParams, theParams.min, theWorkingArea, aHasMinMaxLabels);
    
    //Draw large steps and labels
    bool aHasSegmentLabels = bitsetHas(theParams.options, cvui_TRACKBAR_HIDE_SEGMENT_LABELS) == false;
    // TODO: check min, max and step to prevent infinite loop.
    for (long double aValue = theParams.min; aValue <= theParams.max; aValue += aSegmentLength) {
      block_trackbarSegmentLabel(theBlock, theShape, theParams, aValue, theWorkingArea, aHasSegmentLabels);
    }
    
    // Render the max value label
    block_trackbarSegmentLabel(theBlock, theShape, theParams, theParams.max, theWorkingArea, aHasMinMaxLabels);
}

void block_trackbarHandle(cvui_block_t& theBlock, int theState, CRect& theShape, double theValue, const TrackbarParams &theParams, CRect& theWorkingArea) {
		CPoint aBarTopLeft = cPoint(theWorkingArea.x, theWorkingArea.y + theWorkingArea.height / 2);
    int aBarHeight = 7;
    
    // Draw the rectangle representing the handle
    int aPixelX = block_trackbarValueToXPixel(theParams, theShape, theValue);
    int aIndicatorWidth = 3;
    int aIndicatorHeight = 4;
    CPoint aPoint1 = cPoint(aPixelX - aIndicatorWidth, aBarTopLeft.y - aIndicatorHeight);
    CPoint aPoint2 = cPoint(aPixelX + aIndicatorWidth, aBarTopLeft.y + aBarHeight + aIndicatorHeight);
    CRect aRect = cRectPP(aPoint1, aPoint2);
    
    int aFillColor = theState == CVUI_OVER ? 0x525252 : 0x424242;
    
    block_rect(theBlock, aRect, 0x212121, 0x212121);
    aRect.x += 1; aRect.y += 1; aRect.width -= 2; aRect.height -= 2;
    block_rect(theBlock, aRect, 0x515151, aFillColor);
    
    bool aShowLabel = bitsetHas(theParams.options, cvui_TRACKBAR_HIDE_VALUE_LABEL) == false;
    
    // Draw the handle label
    if (aShowLabel) {
      CPoint aTextPos = cPoint(aPixelX, aPoint2.y + 11);
      _snprintf(gBuffer, countof(gBuffer), theParams.labelFormat, static_cast<long double>(theValue));
      block_putTextCentered(theBlock, aTextPos, gBuffer);
    }
}

void block_trackbar(cvui_block_t& theBlock, int theState, CRect& theShape, double theValue, const TrackbarParams &theParams) {
		CRect aWorkingArea = cRect(theShape.x + gTrackbarMarginX, theShape.y, theShape.width - 2 * gTrackbarMarginX, theShape.height);
    
    block_trackbarPath(theBlock, theState, theShape, theValue, theParams, aWorkingArea);
    
    bool aHideAllLabels = bitsetHas(theParams.options, cvui_TRACKBAR_HIDE_LABELS);
    bool aShowSteps = bitsetHas(theParams.options, cvui_TRACKBAR_HIDE_STEP_SCALE) == false;
    
    if (aShowSteps && !aHideAllLabels) {
      block_trackbarSteps(theBlock, theState, theShape, theValue, theParams, aWorkingArea);
    }
    
    if (!aHideAllLabels) {
      block_trackbarSegments(theBlock, theState, theShape, theValue, theParams, aWorkingArea);
    }
    
    block_trackbarHandle(theBlock, theState, theShape, theValue, theParams, aWorkingArea);
}

bool trackbar(cvui_block_t& theBlock, int theX, int theY, int theWidth, long double *theValue, const TrackbarParams & theParams) {
		cvui_mouse_t& aMouse = getContext()->mouse;
    CRect aContentArea = cRect(theX, theY, theWidth, 45);
    long double aValue = *theValue;
    bool aMouseIsOver = cRectContains(aContentArea, aMouse.position);
    
    block_trackbar(theBlock, aMouseIsOver ? CVUI_OVER : CVUI_OUT, aContentArea, *theValue, theParams);
    
    if (aMouse.anyButton.pressed && aMouseIsOver) {
      *theValue = trackbarXPixelToValue(theParams, aContentArea, aMouse.position.x);
      
      if (bitsetHas(theParams.options, cvui_TRACKBAR_DISCRETE)) {
        trackbarForceValuesAsMultiplesOfSmallStep(theParams, theValue);
      }
    }
    
    // Update the layout flow
    CSize aSize = cRectSize(aContentArea);
    block_updateLayoutFlow(theBlock, aSize);
    
    return (*theValue != aValue);
}


void addWeighted_u8(int count, const uchar* src1, double alpha, const uchar* src2, double beta, double gamma, uchar* dst, int dtype = -1) {
  int i;
  for (i=0; i<count; ++i) {
    dst[i] = (uchar)(src1[i] * alpha + src2[i] * beta + gamma);
  }
  return ;
}

void addWeighted(const img_t* src1, double alpha, const img_t* src2,
                 double beta, double gamma, img_t* dst, int dtype = -1) {
  int count = cvGetSize0D(src1);
  ASSERT(count == cvGetSize0D(src2));
  ASSERT(count == cvGetSize0D(dst));
  addWeighted_u8(count, src1->data, alpha, src2->data, beta, gamma, dst->data, dtype);
}

void block_window(cvui_block_t& theBlock, CRect& theTitleBar, CRect& theContent, const char* theTitle) {
		bool aTransparecy = false;
    double aAlpha = 0.3;
    img_t aOverlay[1] = {0};
    
    // Render the title bar.
    // First the border
    rectangle(theBlock.where, theTitleBar, cScalar(0x4A, 0x4A, 0x4A));
    // then the inside
    theTitleBar.x++; theTitleBar.y++; theTitleBar.width -= 2; theTitleBar.height -= 2;
    rectangle(theBlock.where, theTitleBar, cScalar(0x21, 0x21, 0x21), CVUI_FILLED);
    
    // Render title text.
    CPoint aPos = cPoint(theTitleBar.x + 5, theTitleBar.y + 12);
    ::putText(theBlock.where, theTitle, aPos, CC_FONT_HERSHEY_SIMPLEX, 0.4, cScalar(0xCE, 0xCE, 0xCE), 1, CVUI_ANTIALISED);
    
    // Render the body.
    // First the border.
    ::rectangle(theBlock.where, theContent, cScalar(0x4A, 0x4A, 0x4A));
    
    // Then the filling.
    theContent.x++; theContent.y++; theContent.width -= 2; theContent.height -= 2;
    rectangle(aOverlay, theContent, cScalar(0x31, 0x31, 0x31), CVUI_FILLED);
    
    if (aTransparecy) {
      cvCloneMat(aOverlay, theBlock.where);
      rectangle(aOverlay, theContent, cScalar(0x31, 0x31, 0x31), CVUI_FILLED);
      addWeighted(aOverlay, aAlpha, theBlock.where, 1.0 - aAlpha, 0.0, theBlock.where);
    }
    else {
      rectangle(theBlock.where, theContent, cScalar(0x31, 0x31, 0x31), CVUI_FILLED);
    }
    imfree(aOverlay);
}

void block_window(cvui_block_t& theBlock, int theX, int theY, int theWidth, int theHeight, const char* theTitle) {
		CRect aTitleBar = cRect(theX, theY, theWidth, 20);
    CRect aContent = cRect(theX, theY + aTitleBar.height, theWidth, theHeight - aTitleBar.height);
    
    block_window(theBlock, aTitleBar, aContent, theTitle);
    
    // Update the layout flow
    CSize aSize = cSize(theWidth, theHeight);
    block_updateLayoutFlow(theBlock, aSize);
}

void block_rect(cvui_block_t& theBlock, int theX, int theY, int theWidth, int theHeight, unsigned int theBorderColor, unsigned int theFillingColor = 0xff000000) {
		CPoint aAnchor = cPoint(theX, theY);
    CRect aRect = cRect(theX, theY, theWidth, theHeight);
    
    aRect.x = aRect.width < 0 ? aAnchor.x + aRect.width : aAnchor.x;
    aRect.y = aRect.height < 0 ? aAnchor.y + aRect.height : aAnchor.y;
    aRect.width = ABS(aRect.width);
    aRect.height = ABS(aRect.height);
    
    block_rect(theBlock, aRect, theBorderColor, theFillingColor);
    
    // Update the layout flow
    CSize aSize = cSize(aRect.width, aRect.height);
    block_updateLayoutFlow(theBlock, aSize);
}

void block_sparkline(cvui_block_t& theBlock, const double* theValues, int theValues_size, CRect &theRect, double theMin, double theMax, unsigned int theColor) {
		int aSize = theValues_size, i;
    double aGap, aPosX, aScale = 0, x, y;
    
    aScale = theMax - theMin;
    aGap = (double)theRect.width / aSize;
    aPosX = theRect.x;
    
    for (i = 0; i <= aSize - 2; i++) {
      x = aPosX;
      y = (theValues[i] - theMin) / aScale * -(theRect.height - 5) + theRect.y + theRect.height - 5;
      CPoint aPoint1 = cPoint((int)x, (int)y);
      
      x = aPosX + aGap;
      y = (theValues[i + 1] - theMin) / aScale * -(theRect.height - 5) + theRect.y + theRect.height - 5;
      CPoint aPoint2 = cPoint((int)x, (int)y);
      
      line(theBlock.where, aPoint1, aPoint2, hexToScalar(theColor));
      aPosX += aGap;
    }
}

void block_sparkline(cvui_block_t& theBlock, const double* theValues, int theValues_size, int theX, int theY, int theWidth, int theHeight, unsigned int theColor) {
		double aMin, aMax;
    CRect aRect = cRect(theX, theY, theWidth, theHeight);
    int aHowManyValues = theValues_size;
    
    if (aHowManyValues >= 2) {
      findMinMax(theValues, theValues_size, &aMin, &aMax);
      block_sparkline(theBlock, theValues, theValues_size, aRect, aMin, aMax, theColor);
    } else {
      block_text(theBlock, theX, theY, aHowManyValues == 0 ? "No data." : "Insufficient data points.", 0.4, 0xCECECE, false);
    }
    
    // Update the layout flow
    CSize aSize = cSize(theWidth, theHeight);
    block_updateLayoutFlow(theBlock, aSize);
}


TrackbarParams makeTrackbarParams(double theMin, double theMax, double theStep, int theSegments, const char *theLabelFormat, unsigned int theOptions) {
		TrackbarParams aParams;
    
    aParams.min = (long double)theMin;
    aParams.max = (long double)theMax;
    aParams.step = (long double)theStep;
    aParams.options = theOptions;
    aParams.segments = theSegments;
    strncpy(aParams.labelFormat, theLabelFormat, MAX_NAME);
    
    return aParams;
}

template <typename num_type>
bool cvui_trackbar(int theWidth, num_type *theValue, const TrackbarParams& theParams) {
		cvui_block_t& aBlock = topBlock();
    
    long double aValueAsDouble = static_cast<long double>(*theValue);
    bool aResult = trackbar(aBlock, aBlock.anchor.x, aBlock.anchor.y, theWidth, &aValueAsDouble, theParams);
    *theValue = static_cast<num_type>(aValueAsDouble);
    
    return aResult;
}

template <typename num_type>
bool cvui_trackbar(img_t* theWhere, int theX, int theY, int theWidth, num_type *theValue, const TrackbarParams& theParams) {
		gScreen.where = theWhere;
    
    long double aValueAsDouble = static_cast<long double>(*theValue);
    bool aResult = trackbar(gScreen, theX, theY, theWidth, &aValueAsDouble, theParams);
    *theValue = static_cast<num_type>(aValueAsDouble);
    
    return aResult;
}


template <typename num_type>
bool cvui_trackbar(img_t* theWhere, int theX, int theY, int theWidth, num_type *theValue, num_type theMin, num_type theMax, int theSegments, const char *theLabelFormat, unsigned int theOptions, num_type theDiscreteStep) {
  TrackbarParams aParams = makeTrackbarParams(theMin, theMax, theDiscreteStep, theSegments, theLabelFormat, theOptions);
  return cvui_trackbar<num_type>(theWhere, theX, theY, theWidth, theValue, aParams);
}

template <typename num_type>
bool cvui_trackbar(int theWidth, num_type *theValue, num_type theMin, num_type theMax, int theSegments, const char *theLabelFormat, unsigned int theOptions, num_type theDiscreteStep) {
  TrackbarParams aParams = makeTrackbarParams(theMin, theMax, theDiscreteStep, theSegments, theLabelFormat, theOptions);
  return cvui_trackbar<num_type>(theWidth, theValue, aParams);
}


int _cvui_handleMouse(int theEvent, int theX, int theY, int theFlags, void* theData) {
  int aButtons[3] = { cvui_LEFT_BUTTON, cvui_MIDDLE_BUTTON, cvui_RIGHT_BUTTON };
  WindowMsg aEventsDown[3] = { MSG_LBUTTONDOWN, MSG_MBUTTONDOWN, MSG_RBUTTONDOWN };
  WindowMsg aEventsUp[3] = { MSG_LBUTTONUP, MSG_MBUTTONUP, MSG_RBUTTONUP };
  
  cvui_context_t *aContext = (cvui_context_t *)theData;
  
  for (int i = 0; i < 3; i++) {
    int aBtn = aButtons[i];
    
    if (theEvent == aEventsDown[i]) {
      aContext->mouse.anyButton.justPressed = true;
      aContext->mouse.anyButton.pressed = true;
      aContext->mouse.buttons[aBtn].justPressed = true;
      aContext->mouse.buttons[aBtn].pressed = true;
      
    } else if (theEvent == aEventsUp[i]) {
      aContext->mouse.anyButton.justReleased = true;
      aContext->mouse.anyButton.pressed = false;
      aContext->mouse.buttons[aBtn].justReleased = true;
      aContext->mouse.buttons[aBtn].pressed = false;
    }
  }
  
  aContext->mouse.position.x = theX;
  aContext->mouse.position.y = theY;
  return 0;
}

void cvui_watch(const char* theWindowName, bool theCreateNamedWindow = true) {
  cvui_context_t* aContex = NULL;
  
  if (theCreateNamedWindow) {
    namedWindow(theWindowName);
  }
  
  aContex = findContext(theWindowName, 1);
  strncpy(aContex->windowName, theWindowName, MAX_NAME);
  aContex->mouse.position.x = 0;
  aContex->mouse.position.y = 0;
  
  resetMouseButton(aContex->mouse.anyButton);
  resetMouseButton(aContex->mouse.buttons[cvui_RIGHT_BUTTON]);
  resetMouseButton(aContex->mouse.buttons[cvui_MIDDLE_BUTTON]);
  resetMouseButton(aContex->mouse.buttons[cvui_LEFT_BUTTON]);
  
  setMouseCallback(theWindowName, _cvui_handleMouse, aContex);
}

void cvui_init(const char* theWindowName, int theDelayWaitKey = -1, bool theCreateNamedWindow = true) {
  init(theWindowName, theDelayWaitKey);
  cvui_watch(theWindowName, theCreateNamedWindow);
}

void cvui_init(const char* theWindowNames[], size_t theHowManyWindows, int theDelayWaitKey = -1, bool theCreateNamedWindows = true) {
  init(theWindowNames[0], theDelayWaitKey);
  
  for (size_t i = 0; i < theHowManyWindows; i++) {
    cvui_watch(theWindowNames[i], theCreateNamedWindows);
  }
}

void cvui_context(const char* theWindowName) {
  strncpy(gCurrentContext, theWindowName, MAX_NAME);
}

void cvui_update(const char* theWindowName = "") {
  cvui_context_t* aContext = getContext(theWindowName);
  
  aContext->mouse.anyButton.justReleased = false;
  aContext->mouse.anyButton.justPressed = false;
  
  for (int i = cvui_LEFT_BUTTON; i <= cvui_RIGHT_BUTTON; i++) {
    aContext->mouse.buttons[i].justReleased = false;
    aContext->mouse.buttons[i].justPressed = false;
  }
  
  resetRenderingBuffer(gScreen);
  
  // If we were told to keep track of the keyboard shortcuts, we
  // proceed to handle opencv event queue.
  if (gDelayWaitKey > 0) {
    gLastKeyPressed = waitKey(gDelayWaitKey);
  }
  
  if (!blockStackEmpty()) {
    error(2, "Calling update() before finishing all begin*()/end*() calls. Did you forget to call a begin*() or an end*()? Check if every begin*() has an appropriate end*() call before you call update().");
  }
}

void cvui_imshow(const char* theWindowName, InputArray theFrame) {
  cvui_update(theWindowName);
  imshow2(theWindowName, theFrame);
}

int lastKeyPressed() {
  return gLastKeyPressed;
}

bool cvui_mouse(const char* theWindowName, int theQuery) {
  cvui_mouse_btn_t& aButton = getContext(theWindowName)->mouse.anyButton;
  bool aRet = isMouseButton(aButton, theQuery);
  
  return aRet;
}

CPoint cvui_mouse(const char* theWindowName = "") {
  return getContext(theWindowName)->mouse.position;
}

bool cvui_mouse(int theQuery) {
  return cvui_mouse("", theQuery);
}

bool cvui_mouse(const char* theWindowName, int theButton, int theQuery) {
  if (theButton != cvui_RIGHT_BUTTON && theButton != cvui_MIDDLE_BUTTON && theButton != cvui_LEFT_BUTTON) {
    error(6, "Invalid mouse button. Are you using one of the available: cvui_{RIGHT,MIDDLE,LEFT}_BUTTON ?");
  }
  
  cvui_mouse_btn_t& aButton = getContext(theWindowName)->mouse.buttons[theButton];
  bool aRet = isMouseButton(aButton, theQuery);
  
  return aRet;
}

bool cvui_mouse(int theButton, int theQuery) {
  return cvui_mouse("", theButton, theQuery);
}

bool cvui_button(img_t* theWhere, int theX, int theY, const char* theLabel) {
  gScreen.where = theWhere;
  return block_button(gScreen, theX, theY, theLabel);
}

bool button(img_t* theWhere, int theX, int theY, int theWidth, int theHeight, const char* theLabel) {
  gScreen.where = theWhere;
  return block_button(gScreen, theX, theY, theWidth, theHeight, theLabel, true);
}

bool cvui_button(img_t* theWhere, int theX, int theY, img_t* theIdle, img_t* theOver, img_t* theDown) {
  gScreen.where = theWhere;
  return block_button(gScreen, theX, theY, theIdle, theOver, theDown, true);
}

void cvui_image(img_t* theWhere, int theX, int theY, img_t* theImage) {
  gScreen.where = theWhere;
  image(gScreen, theX, theY, theImage);
}

bool cvui_checkbox(img_t* theWhere, int theX, int theY, const char* theLabel, bool *theState, unsigned int theColor = 0xCECECE) {
  gScreen.where = theWhere;
  return block_checkbox(gScreen, theX, theY, theLabel, theState, theColor);
}

void cvui_text(img_t* theWhere, int theX, int theY, const char* theText, double theFontScale = 0.5, unsigned int theColor = 0xCECECE) {
  gScreen.where = theWhere;
  block_text(gScreen, theX, theY, theText, theFontScale, theColor, true);
}

void cvui_printf(img_t* theWhere, int theX, int theY, double theFontScale, unsigned int theColor, const char *theFmt, ...) {
  va_list aArgs;
  
  va_start(aArgs, theFmt);
  _vsnprintf(gBuffer, countof(gBuffer), theFmt, aArgs);
  va_end(aArgs);
  
  gScreen.where = theWhere;
  block_text(gScreen, theX, theY, gBuffer, theFontScale, theColor, true);
}

void cvui_printf(img_t* theWhere, int theX, int theY, const char *theFmt, ...) {
  va_list aArgs;
  
  va_start(aArgs, theFmt);
  _vsnprintf(gBuffer, countof(gBuffer), theFmt, aArgs);
  va_end(aArgs);
  
  gScreen.where = theWhere;
  block_text(gScreen, theX, theY, gBuffer, 0.4, 0xCECECE, true);
}

int cvui_counter(img_t* theWhere, int theX, int theY, int *theValue, int theStep = 1, const char *theFormat = "%d") {
  gScreen.where = theWhere;
  return block_counter(gScreen, theX, theY, theValue, theStep, theFormat);
}

double cvui_counter(img_t* theWhere, int theX, int theY, double *theValue, double theStep = 1, const char *theFormat = "%d") {
  gScreen.where = theWhere;
  return block_counter(gScreen, theX, theY, theValue, theStep, theFormat);
}

void cvui_window(img_t* theWhere, int theX, int theY, int theWidth, int theHeight, const char* theTitle) {
  gScreen.where = theWhere;
  block_window(gScreen, theX, theY, theWidth, theHeight, theTitle);
}

void cvui_rect(img_t* theWhere, int theX, int theY, int theWidth, int theHeight, unsigned int theBorderColor, unsigned int theFillingColor = 0xff000000) {
  gScreen.where = theWhere;
  block_rect(gScreen, theX, theY, theWidth, theHeight, theBorderColor, theFillingColor);
}

void cvui_sparkline(img_t* theWhere, const double* theValues, int theValues_size, int theX, int theY, int theWidth, int theHeight, unsigned int theColor) {
  gScreen.where = theWhere;
  block_sparkline(gScreen, theValues, theValues_size, theX, theY, theWidth, theHeight, theColor);
}

void cvui_beginRow(img_t* theWhere, int theX, int theY, int theWidth, int theHeight, int thePadding) {
  cvui_begin(CVUI_ROW, theWhere, theX, theY, theWidth, theHeight, thePadding);
}

void cvui_endRow() {
  cvui_end(CVUI_ROW);
}

void cvui_beginColumn(img_t* theWhere, int theX, int theY, int theWidth, int theHeight, int thePadding) {
  cvui_begin(CVUI_COLUMN, theWhere, theX, theY, theWidth, theHeight, thePadding);
}

void cvui_endColumn() {
  cvui_end(CVUI_COLUMN);
}

void cvui_beginRow(int theWidth, int theHeight, int thePadding) {
  cvui_block_t& aBlock = topBlock();
  cvui_begin(CVUI_ROW, aBlock.where, aBlock.anchor.x, aBlock.anchor.y, theWidth, theHeight, thePadding);
}

void cvui_beginColumn(int theWidth, int theHeight, int thePadding) {
  cvui_block_t& aBlock = topBlock();
  cvui_begin(CVUI_COLUMN, aBlock.where, aBlock.anchor.x, aBlock.anchor.y, theWidth, theHeight, thePadding);
}

void cvui_space(int theValue) {
  cvui_block_t& aBlock = topBlock();
  CSize aSize = cSize(theValue, theValue);
  block_updateLayoutFlow(aBlock, aSize);
}

bool cvui_button(const char* theLabel) {
  cvui_block_t& aBlock = topBlock();
  return block_button(aBlock, aBlock.anchor.x, aBlock.anchor.y, theLabel);
}

bool cvui_button(int theWidth, int theHeight, const char* theLabel) {
  cvui_block_t& aBlock = topBlock();
  return block_button(aBlock, aBlock.anchor.x, aBlock.anchor.y, theWidth, theHeight, theLabel, true);
}

bool cvui_button(img_t* theIdle, img_t* theOver, img_t* theDown) {
  cvui_block_t& aBlock = topBlock();
  return block_button(aBlock, aBlock.anchor.x, aBlock.anchor.y, theIdle, theOver, theDown, true);
}

void cvui_image(img_t* theImage) {
  cvui_block_t& aBlock = topBlock();
  image(aBlock, aBlock.anchor.x, aBlock.anchor.y, theImage);
}

bool cvui_checkbox(const char* theLabel, bool *theState, unsigned int theColor) {
  cvui_block_t& aBlock = topBlock();
  return block_checkbox(aBlock, aBlock.anchor.x, aBlock.anchor.y, theLabel, theState, theColor);
}

void cvui_text(const char* theText, double theFontScale, unsigned int theColor = 0xCECECE) {
  cvui_block_t& aBlock = topBlock();
  block_text(aBlock, aBlock.anchor.x, aBlock.anchor.y, theText, theFontScale, theColor, true);
}

void cvui_printf(double theFontScale, unsigned int theColor, const char *theFmt, ...) {
  cvui_block_t& aBlock = topBlock();
  va_list aArgs;
  
  va_start(aArgs, theFmt);
  _vsnprintf(gBuffer, countof(gBuffer), theFmt, aArgs);
  va_end(aArgs);
  
  block_text(aBlock, aBlock.anchor.x, aBlock.anchor.y, gBuffer, theFontScale, theColor, true);
}

void cvui_printf(const char *theFmt, ...) {
  cvui_block_t& aBlock = topBlock();
  va_list aArgs;
  
  va_start(aArgs, theFmt);
  _vsnprintf(gBuffer, countof(gBuffer), theFmt, aArgs);
  va_end(aArgs);
  
  block_text(aBlock, aBlock.anchor.x, aBlock.anchor.y, gBuffer, 0.4, 0xCECECE, true);
}

int cvui_counter(int *theValue, int theStep, const char *theFormat) {
  cvui_block_t& aBlock = topBlock();
  return block_counter(aBlock, aBlock.anchor.x, aBlock.anchor.y, theValue, theStep, theFormat);
}

double cvui_counter(double *theValue, double theStep, const char *theFormat) {
  cvui_block_t& aBlock = topBlock();
  return block_counter(aBlock, aBlock.anchor.x, aBlock.anchor.y, theValue, theStep, theFormat);
}

void cvui_window(int theWidth, int theHeight, const char* theTitle) {
  cvui_block_t& aBlock = topBlock();
  block_window(aBlock, aBlock.anchor.x, aBlock.anchor.y, theWidth, theHeight, theTitle);
}

void cvui_rect(int theWidth, int theHeight, unsigned int theBorderColor, unsigned int theFillingColor = 0xff000000) {
  cvui_block_t& aBlock = topBlock();
  block_rect(aBlock, aBlock.anchor.x, aBlock.anchor.y, theWidth, theHeight, theBorderColor, theFillingColor);
}

void cvui_sparkline(const double* theValues, int theValues_size, int theWidth, int theHeight, unsigned int theColor) {
  cvui_block_t& aBlock = topBlock();
  block_sparkline(aBlock, theValues, theValues_size, aBlock.anchor.x, aBlock.anchor.y, theWidth, theHeight, theColor);
}