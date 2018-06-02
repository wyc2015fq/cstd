



#ifdef HAVE_OPENGL
#  include "gl_core_3_1.hpp"
#  ifdef HAVE_CUDA
#    include <cuda_gl_interop.h>
#  endif
#else // HAVE_OPENGL
#  define NO_OPENGL_SUPPORT_ERROR CC_ErrorNoReturn(CC_StsBadFunc, "OpenCV was build without OpenGL support")
#endif // HAVE_OPENGL

using namespace cv;
using namespace cuda;

namespace
{
    #ifndef HAVE_OPENGL
        inline void throw_no_ogl() { CC_Error(CC_OpenGlNotSupported, "The library is compiled without OpenGL support"); }
    #else
        inline void throw_no_ogl() { CC_Error(CC_OpenGlApiCallError, "OpenGL context doesn't exist"); }

    bool checkError(const char* file, const int line, const char* func = 0)
    {
        GLenum err = gl::GetError();

        if (err != gl::NO_ERROR_)
        {
            const char* msg;

            switch (err)
            {
            case gl::INVALID_ENUM:
                msg = "An unacceptable value is specified for an enumerated argument";
                break;

            case gl::INVALID_VALUE:
                msg = "A numeric argument is out of range";
                break;

            case gl::INVALID_OPERATION:
                msg = "The specified operation is not allowed in the current state";
                break;

            case gl::OUT_OF_MEMORY:
                msg = "There is not enough memory left to execute the command";
                break;

            default:
                msg = "Unknown error";
            };

            cvError(CC_OpenGlApiCallError, func, msg, file, line);

            return false;
        }

        return true;
    }
    #endif

    #define CC_CheckGlError() CC_DbgAssert( (checkError(__FILE__, __LINE__, CC_Func)) )
} // namespace

#ifdef HAVE_OPENGL
namespace
{
    const GLenum gl_types[] = { gl::UNSIGNED_BYTE, gl::BYTE, gl::UNSIGNED_SHORT, gl::SHORT, gl::INT, gl::FLOAT, gl::DOUBLE };
}
#endif

////////////////////////////////////////////////////////////////////////
// setGlDevice

void cuda::setGlDevice(int device)
{
#ifndef HAVE_OPENGL
    (void) device;
    throw_no_ogl();
#else
    #ifndef HAVE_CUDA
        (void) device;
        throw_no_cuda();
    #else
        cudaSafeCall( cudaGLSetGLDevice(device) );
    #endif
#endif
}

////////////////////////////////////////////////////////////////////////
// CudaResource

#if defined(HAVE_OPENGL) && defined(HAVE_CUDA)

namespace
{
    class CudaResource
    {
    public:
        CudaResource();
        ~CudaResource();

        void registerBuffer(GLuint buffer);
        void release();

        void copyFrom(const void* src, size_t spitch, size_t width, size_t height, cudaStream_t stream = 0);
        void copyTo(void* dst, size_t dpitch, size_t width, size_t height, cudaStream_t stream = 0);

        void* map(cudaStream_t stream = 0);
        void unmap(cudaStream_t stream = 0);

    private:
        cudaGraphicsResource_t resource_;
        GLuint buffer_;

        class GraphicsMapHolder;
    };

    CudaResource::CudaResource() : resource_(0), buffer_(0)
    {
    }

    CudaResource::~CudaResource()
    {
        release();
    }

    void CudaResource::registerBuffer(GLuint buffer)
    {
        CC_DbgAssert( buffer != 0 );

        if (buffer_ == buffer)
            return;

        cudaGraphicsResource_t resource;
        cudaSafeCall( cudaGraphicsGLRegisterBuffer(&resource, buffer, cudaGraphicsMapFlagsNone) );

        release();

        resource_ = resource;
        buffer_ = buffer;
    }

    void CudaResource::release()
    {
        if (resource_)
            cudaGraphicsUnregisterResource(resource_);

        resource_ = 0;
        buffer_ = 0;
    }

    class CudaResource::GraphicsMapHolder
    {
    public:
        GraphicsMapHolder(cudaGraphicsResource_t* resource, cudaStream_t stream);
        ~GraphicsMapHolder();

        void reset();

    private:
        cudaGraphicsResource_t* resource_;
        cudaStream_t stream_;
    };

    CudaResource::GraphicsMapHolder::GraphicsMapHolder(cudaGraphicsResource_t* resource, cudaStream_t stream) : resource_(resource), stream_(stream)
    {
        if (resource_)
            cudaSafeCall( cudaGraphicsMapResources(1, resource_, stream_) );
    }

    CudaResource::GraphicsMapHolder::~GraphicsMapHolder()
    {
        if (resource_)
            cudaGraphicsUnmapResources(1, resource_, stream_);
    }

    void CudaResource::GraphicsMapHolder::reset()
    {
        resource_ = 0;
    }

    void CudaResource::copyFrom(const void* src, size_t spitch, size_t width, size_t height, cudaStream_t stream)
    {
        CC_DbgAssert( resource_ != 0 );

        GraphicsMapHolder h(&resource_, stream);
        (void) h;

        void* dst;
        size_t size;
        cudaSafeCall( cudaGraphicsResourceGetMappedPointer(&dst, &size, resource_) );

        CC_DbgAssert( width * height == size );

        if (stream == 0)
            cudaSafeCall( cudaMemcpy2D(dst, width, src, spitch, width, height, cudaMemcpyDeviceToDevice) );
        else
            cudaSafeCall( cudaMemcpy2DAsync(dst, width, src, spitch, width, height, cudaMemcpyDeviceToDevice, stream) );
    }

    void CudaResource::copyTo(void* dst, size_t dpitch, size_t width, size_t height, cudaStream_t stream)
    {
        CC_DbgAssert( resource_ != 0 );

        GraphicsMapHolder h(&resource_, stream);
        (void) h;

        void* src;
        size_t size;
        cudaSafeCall( cudaGraphicsResourceGetMappedPointer(&src, &size, resource_) );

        CC_DbgAssert( width * height == size );

        if (stream == 0)
            cudaSafeCall( cudaMemcpy2D(dst, dpitch, src, width, width, height, cudaMemcpyDeviceToDevice) );
        else
            cudaSafeCall( cudaMemcpy2DAsync(dst, dpitch, src, width, width, height, cudaMemcpyDeviceToDevice, stream) );
    }

    void* CudaResource::map(cudaStream_t stream)
    {
        CC_DbgAssert( resource_ != 0 );

        GraphicsMapHolder h(&resource_, stream);

        void* ptr;
        size_t size;
        cudaSafeCall( cudaGraphicsResourceGetMappedPointer(&ptr, &size, resource_) );

        h.reset();

        return ptr;
    }

    void CudaResource::unmap(cudaStream_t stream)
    {
        CC_Assert( resource_ != 0 );

        cudaGraphicsUnmapResources(1, &resource_, stream);
    }
}

#endif

////////////////////////////////////////////////////////////////////////
// ogl::Buffer

#ifndef HAVE_OPENGL

class ogl::Buffer::Impl
{
};

#else

class ogl::Buffer::Impl
{
public:
    static const Ptr<Impl>& empty();

    Impl(GLuint bufId, bool autoRelease);
    Impl(GLsizeiptr size, const GLvoid* data, GLenum target, bool autoRelease);
    ~Impl();

    void bind(GLenum target) const;

    void copyFrom(GLuint srcBuf, GLsizeiptr size);

    void copyFrom(GLsizeiptr size, const GLvoid* data);
    void copyTo(GLsizeiptr size, GLvoid* data) const;

