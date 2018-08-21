#ifndef _AFX_H_
#define _AFX_H_
#define _AFX_NO_RICHEDIT_SUPPORT
#include <crtdbg.h>
#define VS_VERSION_INFO     1
#ifdef APSTUDIO_INVOKED
#define APSTUDIO_HIDDEN_SYMBOLS // Ignore following symbols
#endif
#ifdef IDC_STATIC
#undef IDC_STATIC
#endif
#define IDC_STATIC      (-1)
#ifdef APSTUDIO_INVOKED
#define APSTUDIO_HIDDEN_SYMBOLS
#endif
/////////////////////////////////////////////////////////////////////////////
// MFC resource types (see Technical note TN024 for implementation details)
#ifndef RC_INVOKED
#define RT_DLGINIT  MAKEINTRESOURCE(240)
#define RT_TOOLBAR  MAKEINTRESOURCE(241)
#endif
/////////////////////////////////////////////////////////////////////////////
#ifdef APSTUDIO_INVOKED
#undef APSTUDIO_HIDDEN_SYMBOLS
#endif
/////////////////////////////////////////////////////////////////////////////
// General style bits etc
// ControlBar styles
#define CBRS_ALIGN_LEFT     0x1000L
#define CBRS_ALIGN_TOP      0x2000L
#define CBRS_ALIGN_RIGHT    0x4000L
#define CBRS_ALIGN_BOTTOM   0x8000L
#define CBRS_ALIGN_ANY      0xF000L
#define CBRS_BORDER_LEFT    0x0100L
#define CBRS_BORDER_TOP     0x0200L
#define CBRS_BORDER_RIGHT   0x0400L
#define CBRS_BORDER_BOTTOM  0x0800L
#define CBRS_BORDER_ANY     0x0F00L
#define CBRS_TOOLTIPS       0x0010L
#define CBRS_FLYBY          0x0020L
#define CBRS_FLOAT_MULTI    0x0040L
#define CBRS_BORDER_3D      0x0080L
#define CBRS_HIDE_INPLACE   0x0008L
#define CBRS_SIZE_DYNAMIC   0x0004L
#define CBRS_SIZE_FIXED     0x0002L
#define CBRS_FLOATING       0x0001L
#define CBRS_GRIPPER        0x00400000L
#define CBRS_ORIENT_HORZ    (CBRS_ALIGN_TOP|CBRS_ALIGN_BOTTOM)
#define CBRS_ORIENT_VERT    (CBRS_ALIGN_LEFT|CBRS_ALIGN_RIGHT)
#define CBRS_ORIENT_ANY     (CBRS_ORIENT_HORZ|CBRS_ORIENT_VERT)
#define CBRS_ALL            0x0040FFFFL
// the CBRS_ style is made up of an alignment style and a draw border style
//  the alignment styles are mutually exclusive
//  the draw border styles may be combined
#define CBRS_NOALIGN        0x00000000L
#define CBRS_LEFT           (CBRS_ALIGN_LEFT|CBRS_BORDER_RIGHT)
#define CBRS_TOP            (CBRS_ALIGN_TOP|CBRS_BORDER_BOTTOM)
#define CBRS_RIGHT          (CBRS_ALIGN_RIGHT|CBRS_BORDER_LEFT)
#define CBRS_BOTTOM         (CBRS_ALIGN_BOTTOM|CBRS_BORDER_TOP)
/////////////////////////////////////////////////////////////////////////////
// Standard window components
// Mode indicators in status bar - these are routed like commands
#define ID_INDICATOR_EXT                0xE700  // extended selection indicator
#define ID_INDICATOR_CAPS               0xE701  // cap lock indicator
#define ID_INDICATOR_NUM                0xE702  // num lock indicator
#define ID_INDICATOR_SCRL               0xE703  // scroll lock indicator
#define ID_INDICATOR_OVR                0xE704  // overtype mode indicator
#define ID_INDICATOR_REC                0xE705  // record mode indicator
#define ID_INDICATOR_KANA               0xE706  // kana lock indicator
#define ID_SEPARATOR                    0   // special separator value
#ifndef RC_INVOKED  // code only
// Standard control bars (IDW = window ID)
#define AFX_IDW_CONTROLBAR_FIRST        0xE800
#define AFX_IDW_CONTROLBAR_LAST         0xE8FF
#define AFX_IDW_TOOLBAR                 0xE800  // main Toolbar for window
#define AFX_IDW_STATUS_BAR              0xE801  // Status bar window
#define AFX_IDW_PREVIEW_BAR             0xE802  // PrintPreview Dialog Bar
#define AFX_IDW_RESIZE_BAR              0xE803  // OLE in-place resize bar
#define AFX_IDW_REBAR                   0xE804  // COMCTL32 "rebar" Bar
#define AFX_IDW_DIALOGBAR               0xE805  // CDialogBar
// Note: If your application supports docking toolbars, you should
//  not use the following IDs for your own toolbars.  The IDs chosen
//  are at the top of the first 32 such that the bars will be hidden
//  while in print preview mode, and are not likely to conflict with
//  IDs your application may have used succesfully in the past.
#define AFX_IDW_DOCKBAR_TOP             0xE81B
#define AFX_IDW_DOCKBAR_LEFT            0xE81C
#define AFX_IDW_DOCKBAR_RIGHT           0xE81D
#define AFX_IDW_DOCKBAR_BOTTOM          0xE81E
#define AFX_IDW_DOCKBAR_FLOAT           0xE81F
// Macro for mapping standard control bars to bitmask (limit of 32)
#define AFX_CONTROLBAR_MASK(nIDC)   (1L << (nIDC - AFX_IDW_CONTROLBAR_FIRST))
// parts of Main Frame
#define AFX_IDW_PANE_FIRST              0xE900  // first pane (256 max)
#define AFX_IDW_PANE_LAST               0xE9ff
#define AFX_IDW_HSCROLL_FIRST           0xEA00  // first Horz scrollbar (16 max)
#define AFX_IDW_VSCROLL_FIRST           0xEA10  // first Vert scrollbar (16 max)
#define AFX_IDW_SIZE_BOX                0xEA20  // size box for splitters
#define AFX_IDW_PANE_SAVE               0xEA21  // to shift AFX_IDW_PANE_FIRST
#endif //!RC_INVOKED
#ifndef APSTUDIO_INVOKED
// common style for form views
#define AFX_WS_DEFAULT_VIEW             (WS_CHILD | WS_VISIBLE | WS_BORDER)
#endif //!APSTUDIO_INVOKED
/////////////////////////////////////////////////////////////////////////////
// Standard app configurable strings
// for application title (defaults to EXE name or name in constructor)
#define AFX_IDS_APP_TITLE               0xE000
// idle message bar line
#define AFX_IDS_IDLEMESSAGE             0xE001
// message bar line when in shift-F1 help mode
#define AFX_IDS_HELPMODEMESSAGE         0xE002
// document title when editing OLE embedding
#define AFX_IDS_APP_TITLE_EMBEDDING     0xE003
// company name
#define AFX_IDS_COMPANY_NAME            0xE004
// object name when server is inplace
#define AFX_IDS_OBJ_TITLE_INPLACE       0xE005
/////////////////////////////////////////////////////////////////////////////
// Standard Commands
// File commands
#define ID_FILE_NEW                     0xE100
#define ID_FILE_OPEN                    0xE101
#define ID_FILE_CLOSE                   0xE102
#define ID_FILE_SAVE                    0xE103
#define ID_FILE_SAVE_AS                 0xE104
#define ID_FILE_PAGE_SETUP              0xE105
#define ID_FILE_PRINT_SETUP             0xE106
#define ID_FILE_PRINT                   0xE107
#define ID_FILE_PRINT_DIRECT            0xE108
#define ID_FILE_PRINT_PREVIEW           0xE109
#define ID_FILE_UPDATE                  0xE10A
#define ID_FILE_SAVE_COPY_AS            0xE10B
#define ID_FILE_SEND_MAIL               0xE10C
#define ID_FILE_MRU_FIRST               0xE110
#define ID_FILE_MRU_FILE1               0xE110          // range - 16 max
#define ID_FILE_MRU_FILE2               0xE111
#define ID_FILE_MRU_FILE3               0xE112
#define ID_FILE_MRU_FILE4               0xE113
#define ID_FILE_MRU_FILE5               0xE114
#define ID_FILE_MRU_FILE6               0xE115
#define ID_FILE_MRU_FILE7               0xE116
#define ID_FILE_MRU_FILE8               0xE117
#define ID_FILE_MRU_FILE9               0xE118
#define ID_FILE_MRU_FILE10              0xE119
#define ID_FILE_MRU_FILE11              0xE11A
#define ID_FILE_MRU_FILE12              0xE11B
#define ID_FILE_MRU_FILE13              0xE11C
#define ID_FILE_MRU_FILE14              0xE11D
#define ID_FILE_MRU_FILE15              0xE11E
#define ID_FILE_MRU_FILE16              0xE11F
#define ID_FILE_MRU_LAST                0xE11F
// Edit commands
#define ID_EDIT_CLEAR                   0xE120
#define ID_EDIT_CLEAR_ALL               0xE121
#define ID_EDIT_COPY                    0xE122
#define ID_EDIT_CUT                     0xE123
#define ID_EDIT_FIND                    0xE124
#define ID_EDIT_PASTE                   0xE125
#define ID_EDIT_PASTE_LINK              0xE126
#define ID_EDIT_PASTE_SPECIAL           0xE127
#define ID_EDIT_REPEAT                  0xE128
#define ID_EDIT_REPLACE                 0xE129
#define ID_EDIT_SELECT_ALL              0xE12A
#define ID_EDIT_UNDO                    0xE12B
#define ID_EDIT_REDO                    0xE12C
// Window commands
#define ID_WINDOW_NEW                   0xE130
#define ID_WINDOW_ARRANGE               0xE131
#define ID_WINDOW_CASCADE               0xE132
#define ID_WINDOW_TILE_HORZ             0xE133
#define ID_WINDOW_TILE_VERT             0xE134
#define ID_WINDOW_SPLIT                 0xE135
#ifndef RC_INVOKED      // code only
#define AFX_IDM_WINDOW_FIRST            0xE130
#define AFX_IDM_WINDOW_LAST             0xE13F
#define AFX_IDM_FIRST_MDICHILD          0xFF00  // window list starts here
#endif //!RC_INVOKED
// Help and App commands
#define ID_APP_ABOUT                    0xE140
#define ID_APP_EXIT                     0xE141
#define ID_HELP_INDEX                   0xE142
#define ID_HELP_FINDER                  0xE143
#define ID_HELP_USING                   0xE144
#define ID_CONTEXT_HELP                 0xE145      // shift-F1
// special commands for processing help
#define ID_HELP                         0xE146      // first attempt for F1
#define ID_DEFAULT_HELP                 0xE147      // last attempt
// Misc
#define ID_NEXT_PANE                    0xE150
#define ID_PREV_PANE                    0xE151
// Format
#define ID_FORMAT_FONT                  0xE160
// OLE commands
#define ID_OLE_INSERT_NEW               0xE200
#define ID_OLE_EDIT_LINKS               0xE201
#define ID_OLE_EDIT_CONVERT             0xE202
#define ID_OLE_EDIT_CHANGE_ICON         0xE203
#define ID_OLE_EDIT_PROPERTIES          0xE204
#define ID_OLE_VERB_FIRST               0xE210     // range - 16 max
#ifndef RC_INVOKED      // code only
#define ID_OLE_VERB_LAST                0xE21F
#endif //!RC_INVOKED
// for print preview dialog bar
#define AFX_ID_PREVIEW_CLOSE            0xE300
#define AFX_ID_PREVIEW_NUMPAGE          0xE301      // One/Two Page button
#define AFX_ID_PREVIEW_NEXT             0xE302
#define AFX_ID_PREVIEW_PREV             0xE303
#define AFX_ID_PREVIEW_PRINT            0xE304
#define AFX_ID_PREVIEW_ZOOMIN           0xE305
#define AFX_ID_PREVIEW_ZOOMOUT          0xE306
// View commands (same number used as IDW used for control bar)
#define ID_VIEW_TOOLBAR                 0xE800
#define ID_VIEW_STATUS_BAR              0xE801
#define ID_VIEW_REBAR                   0xE804
#define ID_VIEW_AUTOARRANGE         0xE805
// E810 -> E81F must be kept in order for RANGE macros
#define ID_VIEW_SMALLICON               0xE810
#define ID_VIEW_LARGEICON               0xE811
#define ID_VIEW_LIST                   0xE812
#define ID_VIEW_DETAILS                 0xE813
#define ID_VIEW_LINEUP                  0xE814
#define ID_VIEW_BYNAME                  0xE815
#define AFX_ID_VIEW_MINIMUM              ID_VIEW_SMALLICON
#define AFX_ID_VIEW_MAXIMUM              ID_VIEW_BYNAME
// E800 -> E8FF reserved for other control bar commands
// RecordForm commands
#define ID_RECORD_FIRST                 0xE900
#define ID_RECORD_LAST                  0xE901
#define ID_RECORD_NEXT                  0xE902
#define ID_RECORD_PREV                  0xE903
/////////////////////////////////////////////////////////////////////////////
// Standard control IDs
#ifdef IDC_STATIC
#undef IDC_STATIC
#endif
#define IDC_STATIC              (-1)     // all static controls
/////////////////////////////////////////////////////////////////////////////
// Standard string error/warnings
#ifndef RC_INVOKED      // code only
#define AFX_IDS_SCFIRST                 0xEF00
#endif //!RC_INVOKED
#define AFX_IDS_SCSIZE                  0xEF00
#define AFX_IDS_SCMOVE                  0xEF01
#define AFX_IDS_SCMINIMIZE              0xEF02
#define AFX_IDS_SCMAXIMIZE              0xEF03
#define AFX_IDS_SCNEXTWINDOW            0xEF04
#define AFX_IDS_SCPREVWINDOW            0xEF05
#define AFX_IDS_SCCLOSE                 0xEF06
#define AFX_IDS_SCRESTORE               0xEF12
#define AFX_IDS_SCTASKLIST              0xEF13
#define AFX_IDS_MDICHILD                0xEF1F
#define AFX_IDS_DESKACCESSORY           0xEFDA
// General strings
#define AFX_IDS_OPENFILE                0xF000
#define AFX_IDS_SAVEFILE                0xF001
#define AFX_IDS_ALLFILTER               0xF002
#define AFX_IDS_UNTITLED                0xF003
#define AFX_IDS_SAVEFILECOPY            0xF004
#define AFX_IDS_PREVIEW_CLOSE           0xF005
#define AFX_IDS_UNNAMED_FILE            0xF006
#define AFX_IDS_HIDE                    0xF011
// MFC Standard Exception Error messages
#define AFX_IDP_NO_ERROR_AVAILABLE      0xF020
#define AFX_IDS_NOT_SUPPORTED_EXCEPTION 0xF021
#define AFX_IDS_RESOURCE_EXCEPTION      0xF022
#define AFX_IDS_MEMORY_EXCEPTION        0xF023
#define AFX_IDS_USER_EXCEPTION          0xF024
// Printing and print preview strings
#define AFX_IDS_PRINTONPORT             0xF040
#define AFX_IDS_ONEPAGE                 0xF041
#define AFX_IDS_TWOPAGE                 0xF042
#define AFX_IDS_PRINTPAGENUM            0xF043
#define AFX_IDS_PREVIEWPAGEDESC         0xF044
#define AFX_IDS_PRINTDEFAULTEXT         0xF045
#define AFX_IDS_PRINTDEFAULT            0xF046
#define AFX_IDS_PRINTFILTER             0xF047
#define AFX_IDS_PRINTCAPTION            0xF048
#define AFX_IDS_PRINTTOFILE             0xF049
// OLE strings
#define AFX_IDS_OBJECT_MENUITEM         0xF080
#define AFX_IDS_EDIT_VERB               0xF081
#define AFX_IDS_ACTIVATE_VERB           0xF082
#define AFX_IDS_CHANGE_LINK             0xF083
#define AFX_IDS_AUTO                    0xF084
#define AFX_IDS_MANUAL                  0xF085
#define AFX_IDS_FROZEN                  0xF086
#define AFX_IDS_ALL_FILES               0xF087
// dynamically changing menu items
#define AFX_IDS_SAVE_MENU               0xF088
#define AFX_IDS_UPDATE_MENU             0xF089
#define AFX_IDS_SAVE_AS_MENU            0xF08A
#define AFX_IDS_SAVE_COPY_AS_MENU       0xF08B
#define AFX_IDS_EXIT_MENU               0xF08C
#define AFX_IDS_UPDATING_ITEMS          0xF08D
// COlePasteSpecialDialog defines
#define AFX_IDS_METAFILE_FORMAT         0xF08E
#define AFX_IDS_DIB_FORMAT              0xF08F
#define AFX_IDS_BITMAP_FORMAT           0xF090
#define AFX_IDS_LINKSOURCE_FORMAT       0xF091
#define AFX_IDS_EMBED_FORMAT            0xF092
// other OLE utility strings
#define AFX_IDS_PASTELINKEDTYPE         0xF094
#define AFX_IDS_UNKNOWNTYPE             0xF095
#define AFX_IDS_RTF_FORMAT              0xF096
#define AFX_IDS_TEXT_FORMAT             0xF097
// OLE datatype format error strings
#define AFX_IDS_INVALID_CURRENCY        0xF098
#define AFX_IDS_INVALID_DATETIME        0xF099
#define AFX_IDS_INVALID_DATETIMESPAN    0xF09A
// General error / prompt strings
#define AFX_IDP_INVALID_FILENAME        0xF100
#define AFX_IDP_FAILED_TO_OPEN_DOC      0xF101
#define AFX_IDP_FAILED_TO_SAVE_DOC      0xF102
#define AFX_IDP_ASK_TO_SAVE             0xF103
#define AFX_IDP_FAILED_TO_CREATE_DOC    0xF104
#define AFX_IDP_FILE_TOO_LARGE          0xF105
#define AFX_IDP_FAILED_TO_START_PRINT   0xF106
#define AFX_IDP_FAILED_TO_LAUNCH_HELP   0xF107
#define AFX_IDP_INTERNAL_FAILURE        0xF108      // general failure
#define AFX_IDP_COMMAND_FAILURE         0xF109      // command failure
#define AFX_IDP_FAILED_MEMORY_ALLOC     0xF10A
#define AFX_IDP_UNREG_DONE              0xF10B
#define AFX_IDP_UNREG_FAILURE           0xF10C
#define AFX_IDP_DLL_LOAD_FAILED         0xF10D
#define AFX_IDP_DLL_BAD_VERSION         0xF10E
// DDV parse errors
#define AFX_IDP_PARSE_INT               0xF110
#define AFX_IDP_PARSE_REAL              0xF111
#define AFX_IDP_PARSE_INT_RANGE         0xF112
#define AFX_IDP_PARSE_REAL_RANGE        0xF113
#define AFX_IDP_PARSE_STRING_SIZE       0xF114
#define AFX_IDP_PARSE_RADIO_BUTTON      0xF115
#define AFX_IDP_PARSE_BYTE              0xF116
#define AFX_IDP_PARSE_UINT              0xF117
#define AFX_IDP_PARSE_DATETIME          0xF118
#define AFX_IDP_PARSE_CURRENCY          0xF119
// CFile/CArchive error strings for user failure
#define AFX_IDP_FAILED_INVALID_FORMAT   0xF120
#define AFX_IDP_FAILED_INVALID_PATH     0xF121
#define AFX_IDP_FAILED_DISK_FULL        0xF122
#define AFX_IDP_FAILED_ACCESS_READ      0xF123
#define AFX_IDP_FAILED_ACCESS_WRITE     0xF124
#define AFX_IDP_FAILED_IO_ERROR_READ    0xF125
#define AFX_IDP_FAILED_IO_ERROR_WRITE   0xF126
// OLE errors / prompt strings
#define AFX_IDP_STATIC_OBJECT           0xF180
#define AFX_IDP_FAILED_TO_CONNECT       0xF181
#define AFX_IDP_SERVER_BUSY             0xF182
#define AFX_IDP_BAD_VERB                0xF183
#define AFX_IDS_NOT_DOCOBJECT                0xF184
#define AFX_IDP_FAILED_TO_NOTIFY        0xF185
#define AFX_IDP_FAILED_TO_LAUNCH        0xF186
#define AFX_IDP_ASK_TO_UPDATE           0xF187
#define AFX_IDP_FAILED_TO_UPDATE        0xF188
#define AFX_IDP_FAILED_TO_REGISTER      0xF189
#define AFX_IDP_FAILED_TO_AUTO_REGISTER 0xF18A
#define AFX_IDP_FAILED_TO_CONVERT       0xF18B
#define AFX_IDP_GET_NOT_SUPPORTED       0xF18C
#define AFX_IDP_SET_NOT_SUPPORTED       0xF18D
#define AFX_IDP_ASK_TO_DISCARD          0xF18E
#define AFX_IDP_FAILED_TO_CREATE        0xF18F
// MAPI errors / prompt strings
#define AFX_IDP_FAILED_MAPI_LOAD        0xF190
#define AFX_IDP_INVALID_MAPI_DLL        0xF191
#define AFX_IDP_FAILED_MAPI_SEND        0xF192
#define AFX_IDP_FILE_NONE               0xF1A0
#define AFX_IDP_FILE_GENERIC            0xF1A1
#define AFX_IDP_FILE_NOT_FOUND          0xF1A2
#define AFX_IDP_FILE_BAD_PATH           0xF1A3
#define AFX_IDP_FILE_TOO_MANY_OPEN      0xF1A4
#define AFX_IDP_FILE_ACCESS_DENIED      0xF1A5
#define AFX_IDP_FILE_INVALID_FILE       0xF1A6
#define AFX_IDP_FILE_REMOVE_CURRENT     0xF1A7
#define AFX_IDP_FILE_DIR_FULL           0xF1A8
#define AFX_IDP_FILE_BAD_SEEK           0xF1A9
#define AFX_IDP_FILE_HARD_IO            0xF1AA
#define AFX_IDP_FILE_SHARING            0xF1AB
#define AFX_IDP_FILE_LOCKING            0xF1AC
#define AFX_IDP_FILE_DISKFULL           0xF1AD
#define AFX_IDP_FILE_EOF                0xF1AE
#define AFX_IDP_ARCH_NONE               0xF1B0
#define AFX_IDP_ARCH_GENERIC            0xF1B1
#define AFX_IDP_ARCH_READONLY           0xF1B2
#define AFX_IDP_ARCH_ENDOFFILE          0xF1B3
#define AFX_IDP_ARCH_WRITEONLY          0xF1B4
#define AFX_IDP_ARCH_BADINDEX           0xF1B5
#define AFX_IDP_ARCH_BADCLASS           0xF1B6
#define AFX_IDP_ARCH_BADSCHEMA          0xF1B7
#define AFX_IDS_OCC_SCALEUNITS_PIXELS   0xF1C0
// 0xf200-0xf20f reserved
// font names and point sizes
#define AFX_IDS_STATUS_FONT             0xF230
#define AFX_IDS_TOOLTIP_FONT            0xF231
#define AFX_IDS_UNICODE_FONT            0xF232
#define AFX_IDS_MINI_FONT               0xF233
// ODBC Database errors / prompt strings
#ifndef RC_INVOKED      // code only
#define AFX_IDP_SQL_FIRST                       0xF280
#endif //!RC_INVOKED
#define AFX_IDP_SQL_CONNECT_FAIL                0xF281
#define AFX_IDP_SQL_RECORDSET_FORWARD_ONLY      0xF282
#define AFX_IDP_SQL_EMPTY_COLUMN_LIST           0xF283
#define AFX_IDP_SQL_FIELD_SCHEMA_MISMATCH       0xF284
#define AFX_IDP_SQL_ILLEGAL_MODE                0xF285
#define AFX_IDP_SQL_MULTIPLE_ROWS_AFFECTED      0xF286
#define AFX_IDP_SQL_NO_CURRENT_RECORD           0xF287
#define AFX_IDP_SQL_NO_ROWS_AFFECTED            0xF288
#define AFX_IDP_SQL_RECORDSET_READONLY          0xF289
#define AFX_IDP_SQL_SQL_NO_TOTAL                0xF28A
#define AFX_IDP_SQL_ODBC_LOAD_FAILED            0xF28B
#define AFX_IDP_SQL_DYNASET_NOT_SUPPORTED       0xF28C
#define AFX_IDP_SQL_SNAPSHOT_NOT_SUPPORTED      0xF28D
#define AFX_IDP_SQL_API_CONFORMANCE             0xF28E
#define AFX_IDP_SQL_SQL_CONFORMANCE             0xF28F
#define AFX_IDP_SQL_NO_DATA_FOUND               0xF290
#define AFX_IDP_SQL_ROW_UPDATE_NOT_SUPPORTED    0xF291
#define AFX_IDP_SQL_ODBC_V2_REQUIRED            0xF292
#define AFX_IDP_SQL_NO_POSITIONED_UPDATES       0xF293
#define AFX_IDP_SQL_LOCK_MODE_NOT_SUPPORTED     0xF294
#define AFX_IDP_SQL_DATA_TRUNCATED              0xF295
#define AFX_IDP_SQL_ROW_FETCH                   0xF296
#define AFX_IDP_SQL_INCORRECT_ODBC              0xF297
#define AFX_IDP_SQL_UPDATE_DELETE_FAILED        0xF298
#define AFX_IDP_SQL_DYNAMIC_CURSOR_NOT_SUPPORTED    0xF299
#define AFX_IDP_SQL_FIELD_NOT_FOUND             0xF29A
#define AFX_IDP_SQL_BOOKMARKS_NOT_SUPPORTED     0xF29B
#define AFX_IDP_SQL_BOOKMARKS_NOT_ENABLED       0xF29C
// ODBC Database strings
#define AFX_IDS_DELETED                         0xF29D
// DAO Database errors / prompt strings
#ifndef RC_INVOKED      // code only
#define AFX_IDP_DAO_FIRST                       0xF2B0
#endif //!RC_INVOKED
#define AFX_IDP_DAO_ENGINE_INITIALIZATION       0xF2B0
#define AFX_IDP_DAO_DFX_BIND                    0xF2B1
#define AFX_IDP_DAO_OBJECT_NOT_OPEN             0xF2B2
// ICDAORecordset::GetRows Errors
//  These are not placed in DAO Errors collection
//  and must be handled directly by MFC.
#define AFX_IDP_DAO_ROWTOOSHORT                 0xF2B3
#define AFX_IDP_DAO_BADBINDINFO                 0xF2B4
#define AFX_IDP_DAO_COLUMNUNAVAILABLE           0xF2B5
/////////////////////////////////////////////////////////////////////////////
// Strings for ISAPI support
#define AFX_IDS_HTTP_TITLE              0xF2D1
#define AFX_IDS_HTTP_NO_TEXT            0xF2D2
#define AFX_IDS_HTTP_BAD_REQUEST        0xF2D3
#define AFX_IDS_HTTP_AUTH_REQUIRED      0xF2D4
#define AFX_IDS_HTTP_FORBIDDEN          0xF2D5
#define AFX_IDS_HTTP_NOT_FOUND          0xF2D6
#define AFX_IDS_HTTP_SERVER_ERROR       0xF2D7
#define AFX_IDS_HTTP_NOT_IMPLEMENTED    0xF2D8
/////////////////////////////////////////////////////////////////////////////
// AFX implementation - control IDs (AFX_IDC)
// Parts of dialogs
#define AFX_IDC_LISTBOX                 100
#define AFX_IDC_CHANGE                  101
// for print dialog
#define AFX_IDC_PRINT_DOCNAME           201
#define AFX_IDC_PRINT_PRINTERNAME       202
#define AFX_IDC_PRINT_PORTNAME          203
#define AFX_IDC_PRINT_PAGENUM           204
// Property Sheet control id's (determined with Spy++)
#define ID_APPLY_NOW                    0x3021
#define ID_WIZBACK                      0x3023
#define ID_WIZNEXT                      0x3024
#define ID_WIZFINISH                    0x3025
#define AFX_IDC_TAB_CONTROL             0x3020
/////////////////////////////////////////////////////////////////////////////
// IDRs for standard components
#ifndef RC_INVOKED  // code only
// These are really COMMDLG dialogs, so there usually isn't a resource
// for them, but these IDs are used as help IDs.
#define AFX_IDD_FILEOPEN                28676
#define AFX_IDD_FILESAVE                28677
#define AFX_IDD_FONT                    28678
#define AFX_IDD_COLOR                   28679
#define AFX_IDD_PRINT                   28680
#define AFX_IDD_PRINTSETUP              28681
#define AFX_IDD_FIND                    28682
#define AFX_IDD_REPLACE                 28683
#endif //!RC_INVOKED
// Standard dialogs app should leave alone (0x7801->)
#define AFX_IDD_NEWTYPEDLG              30721
#define AFX_IDD_PRINTDLG                30722
#define AFX_IDD_PREVIEW_TOOLBAR         30723
// Dialogs defined for OLE2UI library
#define AFX_IDD_INSERTOBJECT            30724
#define AFX_IDD_CHANGEICON              30725
#define AFX_IDD_CONVERT                 30726
#define AFX_IDD_PASTESPECIAL            30727
#define AFX_IDD_EDITLINKS               30728
#define AFX_IDD_FILEBROWSE              30729
#define AFX_IDD_BUSY                    30730
#define AFX_IDD_OBJECTPROPERTIES        30732
#define AFX_IDD_CHANGESOURCE            30733
// Standard cursors (0x7901->)
// AFX_IDC = Cursor resources
#define AFX_IDC_CONTEXTHELP             30977       // context sensitive help
#define AFX_IDC_MAGNIFY                 30978       // print preview zoom
#define AFX_IDC_SMALLARROWS             30979       // splitter
#define AFX_IDC_HSPLITBAR               30980       // splitter
#define AFX_IDC_VSPLITBAR               30981       // splitter
#define AFX_IDC_NODROPCRSR              30982       // No Drop Cursor
#define AFX_IDC_TRACKNWSE               30983       // tracker
#define AFX_IDC_TRACKNESW               30984       // tracker
#define AFX_IDC_TRACKNS                 30985       // tracker
#define AFX_IDC_TRACKWE                 30986       // tracker
#define AFX_IDC_TRACK4WAY               30987       // tracker
#define AFX_IDC_MOVE4WAY                30988       // resize bar (server only)
// Mini frame window bitmap ID
#define AFX_IDB_MINIFRAME_MENU          30994
// CheckListBox checks bitmap ID
#define AFX_IDB_CHECKLISTBOX_NT         30995
#define AFX_IDB_CHECKLISTBOX_95         30996
// AFX standard accelerator resources
#define AFX_IDR_PREVIEW_ACCEL           30997
// AFX standard ICON IDs (for MFC V1 apps) (0x7A01->)
#define AFX_IDI_STD_MDIFRAME            31233
#define AFX_IDI_STD_FRAME               31234
/////////////////////////////////////////////////////////////////////////////
// AFX OLE control implementation - control IDs (AFX_IDC)
// Font property page
#define AFX_IDC_FONTPROP                1000
#define AFX_IDC_FONTNAMES               1001
#define AFX_IDC_FONTSTYLES              1002
#define AFX_IDC_FONTSIZES               1003
#define AFX_IDC_STRIKEOUT               1004
#define AFX_IDC_UNDERLINE               1005
#define AFX_IDC_SAMPLEBOX               1006
// Color property page
#define AFX_IDC_COLOR_BLACK             1100
#define AFX_IDC_COLOR_WHITE             1101
#define AFX_IDC_COLOR_RED               1102
#define AFX_IDC_COLOR_GREEN             1103
#define AFX_IDC_COLOR_BLUE              1104
#define AFX_IDC_COLOR_YELLOW            1105
#define AFX_IDC_COLOR_MAGENTA           1106
#define AFX_IDC_COLOR_CYAN              1107
#define AFX_IDC_COLOR_GRAY              1108
#define AFX_IDC_COLOR_LIGHTGRAY         1109
#define AFX_IDC_COLOR_DARKRED           1110
#define AFX_IDC_COLOR_DARKGREEN         1111
#define AFX_IDC_COLOR_DARKBLUE          1112
#define AFX_IDC_COLOR_LIGHTBROWN        1113
#define AFX_IDC_COLOR_DARKMAGENTA       1114
#define AFX_IDC_COLOR_DARKCYAN          1115
#define AFX_IDC_COLORPROP               1116
#define AFX_IDC_SYSTEMCOLORS            1117
// Picture porperty page
#define AFX_IDC_PROPNAME                1201
#define AFX_IDC_PICTURE                 1202
#define AFX_IDC_BROWSE                  1203
#define AFX_IDC_CLEAR                   1204
/////////////////////////////////////////////////////////////////////////////
// IDRs for OLE control standard components
// Standard propery page dialogs app should leave alone (0x7E01->)
#define AFX_IDD_PROPPAGE_COLOR         32257
#define AFX_IDD_PROPPAGE_FONT          32258
#define AFX_IDD_PROPPAGE_PICTURE       32259
#define AFX_IDB_TRUETYPE               32384
/////////////////////////////////////////////////////////////////////////////
// Standard OLE control strings
// OLE Control page strings
#define AFX_IDS_PROPPAGE_UNKNOWN        0xFE01
#define AFX_IDS_COLOR_DESKTOP           0xFE04
#define AFX_IDS_COLOR_APPWORKSPACE      0xFE05
#define AFX_IDS_COLOR_WNDBACKGND        0xFE06
#define AFX_IDS_COLOR_WNDTEXT           0xFE07
#define AFX_IDS_COLOR_MENUBAR           0xFE08
#define AFX_IDS_COLOR_MENUTEXT          0xFE09
#define AFX_IDS_COLOR_ACTIVEBAR         0xFE0A
#define AFX_IDS_COLOR_INACTIVEBAR       0xFE0B
#define AFX_IDS_COLOR_ACTIVETEXT        0xFE0C
#define AFX_IDS_COLOR_INACTIVETEXT      0xFE0D
#define AFX_IDS_COLOR_ACTIVEBORDER      0xFE0E
#define AFX_IDS_COLOR_INACTIVEBORDER    0xFE0F
#define AFX_IDS_COLOR_WNDFRAME          0xFE10
#define AFX_IDS_COLOR_SCROLLBARS        0xFE11
#define AFX_IDS_COLOR_BTNFACE           0xFE12
#define AFX_IDS_COLOR_BTNSHADOW         0xFE13
#define AFX_IDS_COLOR_BTNTEXT           0xFE14
#define AFX_IDS_COLOR_BTNHIGHLIGHT      0xFE15
#define AFX_IDS_COLOR_DISABLEDTEXT      0xFE16
#define AFX_IDS_COLOR_HIGHLIGHT         0xFE17
#define AFX_IDS_COLOR_HIGHLIGHTTEXT     0xFE18
#define AFX_IDS_REGULAR                 0xFE19
#define AFX_IDS_BOLD                    0xFE1A
#define AFX_IDS_ITALIC                  0xFE1B
#define AFX_IDS_BOLDITALIC              0xFE1C
#define AFX_IDS_SAMPLETEXT              0xFE1D
#define AFX_IDS_DISPLAYSTRING_FONT      0xFE1E
#define AFX_IDS_DISPLAYSTRING_COLOR     0xFE1F
#define AFX_IDS_DISPLAYSTRING_PICTURE   0xFE20
#define AFX_IDS_PICTUREFILTER           0xFE21
#define AFX_IDS_PICTYPE_UNKNOWN         0xFE22
#define AFX_IDS_PICTYPE_NONE            0xFE23
#define AFX_IDS_PICTYPE_BITMAP          0xFE24
#define AFX_IDS_PICTYPE_METAFILE        0xFE25
#define AFX_IDS_PICTYPE_ICON            0xFE26
#define AFX_IDS_COLOR_PPG               0xFE28
#define AFX_IDS_COLOR_PPG_CAPTION       0xFE29
#define AFX_IDS_FONT_PPG                0xFE2A
#define AFX_IDS_FONT_PPG_CAPTION        0xFE2B
#define AFX_IDS_PICTURE_PPG             0xFE2C
#define AFX_IDS_PICTURE_PPG_CAPTION     0xFE2D
#define AFX_IDS_PICTUREBROWSETITLE      0xFE30
#define AFX_IDS_BORDERSTYLE_0           0xFE31
#define AFX_IDS_BORDERSTYLE_1           0xFE32
// OLE Control verb names
#define AFX_IDS_VERB_EDIT               0xFE40
#define AFX_IDS_VERB_PROPERTIES         0xFE41
// OLE Control internal error messages
#define AFX_IDP_PICTURECANTOPEN         0xFE83
#define AFX_IDP_PICTURECANTLOAD         0xFE84
#define AFX_IDP_PICTURETOOLARGE         0xFE85
#define AFX_IDP_PICTUREREADFAILED       0xFE86
// Standard OLE Control error strings
#define AFX_IDP_E_ILLEGALFUNCTIONCALL       0xFEA0
#define AFX_IDP_E_OVERFLOW                  0xFEA1
#define AFX_IDP_E_OUTOFMEMORY               0xFEA2
#define AFX_IDP_E_DIVISIONBYZERO            0xFEA3
#define AFX_IDP_E_OUTOFSTRINGSPACE          0xFEA4
#define AFX_IDP_E_OUTOFSTACKSPACE           0xFEA5
#define AFX_IDP_E_BADFILENAMEORNUMBER       0xFEA6
#define AFX_IDP_E_FILENOTFOUND              0xFEA7
#define AFX_IDP_E_BADFILEMODE               0xFEA8
#define AFX_IDP_E_FILEALREADYOPEN           0xFEA9
#define AFX_IDP_E_DEVICEIOERROR             0xFEAA
#define AFX_IDP_E_FILEALREADYEXISTS         0xFEAB
#define AFX_IDP_E_BADRECORDLENGTH           0xFEAC
#define AFX_IDP_E_DISKFULL                  0xFEAD
#define AFX_IDP_E_BADRECORDNUMBER           0xFEAE
#define AFX_IDP_E_BADFILENAME               0xFEAF
#define AFX_IDP_E_TOOMANYFILES              0xFEB0
#define AFX_IDP_E_DEVICEUNAVAILABLE         0xFEB1
#define AFX_IDP_E_PERMISSIONDENIED          0xFEB2
#define AFX_IDP_E_DISKNOTREADY              0xFEB3
#define AFX_IDP_E_PATHFILEACCESSERROR       0xFEB4
#define AFX_IDP_E_PATHNOTFOUND              0xFEB5
#define AFX_IDP_E_INVALIDPATTERNSTRING      0xFEB6
#define AFX_IDP_E_INVALIDUSEOFNULL          0xFEB7
#define AFX_IDP_E_INVALIDFILEFORMAT         0xFEB8
#define AFX_IDP_E_INVALIDPROPERTYVALUE      0xFEB9
#define AFX_IDP_E_INVALIDPROPERTYARRAYINDEX 0xFEBA
#define AFX_IDP_E_SETNOTSUPPORTEDATRUNTIME  0xFEBB
#define AFX_IDP_E_SETNOTSUPPORTED           0xFEBC
#define AFX_IDP_E_NEEDPROPERTYARRAYINDEX    0xFEBD
#define AFX_IDP_E_SETNOTPERMITTED           0xFEBE
#define AFX_IDP_E_GETNOTSUPPORTEDATRUNTIME  0xFEBF
#define AFX_IDP_E_GETNOTSUPPORTED           0xFEC0
#define AFX_IDP_E_PROPERTYNOTFOUND          0xFEC1
#define AFX_IDP_E_INVALIDCLIPBOARDFORMAT    0xFEC2
#define AFX_IDP_E_INVALIDPICTURE            0xFEC3
#define AFX_IDP_E_PRINTERERROR              0xFEC4
#define AFX_IDP_E_CANTSAVEFILETOTEMP        0xFEC5
#define AFX_IDP_E_SEARCHTEXTNOTFOUND        0xFEC6
#define AFX_IDP_E_REPLACEMENTSTOOLONG       0xFEC7
/////////////////////////////////////////////////////////////////////////////

