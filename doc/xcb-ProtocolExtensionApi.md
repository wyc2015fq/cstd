#    xcb-ProtocolExtensionApi    

   [Edit](https://secure.freedesktop.org/write/xcb/ikiwiki.cgi?do=edit&page=ProtocolExtensionApi)    [Page History](https://cgit.freedesktop.org/wiki/xcb/log/ProtocolExtensionApi.mdwn)   [Repo Info](https://cgit.freedesktop.org/wiki/xcb/)      

These functions are expected to be useful only to X protocol  extension implementations built on XCB. They are declared in xcbext.h.

Care is required when calling these functions, as they generally do  not have the type-safety properties of the public API functions.



## `struct xcb_extension_t`

```
    struct xcb_extension_t {
        const char *name;         // the X name of this extension, stored as a null-terminated string
        int global_id;            // a small integer unique across all extensions in the current process
    };
```

## `xcb_protocol_request_t`

```
    typedef struct {
        size_t count;            // number of elements in the iovec array
        xcb_extension_t *ext;    // extension identifier, or null for the core protocol
        uint8_t opcode;          // major opcode of the request for the core protocol, or minor opcode for an extension
        uint8_t isvoid;          // zero if this request will return a reply; non-zero otherwise
    } xcb_protocol_request_t;
```

Values for a particular request that are always constant.

## `xcb_send_request`

(`xcb_out.c`)

```
    int
    xcb_send_request (xcb_connection *c,
                      int flags,                                // a combination of the flags `XCB_REQUEST_CHECKED`, `XCB_REQUEST_RAW`, and `XCB_REQUEST_DISCARD_REPLY`
                      unsigned int *sequence,
                      struct iovec *vector,
                      const xcb_protocol_request_t *request );
```

## `xcb_wait_for_reply`

(`xcb_in.c`)

```
    void *
    xcb_wait_for_reply (xcb_connection *c,
                        unsigned int request,
                        xcb_generic_error_t **e );
```

## `xcb_poll_for_reply`

(`xcb_in.c`)

```
    int
    xcb_poll_for_reply (xcb_connection *c,
                        unsigned int request,
                        void **reply,
                        xcb_generic_error_t **e );
```

Returns: 1 if the reply was recieved (reply or error is filled in), 0 if the reply has not yet arrived.

## `xcb_popcount`

(`xcb_util.c`)

```
    int
    xcb_popcount (uint32_t mask);
```

Returns the number of bits in the binary representation of `mask` that are equal to 1. This is also called a "population count".

 Links:  [XcbApi](https://xcb.freedesktop.org/XcbApi/)  [XcbPythonBinding](https://xcb.freedesktop.org/XcbPythonBinding/)   

 Last edited Wed 29 Oct 2008 11:50:08 PM UTC