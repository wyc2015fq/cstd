// This file is part of Eigen, a lightweight C++ template library
// for linear algebra.
//
// Copyright (C) 2008-2015 Gael Guennebaud <gael.guennebaud@inria.fr>
// Copyright (C) 2006-2008 Benoit Jacob <jacob.benoit.1@gmail.com>
//
// This Source Code Form is subject to the terms of the Mozilla
// Public License v. 2.0. If a copy of the MPL was not distributed
// with this file, You can obtain one at http://mozilla.org/MPL/2.0/.

#ifndef EIGEN_META_H
#define EIGEN_META_H

#if defined(__CUDA_ARCH__)
//#include <cfloat>
//#include <math_constants.h>
#endif

#if EIGEN_COMP_ICC>=1600 &&  __cplusplus >= 201103L
//#include <cstdint>
#endif

typedef EIGEN_DEFAULT_DENSE_INDEX_TYPE DenseIndex;

/**
 * \brief The Index type as used for the API.
 * \details To change this, \c \#define the preprocessor symbol \c EIGEN_DEFAULT_DENSE_INDEX_TYPE.
 * \sa \blank \ref TopicPreprocessorDirectives, StorageIndex.
 */

typedef EIGEN_DEFAULT_DENSE_INDEX_TYPE Index;


#endif // EIGEN_META_H