/////////////////////////////////////////////////////////////////////////////
#ifndef SIZE_T_MAX
#define SIZE_T_MAX  UINT_MAX
#endif
#define AFX_CORE_DATA
#define AFX_DATA
#define AFX_DATADEF
#define AFX_API

// Implementation structures
struct AFX_SIZEPARENTPARAMS;    // Control bar implementation
struct AFX_CMDHANDLERINFO;      // Command routing implementation

// Classes declared in this file

	//CDC
		class CPreviewDC;               // Virtual DC for print preview

	//CCmdTarget
		//CWnd
			//CView
				class CPreviewView;     // Print preview view
		//CFrameWnd
			class COleCntrFrameWnd;
			//CMiniFrameWnd
				class CMiniDockFrameWnd;

class CDockContext;                     // for dragging control bars

/////////////////////////////////////////////////////////////////////////////

#undef AFX_DATA
#define AFX_DATA AFX_CORE_DATA

/////////////////////////////////////////////////////////////////////////////
// Global ID ranges (see Technical note TN020 for more details)

// 8000 -> FFFF command IDs (used for menu items, accelerators and controls)
#define IS_COMMAND_ID(nID)  ((nID) & 0x8000)

// 8000 -> DFFF : user commands
// E000 -> EFFF : AFX commands and other things
// F000 -> FFFF : standard windows commands and other things etc
	// E000 -> E7FF standard commands
	// E800 -> E8FF control bars (first 32 are special)
	// E900 -> EEFF standard window controls/components
	// EF00 -> EFFF SC_ menu help
	// F000 -> FFFF standard strings
