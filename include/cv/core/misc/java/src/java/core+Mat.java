package org.opencv.core;

// C++: class CvMat
//javadoc: CvMat
public class CvMat {

    public final long nativeObj;

    public CvMat(long addr)
    {
        if (addr == 0)
            throw new java.lang.UnsupportedOperationException("Native object address is NULL");
        nativeObj = addr;
    }

    //
    // C++: CvMat::CvMat()
    //

    // javadoc: CvMat::CvMat()
    public CvMat()
    {

        nativeObj = n_Mat();

        return;
    }

    //
    // C++: CvMat::CvMat(int rows, int cols, int type)
    //

    // javadoc: CvMat::CvMat(rows, cols, type)
    public CvMat(int rows, int cols, int type)
    {

        nativeObj = n_Mat(rows, cols, type);

        return;
    }

    //
    // C++: CvMat::CvMat(Size size, int type)
    //

    // javadoc: CvMat::CvMat(size, type)
    public CvMat(Size size, int type)
    {

        nativeObj = n_Mat(size.width, size.height, type);

        return;
    }

    //
    // C++: CvMat::CvMat(int rows, int cols, int type, Scalar s)
    //

    // javadoc: CvMat::CvMat(rows, cols, type, s)
    public CvMat(int rows, int cols, int type, Scalar s)
    {

        nativeObj = n_Mat(rows, cols, type, s.val[0], s.val[1], s.val[2], s.val[3]);

        return;
    }

    //
    // C++: CvMat::CvMat(Size size, int type, Scalar s)
    //

    // javadoc: CvMat::CvMat(size, type, s)
    public CvMat(Size size, int type, Scalar s)
    {

        nativeObj = n_Mat(size.width, size.height, type, s.val[0], s.val[1], s.val[2], s.val[3]);

        return;
    }

    //
    // C++: CvMat::CvMat(CvMat m, Range rowRange, Range colRange = Range::all())
    //

    // javadoc: CvMat::CvMat(m, rowRange, colRange)
    public CvMat(CvMat m, Range rowRange, Range colRange)
    {

        nativeObj = n_Mat(m.nativeObj, rowRange.start, rowRange.end, colRange.start, colRange.end);

        return;
    }

    // javadoc: CvMat::CvMat(m, rowRange)
    public CvMat(CvMat m, Range rowRange)
    {

        nativeObj = n_Mat(m.nativeObj, rowRange.start, rowRange.end);

        return;
    }

    //
    // C++: CvMat::CvMat(CvMat m, Rect roi)
    //

    // javadoc: CvMat::CvMat(m, roi)
    public CvMat(CvMat m, Rect roi)
    {

        nativeObj = n_Mat(m.nativeObj, roi.y, roi.y + roi.height, roi.x, roi.x + roi.width);

        return;
    }

    //
    // C++: CvMat CvMat::adjustROI(int dtop, int dbottom, int dleft, int dright)
    //

    // javadoc: CvMat::adjustROI(dtop, dbottom, dleft, dright)
    public CvMat adjustROI(int dtop, int dbottom, int dleft, int dright)
    {

        CvMat retVal = new CvMat(n_adjustROI(nativeObj, dtop, dbottom, dleft, dright));

        return retVal;
    }

    //
    // C++: void CvMat::assignTo(CvMat m, int type = -1)
    //

    // javadoc: CvMat::assignTo(m, type)
    public void assignTo(CvMat m, int type)
    {

        n_assignTo(nativeObj, m.nativeObj, type);

        return;
    }

    // javadoc: CvMat::assignTo(m)
    public void assignTo(CvMat m)
    {

        n_assignTo(nativeObj, m.nativeObj);

        return;
    }

    //
    // C++: int CvMat::channels()
    //

    // javadoc: CvMat::channels()
    public int channels()
    {

        int retVal = n_channels(nativeObj);

        return retVal;
    }

    //
    // C++: int CvMat::checkVector(int elemChannels, int depth = -1, bool
    // requireContinuous = true)
    //

    // javadoc: CvMat::checkVector(elemChannels, depth, requireContinuous)
    public int checkVector(int elemChannels, int depth, boolean requireContinuous)
    {

        int retVal = n_checkVector(nativeObj, elemChannels, depth, requireContinuous);

        return retVal;
    }

    // javadoc: CvMat::checkVector(elemChannels, depth)
    public int checkVector(int elemChannels, int depth)
    {

        int retVal = n_checkVector(nativeObj, elemChannels, depth);

        return retVal;
    }

