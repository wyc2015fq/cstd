#include "opencv2/core.hpp"
#include <iostream>
#include <string>

static CvFileStorage * three_same_ways_of_write_base64()
{
    CvFileStorage * fs = 0;
    RNG rng;
    switch ( rng.uniform( 0, 2 ) )
    {
    case 0:
        //! [suffix_in_file_name]
        fs = cvOpenFileStorage( "example.yml?base64", 0, CC_STORAGE_WRITE );
        //! [suffix_in_file_name]
        break;
    case 1:
        //! [flag_write_base64]
        fs = cvOpenFileStorage( "example.yml"       , 0, CC_STORAGE_WRITE_BASE64 );
        //! [flag_write_base64]
        break;
    case 2:
        //! [flag_write_and_flag_base64]
        fs = cvOpenFileStorage( "example.yml"       , 0, CC_STORAGE_WRITE | CC_STORAGE_BASE64 );
        //! [flag_write_and_flag_base64]
        break;
    default:
        break;
    }
    return fs;
}

static void two_ways_to_write_rawdata_in_base64()
{
    std::vector<int> rawdata(10, 0x00010203);

    {   // [1]
        //! [without_base64_flag]
        CvFileStorage* fs = cvOpenFileStorage( "example.xml", 0, CC_STORAGE_WRITE );
        // both CC_NODE_SEQ and "binary" are necessary.
        cvWriteStructBegin(fs, "rawdata", CC_NODE_SEQ | CC_NODE_FLOW, "binary");
        cvWriteRawDataBase64(fs, rawdata.data(), static_cast<int>(rawdata.size()), "i");
        cvWriteStructEnd(fs);
        cvReleaseFileStorage( &fs );
        //! [without_base64_flag]
    }

    {   // [2]
        //! [with_write_base64_flag]
        CvFileStorage* fs = cvOpenFileStorage( "example.xml", 0, CC_STORAGE_WRITE_BASE64);
        // parameter, typename "binary" could be omitted.
        cvWriteStructBegin(fs, "rawdata", CC_NODE_SEQ | CC_NODE_FLOW);
        cvWriteRawData(fs, rawdata.data(), static_cast<int>(rawdata.size()), "i");
        cvWriteStructEnd(fs);
        cvReleaseFileStorage( &fs );
        //! [with_write_base64_flag]
    }
}

int main(int /* argc */, char** /* argv */)
{
    {   // base64 mode
        CvFileStorage * fs = three_same_ways_of_write_base64();
        cvReleaseFileStorage( &fs );
    }

    {   // output rawdata by `cvWriteRawdata*`
        two_ways_to_write_rawdata_in_base64();
    }

    return 0;
}