#define ID_COMMAND_FROM_SC(sc)  (((sc - 0xF000) >> 4) + AFX_IDS_SCFIRST)

// 0000 -> 7FFF IDR range
// 0000 -> 6FFF : user resources
// 7000 -> 7FFF : AFX (and standard windows) resources
// IDR ranges (NOTE: IDR_ values must be <32768)
#define ASSERT_VALID_IDR(nIDR) ASSERT((nIDR) != 0 && (nIDR) < 0x8000)

/////////////////////////////////////////////////////////////////////////////
// Context sensitive help support (see Technical note TN028 for more details)

// Help ID bases
#define HID_BASE_COMMAND    0x00010000UL        // ID and IDM
#define HID_BASE_RESOURCE   0x00020000UL        // IDR and IDD
#define HID_BASE_PROMPT     0x00030000UL        // IDP
#define HID_BASE_NCAREAS    0x00040000UL
#define HID_BASE_CONTROL    0x00050000UL        // IDC
#define HID_BASE_DISPATCH   0x00060000UL        // IDispatch help codes

/////////////////////////////////////////////////////////////////////////////
// Internal AFX Windows messages (see Technical note TN024 for more details)
// (0x0360 - 0x037F are reserved for MFC)

#define WM_QUERYAFXWNDPROC  0x0360  // lResult = 1 if processed by AfxWndProc
#define WM_SIZEPARENT       0x0361  // lParam = &AFX_SIZEPARENTPARAMS
#define WM_SETMESSAGESTRING 0x0362  // wParam = nIDS (or 0),
									// lParam = lpszOther (or NULL)
#define WM_IDLEUPDATECMDUI  0x0363  // wParam == bDisableIfNoHandler
#define WM_INITIALUPDATE    0x0364  // (params unused) - sent to children
#define WM_COMMANDHELP      0x0365  // lResult = TRUE/FALSE,
									// lParam = dwContext
#define WM_HELPHITTEST      0x0366  // lResult = dwContext,
									// lParam = MAKELONG(x,y)
#define WM_EXITHELPMODE     0x0367  // (params unused)
#define WM_RECALCPARENT     0x0368  // force RecalcLayout on frame window
									//  (only for inplace frame windows)
#define WM_SIZECHILD        0x0369  // special notify from COleResizeBar
									// wParam = ID of child window
									// lParam = lpRectNew (new position/size)
#define WM_KICKIDLE         0x036A  // (params unused) causes idles to kick in
#define WM_QUERYCENTERWND   0x036B  // lParam = HWND to use as centering parent
#define WM_DISABLEMODAL     0x036C  // lResult = 0, disable during modal state
									// lResult = 1, don't disable
#define WM_FLOATSTATUS      0x036D  // wParam combination of FS_* flags below

// WM_ACTIVATETOPLEVEL is like WM_ACTIVATEAPP but works with hierarchies
//   of mixed processes (as is the case with OLE in-place activation)
#define WM_ACTIVATETOPLEVEL 0x036E  // wParam = nState (like WM_ACTIVATE)
									// lParam = pointer to HWND[2]
									//  lParam[0] = hWnd getting WM_ACTIVATE
									//  lParam[1] = hWndOther

#define WM_QUERY3DCONTROLS  0x036F  // lResult != 0 if 3D controls wanted

// Note: Messages 0x0370, 0x0371, and 0x372 were incorrectly used by
//  some versions of Windows.  To remain compatible, MFC does not
//  use messages in that range.
#define WM_RESERVED_0370    0x0370
#define WM_RESERVED_0371    0x0371
#define WM_RESERVED_0372    0x0372

// WM_SOCKET_NOTIFY and WM_SOCKET_DEAD are used internally by MFC's
// Windows sockets implementation.  For more information, see sockcore.cpp
#define WM_SOCKET_NOTIFY    0x0373
#define WM_SOCKET_DEAD      0x0374

// same as WM_SETMESSAGESTRING except not popped if IsTracking()
#define WM_POPMESSAGESTRING 0x0375

// WM_HELPPROMPTADDR is used internally to get the address of
//  m_dwPromptContext from the associated frame window. This is used
//  during message boxes to setup for F1 help while that msg box is
//  displayed. lResult is the address of m_dwPromptContext.
#define WM_HELPPROMPTADDR   0x0376

// Constants used in DLGINIT resources for OLE control containers
// NOTE: These are NOT real Windows messages they are simply tags
// used in the control resource and are never used as 'messages'
#define WM_OCC_LOADFROMSTREAM           0x0376
#define WM_OCC_LOADFROMSTORAGE          0x0377
#define WM_OCC_INITNEW                  0x0378
#define WM_OCC_LOADFROMSTREAM_EX        0x037A
#define WM_OCC_LOADFROMSTORAGE_EX       0x037B

// Marker used while rearranging the message queue
#define WM_QUEUE_SENTINEL   0x0379

// Note: Messages 0x037C - 0x37E reserved for future MFC use.
#define WM_RESERVED_037C    0x037C
#define WM_RESERVED_037D    0x037D
#define WM_RESERVED_037E    0x037E

// WM_FORWARDMSG - used by ATL to forward a message to another window for processing
//  WPARAM - DWORD dwUserData - defined by user
//  LPARAM - LPMSG pMsg - a pointer to the MSG structure
//  return value - 0 if the message was not processed, nonzero if it was
#define WM_FORWARDMSG       0x037F

// like ON_MESSAGE but no return value
#define ON_MESSAGE_VOID(message, memberFxn) \
	{ message, 0, 0, 0, AfxSig_vv, \
		(AFX_PMSG)(AFX_PMSGW)(void (AFX_MSG_CALL CWnd::*)(void))&memberFxn },


// PASCAL is used for static member functions
#ifndef PASCAL
#define PASCAL  __stdcall
#endif
// FASTCALL is used for static member functions with little or no params
#ifndef FASTCALL
#define FASTCALL __fastcall
#endif
// CDECL and EXPORT are defined in case WINDOWS.H doesn't
#ifndef CDECL
#define CDECL __cdecl
#endif
#ifndef EXPORT
#define EXPORT
#endif
// UNALIGNED is used for unaligned data access (in CArchive mostly)
#ifndef UNALIGNED
#define UNALIGNED
#endif
// AFXAPI is used on global public functions
#ifndef AFXAPI
#define AFXAPI __stdcall
#endif
// AFXOLEAPI is used for some special OLE functions
#ifndef AFXOLEAPI
#define AFXOLEAPI __stdcall
#endif
// AFX_CDECL is used for rare functions taking variable arguments
#ifndef AFX_CDECL
#define AFX_CDECL __cdecl
#endif
// AFX_EXPORT is used for functions which need to be exported
#ifndef AFX_EXPORT
#define AFX_EXPORT EXPORT
#endif
#ifndef AFX_STATIC
#define AFX_STATIC extern
#define AFX_STATIC_DATA extern __declspec(selectany)
#endif
// The following macros are used to enable export/import
// for data
#ifndef AFX_DATA_EXPORT
#define AFX_DATA_EXPORT __declspec(dllexport)
#endif
#ifndef AFX_DATA_IMPORT
#define AFX_DATA_IMPORT __declspec(dllimport)
#endif
// for classes
#ifndef AFX_CLASS_EXPORT
#define AFX_CLASS_EXPORT __declspec(dllexport)
#endif
#ifndef AFX_CLASS_IMPORT
#define AFX_CLASS_IMPORT __declspec(dllimport)
#endif
// for global APIs
#ifndef AFX_API_EXPORT
#define AFX_API_EXPORT __declspec(dllexport)
#endif
#ifndef AFX_API_IMPORT
#define AFX_API_IMPORT __declspec(dllimport)
#endif


/////////////////////////////////////////////////////////////////////////////
struct CRuntimeClass;          // object type information
class CObject;                        // the root of all objects classes
class CException;                 // the root of all exceptions
class CArchiveException;      // archive exception
class CFileException;         // file exception
class CSimpleException;
class CMemoryException;       // out-of-memory exception
class CNotSupportedException; // feature not supported exception
class CFile;                      // raw binary file
class CStdioFile;             // buffered stdio text/binary file
class CMemFile;               // memory based file
// Non CObject classes
class CString;                        // growable string type
class CTimeSpan;                      // time/date difference
class CTime;                          // absolute time/date
struct CFileStatus;                   // file status information
struct CMemoryState;                  // diagnostic memory support
class CArchive;                       // object persistence tool
class CDumpContext;                   // object diagnostic dumping


// abstract iteration position
struct __POSITION { };
typedef __POSITION* POSITION;
struct _AFX_DOUBLE  {
  BYTE doubleBits[sizeof(double)];
};
struct _AFX_FLOAT   {
  BYTE floatBits[sizeof(float)];
};
// Standard constants
#undef FALSE
#undef TRUE
#undef NULL
#define FALSE   0
#define TRUE    1
#define NULL    0
#ifdef _DEBUG
BOOL AFXAPI AfxAssertFailedLine(LPCSTR lpszFileName, int nLine);
void AFX_CDECL AfxTrace(LPCTSTR lpszFormat, ...);
// Note: file names are still ANSI strings (filenames rarely need UNICODE)
void AFXAPI AfxAssertValidObject(const CObject* pOb,
    LPCSTR lpszFileName, int nLine);
void AFXAPI AfxDump(const CObject* pOb); // Dump an object from CodeView
#define THIS_FILE          __FILE__
//#define TRACE              ::AfxTrace
//#define ASSERT(f)   do { if (!(f) && AfxAssertFailedLine(THIS_FILE, __LINE__)) AfxDebugBreak();  } while (0)
//#define VERIFY(f)          ASSERT(f)
#define ASSERT_VALID(pOb)  (::AfxAssertValidObject(pOb, THIS_FILE, __LINE__))
#define DEBUG_ONLY(f)      (f)
// The following trace macros are provided for backward compatiblity
//  (they also take a fixed number of parameters which provides
//   some amount of extra error checking)
#define TRACE0(sz)              ::AfxTrace(_T("%s"), _T(sz))
#define TRACE1(sz, p1)          ::AfxTrace(_T(sz), p1)
#define TRACE2(sz, p1, p2)      ::AfxTrace(_T(sz), p1, p2)
#define TRACE3(sz, p1, p2, p3)  ::AfxTrace(_T(sz), p1, p2, p3)
// These AFX_DUMP macros also provided for backward compatibility
#define AFX_DUMP0(dc, sz)   dc << _T(sz)
#define AFX_DUMP1(dc, sz, p1) dc << _T(sz) << p1
#else   // _DEBUG
#define ASSERT(f)          ((void)0)
#define VERIFY(f)          ((void)(f))
#define ASSERT_VALID(pOb)  ((void)0)
#define DEBUG_ONLY(f)      ((void)0)
inline void AFX_CDECL AfxTrace(LPCTSTR, ...) { }
#define TRACE              1 ? (void)0 : ::AfxTrace
#define TRACE0(sz)
#define TRACE1(sz, p1)
#define TRACE2(sz, p1, p2)
#define TRACE3(sz, p1, p2, p3)
#endif // !_DEBUG
#define ASSERT_POINTER(p, type)   ASSERT(((p) != NULL) && AfxIsValidAddress((p), sizeof(type), FALSE))
#define ASSERT_NULL_OR_POINTER(p, type)   ASSERT(((p) == NULL) || AfxIsValidAddress((p), sizeof(type), FALSE))
#undef UNUSED
#ifdef _DEBUG
#define UNUSED(x)
#else
#define UNUSED(x) x
#endif
#define UNUSED_ALWAYS(x) x
#define BEFORE_START_POSITION ((POSITION)-1L)
#define _AFX     1      // Microsoft Application Framework Classes
#ifndef _MFC_VER
#define _MFC_VER 0x0600 // Microsoft Foundation Classes version 6.00, VC++ 6.0
#endif
BOOL AFXAPI AfxInitialize(BOOL bDLL = FALSE, DWORD dwVersion = _MFC_VER);
/////////////////////////////////////////////////////////////////////////////
// Basic object model
struct CRuntimeClass {
  // Attributes
  LPCSTR m_lpszClassName;
  int m_nObjectSize;
  UINT m_wSchema; // schema number of the loaded class
  CObject* (PASCAL* m_pfnCreateObject)(); // NULL => abstract class
#ifdef _AFXDLL
  CRuntimeClass* (PASCAL* m_pfnGetBaseClass)();
#else
  CRuntimeClass* m_pBaseClass;
#endif
  // Operations
  CObject* CreateObject();
  BOOL IsDerivedFrom(const CRuntimeClass* pBaseClass) const;
  // Implementation
  void Store(CArchive& ar) const;
  static CRuntimeClass* PASCAL Load(CArchive& ar, UINT* pwSchemaNum);
  // CRuntimeClass objects linked together in simple list
  CRuntimeClass* m_pNextClass;       // linked list of registered classes
};
/////////////////////////////////////////////////////////////////////////////
// Strings
#ifndef _OLEAUTO_H_
#ifdef OLE2ANSI
typedef LPSTR BSTR;
#else
typedef LPWSTR BSTR;// must (semantically) match typedef in oleauto.h
#endif
#endif
struct CStringData {
  long nRefs;             // reference count
  int nDataLength;        // length of data (including terminator)
  int nAllocLength;       // length of allocation
  // TCHAR data[nAllocLength]
  TCHAR* data() {         // TCHAR* to managed data
    return (TCHAR*)(this + 1);
  }
};
class CString
{
public:
  // Constructors
  // constructs empty CString
  CString();
  // copy constructor
  CString(const CString& stringSrc);
  // from a single character
  CString(TCHAR ch, int nRepeat = 1);
  // from an ANSI string (converts to TCHAR)
  CString(LPCSTR lpsz);
  // from a UNICODE string (converts to TCHAR)
  CString(LPCWSTR lpsz);
  // subset of characters from an ANSI string (converts to TCHAR)
  CString(LPCSTR lpch, int nLength);
  // subset of characters from a UNICODE string (converts to TCHAR)
  CString(LPCWSTR lpch, int nLength);
  // from unsigned characters
  CString(const unsigned char* psz);
  // Attributes & Operations
  // get data length
  int GetLength() const;
  // TRUE if zero length
  BOOL IsEmpty() const;
  // clear contents to empty
  void Empty();
  // return single character at zero-based index
  TCHAR GetAt(int nIndex) const;
  // return single character at zero-based index
  TCHAR operator[](int nIndex) const;
  // set a single character at zero-based index
  void SetAt(int nIndex, TCHAR ch);
  // return pointer to const string
  operator LPCTSTR() const;
  // overloaded assignment
  // ref-counted copy from another CString
  const CString& operator=(const CString& stringSrc);
  // set string content to single character
  const CString& operator=(TCHAR ch);
#ifdef _UNICODE
  const CString& operator=(char ch);
#endif
  // copy string content from ANSI string (converts to TCHAR)
  const CString& operator=(LPCSTR lpsz);
  // copy string content from UNICODE string (converts to TCHAR)
  const CString& operator=(LPCWSTR lpsz);
  // copy string content from unsigned chars
  const CString& operator=(const unsigned char* psz);
  // string concatenation
  // concatenate from another CString
  const CString& operator+=(const CString& string);
  // concatenate a single character
  const CString& operator+=(TCHAR ch);
#ifdef _UNICODE
  // concatenate an ANSI character after converting it to TCHAR
  const CString& operator+=(char ch);
#endif
  // concatenate a UNICODE character after converting it to TCHAR
  const CString& operator+=(LPCTSTR lpsz);
  friend CString AFXAPI operator+(const CString& string1,
      const CString& string2);
  friend CString AFXAPI operator+(const CString& string, TCHAR ch);
  friend CString AFXAPI operator+(TCHAR ch, const CString& string);
#ifdef _UNICODE
  friend CString AFXAPI operator+(const CString& string, char ch);
  friend CString AFXAPI operator+(char ch, const CString& string);
#endif
  friend CString AFXAPI operator+(const CString& string, LPCTSTR lpsz);
  friend CString AFXAPI operator+(LPCTSTR lpsz, const CString& string);
  // string comparison
  // straight character comparison
  int Compare(LPCTSTR lpsz) const;
  // compare ignoring case
  int CompareNoCase(LPCTSTR lpsz) const;
  // NLS aware comparison, case sensitive
  int Collate(LPCTSTR lpsz) const;
  // NLS aware comparison, case insensitive
  int CollateNoCase(LPCTSTR lpsz) const;
  // simple sub-string extraction
  // return nCount characters starting at zero-based nFirst
  CString Mid(int nFirst, int nCount) const;
  // return all characters starting at zero-based nFirst
  CString Mid(int nFirst) const;
  // return first nCount characters in string
  CString Left(int nCount) const;
  // return nCount characters from end of string
  CString Right(int nCount) const;
  //  characters from beginning that are also in passed string
  CString SpanIncluding(LPCTSTR lpszCharSet) const;
  // characters from beginning that are not also in passed string
  CString SpanExcluding(LPCTSTR lpszCharSet) const;
  // upper/lower/reverse conversion
  // NLS aware conversion to uppercase
  void MakeUpper();
  // NLS aware conversion to lowercase
  void MakeLower();
  // reverse string right-to-left
  void MakeReverse();
  // trimming whitespace (either side)
  // remove whitespace starting from right edge
  void TrimRight();
  // remove whitespace starting from left side
  void TrimLeft();
  // trimming anything (either side)
  // remove continuous occurrences of chTarget starting from right
  void TrimRight(TCHAR chTarget);
  // remove continuous occcurrences of characters in passed string,
  // starting from right
  void TrimRight(LPCTSTR lpszTargets);
  // remove continuous occurrences of chTarget starting from left
  void TrimLeft(TCHAR chTarget);
  // remove continuous occcurrences of characters in
  // passed string, starting from left
  void TrimLeft(LPCTSTR lpszTargets);
  // advanced manipulation
  // replace occurrences of chOld with chNew
  int Replace(TCHAR chOld, TCHAR chNew);
  // replace occurrences of substring lpszOld with lpszNew;
  // empty lpszNew removes instances of lpszOld
  int Replace(LPCTSTR lpszOld, LPCTSTR lpszNew);
  // remove occurrences of chRemove
  int Remove(TCHAR chRemove);
  // insert character at zero-based index; concatenates
  // if index is past end of string
  int Insert(int nIndex, TCHAR ch);
  // insert substring at zero-based index; concatenates
  // if index is past end of string
  int Insert(int nIndex, LPCTSTR pstr);
  // delete nCount characters starting at zero-based index
  int Delete(int nIndex, int nCount = 1);
  // searching
  // find character starting at left, -1 if not found
  int Find(TCHAR ch) const;
  // find character starting at right
  int ReverseFind(TCHAR ch) const;
  // find character starting at zero-based index and going right
  int Find(TCHAR ch, int nStart) const;
  // find first instance of any character in passed string
  int FindOneOf(LPCTSTR lpszCharSet) const;
  // find first instance of substring
  int Find(LPCTSTR lpszSub) const;
  // find first instance of substring starting at zero-based index
  int Find(LPCTSTR lpszSub, int nStart) const;
  // simple formatting
  // printf-like formatting using passed string
  void AFX_CDECL Format(LPCTSTR lpszFormat, ...);
  // printf-like formatting using referenced string resource
  void AFX_CDECL Format(UINT nFormatID, ...);
  // printf-like formatting using variable arguments parameter
  void FormatV(LPCTSTR lpszFormat, va_list argList);
  // formatting for localization (uses FormatMessage API)
  // format using FormatMessage API on passed string
  void AFX_CDECL FormatMessage(LPCTSTR lpszFormat, ...);
  // format using FormatMessage API on referenced string resource
  void AFX_CDECL FormatMessage(UINT nFormatID, ...);
  // input and output
#ifdef _DEBUG
  friend CDumpContext& AFXAPI operator<<(CDumpContext& dc,
      const CString& string);
#endif
  friend CArchive& AFXAPI operator<<(CArchive& ar, const CString& string);
  friend CArchive& AFXAPI operator>>(CArchive& ar, CString& string);
  // load from string resource
  BOOL LoadString(UINT nID);
#ifndef _UNICODE
  // ANSI <-> OEM support (convert string in place)
  // convert string from ANSI to OEM in-place
  void AnsiToOem();
  // convert string from OEM to ANSI in-place
  void OemToAnsi();
#endif
#ifndef _AFX_NO_BSTR_SUPPORT
  // OLE BSTR support (use for OLE automation)
  // return a BSTR initialized with this CString's data
  BSTR AllocSysString() const;
  // reallocates the passed BSTR, copies content of this CString to it
  BSTR SetSysString(BSTR* pbstr) const;
#endif
  // Access to string implementation buffer as "C" character array
  // get pointer to modifiable buffer at least as long as nMinBufLength
  LPTSTR GetBuffer(int nMinBufLength);
  // release buffer, setting length to nNewLength (or to first nul if -1)
  void ReleaseBuffer(int nNewLength = -1);
  // get pointer to modifiable buffer exactly as long as nNewLength
  LPTSTR GetBufferSetLength(int nNewLength);
  // release memory allocated to but unused by string
  void FreeExtra();
  // Use LockBuffer/UnlockBuffer to turn refcounting off
  // turn refcounting back on
  LPTSTR LockBuffer();
  // turn refcounting off
  void UnlockBuffer();
  // Implementation
public:
  ~CString();
  int GetAllocLength() const;
protected:
  LPTSTR m_pchData;   // pointer to ref counted string data
  // implementation helpers
  CStringData* GetData() const;
  void Init();
  void AllocCopy(CString& dest, int nCopyLen, int nCopyIndex, int nExtraLen) const;
  void AllocBuffer(int nLen);
  void AssignCopy(int nSrcLen, LPCTSTR lpszSrcData);
  void ConcatCopy(int nSrc1Len, LPCTSTR lpszSrc1Data, int nSrc2Len, LPCTSTR lpszSrc2Data);
  void ConcatInPlace(int nSrcLen, LPCTSTR lpszSrcData);
  void CopyBeforeWrite();
  void AllocBeforeWrite(int nLen);
  void Release();
  static void PASCAL Release(CStringData* pData);
  static int PASCAL SafeStrlen(LPCTSTR lpsz);
  static void FASTCALL FreeData(CStringData* pData);
};
// Compare helpers
bool AFXAPI operator==(const CString& s1, const CString& s2);
bool AFXAPI operator==(const CString& s1, LPCTSTR s2);
bool AFXAPI operator==(LPCTSTR s1, const CString& s2);
bool AFXAPI operator!=(const CString& s1, const CString& s2);
bool AFXAPI operator!=(const CString& s1, LPCTSTR s2);
bool AFXAPI operator!=(LPCTSTR s1, const CString& s2);
bool AFXAPI operator<(const CString& s1, const CString& s2);
bool AFXAPI operator<(const CString& s1, LPCTSTR s2);
bool AFXAPI operator<(LPCTSTR s1, const CString& s2);
bool AFXAPI operator>(const CString& s1, const CString& s2);
bool AFXAPI operator>(const CString& s1, LPCTSTR s2);
bool AFXAPI operator>(LPCTSTR s1, const CString& s2);
bool AFXAPI operator<=(const CString& s1, const CString& s2);
bool AFXAPI operator<=(const CString& s1, LPCTSTR s2);
bool AFXAPI operator<=(LPCTSTR s1, const CString& s2);
bool AFXAPI operator>=(const CString& s1, const CString& s2);
bool AFXAPI operator>=(const CString& s1, LPCTSTR s2);
bool AFXAPI operator>=(LPCTSTR s1, const CString& s2);
// conversion helpers
int AFX_CDECL _wcstombsz(char* mbstr, const wchar_t* wcstr, size_t count);
int AFX_CDECL _mbstowcsz(wchar_t* wcstr, const char* mbstr, size_t count);
// Globals
extern AFX_DATA TCHAR afxChNil;
#ifdef _AFXDLL
const CString& AFXAPI AfxGetEmptyString();
#define afxEmptyString AfxGetEmptyString()
#else
extern LPCTSTR _afxPchNil;
#define afxEmptyString ((CString&)*(CString*)&_afxPchNil)
#endif
/////////////////////////////////////////////////////////////////////////////
// class CObject is the root of all compliant objects
#define AFX_NOVTABLE
#ifdef _AFXDLL
class CObject
#else
class AFX_NOVTABLE CObject
#endif
{
public:
  // Object model (types, destruction, allocation)
  virtual CRuntimeClass* GetRuntimeClass() const;
  virtual ~CObject();  // virtual destructors are necessary
  // Diagnostic allocations
  void* PASCAL operator new(size_t nSize);
  void* PASCAL operator new(size_t, void* p);
  void PASCAL operator delete(void* p);
#if _MSC_VER >= 1200
  void PASCAL operator delete(void* p, void* pPlace);
#endif
#if defined(_DEBUG) && !defined(_AFX_NO_DEBUG_CRT)
  // for file name/line number tracking using DEBUG_NEW
  void* PASCAL operator new(size_t nSize, LPCSTR lpszFileName, int nLine);
#if _MSC_VER >= 1200
  void PASCAL operator delete(void* p, LPCSTR lpszFileName, int nLine);
#endif
#endif
  // Disable the copy constructor and assignment by default so you will get
  //   compiler errors instead of unexpected behaviour if you pass objects
  //   by value or assign objects.
protected:
  CObject();
private:
  CObject(const CObject& objectSrc);              // no implementation
  void operator=(const CObject& objectSrc);       // no implementation
  // Attributes
public:
  BOOL IsSerializable() const;
  BOOL IsKindOf(const CRuntimeClass* pClass) const;
  // Overridables
  virtual void Serialize(CArchive& ar);
#if defined(_DEBUG) || defined(_AFXDLL)
  // Diagnostic Support
  virtual void AssertValid() const;
  virtual void Dump(CDumpContext& dc) const;
#endif
  // Implementation
public:
  static const AFX_DATA CRuntimeClass classCObject;
#ifdef _AFXDLL
  static CRuntimeClass* PASCAL _GetBaseClass();
#endif
};
// Helper macros
#define RUNTIME_CLASS(class_name) ((CRuntimeClass*)(&class_name::class##class_name))
#define ASSERT_KINDOF(class_name, object)   ASSERT((object)->IsKindOf(RUNTIME_CLASS(class_name)))
// RTTI helper macros/functions
const CObject* AFX_CDECL AfxDynamicDownCast(CRuntimeClass* pClass, const CObject* pObject);
CObject* AFX_CDECL AfxDynamicDownCast(CRuntimeClass* pClass, CObject* pObject);
#define DYNAMIC_DOWNCAST(class_name, object)   (class_name*)AfxDynamicDownCast(RUNTIME_CLASS(class_name), object)
#ifdef _DEBUG
const CObject* AFX_CDECL AfxStaticDownCast(CRuntimeClass* pClass, const CObject* pObject);
CObject* AFX_CDECL AfxStaticDownCast(CRuntimeClass* pClass, CObject* pObject);
#define STATIC_DOWNCAST(class_name, object)   ((class_name*)AfxStaticDownCast(RUNTIME_CLASS(class_name), object))
#else
#define STATIC_DOWNCAST(class_name, object) ((class_name*)object)
#endif
//////////////////////////////////////////////////////////////////////////////
// Helper macros for declaring CRuntimeClass compatible classes
#ifdef _AFXDLL
#define DECLARE_DYNAMIC(class_name) \
  protected: \
  static CRuntimeClass* PASCAL _GetBaseClass(); \
  public: \
  static const AFX_DATA CRuntimeClass class##class_name; \
  virtual CRuntimeClass* GetRuntimeClass() const;