    // javadoc: CvMat::checkVector(elemChannels)
    public int checkVector(int elemChannels)
    {

        int retVal = n_checkVector(nativeObj, elemChannels);

        return retVal;
    }

    //
    // C++: CvMat CvMat::clone()
    //

    // javadoc: CvMat::clone()
    public CvMat clone()
    {

        CvMat retVal = new CvMat(n_clone(nativeObj));

        return retVal;
    }

    //
    // C++: CvMat CvMat::col(int x)
    //

    // javadoc: CvMat::col(x)
    public CvMat col(int x)
    {

        CvMat retVal = new CvMat(n_col(nativeObj, x));

        return retVal;
    }

    //
    // C++: CvMat CvMat::colRange(int startcol, int endcol)
    //

    // javadoc: CvMat::colRange(startcol, endcol)
    public CvMat colRange(int startcol, int endcol)
    {

        CvMat retVal = new CvMat(n_colRange(nativeObj, startcol, endcol));

        return retVal;
    }

    //
    // C++: CvMat CvMat::colRange(Range r)
    //

    // javadoc: CvMat::colRange(r)
    public CvMat colRange(Range r)
    {

        CvMat retVal = new CvMat(n_colRange(nativeObj, r.start, r.end));

        return retVal;
    }

    //
    // C++: int CvMat::dims()
    //

    // javadoc: CvMat::dims()
    public int dims()
    {

        int retVal = n_dims(nativeObj);

        return retVal;
    }

    //
    // C++: int CvMat::cols()
    //

    // javadoc: CvMat::cols()
    public int cols()
    {

        int retVal = n_cols(nativeObj);

        return retVal;
    }

    //
    // C++: void CvMat::convertTo(CvMat& m, int rtype, double alpha = 1, double beta
    // = 0)
    //

    // javadoc: CvMat::convertTo(m, rtype, alpha, beta)
    public void convertTo(CvMat m, int rtype, double alpha, double beta)
    {

        n_convertTo(nativeObj, m.nativeObj, rtype, alpha, beta);

        return;
    }

    // javadoc: CvMat::convertTo(m, rtype, alpha)
    public void convertTo(CvMat m, int rtype, double alpha)
    {

        n_convertTo(nativeObj, m.nativeObj, rtype, alpha);

        return;
    }

    // javadoc: CvMat::convertTo(m, rtype)
    public void convertTo(CvMat m, int rtype)
    {

        n_convertTo(nativeObj, m.nativeObj, rtype);

        return;
    }

    //
    // C++: void CvMat::copyTo(CvMat& m)
    //

    // javadoc: CvMat::copyTo(m)
    public void copyTo(CvMat m)
    {

        n_copyTo(nativeObj, m.nativeObj);

        return;
    }

    //
    // C++: void CvMat::copyTo(CvMat& m, CvMat mask)
    //

    // javadoc: CvMat::copyTo(m, mask)
    public void copyTo(CvMat m, CvMat mask)
    {

        n_copyTo(nativeObj, m.nativeObj, mask.nativeObj);

        return;
    }

    //
    // C++: void CvMat::create(int rows, int cols, int type)
    //

    // javadoc: CvMat::create(rows, cols, type)
    public void create(int rows, int cols, int type)
    {

        n_create(nativeObj, rows, cols, type);

        return;
    }

    //
    // C++: void CvMat::create(Size size, int type)
    //

    // javadoc: CvMat::create(size, type)
    public void create(Size size, int type)
    {

        n_create(nativeObj, size.width, size.height, type);

        return;
    }

    //
    // C++: CvMat CvMat::cross(CvMat m)
    //

    // javadoc: CvMat::cross(m)
    public CvMat cross(CvMat m)
    {

        CvMat retVal = new CvMat(n_cross(nativeObj, m.nativeObj));

        return retVal;
    }

    //
    // C++: long CvMat::dataAddr()
    //

    // javadoc: CvMat::dataAddr()
    public long dataAddr()
    {

        long retVal = n_dataAddr(nativeObj);

        return retVal;
    }

    //
    // C++: int CvMat::depth()
    //

    // javadoc: CvMat::depth()
    public int depth()
    {

        int retVal = n_depth(nativeObj);

        return retVal;
    }

    //
    // C++: CvMat CvMat::diag(int d = 0)
    //

    // javadoc: CvMat::diag(d)
    public CvMat diag(int d)
    {

        CvMat retVal = new CvMat(n_diag(nativeObj, d));

        return retVal;
    }

    // javadoc: CvMat::diag()
    public CvMat diag()
    {

        CvMat retVal = new CvMat(n_diag(nativeObj, 0));

        return retVal;
    }

