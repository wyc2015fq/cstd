package org.opencv.test.core;

import org.opencv.core.CvType;
import org.opencv.test.OpenCVTestCase;

public class CvTypeTest extends OpenCVTestCase {

    public void testMakeType() {
        assertEquals(CvType.CC_8UC4, CvType.makeType(CvType.CC_8U, 4));
    }

    public void testCC_8UC() {
        assertEquals(CvType.CC_8UC4, CvType.CC_8UC(4));
    }

    public void testCC_8SC() {
        assertEquals(CvType.CC_8SC4, CvType.CC_8SC(4));
    }

    public void testCC_16UC() {
        assertEquals(CvType.CC_16UC4, CvType.CC_16UC(4));
    }

    public void testCC_16SC() {
        assertEquals(CvType.CC_16SC4, CvType.CC_16SC(4));
    }

    public void testCC_32SC() {
        assertEquals(CvType.CC_32SC4, CvType.CC_32SC(4));
    }

    public void testCC_32FC() {
        assertEquals(CvType.CC_32FC4, CvType.CC_32FC(4));
    }

    public void testCC_64FC() {
        assertEquals(CvType.CC_64FC4, CvType.CC_64FC(4));
    }

    public void testChannels() {
        assertEquals(1, CvType.channels(CvType.CC_64F));
    }

    public void testDepth() {
        assertEquals(CvType.CC_64F, CvType.depth(CvType.CC_64FC3));
    }

    public void testIsInteger() {
        assertFalse(CvType.isInteger(CvType.CC_32FC3));
        assertTrue(CvType.isInteger(CvType.CC_16S));
    }

    public void testELEM_SIZE() {
        assertEquals(3 * 8, CvType.ELEM_SIZE(CvType.CC_64FC3));
    }

    public void testTypeToString() {
        assertEquals("CC_32FC1", CvType.typeToString(CvType.CC_32F));
        assertEquals("CC_32FC3", CvType.typeToString(CvType.CC_32FC3));
        assertEquals("CC_32FC(128)", CvType.typeToString(CvType.CC_32FC(128)));
    }

}