#define _DECLARE_DYNAMIC(class_name) \
  protected: \
  static CRuntimeClass* PASCAL _GetBaseClass(); \
  public: \
  static AFX_DATA CRuntimeClass class##class_name; \
  virtual CRuntimeClass* GetRuntimeClass() const;
#else
#define DECLARE_DYNAMIC(class_name) \
  public: \
  static const AFX_DATA CRuntimeClass class##class_name; \
  virtual CRuntimeClass* GetRuntimeClass() const;
#define _DECLARE_DYNAMIC(class_name) \
  public: \
  static AFX_DATA CRuntimeClass class##class_name; \
  virtual CRuntimeClass* GetRuntimeClass() const;
#endif
// not serializable, but dynamically constructable
#define DECLARE_DYNCREATE(class_name)   DECLARE_DYNAMIC(class_name) \
  static CObject* PASCAL CreateObject();
#define _DECLARE_DYNCREATE(class_name)   _DECLARE_DYNAMIC(class_name) \
  static CObject* PASCAL CreateObject();
#define DECLARE_SERIAL(class_name) \
  _DECLARE_DYNCREATE(class_name) \
  AFX_API friend CArchive& AFXAPI operator>>(CArchive& ar, class_name* &pOb);
// generate static object constructor for class registration
void AFXAPI AfxClassInit(CRuntimeClass* pNewClass);
struct AFX_CLASSINIT {
  AFX_CLASSINIT(CRuntimeClass* pNewClass) {
    AfxClassInit(pNewClass);
  }
};
struct AFX_CLASSINIT_COMPAT {
  AFX_CLASSINIT_COMPAT(CRuntimeClass* pNewClass);
};
#ifdef _AFXDLL
#define IMPLEMENT_RUNTIMECLASS(class_name, base_class_name, wSchema, pfnNew) \
  CRuntimeClass* PASCAL class_name::_GetBaseClass() \
  { return RUNTIME_CLASS(base_class_name); } \
  AFX_COMDAT const AFX_DATADEF CRuntimeClass class_name::class##class_name = { \
    #class_name, sizeof(class class_name), wSchema, pfnNew, \
    &class_name::_GetBaseClass, NULL }; \
  CRuntimeClass* class_name::GetRuntimeClass() const \
  { return RUNTIME_CLASS(class_name); }
#define _IMPLEMENT_RUNTIMECLASS(class_name, base_class_name, wSchema, pfnNew) \
  CRuntimeClass* PASCAL class_name::_GetBaseClass() \
  { return RUNTIME_CLASS(base_class_name); } \
  AFX_COMDAT AFX_DATADEF CRuntimeClass class_name::class##class_name = { \
    #class_name, sizeof(class class_name), wSchema, pfnNew, \
    &class_name::_GetBaseClass, NULL }; \
  CRuntimeClass* class_name::GetRuntimeClass() const \
  { return RUNTIME_CLASS(class_name); }
#else
#define IMPLEMENT_RUNTIMECLASS(class_name, base_class_name, wSchema, pfnNew) \
  AFX_COMDAT const AFX_DATADEF CRuntimeClass class_name::class##class_name = { \
    #class_name, sizeof(class class_name), wSchema, pfnNew, \
    RUNTIME_CLASS(base_class_name), NULL }; \
  CRuntimeClass* class_name::GetRuntimeClass() const \
  { return RUNTIME_CLASS(class_name); }
#define _IMPLEMENT_RUNTIMECLASS(class_name, base_class_name, wSchema, pfnNew) \
  AFX_DATADEF CRuntimeClass class_name::class##class_name = { \
    #class_name, sizeof(class class_name), wSchema, pfnNew, \
    RUNTIME_CLASS(base_class_name), NULL }; \
  CRuntimeClass* class_name::GetRuntimeClass() const \
  { return RUNTIME_CLASS(class_name); }
#endif
#define IMPLEMENT_DYNAMIC(class_name, base_class_name) \
  IMPLEMENT_RUNTIMECLASS(class_name, base_class_name, 0xFFFF, NULL)
#define IMPLEMENT_DYNCREATE(class_name, base_class_name) \
  CObject* PASCAL class_name::CreateObject() \
  { return new class_name; } \
  IMPLEMENT_RUNTIMECLASS(class_name, base_class_name, 0xFFFF, \
      class_name::CreateObject)
#define IMPLEMENT_SERIAL(class_name, base_class_name, wSchema) \
  CObject* PASCAL class_name::CreateObject() \
  { return new class_name; } \
  _IMPLEMENT_RUNTIMECLASS(class_name, base_class_name, wSchema, \
      class_name::CreateObject) \
  AFX_CLASSINIT _init_##class_name(RUNTIME_CLASS(class_name)); \
  CArchive& AFXAPI operator>>(CArchive& ar, class_name* &pOb) \
  { pOb = (class_name*) ar.ReadObject(RUNTIME_CLASS(class_name)); \
    return ar; }
// optional bit for schema number that enables object versioning
#define VERSIONABLE_SCHEMA  (0x80000000)
/////////////////////////////////////////////////////////////////////////////
// other helpers
// zero fill everything after the vtbl pointer
#define AFX_ZERO_INIT_OBJECT(base_class) \
  memset(((base_class*)this)+1, 0, sizeof(*this) - sizeof(class base_class));