    //
    // C++: static CvMat CvMat::diag(CvMat d)
    //

    // javadoc: CvMat::diag(d)
    public static CvMat diag(CvMat d)
    {

        CvMat retVal = new CvMat(n_diag(d.nativeObj));

        return retVal;
    }

    //
    // C++: double CvMat::dot(CvMat m)
    //

    // javadoc: CvMat::dot(m)
    public double dot(CvMat m)
    {

        double retVal = n_dot(nativeObj, m.nativeObj);

        return retVal;
    }

    //
    // C++: size_t CvMat::elemSize()
    //

    // javadoc: CvMat::elemSize()
    public long elemSize()
    {

        long retVal = n_elemSize(nativeObj);

        return retVal;
    }

    //
    // C++: size_t CvMat::elemSize1()
    //

    // javadoc: CvMat::elemSize1()
    public long elemSize1()
    {

        long retVal = n_elemSize1(nativeObj);

        return retVal;
    }

    //
    // C++: bool CvMat::empty()
    //

    // javadoc: CvMat::empty()
    public boolean empty()
    {

        boolean retVal = n_empty(nativeObj);

        return retVal;
    }

    //
    // C++: static CvMat CvMat::eye(int rows, int cols, int type)
    //

    // javadoc: CvMat::eye(rows, cols, type)
    public static CvMat eye(int rows, int cols, int type)
    {

        CvMat retVal = new CvMat(n_eye(rows, cols, type));

        return retVal;
    }

    //
    // C++: static CvMat CvMat::eye(Size size, int type)
    //

    // javadoc: CvMat::eye(size, type)
    public static CvMat eye(Size size, int type)
    {

        CvMat retVal = new CvMat(n_eye(size.width, size.height, type));

        return retVal;
    }

    //
    // C++: CvMat CvMat::inv(int method = DECOMP_LU)
    //

    // javadoc: CvMat::inv(method)
    public CvMat inv(int method)
    {

        CvMat retVal = new CvMat(n_inv(nativeObj, method));

        return retVal;
    }

    // javadoc: CvMat::inv()
    public CvMat inv()
    {

        CvMat retVal = new CvMat(n_inv(nativeObj));

        return retVal;
    }

    //
    // C++: bool CvMat::isContinuous()
    //

    // javadoc: CvMat::isContinuous()
    public boolean isContinuous()
    {

        boolean retVal = n_isContinuous(nativeObj);

        return retVal;
    }

    //
    // C++: bool CvMat::isSubmatrix()
    //

    // javadoc: CvMat::isSubmatrix()
    public boolean isSubmatrix()
    {

        boolean retVal = n_isSubmatrix(nativeObj);

        return retVal;
    }

    //
    // C++: void CvMat::locateROI(Size wholeSize, Point ofs)
    //

    // javadoc: CvMat::locateROI(wholeSize, ofs)
    public void locateROI(Size wholeSize, Point ofs)
    {
        double[] wholeSize_out = new double[2];
        double[] ofs_out = new double[2];
        locateROI_0(nativeObj, wholeSize_out, ofs_out);
        if(wholeSize!=null){ wholeSize.width = wholeSize_out[0]; wholeSize.height = wholeSize_out[1]; }
        if(ofs!=null){ ofs.x = ofs_out[0]; ofs.y = ofs_out[1]; }
        return;
    }

    //
    // C++: CvMat CvMat::mul(CvMat m, double scale = 1)
    //

    // javadoc: CvMat::mul(m, scale)
    public CvMat mul(CvMat m, double scale)
    {

        CvMat retVal = new CvMat(n_mul(nativeObj, m.nativeObj, scale));

        return retVal;
    }

    // javadoc: CvMat::mul(m)
    public CvMat mul(CvMat m)
    {

        CvMat retVal = new CvMat(n_mul(nativeObj, m.nativeObj));

        return retVal;
    }

    //
    // C++: static CvMat CvMat::ones(int rows, int cols, int type)
    //

    // javadoc: CvMat::ones(rows, cols, type)
    public static CvMat ones(int rows, int cols, int type)
    {

        CvMat retVal = new CvMat(n_ones(rows, cols, type));

        return retVal;
    }

    //
    // C++: static CvMat CvMat::ones(Size size, int type)
    //

    // javadoc: CvMat::ones(size, type)
    public static CvMat ones(Size size, int type)
    {

        CvMat retVal = new CvMat(n_ones(size.width, size.height, type));

        return retVal;
    }

    //
    // C++: void CvMat::push_back(CvMat m)
    //

