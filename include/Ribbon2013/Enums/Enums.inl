
enum RibbonTheme
{
  RibbonThemeNormal,
    RibbonThemeBlue,
    RibbonThemeBlack,
    RibbonThemeGreen,
    RibbonThemePurple,
    RibbonThemeJellyBelly,
    RibbonThemeHalloween
};

enum RibbonOrbStyle
{
  RibbonOrbStyleOffice_2007,
    RibbonOrbStyleOffice_2010,
    RibbonOrbStyleOffice_2013 //Michael Spradlin - 05/03/2013 Office 2013 Style Changes
};

/// <summary>
/// Represents the directions that arrows can have
/// </summary>
enum RibbonArrowDirection
{
  /// <summary>
  /// The arrow points up
  /// </summary>
  RibbonArrowDirectionUp,
    
    /// <summary>
    /// The arrow points down
    /// </summary>
    RibbonArrowDirectionDown,
    
    /// <summary>
    /// The arrow points right
    /// </summary>
    RibbonArrowDirectionRight,
    
    /// <summary>
    /// The arrow points left
    /// </summary>
    RibbonArrowDirectionLeft
};

enum RibbonButtonStyle
{
  /// <summary>
  /// Simple clickable button
  /// </summary>
  RibbonButtonStyleNormal,
    /// <summary>
    /// Button with a right side drop down
    /// </summary>
    RibbonButtonStyleDropDown,
    /// <summary>
    /// Button with an optional dropdown attachment on the right
    /// </summary>
    RibbonButtonStyleSplitDropDown,
    /// <summary>
    /// Mimics a standard drop down list item with no image
    /// </summary>
    RibbonButtonStyleDropDownListItem,
};

/// <summary>
/// Possible modes for the ribbon to be placed on the window
/// </summary>
enum RibbonWindowMode
{
  RibbonWindowModeInsideWindow,
    RibbonWindowModeNonClientAreaCustomDrawn,
    RibbonWindowModeNonClientAreaGlass
};

/// <summary>
/// Represents possible flow directions of items on the panels
/// </summary>
enum RibbonPanelFlowDirection
{
  /// <summary>
  /// Layout of items flows to the left, then down
  /// </summary>
  RibbonPanelFlowDirectionLeft = 2,
    /// <summary>
    /// Layout of items flows to the Right, then down
    /// </summary>
    RibbonPanelFlowDirectionRight = 1,
    /// <summary>
    /// Layout of items flows to the bottom, then to the right
    /// </summary>
    RibbonPanelFlowDirectionBottom = 0
};

/// <summary>
/// Represents the size modes that a RibbonElement can be
/// </summary>
enum RibbonElementSizeMode
{
  /// <summary>
  /// The item is being shown on a dropdown
  /// </summary>
  RibbonElementSizeModeDropDown = 5,
    
    /// <summary>
    /// Maximum size the element can reach
    /// </summary>
    RibbonElementSizeModeLarge = 4,
    
    /// <summary>
    /// A medium size for the element when not much space is available
    /// </summary>
    RibbonElementSizeModeRibbonElementSizeModeMedium = 3,
    
    /// <summary>
    /// The minimum size the element can be
    /// </summary>
    RibbonElementSizeModeCompact = 2,
    
    /// <summary>
    /// The item doesn't fit as compact, so it must be shown as a single button
    /// </summary>
    RibbonElementSizeModeOverflow = 1,
    
    /// <summary>
    /// No size mode specified
    /// </summary>
    RibbonElementSizeModeNone = 0,
};
