
#ifndef _VTKRENDERERCOLLECTION_H_
#define _VTKRENDERERCOLLECTION_H_
#include "vtkRenderer.h"

#include <cstdlib>

// Forward the Render() method to each renderer in the list.

void vtkRenderers_Renderer(vtkRendererCollection* rens)
{

  vtkRenderer      *ren, *firstRen;
  vtkRenderWindow  *renWin;
  int               numLayers, i, j;

  if (rens->first == 0)
  {
    // We cannot determine the number of layers because there are no
    // renderers.  No problem, just return.
    return;
  }
  firstRen = rens->first;
  renWin = firstRen->GetRenderWindow();
  numLayers = renWin->GetNumberOfLayers();

  // Only have the renderers render from back to front.  This is necessary
  // because transparent renderers clear the z-buffer before each render and
  // then overlay their image.
  for (i = 0; i < numLayers; i++)
  {
    foreach_next(rens, ren)
    {
      if (ren->GetLayer() == i)
      {
        ren->Render();
      }
    }
  }

  // Let the user know if they have put a renderer at an unused layer.
  foreach_next(rens, ren)
  {
    if (ren->GetLayer() < 0 || ren->GetLayer() >= numLayers)
    {
      vtkErrorMacro(<< "Invalid layer for renderer: not rendered.");
    }
  }
}


#endif // _VTKRENDERERCOLLECTION_H_
