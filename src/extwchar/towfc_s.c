/*------------------------------------------------------------------
 * towfc_s.c
 *
 * September 2017, Reini Urban
 *
 * Copyright (c) 2017 by Reini Urban
 * All rights reserved.
 *
 * Permission is hereby granted, free of charge, to any person
 * obtaining a copy of this software and associated documentation
 * files (the "Software"), to deal in the Software without
 * restriction, including without limitation the rights to use,
 * copy, modify, merge, publish, distribute, sublicense, and/or
 * sell copies of the Software, and to permit persons to whom the
 * Software is furnished to do so, subject to the following
 * conditions:
 *
 * The above copyright notice and this permission notice shall be
 * included in all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
 * EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES
 * OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND
 * NONINFRINGEMENT.  IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT
 * HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY,
 * WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
 * FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR
 * OTHER DEALINGS IN THE SOFTWARE.
 *------------------------------------------------------------------
 */

#include "safeclib_private.h"

#include <ctype.h>
#include <wctype.h>

/* Unicode 10.0 */

/* created via test_towfc_s() ignore_f = 0 */
static const struct {
    unsigned short upper;
    unsigned short lower1;
    unsigned short lower2;
} tbl2[] = {
    { 0x00DF, 0x0073,0x0073 },	/* LATIN SMALL LETTER SHARP S */
    { 0x0130, 0x0069,0x0307 },	/* LATIN CAPITAL LETTER I WITH DOT ABOVE */
    { 0x0149, 0x02BC,0x006E },	/* LATIN SMALL LETTER N PRECEDED BY APOSTROPHE */
    { 0x01F0, 0x006A,0x030C },	/* LATIN SMALL LETTER J WITH CARON */
    { 0x0587, 0x0565,0x0582 },	/* ARMENIAN SMALL LIGATURE ECH YIWN */
    { 0x1E96, 0x0068,0x0331 },	/* LATIN SMALL LETTER H WITH LINE BELOW */
    { 0x1E97, 0x0074,0x0308 },	/* LATIN SMALL LETTER T WITH DIAERESIS */
    { 0x1E98, 0x0077,0x030A },	/* LATIN SMALL LETTER W WITH RING ABOVE */
    { 0x1E99, 0x0079,0x030A },	/* LATIN SMALL LETTER Y WITH RING ABOVE */
    { 0x1E9A, 0x0061,0x02BE },	/* LATIN SMALL LETTER A WITH RIGHT HALF RING */
    { 0x1E9E, 0x0073,0x0073 },	/* LATIN CAPITAL LETTER SHARP S */
    { 0x1F50, 0x03C5,0x0313 },	/* GREEK SMALL LETTER UPSILON WITH PSILI */
    { 0x1F80, 0x1F00,0x03B9 },	/* GREEK SMALL LETTER ALPHA WITH PSILI AND YPOGEGRAMMENI */
    { 0x1F81, 0x1F01,0x03B9 },	/* GREEK SMALL LETTER ALPHA WITH DASIA AND YPOGEGRAMMENI */
    { 0x1F82, 0x1F02,0x03B9 },	/* GREEK SMALL LETTER ALPHA WITH PSILI AND VARIA AND YPOGEGRAMMENI */
    { 0x1F83, 0x1F03,0x03B9 },	/* GREEK SMALL LETTER ALPHA WITH DASIA AND VARIA AND YPOGEGRAMMENI */
    { 0x1F84, 0x1F04,0x03B9 },	/* GREEK SMALL LETTER ALPHA WITH PSILI AND OXIA AND YPOGEGRAMMENI */
    { 0x1F85, 0x1F05,0x03B9 },	/* GREEK SMALL LETTER ALPHA WITH DASIA AND OXIA AND YPOGEGRAMMENI */
    { 0x1F86, 0x1F06,0x03B9 },	/* GREEK SMALL LETTER ALPHA WITH PSILI AND PERISPOMENI AND YPOGEGRAMMENI */
    { 0x1F87, 0x1F07,0x03B9 },	/* GREEK SMALL LETTER ALPHA WITH DASIA AND PERISPOMENI AND YPOGEGRAMMENI */
    { 0x1F88, 0x1F00,0x03B9 },	/* GREEK CAPITAL LETTER ALPHA WITH PSILI AND PROSGEGRAMMENI */
    { 0x1F89, 0x1F01,0x03B9 },	/* GREEK CAPITAL LETTER ALPHA WITH DASIA AND PROSGEGRAMMENI */
    { 0x1F8A, 0x1F02,0x03B9 },	/* GREEK CAPITAL LETTER ALPHA WITH PSILI AND VARIA AND PROSGEGRAMMENI */
    { 0x1F8B, 0x1F03,0x03B9 },	/* GREEK CAPITAL LETTER ALPHA WITH DASIA AND VARIA AND PROSGEGRAMMENI */
    { 0x1F8C, 0x1F04,0x03B9 },	/* GREEK CAPITAL LETTER ALPHA WITH PSILI AND OXIA AND PROSGEGRAMMENI */
    { 0x1F8D, 0x1F05,0x03B9 },	/* GREEK CAPITAL LETTER ALPHA WITH DASIA AND OXIA AND PROSGEGRAMMENI */
    { 0x1F8E, 0x1F06,0x03B9 },	/* GREEK CAPITAL LETTER ALPHA WITH PSILI AND PERISPOMENI AND PROSGEGRAMMENI */
    { 0x1F8F, 0x1F07,0x03B9 },	/* GREEK CAPITAL LETTER ALPHA WITH DASIA AND PERISPOMENI AND PROSGEGRAMMENI */
    { 0x1F90, 0x1F20,0x03B9 },	/* GREEK SMALL LETTER ETA WITH PSILI AND YPOGEGRAMMENI */
    { 0x1F91, 0x1F21,0x03B9 },	/* GREEK SMALL LETTER ETA WITH DASIA AND YPOGEGRAMMENI */
    { 0x1F92, 0x1F22,0x03B9 },	/* GREEK SMALL LETTER ETA WITH PSILI AND VARIA AND YPOGEGRAMMENI */
    { 0x1F93, 0x1F23,0x03B9 },	/* GREEK SMALL LETTER ETA WITH DASIA AND VARIA AND YPOGEGRAMMENI */
    { 0x1F94, 0x1F24,0x03B9 },	/* GREEK SMALL LETTER ETA WITH PSILI AND OXIA AND YPOGEGRAMMENI */
    { 0x1F95, 0x1F25,0x03B9 },	/* GREEK SMALL LETTER ETA WITH DASIA AND OXIA AND YPOGEGRAMMENI */
    { 0x1F96, 0x1F26,0x03B9 },	/* GREEK SMALL LETTER ETA WITH PSILI AND PERISPOMENI AND YPOGEGRAMMENI */
    { 0x1F97, 0x1F27,0x03B9 },	/* GREEK SMALL LETTER ETA WITH DASIA AND PERISPOMENI AND YPOGEGRAMMENI */
    { 0x1F98, 0x1F20,0x03B9 },	/* GREEK CAPITAL LETTER ETA WITH PSILI AND PROSGEGRAMMENI */
    { 0x1F99, 0x1F21,0x03B9 },	/* GREEK CAPITAL LETTER ETA WITH DASIA AND PROSGEGRAMMENI */
    { 0x1F9A, 0x1F22,0x03B9 },	/* GREEK CAPITAL LETTER ETA WITH PSILI AND VARIA AND PROSGEGRAMMENI */
    { 0x1F9B, 0x1F23,0x03B9 },	/* GREEK CAPITAL LETTER ETA WITH DASIA AND VARIA AND PROSGEGRAMMENI */
    { 0x1F9C, 0x1F24,0x03B9 },	/* GREEK CAPITAL LETTER ETA WITH PSILI AND OXIA AND PROSGEGRAMMENI */
    { 0x1F9D, 0x1F25,0x03B9 },	/* GREEK CAPITAL LETTER ETA WITH DASIA AND OXIA AND PROSGEGRAMMENI */
    { 0x1F9E, 0x1F26,0x03B9 },	/* GREEK CAPITAL LETTER ETA WITH PSILI AND PERISPOMENI AND PROSGEGRAMMENI */
    { 0x1F9F, 0x1F27,0x03B9 },	/* GREEK CAPITAL LETTER ETA WITH DASIA AND PERISPOMENI AND PROSGEGRAMMENI */
    { 0x1FA0, 0x1F60,0x03B9 },	/* GREEK SMALL LETTER OMEGA WITH PSILI AND YPOGEGRAMMENI */
    { 0x1FA1, 0x1F61,0x03B9 },	/* GREEK SMALL LETTER OMEGA WITH DASIA AND YPOGEGRAMMENI */
    { 0x1FA2, 0x1F62,0x03B9 },	/* GREEK SMALL LETTER OMEGA WITH PSILI AND VARIA AND YPOGEGRAMMENI */
    { 0x1FA3, 0x1F63,0x03B9 },	/* GREEK SMALL LETTER OMEGA WITH DASIA AND VARIA AND YPOGEGRAMMENI */
    { 0x1FA4, 0x1F64,0x03B9 },	/* GREEK SMALL LETTER OMEGA WITH PSILI AND OXIA AND YPOGEGRAMMENI */
    { 0x1FA5, 0x1F65,0x03B9 },	/* GREEK SMALL LETTER OMEGA WITH DASIA AND OXIA AND YPOGEGRAMMENI */
    { 0x1FA6, 0x1F66,0x03B9 },	/* GREEK SMALL LETTER OMEGA WITH PSILI AND PERISPOMENI AND YPOGEGRAMMENI */
    { 0x1FA7, 0x1F67,0x03B9 },	/* GREEK SMALL LETTER OMEGA WITH DASIA AND PERISPOMENI AND YPOGEGRAMMENI */
    { 0x1FA8, 0x1F60,0x03B9 },	/* GREEK CAPITAL LETTER OMEGA WITH PSILI AND PROSGEGRAMMENI */
    { 0x1FA9, 0x1F61,0x03B9 },	/* GREEK CAPITAL LETTER OMEGA WITH DASIA AND PROSGEGRAMMENI */
    { 0x1FAA, 0x1F62,0x03B9 },	/* GREEK CAPITAL LETTER OMEGA WITH PSILI AND VARIA AND PROSGEGRAMMENI */
    { 0x1FAB, 0x1F63,0x03B9 },	/* GREEK CAPITAL LETTER OMEGA WITH DASIA AND VARIA AND PROSGEGRAMMENI */
    { 0x1FAC, 0x1F64,0x03B9 },	/* GREEK CAPITAL LETTER OMEGA WITH PSILI AND OXIA AND PROSGEGRAMMENI */
    { 0x1FAD, 0x1F65,0x03B9 },	/* GREEK CAPITAL LETTER OMEGA WITH DASIA AND OXIA AND PROSGEGRAMMENI */
    { 0x1FAE, 0x1F66,0x03B9 },	/* GREEK CAPITAL LETTER OMEGA WITH PSILI AND PERISPOMENI AND PROSGEGRAMMENI */
    { 0x1FAF, 0x1F67,0x03B9 },	/* GREEK CAPITAL LETTER OMEGA WITH DASIA AND PERISPOMENI AND PROSGEGRAMMENI */
    { 0x1FB2, 0x1F70,0x03B9 },	/* GREEK SMALL LETTER ALPHA WITH VARIA AND YPOGEGRAMMENI */
    { 0x1FB3, 0x03B1,0x03B9 },	/* GREEK SMALL LETTER ALPHA WITH YPOGEGRAMMENI */
    { 0x1FB4, 0x03AC,0x03B9 },	/* GREEK SMALL LETTER ALPHA WITH OXIA AND YPOGEGRAMMENI */
    { 0x1FB6, 0x03B1,0x0342 },	/* GREEK SMALL LETTER ALPHA WITH PERISPOMENI */
    { 0x1FBC, 0x03B1,0x03B9 },	/* GREEK CAPITAL LETTER ALPHA WITH PROSGEGRAMMENI */
    { 0x1FC2, 0x1F74,0x03B9 },	/* GREEK SMALL LETTER ETA WITH VARIA AND YPOGEGRAMMENI */
    { 0x1FC3, 0x03B7,0x03B9 },	/* GREEK SMALL LETTER ETA WITH YPOGEGRAMMENI */
    { 0x1FC4, 0x03AE,0x03B9 },	/* GREEK SMALL LETTER ETA WITH OXIA AND YPOGEGRAMMENI */
    { 0x1FC6, 0x03B7,0x0342 },	/* GREEK SMALL LETTER ETA WITH PERISPOMENI */
    { 0x1FCC, 0x03B7,0x03B9 },	/* GREEK CAPITAL LETTER ETA WITH PROSGEGRAMMENI */
    { 0x1FD6, 0x03B9,0x0342 },	/* GREEK SMALL LETTER IOTA WITH PERISPOMENI */
    { 0x1FE4, 0x03C1,0x0313 },	/* GREEK SMALL LETTER RHO WITH PSILI */
    { 0x1FE6, 0x03C5,0x0342 },	/* GREEK SMALL LETTER UPSILON WITH PERISPOMENI */
    { 0x1FF2, 0x1F7C,0x03B9 },	/* GREEK SMALL LETTER OMEGA WITH VARIA AND YPOGEGRAMMENI */
    { 0x1FF3, 0x03C9,0x03B9 },	/* GREEK SMALL LETTER OMEGA WITH YPOGEGRAMMENI */
    { 0x1FF4, 0x03CE,0x03B9 },	/* GREEK SMALL LETTER OMEGA WITH OXIA AND YPOGEGRAMMENI */
    { 0x1FF6, 0x03C9,0x0342 },	/* GREEK SMALL LETTER OMEGA WITH PERISPOMENI */
    { 0x1FFC, 0x03C9,0x03B9 },	/* GREEK CAPITAL LETTER OMEGA WITH PROSGEGRAMMENI */
    { 0xFB00, 0x0066,0x0066 },	/* LATIN SMALL LIGATURE FF */
    { 0xFB01, 0x0066,0x0069 },	/* LATIN SMALL LIGATURE FI */
    { 0xFB02, 0x0066,0x006C },	/* LATIN SMALL LIGATURE FL */
    { 0xFB05, 0x0073,0x0074 },	/* LATIN SMALL LIGATURE LONG S T */
    { 0xFB06, 0x0073,0x0074 },	/* LATIN SMALL LIGATURE ST */
    { 0xFB13, 0x0574,0x0576 },	/* ARMENIAN SMALL LIGATURE MEN NOW */
    { 0xFB14, 0x0574,0x0565 },	/* ARMENIAN SMALL LIGATURE MEN ECH */
    { 0xFB15, 0x0574,0x056B },	/* ARMENIAN SMALL LIGATURE MEN INI */
    { 0xFB16, 0x057E,0x0576 },	/* ARMENIAN SMALL LIGATURE VEW NOW */
    { 0xFB17, 0x0574,0x056D },	/* ARMENIAN SMALL LIGATURE MEN XEH */
    { 0,0,0 }
};

