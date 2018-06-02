package org.opencv.core;

public final class CvType {

    // type depth constants
    public static final int
            CC_8U = 0, CC_8S = 1,
            CC_16U = 2, CC_16S = 3,
            CC_32S = 4,
            CC_32F = 5,
            CC_64F = 6,
            CC_USRTYPE1 = 7;

    // predefined type constants
    public static final int
            CC_8UC1 = CC_8UC(1), CC_8UC2 = CC_8UC(2), CC_8UC3 = CC_8UC(3), CC_8UC4 = CC_8UC(4),
            CC_8SC1 = CC_8SC(1), CC_8SC2 = CC_8SC(2), CC_8SC3 = CC_8SC(3), CC_8SC4 = CC_8SC(4),
            CC_16UC1 = CC_16UC(1), CC_16UC2 = CC_16UC(2), CC_16UC3 = CC_16UC(3), CC_16UC4 = CC_16UC(4),
            CC_16SC1 = CC_16SC(1), CC_16SC2 = CC_16SC(2), CC_16SC3 = CC_16SC(3), CC_16SC4 = CC_16SC(4),
            CC_32SC1 = CC_32SC(1), CC_32SC2 = CC_32SC(2), CC_32SC3 = CC_32SC(3), CC_32SC4 = CC_32SC(4),
            CC_32FC1 = CC_32FC(1), CC_32FC2 = CC_32FC(2), CC_32FC3 = CC_32FC(3), CC_32FC4 = CC_32FC(4),
            CC_64FC1 = CC_64FC(1), CC_64FC2 = CC_64FC(2), CC_64FC3 = CC_64FC(3), CC_64FC4 = CC_64FC(4);

    private static final int CC_CN_MAX = 512, CC_CN_SHIFT = 3, CC_DEPTH_MAX = (1 << CC_CN_SHIFT);

    public static final int makeType(int depth, int channels) {
        if (channels <= 0 || channels >= CC_CN_MAX) {
            throw new java.lang.UnsupportedOperationException(
                    "Channels count should be 1.." + (CC_CN_MAX - 1));
        }
        if (depth < 0 || depth >= CC_DEPTH_MAX) {
            throw new java.lang.UnsupportedOperationException(
                    "Data type depth should be 0.." + (CC_DEPTH_MAX - 1));
        }
        return (depth & (CC_DEPTH_MAX - 1)) + ((channels - 1) << CC_CN_SHIFT);
    }

    public static final int CC_8UC(int ch) {
        return makeType(CC_8U, ch);
    }

    public static final int CC_8SC(int ch) {
        return makeType(CC_8S, ch);
    }

    public static final int CC_16UC(int ch) {
        return makeType(CC_16U, ch);
    }

    public static final int CC_16SC(int ch) {
        return makeType(CC_16S, ch);
    }

    public static final int CC_32SC(int ch) {
        return makeType(CC_32S, ch);
    }

    public static final int CC_32FC(int ch) {
        return makeType(CC_32F, ch);
    }

    public static final int CC_64FC(int ch) {
        return makeType(CC_64F, ch);
    }

    public static final int channels(int type) {
        return (type >> CC_CN_SHIFT) + 1;
    }

    public static final int depth(int type) {
        return type & (CC_DEPTH_MAX - 1);
    }

    public static final boolean isInteger(int type) {
        return depth(type) < CC_32F;
    }

    public static final int ELEM_SIZE(int type) {
        switch (depth(type)) {
        case CC_8U:
        case CC_8S:
            return channels(type);
        case CC_16U:
        case CC_16S:
            return 2 * channels(type);
        case CC_32S:
        case CC_32F:
            return 4 * channels(type);
        case CC_64F:
            return 8 * channels(type);
        default:
            throw new java.lang.UnsupportedOperationException(
                    "Unsupported CvType value: " + type);
        }
    }

    public static final String typeToString(int type) {
        String s;
        switch (depth(type)) {
        case CC_8U:
            s = "CC_8U";
            break;
        case CC_8S:
            s = "CC_8S";
            break;
        case CC_16U:
            s = "CC_16U";
            break;
        case CC_16S:
            s = "CC_16S";
            break;
        case CC_32S:
            s = "CC_32S";
            break;
        case CC_32F:
            s = "CC_32F";
            break;
        case CC_64F:
            s = "CC_64F";
            break;
        case CC_USRTYPE1:
            s = "CC_USRTYPE1";
            break;
        default:
            throw new java.lang.UnsupportedOperationException(
                    "Unsupported CvType value: " + type);
        }

        int ch = channels(type);
        if (ch <= 4)
            return s + "C" + ch;
        else
            return s + "C(" + ch + ")";
    }

}
