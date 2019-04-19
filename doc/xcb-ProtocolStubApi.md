#    xcb-ProtocolStubApi    

   [Edit](https://secure.freedesktop.org/write/xcb/ikiwiki.cgi?do=edit&page=ProtocolStubApi)    [Page History](https://cgit.freedesktop.org/wiki/xcb/log/ProtocolStubApi.mdwn)   [Repo Info](https://cgit.freedesktop.org/wiki/xcb/)      

These are not single functions or types, but rather naming  conventions intended to be used in the core protocol API and the public  API for all extensions. All declarations associated with the core  protocol are in xcb.h, while each extension provides all of its  declarations in its own header file.

One of the design goals of this API is that users should be able to  learn these conventions, then read documentation on the binary encoding  of the core protocol or any extension, and immediately determine the  correct function name and argument list to generate the desired request.

Most of the X Window Protocol documentation lives at <http://www.x.org/releases/X11R7.7/doc/index.html#protocol>.

[!] ***TODO:** link to online descriptions of the core X protocol as well as de-facto standard extension protocols.*



## Extensions

### `xcb_extension_id`

```
    xcb_extension_t xcb_extension_id
```

(xcb.h)

A unique, opaque object identifying this extension for [`xcb_get_extension_data()`](https://xcb.freedesktop.org/ProtocolStubApi/#xcb_get_extension_data).

## Fields

Several conventions apply to field names.

- Fields have underscore-separated lower-case names.
- Where a field represents an object in an X protocol specification  document, the name is formed by replacing dash ("-") and space (" ")  characters with underscores ("_").
- Where a field holds the length of field "foo", its name is "foo_len".

## Types

The C name for the X type with name "Name" is `xcb_name_t`. If the type is from an extension, then "Name" is composed by prefixing the extension name onto the type name.

[!] ***TODO:**Links to XCBProtocolLanguage should be changed to point to appropriate XmlXcb documentation.*

Types are declared using the [STRUCT](https://xcb.freedesktop.org/XmlXcb/#STRUCT), [UNION](https://xcb.freedesktop.org/XmlXcb/#UNION), [XIDTYPE](https://xcb.freedesktop.org/XmlXcb/#XIDTYPE), [ENUM](https://xcb.freedesktop.org/XmlXcb/#ENUM), and [TYPEDEF](https://xcb.freedesktop.org/XmlXcb/#TYPEDEF) protocol description macros.



### `xcb_name_iterator_t`

```
    typedef struct {
             xcb_name_t *data;
             int rem;
             int index;
    } xcb_name_iterator_t;
```

An iterator may only be created for types declared using the [STRUCT](https://xcb.freedesktop.org/XmlXcb/#STRUCT) and [XIDTYPE](https://xcb.freedesktop.org/XmlXcb/#XIDTYPE) protocol description macros.

A special iterator type, `xcb_generic_iterator_t`, is declared in xcb.h.

### `xcb_name_next`

```
    void
    xcb_name_next (xcb_name_iterator_t *i);
```

Every [iterator](https://xcb.freedesktop.org/ProtocolStubApi/#xcb_name_iterator_t) has a `xcb_name_next` function.

### `xcb_name_end`

```
    xcb_generic_iterator_t
    xcb_name_end (xcb_name_iterator_t i);
```

Every [iterator](https://xcb.freedesktop.org/ProtocolStubApi/#xcb_name_iterator_t) has a `xcb_name_end` function.

### `xcb_type_name_field_iterator`

```
    xcb_base_type_iterator_t
    xcb_type_name_field_iterator (xcb_type_name_t *R);
```

Declared by the [LISTFIELD](https://xcb.freedesktop.org/XmlXcb/#LISTFIELD) protocol description macro.

### `xcb_type_name_field`

```
    BaseType *
    xcb_type_name_field (xcb_type_name_t *R);
```

Declared by the [LISTFIELD](https://xcb.freedesktop.org/XmlXcb/#LISTFIELD) protocol description macro.

### `xcb_type_name_field_length`

```
    int
    xcb_type_name_field_length (xcb_type_name_t *R);
```

Declared by the [LISTFIELD](https://xcb.freedesktop.org/XmlXcb/#LISTFIELD) protocol description macro.

### `xcb_type_name_field_end`

```
    xcb_generic_iterator_t
    xcb_type_name_field_end (xcb_type_name_t *R);
```

Declared by the [LISTFIELD](https://xcb.freedesktop.org/XmlXcb/#LISTFIELD) protocol description macro.

## Requests

Every request declared using the [REQUEST](https://xcb.freedesktop.org/XmlXcb/#REQUEST) or [VOIDREQUEST](https://xcb.freedesktop.org/XmlXcb/#VOIDREQUEST) protocol description macros generates an assortment of types and functions.

The base C name for all types and functions related to the X request with name "Name" is `xcb_name`. If the request is from an extension, then "Name" is composed by prefixing the extension name onto the request name.



### `xcb_name_cookie_t`

```
    typedef struct {
        unsigned int sequence;
    } xcb_name_cookie_t;
```

A special cookie type, `xcb_void_cookie_t`, is declared in xcb.h. See [`xcb_name`](https://xcb.freedesktop.org/ProtocolStubApi/#xcb_name).

### `xcb_name_request_t`

```
    typedef struct {
        uint8_t major_opcode;
        /* first single-byte field, uint8_t minor_opcode, or one byte of padding */
        uint16_t length;
        ...
    } xcb_name_request_t;
```

### `xcb_name_reply_t`

```
    typedef struct {
        uint8_t response_type;
        /* first single-byte field or one byte of padding */
        uint16_t sequence;
        uint32_t length;
        ...
        /* padding to 32 bytes */
    } xcb_name_reply_t;
```

`response_type` always equals 1 in a reply.

A special reply type, `xcb_generic_reply_t`, is declared in xcb.h. It contains only the fields common to all replies.



### `xcb_name`

```
    Cookie
    xcb_name (xcb_connection_t *c, ...); 
```

Delivers a request with the specified arguments to the X server.

If the request will cause a reply to be generated, then an [`xcb_name_cookie_t`](https://xcb.freedesktop.org/ProtocolStubApi/#xcb_name_cookie_t) will be returned and the [`xcb_name_reply_t`](https://xcb.freedesktop.org/ProtocolStubApi/#xcb_name_reply_t)  function should be used to obtain the actual reply. Any error generated  by the X server in response to this request will be returned through  any supplied error pointer in the corresponding Reply function rather  than being placed in the event queue. If no error pointer is supplied,  the error will be discarded.

Otherwise, an [`xcb_void_cookie_t`](https://xcb.freedesktop.org/ProtocolStubApi/#xcb_name_cookie_t) will be returned, and no [`xcb_name_reply_t`](https://xcb.freedesktop.org/ProtocolStubApi/#xcb_name_reply_t) function will be provided. Any error generated by the X server in response to this request will be placed in the event queue.



### `xcb_name_checked`

```
    xcb_void_cookie_t
    xcb_name_checked (xcb_connection_t *c, ...);
```

Delivers a request with the specified arguments to the X server.

This form can be used only if the request will not cause a reply to be generated. It behaves as `xcb_name`, except that any returned error will be saved for handling by [`xcb_request_check()`](https://xcb.freedesktop.org/ProtocolStubApi/#xcb_request_check) rather than being placed in the event queue.



### `xcb_name_unchecked`

```
    Cookie
    xcb_name_unchecked (xcb_connection_t *c, ...);
```

Delivers a request with the specified arguments to the X server.

This form can be used only if the request will cause a reply to be generated. It behaves as `xcb_name`, except that any returned error will be placed in the event queue rather than being handled by [`xcb_name_reply()`](https://xcb.freedesktop.org/ProtocolStubApi/#xcb_name_reply).



### `xcb_name_reply`

```
    xcb_name_reply_t *
    xcb_name_reply (xcb_connection_t *c,
                     xcb_name_cookie_t cookie,
                     xcb_generic_error_t **e );
```

The `xcb_generic_error_t **e` supplied to this function  must be zero if the cookie was produced using the unchecked version of  the request function; otherwise, if an error occurs and the pointer is  nonzero the error will be stored into it. See [`xcb_name()`](https://xcb.freedesktop.org/ProtocolStubApi/#xcb_name) and [`xcb_name_unchecked()`](https://xcb.freedesktop.org/ProtocolStubApi/#xcb_name_unchecked).  This function returns a pointer to freshly-malloced storage that may be freed when it is no longer needed.



### `xcb_request_check`

```
    xcb_generic_error_t *
    xcb_request_check (xcb_connection_t *c,
                       xcb_void_cookie_t cookie );
```

The `xcb_void_cookie_t cookie` supplied to this function must have resulted from a call to [`xcb_name_checked()`](https://xcb.freedesktop.org/ProtocolStubApi/#xcb_name_checked).  This function will block until one of two conditions happens. If an  error is received, it will be returned. If no error can be received,  because the sequence number has advanced beyond that provided in `cookie`, 0 will be returned.

/!\ Note that this function will perform an `xcb_aux_sync()` if needed to ensure that the sequence number will advance beyond that provided in `cookie`;  this is a convenience to avoid races in determining whether the sync is  needed. If you want a version of this call that never performs this  implicit sync (e.g. because you are writing in a multi-threaded  implementation in which another thread will eventually issue a request),  please let us know, and we will consider adding it.

### `xcb_name_field_iterator`

```
    xcb_base_type_iterator_t
    xcb_name_field_iterator (xcb_name_reply_t *R);
```

### `xcb_name_field`

```
    BaseType *
    xcb_name_field (xcb_name_reply_t *R);
```

### `xcb_name_field_length`

```
    int
    xcb_name_field_length (xcb_name_reply_t *R);
```

### `xcb_name_field_end`

```
    xcb_generic_iterator_t
    xcb_name_field_end (xcb_name_reply_t *R);
```

## Events and Errors

### `xcb_event_name_event_t`

```
    typedef struct {
         uint8_t response_type;
         /* first single-byte field or one byte of padding */
         uint16_t sequence;
         ...
         /* padding to 32 bytes */
    } xcb_event_name_event_t;
```

`response_type` equals the event number. See [`XCB_EVENT_NAME`](https://xcb.freedesktop.org/ProtocolStubApi/#XCB_EVENT_NAME).

A special event type, `xcb_generic_event_t`, is declared in xcb.h. It contains only the fields common to all events.



### `XCB_EVENT_NAME`

```
    #define XCB_EVENT_NAME number
```

### `xcb_error_name_error_t`

```
    typedef struct {
        uint8_t response_type;
        uint8_t error_code;
        uint16_t sequence;
        ...
        /* padding to 32 bytes */
    } xcb_error_name_error_t;
```

`response_type` always equals 0 for errors.

`error_code` equals the error number. See [`XCB_ERROR_NAME`](https://xcb.freedesktop.org/ProtocolStubApi/#XCB_ERROR_NAME).

A special error type, `xcb_generic_error_t`, is declared in xcb.h. It contains only the fields common to all errors.



### `XCB_ERROR_NAME`

```
    #define XCB_ERROR_NAME number
```

 Links:  [XcbApi](https://xcb.freedesktop.org/XcbApi/)   

 Last edited Sat 14 Sep 2013 12:39:25 AM UTC