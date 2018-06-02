// This file is part of OpenCV project.
// It is subject to the license terms in the LICENSE file found in the top-level directory
// of this distribution and at http://opencv.org/license.html.

// Copyright (C) 2016, Intel Corporation, all rights reserved.
// Third party copyrights are property of their respective owners.

// OpenVX related definitions and declarations

#pragma once
#ifndef OPENCC_OVX_DEFS_HPP
#define OPENCC_OVX_DEFS_HPP

#include "cvconfig.h"

// utility macro for running OpenVX-based implementations
#ifdef HAVE_OPENVX

#define IVX_HIDE_INFO_WARNINGS
#define IVX_USE_OPENCV
#include "ivx.hpp"

#define CC_OVX_RUN(condition, func, ...)          \
    if (useOpenVX() && (condition) && func)   \
    {                                             \
        return __VA_ARGS__;                       \
    }

#else
    #define CC_OVX_RUN(condition, func, ...)
#endif // HAVE_OPENVX

// Throw an error in debug mode or try another implementation in release
#ifdef _DEBUG
#define VX_DbgThrow(s) CC_Error(Error::StsInternal, (s))
#else
#define VX_DbgThrow(s) return false
#endif

#endif // OPENCC_OVX_DEFS_HPP