    // javadoc: CvMat::push_back(m)
    public void push_back(CvMat m)
    {

        n_push_back(nativeObj, m.nativeObj);

        return;
    }

    //
    // C++: void CvMat::release()
    //

    // javadoc: CvMat::release()
    public void release()
    {

        n_release(nativeObj);

        return;
    }

    //
    // C++: CvMat CvMat::reshape(int cn, int rows = 0)
    //

    // javadoc: CvMat::reshape(cn, rows)
    public CvMat reshape(int cn, int rows)
    {

        CvMat retVal = new CvMat(n_reshape(nativeObj, cn, rows));

        return retVal;
    }

    // javadoc: CvMat::reshape(cn)
    public CvMat reshape(int cn)
    {

        CvMat retVal = new CvMat(n_reshape(nativeObj, cn));

        return retVal;
    }

    //
    // C++: CvMat CvMat::row(int y)
    //

    // javadoc: CvMat::row(y)
    public CvMat row(int y)
    {

        CvMat retVal = new CvMat(n_row(nativeObj, y));

        return retVal;
    }

    //
    // C++: CvMat CvMat::rowRange(int startrow, int endrow)
    //

    // javadoc: CvMat::rowRange(startrow, endrow)
    public CvMat rowRange(int startrow, int endrow)
    {

        CvMat retVal = new CvMat(n_rowRange(nativeObj, startrow, endrow));

        return retVal;
    }

    //
    // C++: CvMat CvMat::rowRange(Range r)
    //

    // javadoc: CvMat::rowRange(r)
    public CvMat rowRange(Range r)
    {

        CvMat retVal = new CvMat(n_rowRange(nativeObj, r.start, r.end));

        return retVal;
    }

    //
    // C++: int CvMat::rows()
    //

    // javadoc: CvMat::rows()
    public int rows()
    {

        int retVal = n_rows(nativeObj);

        return retVal;
    }

    //
    // C++: CvMat CvMat::operator =(Scalar s)
    //

    // javadoc: CvMat::operator =(s)
    public CvMat setTo(Scalar s)
    {

        CvMat retVal = new CvMat(n_setTo(nativeObj, s.val[0], s.val[1], s.val[2], s.val[3]));

        return retVal;
    }

    //
    // C++: CvMat CvMat::setTo(Scalar value, CvMat mask = CvMat())
    //

    // javadoc: CvMat::setTo(value, mask)
    public CvMat setTo(Scalar value, CvMat mask)
    {

        CvMat retVal = new CvMat(n_setTo(nativeObj, value.val[0], value.val[1], value.val[2], value.val[3], mask.nativeObj));

        return retVal;
    }

    //
    // C++: CvMat CvMat::setTo(CvMat value, CvMat mask = CvMat())
    //

    // javadoc: CvMat::setTo(value, mask)
    public CvMat setTo(CvMat value, CvMat mask)
    {

        CvMat retVal = new CvMat(n_setTo(nativeObj, value.nativeObj, mask.nativeObj));

        return retVal;
    }

    // javadoc: CvMat::setTo(value)
    public CvMat setTo(CvMat value)
    {

        CvMat retVal = new CvMat(n_setTo(nativeObj, value.nativeObj));

        return retVal;
    }

    //
    // C++: Size CvMat::size()
    //

    // javadoc: CvMat::size()
    public Size size()
    {

        Size retVal = new Size(n_size(nativeObj));

        return retVal;
    }

    //
    // C++: size_t CvMat::step1(int i = 0)
    //

    // javadoc: CvMat::step1(i)
    public long step1(int i)
    {

        long retVal = n_step1(nativeObj, i);

        return retVal;
    }

    // javadoc: CvMat::step1()
    public long step1()
    {

        long retVal = n_step1(nativeObj);

        return retVal;
    }

    //
    // C++: CvMat CvMat::operator()(int rowStart, int rowEnd, int colStart, int
    // colEnd)
    //

    // javadoc: CvMat::operator()(rowStart, rowEnd, colStart, colEnd)
    public CvMat submat(int rowStart, int rowEnd, int colStart, int colEnd)
    {

        CvMat retVal = new CvMat(n_submat_rr(nativeObj, rowStart, rowEnd, colStart, colEnd));

        return retVal;
    }

    //
    // C++: CvMat CvMat::operator()(Range rowRange, Range colRange)
    //

    // javadoc: CvMat::operator()(rowRange, colRange)
    public CvMat submat(Range rowRange, Range colRange)
    {

        CvMat retVal = new CvMat(n_submat_rr(nativeObj, rowRange.start, rowRange.end, colRange.start, colRange.end));

        return retVal;
    }

