
#ifndef _FACE_INTERFACE_INL_
#define _FACE_INTERFACE_INL_

#if defined _WIN32 || defined __linux__
#include "face_interface_arm.inl"
#else
#include "face_interface_dsp.inl"
#endif

#endif // _FACE_INTERFACE_INL_
