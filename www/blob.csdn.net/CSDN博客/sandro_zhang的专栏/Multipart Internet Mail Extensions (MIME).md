# Multipart Internet Mail Extensions (MIME) - sandro_zhang的专栏 - CSDN博客
2011年11月19日 21:40:29[sandro_zhang](https://me.csdn.net/sandro_zhang)阅读数：689
|**Description of Data Content**|**Typical Filename Extensions**||**MIME type/subtype**|
|----|----|----|----|
|||||
|**Text and Text-Related Types**||||
|HTML text data (RFC 1866)|html htm||text/html|
|Plain text: documents; program listings|txt c c++ pl cc h||text/plain|
|Richtext (obsolete - replaced by text/enriched)|||text/richtext|
|Structure enhanced text|(etx?)||text/x-setext|
|Enriched text markup (RFC 1896)|||text/enriched|
|Tab-separated values (tabular)|(tsv?)||text/tab-separated-values|
|SGML documents (RFC 1874)|||text/sgml|
|Speech synthesis data (*MVP Solutions*)|talk||text/x-speech|
|||||
|**Document Stylesheet Types**||||
|Cascading Stylesheets|css||text/css|
|DSSSL-online stylesheets|||application/dsssl *(proposed)*|
|||||
|**Image Types**||||
|GIF|gif||image/gif|
|X-Windows bitmap (b/w)|xbm||image/x-xbitmap|
|X-Windows pixelmap (8-bit color)|xpm||image/x-xpixmap|
|Portable Network Graphics|png||image/x-png|
|Image Exchange Format (RFC 1314)|ief||image/ief|
|JPEG|jpeg jpg jpe||image/jpeg|
|TIFF|tiff tif||image/tiff|
|RGB|rgb||image/rgb|
||||image/x-rgb|
|Group III Fax (RFC 1494)|g3f||image/g3fax|
|X Windowdump format|xwd||image/x-xwindowdump|
|Macintosh PICT format|pict||image/x-pict|
|PPM (UNIX PPM package)|ppm||image/x-portable-pixmap|
|PGM (UNIX PPM package)|pgm||image/x-portable-graymap|
|PBM (UNIX PPM package)|pbm||image/x-portable-bitmap|
|PNM (UNIX PPM package)|pnm||image/x-portable-anymap|
|Microsoft Windows bitmap|bmp||image/x-ms-bmp|
|CMU raster|ras||image/x-cmu-raster|
|Kodak Photo-CD|pcd||image/x-photo-cd|
|Computer Graphics Metafile|cgm||image/cgm|
|North Am. Presentation Layer Protocol|||image/naplps|
|CALS Type 1 or 2|mil cal||image/x-cals|
|Fractal Image Format (*Iterated Systems*)|fif||image/fif|
|QuickSilver active image (*Micrografx*)|dsf||image/x-mgx-dsf|
|CMX vector image (*Corel*)|cmx||image/x-cmx|
|Wavelet-compressed (*Summus*)|wi||image/wavelet|
|AutoCad Drawing (*SoftSource*)|dwg||image/vnd.dwg|
||||image/x-dwg|
|AutoCad DXF file (*SoftSource*)|dxf||image/vnd.dxf|
||||image/x-dxf|
|Simple Vector Format (*SoftSource*)|svf||image/vnd.svf|
||||*also* vector/x-svf|
|||||
|**Audio/Voice/Music Related Types**||||
|"basic"audio - 8-bit u-law PCM|au snd||audio/basic|
|Macintosh audio format (AIpple)|aif aiff aifc||audio/x-aiff|
|*Microsoft* audio|wav||audio/x-wav|
|MPEG audio|mpa abs mpega||audio/x-mpeg|
|MPEG-2 audio|mp2a mpa2||audio/x-mpeg-2|
|compressed speech (*Echo Speech Corp.*)|es||audio/echospeech|
|Toolvox speech audio (*Voxware*)|vox||audio/voxware|
|RapidTransit compressed audio (*Fast Man*)|lcc||application/fastman|
|Realaudio (*Progressive Networks*)|ra ram||application/x-pn-realaudio|
|NIFF music notation data format|||application/vnd.music-niff|
|MIDI music data|mmid||x-music/x-midi|
|Koan music data (*SSeyo*)|skp||application/vnd.koan|
||||application/x-koan|
|Speech synthesis data (*MVP Solutions*)|talk||text/x-speech|
|||||
|**Video Types**||||
|MPEG video|mpeg mpg mpe||video/mpeg|
|MPEG-2 video|mpv2 mp2v||video/mpeg-2|
|Macintosh Quicktime|qt mov||video/quicktime|
|Microsoft video|avi||video/x-msvideo|
|SGI Movie format|movie||video/x-sgi-movie|
|VDOlive streaming video (*VDOnet)*|vdo||video/vdo|
|Vivo streaming video (*Vivo software*)|viv||video/vnd.vivo|
||||video/vivo|
|**Special HTTP/Web Application Types**||||
|Proxy autoconfiguration (*Netscape* browsers)|pac||application/x-ns-proxy-autoconfig|
|See Chapter 6|||application/x-www-form-urlencoded|
|See Chapter 9|||application/x-www-local-exec|
|See Chapter 9 (*Netscape* extension)|||multipart/x-mixed-replace|
|See Chapter 9 and Appendix B|||multipart/form-data|
|Netscape Cooltalk chat data (*Netscape*)|ice||x-conference/x-cooltalk|
|Interactive chat (*Ichat*)|||application/x-chat|
|||||
|**Application Types**||||
|||||
|**Text-Related**||||
|PostScript|ai eps ps||application/postscript|
|*Microsoft* Rich Text Format|rtf||application/rtf|
|*Adobe* Acrobat PDF|pdf||application/pdf|
||||application/x-pdf|
|Maker Interchange Format (*FrameMaker*)|mif||application/vnd.mif|
||||application/x-mif|
|Troff document|t tr roff||application/x-troff|
|Troff document with MAN macros|man||application/x-troff-man|
|Troff document with ME macros|me||application/x-troff-me|
|Troff document with MS macros|ms||application/x-troff-ms|
|LaTeX document|latex||application/x-latex|
|Tex/LateX document|tex||application/x-tex|
|GNU TexInfo document|texinfo texi||application/x-texinfo|
|TeX dvi format|dvi||application/x-dvi|
|MacWrite document|??||application/macwriteii|
|MS word document|??||application/msword|
|WordPerfect 5.1 document|??||application/wordperfect5.1|
|SGML application (RFC 1874)|||application/sgml|
|Office Document Architecture|oda||application/oda|
|Envoy Document|evy||application/envoy|
|Wang Info. Tranfer Format (*Wang*)|||application/wita|
|DEC Document Transfer Format (*DEC*)|||application/dec-dx|
|IBM Document Content Architecture (*IBM*)|||application/dca-rft|
|||||
|CommonGround Digital Paper (*No Hands Software*)|||application/commonground|
|FrameMaker Documents (*Frame*)|doc fm frm frame||application/vnd.framemaker|
||||application/x-framemaker|
|Remote printing at arbitrary printers (RFC 1486)|||application/remote-printing|
|||||
|**Archive/Compressed Archives**||||
|Gnu tar format|gtar||application/x-gtar|
|4.3BSD tar format|tar||application/x-tar|
|POSIX tar format|ustar||application/x-ustar|
|Old CPIO format|bcpio||application/x-bcpio|
|POSIX CPIO format|cpio||application/x-cpio|
|UNIX sh shell archive|shar||application/x-shar|
|DOS/PC - Pkzipped archive|zip||application/zip|
|Macintosh Binhexed archive|hqx||application/mac-binhex40|
|Macintosh Stuffit Archive|sit sea||application/x-stuffit|
|Fractal Image Format|fif||application/fractals|
|Binary, UUencoded|bin uu||application/octet-stream|
|PC executable|exe||application/octet-stream|
|WAIS "sources"|src wsrc||application/x-wais-source|
|NCSA HDF data format|hdf||application/hdf|
|||||
|**Downloadable Program/Scripts**||||
|Javascript program|js ls mocha||text/javascript|
||||application/x-javascript|
|VBScript program|||text/vbscript|
|UNIX bourne shell program|sh||application/x-sh|
|UNIX c-shell program|csh||application/x-csh|
|Perl program|pl||application/x-perl|
|Tcl (Tool Control Language) program|tcl||application/x-tcl|
|Atomicmail program scripts (obsolete)|||application/atomicmail|
|Slate documents - executable enclosures (*BBN*)|||application/slate|
|Undefined binary data (often executable progs)|||application/octet-stream|
|RISC OS Executable programs (*ANT Limited*)|||application/riscos|
|||||
|**Animation/Multimedia**||||
|Andrew Toolkit inset|||application/andrew-inset|
|FutureSplash vector animation (*FutureWave*)|spl||application/futuresplash|
|mBED multimedia data (*mBED*)|mbd||application/mbedlet|
|Macromedia Shockwave (*Macromedia*)|||application/x-director|
|*Sizzler* real-time video/animation|||application/x-sprite|
|PowerMedia multimedia (*RadMedia*)|rad||application/x-rad-powermedia|
|||||
|**Presentation**||||
|PowerPoint presentation (*Microsoft*)|ppz||application/mspowerpoint|
|PointPlus presentation data (*Net Scene*)|css||application/x-pointplus|
|ASAP WordPower (*Software Publishing Corp.*)|asp||application/x-asap|
|Astound Web Player multimedia data (*GoldDisk*)|asn||application/astound|
|**Special Embedded Object **||||
|OLE script e.g. Visual Basic (*Ncompass*)|axs||application/x-olescript|
|OLE Object (*Microsoft/NCompass*)|ods||application/x-oleobject|
|OpenScape OLE/OCX objects (*Business@Web*)|opp||x-form/x-openscape|
|Visual Basic objects (*Amara*)|wba||application/x-webbasic|
|Specialized data entry forms (*Alpha Software*)|frm||application/x-alpha-form|
|client-server objects (*Wayfarer Communications*)|wfx||x-script/x-wfxclient|
|**General Applications**||||
|Undefined binary data (often executable progs)|||application/octet-stream|
|CALS (U.S. D.O.D data format - RFC 1895)|||application/cals-1840|
|Pointcast news data (*Pointcast*)|pcn||application/x-pcn|
|Excel spreadsheet (*Microsoft*)|||application/vnd.ms-excel|
||||application/x-msexcel|
||||application/ms-excel|
|PowerPoint (*Microsoft*)|ppt||application/vnd.ms-powerpoint|
||||application/ms-powerpoint|
|Microsoft Project (*Microsoft*)|||application/vnd.ms-project|
|Works data (*Microsoft*)|||application/vnd.ms-works|
|MAPI data (*Microsoft*)|||application/vnd.ms-tnef|
|Artgallery data (Microsoft)|||application/vnd.artgalry|
|SourceView document (*Dataware Electronics*)|svd||application/vnd.svd|
|Truedoc (*Bitstream*)|||application/vnd.truedoc|
|Net Install - software install (*20/20 Software*)|ins||application/x-net-install|
|Carbon Copy - remote control/access (*Microcom*)|ccv||application/ccv|
|Spreadsheets (*Visual Components*)|vts||workbook/formulaone|
|Cybercash digital money (*Cybercash*)|||application/cybercash|
|Format for sending generic Macintosh files|||application/applefile|
|Active message -- connect to active mail app.|||application/activemessage|
|X.400 mail message body part (RFC 1494)|||application/x400-bp|
|USENET news message id (RFC 1036)|||application/news-message-id|
|USENET news message (RFC 1036)|||application/news-transmission|
|||||
|**Multipart Types (mostly email)**||||
|Messages with multiple parts|||multipart/mixed|
|Messages with multiple, alternative parts|||multipart/alternative|
|Message with multiple, related parts|||multipart/related|
|Multiple parts are digests|||multipart/digest|
|For reporting of email status (admin.)|||multipart/report|
|Order of parts does not matter|||multipart/parallel|
|Macintosh file data|||multipart/appledouble|
|Aggregate messages; descriptor as header|||multipart/header-set|
|Container for voice-mail|||multipart/voice-message|
|HTML FORM data (see Ch. 9 and App. B)|||multipart/form-data|
|Infinite multiparts - See Chapter 9 (*Netscape*)|||multipart/x-mixed-replace|
|||||
|**Message Types (mostly email)**||||
|MIME message|||message/rfc822|
|Partial message|||message/partial|
|Message containing external references|||message/external-body|
|Message containing USENET news|||message/news|
|HTTP message|||message/http|
|||||
|**2D/3D Data/Virtual Reality Types**||||
|VRML data file|wrl vrml||x-world/x-vrml(changing to model/vrml)|
|WIRL - VRML data (*VREAM*)|vrw||x-world/x-vream|
|Play3D 3d scene data (*Play3D*)|p3d||application/x-p3d|
|Viscape Interactive 3d world data (*Superscape*)|svr||x-world/x-svr|
|WebActive 3d data (*Plastic Thought*)|wvr||x-world/x-wvr|
|QuickDraw3D scene data (*Apple*)|3dmf||x-world/x-3dmf|
|||||
|**Scientific/Math/CAD Types**||||
|Chemical types -- to communicate information about chemical models|||chemical/* (several subtypes)|
|Mathematica notebook|ma||application/mathematica|
|Computational meshes for numerical simulations|msh||x-model/x-mesh(evolving to model/mesh)|
|Vis5D 5-dimensional data|v5d||application/vis5d|
|IGES models -- CAD/CAM (CGM) data|igs||application/iges(evolving to model/iges?)|
|Autocad WHIP vector drawings|dwf||drawing/x-dwf|
|||||
|**Largely Platform-Specific Types**||||
|**Silicon Graphics Specific Types**||||
|Showcase Presentations|showcase slides sc sho show||application/x-showcase|
|Insight Manual pages|ins insight||application/x-insight|
|Iris Annotator data|ano||application/x-annotator|
|Directory Viewer|dir||application/x-dirview|
|Software License|lic||application/x-enterlicense|
|Fax manager file|faxmgr||application/x-fax-manager|
|Fax job data file|faxmgrjob||application/x-fax-manager-job|
|IconBook data|icnbk||application/x-iconbook|
|?|wb||application/x-inpview|
|Installable software in 'inst' format|inst||application/x-install|
|Mail folder|mail||application/x-mailfolder|
|?|pp ppages||application/x-ppages|
|Data for printer (via lpr)|sgi-lpr||application/x-sgi-lpr|
|Software in 'tardist' format|tardist||application/x-tardist|
|Software in compressed 'tardist' format|ztardist||application/x-ztardist|
|WingZ spreadsheet|wkz||application/x-wingz|
|Open Inventor 3-D scenes|iv||graphics/x-inventor|
感谢：http://www.utoronto.ca/webdocs/HTMLdocs/Book/Book-3ed/appb/mimetype.html
            