    //
    // C++: CvMat CvMat::operator()(Rect roi)
    //

    // javadoc: CvMat::operator()(roi)
    public CvMat submat(Rect roi)
    {

        CvMat retVal = new CvMat(n_submat(nativeObj, roi.x, roi.y, roi.width, roi.height));

        return retVal;
    }

    //
    // C++: CvMat CvMat::t()
    //

    // javadoc: CvMat::t()
    public CvMat t()
    {

        CvMat retVal = new CvMat(n_t(nativeObj));

        return retVal;
    }

    //
    // C++: size_t CvMat::total()
    //

    // javadoc: CvMat::total()
    public long total()
    {

        long retVal = n_total(nativeObj);

        return retVal;
    }

    //
    // C++: int CvMat::type()
    //

    // javadoc: CvMat::type()
    public int type()
    {

        int retVal = n_type(nativeObj);

        return retVal;
    }

    //
    // C++: static CvMat CvMat::zeros(int rows, int cols, int type)
    //

    // javadoc: CvMat::zeros(rows, cols, type)
    public static CvMat zeros(int rows, int cols, int type)
    {

        CvMat retVal = new CvMat(n_zeros(rows, cols, type));

        return retVal;
    }

    //
    // C++: static CvMat CvMat::zeros(Size size, int type)
    //

    // javadoc: CvMat::zeros(size, type)
    public static CvMat zeros(Size size, int type)
    {

        CvMat retVal = new CvMat(n_zeros(size.width, size.height, type));

        return retVal;
    }

    @Override
    protected void finalize() throws Throwable {
        n_delete(nativeObj);
        super.finalize();
    }

    // javadoc:CvMat::toString()
    @Override
    public String toString() {
        return "CvMat [ " +
                rows() + "*" + cols() + "*" + CvType.typeToString(type()) +
                ", isCont=" + isContinuous() + ", isSubmat=" + isSubmatrix() +
                ", nativeObj=0x" + Long.toHexString(nativeObj) +
                ", dataAddr=0x" + Long.toHexString(dataAddr()) +
                " ]";
    }

    // javadoc:CvMat::dump()
    public String dump() {
        return nDump(nativeObj);
    }

    // javadoc:CvMat::put(row,col,data)
    public int put(int row, int col, double... data) {
        int t = type();
        if (data == null || data.length % CvType.channels(t) != 0)
            throw new java.lang.UnsupportedOperationException(
                    "Provided data element number (" +
                            (data == null ? 0 : data.length) +
                            ") should be multiple of the CvMat channels count (" +
                            CvType.channels(t) + ")");
        return nPutD(nativeObj, row, col, data.length, data);
    }

    // javadoc:CvMat::put(row,col,data)
    public int put(int row, int col, float[] data) {
        int t = type();
        if (data == null || data.length % CvType.channels(t) != 0)
            throw new java.lang.UnsupportedOperationException(
                    "Provided data element number (" +
                            (data == null ? 0 : data.length) +
                            ") should be multiple of the CvMat channels count (" +
                            CvType.channels(t) + ")");
        if (CvType.depth(t) == CvType.CC_32F) {
            return nPutF(nativeObj, row, col, data.length, data);
        }
        throw new java.lang.UnsupportedOperationException("CvMat data type is not compatible: " + t);
    }

    // javadoc:CvMat::put(row,col,data)
    public int put(int row, int col, int[] data) {
        int t = type();
        if (data == null || data.length % CvType.channels(t) != 0)
            throw new java.lang.UnsupportedOperationException(
                    "Provided data element number (" +
                            (data == null ? 0 : data.length) +
                            ") should be multiple of the CvMat channels count (" +
                            CvType.channels(t) + ")");
        if (CvType.depth(t) == CvType.CC_32S) {
            return nPutI(nativeObj, row, col, data.length, data);
        }
        throw new java.lang.UnsupportedOperationException("CvMat data type is not compatible: " + t);
    }

    // javadoc:CvMat::put(row,col,data)
    public int put(int row, int col, short[] data) {
        int t = type();
        if (data == null || data.length % CvType.channels(t) != 0)
            throw new java.lang.UnsupportedOperationException(
                    "Provided data element number (" +
                            (data == null ? 0 : data.length) +
                            ") should be multiple of the CvMat channels count (" +
                            CvType.channels(t) + ")");
        if (CvType.depth(t) == CvType.CC_16U || CvType.depth(t) == CvType.CC_16S) {
            return nPutS(nativeObj, row, col, data.length, data);
        }
        throw new java.lang.UnsupportedOperationException("CvMat data type is not compatible: " + t);
    }

