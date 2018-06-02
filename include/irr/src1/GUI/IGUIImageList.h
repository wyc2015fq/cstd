// This file is part of the "Irrlicht Engine".
// written by Reinhard Ostermeier, reinhard@nospam.r-ostermeier.de
#ifndef __I_GUI_IMAGE_LIST_H_INCLUDED__
#define __I_GUI_IMAGE_LIST_H_INCLUDED__
//#include "IGUIElement.h"
//! Font interface.
class IGUIImageList
{
public:
  //! Destructor
  ~IGUIImageList() {};
  //! Draws an image and clips it to the specified rectangle if wanted
  //! \param index: Index of the image
  //! \param destPos: Position of the image to draw
  //! \param clip: Optional pointer to a rectalgle against which the text will be clipped.
  //! If the pointer is null, no clipping will be done.
  void draw(s32 index, const position2di& destPos,
      const IRECT* clip = 0) = 0;
  //! Returns the count of Images in the list.
  //! \return Returns the count of Images in the list.
  s32 getImageCount() = 0;
  //! Returns the size of the images in the list.
  //! \return Returns the size of the images in the list.
  dimension2d<s32> getImageSize() = 0;
};
#endif