static const struct {
    unsigned short upper;
    unsigned short lower1;
    unsigned short lower2;
    unsigned short lower3;
} tbl3[] = {
    { 0x0390, 0x03B9,0x0308,0x0301 },	/* GREEK SMALL LETTER IOTA WITH DIALYTIKA AND TONOS */
    { 0x03B0, 0x03C5,0x0308,0x0301 },	/* GREEK SMALL LETTER UPSILON WITH DIALYTIKA AND TONOS */
    { 0x1F52, 0x03C5,0x0313,0x0300 },	/* GREEK SMALL LETTER UPSILON WITH PSILI AND VARIA */
    { 0x1F54, 0x03C5,0x0313,0x0301 },	/* GREEK SMALL LETTER UPSILON WITH PSILI AND OXIA */
    { 0x1F56, 0x03C5,0x0313,0x0342 },	/* GREEK SMALL LETTER UPSILON WITH PSILI AND PERISPOMENI */
    { 0x1FB7, 0x03B1,0x0342,0x03B9 },	/* GREEK SMALL LETTER ALPHA WITH PERISPOMENI AND YPOGEGRAMMENI */
    { 0x1FC7, 0x03B7,0x0342,0x03B9 },	/* GREEK SMALL LETTER ETA WITH PERISPOMENI AND YPOGEGRAMMENI */
    { 0x1FD2, 0x03B9,0x0308,0x0300 },	/* GREEK SMALL LETTER IOTA WITH DIALYTIKA AND VARIA */
    { 0x1FD3, 0x03B9,0x0308,0x0301 },	/* GREEK SMALL LETTER IOTA WITH DIALYTIKA AND OXIA */
    { 0x1FD7, 0x03B9,0x0308,0x0342 },	/* GREEK SMALL LETTER IOTA WITH DIALYTIKA AND PERISPOMENI */
    { 0x1FE2, 0x03C5,0x0308,0x0300 },	/* GREEK SMALL LETTER UPSILON WITH DIALYTIKA AND VARIA */
    { 0x1FE3, 0x03C5,0x0308,0x0301 },	/* GREEK SMALL LETTER UPSILON WITH DIALYTIKA AND OXIA */
    { 0x1FE7, 0x03C5,0x0308,0x0342 },	/* GREEK SMALL LETTER UPSILON WITH DIALYTIKA AND PERISPOMENI */
    { 0x1FF7, 0x03C9,0x0342,0x03B9 },	/* GREEK SMALL LETTER OMEGA WITH PERISPOMENI AND YPOGEGRAMMENI */
    { 0xFB03, 0x0066,0x0066,0x0069 },	/* LATIN SMALL LIGATURE FFI */
    { 0xFB04, 0x0066,0x0066,0x006C },	/* LATIN SMALL LIGATURE FFL */
    { 0, 0,0,0, }
};