    // javadoc:CvMat::put(row,col,data)
    public int put(int row, int col, byte[] data) {
        int t = type();
        if (data == null || data.length % CvType.channels(t) != 0)
            throw new java.lang.UnsupportedOperationException(
                    "Provided data element number (" +
                            (data == null ? 0 : data.length) +
                            ") should be multiple of the CvMat channels count (" +
                            CvType.channels(t) + ")");
        if (CvType.depth(t) == CvType.CC_8U || CvType.depth(t) == CvType.CC_8S) {
            return nPutB(nativeObj, row, col, data.length, data);
        }
        throw new java.lang.UnsupportedOperationException("CvMat data type is not compatible: " + t);
    }

    // javadoc:CvMat::get(row,col,data)
    public int get(int row, int col, byte[] data) {
        int t = type();
        if (data == null || data.length % CvType.channels(t) != 0)
            throw new java.lang.UnsupportedOperationException(
                    "Provided data element number (" +
                            (data == null ? 0 : data.length) +
                            ") should be multiple of the CvMat channels count (" +
                            CvType.channels(t) + ")");
        if (CvType.depth(t) == CvType.CC_8U || CvType.depth(t) == CvType.CC_8S) {
            return nGetB(nativeObj, row, col, data.length, data);
        }
        throw new java.lang.UnsupportedOperationException("CvMat data type is not compatible: " + t);
    }

    // javadoc:CvMat::get(row,col,data)
    public int get(int row, int col, short[] data) {
        int t = type();
        if (data == null || data.length % CvType.channels(t) != 0)
            throw new java.lang.UnsupportedOperationException(
                    "Provided data element number (" +
                            (data == null ? 0 : data.length) +
                            ") should be multiple of the CvMat channels count (" +
                            CvType.channels(t) + ")");
        if (CvType.depth(t) == CvType.CC_16U || CvType.depth(t) == CvType.CC_16S) {
            return nGetS(nativeObj, row, col, data.length, data);
        }
        throw new java.lang.UnsupportedOperationException("CvMat data type is not compatible: " + t);
    }

    // javadoc:CvMat::get(row,col,data)
    public int get(int row, int col, int[] data) {
        int t = type();
        if (data == null || data.length % CvType.channels(t) != 0)
            throw new java.lang.UnsupportedOperationException(
                    "Provided data element number (" +
                            (data == null ? 0 : data.length) +
                            ") should be multiple of the CvMat channels count (" +
                            CvType.channels(t) + ")");
        if (CvType.depth(t) == CvType.CC_32S) {
            return nGetI(nativeObj, row, col, data.length, data);
        }
        throw new java.lang.UnsupportedOperationException("CvMat data type is not compatible: " + t);
    }

    // javadoc:CvMat::get(row,col,data)
    public int get(int row, int col, float[] data) {
        int t = type();
        if (data == null || data.length % CvType.channels(t) != 0)
            throw new java.lang.UnsupportedOperationException(
                    "Provided data element number (" +
                            (data == null ? 0 : data.length) +
                            ") should be multiple of the CvMat channels count (" +
                            CvType.channels(t) + ")");
        if (CvType.depth(t) == CvType.CC_32F) {
            return nGetF(nativeObj, row, col, data.length, data);
        }
        throw new java.lang.UnsupportedOperationException("CvMat data type is not compatible: " + t);
    }

    // javadoc:CvMat::get(row,col,data)
    public int get(int row, int col, double[] data) {
        int t = type();
        if (data == null || data.length % CvType.channels(t) != 0)
            throw new java.lang.UnsupportedOperationException(
                    "Provided data element number (" +
                            (data == null ? 0 : data.length) +
                            ") should be multiple of the CvMat channels count (" +
                            CvType.channels(t) + ")");
        if (CvType.depth(t) == CvType.CC_64F) {
            return nGetD(nativeObj, row, col, data.length, data);
        }
        throw new java.lang.UnsupportedOperationException("CvMat data type is not compatible: " + t);
    }

    // javadoc:CvMat::get(row,col)
    public double[] get(int row, int col) {
        return nGet(nativeObj, row, col);
    }

    // javadoc:CvMat::height()
    public int height() {
        return rows();
    }

    // javadoc:CvMat::width()
    public int width() {
        return cols();
    }

    // javadoc:CvMat::getNativeObjAddr()
    public long getNativeObjAddr() {
        return nativeObj;
    }

