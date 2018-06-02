// This file is part of OpenCV project.
// It is subject to the license terms in the LICENSE file found in the top-level directory
// of this distribution and at http://opencv.org/license.html.

// Copyright (C) 2014, Itseez, Inc., all rights reserved.
// Third party copyrights are property of their respective owners.

#include "../test_precomp.hpp"
#include "opencv2/ts/ocl_test.hpp"

#ifdef HAVE_OPENCL

namespace cvtest {
namespace ocl {

TEST(Image2D, createAliasEmptyUMat)
{
    if (ocl::haveOpenCL())
    {
        UMat um;
        EXPECT_FALSE(ocl::Image2D::canCreateAlias(um));
    }
    else
        std::cout << "OpenCL runtime not found. Test skipped." << std::endl;
}

TEST(Image2D, createImage2DWithEmptyUMat)
{
    if (ocl::haveOpenCL())
    {
        UMat um;
        EXPECT_ANY_THROW(ocl::Image2D image(um));
    }
    else
        std::cout << "OpenCL runtime not found. Test skipped." << std::endl;
}

TEST(Image2D, createAlias)
{
    if (ocl::haveOpenCL())
    {
        const ocl::Device & d = ocl::Device::getDefault();
        int minor = d.deviceVersionMinor(), major = d.deviceVersionMajor();

        // aliases is OpenCL 1.2 extension
        if (1 < major || (1 == major && 2 <= minor))
        {
            UMat um(128, 128, CC_8UC1);
            bool isFormatSupported = false, canCreateAlias = false;

            EXPECT_NO_THROW(isFormatSupported = ocl::Image2D::isFormatSupported(CC_8U, 1, false));
            EXPECT_NO_THROW(canCreateAlias = ocl::Image2D::canCreateAlias(um));

            if (isFormatSupported && canCreateAlias)
            {
                EXPECT_NO_THROW(ocl::Image2D image(um, false, true));
            }
            else
                std::cout << "Impossible to create alias for selected image. Test skipped." << std::endl;
        }
    }
    else
        std::cout << "OpenCL runtime not found. Test skipped" << std::endl;
}

TEST(Image2D, turnOffOpenCL)
{
    if (ocl::haveOpenCL())
    {
        // save the current state
        bool useOCL = ocl::useOpenCL();
        bool isFormatSupported = false;

        ocl::setUseOpenCL(true);
        UMat um(128, 128, CC_8UC1);

        ocl::setUseOpenCL(false);
        EXPECT_NO_THROW(isFormatSupported = ocl::Image2D::isFormatSupported(CC_8U, 1, true));

        if (isFormatSupported)
        {
            EXPECT_NO_THROW(ocl::Image2D image(um));
        }
        else
            std::cout << "CC_8UC1 is not supported for OpenCL images. Test skipped." << std::endl;
    
        // reset state to the previous one
        ocl::setUseOpenCL(useOCL);
    }
    else
        std::cout << "OpenCL runtime not found. Test skipped." << std::endl;
}

} } // namespace cvtest::ocl

#endif // HAVE_OPENCL