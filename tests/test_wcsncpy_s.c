/*------------------------------------------------------------------
 * test_wcsncpy_s
 * File 'wchar/wcsncpy_s.c'
 * Lines executed:96.43% of 56
 *
 *------------------------------------------------------------------
 */

#include "test_private.h"
#include "safe_str_lib.h"

#ifdef HAVE_WCSNCPY_S
# define HAVE_NATIVE 1
#else
# define HAVE_NATIVE 0
#endif
#include "test_msvcrt.h"

#define MAX   ( 128 )
#define LEN   ( 128 )

static wchar_t   str1[LEN];
static wchar_t   str2[LEN];
static wchar_t   dest[LEN];

int test_wcsncpy_s (void)
{
    errno_t rc;
    rsize_t nlen;
    int errs = 0;

/*--------------------------------------------------*/

    nlen = 5;
    if (use_msvcrt)
        printf("Using msvcrt...\n");
    rc = wcsncpy_s(NULL, LEN, str2, nlen);
    if ( use_msvcrt && rc == ESNULLP ) {
        printf("safec.dll overriding msvcrt.dll\n");
        use_msvcrt = false;
    }
    ERR_MSVC(ESNULLP, EINVAL);

/*--------------------------------------------------*/

    wcscpy(str1, L"aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa");

    nlen = 5;
    rc = wcsncpy_s(str1, 5, NULL, nlen);
    ERR_MSVC(ESNULLP, EINVAL);
    WCHECK_SLACK(str1, 5);

/*--------------------------------------------------*/

    wcscpy(str1, L"aaaa");
    str2[0] = L'\0';

    rc = wcsncpy_s(str1, 5, str2, 0);
    ERR(EOK);
    WEXPSTR(str1, L"");

    wcscpy(str1, L"a");
    rc = wcsncpy_s(str1, 1, str2, 0);
    ERR(EOK);
    WEXPSTR(str1, L"");

/*--------------------------------------------------*/

    wcscpy(str1, L"aaaaa");
    nlen = 5;
    rc = wcsncpy_s(str1, 5, NULL, nlen);
    ERR_MSVC(ESNULLP, EINVAL);
    WCHECK_SLACK(str1, 5);

/*--------------------------------------------------*/

    wcscpy(str1, L"untouched");
    nlen = 5;

    rc = wcsncpy_s(str1, 0, str2, nlen);
    ERR_MSVC(ESZEROL, EINVAL);
    WEXPSTR(str1, L"untouched");

/*--------------------------------------------------*/

    rc = wcsncpy_s(str1, (RSIZE_MAX_STR+1), str2, nlen);
    ERR_MSVC(ESLEMAX, 0);
    if (use_msvcrt) {
        WEXPSTR(str1, L"untouched");
    }

/*--------------------------------------------------*/

   wcscpy(str1, L"aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa");
   wcscpy(str2, L"bbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbb");

    rc = wcsncpy_s(str1, 5, str2, (RSIZE_MAX_STR+1));
    ERR_MSVC(ESLEMAX, 0);
    if (!use_msvcrt) {
        WEXPSTR(str1, L"");
    }

/*--------------------------------------------------*/

    wcscpy(str1, L"aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa");
    str2[0] = L'\0';
    nlen = 5;

    rc = wcsncpy_s(&str1[0], LEN/2, &str2[0], nlen);
    ERR(EOK)
    WCHECK_SLACK(str1, LEN/2);

/*--------------------------------------------------*/

    wcscpy(str1, L"aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa");
    nlen = 5;

    /* test overlap */
    rc = wcsncpy_s(str1, LEN, str1, nlen);
    ERR(ESOVRLP)
    WCHECK_SLACK(str1, LEN);

/*--------------------------------------------------*/

    wcscpy(str1, L"aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa");
    nlen = 18;

    rc = wcsncpy_s(&str1[0], LEN, &str1[5], nlen);
    ERR_MSVC(ESOVRLP, 0);
    if (!use_msvcrt) {
        WCHECK_SLACK(str1, LEN);
    }

/*--------------------------------------------------*/

    wcscpy(str1, L"keep it simple");
    str2[0] = L'\0';

    nlen = 10;
    rc = wcsncpy_s(str1, LEN, str2, nlen);
    ERR(EOK)
    WCHECK_SLACK(str1, LEN);

/*--------------------------------------------------*/

    str1[0] = L'\0';
    wcscpy(str2, L"keep it simple");

    nlen = 20;
    rc = wcsncpy_s(str1, LEN, str2, nlen);
    ERR(EOK)
    WEXPSTR(str1, str2);
    WCHECK_SLACK(&str1[14], LEN-14);

/*--------------------------------------------------*/

    wcscpy(str1, L"qqweqeqeqeq");
    wcscpy(str2, L"keep it simple");

    nlen = 32;
    rc = wcsncpy_s(str1, LEN, str2, nlen);
    ERR(EOK)
    WEXPSTR(str1, str2);
    WCHECK_SLACK(&str1[14], LEN-14);

/*--------------------------------------------------*/

    wcscpy(str1, L"qqweqeqeqeq");
    wcscpy(str2, L"keep it simple");

    rc = wcsncpy_s(str1, 1, str2, nlen);
    ERR_MSVC(ESNOSPC, ERANGE);
    WCHECK_SLACK(str1, 1);

/*--------------------------------------------------*/

    wcscpy(str1, L"qqweqeqeqeq");
    wcscpy(str2, L"keep it simple");

    rc = wcsncpy_s(str1, 2, str2, nlen);
    ERR_MSVC(ESNOSPC, ERANGE);
    WCHECK_SLACK(str1, 2);

/*--------------------------------------------------*/
/* TR example */

    wcscpy(dest, L"                            ");
    wcscpy(str1, L"hello");

    rc = wcsncpy_s(dest, 6, str1, 100);
    ERR(EOK)
    WEXPSTR(dest, str1);
    WCHECK_SLACK(&dest[5], 6-5);

/*--------------------------------------------------*/
/* TR example */

    wcscpy(dest, L"                            ");
    wcscpy(str2, L"goodbye");

    rc = wcsncpy_s(dest, 5, str2, 7);
    ERR_MSVC(ESNOSPC, ERANGE);
    WCHECK_SLACK(dest, 5);

/*--------------------------------------------------*/
/* TR example */

    wcscpy(dest, L"                            ");
    wcscpy(str2, L"goodbye");

    rc = wcsncpy_s(dest, 5, str2, 4);
    ERR(EOK)
    WEXPSTR(dest, L"good")
    WCHECK_SLACK(&dest[4], 5-4);

/*--------------------------------------------------*/

    wcscpy(dest, L"                            ");
    wcscpy(str2, L"good");

    /*   strnlen("good") < 5   */
    rc = wcsncpy_s(dest, 5, str2, 8);
    ERR(EOK)
    WEXPSTR(dest, L"good")
    WCHECK_SLACK(&dest[4], 5-4);

/*--------------------------------------------------*/

    wcscpy(str1, L"qq12345weqeqeqeq");
    wcscpy(str2, L"it");

    nlen = 10;
    rc = wcsncpy_s(str1, 10, str2, nlen);
    ERR(EOK)
    WEXPSTR(str1, str2)
    WCHECK_SLACK(&str1[2], 10-2);

/*--------------------------------------------------*/

    return (errs);
}

#ifndef __KERNEL__
/* simple hack to get this to work for both userspace and Linux kernel,
   until a better solution can be created. */
int main (void)
{
    return (test_wcsncpy_s());
}
#endif
