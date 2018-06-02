/*
 * $Id: national.h,v 1.5 2004/04/13 17:23:58 broeker Exp $
 */

/* GNUPLOT - national.h */




#ifndef GNUPLOT_NATIONAL_H
# define GNUPLOT_NATIONAL_H

/* if setlocale is available, these strings will be overridden
 * by strftime, or they may not be used at all if the run-time
 * system provides global variables with these strings
 */

#ifdef NORWEGIAN
/* MONTH-names */
#define AMON01 "Jan"
#define AMON02 "Feb"
#define AMON03 "Mar"
#define AMON04 "Apr"
#define AMON05 "Mai"
#define AMON06 "Jun"
#define AMON07 "Jul"
#define AMON08 "Aug"
#define AMON09 "Sep"
#define AMON10 "Okt"
#define AMON11 "Nov"
#define AMON12 "Des"
/* sorry, I dont know the full names */
#define FMON01 "January"
#define FMON02 "February"
#define FMON03 "March"
#define FMON04 "April"
#define FMON05 "May"
#define FMON06 "June"
#define FMON07 "July"
#define FMON08 "August"
#define FMON09 "September"
#define FMON10 "October"
#define FMON11 "November"
#define FMON12 "December"


/* DAY names */
#define ADAY0 "Sxn"
#define ADAY1 "Man"
#define ADAY2 "Tir"
#define ADAY3 "Ons"
#define ADAY4 "Tor"
#define ADAY5 "Fre"
#define ADAY6 "Lxr"

#define FDAY0 "Sunday"
#define FDAY1 "Monday"
#define FDAY2 "Tuesday"
#define FDAY3 "Wednesday"
#define FDAY4 "Thursday"
#define FDAY5 "Friday"
#define FDAY6 "Saturday"


#elif defined(HUNGARIAN)

#define AMON01 "jan"
#define AMON02 "febr"
#define AMON03 "m&aacute;rc"
#define AMON04 "&aacute;pr"
#define AMON05 "m&aacute;j"
#define AMON06 "j&uacute;n"
#define AMON07 "j&uacute;l"
#define AMON08 "aug"
#define AMON09 "szept"
#define AMON10 "okt"
#define AMON11 "nov"
#define AMON12 "dec"

#define FMON01 "janu&aacute;r"
#define FMON02 "febru&aacute;r"
#define FMON03 "m&aacute;rcius"
#define FMON04 "&aacute;prilis"
#define FMON05 "m&aacute;jus"
#define FMON06 "j&uacute;nius"
#define FMON07 "j&uacute;lius"
#define FMON08 "augusztus"
#define FMON09 "szeptember"
#define FMON10 "okt&oacute;ber"
#define FMON11 "november"
#define FMON12 "december"


/* DAY names */
#define ADAY0 "vas"
#define ADAY1 "h&eacute;t"
#define ADAY2 "kedd"
#define ADAY3 "sze"
#define ADAY4 "cs&uuml;t"
#define ADAY5 "p&eacute;n"
#define ADAY6 "szo"

#define FDAY0 "vas&aacute;rnap"
#define FDAY1 "h&eacute;tf&otilde;"
#define FDAY2 "kedd"
#define FDAY3 "szerda"
#define FDAY4 "cs&uacute;t&ouml;rt&ouml;k"
#define FDAY5 "p&eacute;ntek"
#define FDAY6 "szombat"

#else

/* MONTH-names */
#define AMON01 "Jan"
#define AMON02 "Feb"
#define AMON03 "Mar"
#define AMON04 "Apr"
#define AMON05 "May"
#define AMON06 "Jun"
#define AMON07 "Jul"
#define AMON08 "Aug"
#define AMON09 "Sep"
#define AMON10 "Oct"
#define AMON11 "Nov"
#define AMON12 "Dec"

#define FMON01 "January"
#define FMON02 "February"
#define FMON03 "March"
#define FMON04 "April"
#define FMON05 "May"
#define FMON06 "June"
#define FMON07 "July"
#define FMON08 "August"
#define FMON09 "September"
#define FMON10 "October"
#define FMON11 "November"
#define FMON12 "December"


/* DAY names */
#define ADAY0 "Sun"
#define ADAY1 "Mon"
#define ADAY2 "Tue"
#define ADAY3 "Wed"
#define ADAY4 "Thu"
#define ADAY5 "Fri"
#define ADAY6 "Sat"

#define FDAY0 "Sunday"
#define FDAY1 "Monday"
#define FDAY2 "Tuesday"
#define FDAY3 "Wednesday"
#define FDAY4 "Thursday"
#define FDAY5 "Friday"
#define FDAY6 "Saturday"
#endif /* language */

#endif /* GNUPLOT_NATIONAL_H */
