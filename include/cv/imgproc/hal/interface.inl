#ifndef OPENCC_IMGPROC_HAL_INTERFACE_H
#define OPENCC_IMGPROC_HAL_INTERFACE_H

//! @addtogroup imgproc_hal_interface
//! @{

//! @name Interpolation modes
//! @sa InterpolationFlags
//! @{
#define CC_HAL_INTER_NEAREST 0
#define CC_HAL_INTER_LINEAR 1
#define CC_HAL_INTER_CUBIC 2
#define CC_HAL_INTER_AREA 3
#define CC_HAL_INTER_LANCZOS4 4
//! @}

//! @name Morphology operations
//! @sa MorphTypes
//! @{
#define CC_MORPH_ERODE 0
#define CC_MORPH_DILATE 1
//! @}

//! @}

#endif
