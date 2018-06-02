// This file is part of Eigen, a lightweight C++ template library
// for linear algebra.
//
// Copyright (C) 2006-2010 Benoit Jacob <jacob.benoit.1@gmail.com>
//
// This Source Code Form is subject to the terms of the Mozilla
// Public License v. 2.0. If a copy of the MPL was not distributed
// with this file, You can obtain one at http://mozilla.org/MPL/2.0/.

#ifndef EIGEN_NUMTRAITS_H
#define EIGEN_NUMTRAITS_H

// default implementation of digits10(), based on numeric_limits if specialized,
// 0 for integer types, and log10(epsilon()) otherwise.

#define default_digits10_impl (int)(ceil(-log10(epsilon())))



#endif // EIGEN_NUMTRAITS_H