    void* mapHost(GLenum access);
    void unmapHost();

#ifdef HAVE_CUDA
    void copyFrom(const void* src, size_t spitch, size_t width, size_t height, cudaStream_t stream = 0);
    void copyTo(void* dst, size_t dpitch, size_t width, size_t height, cudaStream_t stream = 0) const;

    void* mapDevice(cudaStream_t stream = 0);
    void unmapDevice(cudaStream_t stream = 0);
#endif

    void setAutoRelease(bool flag) { autoRelease_ = flag; }

    GLuint bufId() const { return bufId_; }

private:
    Impl();

    GLuint bufId_;
    bool autoRelease_;

#ifdef HAVE_CUDA
    mutable CudaResource cudaResource_;
#endif
};

const Ptr<ogl::Buffer::Impl>& ogl::Buffer::Impl::empty()
{
    static Ptr<Impl> p(new Impl);
    return p;
}

ogl::Buffer::Impl::Impl() : bufId_(0), autoRelease_(false)
{
}

ogl::Buffer::Impl::Impl(GLuint abufId, bool autoRelease) : bufId_(abufId), autoRelease_(autoRelease)
{
    CC_Assert( gl::IsBuffer(abufId) == gl::TRUE_ );
}

ogl::Buffer::Impl::Impl(GLsizeiptr size, const GLvoid* data, GLenum target, bool autoRelease) : bufId_(0), autoRelease_(autoRelease)
{
    gl::GenBuffers(1, &bufId_);
    CC_CheckGlError();

    CC_Assert( bufId_ != 0 );

    gl::BindBuffer(target, bufId_);
    CC_CheckGlError();

    gl::BufferData(target, size, data, gl::DYNAMIC_DRAW);
    CC_CheckGlError();

    gl::BindBuffer(target, 0);
    CC_CheckGlError();
}

ogl::Buffer::Impl::~Impl()
{
    if (autoRelease_ && bufId_)
        gl::DeleteBuffers(1, &bufId_);
}

void ogl::Buffer::Impl::bind(GLenum target) const
{
    gl::BindBuffer(target, bufId_);
    CC_CheckGlError();
}

void ogl::Buffer::Impl::copyFrom(GLuint srcBuf, GLsizeiptr size)
{
    gl::BindBuffer(gl::COPY_WRITE_BUFFER, bufId_);
    CC_CheckGlError();

    gl::BindBuffer(gl::COPY_READ_BUFFER, srcBuf);
    CC_CheckGlError();

    gl::CopyBufferSubData(gl::COPY_READ_BUFFER, gl::COPY_WRITE_BUFFER, 0, 0, size);
    CC_CheckGlError();
}

void ogl::Buffer::Impl::copyFrom(GLsizeiptr size, const GLvoid* data)
{
    gl::BindBuffer(gl::COPY_WRITE_BUFFER, bufId_);
    CC_CheckGlError();

    gl::BufferSubData(gl::COPY_WRITE_BUFFER, 0, size, data);
    CC_CheckGlError();
}

void ogl::Buffer::Impl::copyTo(GLsizeiptr size, GLvoid* data) const
{
    gl::BindBuffer(gl::COPY_READ_BUFFER, bufId_);
    CC_CheckGlError();

    gl::GetBufferSubData(gl::COPY_READ_BUFFER, 0, size, data);
    CC_CheckGlError();
}

void* ogl::Buffer::Impl::mapHost(GLenum access)
{
    gl::BindBuffer(gl::COPY_READ_BUFFER, bufId_);
    CC_CheckGlError();

    GLvoid* data = gl::MapBuffer(gl::COPY_READ_BUFFER, access);
    CC_CheckGlError();

    return data;
}

void ogl::Buffer::Impl::unmapHost()
{
    gl::UnmapBuffer(gl::COPY_READ_BUFFER);
}

#ifdef HAVE_CUDA

void ogl::Buffer::Impl::copyFrom(const void* src, size_t spitch, size_t width, size_t height, cudaStream_t stream)
{
    cudaResource_.registerBuffer(bufId_);
    cudaResource_.copyFrom(src, spitch, width, height, stream);
}

void ogl::Buffer::Impl::copyTo(void* dst, size_t dpitch, size_t width, size_t height, cudaStream_t stream) const
{
    cudaResource_.registerBuffer(bufId_);
    cudaResource_.copyTo(dst, dpitch, width, height, stream);
}

void* ogl::Buffer::Impl::mapDevice(cudaStream_t stream)
{
    cudaResource_.registerBuffer(bufId_);
    return cudaResource_.map(stream);
}

void ogl::Buffer::Impl::unmapDevice(cudaStream_t stream)
{
    cudaResource_.unmap(stream);
}

#endif // HAVE_CUDA

#endif // HAVE_OPENGL

ogl::Buffer::Buffer() : rows_(0), cols_(0), type_(0)
{
#ifndef HAVE_OPENGL
    throw_no_ogl();
#else
    impl_ = Impl::empty();
#endif
}

ogl::Buffer::Buffer(int arows, int acols, int atype, unsigned int abufId, bool autoRelease) : rows_(0), cols_(0), type_(0)
{
#ifndef HAVE_OPENGL
    (void) arows;
    (void) acols;
    (void) atype;
    (void) abufId;
    (void) autoRelease;
    throw_no_ogl();
#else
    impl_.reset(new Impl(abufId, autoRelease));
    rows_ = arows;
    cols_ = acols;
    type_ = atype;
#endif
}

ogl::Buffer::Buffer(Size asize, int atype, unsigned int abufId, bool autoRelease) : rows_(0), cols_(0), type_(0)
{
#ifndef HAVE_OPENGL
    (void) asize;
    (void) atype;
    (void) abufId;
    (void) autoRelease;
    throw_no_ogl();
#else
    impl_.reset(new Impl(abufId, autoRelease));
    rows_ = asize.height;
    cols_ = asize.width;
    type_ = atype;
#endif
}

ogl::Buffer::Buffer(const img_t* arr, Target target, bool autoRelease) : rows_(0), cols_(0), type_(0)
{
#ifndef HAVE_OPENGL
    (void) arr;
    (void) target;
    (void) autoRelease;
    throw_no_ogl();
#else
    const int kind = arr.kind();

    switch (kind)
    {
    case _InputArray::OPENGL_BUFFER:
    case _InputArray::CUDA_GPU_MAT:
        copyFrom(arr, target, autoRelease);
        break;

    default:
        {
            img_t mat = arr;
            CC_Assert( mat CC_IS_CONT_MAT() );
            const GLsizeiptr asize = mat->rows * mat->cols * mat CC_ELEM_SIZE();
            impl_.reset(new Impl(asize, mat.data, target, autoRelease));
            rows_ = mat->rows;
            cols_ = mat->cols;
            type_ = mat CC_MAT_TYPE();
            break;
        }
    }
#endif
}

void ogl::Buffer::create(int arows, int acols, int atype, Target target, bool autoRelease)
{
#ifndef HAVE_OPENGL
    (void) arows;
    (void) acols;
    (void) atype;
    (void) target;
    (void) autoRelease;
    throw_no_ogl();
#else
    if (rows_ != arows || cols_ != acols || type_ != atype)
    {
        const GLsizeiptr asize = arows * acols * CC_ELEM_SIZE(atype);
        impl_.reset(new Impl(asize, 0, target, autoRelease));
        rows_ = arows;
        cols_ = acols;
        type_ = atype;
    }
#endif
}

