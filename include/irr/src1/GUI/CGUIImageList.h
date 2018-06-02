// This file is part of the "Irrlicht Engine".
// written by Reinhard Ostermeier, reinhard@nospam.r-ostermeier.de
#ifndef __C_GUI_IMAGE_LIST_H_INCLUDED__
#define __C_GUI_IMAGE_LIST_H_INCLUDED__
//#include "IGUIImageList.h"
//#include "IVideoDriver.h"
class CGUIImageList : public IGUIImageList
{
public:
  //! constructor
  CGUIImageList(IVideoDriver* Driver);
  //! destructor
  ~CGUIImageList();
  //! Creates the image list from texture.
  //! \param texture: The texture to use
  //! \param imageSize: Size of a single image
  //! \param useAlphaChannel: true if the alpha channel from the texture should be used
  //! \return
  //! true if the image list was created
  bool createImageList(
      ITexture*      texture,
      dimension2d<s32>  imageSize,
      bool              useAlphaChannel);
  //! Draws an image and clips it to the specified rectangle if wanted
  //! \param index: Index of the image
  //! \param destPos: Position of the image to draw
  //! \param clip: Optional pointer to a rectalgle against which the text will be clipped.
  //! If the pointer is null, no clipping will be done.
  void draw(s32 index, const position2di& destPos,
      const IRECT* clip = 0);
  //! Returns the count of Images in the list.
  //! \return Returns the count of Images in the list.
  s32 getImageCount() const {
    return ImageCount;
  }
  //! Returns the size of the images in the list.
  //! \return Returns the size of the images in the list.
  dimension2d<s32> getImageSize() const {
    return ImageSize;
  }
private:
  IVideoDriver*    Driver;
  ITexture*      Texture;
  s32             ImageCount;
  dimension2d<s32>  ImageSize;
  s32             ImagesPerRow;
  bool              UseAlphaChannel;
};
#endif

