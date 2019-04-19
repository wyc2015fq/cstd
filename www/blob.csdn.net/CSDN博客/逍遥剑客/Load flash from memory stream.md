# Load flash from memory stream - 逍遥剑客 - CSDN博客
2010年08月09日 16:59:00[xoyojank](https://me.csdn.net/xoyojank)阅读数：2158
http://stackoverflow.com/questions/423166/flash-activex-how-to-load-movie-from-memory-or-resource-or-stream
void flash_load_memory(FlashWidget* w, void* data, ULONG size) {
        FlashMemoryStream fStream = FlashMemoryStream(data, size);
        IPersistStreamInit* psStreamInit = NULL;
        w->mFlashInterface->QueryInterface(IID_IPersistStreamInit,(LPVOID*) &psStreamInit);
        if(psStreamInit) {
            psStreamInit->InitNew();
            psStreamInit->Load((LPSTREAM)&fStream);
            psStreamInit->Release();
        }
    }
class FlashMemoryStream : IStream {
    public:
        FlashMemoryStream(void* data,ULONG size) {
            this->data = data;
            this->size = size;
            this->pos = 0;
        }
        HRESULT STDMETHODCALLTYPE QueryInterface(REFIID riid, LPVOID* ppv) {
            return E_NOTIMPL;
        }
        ULONG STDMETHODCALLTYPE AddRef() {  
            return E_NOTIMPL;
        }
        ULONG STDMETHODCALLTYPE Release() {  
            return E_NOTIMPL;
        }
        // IStream methods
        STDMETHOD(Read) (void *pv,ULONG cb,ULONG *pcbRead) {
            if(pos == 0 && cb == 4) {
                memcpy(pv,"fUfU",4);
                pos += 4;
                return S_OK;
            }
            else if(pos == 4 && cb == 4) {
                memcpy(pv,&size,4);
                size += 8;
                pos += 4;
                return S_OK;
            }
            else {
                if(pos + cb > size) cb = size - pos;
                if(cb == 0) return S_FALSE;
                memcpy(pv,(char*)data + pos - 8,cb);
                if(pcbRead) (*pcbRead) = cb;
                pos += cb;
                return S_OK;
            }
        }
        STDMETHOD(Write) (void const *pv,ULONG cb,ULONG *pcbWritten) { return E_NOTIMPL; }
        STDMETHOD(Seek) (LARGE_INTEGER dlibMove,DWORD dwOrigin,ULARGE_INTEGER *plibNewPosition) { return E_NOTIMPL; }
        STDMETHOD(SetSize) (ULARGE_INTEGER libNewSize) { return E_NOTIMPL; }
        STDMETHOD(CopyTo) (IStream *pstm,ULARGE_INTEGER cb,ULARGE_INTEGER *pcbRead,ULARGE_INTEGER *pcbWritten) { return E_NOTIMPL; }
        STDMETHOD(Commit) (DWORD grfCommitFlags) { return E_NOTIMPL; }
        STDMETHOD(Revert) (void) { return E_NOTIMPL; }
        STDMETHOD(LockRegion) (ULARGE_INTEGER libOffset,ULARGE_INTEGER cb,DWORD dwLockType) { return E_NOTIMPL; }
        STDMETHOD(UnlockRegion) (ULARGE_INTEGER libOffset,ULARGE_INTEGER cb,DWORD dwLockType) { return E_NOTIMPL; }
        STDMETHOD(Stat) (STATSTG *pstatstg,DWORD grfStatFlag) { return E_NOTIMPL; }
        STDMETHOD(Clone) (IStream **ppstm) { return E_NOTIMPL; }
        void* data;
        ULONG size;
        ULONG pos;
    };