/* return the number of wide lower-case characters needed to foldcase
   the given uppercase character. */
int
iswfc(wint_t wc)
{
    /* the slow variant would walk the 2 loops */
    if (likely((wc < 0xdf) ||
               (wc > 0x0587 && wc < 0x1e96) ||
               (wc > 0x1FFC && wc < 0xFB00) ||
               (wc > 0xFB17) ))
        return 0;
    if (wc < 0x1e96) {
        if (wc == 0xdf ||
            wc == 0x130 ||
            wc == 0x149 ||
            wc == 0x1f0 ||
            wc == 0x587)
            return 2;
        else if (wc == 0x390 ||
                 wc == 0x3b0)
            return 3;
        else
            return 0;
    }
    if (wc <= 0x1e9a || wc == 0x1e9e || wc == 0x1f50)
        return 2;
    if (wc < 0x1f50)
        return 0;
    if (wc == 0x1f52 || wc == 0x1f54 || wc == 0x1f56)
        return 3;
    if (wc < 0x1f80)
        return 0;
    if (wc <= 0x1faf || (wc >= 0x1fb2 && wc < 0x1fb6))
        return 2;
    if (wc == 0x1fb7 || wc == 0x1fc7 || wc == 0x1fd2 || wc == 0x1fd3 || wc == 0x1fd7 ||
        wc == 0x1fe2 || wc == 0x1fe3 || wc == 0x1fe7 || wc == 0x1ff7)
        return 3;
    if (wc == 0x1fb6 || wc == 0x1fbc ||
        (wc >= 0x1fc2 && wc <= 0x1fc6) ||
        wc == 0x1fcc ||
        wc == 0x1fd6 ||
        wc == 0x1fe4 ||
        wc == 0x1fe6 ||
        (wc >= 0x1ff2 && wc <= 0x1ff4) ||
        wc == 0x1ff6 ||
        wc == 0x1ffc)
        return 2;
    if (wc < 0xfb00 || wc > 0xfb17)
        return 0;
    if (wc == 0xfb03 || wc == 0xfb04)
        return 3;
    return 2;
}