void ogl::Buffer::release()
{
#ifdef HAVE_OPENGL
    if (impl_)
        impl_->setAutoRelease(true);
    impl_ = Impl::empty();
    rows_ = 0;
    cols_ = 0;
    type_ = 0;
#endif
}

void ogl::Buffer::setAutoRelease(bool flag)
{
#ifndef HAVE_OPENGL
    (void) flag;
    throw_no_ogl();
#else
    impl_->setAutoRelease(flag);
#endif
}

void ogl::Buffer::copyFrom(const img_t* arr, Target target, bool autoRelease)
{
#ifndef HAVE_OPENGL
    (void) arr;
    (void) target;
    (void) autoRelease;
    throw_no_ogl();
#else
    const int kind = arr.kind();

    const Size asize = arr.size();
    const int atype = arr CC_MAT_TYPE();
    create(asize, atype, target, autoRelease);

    switch (kind)
    {
    case _InputArray::OPENGL_BUFFER:
        {
            ogl::Buffer buf = arr.getOGlBuffer();
            impl_->copyFrom(buf.bufId(), asize.area() * CC_ELEM_SIZE(atype));
            break;
        }

    case _InputArray::CUDA_GPU_MAT:
        {
            #ifndef HAVE_CUDA
                throw_no_cuda();
            #else
                GpuMat dmat = arr.getGpuMat();
                impl_->copyFrom(dmat.data, dmat->step, dmat->cols * dmat CC_ELEM_SIZE(), dmat->rows);
            #endif

            break;
        }

    default:
        {
            img_t mat = arr;
            CC_Assert( mat CC_IS_CONT_MAT() );
            impl_->copyFrom(asize.area() * CC_ELEM_SIZE(atype), mat.data);
        }
    }
#endif
}

void ogl::Buffer::copyFrom(const img_t* arr, cuda::Stream& stream, Target target, bool autoRelease)
{
#ifndef HAVE_OPENGL
    (void) arr;
    (void) stream;
    (void) target;
    (void) autoRelease;
    throw_no_ogl();
#else
    #ifndef HAVE_CUDA
        (void) arr;
        (void) stream;
        (void) target;
        (void) autoRelease;
        throw_no_cuda();
    #else
        GpuMat dmat = arr.getGpuMat();

        create(dmat.size(), dmat CC_MAT_TYPE(), target, autoRelease);

        impl_->copyFrom(dmat.data, dmat->step, dmat->cols * dmat CC_ELEM_SIZE(), dmat->rows, cuda::StreamAccessor::getStream(stream));
    #endif
#endif
}

void ogl::Buffer::copyTo(img_t* arr) const
{
#ifndef HAVE_OPENGL
    (void) arr;
    throw_no_ogl();
#else
    const int kind = arr.kind();

    switch (kind)
    {
    case _InputArray::OPENGL_BUFFER:
        {
            arr.getOGlBufferRef().copyFrom(*this);
            break;
        }

    case _InputArray::CUDA_GPU_MAT:
        {
            #ifndef HAVE_CUDA
                throw_no_cuda();
            #else
                GpuMat& dmat = arr.getGpuMatRef();
                dmat cvSetMat(rows_, cols_, type_);
                impl_->copyTo(dmat.data, dmat->step, dmat->cols * dmat CC_ELEM_SIZE(), dmat->rows);
            #endif

            break;
        }

    default:
        {
            arr cvSetMat(rows_, cols_, type_);
            img_t mat = arr;
            CC_Assert( mat CC_IS_CONT_MAT() );
            impl_->copyTo(mat->rows * mat->cols * mat CC_ELEM_SIZE(), mat.data);
        }
    }
#endif
}

void ogl::Buffer::copyTo(img_t* arr, cuda::Stream& stream) const
{
#ifndef HAVE_OPENGL
    (void) arr;
    (void) stream;
    throw_no_ogl();
#else
    #ifndef HAVE_CUDA
        (void) arr;
        (void) stream;
        throw_no_cuda();
    #else
        arr cvSetMat(rows_, cols_, type_);
        GpuMat dmat = arr.getGpuMat();
        impl_->copyTo(dmat.data, dmat->step, dmat->cols * dmat CC_ELEM_SIZE(), dmat->rows, cuda::StreamAccessor::getStream(stream));
    #endif
#endif
}

ogl::Buffer ogl::Buffer::clone(Target target, bool autoRelease) const
{
#ifndef HAVE_OPENGL
    (void) target;
    (void) autoRelease;
    throw_no_ogl();
    return ogl::Buffer();
#else
    ogl::Buffer buf;
    buf.copyFrom(*this, target, autoRelease);
    return buf;
#endif
}

void ogl::Buffer::bind(Target target) const
{
#ifndef HAVE_OPENGL
    (void) target;
    throw_no_ogl();
#else
    impl_->bind(target);
#endif
}

void ogl::Buffer::unbind(Target target)
{
#ifndef HAVE_OPENGL
    (void) target;
    throw_no_ogl();
#else
    gl::BindBuffer(target, 0);
    CC_CheckGlError();
#endif
}

img_t ogl::Buffer::mapHost(Access access)
{
#ifndef HAVE_OPENGL
    (void) access;
    throw_no_ogl();
    return img_t();
#else
    return img_t(rows_, cols_, type_, impl_->mapHost(access));
#endif
}

void ogl::Buffer::unmapHost()
{
#ifndef HAVE_OPENGL
    throw_no_ogl();
#else
    return impl_->unmapHost();
#endif
}

GpuMat ogl::Buffer::mapDevice()
{
#ifndef HAVE_OPENGL
    throw_no_ogl();
    return GpuMat();
#else
    #ifndef HAVE_CUDA
        throw_no_cuda();
        return GpuMat();
    #else
        return GpuMat(rows_, cols_, type_, impl_->mapDevice());
    #endif
#endif
}

void ogl::Buffer::unmapDevice()
{
#ifndef HAVE_OPENGL
    throw_no_ogl();
#else
    #ifndef HAVE_CUDA
        throw_no_cuda();
    #else
        impl_->unmapDevice();
    #endif
#endif
}

cuda::GpuMat ogl::Buffer::mapDevice(cuda::Stream& stream)
{
#ifndef HAVE_OPENGL
    (void) stream;
    throw_no_ogl();
    return GpuMat();
#else
    #ifndef HAVE_CUDA
        (void) stream;
        throw_no_cuda();
        return GpuMat();
    #else
        return GpuMat(rows_, cols_, type_, impl_->mapDevice(cuda::StreamAccessor::getStream(stream)));
    #endif
#endif
}

void ogl::Buffer::unmapDevice(cuda::Stream& stream)
{
#ifndef HAVE_OPENGL
    (void) stream;
    throw_no_ogl();
#else
    #ifndef HAVE_CUDA
        (void) stream;
        throw_no_cuda();
    #else
        impl_->unmapDevice(cuda::StreamAccessor::getStream(stream));
    #endif
#endif
}

unsigned int ogl::Buffer::bufId() const
{
#ifndef HAVE_OPENGL
    throw_no_ogl();
    return 0;
#else
    return impl_->bufId();
#endif
}


//////////////////////////////////////////////////////////////////////////////////////////
// ogl::Texture

#ifndef HAVE_OPENGL

class ogl::Texture2D::Impl
{
};

#else

class ogl::Texture2D::Impl
{
public:
    static const Ptr<Impl> empty();

    Impl(GLuint texId, bool autoRelease);
    Impl(GLint internalFormat, GLsizei width, GLsizei height, GLenum format, GLenum type, const GLvoid* pixels, bool autoRelease);
    ~Impl();

