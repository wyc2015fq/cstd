# xcb-PublicApi    



The XCB public API contains functions that are auxilliary to the X Protocol.



## Types

These types are declared in xcb.h.



### `xcb_connection_t`

(xcb.h)

```
    typedef struct xcb_connection_t xcb_connection_t;
```

An `xcb_connection_t` is an opaque structure containing  all data that XCB needs to communicate with an X server. The structure  is defined in xcbint.h.



### `xcb_extension_t`

(xcb.h)

```
    typedef struct xcb_extension_t xcb_extension_t;
```

An `xcb_extension_t` is an opaque structure used as key for [`xcb_get_extension_data`](https://xcb.freedesktop.org/PublicApi/#xcb_get_extension_data).



### `xcb_auth_info_t`

(xcb.h)

```
    typedef struct xcb_auth_info_t {
        int namelen;
        char *name;       // string containing the authentication protocol name, such as &quot;MIT-MAGIC-COOKIE-1&quot; or &quot;XDM-AUTHORIZATION-1&quot;.
        int datalen;
        char *data;       // interpreted in a protocol-specific manner
    } xcb_auth_info_t;
```

A container for authorization information to be sent to the X server.

------

These functions are declared in xcb.h.



## `xcb_connect`

(`xcb_util.c`)

```
    xcb_connection_t *
    xcb_connect (const char *display,
                 int *screen );
```

Connects to the X server specified by `display`. If `display`  is null, uses the value of the DISPLAY environment variable. If a  particular screen on that server is preferred, the int pointed to by `screen` (if non-null) will be set to that screen; otherwise the screen will be set to 0.



## `xcb_connect_to_display_with_auth_info`

(`xcb_util.c`)

```
    xcb_connection_t *
    xcb_connect_to_display_with_auth_info (const char *display,
                                           xcb_auth_info_t *auth,
                                           int *screen );
```

Connects to the X server specified by `display`, using the given authorization information. If a particular screen on that server is preferred, the int pointed to by `screen` (if non-null) will be set to that screen; otherwise the screen will be set to 0.



## `xcb_connect_to_fd`

(`xcb_conn.c`)

```
    xcb_connection_t *
    xcb_connect_to_fd (int fd,                                    // a file descriptor bidirectionally connected to an X server.
                       xcb_auth_info_t *auth_info );   // authentication data, or 0 if the connection should be unauthenticated. xcb_get_auth_info returns appropriate authentication data
```

Connects to an X server, given an open socket and a suitable [`xcb_auth_info_t`](https://xcb.freedesktop.org/PublicApi/#xcb_auth_info_t).

Returns an [xcb_connection_t](https://xcb.freedesktop.org/PublicApi/#xcb_connection_t).

See also [`xcb_connect`](https://xcb.freedesktop.org/PublicApi/#xcb_connect).



## `xcb_disconnect`

(`xcb_conn.c`)

```
    void
    xcb_disconnect (xcb_connection *c);
```

Closes the file descriptor and frees all memory associated with the connection. Only close a connection once.



## `xcb_parse_display`

(`xcb_util.c`)

```
    int
    xcb_parse_display (const char *name,         // the display name to parse; if null or empty, uses the environment variable DISPLAY.
                       char **host,     // a non-null pointer to a pointer which will be set to a malloc&#39;d copy of the hostname.
                       int *display,    // a non-null pointer to an int which will be set to the display number
                       int *screen );   // a (possibly null) pointer to an int which will be set to the preferred screen number, or set to 0 if the display string does not contain a screen number
```

Parses a display string `name` in the form documented by X(7x). Has no side effects on failure.

Returns 0 on failure (perhaps `name` was null or unparsable, or malloc failed); non-zero on success.



## `xcb_get_setup`

(`xcb_conn.c`)

```
    const xcb_setup_t *
    xcb_get_setup (xcb_connection_t *c);
```

Accessor for the data returned by the server when the [`xcb_connection_t`](https://xcb.freedesktop.org/PublicApi/#xcb_connection_t) was initialized.

This data includes

- the server's required format for images,
- a list of available visuals,
- a list of available screens,
- the server's maximum request length (in the absence of the BIG-REQUESTS extension),
- and other assorted information.

See the X protocol specification and `xcb_types.h` for more details.

This routine returns a pointer into the connection data.  It should  not be freed, and will become invalid when the connection is freed.



## `xcb_get_file_descriptor`

(`xcb_conn.c`)

```
    int
    xcb_get_file_descriptor (xcb_connection_t *c);
```

Accessor for the file descriptor that was passed to the [`xcb_connect_to_fd`](https://xcb.freedesktop.org/PublicApi/#xcb_connect_to_fd) call that returned `c`.



## `xcb_get_maximum_request_length`

(`xcb_out.c`)

```
    uint32_t
    xcb_get_maximum_request_length (xcb_connection_t *c);
```

In the absence of the BIG-REQUESTS extension, returns the  maximum_request_length field from the connection setup data, which may  be as much as 65535. If the server supports BIG-REQUESTS, then the  maximum_request_length field from the reply to the [?](https://secure.freedesktop.org/write/xcb/ikiwiki.cgi?do=create&from=PublicApi&page=BigRequestsEnable)BigRequestsEnable request will be returned instead.

Note that this length is measured in four-byte units, making the  theoretical maximum lengths roughly 256kB without BIG-REQUESTS and 16GB  with.



## `xcb_wait_for_event`

(`xcb_in.c` [!] )

```
    xcb_generic_event_t *
    xcb_wait_for_event (xcb_connection_t *c);
```

Returns the next event or error from the server, or returns null in  the event of an I/O error. Blocks until either an event or error arrive,  or an I/O error occurs.



## `xcb_poll_for_event`

(`xcb_in.c`)

```
    xcb_generic_event_t *
    xcb_poll_for_event (xcb_connection_t *c);
```

Returns the next event or error from the server, if one is available,  or returns null otherwise. If no event is available, that might be  because an I/O error like connection close occurred while attempting to  read the next event. You can use [`xcb_connection_has_error`](https://xcb.freedesktop.org/PublicApi/#xcb_connection_has_error) to check for this condition.



## `xcb_connection_has_error`

```
    int
    xcb_connection_has_error (xcb_connection_t *c);
```

Returns non-zero if the connection has an error or zero if the  connection is still valid. If this ever returns non-zero, then the  connection is invalid, as if `xcb_disconnect` had been called.



## `xcb_flush`

(`xcb_out.c`)

```
    int
    xcb_flush (xcb_connection_t *c);
```

Forces any buffered output to be written to the server. Blocks until the write is complete.

Returns: 1 on success, 0 on failure



## `xcb_get_extension_data`

(`xcb_ext.c`)

```
    const xcb_query_extension_reply_t *
    xcb_get_extension_data (xcb_connection_t *c,
                            xcb_extension_t *ext );
```

This function is the primary interface to the "extension cache", which caches reply information from [?](https://secure.freedesktop.org/write/xcb/ikiwiki.cgi?do=create&from=PublicApi&page=QueryExtension)QueryExtension requests. Invoking this function may cause a call to `xcb_query_extension` to retrieve extension information from the server, and may block until extension data is received from the server.

Do **not** free the returned `xcb_query_extension_reply_t` - this storage is managed by the cache itself.



## `xcb_prefetch_extension_data`

(`xcb_ext.c`)

```
    void
    xcb_prefetch_extension_data (xcb_connection_t *c,
                                 xcb_extension_t *ext );
```

This function allows a "prefetch" of extension data into the extension cache. Invoking the function may cause a call to `xcb_query_extension`, but will not block waiting for the reply. [`xcb_get_extension_data`](https://xcb.freedesktop.org/PublicApi/#xcb_get_extension_data) will return the prefetched data after possibly blocking while it is retrieved.

## `xcb_generate_id`

(`xcb_xid.c`)

```
    uint32_t
    xcb_generate_id (xcb_connection *c);
```

This function allocates an XID prior to creating a new X object. For example, `xcb_create_window`:

```
    xcb_window_t window = xcb_generate_id (connection);
    xcb_create_window (connection, depth, window, ... );
```

 Links:  [XcbApi](https://xcb.freedesktop.org/XcbApi/)  [XcbPythonBinding](https://xcb.freedesktop.org/XcbPythonBinding/)   