errno_t
towfc_s(wchar_t *restrict dest, rsize_t dmax, const wint_t src)
{
    int i;

    if (unlikely(dest == NULL)) {
        invoke_safe_str_constraint_handler("_towfc_s: "
                   "dest is null",
                   NULL, ESNULLP);
        return (ESNULLP);
    }
    if (unlikely(dmax < 4)) {
        invoke_safe_str_constraint_handler("_towfc_s: "
                   "dmax is < 4",
                   NULL, ESLEMIN);
        return (ESLEMIN);
    }
    dest[0] = L'\0';
    if (unlikely(dmax > RSIZE_MAX_STR)) {
        invoke_safe_str_constraint_handler("_towfc_s: "
                   "dmax exceeds max",
                   NULL, ESLEMAX);
        return (ESLEMAX);
    }

    for (i=0; tbl2[i].upper; i++) {
        if (src == tbl2[i].upper) {
            dest[0] = tbl2[i].lower1;
            dest[1] = tbl2[i].lower2;
            dest[2] = L'\0';
            return EOK;
        }
    }
    for (i=0; tbl3[i].upper; i++) {
        if (src == tbl3[i].upper) {
            dest[0] = tbl3[i].lower1;
            dest[1] = tbl3[i].lower2;
            dest[2] = tbl3[i].lower2;
            dest[3] = L'\0';
            return EOK;
        }
    }

    dest[0] = src < 128 ? tolower(src) : _towcase(src, 1);
    dest[1] = L'\0';
    return (wint_t)dest[0] == src ? ESNOTFND : EOK;
}