    void copyFrom(GLsizei width, GLsizei height, GLenum format, GLenum type, const GLvoid *pixels);
    void copyTo(GLenum format, GLenum type, GLvoid* pixels) const;

    void bind() const;

    void setAutoRelease(bool flag) { autoRelease_ = flag; }

    GLuint texId() const { return texId_; }

private:
    Impl();

    GLuint texId_;
    bool autoRelease_;
};

const Ptr<ogl::Texture2D::Impl> ogl::Texture2D::Impl::empty()
{
    static Ptr<Impl> p(new Impl);
    return p;
}

ogl::Texture2D::Impl::Impl() : texId_(0), autoRelease_(false)
{
}

ogl::Texture2D::Impl::Impl(GLuint atexId, bool autoRelease) : texId_(atexId), autoRelease_(autoRelease)
{
    CC_Assert( gl::IsTexture(atexId) == gl::TRUE_ );
}

ogl::Texture2D::Impl::Impl(GLint internalFormat, GLsizei width, GLsizei height, GLenum format, GLenum type, const GLvoid* pixels, bool autoRelease) : texId_(0), autoRelease_(autoRelease)
{
    gl::GenTextures(1, &texId_);
    CC_CheckGlError();

    CC_Assert(texId_ != 0);

    gl::BindTexture(gl::TEXTURE_2D, texId_);
    CC_CheckGlError();

    gl::PixelStorei(gl::UNPACK_ALIGNMENT, 1);
    CC_CheckGlError();

    gl::TexImage2D(gl::TEXTURE_2D, 0, internalFormat, width, height, 0, format, type, pixels);
    CC_CheckGlError();

    gl::GenerateMipmap(gl::TEXTURE_2D);
    CC_CheckGlError();
}

ogl::Texture2D::Impl::~Impl()
{
    if (autoRelease_ && texId_)
        gl::DeleteTextures(1, &texId_);
}

void ogl::Texture2D::Impl::copyFrom(GLsizei width, GLsizei height, GLenum format, GLenum type, const GLvoid *pixels)
{
    gl::BindTexture(gl::TEXTURE_2D, texId_);
    CC_CheckGlError();

    gl::PixelStorei(gl::UNPACK_ALIGNMENT, 1);
    CC_CheckGlError();

    gl::TexSubImage2D(gl::TEXTURE_2D, 0, 0, 0, width, height, format, type, pixels);
    CC_CheckGlError();

    gl::GenerateMipmap(gl::TEXTURE_2D);
    CC_CheckGlError();
}

void ogl::Texture2D::Impl::copyTo(GLenum format, GLenum type, GLvoid* pixels) const
{
    gl::BindTexture(gl::TEXTURE_2D, texId_);
    CC_CheckGlError();

    gl::PixelStorei(gl::PACK_ALIGNMENT, 1);
    CC_CheckGlError();

    gl::GetTexImage(gl::TEXTURE_2D, 0, format, type, pixels);
    CC_CheckGlError();
}

void ogl::Texture2D::Impl::bind() const
{
    gl::BindTexture(gl::TEXTURE_2D, texId_);
    CC_CheckGlError();
}

#endif // HAVE_OPENGL

ogl::Texture2D::Texture2D() : rows_(0), cols_(0), format_(NONE)
{
#ifndef HAVE_OPENGL
    throw_no_ogl();
#else
    impl_ = Impl::empty();
#endif
}

ogl::Texture2D::Texture2D(int arows, int acols, Format aformat, unsigned int atexId, bool autoRelease) : rows_(0), cols_(0), format_(NONE)
{
#ifndef HAVE_OPENGL
    (void) arows;
    (void) acols;
    (void) aformat;
    (void) atexId;
    (void) autoRelease;
    throw_no_ogl();
#else
    impl_.reset(new Impl(atexId, autoRelease));
    rows_ = arows;
    cols_ = acols;
    format_ = aformat;
#endif
}

ogl::Texture2D::Texture2D(Size asize, Format aformat, unsigned int atexId, bool autoRelease) : rows_(0), cols_(0), format_(NONE)
{
#ifndef HAVE_OPENGL
    (void) asize;
    (void) aformat;
    (void) atexId;
    (void) autoRelease;
    throw_no_ogl();
#else
    impl_.reset(new Impl(atexId, autoRelease));
    rows_ = asize.height;
    cols_ = asize.width;
    format_ = aformat;
#endif
}

ogl::Texture2D::Texture2D(const img_t* arr, bool autoRelease) : rows_(0), cols_(0), format_(NONE)
{
#ifndef HAVE_OPENGL
    (void) arr;
    (void) autoRelease;
    throw_no_ogl();
#else
    const int kind = arr.kind();

    const Size asize = arr.size();
    const int atype = arr CC_MAT_TYPE();

    const int depth = CC_MAT_DEPTH(atype);
    const int cn = CC_MAT_CN(atype);

    CC_Assert( depth <= CC_32F );
    CC_Assert( cn == 1 || cn == 3 || cn == 4 );

    const Format internalFormats[] =
    {
        NONE, DEPTH_COMPONENT, NONE, RGB, RGBA
    };
    const GLenum srcFormats[] =
    {
        0, gl::DEPTH_COMPONENT, 0, gl::BGR, gl::BGRA
    };

    switch (kind)
    {
    case _InputArray::OPENGL_BUFFER:
        {
            ogl::Buffer buf = arr.getOGlBuffer();
            buf.bind(ogl::Buffer::PIXEL_UNPACK_BUFFER);
            impl_.reset(new Impl(internalFormats[cn], asize.width, asize.height, srcFormats[cn], gl_types[depth], 0, autoRelease));
            ogl::Buffer::unbind(ogl::Buffer::PIXEL_UNPACK_BUFFER);
            break;
        }

    case _InputArray::CUDA_GPU_MAT:
        {
            #ifndef HAVE_CUDA
                throw_no_cuda();
            #else
                GpuMat dmat = arr.getGpuMat();
                ogl::Buffer buf(dmat, ogl::Buffer::PIXEL_UNPACK_BUFFER);
                buf.setAutoRelease(true);
                buf.bind(ogl::Buffer::PIXEL_UNPACK_BUFFER);
                impl_.reset(new Impl(internalFormats[cn], asize.width, asize.height, srcFormats[cn], gl_types[depth], 0, autoRelease));
                ogl::Buffer::unbind(ogl::Buffer::PIXEL_UNPACK_BUFFER);
            #endif

            break;
        }

    default:
        {
            img_t mat = arr;
            CC_Assert( mat CC_IS_CONT_MAT() );
            ogl::Buffer::unbind(ogl::Buffer::PIXEL_UNPACK_BUFFER);
            impl_.reset(new Impl(internalFormats[cn], asize.width, asize.height, srcFormats[cn], gl_types[depth], mat.data, autoRelease));
            break;
        }
    }

    rows_ = asize.height;
    cols_ = asize.width;
    format_ = internalFormats[cn];
#endif
}

void ogl::Texture2D::create(int arows, int acols, Format aformat, bool autoRelease)
{
#ifndef HAVE_OPENGL
    (void) arows;
    (void) acols;
    (void) aformat;
    (void) autoRelease;
    throw_no_ogl();
#else
    if (rows_ != arows || cols_ != acols || format_ != aformat)
    {
        ogl::Buffer::unbind(ogl::Buffer::PIXEL_UNPACK_BUFFER);
        impl_.reset(new Impl(aformat, acols, arows, aformat, gl::FLOAT, 0, autoRelease));
        rows_ = arows;
        cols_ = acols;
        format_ = aformat;
    }
#endif
}

