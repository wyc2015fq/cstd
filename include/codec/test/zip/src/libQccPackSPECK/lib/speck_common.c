/*
 * 
 * QccPack: Quantization, compression, and coding libraries
 * Copyright (C) 1997-2009  James E. Fowler
 * 
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Library General Public
 * License as published by the Free Software Foundation; either
 * version 2 of the License, or (at your option) any later version.
 * 
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Library General Public License for more details.
 * 
 * You should have received a copy of the GNU Library General Public
 * License along with this library; if not, write to the
 * Free Software Foundation, Inc., 675 Mass Ave, Cambridge,
 * MA 02139, USA.
 * 
 */


/*
 * ----------------------------------------------------------------------------
 * 
 * Public License for the SPECK Algorithm
 * Version 1.1, March 8, 2004
 * 
 * ----------------------------------------------------------------------------
 * 
 * The Set-Partitioning Embedded Block (SPECK) algorithm is protected by US
 * Patent #6,671,413 (issued December 30, 2003) and by patents pending.
 * An implementation of the SPECK algorithm is included herein with the
 * gracious permission of Dr. William A. Pearlman, exclusive holder of patent
 * rights. Dr. Pearlman has granted the following license governing the terms
 * and conditions for use, copying, distribution, and modification of the
 * SPECK algorithm implementation contained herein (hereafter referred to as
 * "the SPECK source code").
 * 
 * 0. Use of the SPECK source code, including any executable-program or
 *    linkable-library form resulting from its compilation, is restricted to
 *    solely academic or non-commercial research activities.
 * 
 * 1. Any other use, including, but not limited to, use in the development
 *    of a commercial product, use in a commercial application, or commercial
 *    distribution, is prohibited by this license. Such acts require a separate
 *    license directly from Dr. Pearlman.
 * 
 * 2. For academic and non-commercial purposes, this license does not restrict
 *    use; copying, distribution, and modification are permitted under the
 *    terms of the GNU General Public License as published by the Free Software
 *    Foundation, with the further restriction that the terms of the present
 *    license shall also apply to all subsequent copies, distributions,
 *    or modifications of the SPECK source code.
 * 
 * NO WARRANTY
 * 
 * 3. Dr. Pearlman dislaims all warranties, expressed or implied, including
 *    without limitation any warranty whatsoever as to the fitness for a
 *    particular use or the merchantability of the SPECK source code.
 * 
 * 4. In no event shall Dr. Pearlman be liable for any loss of profits, loss
 *    of business, loss of use or loss of data, nor for indirect, special,
 *    incidental or consequential damages of any kind related to use of the
 *    SPECK source code.
 * 
 * 
 * END OF TERMS AND CONDITIONS
 * 
 * 
 * Persons desiring to license the SPECK algorithm for commercial purposes or
 * for uses otherwise prohibited by this license may wish to contact
 * Dr. Pearlman regarding the possibility of negotiating such licenses:
 * 
 *   Dr. William A. Pearlman
 *   Dept. of Electrical, Computer, and Systems Engineering
 *   Rensselaer Polytechnic Institute
 *   Troy, NY 12180-3590
 *   U.S.A.
 *   email: pearlman@ecse.rpi.edu
 *   tel.: (518) 276-6082
 *   fax: (518) 276-6261
 *  
 * ----------------------------------------------------------------------------
 */


#include "libQccPack.h"


void QccSPECKHeader(void)
{
  QccString header;

  QccStringSprintf(header,
                   "%s\n\nQccPackSPECK Module Version %d.%d %s,\n%s\n\nfor:",
                   QCCSPECK_LICENSE,
                   QCCSPECK_MAJORVERSION,
                   QCCSPECK_MINORVERSION,
                   QCCSPECK_DATE,
                   QCCSPECK_COPYRIGHT);

  QccSetUserHeader(header);
}


void QccSPECK3DHeader(void)
{
  QccString header;

  QccStringSprintf(header,
                   "%s\n\nQccPackSPECK Module Version %d.%d %s,\n%s\n\nfor:",
                   QCCSPECK3D_LICENSE,
                   QCCSPECK_MAJORVERSION,
                   QCCSPECK_MINORVERSION,
                   QCCSPECK_DATE,
                   QCCSPECK_COPYRIGHT);

  QccSetUserHeader(header);
}