    // C++: CvMat::CvMat()
    private static native long n_Mat();

    // C++: CvMat::CvMat(int rows, int cols, int type)
    private static native long n_Mat(int rows, int cols, int type);

    // C++: CvMat::CvMat(Size size, int type)
    private static native long n_Mat(double size_width, double size_height, int type);

    // C++: CvMat::CvMat(int rows, int cols, int type, Scalar s)
    private static native long n_Mat(int rows, int cols, int type, double s_val0, double s_val1, double s_val2, double s_val3);

    // C++: CvMat::CvMat(Size size, int type, Scalar s)
    private static native long n_Mat(double size_width, double size_height, int type, double s_val0, double s_val1, double s_val2, double s_val3);

    // C++: CvMat::CvMat(CvMat m, Range rowRange, Range colRange = Range::all())
    private static native long n_Mat(long m_nativeObj, int rowRange_start, int rowRange_end, int colRange_start, int colRange_end);

    private static native long n_Mat(long m_nativeObj, int rowRange_start, int rowRange_end);

    // C++: CvMat CvMat::adjustROI(int dtop, int dbottom, int dleft, int dright)
    private static native long n_adjustROI(long nativeObj, int dtop, int dbottom, int dleft, int dright);

    // C++: void CvMat::assignTo(CvMat m, int type = -1)
    private static native void n_assignTo(long nativeObj, long m_nativeObj, int type);

    private static native void n_assignTo(long nativeObj, long m_nativeObj);

    // C++: int CvMat::channels()
    private static native int n_channels(long nativeObj);

    // C++: int CvMat::checkVector(int elemChannels, int depth = -1, bool
    // requireContinuous = true)
    private static native int n_checkVector(long nativeObj, int elemChannels, int depth, boolean requireContinuous);

    private static native int n_checkVector(long nativeObj, int elemChannels, int depth);

    private static native int n_checkVector(long nativeObj, int elemChannels);

    // C++: CvMat CvMat::clone()
    private static native long n_clone(long nativeObj);

    // C++: CvMat CvMat::col(int x)
    private static native long n_col(long nativeObj, int x);

    // C++: CvMat CvMat::colRange(int startcol, int endcol)
    private static native long n_colRange(long nativeObj, int startcol, int endcol);

    // C++: int CvMat::dims()
    private static native int n_dims(long nativeObj);

    // C++: int CvMat::cols()
    private static native int n_cols(long nativeObj);

    // C++: void CvMat::convertTo(CvMat& m, int rtype, double alpha = 1, double beta
    // = 0)
    private static native void n_convertTo(long nativeObj, long m_nativeObj, int rtype, double alpha, double beta);

    private static native void n_convertTo(long nativeObj, long m_nativeObj, int rtype, double alpha);

    private static native void n_convertTo(long nativeObj, long m_nativeObj, int rtype);

    // C++: void CvMat::copyTo(CvMat& m)
    private static native void n_copyTo(long nativeObj, long m_nativeObj);

    // C++: void CvMat::copyTo(CvMat& m, CvMat mask)
    private static native void n_copyTo(long nativeObj, long m_nativeObj, long mask_nativeObj);

    // C++: void CvMat::create(int rows, int cols, int type)
    private static native void n_create(long nativeObj, int rows, int cols, int type);

    // C++: void CvMat::create(Size size, int type)
    private static native void n_create(long nativeObj, double size_width, double size_height, int type);

    // C++: CvMat CvMat::cross(CvMat m)
    private static native long n_cross(long nativeObj, long m_nativeObj);

    // C++: long CvMat::dataAddr()
    private static native long n_dataAddr(long nativeObj);

    // C++: int CvMat::depth()
    private static native int n_depth(long nativeObj);

    // C++: CvMat CvMat::diag(int d = 0)
    private static native long n_diag(long nativeObj, int d);

    // C++: static CvMat CvMat::diag(CvMat d)
    private static native long n_diag(long d_nativeObj);

    // C++: double CvMat::dot(CvMat m)
    private static native double n_dot(long nativeObj, long m_nativeObj);

    // C++: size_t CvMat::elemSize()
    private static native long n_elemSize(long nativeObj);

    // C++: size_t CvMat::elemSize1()
    private static native long n_elemSize1(long nativeObj);

    // C++: bool CvMat::empty()
    private static native boolean n_empty(long nativeObj);

    // C++: static CvMat CvMat::eye(int rows, int cols, int type)
    private static native long n_eye(int rows, int cols, int type);