void ogl::Texture2D::release()
{
#ifdef HAVE_OPENGL
    if (impl_)
        impl_->setAutoRelease(true);
    impl_ = Impl::empty();
    rows_ = 0;
    cols_ = 0;
    format_ = NONE;
#endif
}

void ogl::Texture2D::setAutoRelease(bool flag)
{
#ifndef HAVE_OPENGL
    (void) flag;
    throw_no_ogl();
#else
    impl_->setAutoRelease(flag);
#endif
}

void ogl::Texture2D::copyFrom(const img_t* arr, bool autoRelease)
{
#ifndef HAVE_OPENGL
    (void) arr;
    (void) autoRelease;
    throw_no_ogl();
#else
    const int kind = arr.kind();

    const Size asize = arr.size();
    const int atype = arr CC_MAT_TYPE();

    const int depth = CC_MAT_DEPTH(atype);
    const int cn = CC_MAT_CN(atype);

    CC_Assert( depth <= CC_32F );
    CC_Assert( cn == 1 || cn == 3 || cn == 4 );

    const Format internalFormats[] =
    {
        NONE, DEPTH_COMPONENT, NONE, RGB, RGBA
    };
    const GLenum srcFormats[] =
    {
        0, gl::DEPTH_COMPONENT, 0, gl::BGR, gl::BGRA
    };

    create(asize, internalFormats[cn], autoRelease);

    switch(kind)
    {
    case _InputArray::OPENGL_BUFFER:
        {
            ogl::Buffer buf = arr.getOGlBuffer();
            buf.bind(ogl::Buffer::PIXEL_UNPACK_BUFFER);
            impl_->copyFrom(asize.width, asize.height, srcFormats[cn], gl_types[depth], 0);
            ogl::Buffer::unbind(ogl::Buffer::PIXEL_UNPACK_BUFFER);
            break;
        }

    case _InputArray::CUDA_GPU_MAT:
        {
            #ifndef HAVE_CUDA
                throw_no_cuda();
            #else
                GpuMat dmat = arr.getGpuMat();
                ogl::Buffer buf(dmat, ogl::Buffer::PIXEL_UNPACK_BUFFER);
                buf.setAutoRelease(true);
                buf.bind(ogl::Buffer::PIXEL_UNPACK_BUFFER);
                impl_->copyFrom(asize.width, asize.height, srcFormats[cn], gl_types[depth], 0);
                ogl::Buffer::unbind(ogl::Buffer::PIXEL_UNPACK_BUFFER);
            #endif

            break;
        }

    default:
        {
            img_t mat = arr;
            CC_Assert( mat CC_IS_CONT_MAT() );
            ogl::Buffer::unbind(ogl::Buffer::PIXEL_UNPACK_BUFFER);
            impl_->copyFrom(asize.width, asize.height, srcFormats[cn], gl_types[depth], mat.data);
        }
    }
#endif
}

void ogl::Texture2D::copyTo(img_t* arr, int ddepth, bool autoRelease) const
{
#ifndef HAVE_OPENGL
    (void) arr;
    (void) ddepth;
    (void) autoRelease;
    throw_no_ogl();
#else
    const int kind = arr.kind();

    const int cn = format_ == DEPTH_COMPONENT ? 1: format_ == RGB ? 3 : 4;
    const GLenum dstFormat = format_ == DEPTH_COMPONENT ? gl::DEPTH_COMPONENT : format_ == RGB ? gl::BGR : gl::BGRA;

    switch(kind)
    {
    case _InputArray::OPENGL_BUFFER:
        {
            ogl::Buffer& buf = arr.getOGlBufferRef();
            buf cvSetMat(rows_, cols_, CC_MAKE_TYPE(ddepth, cn), ogl::Buffer::PIXEL_PACK_BUFFER, autoRelease);
            buf.bind(ogl::Buffer::PIXEL_PACK_BUFFER);
            impl_->copyTo(dstFormat, gl_types[ddepth], 0);
            ogl::Buffer::unbind(ogl::Buffer::PIXEL_PACK_BUFFER);
            break;
        }

    case _InputArray::CUDA_GPU_MAT:
        {
            #ifndef HAVE_CUDA
                throw_no_cuda();
            #else
                ogl::Buffer buf(rows_, cols_, CC_MAKE_TYPE(ddepth, cn), ogl::Buffer::PIXEL_PACK_BUFFER);
                buf.setAutoRelease(true);
                buf.bind(ogl::Buffer::PIXEL_PACK_BUFFER);
                impl_->copyTo(dstFormat, gl_types[ddepth], 0);
                ogl::Buffer::unbind(ogl::Buffer::PIXEL_PACK_BUFFER);
                buf.copyTo(arr);
            #endif

            break;
        }

    default:
        {
            arr cvSetMat(rows_, cols_, CC_MAKE_TYPE(ddepth, cn));
            img_t mat = arr;
            CC_Assert( mat CC_IS_CONT_MAT() );
            ogl::Buffer::unbind(ogl::Buffer::PIXEL_PACK_BUFFER);
            impl_->copyTo(dstFormat, gl_types[ddepth], mat.data);
        }
    }
#endif
}

void ogl::Texture2D::bind() const
{
#ifndef HAVE_OPENGL
    throw_no_ogl();
#else
    impl_->bind();
#endif
}

unsigned int ogl::Texture2D::texId() const
{
#ifndef HAVE_OPENGL
    throw_no_ogl();
    return 0;
#else
    return impl_->texId();
#endif
}


////////////////////////////////////////////////////////////////////////
// ogl::Arrays

void ogl::Arrays::setVertexArray(const img_t* vertex)
{
    const int cn = vertex CC_MAT_CN();
    const int depth = vertex CC_MAT_DEPTH();

    CC_Assert( cn == 2 || cn == 3 || cn == 4 );
    CC_Assert( depth == CC_16S || depth == CC_32S || depth == CC_32F || depth == CC_64F );

    if (vertex.kind() == _InputArray::OPENGL_BUFFER)
        vertex_ = vertex.getOGlBuffer();
    else
        vertex_.copyFrom(vertex);

    size_ = vertex_.size().area();
}

void ogl::Arrays::resetVertexArray()
{
    vertex_.release();
    size_ = 0;
}

void ogl::Arrays::setColorArray(const img_t* color)
{
    const int cn = color CC_MAT_CN();

    CC_Assert( cn == 3 || cn == 4 );

    if (color.kind() == _InputArray::OPENGL_BUFFER)
        color_ = color.getOGlBuffer();
    else
        color_.copyFrom(color);
}

void ogl::Arrays::resetColorArray()
{
    color_.release();
}

void ogl::Arrays::setNormalArray(const img_t* normal)
{
    const int cn = normal CC_MAT_CN();
    const int depth = normal CC_MAT_DEPTH();

    CC_Assert( cn == 3 );
    CC_Assert( depth == CC_8S || depth == CC_16S || depth == CC_32S || depth == CC_32F || depth == CC_64F );

    if (normal.kind() == _InputArray::OPENGL_BUFFER)
        normal_ = normal.getOGlBuffer();
    else
        normal_.copyFrom(normal);
}

void ogl::Arrays::resetNormalArray()
{
    normal_.release();
}

void ogl::Arrays::setTexCoordArray(const img_t* texCoord)
{
    const int cn = texCoord CC_MAT_CN();
    const int depth = texCoord CC_MAT_DEPTH();

    CC_Assert( cn >= 1 && cn <= 4 );
    CC_Assert( depth == CC_16S || depth == CC_32S || depth == CC_32F || depth == CC_64F );

    if (texCoord.kind() == _InputArray::OPENGL_BUFFER)
        texCoord_ = texCoord.getOGlBuffer();
    else
        texCoord_.copyFrom(texCoord);
}

