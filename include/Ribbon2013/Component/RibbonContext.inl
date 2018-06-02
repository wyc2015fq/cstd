
/// <summary>
/// Represents a context on the Ribbon
/// </summary>
/// <remarks>Contexts are useful when some tabs are volatile, depending on some selection. A RibbonTabContext can be added to the ribbon by calling Ribbon.Contexts.Add</remarks>
struct RibbonContext// : Component
{
  /// <summary>
  /// Gets or sets the text of the Context
  /// </summary>
  const char* Text;
  
  /// <summary>
  /// Gets or sets the color of the glow that indicates a context
  /// </summary>
  Color GlowColor;
  
  /// <summary>
  /// Gets the Ribbon that owns this context
  /// </summary>
  //Ribbon Owner;
  
  //Ribbon _owner;
  //RibbonTabCollection Tabs
  
  /// <summary>
  /// Sets the value of the Owner Property
  /// </summary>
};