    // C++: static CvMat CvMat::eye(Size size, int type)
    private static native long n_eye(double size_width, double size_height, int type);

    // C++: CvMat CvMat::inv(int method = DECOMP_LU)
    private static native long n_inv(long nativeObj, int method);

    private static native long n_inv(long nativeObj);

    // C++: bool CvMat::isContinuous()
    private static native boolean n_isContinuous(long nativeObj);

    // C++: bool CvMat::isSubmatrix()
    private static native boolean n_isSubmatrix(long nativeObj);

    // C++: void CvMat::locateROI(Size wholeSize, Point ofs)
    private static native void locateROI_0(long nativeObj, double[] wholeSize_out, double[] ofs_out);

    // C++: CvMat CvMat::mul(CvMat m, double scale = 1)
    private static native long n_mul(long nativeObj, long m_nativeObj, double scale);

    private static native long n_mul(long nativeObj, long m_nativeObj);

    // C++: static CvMat CvMat::ones(int rows, int cols, int type)
    private static native long n_ones(int rows, int cols, int type);

    // C++: static CvMat CvMat::ones(Size size, int type)
    private static native long n_ones(double size_width, double size_height, int type);

    // C++: void CvMat::push_back(CvMat m)
    private static native void n_push_back(long nativeObj, long m_nativeObj);

    // C++: void CvMat::release()
    private static native void n_release(long nativeObj);

    // C++: CvMat CvMat::reshape(int cn, int rows = 0)
    private static native long n_reshape(long nativeObj, int cn, int rows);

    private static native long n_reshape(long nativeObj, int cn);

    // C++: CvMat CvMat::row(int y)
    private static native long n_row(long nativeObj, int y);

    // C++: CvMat CvMat::rowRange(int startrow, int endrow)
    private static native long n_rowRange(long nativeObj, int startrow, int endrow);

    // C++: int CvMat::rows()
    private static native int n_rows(long nativeObj);

    // C++: CvMat CvMat::operator =(Scalar s)
    private static native long n_setTo(long nativeObj, double s_val0, double s_val1, double s_val2, double s_val3);

    // C++: CvMat CvMat::setTo(Scalar value, CvMat mask = CvMat())
    private static native long n_setTo(long nativeObj, double s_val0, double s_val1, double s_val2, double s_val3, long mask_nativeObj);

    // C++: CvMat CvMat::setTo(CvMat value, CvMat mask = CvMat())
    private static native long n_setTo(long nativeObj, long value_nativeObj, long mask_nativeObj);

    private static native long n_setTo(long nativeObj, long value_nativeObj);

    // C++: Size CvMat::size()
    private static native double[] n_size(long nativeObj);

    // C++: size_t CvMat::step1(int i = 0)
    private static native long n_step1(long nativeObj, int i);

    private static native long n_step1(long nativeObj);

    // C++: CvMat CvMat::operator()(Range rowRange, Range colRange)
    private static native long n_submat_rr(long nativeObj, int rowRange_start, int rowRange_end, int colRange_start, int colRange_end);

    // C++: CvMat CvMat::operator()(Rect roi)
    private static native long n_submat(long nativeObj, int roi_x, int roi_y, int roi_width, int roi_height);

    // C++: CvMat CvMat::t()
    private static native long n_t(long nativeObj);

    // C++: size_t CvMat::total()
    private static native long n_total(long nativeObj);

    // C++: int CvMat::type()
    private static native int n_type(long nativeObj);

    // C++: static CvMat CvMat::zeros(int rows, int cols, int type)
    private static native long n_zeros(int rows, int cols, int type);

    // C++: static CvMat CvMat::zeros(Size size, int type)
    private static native long n_zeros(double size_width, double size_height, int type);

    // native support for java finalize()
    private static native void n_delete(long nativeObj);

    private static native int nPutD(long self, int row, int col, int count, double[] data);

    private static native int nPutF(long self, int row, int col, int count, float[] data);

    private static native int nPutI(long self, int row, int col, int count, int[] data);

    private static native int nPutS(long self, int row, int col, int count, short[] data);

    private static native int nPutB(long self, int row, int col, int count, byte[] data);

    private static native int nGetB(long self, int row, int col, int count, byte[] vals);

    private static native int nGetS(long self, int row, int col, int count, short[] vals);

    private static native int nGetI(long self, int row, int col, int count, int[] vals);

    private static native int nGetF(long self, int row, int col, int count, float[] vals);

    private static native int nGetD(long self, int row, int col, int count, double[] vals);

    private static native double[] nGet(long self, int row, int col);

    private static native String nDump(long self);
}