void ogl::Arrays::resetTexCoordArray()
{
    texCoord_.release();
}

void ogl::Arrays::release()
{
    resetVertexArray();
    resetColorArray();
    resetNormalArray();
    resetTexCoordArray();
}

void ogl::Arrays::setAutoRelease(bool flag)
{
    vertex_.setAutoRelease(flag);
    color_.setAutoRelease(flag);
    normal_.setAutoRelease(flag);
    texCoord_.setAutoRelease(flag);
}

void ogl::Arrays::bind() const
{
#ifndef HAVE_OPENGL
    throw_no_ogl();
#else
    CC_Assert( texCoord_.empty() || texCoord_.size().area() == size_ );
    CC_Assert( normal_.empty() || normal_.size().area() == size_ );
    CC_Assert( color_.empty() || color_.size().area() == size_ );

    if (texCoord_.empty())
    {
        gl::DisableClientState(gl::TEXTURE_COORD_ARRAY);
        CC_CheckGlError();
    }
    else
    {
        gl::EnableClientState(gl::TEXTURE_COORD_ARRAY);
        CC_CheckGlError();

        texCoord_.bind(ogl::Buffer::ARRAY_BUFFER);

        gl::TexCoordPointer(texCoord_ CC_MAT_CN(), gl_types[texCoord_ CC_MAT_DEPTH()], 0, 0);
        CC_CheckGlError();
    }

    if (normal_.empty())
    {
        gl::DisableClientState(gl::NORMAL_ARRAY);
        CC_CheckGlError();
    }
    else
    {
        gl::EnableClientState(gl::NORMAL_ARRAY);
        CC_CheckGlError();

        normal_.bind(ogl::Buffer::ARRAY_BUFFER);

        gl::NormalPointer(gl_types[normal_ CC_MAT_DEPTH()], 0, 0);
        CC_CheckGlError();
    }

    if (color_.empty())
    {
        gl::DisableClientState(gl::COLOR_ARRAY);
        CC_CheckGlError();
    }
    else
    {
        gl::EnableClientState(gl::COLOR_ARRAY);
        CC_CheckGlError();

        color_.bind(ogl::Buffer::ARRAY_BUFFER);

        const int cn = color_ CC_MAT_CN();

        gl::ColorPointer(cn, gl_types[color_ CC_MAT_DEPTH()], 0, 0);
        CC_CheckGlError();
    }

    if (vertex_.empty())
    {
        gl::DisableClientState(gl::VERTEX_ARRAY);
        CC_CheckGlError();
    }
    else
    {
        gl::EnableClientState(gl::VERTEX_ARRAY);
        CC_CheckGlError();

        vertex_.bind(ogl::Buffer::ARRAY_BUFFER);

        gl::VertexPointer(vertex_ CC_MAT_CN(), gl_types[vertex_ CC_MAT_DEPTH()], 0, 0);
        CC_CheckGlError();
    }

    ogl::Buffer::unbind(ogl::Buffer::ARRAY_BUFFER);
#endif
}

////////////////////////////////////////////////////////////////////////
// Rendering

void ogl::render(const ogl::Texture2D& tex, Rect_<double> wndRect, Rect_<double> texRect)
{
#ifndef HAVE_OPENGL
    (void) tex;
    (void) wndRect;
    (void) texRect;
    throw_no_ogl();
#else
    if (!tex.empty())
    {
        gl::MatrixMode(gl::PROJECTION);
        gl::LoadIdentity();
        gl::Ortho(0.0, 1.0, 1.0, 0.0, -1.0, 1.0);
        CC_CheckGlError();

        gl::MatrixMode(gl::MODELVIEW);
        gl::LoadIdentity();
        CC_CheckGlError();

        gl::Disable(gl::LIGHTING);
        CC_CheckGlError();

        tex.bind();

        gl::Enable(gl::TEXTURE_2D);
        CC_CheckGlError();

        gl::TexEnvi(gl::TEXTURE_ENV, gl::TEXTURE_ENV_MODE, gl::REPLACE);
        CC_CheckGlError();

        gl::TexParameteri(gl::TEXTURE_2D, gl::TEXTURE_MIN_FILTER, gl::LINEAR);
        CC_CheckGlError();

        const float vertex[] =
        {
            wndRect.x, wndRect.y, 0.0f,
            wndRect.x, (wndRect.y + wndRect.height), 0.0f,
            wndRect.x + wndRect.width, (wndRect.y + wndRect.height), 0.0f,
            wndRect.x + wndRect.width, wndRect.y, 0.0f
        };
        const float texCoords[] =
        {
            texRect.x, texRect.y,
            texRect.x, texRect.y + texRect.height,
            texRect.x + texRect.width, texRect.y + texRect.height,
            texRect.x + texRect.width, texRect.y
        };

        ogl::Buffer::unbind(ogl::Buffer::ARRAY_BUFFER);

        gl::EnableClientState(gl::TEXTURE_COORD_ARRAY);
        CC_CheckGlError();

        gl::TexCoordPointer(2, gl::FLOAT, 0, texCoords);
        CC_CheckGlError();

        gl::DisableClientState(gl::NORMAL_ARRAY);
        gl::DisableClientState(gl::COLOR_ARRAY);
        CC_CheckGlError();

        gl::EnableClientState(gl::VERTEX_ARRAY);
        CC_CheckGlError();

        gl::VertexPointer(3, gl::FLOAT, 0, vertex);
        CC_CheckGlError();

        gl::DrawArrays(gl::QUADS, 0, 4);
        CC_CheckGlError();
    }
#endif
}

void ogl::render(const ogl::Arrays& arr, int mode, Scalar color)
{
#ifndef HAVE_OPENGL
    (void) arr;
    (void) mode;
    (void) color;
    throw_no_ogl();
#else
    if (!arr.empty())
    {
        gl::Color3d(color[0] / 255.0, color[1] / 255.0, color[2] / 255.0);

        arr.bind();

        gl::DrawArrays(mode, 0, arr.size());
    }
#endif
}

void ogl::render(const ogl::Arrays& arr, const img_t* indices, int mode, Scalar color)
{
#ifndef HAVE_OPENGL
    (void) arr;
    (void) indices;
    (void) mode;
    (void) color;
    throw_no_ogl();
#else
    if (!arr.empty() && !indices.empty())
    {
        gl::Color3d(color[0] / 255.0, color[1] / 255.0, color[2] / 255.0);

        arr.bind();

        const int kind = indices.kind();

        switch (kind)
        {
        case _InputArray::OPENGL_BUFFER :
            {
                ogl::Buffer buf = indices.getOGlBuffer();

                const int depth = buf CC_MAT_DEPTH();

                CC_Assert( buf CC_MAT_CN() == 1 );
                CC_Assert( depth <= CC_32S );

                GLenum type;
                if (depth < CC_16U)
                    type = gl::UNSIGNED_BYTE;
                else if (depth < CC_32S)
                    type = gl::UNSIGNED_SHORT;
                else
                    type = gl::UNSIGNED_INT;

                buf.bind(ogl::Buffer::ELEMENT_ARRAY_BUFFER);

                gl::DrawElements(mode, buf.size().area(), type, 0);

                ogl::Buffer::unbind(ogl::Buffer::ELEMENT_ARRAY_BUFFER);

                break;
            }

        default:
            {
                img_t mat = indices;

                const int depth = mat CC_MAT_DEPTH();

                CC_Assert( mat CC_MAT_CN() == 1 );
                CC_Assert( depth <= CC_32S );
                CC_Assert( mat CC_IS_CONT_MAT() );

                GLenum type;
                if (depth < CC_16U)
                    type = gl::UNSIGNED_BYTE;
                else if (depth < CC_32S)
                    type = gl::UNSIGNED_SHORT;
                else
                    type = gl::UNSIGNED_INT;

                ogl::Buffer::unbind(ogl::Buffer::ELEMENT_ARRAY_BUFFER);

                gl::DrawElements(mode, mat.size().area(), type, mat.data);
            }
        }
    }
#endif
}