/////////////////////////////////////////////////////////////////////////////
// Exceptions
#ifdef _AFXDLL
class CException : public CObject
#else
class AFX_NOVTABLE CException : public CObject
#endif
{
  // abstract class for dynamic type checking
  DECLARE_DYNAMIC(CException)
public:
  // Constructors
  CException();   // sets m_bAutoDelete = TRUE
  CException(BOOL bAutoDelete);   // sets m_bAutoDelete = bAutoDelete
  // Operations
  void Delete();  // use to delete exception in 'catch' block
  virtual BOOL GetErrorMessage(LPTSTR lpszError, UINT nMaxError,
      PUINT pnHelpContext = NULL);
  virtual int ReportError(UINT nType = MB_OK, UINT nMessageID = 0);
  // Implementation (setting m_bAutoDelete to FALSE is advanced)
public:
  virtual ~CException();
  BOOL m_bAutoDelete;
#ifdef _DEBUG
  void PASCAL operator delete(void* pbData);
#if _MSC_VER >= 1200
  void PASCAL operator delete(void* pbData, LPCSTR lpszFileName, int nLine);
#endif
protected:
  BOOL m_bReadyForDelete;
#endif
};
#ifdef _AFXDLL
class CSimpleException : public CException
#else
class AFX_NOVTABLE CSimpleException : public CException
#endif
{
  // base class for resource-critical MFC exceptions
  // handles ownership and initialization of an error message
public:
  // Constructors
  CSimpleException();
  CSimpleException(BOOL bAutoDelete);
  // Operations
  virtual BOOL GetErrorMessage(LPTSTR lpszError, UINT nMaxError,
      PUINT pnHelpContext = NULL);
  // Implementation (setting m_bAutoDelete to FALSE is advanced)
public:
  virtual ~CSimpleException();
  BOOL m_bAutoDelete;
  void InitString();      // used during MFC initialization
protected:
  BOOL m_bInitialized;
  BOOL m_bLoaded;
  TCHAR m_szMessage[128];
  UINT m_nResourceID;
#ifdef _DEBUG
  BOOL m_bReadyForDelete;
#endif
};
// helper routines for non-C++ EH implementations
#ifdef _AFX_OLD_EXCEPTIONS
BOOL AFXAPI AfxCatchProc(CRuntimeClass* pClass);
void AFXAPI AfxThrow(CException* pException);
#else
// for THROW_LAST auto-delete backward compatiblity
void AFXAPI AfxThrowLastCleanup();
#endif
// other out-of-line helper functions
void AFXAPI AfxTryCleanup();
#ifndef _AFX_JUMPBUF
// Use portable 'jmp_buf' defined by ANSI by default.
#define _AFX_JUMPBUF jmp_buf
#endif
// Placed on frame for EXCEPTION linkage, or CException cleanup
struct AFX_EXCEPTION_LINK {
#ifdef _AFX_OLD_EXCEPTIONS
  union {
    _AFX_JUMPBUF m_jumpBuf;
    struct {
      void (PASCAL* pfnCleanup)(AFX_EXCEPTION_LINK* pLink);
      void* pvData;       // extra data follows
    } m_callback;       // callback for cleanup (nType != 0)
  };
  UINT m_nType;               // 0 for setjmp, !=0 for user extension
#endif //!_AFX_OLD_EXCEPTIONS
  AFX_EXCEPTION_LINK* m_pLinkPrev;    // previous top, next in handler chain
  CException* m_pException;   // current exception (NULL in TRY block)
  AFX_EXCEPTION_LINK();       // for initialization and linking
  ~AFX_EXCEPTION_LINK() {     // for cleanup and unlinking
    AfxTryCleanup();
  };
};
// Exception global state - never access directly
struct AFX_EXCEPTION_CONTEXT {
  AFX_EXCEPTION_LINK* m_pLinkTop;
  // Note: most of the exception context is now in the AFX_EXCEPTION_LINK
};
#ifndef _PNH_DEFINED
typedef int (__cdecl* _PNH)(size_t);
#define _PNH_DEFINED
#endif
_PNH AFXAPI AfxGetNewHandler();
_PNH AFXAPI AfxSetNewHandler(_PNH pfnNewHandler);
int AFX_CDECL AfxNewHandler(size_t nSize);
void AFXAPI AfxAbort();
#ifdef _AFX_OLD_EXCEPTIONS
// Obsolete and non-portable: setting terminate handler
//  use CWinApp::ProcessWndProcException for Windows apps instead
//  can also use set_terminate which is part of C++ standard library
//      (these are provided for backward compatibility)
void AFXAPI AfxTerminate();
typedef void (AFXAPI* AFX_TERM_PROC)();
AFX_TERM_PROC AFXAPI AfxSetTerminate(AFX_TERM_PROC);
#endif
/////////////////////////////////////////////////////////////////////////////
// Exception macros using try, catch and throw
//  (for backward compatibility to previous versions of MFC)
#ifndef _AFX_OLD_EXCEPTIONS
#define TRY { AFX_EXCEPTION_LINK _afxExceptionLink; try {
#define CATCH(class, e) } catch (class* e) \
  { ASSERT(e->IsKindOf(RUNTIME_CLASS(class))); \
    _afxExceptionLink.m_pException = e;
#define AND_CATCH(class, e) } catch (class* e) \
  { ASSERT(e->IsKindOf(RUNTIME_CLASS(class))); \
    _afxExceptionLink.m_pException = e;
#define END_CATCH } }
#define THROW(e) throw e
#define THROW_LAST() (AfxThrowLastCleanup(), throw)
// Advanced macros for smaller code
#define CATCH_ALL(e) } catch (CException* e) \
  { { ASSERT(e->IsKindOf(RUNTIME_CLASS(CException))); \
      _afxExceptionLink.m_pException = e;
#define AND_CATCH_ALL(e) } catch (CException* e) \
  { { ASSERT(e->IsKindOf(RUNTIME_CLASS(CException))); \
      _afxExceptionLink.m_pException = e;
#define END_CATCH_ALL } } }
#define END_TRY } catch (CException* e) \
  { ASSERT(e->IsKindOf(RUNTIME_CLASS(CException))); \
    _afxExceptionLink.m_pException = e; } }
#else //_AFX_OLD_EXCEPTIONS
/////////////////////////////////////////////////////////////////////////////
// Exception macros using setjmp and longjmp
//  (for portability to compilers with no support for C++ exception handling)
#define TRY \
  { AFX_EXCEPTION_LINK _afxExceptionLink; \
    if (::setjmp(_afxExceptionLink.m_jumpBuf) == 0)
#define CATCH(class, e) \
  else if (::AfxCatchProc(RUNTIME_CLASS(class))) \
  { class* e = (class*)_afxExceptionLink.m_pException;
#define AND_CATCH(class, e) \
  } else if (::AfxCatchProc(RUNTIME_CLASS(class))) \
  { class* e = (class*)_afxExceptionLink.m_pException;
#define END_CATCH   } else { ::AfxThrow(NULL); } }
#define THROW(e) AfxThrow(e)
#define THROW_LAST() AfxThrow(NULL)
// Advanced macros for smaller code
#define CATCH_ALL(e)   else { CException* e = _afxExceptionLink.m_pException;
#define AND_CATCH_ALL(e)   } else { CException* e = _afxExceptionLink.m_pException;
#define END_CATCH_ALL } }
#define END_TRY }
#endif //_AFX_OLD_EXCEPTIONS
/////////////////////////////////////////////////////////////////////////////
// Standard Exception classes
class CMemoryException : public CSimpleException
{
  DECLARE_DYNAMIC(CMemoryException)
public:
  CMemoryException();
  // Implementation
public:
  CMemoryException(BOOL bAutoDelete);
  CMemoryException(BOOL bAutoDelete, UINT nResourceID);
  virtual ~CMemoryException();
};
class CNotSupportedException : public CSimpleException
{
  DECLARE_DYNAMIC(CNotSupportedException)
public:
  CNotSupportedException();
  // Implementation
public:
  CNotSupportedException(BOOL bAutoDelete);
  CNotSupportedException(BOOL bAutoDelete, UINT nResourceID);
  virtual ~CNotSupportedException();
};
class CArchiveException : public CException
{
  DECLARE_DYNAMIC(CArchiveException)
public:
  enum {
    none,
    generic,
    readOnly,
    endOfFile,
    writeOnly,
    badIndex,
    badClass,
    badSchema
  };
  // Constructor
  CArchiveException(int cause = CArchiveException::none,
      LPCTSTR lpszArchiveName = NULL);
  // Attributes
  int m_cause;
  CString m_strFileName;
  // Implementation
public:
  virtual ~CArchiveException();
#ifdef _DEBUG
  virtual void Dump(CDumpContext& dc) const;
#endif
  virtual BOOL GetErrorMessage(LPTSTR lpszError, UINT nMaxError,
      PUINT pnHelpContext = NULL);
};
class CFileException : public CException
{
  DECLARE_DYNAMIC(CFileException)
public:
  enum {
    none,
    generic,
    fileNotFound,
    badPath,
    tooManyOpenFiles,
    accessDenied,
    invalidFile,
    removeCurrentDir,
    directoryFull,
    badSeek,
    hardIO,
    sharingViolation,
    lockViolation,
    diskFull,
    endOfFile
  };
  // Constructor
  CFileException(int cause = CFileException::none, LONG lOsError = -1,
      LPCTSTR lpszArchiveName = NULL);
  // Attributes
  int     m_cause;
  LONG    m_lOsError;
  CString m_strFileName;
  // Operations
  // convert a OS dependent error code to a Cause
  static int PASCAL OsErrorToException(LONG lOsError);
  static int PASCAL ErrnoToException(int nErrno);
  // helper functions to throw exception after converting to a Cause
  static void PASCAL ThrowOsError(LONG lOsError, LPCTSTR lpszFileName = NULL);
  static void PASCAL ThrowErrno(int nErrno, LPCTSTR lpszFileName = NULL);
  // Implementation
public:
  virtual ~CFileException();
#ifdef _DEBUG
  virtual void Dump(CDumpContext&) const;
#endif
  virtual BOOL GetErrorMessage(LPTSTR lpszError, UINT nMaxError,
      PUINT pnHelpContext = NULL);
};
/////////////////////////////////////////////////////////////////////////////
// Standard exception throws
void AFXAPI AfxThrowMemoryException();
void AFXAPI AfxThrowNotSupportedException();
void AFXAPI AfxThrowArchiveException(int cause,
    LPCTSTR lpszArchiveName = NULL);
void AFXAPI AfxThrowFileException(int cause, LONG lOsError = -1,
    LPCTSTR lpszFileName = NULL);
/////////////////////////////////////////////////////////////////////////////
// File - raw unbuffered disk file I/O
class CFile : public CObject
{
  DECLARE_DYNAMIC(CFile)
public:
  // Flag values
  enum OpenFlags {
    modeRead =          0x0000,
    modeWrite =         0x0001,
    modeReadWrite =     0x0002,
    shareCompat =       0x0000,
    shareExclusive =    0x0010,
    shareDenyWrite =    0x0020,
    shareDenyRead =     0x0030,
    shareDenyNone =     0x0040,
    modeNoInherit =     0x0080,
    modeCreate =        0x1000,
    modeNoTruncate =    0x2000,
    typeText =          0x4000, // typeText and typeBinary are used in
    typeBinary = (int)0x8000   // derived classes only
  };
  enum Attribute {
    normal =    0x00,
    readOnly =  0x01,
    hidden =    0x02,
    system =    0x04,
    volume =    0x08,
    directory = 0x10,
    archive =   0x20
  };
  enum SeekPosition { begin = 0x0, current = 0x1, end = 0x2 };
  enum { hFileNull = -1 };
  // Constructors
  CFile();
  CFile(int hFile);
  CFile(LPCTSTR lpszFileName, UINT nOpenFlags);
  // Attributes
  UINT m_hFile;
  operator HFILE() const;
  virtual DWORD GetPosition() const;
  BOOL GetStatus(CFileStatus& rStatus) const;
  virtual CString GetFileName() const;
  virtual CString GetFileTitle() const;
  virtual CString GetFilePath() const;
  virtual void SetFilePath(LPCTSTR lpszNewName);
  // Operations
  virtual BOOL Open(LPCTSTR lpszFileName, UINT nOpenFlags,
      CFileException* pError = NULL);
  static void PASCAL Rename(LPCTSTR lpszOldName,
      LPCTSTR lpszNewName);
  static void PASCAL Remove(LPCTSTR lpszFileName);
  static BOOL PASCAL GetStatus(LPCTSTR lpszFileName,
      CFileStatus& rStatus);
  static void PASCAL SetStatus(LPCTSTR lpszFileName,
      const CFileStatus& status);
  DWORD SeekToEnd();
  void SeekToBegin();
  // backward compatible ReadHuge and WriteHuge
  DWORD ReadHuge(void* lpBuffer, DWORD dwCount);
  void WriteHuge(const void* lpBuffer, DWORD dwCount);
  // Overridables
  virtual CFile* Duplicate() const;
  virtual LONG Seek(LONG lOff, UINT nFrom);
  virtual void SetLength(DWORD dwNewLen);
  virtual DWORD GetLength() const;
  virtual UINT Read(void* lpBuf, UINT nCount);
  virtual void Write(const void* lpBuf, UINT nCount);
  virtual void LockRange(DWORD dwPos, DWORD dwCount);
  virtual void UnlockRange(DWORD dwPos, DWORD dwCount);
  virtual void Abort();
  virtual void Flush();
  virtual void Close();
  // Implementation
public:
  virtual ~CFile();
#ifdef _DEBUG
  virtual void AssertValid() const;
  virtual void Dump(CDumpContext& dc) const;
#endif
  enum BufferCommand { bufferRead, bufferWrite, bufferCommit, bufferCheck };
  virtual UINT GetBufferPtr(UINT nCommand, UINT nCount = 0,
      void** ppBufStart = NULL, void** ppBufMax = NULL);
protected:
  BOOL m_bCloseOnDelete;
  CString m_strFileName;
};
/////////////////////////////////////////////////////////////////////////////
// STDIO file implementation
class CStdioFile : public CFile
{
  DECLARE_DYNAMIC(CStdioFile)
public:
  // Constructors
  CStdioFile();
  CStdioFile(FILE* pOpenStream);
  CStdioFile(LPCTSTR lpszFileName, UINT nOpenFlags);
  // Attributes
  FILE* m_pStream;    // stdio FILE
  // m_hFile from base class is _fileno(m_pStream)
  // Operations
  // reading and writing strings
  virtual void WriteString(LPCTSTR lpsz);
  virtual LPTSTR ReadString(LPTSTR lpsz, UINT nMax);
  virtual BOOL ReadString(CString& rString);
  // Implementation
public:
  virtual ~CStdioFile();
#ifdef _DEBUG
  void Dump(CDumpContext& dc) const;
#endif
  virtual DWORD GetPosition() const;
  virtual BOOL Open(LPCTSTR lpszFileName, UINT nOpenFlags,
      CFileException* pError = NULL);
  virtual UINT Read(void* lpBuf, UINT nCount);
  virtual void Write(const void* lpBuf, UINT nCount);
  virtual LONG Seek(LONG lOff, UINT nFrom);
  virtual void Abort();
  virtual void Flush();
  virtual void Close();
  // Unsupported APIs
  virtual CFile* Duplicate() const;
  virtual void LockRange(DWORD dwPos, DWORD dwCount);
  virtual void UnlockRange(DWORD dwPos, DWORD dwCount);
};
////////////////////////////////////////////////////////////////////////////
// Memory based file implementation
class CMemFile : public CFile
{
  DECLARE_DYNAMIC(CMemFile)
public:
  // Constructors
  CMemFile(UINT nGrowBytes = 1024);
  CMemFile(BYTE* lpBuffer, UINT nBufferSize, UINT nGrowBytes = 0);
  // Operations
  void Attach(BYTE* lpBuffer, UINT nBufferSize, UINT nGrowBytes = 0);
  BYTE* Detach();
  // Advanced Overridables
protected:
  virtual BYTE* Alloc(DWORD nBytes);
  virtual BYTE* Realloc(BYTE* lpMem, DWORD nBytes);
  virtual BYTE* Memcpy(BYTE* lpMemTarget, const BYTE* lpMemSource, UINT nBytes);
  virtual void Free(BYTE* lpMem);
  virtual void GrowFile(DWORD dwNewLen);
  // Implementation
protected:
  UINT m_nGrowBytes;
  DWORD m_nPosition;
  DWORD m_nBufferSize;
  DWORD m_nFileSize;
  BYTE* m_lpBuffer;
  BOOL m_bAutoDelete;
public:
  virtual ~CMemFile();
#ifdef _DEBUG
  virtual void Dump(CDumpContext& dc) const;
  virtual void AssertValid() const;
#endif
  virtual DWORD GetPosition() const;
  BOOL GetStatus(CFileStatus& rStatus) const;
  virtual LONG Seek(LONG lOff, UINT nFrom);
  virtual void SetLength(DWORD dwNewLen);
  virtual UINT Read(void* lpBuf, UINT nCount);
  virtual void Write(const void* lpBuf, UINT nCount);
  virtual void Abort();
  virtual void Flush();
  virtual void Close();
  virtual UINT GetBufferPtr(UINT nCommand, UINT nCount = 0,
      void** ppBufStart = NULL, void** ppBufMax = NULL);
  // Unsupported APIs
  virtual CFile* Duplicate() const;
  virtual void LockRange(DWORD dwPos, DWORD dwCount);
  virtual void UnlockRange(DWORD dwPos, DWORD dwCount);
};
////////////////////////////////////////////////////////////////////////////
// Local file searches
class CFileFind : public CObject
{
public:
  CFileFind();
  virtual ~CFileFind();
  // Attributes
public:
  DWORD GetLength() const;
#if defined(_X86_) || defined(_ALPHA_)
  __int64 GetLength64() const;
#endif
  virtual CString GetFileName() const;
  virtual CString GetFilePath() const;
  virtual CString GetFileTitle() const;
  virtual CString GetFileURL() const;
  virtual CString GetRoot() const;
  virtual BOOL GetLastWriteTime(FILETIME* pTimeStamp) const;
  virtual BOOL GetLastAccessTime(FILETIME* pTimeStamp) const;
  virtual BOOL GetCreationTime(FILETIME* pTimeStamp) const;
  virtual BOOL GetLastWriteTime(CTime& refTime) const;
  virtual BOOL GetLastAccessTime(CTime& refTime) const;
  virtual BOOL GetCreationTime(CTime& refTime) const;
  virtual BOOL MatchesMask(DWORD dwMask) const;
  virtual BOOL IsDots() const;
  // these aren't virtual because they all use MatchesMask(), which is
  BOOL IsReadOnly() const;
  BOOL IsDirectory() const;
  BOOL IsCompressed() const;
  BOOL IsSystem() const;
  BOOL IsHidden() const;
  BOOL IsTemporary() const;
  BOOL IsNormal() const;
  BOOL IsArchived() const;
  // Operations
  void Close();
  virtual BOOL FindFile(LPCTSTR pstrName = NULL, DWORD dwUnused = 0);
  virtual BOOL FindNextFile();
protected:
  virtual void CloseContext();
  // Implementation
protected:
  void* m_pFoundInfo;
  void* m_pNextInfo;
  HANDLE m_hContext;
  BOOL m_bGotLast;
  CString m_strRoot;
  TCHAR m_chDirSeparator;     // not '\\' for Internet classes
#ifdef _DEBUG
  void Dump(CDumpContext& dc) const;
  void AssertValid() const;
#endif
  DECLARE_DYNAMIC(CFileFind)
};
/////////////////////////////////////////////////////////////////////////////
// CTimeSpan and CTime
class CTimeSpan
{
public:
  // Constructors
  CTimeSpan();
  CTimeSpan(time_t time);
  CTimeSpan(LONG lDays, int nHours, int nMins, int nSecs);
  CTimeSpan(const CTimeSpan& timeSpanSrc);
  const CTimeSpan& operator=(const CTimeSpan& timeSpanSrc);
  // Attributes
  // extract parts
  LONG GetDays() const;   // total # of days
  LONG GetTotalHours() const;
  int GetHours() const;
  LONG GetTotalMinutes() const;
  int GetMinutes() const;
  LONG GetTotalSeconds() const;
  int GetSeconds() const;
  // Operations
  // time math
  CTimeSpan operator-(CTimeSpan timeSpan) const;
  CTimeSpan operator+(CTimeSpan timeSpan) const;
  const CTimeSpan& operator+=(CTimeSpan timeSpan);
  const CTimeSpan& operator-=(CTimeSpan timeSpan);
  BOOL operator==(CTimeSpan timeSpan) const;
  BOOL operator!=(CTimeSpan timeSpan) const;
  BOOL operator<(CTimeSpan timeSpan) const;
  BOOL operator>(CTimeSpan timeSpan) const;
  BOOL operator<=(CTimeSpan timeSpan) const;
  BOOL operator>=(CTimeSpan timeSpan) const;
#ifdef _UNICODE
  // for compatibility with MFC 3.x
  CString Format(LPCSTR pFormat) const;
#endif
  CString Format(LPCTSTR pFormat) const;
  CString Format(UINT nID) const;
  // serialization
#ifdef _DEBUG
  friend CDumpContext& AFXAPI operator<<(CDumpContext& dc, CTimeSpan timeSpan);
#endif
  friend CArchive& AFXAPI operator<<(CArchive& ar, CTimeSpan timeSpan);
  friend CArchive& AFXAPI operator>>(CArchive& ar, CTimeSpan& rtimeSpan);
private:
  time_t m_timeSpan;
  friend class CTime;
};
class CTime
{
public:
  // Constructors
  static CTime PASCAL GetCurrentTime();
  CTime();
  CTime(time_t time);
  CTime(int nYear, int nMonth, int nDay, int nHour, int nMin, int nSec,
      int nDST = -1);
  CTime(WORD wDosDate, WORD wDosTime, int nDST = -1);
  CTime(const CTime& timeSrc);
  CTime(const SYSTEMTIME& sysTime, int nDST = -1);
  CTime(const FILETIME& fileTime, int nDST = -1);
  const CTime& operator=(const CTime& timeSrc);
  const CTime& operator=(time_t t);
  // Attributes
  struct tm* GetGmtTm(struct tm* ptm = NULL) const;
  struct tm* GetLocalTm(struct tm* ptm = NULL) const;
  BOOL GetAsSystemTime(SYSTEMTIME& timeDest) const;
  time_t GetTime() const;
  int GetYear() const;
  int GetMonth() const;       // month of year (1 = Jan)
  int GetDay() const;         // day of month
  int GetHour() const;
  int GetMinute() const;
  int GetSecond() const;
  int GetDayOfWeek() const;   // 1=Sun, 2=Mon, ..., 7=Sat
  // Operations
  // time math
  CTimeSpan operator-(CTime time) const;
  CTime operator-(CTimeSpan timeSpan) const;
  CTime operator+(CTimeSpan timeSpan) const;
  const CTime& operator+=(CTimeSpan timeSpan);
  const CTime& operator-=(CTimeSpan timeSpan);
  BOOL operator==(CTime time) const;
  BOOL operator!=(CTime time) const;
  BOOL operator<(CTime time) const;
  BOOL operator>(CTime time) const;
  BOOL operator<=(CTime time) const;
  BOOL operator>=(CTime time) const;
  // formatting using "C" strftime
  CString Format(LPCTSTR pFormat) const;
  CString FormatGmt(LPCTSTR pFormat) const;
  CString Format(UINT nFormatID) const;
  CString FormatGmt(UINT nFormatID) const;
#ifdef _UNICODE
  // for compatibility with MFC 3.x
  CString Format(LPCSTR pFormat) const;
  CString FormatGmt(LPCSTR pFormat) const;
#endif
  // serialization
#ifdef _DEBUG
  friend CDumpContext& AFXAPI operator<<(CDumpContext& dc, CTime time);
#endif
  friend CArchive& AFXAPI operator<<(CArchive& ar, CTime time);
  friend CArchive& AFXAPI operator>>(CArchive& ar, CTime& rtime);
private:
  time_t m_time;
};
/////////////////////////////////////////////////////////////////////////////
// File status
struct CFileStatus {
  CTime m_ctime;          // creation date/time of file
  CTime m_mtime;          // last modification date/time of file
  CTime m_atime;          // last access date/time of file
  LONG m_size;            // logical size of file in bytes
  BYTE m_attribute;       // logical OR of CFile::Attribute enum values
  BYTE _m_padding;        // pad the structure to a WORD
  TCHAR m_szFullName[_MAX_PATH]; // absolute path name
#ifdef _DEBUG
  void Dump(CDumpContext& dc) const;
#endif
};
/////////////////////////////////////////////////////////////////////////////
// Diagnostic memory management routines
// Low level sanity checks for memory blocks
BOOL AFXAPI AfxIsValidAddress(const void* lp,
    UINT nBytes, BOOL bReadWrite = TRUE);
BOOL AFXAPI AfxIsValidString(LPCWSTR lpsz, int nLength = -1);
BOOL AFXAPI AfxIsValidString(LPCSTR lpsz, int nLength = -1);
#if defined(_DEBUG) && !defined(_AFX_NO_DEBUG_CRT)
// Memory tracking allocation
void* AFX_CDECL operator new(size_t nSize, LPCSTR lpszFileName, int nLine);
#define DEBUG_NEW new(THIS_FILE, __LINE__)
#if _MSC_VER >= 1200
void AFX_CDECL operator delete(void* p, LPCSTR lpszFileName, int nLine);
#endif
void* AFXAPI AfxAllocMemoryDebug(size_t nSize, BOOL bIsObject,
    LPCSTR lpszFileName, int nLine);
void AFXAPI AfxFreeMemoryDebug(void* pbData, BOOL bIsObject);
// Dump any memory leaks since program started
BOOL AFXAPI AfxDumpMemoryLeaks();
// Return TRUE if valid memory block of nBytes
BOOL AFXAPI AfxIsMemoryBlock(const void* p, UINT nBytes,
    LONG* plRequestNumber = NULL);
// Return TRUE if memory is sane or print out what is wrong
BOOL AFXAPI AfxCheckMemory();
#define afxMemDF _crtDbgFlag
enum AfxMemDF { // memory debug/diagnostic flags
  allocMemDF          = 0x01,         // turn on debugging allocator
  delayFreeMemDF      = 0x02,         // delay freeing memory
  checkAlwaysMemDF    = 0x04          // AfxCheckMemory on every alloc/free
};
#ifdef _UNICODE
#define AfxOutputDebugString(lpsz)   do { \
    USES_CONVERSION; \
    _RPT0(_CRT_WARN, W2CA(lpsz)); \
  } while (0)
#else
#define AfxOutputDebugString(lpsz) _RPT0(_CRT_WARN, lpsz)
#endif
// turn on/off tracking for a short while
BOOL AFXAPI AfxEnableMemoryTracking(BOOL bTrack);
// Advanced initialization: for overriding default diagnostics
BOOL AFXAPI AfxDiagnosticInit(void);
// A failure hook returns whether to permit allocation
typedef BOOL (AFXAPI* AFX_ALLOC_HOOK)(size_t nSize, BOOL bObject, LONG lRequestNumber);
// Set new hook, return old (never NULL)
AFX_ALLOC_HOOK AFXAPI AfxSetAllocHook(AFX_ALLOC_HOOK pfnAllocHook);
// Debugger hook on specified allocation request - Obsolete
void AFXAPI AfxSetAllocStop(LONG lRequestNumber);
// Memory state for snapshots/leak detection
struct CMemoryState {
  // Attributes
  enum blockUsage {
    freeBlock,    // memory not used
    objectBlock,  // contains a CObject derived class object
    bitBlock,     // contains ::operator new data
    crtBlock,
    ignoredBlock,
    nBlockUseMax  // total number of usages
  };
  _CrtMemState m_memState;
  LONG m_lCounts[nBlockUseMax];
  LONG m_lSizes[nBlockUseMax];
  LONG m_lHighWaterCount;
  LONG m_lTotalCount;
  CMemoryState();
  // Operations
  void Checkpoint();  // fill with current state
  BOOL Difference(const CMemoryState& oldState,
      const CMemoryState& newState);  // fill with difference
  void UpdateData();
  // Output to afxDump
  void DumpStatistics() const;
  void DumpAllObjectsSince() const;
};
// Enumerate allocated objects or runtime classes
void AFXAPI AfxDoForAllObjects(void (AFX_CDECL* pfn)(CObject* pObject, void* pContext),
    void* pContext);
void AFXAPI AfxDoForAllClasses(void (AFX_CDECL* pfn)(const CRuntimeClass* pClass,
    void* pContext), void* pContext);
#else
// non-_DEBUG_ALLOC version that assume everything is OK
#define DEBUG_NEW new
#define AfxCheckMemory() TRUE
#define AfxIsMemoryBlock(p, nBytes) TRUE
#define AfxEnableMemoryTracking(bTrack) FALSE
#define AfxOutputDebugString(lpsz) ::OutputDebugString(lpsz)
// diagnostic initialization
#ifndef _DEBUG
#define AfxDiagnosticInit() TRUE
#else
BOOL AFXAPI AfxDiagnosticInit(void);
#endif
#endif // _DEBUG
/////////////////////////////////////////////////////////////////////////////
// Archives for serializing CObject data
// needed for implementation
class CPtrArray;
class CMapPtrToPtr;
class CDocument;
class CArchive
{
public:
  // Flag values
  enum Mode { store = 0, load = 1, bNoFlushOnDelete = 2, bNoByteSwap = 4 };
  CArchive(CFile* pFile, UINT nMode, int nBufSize = 4096, void* lpBuf = NULL);
  ~CArchive();
  // Attributes
  BOOL IsLoading() const;
  BOOL IsStoring() const;
  BOOL IsByteSwapping() const;
  BOOL IsBufferEmpty() const;
  CFile* GetFile() const;
  UINT GetObjectSchema(); // only valid when reading a CObject*
  void SetObjectSchema(UINT nSchema);
  // pointer to document being serialized -- must set to serialize
  //  COleClientItems in a document!
  CDocument* m_pDocument;
  // Operations
  UINT Read(void* lpBuf, UINT nMax);
  void Write(const void* lpBuf, UINT nMax);
  void Flush();
  void Close();
  void Abort();   // close and shutdown without exceptions
  // reading and writing strings
  void WriteString(LPCTSTR lpsz);
  LPTSTR ReadString(LPTSTR lpsz, UINT nMax);
  BOOL ReadString(CString& rString);
public:
  // Object I/O is pointer based to avoid added construction overhead.
  // Use the Serialize member function directly for embedded objects.
  friend CArchive& AFXAPI operator<<(CArchive& ar, const CObject* pOb);
  friend CArchive& AFXAPI operator>>(CArchive& ar, CObject*& pOb);
  friend CArchive& AFXAPI operator>>(CArchive& ar, const CObject*& pOb);
  // insertion operations
  CArchive& operator<<(BYTE by);
  CArchive& operator<<(WORD w);
  CArchive& operator<<(LONG l);
  CArchive& operator<<(DWORD dw);
  CArchive& operator<<(float f);
  CArchive& operator<<(double d);
  CArchive& operator<<(int i);
  CArchive& operator<<(short w);
  CArchive& operator<<(char ch);
  CArchive& operator<<(unsigned u);
  // extraction operations
  CArchive& operator>>(BYTE& by);
  CArchive& operator>>(WORD& w);
  CArchive& operator>>(DWORD& dw);
  CArchive& operator>>(LONG& l);
  CArchive& operator>>(float& f);
  CArchive& operator>>(double& d);
  CArchive& operator>>(int& i);
  CArchive& operator>>(short& w);
  CArchive& operator>>(char& ch);
  CArchive& operator>>(unsigned& u);
  // object read/write
  CObject* ReadObject(const CRuntimeClass* pClass);
  void WriteObject(const CObject* pOb);
  // advanced object mapping (used for forced references)
  void MapObject(const CObject* pOb);
  // advanced versioning support
  void WriteClass(const CRuntimeClass* pClassRef);
  CRuntimeClass* ReadClass(const CRuntimeClass* pClassRefRequested = NULL,
      UINT* pSchema = NULL, DWORD* pObTag = NULL);
  void SerializeClass(const CRuntimeClass* pClassRef);
  // advanced operations (used when storing/loading many objects)
  void SetStoreParams(UINT nHashSize = 2053, UINT nBlockSize = 128);
  void SetLoadParams(UINT nGrowBy = 1024);
  // Implementation
public:
  BOOL m_bForceFlat;  // for COleClientItem implementation (default TRUE)
  BOOL m_bDirectBuffer;   // TRUE if m_pFile supports direct buffering
  void FillBuffer(UINT nBytesNeeded);
  void CheckCount();  // throw exception if m_nMapCount is too large
  // special functions for reading and writing (16-bit compatible) counts
  DWORD ReadCount();
  void WriteCount(DWORD dwCount);
  // public for advanced use
  UINT m_nObjectSchema;
  CString m_strFileName;
protected:
  // archive objects cannot be copied or assigned
  CArchive(const CArchive& arSrc);
  void operator=(const CArchive& arSrc);
  BOOL m_nMode;
  BOOL m_bUserBuf;
  int m_nBufSize;
  CFile* m_pFile;
  BYTE* m_lpBufCur;
  BYTE* m_lpBufMax;
  BYTE* m_lpBufStart;
  // array/map for CObject* and CRuntimeClass* load/store
  UINT m_nMapCount;
  union {
    CPtrArray* m_pLoadArray;
    CMapPtrToPtr* m_pStoreMap;
  };
  // map to keep track of mismatched schemas
  CMapPtrToPtr* m_pSchemaMap;
  // advanced parameters (controls performance with large archives)
  UINT m_nGrowSize;
  UINT m_nHashSize;
};
/////////////////////////////////////////////////////////////////////////////
// Diagnostic dumping
// Note: AfxDumpStack is available in release builds, although it is always
//  statically linked so as to not negatively affect the size of MFC42.DLL.
#define AFX_STACK_DUMP_TARGET_TRACE         0x0001
#define AFX_STACK_DUMP_TARGET_CLIPBOARD 0x0002
#define AFX_STACK_DUMP_TARGET_BOTH          0x0003
#define AFX_STACK_DUMP_TARGET_ODS           0x0004
#ifdef _DEBUG
#define AFX_STACK_DUMP_TARGET_DEFAULT       AFX_STACK_DUMP_TARGET_TRACE
#else
#define AFX_STACK_DUMP_TARGET_DEFAULT       AFX_STACK_DUMP_TARGET_CLIPBOARD
#endif
void AFXAPI AfxDumpStack(DWORD dwFlags = AFX_STACK_DUMP_TARGET_DEFAULT);
class CDumpContext
{
public:
  CDumpContext(CFile* pFile = NULL);
  // Attributes
  int GetDepth() const;      // 0 => this object, 1 => children objects
  void SetDepth(int nNewDepth);
  // Operations
  CDumpContext& operator<<(LPCTSTR lpsz);
#ifdef _UNICODE
  CDumpContext& operator<<(LPCSTR lpsz);  // automatically widened
#else
  CDumpContext& operator<<(LPCWSTR lpsz); // automatically thinned
#endif
  CDumpContext& operator<<(const void* lp);
  CDumpContext& operator<<(const CObject* pOb);
  CDumpContext& operator<<(const CObject& ob);
  CDumpContext& operator<<(BYTE by);
  CDumpContext& operator<<(WORD w);
  CDumpContext& operator<<(UINT u);
  CDumpContext& operator<<(LONG l);
  CDumpContext& operator<<(DWORD dw);
  CDumpContext& operator<<(float f);
  CDumpContext& operator<<(double d);
  CDumpContext& operator<<(int n);
  void HexDump(LPCTSTR lpszLine, BYTE* pby, int nBytes, int nWidth);
  void Flush();
  // Implementation
protected:
  // dump context objects cannot be copied or assigned
  CDumpContext(const CDumpContext& dcSrc);
  void operator=(const CDumpContext& dcSrc);
  void OutputString(LPCTSTR lpsz);
  int m_nDepth;
public:
  CFile* m_pFile;
};
#ifdef _DEBUG
extern AFX_DATA CDumpContext afxDump;
extern AFX_DATA BOOL afxTraceEnabled;
#endif
/////////////////////////////////////////////////////////////////////////////
// Classes declared in this file
//CObject
// Arrays
class CByteArray;           // array of BYTE
class CWordArray;           // array of WORD
class CDWordArray;          // array of DWORD
class CUIntArray;           // array of UINT
class CPtrArray;            // array of void*
class CObArray;             // array of CObject*
// Lists
class CPtrList;             // list of void*
class CObList;              // list of CObject*
// Maps (aka Dictionaries)
class CMapWordToOb;         // map from WORD to CObject*
class CMapWordToPtr;        // map from WORD to void*
class CMapPtrToWord;        // map from void* to WORD
class CMapPtrToPtr;         // map from void* to void*
// Special String variants
class CStringArray;         // array of CStrings
class CStringList;          // list of CStrings
class CMapStringToPtr;      // map from CString to void*
class CMapStringToOb;       // map from CString to CObject*
class CMapStringToString;   // map from CString to CString
/////////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////////
class CByteArray : public CObject
{
  DECLARE_SERIAL(CByteArray)
public:
  // Construction
  CByteArray();
  // Attributes
  int GetSize() const;
  int GetUpperBound() const;
  void SetSize(int nNewSize, int nGrowBy = -1);
  // Operations
  // Clean up
  void FreeExtra();
  void RemoveAll();
  // Accessing elements
  BYTE GetAt(int nIndex) const;
  void SetAt(int nIndex, BYTE newElement);
  BYTE& ElementAt(int nIndex);
  // Direct Access to the element data (may return NULL)
  const BYTE* GetData() const;
  BYTE* GetData();
  // Potentially growing the array
  void SetAtGrow(int nIndex, BYTE newElement);
  int Add(BYTE newElement);
  int Append(const CByteArray& src);
  void Copy(const CByteArray& src);
  // overloaded operator helpers
  BYTE operator[](int nIndex) const;
  BYTE& operator[](int nIndex);
  // Operations that move elements around
  void InsertAt(int nIndex, BYTE newElement, int nCount = 1);
  void RemoveAt(int nIndex, int nCount = 1);
  void InsertAt(int nStartIndex, CByteArray* pNewArray);
  // Implementation
protected:
  BYTE* m_pData;   // the actual array of data
  int m_nSize;     // # of elements (upperBound - 1)
  int m_nMaxSize;  // max allocated
  int m_nGrowBy;   // grow amount
public:
  ~CByteArray();
  void Serialize(CArchive&);
#ifdef _DEBUG
  void Dump(CDumpContext&) const;
  void AssertValid() const;
#endif
protected:
  // local typedefs for class templates
  typedef BYTE BASE_TYPE;
  typedef BYTE BASE_ARG_TYPE;
};
////////////////////////////////////////////////////////////////////////////
class CWordArray : public CObject
{
  DECLARE_SERIAL(CWordArray)
public:
  // Construction
  CWordArray();
  // Attributes
  int GetSize() const;
  int GetUpperBound() const;
  void SetSize(int nNewSize, int nGrowBy = -1);
  // Operations
  // Clean up
  void FreeExtra();
  void RemoveAll();
  // Accessing elements
  WORD GetAt(int nIndex) const;
  void SetAt(int nIndex, WORD newElement);
  WORD& ElementAt(int nIndex);
  // Direct Access to the element data (may return NULL)
  const WORD* GetData() const;
  WORD* GetData();
  // Potentially growing the array
  void SetAtGrow(int nIndex, WORD newElement);
  int Add(WORD newElement);
  int Append(const CWordArray& src);
  void Copy(const CWordArray& src);
  // overloaded operator helpers
  WORD operator[](int nIndex) const;
  WORD& operator[](int nIndex);
  // Operations that move elements around
  void InsertAt(int nIndex, WORD newElement, int nCount = 1);
  void RemoveAt(int nIndex, int nCount = 1);
  void InsertAt(int nStartIndex, CWordArray* pNewArray);
  // Implementation
protected:
  WORD* m_pData;   // the actual array of data
  int m_nSize;     // # of elements (upperBound - 1)
  int m_nMaxSize;  // max allocated
  int m_nGrowBy;   // grow amount
public:
  ~CWordArray();
  void Serialize(CArchive&);
#ifdef _DEBUG
  void Dump(CDumpContext&) const;
  void AssertValid() const;
#endif
protected:
  // local typedefs for class templates
  typedef WORD BASE_TYPE;
  typedef WORD BASE_ARG_TYPE;
};
////////////////////////////////////////////////////////////////////////////
class CDWordArray : public CObject
{
  DECLARE_SERIAL(CDWordArray)
public:
  // Construction
  CDWordArray();
  // Attributes
  int GetSize() const;
  int GetUpperBound() const;
  void SetSize(int nNewSize, int nGrowBy = -1);
  // Operations
  // Clean up
  void FreeExtra();
  void RemoveAll();
  // Accessing elements
  DWORD GetAt(int nIndex) const;
  void SetAt(int nIndex, DWORD newElement);
  DWORD& ElementAt(int nIndex);
  // Direct Access to the element data (may return NULL)
  const DWORD* GetData() const;
  DWORD* GetData();
  // Potentially growing the array
  void SetAtGrow(int nIndex, DWORD newElement);
  int Add(DWORD newElement);
  int Append(const CDWordArray& src);
  void Copy(const CDWordArray& src);
  // overloaded operator helpers
  DWORD operator[](int nIndex) const;
  DWORD& operator[](int nIndex);
  // Operations that move elements around
  void InsertAt(int nIndex, DWORD newElement, int nCount = 1);
  void RemoveAt(int nIndex, int nCount = 1);
  void InsertAt(int nStartIndex, CDWordArray* pNewArray);
  // Implementation
protected:
  DWORD* m_pData;   // the actual array of data
  int m_nSize;     // # of elements (upperBound - 1)
  int m_nMaxSize;  // max allocated
  int m_nGrowBy;   // grow amount
public:
  ~CDWordArray();
  void Serialize(CArchive&);
#ifdef _DEBUG
  void Dump(CDumpContext&) const;
  void AssertValid() const;
#endif
protected:
  // local typedefs for class templates
  typedef DWORD BASE_TYPE;
  typedef DWORD BASE_ARG_TYPE;
};
////////////////////////////////////////////////////////////////////////////
class CUIntArray : public CObject
{
  DECLARE_DYNAMIC(CUIntArray)
public:
  // Construction
  CUIntArray();
  // Attributes
  int GetSize() const;
  int GetUpperBound() const;
  void SetSize(int nNewSize, int nGrowBy = -1);
  // Operations
  // Clean up
  void FreeExtra();
  void RemoveAll();
  // Accessing elements
  UINT GetAt(int nIndex) const;
  void SetAt(int nIndex, UINT newElement);
  UINT& ElementAt(int nIndex);
  // Direct Access to the element data (may return NULL)
  const UINT* GetData() const;
  UINT* GetData();
  // Potentially growing the array
  void SetAtGrow(int nIndex, UINT newElement);
  int Add(UINT newElement);
  int Append(const CUIntArray& src);
  void Copy(const CUIntArray& src);
  // overloaded operator helpers
  UINT operator[](int nIndex) const;
  UINT& operator[](int nIndex);
  // Operations that move elements around
  void InsertAt(int nIndex, UINT newElement, int nCount = 1);
  void RemoveAt(int nIndex, int nCount = 1);
  void InsertAt(int nStartIndex, CUIntArray* pNewArray);
  // Implementation
protected:
  UINT* m_pData;   // the actual array of data
  int m_nSize;     // # of elements (upperBound - 1)
  int m_nMaxSize;  // max allocated
  int m_nGrowBy;   // grow amount
public:
  ~CUIntArray();
#ifdef _DEBUG
  void Dump(CDumpContext&) const;
  void AssertValid() const;
#endif
protected:
  // local typedefs for class templates
  typedef UINT BASE_TYPE;
  typedef UINT BASE_ARG_TYPE;
};
////////////////////////////////////////////////////////////////////////////
class CPtrArray : public CObject
{
  DECLARE_DYNAMIC(CPtrArray)
public:
  // Construction
  CPtrArray();
  // Attributes
  int GetSize() const;
  int GetUpperBound() const;
  void SetSize(int nNewSize, int nGrowBy = -1);
  // Operations
  // Clean up
  void FreeExtra();
  void RemoveAll();
  // Accessing elements
  void* GetAt(int nIndex) const;
  void SetAt(int nIndex, void* newElement);
  void*& ElementAt(int nIndex);
  // Direct Access to the element data (may return NULL)
  const void** GetData() const;
  void** GetData();
  // Potentially growing the array
  void SetAtGrow(int nIndex, void* newElement);
  int Add(void* newElement);
  int Append(const CPtrArray& src);
  void Copy(const CPtrArray& src);
  // overloaded operator helpers
  void* operator[](int nIndex) const;
  void*& operator[](int nIndex);
  // Operations that move elements around
  void InsertAt(int nIndex, void* newElement, int nCount = 1);
  void RemoveAt(int nIndex, int nCount = 1);
  void InsertAt(int nStartIndex, CPtrArray* pNewArray);
  // Implementation
protected:
  void** m_pData;   // the actual array of data
  int m_nSize;     // # of elements (upperBound - 1)
  int m_nMaxSize;  // max allocated
  int m_nGrowBy;   // grow amount
public:
  ~CPtrArray();
#ifdef _DEBUG
  void Dump(CDumpContext&) const;
  void AssertValid() const;
#endif
protected:
  // local typedefs for class templates
  typedef void* BASE_TYPE;
  typedef void* BASE_ARG_TYPE;
};
////////////////////////////////////////////////////////////////////////////
class CObArray : public CObject
{
  DECLARE_SERIAL(CObArray)
public:
  // Construction
  CObArray();
  // Attributes
  int GetSize() const;
  int GetUpperBound() const;
  void SetSize(int nNewSize, int nGrowBy = -1);
  // Operations
  // Clean up
  void FreeExtra();
  void RemoveAll();
  // Accessing elements
  CObject* GetAt(int nIndex) const;
  void SetAt(int nIndex, CObject* newElement);
  CObject*& ElementAt(int nIndex);
  // Direct Access to the element data (may return NULL)
  const CObject** GetData() const;
  CObject** GetData();
  // Potentially growing the array
  void SetAtGrow(int nIndex, CObject* newElement);
  int Add(CObject* newElement);
  int Append(const CObArray& src);
  void Copy(const CObArray& src);
  // overloaded operator helpers
  CObject* operator[](int nIndex) const;
  CObject*& operator[](int nIndex);
  // Operations that move elements around
  void InsertAt(int nIndex, CObject* newElement, int nCount = 1);
  void RemoveAt(int nIndex, int nCount = 1);
  void InsertAt(int nStartIndex, CObArray* pNewArray);
  // Implementation
protected:
  CObject** m_pData;   // the actual array of data
  int m_nSize;     // # of elements (upperBound - 1)
  int m_nMaxSize;  // max allocated
  int m_nGrowBy;   // grow amount
public:
  ~CObArray();
  void Serialize(CArchive&);
#ifdef _DEBUG
  void Dump(CDumpContext&) const;
  void AssertValid() const;
#endif
protected:
  // local typedefs for class templates
  typedef CObject* BASE_TYPE;
  typedef CObject* BASE_ARG_TYPE;
};
////////////////////////////////////////////////////////////////////////////
class CStringArray : public CObject
{
  DECLARE_SERIAL(CStringArray)
public:
  // Construction
  CStringArray();
  // Attributes
  int GetSize() const;
  int GetUpperBound() const;
  void SetSize(int nNewSize, int nGrowBy = -1);
  // Operations
  // Clean up
  void FreeExtra();
  void RemoveAll();
  // Accessing elements
  CString GetAt(int nIndex) const;
  void SetAt(int nIndex, LPCTSTR newElement);
  void SetAt(int nIndex, const CString& newElement);
  CString& ElementAt(int nIndex);
  // Direct Access to the element data (may return NULL)
  const CString* GetData() const;
  CString* GetData();
  // Potentially growing the array
  void SetAtGrow(int nIndex, LPCTSTR newElement);
  void SetAtGrow(int nIndex, const CString& newElement);
  int Add(LPCTSTR newElement);
  int Add(const CString& newElement);
  int Append(const CStringArray& src);
  void Copy(const CStringArray& src);
  // overloaded operator helpers
  CString operator[](int nIndex) const;
  CString& operator[](int nIndex);
  // Operations that move elements around
  void InsertAt(int nIndex, LPCTSTR newElement, int nCount = 1);
  void InsertAt(int nIndex, const CString& newElement, int nCount = 1);
  void RemoveAt(int nIndex, int nCount = 1);
  void InsertAt(int nStartIndex, CStringArray* pNewArray);
  // Implementation
protected:
  CString* m_pData;   // the actual array of data
  int m_nSize;     // # of elements (upperBound - 1)
  int m_nMaxSize;  // max allocated
  int m_nGrowBy;   // grow amount
  void InsertEmpty(int nIndex, int nCount);
public:
  ~CStringArray();
  void Serialize(CArchive&);
#ifdef _DEBUG
  void Dump(CDumpContext&) const;
  void AssertValid() const;
#endif
protected:
  // local typedefs for class templates
  typedef CString BASE_TYPE;
  typedef LPCTSTR BASE_ARG_TYPE;
};
/////////////////////////////////////////////////////////////////////////////
class CPtrList : public CObject
{
  DECLARE_DYNAMIC(CPtrList)
protected:
  struct CNode {
    CNode* pNext;
    CNode* pPrev;
    void* data;
  };
public:
  // Construction
  CPtrList(int nBlockSize = 10);
  // Attributes (head and tail)
  // count of elements
  int GetCount() const;
  BOOL IsEmpty() const;
  // peek at head or tail
  void*& GetHead();
  void* GetHead() const;
  void*& GetTail();
  void* GetTail() const;
  // Operations
  // get head or tail (and remove it) - don't call on empty list!
  void* RemoveHead();
  void* RemoveTail();
  // add before head or after tail
  POSITION AddHead(void* newElement);
  POSITION AddTail(void* newElement);
  // add another list of elements before head or after tail
  void AddHead(CPtrList* pNewList);
  void AddTail(CPtrList* pNewList);
  // remove all elements
  void RemoveAll();
  // iteration
  POSITION GetHeadPosition() const;
  POSITION GetTailPosition() const;
  void*& GetNext(POSITION& rPosition); // return *Position++
  void* GetNext(POSITION& rPosition) const; // return *Position++
  void*& GetPrev(POSITION& rPosition); // return *Position--
  void* GetPrev(POSITION& rPosition) const; // return *Position--
  // getting/modifying an element at a given position
  void*& GetAt(POSITION position);
  void* GetAt(POSITION position) const;
  void SetAt(POSITION pos, void* newElement);
  void RemoveAt(POSITION position);
  // inserting before or after a given position
  POSITION InsertBefore(POSITION position, void* newElement);
  POSITION InsertAfter(POSITION position, void* newElement);
  // helper functions (note: O(n) speed)
  POSITION Find(void* searchValue, POSITION startAfter = NULL) const;
  // defaults to starting at the HEAD
  // return NULL if not found
  POSITION FindIndex(int nIndex) const;
  // get the 'nIndex'th element (may return NULL)
  // Implementation
protected:
  CNode* m_pNodeHead;
  CNode* m_pNodeTail;
  int m_nCount;
  CNode* m_pNodeFree;
  struct CPlex* m_pBlocks;
  int m_nBlockSize;
  CNode* NewNode(CNode*, CNode*);
  void FreeNode(CNode*);
public:
  ~CPtrList();
#ifdef _DEBUG
  void Dump(CDumpContext&) const;
  void AssertValid() const;
#endif
  // local typedefs for class templates
  typedef void* BASE_TYPE;
  typedef void* BASE_ARG_TYPE;
};
/////////////////////////////////////////////////////////////////////////////
class CObList : public CObject
{
  DECLARE_SERIAL(CObList)
protected:
  struct CNode {
    CNode* pNext;
    CNode* pPrev;
    CObject* data;
  };
public:
  // Construction
  CObList(int nBlockSize = 10);
  // Attributes (head and tail)
  // count of elements
  int GetCount() const;
  BOOL IsEmpty() const;
  // peek at head or tail
  CObject*& GetHead();
  CObject* GetHead() const;
  CObject*& GetTail();
  CObject* GetTail() const;
  // Operations
  // get head or tail (and remove it) - don't call on empty list!
  CObject* RemoveHead();
  CObject* RemoveTail();
  // add before head or after tail
  POSITION AddHead(CObject* newElement);
  POSITION AddTail(CObject* newElement);
  // add another list of elements before head or after tail
  void AddHead(CObList* pNewList);
  void AddTail(CObList* pNewList);
  // remove all elements
  void RemoveAll();
  // iteration
  POSITION GetHeadPosition() const;
  POSITION GetTailPosition() const;
  CObject*& GetNext(POSITION& rPosition); // return *Position++
  CObject* GetNext(POSITION& rPosition) const; // return *Position++
  CObject*& GetPrev(POSITION& rPosition); // return *Position--
  CObject* GetPrev(POSITION& rPosition) const; // return *Position--
  // getting/modifying an element at a given position
  CObject*& GetAt(POSITION position);
  CObject* GetAt(POSITION position) const;
  void SetAt(POSITION pos, CObject* newElement);
  void RemoveAt(POSITION position);
  // inserting before or after a given position
  POSITION InsertBefore(POSITION position, CObject* newElement);
  POSITION InsertAfter(POSITION position, CObject* newElement);
  // helper functions (note: O(n) speed)
  POSITION Find(CObject* searchValue, POSITION startAfter = NULL) const;
  // defaults to starting at the HEAD
  // return NULL if not found
  POSITION FindIndex(int nIndex) const;
  // get the 'nIndex'th element (may return NULL)
  // Implementation
protected:
  CNode* m_pNodeHead;
  CNode* m_pNodeTail;
  int m_nCount;
  CNode* m_pNodeFree;
  struct CPlex* m_pBlocks;
  int m_nBlockSize;
  CNode* NewNode(CNode*, CNode*);
  void FreeNode(CNode*);
public:
  ~CObList();
  void Serialize(CArchive&);
#ifdef _DEBUG
  void Dump(CDumpContext&) const;
  void AssertValid() const;
#endif
  // local typedefs for class templates
  typedef CObject* BASE_TYPE;
  typedef CObject* BASE_ARG_TYPE;
};
/////////////////////////////////////////////////////////////////////////////
class CStringList : public CObject
{
  DECLARE_SERIAL(CStringList)
protected:
  struct CNode {
    CNode* pNext;
    CNode* pPrev;
    CString data;
  };
public:
  // Construction
  CStringList(int nBlockSize = 10);
  // Attributes (head and tail)
  // count of elements
  int GetCount() const;
  BOOL IsEmpty() const;
  // peek at head or tail
  CString& GetHead();
  CString GetHead() const;
  CString& GetTail();
  CString GetTail() const;
  // Operations
  // get head or tail (and remove it) - don't call on empty list!
  CString RemoveHead();
  CString RemoveTail();
  // add before head or after tail
  POSITION AddHead(LPCTSTR newElement);
  POSITION AddTail(LPCTSTR newElement);
  POSITION AddHead(const CString& newElement);
  POSITION AddTail(const CString& newElement);
  // add another list of elements before head or after tail
  void AddHead(CStringList* pNewList);
  void AddTail(CStringList* pNewList);
  // remove all elements
  void RemoveAll();
  // iteration
  POSITION GetHeadPosition() const;
  POSITION GetTailPosition() const;
  CString& GetNext(POSITION& rPosition); // return *Position++
  CString GetNext(POSITION& rPosition) const; // return *Position++
  CString& GetPrev(POSITION& rPosition); // return *Position--
  CString GetPrev(POSITION& rPosition) const; // return *Position--
  // getting/modifying an element at a given position
  CString& GetAt(POSITION position);
  CString GetAt(POSITION position) const;
  void SetAt(POSITION pos, LPCTSTR newElement);
  void SetAt(POSITION pos, const CString& newElement);
  void RemoveAt(POSITION position);
  // inserting before or after a given position
  POSITION InsertBefore(POSITION position, LPCTSTR newElement);
  POSITION InsertAfter(POSITION position, LPCTSTR newElement);
  POSITION InsertBefore(POSITION position, const CString& newElement);
  POSITION InsertAfter(POSITION position, const CString& newElement);
  // helper functions (note: O(n) speed)
  POSITION Find(LPCTSTR searchValue, POSITION startAfter = NULL) const;
  // defaults to starting at the HEAD
  // return NULL if not found
  POSITION FindIndex(int nIndex) const;
  // get the 'nIndex'th element (may return NULL)
  // Implementation
protected:
  CNode* m_pNodeHead;
  CNode* m_pNodeTail;
  int m_nCount;
  CNode* m_pNodeFree;
  struct CPlex* m_pBlocks;
  int m_nBlockSize;
  CNode* NewNode(CNode*, CNode*);
  void FreeNode(CNode*);
public:
  ~CStringList();
  void Serialize(CArchive&);
#ifdef _DEBUG
  void Dump(CDumpContext&) const;
  void AssertValid() const;
#endif
  // local typedefs for class templates
  typedef CString BASE_TYPE;
  typedef LPCTSTR BASE_ARG_TYPE;
};
/////////////////////////////////////////////////////////////////////////////
class CMapWordToPtr : public CObject
{
  DECLARE_DYNAMIC(CMapWordToPtr)
protected:
  // Association
  struct CAssoc {
    CAssoc* pNext;
    WORD key;
    void* value;
  };
public:
  // Construction
  CMapWordToPtr(int nBlockSize = 10);
  // Attributes
  // number of elements
  int GetCount() const;
  BOOL IsEmpty() const;
  // Lookup
  BOOL Lookup(WORD key, void*& rValue) const;
  // Operations
  // Lookup and add if not there
  void*& operator[](WORD key);
  // add a new (key, value) pair
  void SetAt(WORD key, void* newValue);
  // removing existing (key, ?) pair
  BOOL RemoveKey(WORD key);
  void RemoveAll();
  // iterating all (key, value) pairs
  POSITION GetStartPosition() const;
  void GetNextAssoc(POSITION& rNextPosition, WORD& rKey, void*& rValue) const;
  // advanced features for derived classes
  UINT GetHashTableSize() const;
  void InitHashTable(UINT hashSize, BOOL bAllocNow = TRUE);
  // Overridables: special non-virtual (see map implementation for details)
  // Routine used to user-provided hash keys
  UINT HashKey(WORD key) const;
  // Implementation
protected:
  CAssoc** m_pHashTable;
  UINT m_nHashTableSize;
  int m_nCount;
  CAssoc* m_pFreeList;
  struct CPlex* m_pBlocks;
  int m_nBlockSize;
  CAssoc* NewAssoc();
  void FreeAssoc(CAssoc*);
  CAssoc* GetAssocAt(WORD, UINT&) const;
public:
  ~CMapWordToPtr();
#ifdef _DEBUG
  void Dump(CDumpContext&) const;
  void AssertValid() const;
#endif
protected:
  // local typedefs for CTypedPtrMap class template
  typedef WORD BASE_KEY;
  typedef WORD BASE_ARG_KEY;
  typedef void* BASE_VALUE;
  typedef void* BASE_ARG_VALUE;
};
/////////////////////////////////////////////////////////////////////////////
class CMapPtrToWord : public CObject
{
  DECLARE_DYNAMIC(CMapPtrToWord)
protected:
  // Association
  struct CAssoc {
    CAssoc* pNext;
    void* key;
    WORD value;
  };
public:
  // Construction
  CMapPtrToWord(int nBlockSize = 10);
  // Attributes
  // number of elements
  int GetCount() const;
  BOOL IsEmpty() const;
  // Lookup
  BOOL Lookup(void* key, WORD& rValue) const;
  // Operations
  // Lookup and add if not there
  WORD& operator[](void* key);
  // add a new (key, value) pair
  void SetAt(void* key, WORD newValue);
  // removing existing (key, ?) pair
  BOOL RemoveKey(void* key);
  void RemoveAll();
  // iterating all (key, value) pairs
  POSITION GetStartPosition() const;
  void GetNextAssoc(POSITION& rNextPosition, void*& rKey, WORD& rValue) const;
  // advanced features for derived classes
  UINT GetHashTableSize() const;
  void InitHashTable(UINT hashSize, BOOL bAllocNow = TRUE);
  // Overridables: special non-virtual (see map implementation for details)
  // Routine used to user-provided hash keys
  UINT HashKey(void* key) const;
  // Implementation
protected:
  CAssoc** m_pHashTable;
  UINT m_nHashTableSize;
  int m_nCount;
  CAssoc* m_pFreeList;
  struct CPlex* m_pBlocks;
  int m_nBlockSize;
  CAssoc* NewAssoc();
  void FreeAssoc(CAssoc*);
  CAssoc* GetAssocAt(void*, UINT&) const;
public:
  ~CMapPtrToWord();
#ifdef _DEBUG
  void Dump(CDumpContext&) const;
  void AssertValid() const;
#endif
protected:
  // local typedefs for CTypedPtrMap class template
  typedef void* BASE_KEY;
  typedef void* BASE_ARG_KEY;
  typedef WORD BASE_VALUE;
  typedef WORD BASE_ARG_VALUE;
};
/////////////////////////////////////////////////////////////////////////////
class CMapPtrToPtr : public CObject
{
  DECLARE_DYNAMIC(CMapPtrToPtr)
protected:
  // Association
  struct CAssoc {
    CAssoc* pNext;
    void* key;
    void* value;
  };
public:
  // Construction
  CMapPtrToPtr(int nBlockSize = 10);
  // Attributes
  // number of elements
  int GetCount() const;
  BOOL IsEmpty() const;
  // Lookup
  BOOL Lookup(void* key, void*& rValue) const;
  // Operations
  // Lookup and add if not there
  void*& operator[](void* key);
  // add a new (key, value) pair
  void SetAt(void* key, void* newValue);
  // removing existing (key, ?) pair
  BOOL RemoveKey(void* key);
  void RemoveAll();
  // iterating all (key, value) pairs
  POSITION GetStartPosition() const;
  void GetNextAssoc(POSITION& rNextPosition, void*& rKey, void*& rValue) const;
  // advanced features for derived classes
  UINT GetHashTableSize() const;
  void InitHashTable(UINT hashSize, BOOL bAllocNow = TRUE);
  // Overridables: special non-virtual (see map implementation for details)
  // Routine used to user-provided hash keys
  UINT HashKey(void* key) const;
  // Implementation
protected:
  CAssoc** m_pHashTable;
  UINT m_nHashTableSize;
  int m_nCount;
  CAssoc* m_pFreeList;
  struct CPlex* m_pBlocks;
  int m_nBlockSize;
  CAssoc* NewAssoc();
  void FreeAssoc(CAssoc*);
  CAssoc* GetAssocAt(void*, UINT&) const;
public:
  ~CMapPtrToPtr();
#ifdef _DEBUG
  void Dump(CDumpContext&) const;
  void AssertValid() const;
#endif
  void* GetValueAt(void* key) const;
protected:
  // local typedefs for CTypedPtrMap class template
  typedef void* BASE_KEY;
  typedef void* BASE_ARG_KEY;
  typedef void* BASE_VALUE;
  typedef void* BASE_ARG_VALUE;
};
/////////////////////////////////////////////////////////////////////////////
class CMapWordToOb : public CObject
{
  DECLARE_SERIAL(CMapWordToOb)
protected:
  // Association
  struct CAssoc {
    CAssoc* pNext;
    WORD key;
    CObject* value;
  };
public:
  // Construction
  CMapWordToOb(int nBlockSize = 10);
  // Attributes
  // number of elements
  int GetCount() const;
  BOOL IsEmpty() const;
  // Lookup
  BOOL Lookup(WORD key, CObject*& rValue) const;
  // Operations
  // Lookup and add if not there
  CObject*& operator[](WORD key);
  // add a new (key, value) pair
  void SetAt(WORD key, CObject* newValue);
  // removing existing (key, ?) pair
  BOOL RemoveKey(WORD key);
  void RemoveAll();
  // iterating all (key, value) pairs
  POSITION GetStartPosition() const;
  void GetNextAssoc(POSITION& rNextPosition, WORD& rKey, CObject*& rValue) const;
  // advanced features for derived classes
  UINT GetHashTableSize() const;
  void InitHashTable(UINT hashSize, BOOL bAllocNow = TRUE);
  // Overridables: special non-virtual (see map implementation for details)
  // Routine used to user-provided hash keys
  UINT HashKey(WORD key) const;
  // Implementation
protected:
  CAssoc** m_pHashTable;
  UINT m_nHashTableSize;
  int m_nCount;
  CAssoc* m_pFreeList;
  struct CPlex* m_pBlocks;
  int m_nBlockSize;
  CAssoc* NewAssoc();
  void FreeAssoc(CAssoc*);
  CAssoc* GetAssocAt(WORD, UINT&) const;
public:
  ~CMapWordToOb();
  void Serialize(CArchive&);
#ifdef _DEBUG
  void Dump(CDumpContext&) const;
  void AssertValid() const;
#endif
protected:
  // local typedefs for CTypedPtrMap class template
  typedef WORD BASE_KEY;
  typedef WORD BASE_ARG_KEY;
  typedef CObject* BASE_VALUE;
  typedef CObject* BASE_ARG_VALUE;
};
/////////////////////////////////////////////////////////////////////////////
class CMapStringToPtr : public CObject
{
  DECLARE_DYNAMIC(CMapStringToPtr)
protected:
  // Association
  struct CAssoc {
    CAssoc* pNext;
    UINT nHashValue;  // needed for efficient iteration
    CString key;
    void* value;
  };
public:
  // Construction
  CMapStringToPtr(int nBlockSize = 10);
  // Attributes
  // number of elements
  int GetCount() const;
  BOOL IsEmpty() const;
  // Lookup
  BOOL Lookup(LPCTSTR key, void*& rValue) const;
  BOOL LookupKey(LPCTSTR key, LPCTSTR& rKey) const;
  // Operations
  // Lookup and add if not there
  void*& operator[](LPCTSTR key);
  // add a new (key, value) pair
  void SetAt(LPCTSTR key, void* newValue);
  // removing existing (key, ?) pair
  BOOL RemoveKey(LPCTSTR key);
  void RemoveAll();
  // iterating all (key, value) pairs
  POSITION GetStartPosition() const;
  void GetNextAssoc(POSITION& rNextPosition, CString& rKey, void*& rValue) const;
  // advanced features for derived classes
  UINT GetHashTableSize() const;
  void InitHashTable(UINT hashSize, BOOL bAllocNow = TRUE);
  // Overridables: special non-virtual (see map implementation for details)
  // Routine used to user-provided hash keys
  UINT HashKey(LPCTSTR key) const;
  // Implementation
protected:
  CAssoc** m_pHashTable;
  UINT m_nHashTableSize;
  int m_nCount;
  CAssoc* m_pFreeList;
  struct CPlex* m_pBlocks;
  int m_nBlockSize;
  CAssoc* NewAssoc();
  void FreeAssoc(CAssoc*);
  CAssoc* GetAssocAt(LPCTSTR, UINT&) const;
public:
  ~CMapStringToPtr();
#ifdef _DEBUG
  void Dump(CDumpContext&) const;
  void AssertValid() const;
#endif
protected:
  // local typedefs for CTypedPtrMap class template
  typedef CString BASE_KEY;
  typedef LPCTSTR BASE_ARG_KEY;
  typedef void* BASE_VALUE;
  typedef void* BASE_ARG_VALUE;
};
/////////////////////////////////////////////////////////////////////////////
class CMapStringToOb : public CObject
{
  DECLARE_SERIAL(CMapStringToOb)
protected:
  // Association
  struct CAssoc {
    CAssoc* pNext;
    UINT nHashValue;  // needed for efficient iteration
    CString key;
    CObject* value;
  };
public:
  // Construction
  CMapStringToOb(int nBlockSize = 10);
  // Attributes
  // number of elements
  int GetCount() const;
  BOOL IsEmpty() const;
  // Lookup
  BOOL Lookup(LPCTSTR key, CObject*& rValue) const;
  BOOL LookupKey(LPCTSTR key, LPCTSTR& rKey) const;
  // Operations
  // Lookup and add if not there
  CObject*& operator[](LPCTSTR key);
  // add a new (key, value) pair
  void SetAt(LPCTSTR key, CObject* newValue);
  // removing existing (key, ?) pair
  BOOL RemoveKey(LPCTSTR key);
  void RemoveAll();
  // iterating all (key, value) pairs
  POSITION GetStartPosition() const;
  void GetNextAssoc(POSITION& rNextPosition, CString& rKey, CObject*& rValue) const;
  // advanced features for derived classes
  UINT GetHashTableSize() const;
  void InitHashTable(UINT hashSize, BOOL bAllocNow = TRUE);
  // Overridables: special non-virtual (see map implementation for details)
  // Routine used to user-provided hash keys
  UINT HashKey(LPCTSTR key) const;
  // Implementation
protected:
  CAssoc** m_pHashTable;
  UINT m_nHashTableSize;
  int m_nCount;
  CAssoc* m_pFreeList;
  struct CPlex* m_pBlocks;
  int m_nBlockSize;
  CAssoc* NewAssoc();
  void FreeAssoc(CAssoc*);
  CAssoc* GetAssocAt(LPCTSTR, UINT&) const;
public:
  ~CMapStringToOb();
  void Serialize(CArchive&);
#ifdef _DEBUG
  void Dump(CDumpContext&) const;
  void AssertValid() const;
#endif
protected:
  // local typedefs for CTypedPtrMap class template
  typedef CString BASE_KEY;
  typedef LPCTSTR BASE_ARG_KEY;
  typedef CObject* BASE_VALUE;
  typedef CObject* BASE_ARG_VALUE;
};
/////////////////////////////////////////////////////////////////////////////
class CMapStringToString : public CObject
{
  DECLARE_SERIAL(CMapStringToString)
protected:
  // Association
  struct CAssoc {
    CAssoc* pNext;
    UINT nHashValue;  // needed for efficient iteration
    CString key;
    CString value;
  };
public:
  // Construction
  CMapStringToString(int nBlockSize = 10);
  // Attributes
  // number of elements
  int GetCount() const;
  BOOL IsEmpty() const;
  // Lookup
  BOOL Lookup(LPCTSTR key, CString& rValue) const;
  BOOL LookupKey(LPCTSTR key, LPCTSTR& rKey) const;
  // Operations
  // Lookup and add if not there
  CString& operator[](LPCTSTR key);
  // add a new (key, value) pair
  void SetAt(LPCTSTR key, LPCTSTR newValue);
  // removing existing (key, ?) pair
  BOOL RemoveKey(LPCTSTR key);
  void RemoveAll();
  // iterating all (key, value) pairs
  POSITION GetStartPosition() const;
  void GetNextAssoc(POSITION& rNextPosition, CString& rKey, CString& rValue) const;
  // advanced features for derived classes
  UINT GetHashTableSize() const;
  void InitHashTable(UINT hashSize, BOOL bAllocNow = TRUE);
  // Overridables: special non-virtual (see map implementation for details)
  // Routine used to user-provided hash keys
  UINT HashKey(LPCTSTR key) const;
  // Implementation
protected:
  CAssoc** m_pHashTable;
  UINT m_nHashTableSize;
  int m_nCount;
  CAssoc* m_pFreeList;
  struct CPlex* m_pBlocks;
  int m_nBlockSize;
  CAssoc* NewAssoc();
  void FreeAssoc(CAssoc*);
  CAssoc* GetAssocAt(LPCTSTR, UINT&) const;
public:
  ~CMapStringToString();
  void Serialize(CArchive&);
#ifdef _DEBUG
  void Dump(CDumpContext&) const;
  void AssertValid() const;
#endif
protected:
  // local typedefs for CTypedPtrMap class template
  typedef CString BASE_KEY;
  typedef LPCTSTR BASE_ARG_KEY;
  typedef CString BASE_VALUE;
  typedef LPCTSTR BASE_ARG_VALUE;
};

class CSimpleList;
class CThreadSlotData;                  // for manipulationg thread local storage
class CThreadLocalObject;               // for storing thread local data
class CProcessLocalObject;              // for storing thread local data
class CNoTrackObject;

// template class CTypedSimpleList<>
// template class CThreadLocal<>
// template class CProcessLocal<>

/////////////////////////////////////////////////////////////////////////////
// CSimpleList (simple/small subset of CList)

class CSimpleList
{
public:
	CSimpleList(int nNextOffset = 0);
	void Construct(int nNextOffset);

// Operations
	BOOL IsEmpty() const;
	void AddHead(void* p);
	void RemoveAll();
	void* GetHead() const;
	void* GetNext(void* p) const;
	BOOL Remove(void* p);

// Implementation
	void* m_pHead;
	size_t m_nNextOffset;

	void** GetNextPtr(void* p) const;   // somewhat trusting...
};
#define AFX_INLINE inline
AFX_INLINE CSimpleList::CSimpleList(int nNextOffset)
	{ m_pHead = NULL; m_nNextOffset = nNextOffset; }
AFX_INLINE void CSimpleList::Construct(int nNextOffset)
	{ ASSERT(m_pHead == NULL); m_nNextOffset = nNextOffset; }
AFX_INLINE BOOL CSimpleList::IsEmpty() const
	{ return m_pHead == NULL; }
AFX_INLINE void** CSimpleList::GetNextPtr(void* p) const
	{ ASSERT(p != NULL); return (void**)((BYTE*)p+m_nNextOffset); }
AFX_INLINE void CSimpleList::RemoveAll()
	{ m_pHead = NULL; }
AFX_INLINE void* CSimpleList::GetHead() const
	{ return m_pHead; }
AFX_INLINE void* CSimpleList::GetNext(void* prevElement) const
	{ return *GetNextPtr(prevElement); }

template<class TYPE>
class CTypedSimpleList : public CSimpleList
{
public:
	CTypedSimpleList(int nNextOffset = 0)
		: CSimpleList(nNextOffset) { }
	void AddHead(TYPE p)
		{ CSimpleList::AddHead(p); }
	TYPE GetHead()
		{ return (TYPE)CSimpleList::GetHead(); }
	TYPE GetNext(TYPE p)
		{ return (TYPE)CSimpleList::GetNext(p); }
	BOOL Remove(TYPE p)
		{ return CSimpleList::Remove((TYPE)p); }
	operator TYPE()
		{ return (TYPE)CSimpleList::GetHead(); }
};

/////////////////////////////////////////////////////////////////////////////
// CThreadSlotData - manages owned array of "slots" for thread local storage

struct CThreadData; // private to implementation
struct CSlotData;   // private to implementation

class CThreadSlotData
{
public:
	CThreadSlotData();

// Operations
	int AllocSlot();
	void FreeSlot(int nSlot);
	void* GetValue(int nSlot);
	void SetValue(int nSlot, void* pValue);
	// delete all values in process/thread
	void DeleteValues(HINSTANCE hInst, BOOL bAll = FALSE);
	// assign instance handle to just constructed slots
	void AssignInstance(HINSTANCE hInst);

// Implementation
	DWORD m_tlsIndex;   // used to access system thread-local storage

	int m_nAlloc;       // number of slots allocated (in UINTs)
	int m_nRover;       // (optimization) for quick finding of free slots
	int m_nMax;         // size of slot table below (in bits)
	CSlotData* m_pSlotData; // state of each slot (allocated or not)
	CTypedSimpleList<CThreadData*> m_list;  // list of CThreadData structures
	CRITICAL_SECTION m_sect;

	void* GetThreadValue(int nSlot); // special version for threads only!
	void* PASCAL operator new(size_t, void* p)
		{ return p; }
	void DeleteValues(CThreadData* pData, HINSTANCE hInst);
	~CThreadSlotData();
};

class AFX_NOVTABLE CNoTrackObject
{
public:
	void* PASCAL operator new(size_t nSize);
	void PASCAL operator delete(void*);

#if defined(_DEBUG) && !defined(_AFX_NO_DEBUG_CRT)
	void* PASCAL operator new(size_t nSize, LPCSTR, int);
#if _MSC_VER >= 1200
	void PASCAL operator delete(void* pObject, LPCSTR, int);
#endif
#endif
	virtual ~CNoTrackObject() { }
};

class AFX_NOVTABLE CThreadLocalObject
{
public:
// Attributes
	CNoTrackObject* GetData(CNoTrackObject* (AFXAPI* pfnCreateObject)());
	CNoTrackObject* GetDataNA();

// Implementation
	int m_nSlot;
	~CThreadLocalObject();
};

class AFX_NOVTABLE CProcessLocalObject
{
public:
// Attributes
	CNoTrackObject* GetData(CNoTrackObject* (AFXAPI* pfnCreateObject)());

// Implementation
	CNoTrackObject* volatile m_pObject;
	~CProcessLocalObject();
};

template<class TYPE>
class CThreadLocal : public CThreadLocalObject
{
// Attributes
public:
	AFX_INLINE TYPE* GetData()
	{
		TYPE* pData = (TYPE*)CThreadLocalObject::GetData(&CreateObject);
		ASSERT(pData != NULL);
		return pData;
	}
	AFX_INLINE TYPE* GetDataNA()
	{
		TYPE* pData = (TYPE*)CThreadLocalObject::GetDataNA();
		return pData;
	}
	AFX_INLINE operator TYPE*()
		{ return GetData(); }
	AFX_INLINE TYPE* operator->()
		{ return GetData(); }

// Implementation
public:
	static CNoTrackObject* AFXAPI CreateObject()
		{ return new TYPE; }
};

#define THREAD_LOCAL(class_name, ident_name) \
	AFX_DATADEF CThreadLocal<class_name> ident_name;
#define EXTERN_THREAD_LOCAL(class_name, ident_name) \
	extern AFX_DATA THREAD_LOCAL(class_name, ident_name)

template<class TYPE>
class CProcessLocal : public CProcessLocalObject
{
// Attributes
public:
	AFX_INLINE TYPE* GetData()
	{
		TYPE* pData = (TYPE*)CProcessLocalObject::GetData(&CreateObject);
		ASSERT(pData != NULL);
		return pData;
	}
	AFX_INLINE TYPE* GetDataNA()
		{ return (TYPE*)m_pObject; }
	AFX_INLINE operator TYPE*()
		{ return GetData(); }
	AFX_INLINE TYPE* operator->()
		{ return GetData(); }

// Implementation
public:
	static CNoTrackObject* AFXAPI CreateObject()
		{ return new TYPE; }
};

#define PROCESS_LOCAL(class_name, ident_name) \
	AFX_DATADEF CProcessLocal<class_name> ident_name;
#define EXTERN_PROCESS_LOCAL(class_name, ident_name) \
	extern AFX_DATA PROCESS_LOCAL(class_name, ident_name)

/////////////////////////////////////////////////////////////////////////////

void AFXAPI AfxInitLocalData(HINSTANCE hInstInit);
void AFXAPI AfxTermLocalData(HINSTANCE hInstTerm, BOOL bAll = FALSE);
void AFXAPI AfxTlsAddRef();
void AFXAPI AfxTlsRelease();


/////////////////////////////////////////////////////////////////////////////
// _AFX_DEBUG_STATE

#ifdef _DEBUG

class _AFX_DEBUG_STATE : public CNoTrackObject
{
public:
	_AFX_DEBUG_STATE();
	virtual ~_AFX_DEBUG_STATE();
};

EXTERN_PROCESS_LOCAL(_AFX_DEBUG_STATE, afxDebugState)

#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// _AFX_WIN_STATE

#undef AFX_DATA
#define AFX_DATA

class _AFX_WIN_STATE : public CNoTrackObject
{
#ifndef _AFX_NO_GRAYDLG_SUPPORT
public:
	_AFX_WIN_STATE();
	virtual ~_AFX_WIN_STATE();

	// gray dialog support
	HBRUSH m_hDlgBkBrush; // dialog and message box background brush
	COLORREF m_crDlgTextClr;
#endif //!_AFX_NO_GRAYDLG_SUPPORT

public:
	// printing abort
	BOOL m_bUserAbort;
};

EXTERN_PROCESS_LOCAL(_AFX_WIN_STATE, _afxWinState)

/////////////////////////////////////////////////////////////////////////////
// Type library cache - AFX_INTERNAL

#ifndef _AFX_NO_OLE_SUPPORT

struct ITypeInfo;
typedef ITypeInfo* LPTYPEINFO;

struct ITypeLib;
typedef ITypeLib* LPTYPELIB;

typedef struct _GUID GUID;
#ifndef _REFCLSID_DEFINED
#define REFGUID const GUID &
#endif

class CTypeLibCache
{
public:
	CTypeLibCache() : m_cRef(0), m_lcid((LCID)-1), m_ptlib(NULL), m_ptinfo(NULL) {}
	void Lock();
	void Unlock();
	BOOL Lookup(LCID lcid, LPTYPELIB* pptlib);
	void Cache(LCID lcid, LPTYPELIB ptlib);
	BOOL LookupTypeInfo(LCID lcid, REFGUID guid, LPTYPEINFO* pptinfo);
	void CacheTypeInfo(LCID lcid, REFGUID guid, LPTYPEINFO ptinfo);
	const GUID* m_pTypeLibID;

protected:
	LCID m_lcid;
	LPTYPELIB m_ptlib;
	GUID m_guidInfo;
	LPTYPEINFO m_ptinfo;
	long m_cRef;
};

#endif //!_AFX_NO_OLE_SUPPORT

/////////////////////////////////////////////////////////////////////////////
// AFX_MODULE_STATE : portion of state that is pushed/popped

// forward references required for AFX_MODULE_THREAD_STATE definition
class CWinThread;
class CHandleMap;
class CFrameWnd;

#ifndef _PNH_DEFINED
typedef int (__cdecl * _PNH)( size_t );
#define _PNH_DEFINED
#endif

template<class TYPE>
class CEmbeddedButActsLikePtr
{
public:
	AFX_INLINE TYPE* operator->() { return &m_data; }
	AFX_INLINE operator TYPE*() { return &m_data; }
	TYPE m_data;
};

// AFX_MODULE_THREAD_STATE (local to thread *and* module)
class AFX_MODULE_THREAD_STATE : public CNoTrackObject
{
public:
	AFX_MODULE_THREAD_STATE();
	virtual ~AFX_MODULE_THREAD_STATE();

	// current CWinThread pointer
	CWinThread* m_pCurrentWinThread;

	// list of CFrameWnd objects for thread
	CTypedSimpleList<CFrameWnd*> m_frameList;

	// temporary/permanent map state
	DWORD m_nTempMapLock;           // if not 0, temp maps locked
	CHandleMap* m_pmapHWND;
	CHandleMap* m_pmapHMENU;
	CHandleMap* m_pmapHDC;
	CHandleMap* m_pmapHGDIOBJ;
	CHandleMap* m_pmapHIMAGELIST;

	// thread-local MFC new handler (separate from C-runtime)
	_PNH m_pfnNewHandler;

#ifndef _AFX_NO_SOCKET_SUPPORT
	// WinSock specific thread state
	HWND m_hSocketWindow;
#ifdef _AFXDLL
	CEmbeddedButActsLikePtr<CMapPtrToPtr> m_pmapSocketHandle;
	CEmbeddedButActsLikePtr<CMapPtrToPtr> m_pmapDeadSockets;
	CEmbeddedButActsLikePtr<CPtrList> m_plistSocketNotifications;
#else
	CMapPtrToPtr* m_pmapSocketHandle;
	CMapPtrToPtr* m_pmapDeadSockets;
	CPtrList* m_plistSocketNotifications;
#endif
#endif
};

// forward references required for AFX_MODULE_STATE definition
class CWinApp;
class COleObjectFactory;

class CWnd;

#ifdef _AFXDLL
class CDynLinkLibrary;
#endif

#ifndef _AFX_NO_OCC_SUPPORT
class COccManager;
class COleControlLock;
#endif

#ifndef _AFX_NO_DAO_SUPPORT
class _AFX_DAO_STATE;
#endif

class CTypeLibCacheMap : public CMapPtrToPtr
{
public:
	virtual void RemoveAll(void* pExcept);
};

// AFX_MODULE_STATE (global data for a module)
class AFX_MODULE_STATE : public CNoTrackObject
{
public:
#ifdef _AFXDLL
	AFX_MODULE_STATE(BOOL bDLL, WNDPROC pfnAfxWndProc, DWORD dwVersion);
	AFX_MODULE_STATE(BOOL bDLL, WNDPROC pfnAfxWndProc, DWORD dwVersion,
		BOOL bSystem);
#else
	AFX_MODULE_STATE(BOOL bDLL);
#endif
	~AFX_MODULE_STATE();

	CWinApp* m_pCurrentWinApp;
	HINSTANCE m_hCurrentInstanceHandle;
	HINSTANCE m_hCurrentResourceHandle;
	LPCTSTR m_lpszCurrentAppName;
	BYTE m_bDLL;    // TRUE if module is a DLL, FALSE if it is an EXE
	BYTE m_bSystem; // TRUE if module is a "system" module, FALSE if not
	BYTE m_bReserved[2]; // padding

	DWORD m_fRegisteredClasses; // flags for registered window classes

	// runtime class data
#ifdef _AFXDLL
	CRuntimeClass* m_pClassInit;
#endif
	CTypedSimpleList<CRuntimeClass*> m_classList;

	// OLE object factories
#ifndef _AFX_NO_OLE_SUPPORT
#ifdef _AFXDLL
	COleObjectFactory* m_pFactoryInit;
#endif
	CTypedSimpleList<COleObjectFactory*> m_factoryList;
#endif
	// number of locked OLE objects
	long m_nObjectCount;
	BOOL m_bUserCtrl;

	// AfxRegisterClass and AfxRegisterWndClass data
	TCHAR m_szUnregisterList[4096];

	// variables related to a given process in a module
	//  (used to be AFX_MODULE_PROCESS_STATE)
#ifdef _AFX_OLD_EXCEPTIONS
	// exceptions
	AFX_TERM_PROC m_pfnTerminate;
#endif
	void (PASCAL *m_pfnFilterToolTipMessage)(MSG*, CWnd*);

#ifdef _AFXDLL
	// CDynLinkLibrary objects (for resource chain)
	CTypedSimpleList<CDynLinkLibrary*> m_libraryList;

	// special case for MFCxxLOC.DLL (localized MFC resources)
	HINSTANCE m_appLangDLL;
#endif

#ifndef _AFX_NO_OCC_SUPPORT
	// OLE control container manager
	COccManager* m_pOccManager;
	// locked OLE controls
	CTypedSimpleList<COleControlLock*> m_lockList;
#endif

#ifndef _AFX_NO_DAO_SUPPORT
	_AFX_DAO_STATE* m_pDaoState;
#endif

#ifndef _AFX_NO_OLE_SUPPORT
	// Type library caches
	CTypeLibCache m_typeLibCache;
	CTypeLibCacheMap* m_pTypeLibCacheMap;
#endif

	// define thread local portions of module state
	THREAD_LOCAL(AFX_MODULE_THREAD_STATE, m_thread)
};

AFX_MODULE_STATE* AFXAPI AfxGetAppModuleState();
#ifdef _AFXDLL
AFX_MODULE_STATE* AFXAPI AfxSetModuleState(AFX_MODULE_STATE* pNewState);
#endif
AFX_MODULE_STATE* AFXAPI AfxGetModuleState();
AFX_MODULE_STATE* AFXAPI AfxGetStaticModuleState();

AFX_MODULE_THREAD_STATE* AFXAPI AfxGetModuleThreadState();

#ifdef _AFXDLL
#define _AFX_CMDTARGET_GETSTATE() (m_pModuleState)
#else
#define _AFX_CMDTARGET_GETSTATE() (AfxGetModuleState())
#endif

/////////////////////////////////////////////////////////////////////////////
// macros & classes to manage pushing/popping the module state

/////////////////////////////////////////////////////////////////////////////
// Thread global state

// forward references required for _AFX_THREAD_STATE definition
class CView;
class CToolTipCtrl;
class CControlBar;
#define tagMSG tagMSG2
#define MSG MSG2
#define PMSG PMSG2
#define NPMSG NPMSG2
#define LPMSG LPMSG2
typedef struct tagMSG {
  HWID        hwnd;
  UINT        message;
  WPARAM      wParam;
  LPARAM      lParam;
  DWORD       time;
  POINT       pt;
#ifdef _MAC
  DWORD       lPrivate;
#endif
} MSG, *PMSG, *NPMSG, *LPMSG;
#define DELETE_EXCEPTION(e) do { e->Delete(); } while (0)
class _AFX_THREAD_STATE : public CNoTrackObject
{
public:
	_AFX_THREAD_STATE();
	virtual ~_AFX_THREAD_STATE();

	// override for m_pModuleState in _AFX_APP_STATE
	AFX_MODULE_STATE* m_pModuleState;
	AFX_MODULE_STATE* m_pPrevModuleState;

	// memory safety pool for temp maps
	void* m_pSafetyPoolBuffer;    // current buffer

	// thread local exception context
	AFX_EXCEPTION_CONTEXT m_exceptionContext;

	// CWnd create, gray dialog hook, and other hook data
	CWnd* m_pWndInit;
	CWnd* m_pAlternateWndInit;      // special case commdlg hooking
	DWORD m_dwPropStyle;
	DWORD m_dwPropExStyle;
	HWND m_hWndInit;
	BOOL m_bDlgCreate;

	// other CWnd modal data
	MSG m_lastSentMsg;              // see CWnd::WindowProc
	HWND m_hTrackingWindow;         // see CWnd::TrackPopupMenu
	HMENU m_hTrackingMenu;
	TCHAR m_szTempClassName[96];    // see AfxRegisterWndClass
	HWND m_hLockoutNotifyWindow;    // see CWnd::OnCommand
	BOOL m_bInMsgFilter;

	// other framework modal data
	CView* m_pRoutingView;          // see CCmdTarget::GetRoutingView
	CFrameWnd* m_pRoutingFrame;     // see CCmdTarget::GetRoutingFrame

	// MFC/DB thread-local data
	BOOL m_bWaitForDataSource;

	// common controls thread state
	CToolTipCtrl* m_pToolTip;
	CWnd* m_pLastHit;       // last window to own tooltip
	int m_nLastHit;         // last hittest code
	TOOLINFO m_lastInfo;    // last TOOLINFO structure
	int m_nLastStatus;      // last flyby status message
	CControlBar* m_pLastStatus; // last flyby status control bar

	// OLE control thread-local data
	CWnd* m_pWndPark;       // "parking space" window
	long m_nCtrlRef;        // reference count on parking window
	BOOL m_bNeedTerm;       // TRUE if OleUninitialize needs to be called
};

EXTERN_THREAD_LOCAL(_AFX_THREAD_STATE, _afxThreadState)

_AFX_THREAD_STATE* AFXAPI AfxGetThreadState();

/////////////////////////////////////////////////////////////////////////////

#endif // _AFX_H_