////////////////////////////////////////////////////////////////////////
// CL-GL Interoperability

#ifdef HAVE_OPENCL
#  include "opencv2/core/opencl/runtime/opencl_gl.hpp"
#  ifdef cl_khr_gl_sharing
#    define HAVE_OPENCL_OPENGL_SHARING
#  else
#    define NO_OPENCL_SHARING_ERROR CC_ErrorNoReturn(CC_StsBadFunc, "OpenCV was build without OpenCL/OpenGL sharing support")
#  endif
#else // HAVE_OPENCL
#  define NO_OPENCL_SUPPORT_ERROR CC_ErrorNoReturn(CC_StsBadFunc, "OpenCV was build without OpenCL support")
#endif // HAVE_OPENCL

#if defined(HAVE_OPENGL)
#  if defined(ANDROID)
#    include <EGL/egl.h>
#  elif defined(__linux__)
#    include <GL/glx.h>
#  endif
#endif // HAVE_OPENGL

namespace cv { namespace ogl {

namespace ocl {

Context& initializeContextFromGL()
{
#if !defined(HAVE_OPENGL)
    NO_OPENGL_SUPPORT_ERROR;
#elif !defined(HAVE_OPENCL)
    NO_OPENCL_SUPPORT_ERROR;
#elif !defined(HAVE_OPENCL_OPENGL_SHARING)
    NO_OPENCL_SHARING_ERROR;
#else
    cl_uint numPlatforms;
    cl_int status = clGetPlatformIDs(0, NULL, &numPlatforms);
    if (status != CL_SUCCESS)
        CC_Error(CC_OpenCLInitError, "OpenCL: Can't get number of platforms");
    if (numPlatforms == 0)
        CC_Error(CC_OpenCLInitError, "OpenCL: No available platforms");

    std::vector<cl_platform_id> platforms(numPlatforms);
    status = clGetPlatformIDs(numPlatforms, &platforms[0], NULL);
    if (status != CL_SUCCESS)
        CC_Error(CC_OpenCLInitError, "OpenCL: Can't get number of platforms");

    // TODO Filter platforms by name from OPENCC_OPENCL_DEVICE

    int found = -1;
    cl_device_id device = NULL;
    cl_context context = NULL;

    for (int i = 0; i < (int)numPlatforms; i++)
    {
        // query platform extension: presence of "cl_khr_gl_sharing" extension is requred
        {
            CAutoBuffer<char> extensionStr;

            size_t extensionSize;
            status = clGetPlatformInfo(platforms[i], CL_PLATFORM_EXTENSIONS, 0, NULL, &extensionSize);
            if (status == CL_SUCCESS)
            {
                extensionStr.allocate(extensionSize+1);
                status = clGetPlatformInfo(platforms[i], CL_PLATFORM_EXTENSIONS, extensionSize, (char*)extensionStr, NULL);
            }
            if (status != CL_SUCCESS)
                CC_Error(CC_OpenCLInitError, "OpenCL: Can't get platform extension string");

            if (!strstr((const char*)extensionStr, "cl_khr_gl_sharing"))
                continue;
        }

        clGetGLContextInfoKHR_fn clGetGLContextInfoKHR = (clGetGLContextInfoKHR_fn)
                clGetExtensionFunctionAddressForPlatform(platforms[i], "clGetGLContextInfoKHR");
        if (!clGetGLContextInfoKHR)
            continue;

        cl_context_properties properties[] =
        {
#if defined(WIN32) || defined(_WIN32)
            CL_CONTEXT_PLATFORM, (cl_context_properties)platforms[i],
            CL_GL_CONTEXT_KHR, (cl_context_properties)wglGetCurrentContext(),
            CL_WGL_HDC_KHR, (cl_context_properties)wglGetCurrentDC(),
#elif defined(ANDROID)
            CL_CONTEXT_PLATFORM, (cl_context_properties)platforms[i],
            CL_GL_CONTEXT_KHR, (cl_context_properties)eglGetCurrentContext(),
            CL_EGL_DISPLAY_KHR, (cl_context_properties)eglGetCurrentDisplay(),
#elif defined(__linux__)
            CL_CONTEXT_PLATFORM, (cl_context_properties)platforms[i],
            CL_GL_CONTEXT_KHR, (cl_context_properties)glXGetCurrentContext(),
            CL_GLX_DISPLAY_KHR, (cl_context_properties)glXGetCurrentDisplay(),
#endif
            0
        };

        // query device
        device = NULL;
        status = clGetGLContextInfoKHR(properties, CL_CURRENT_DEVICE_FOR_GL_CONTEXT_KHR, sizeof(cl_device_id), (void*)&device, NULL);
        if (status != CL_SUCCESS)
            continue;

        // create context
        context = clCreateContext(properties, 1, &device, NULL, NULL, &status);
        if (status != CL_SUCCESS)
        {
            clReleaseDevice(device);
        }
        else
        {
            found = i;
            break;
        }
    }

    if (found < 0)
        CC_Error(CC_OpenCLInitError, "OpenCL: Can't create context for OpenGL interop");

    Context& ctx = Context::getDefault(false);
    initializeContextFromHandle(ctx, platforms[found], context, device);
    return ctx;
#endif
}

} // namespace ogl::ocl

void convertToGLTexture2D(const img_t* src, Texture2D& texture)
{
    (void)src; (void)texture;
#if !defined(HAVE_OPENGL)
    NO_OPENGL_SUPPORT_ERROR;
#elif !defined(HAVE_OPENCL)
    NO_OPENCL_SUPPORT_ERROR;
#elif !defined(HAVE_OPENCL_OPENGL_SHARING)
    NO_OPENCL_SHARING_ERROR;
#else
    Size srcSize = cvGetSize(src);
    CC_Assert(srcSize.width == (int)texture->cols() && srcSize.height == (int)texture->rows());

    using namespace ocl;
    Context& ctx = Context::getDefault();
    cl_context context = (cl_context)ctx->tt.data;

    UMat u = src.getUMat();

    // TODO Add support for roi
    CC_Assert(u.offset == 0);
    CC_Assert(u CC_IS_CONT_MAT());

    cl_int status = 0;
    cl_mem clImage = clCreateFromGLTexture(context, CL_MEM_WRITE_ONLY, gl::TEXTURE_2D, 0, texture.texId(), &status);
    if (status != CL_SUCCESS)
        CC_Error(CC_OpenCLApiCallError, "OpenCL: clCreateFromGLTexture failed");

    cl_mem clBuffer = (cl_mem)u.handle(ACCESS_READ);

    cl_command_queue q = (cl_command_queue)Queue::getDefault()->tt.data;
    status = clEnqueueAcquireGLObjects(q, 1, &clImage, 0, NULL, NULL);
    if (status != CL_SUCCESS)
        CC_Error(CC_OpenCLApiCallError, "OpenCL: clEnqueueAcquireGLObjects failed");
    size_t offset = 0; // TODO
    size_t dst_origin[3] = {0, 0, 0};
    size_t region[3] = {u->cols, u->rows, 1};
    status = clEnqueueCopyBufferToImage(q, clBuffer, clImage, offset, dst_origin, region, 0, NULL, NULL);
    if (status != CL_SUCCESS)
        CC_Error(CC_OpenCLApiCallError, "OpenCL: clEnqueueCopyBufferToImage failed");
    status = clEnqueueReleaseGLObjects(q, 1, &clImage, 0, NULL, NULL);
    if (status != CL_SUCCESS)
        CC_Error(CC_OpenCLApiCallError, "OpenCL: clEnqueueReleaseGLObjects failed");

    status = clFinish(q); // TODO Use events
    if (status != CL_SUCCESS)
        CC_Error(CC_OpenCLApiCallError, "OpenCL: clFinish failed");

    status = clReleaseMemObject(clImage); // TODO RAII
    if (status != CL_SUCCESS)
        CC_Error(CC_OpenCLApiCallError, "OpenCL: clReleaseMemObject failed");
#endif
}

void convertFromGLTexture2D(const Texture2D& texture, img_t* dst)
{
    (void)texture; (void)dst;
#if !defined(HAVE_OPENGL)
    NO_OPENGL_SUPPORT_ERROR;
#elif !defined(HAVE_OPENCL)
    NO_OPENCL_SUPPORT_ERROR;
#elif !defined(HAVE_OPENCL_OPENGL_SHARING)
    NO_OPENCL_SHARING_ERROR;
#else
    // check texture format
    const int dtype = CC_8UC4;
    CC_Assert(texture.format() == Texture2D::RGBA);

    int textureType = dtype;
    CC_Assert(textureType >= 0);

    using namespace ocl;
    Context& ctx = Context::getDefault();
    cl_context context = (cl_context)ctx->tt.data;

    // TODO Need to specify ACCESS_WRITE here somehow to prevent useless data copying!
    dst cvSetMat(texture.size(), textureType);
    UMat u = dst.getUMat();

    // TODO Add support for roi
    CC_Assert(u.offset == 0);
    CC_Assert(u CC_IS_CONT_MAT());

    cl_int status = 0;
    cl_mem clImage = clCreateFromGLTexture(context, CL_MEM_READ_ONLY, gl::TEXTURE_2D, 0, texture.texId(), &status);
    if (status != CL_SUCCESS)
        CC_Error(CC_OpenCLApiCallError, "OpenCL: clCreateFromGLTexture failed");

    cl_mem clBuffer = (cl_mem)u.handle(ACCESS_READ);

    cl_command_queue q = (cl_command_queue)Queue::getDefault()->tt.data;
    status = clEnqueueAcquireGLObjects(q, 1, &clImage, 0, NULL, NULL);
    if (status != CL_SUCCESS)
        CC_Error(CC_OpenCLApiCallError, "OpenCL: clEnqueueAcquireGLObjects failed");
    size_t offset = 0; // TODO
    size_t src_origin[3] = {0, 0, 0};
    size_t region[3] = {u->cols, u->rows, 1};
    status = clEnqueueCopyImageToBuffer(q, clImage, clBuffer, src_origin, region, offset, 0, NULL, NULL);
    if (status != CL_SUCCESS)
        CC_Error(CC_OpenCLApiCallError, "OpenCL: clEnqueueCopyImageToBuffer failed");
    status = clEnqueueReleaseGLObjects(q, 1, &clImage, 0, NULL, NULL);
    if (status != CL_SUCCESS)
        CC_Error(CC_OpenCLApiCallError, "OpenCL: clEnqueueReleaseGLObjects failed");

    status = clFinish(q); // TODO Use events
    if (status != CL_SUCCESS)
        CC_Error(CC_OpenCLApiCallError, "OpenCL: clFinish failed");

    status = clReleaseMemObject(clImage); // TODO RAII
    if (status != CL_SUCCESS)
        CC_Error(CC_OpenCLApiCallError, "OpenCL: clReleaseMemObject failed");
#endif
}

//void mapGLBuffer(const Buffer& buffer, UMat& dst, int accessFlags)
UMat mapGLBuffer(const Buffer& buffer, int accessFlags)
{
    (void)buffer; (void)accessFlags;
#if !defined(HAVE_OPENGL)
    NO_OPENGL_SUPPORT_ERROR;
#elif !defined(HAVE_OPENCL)
    NO_OPENCL_SUPPORT_ERROR;
#elif !defined(HAVE_OPENCL_OPENGL_SHARING)
    NO_OPENCL_SHARING_ERROR;
#else
    using namespace ocl;
    Context& ctx = Context::getDefault();
    cl_context context = (cl_context)ctx->tt.data;
    cl_command_queue clQueue = (cl_command_queue)Queue::getDefault()->tt.data;

    int clAccessFlags = 0;
    switch (accessFlags & (ACCESS_READ|ACCESS_WRITE))
    {
    default:
    case ACCESS_READ|ACCESS_WRITE:
        clAccessFlags = CL_MEM_READ_WRITE;
        break;
    case ACCESS_READ:
        clAccessFlags = CL_MEM_READ_ONLY;
        break;
    case ACCESS_WRITE:
        clAccessFlags = CL_MEM_WRITE_ONLY;
        break;
    }

    cl_int status = 0;
    cl_mem clBuffer = clCreateFromGLBuffer(context, clAccessFlags, buffer.bufId(), &status);
    if (status != CL_SUCCESS)
        CC_Error(CC_OpenCLApiCallError, "OpenCL: clCreateFromGLBuffer failed");

    gl::Finish();

    status = clEnqueueAcquireGLObjects(clQueue, 1, &clBuffer, 0, NULL, NULL);
    if (status != CL_SUCCESS)
        CC_Error(CC_OpenCLApiCallError, "OpenCL: clEnqueueAcquireGLObjects failed");

    size_t step = buffer->cols() * buffer CC_ELEM_SIZE();
    int rows = buffer->rows();
    int cols = buffer->cols();
    int type = buffer CC_MAT_TYPE();

    UMat u;
    convertFromBuffer(clBuffer, step, rows, cols, type, u);
    return u;
#endif
}

void unmapGLBuffer(UMat& u)
{
    (void)u;
#if !defined(HAVE_OPENGL)
    NO_OPENGL_SUPPORT_ERROR;
#elif !defined(HAVE_OPENCL)
    NO_OPENCL_SUPPORT_ERROR;
#elif !defined(HAVE_OPENCL_OPENGL_SHARING)
    NO_OPENCL_SHARING_ERROR;
#else
    using namespace ocl;
    cl_command_queue clQueue = (cl_command_queue)Queue::getDefault()->tt.data;

    cl_mem clBuffer = (cl_mem)u.handle(ACCESS_READ);

    u.release();

    cl_int status = clEnqueueReleaseGLObjects(clQueue, 1, &clBuffer, 0, NULL, NULL);
    if (status != CL_SUCCESS)
        CC_Error(CC_OpenCLApiCallError, "OpenCL: clEnqueueReleaseGLObjects failed");

    status = clFinish(clQueue);
    if (status != CL_SUCCESS)
        CC_Error(CC_OpenCLApiCallError, "OpenCL: clFinish failed");

    status = clReleaseMemObject(clBuffer);
    if (status != CL_SUCCESS)
        CC_Error(CC_OpenCLApiCallError, "OpenCL: clReleaseMemObject failed");
#endif
}

}} // namespace ogl
