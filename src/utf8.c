/*
    Copyright © 2013 Syd Logan. All Rights Reserved.

    Redistribution and use in source and binary forms, with or without
modification, are permitted provided that the following conditions are met:

    1. Redistributions of source code must retain the above copyright notice,
this list of conditions and the following disclaimer.

    2. Redistributions in binary form must reproduce the above copyright
notice, this list of conditions and the following disclaimer in the
documentation and/or other materials provided with the distribution.

    3. The name of the author may not be used to endorse or promote products
derived from this software without specific prior written permission.

    THIS SOFTWARE IS PROVIDED BY Syd Logan "AS IS" AND ANY EXPRESS OR IMPLIED
WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF
MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO
EVENT SHALL THE AUTHOR BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR
BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER
IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
POSSIBILITY OF SUCH DAMAGE.
*/

#include <stdio.h>
#include <string.h>
#include <stdlib.h>

/**
 * Convert a string into a UTF-8 byte sequence.
 *
 * @param[in] p string to convert
 * @param[out] lenout length of created string
 *
 * @return UTF-8 byte string or NULL if failure.
 * 
 * @note routine expects "\u" to be removed by called.
 * @note caller must free memory returned by this function.
 * @note we accept strings <= 4 in length.
 */

char *
FromStrToUnicode(char *p, int *lenout)
{
    char *q = p;
    char *ret = NULL;
    int len;
    unsigned int tmp = 0;       

    *lenout = 0;

    if (!q) {
        goto out;
    }

    if ((len = strlen(q)) == 0 || len > 4) {
        goto out;
    }

    for (int i = 0; i < len; i++) {
        if (*(q+i) >= '0' && *(q+i) <= '9') {
            tmp |= *(q+i) - '0';
        } else if (*(q+i) >= 'a' && *(q+i) <= 'f') {
            tmp |= (*(q+i) - 'a') + 0x0a;
        } else if (*(q+i) >= 'A' && *(q+i) <= 'F') {
            tmp |= (*(q+i) - 'A') + 0x0a;
        } else {
            printf("%s: unrecognized utf-8 string\n", __FUNCTION__);
            goto out;
        }
        if (i != len - 1) {
            tmp = tmp << 4;
        }
    }

    len = 0;
    if (tmp >=0 && tmp <= 0x7f) {
        len = 1;
    } else if (tmp >= 0x80 && tmp <= 0x7ff) {
        len = 2;
    } else if (tmp >= 0x0800 && tmp <= 0xffff) {
        len = 3;
    } else {
        // unsupported range, goto out
        goto out;
    }

    *lenout = len;

    ret = q = malloc(len);
    if (ret == (char *) NULL) {
        goto out;
    }

    if (len == 1) {
        *q = tmp & 0x7f;
    } else if (len == 2) {
        *q = 0xC0;
        *q |= (tmp & 0x7C0) >> 6;
        q++;
        *q |= 0x80;
        *q |= tmp & 0x003f;
    } else if (len == 3) {
        *q = 0xE0;
        *q |= (tmp & 0xf000) >> 12;
        q++;
        *q |= 0x80;
        *q |= (tmp & 0x0fc0) >> 6;
        q++;
        *q |= 0x80;
        *q |= tmp & 0x003f;
    } 
out:
    return ret;
}

/**
 *
 * Convert 3 byte (or less) UTF-8 byte stream to a hex string in format
 * "\uabcd".
 *
 * @param[in] u byte stream, not null terminated
 *
 * @return ascii hex string representing the unicode byte stream
 */

char *
FromUnicodeToStr(char *u)
{
    char *q = u;
    char *ret = NULL;
    char *p;
    unsigned char c; 
    int len;

    len = 7;
    ret = malloc(len);
    if (ret == (char *) NULL) {
        goto out;
    }

    ret[0] = '\\';
    ret[1] = 'u';
    ret[2] = 0;
    ret[3] = 0;
    ret[6] = '\0';

    if ((*q & 0x80) == 0x00) {
        c = *q & 0x7f;
        ret[4] = (c & 0xF0) >> 4;
        ret[5] = c & 0x0F;
    } else if ((*q & 0xe0) == 0xc0) {
        c = (*q & 0x1C) >> 2;
        c |= (*q & 0x03) << 6;
        q++;
        c |= (*q & 0x3F);
        ret[4] = (c & 0xF0) >> 4;
        ret[5] = c & 0x0F;
    } else if ((*q & 0xf0) == 0xe0) {
        ret[2] = *q & 0x0F;
        q++;
        ret[3] = (*q & 0x3C) >> 2;
        c = (*q & 0x3) << 6;
        q++;
        c |= (*q & 0x3F);
        ret[4] = (c & 0xf0) >> 4;
        ret[5] = c & 0x0f;
    } else {
        // unsupported, len > 3 
        free(ret);
        ret = NULL;
        goto out;
    }

    // map to ASCII 

    q = (char *) &ret[2];

    int i = 2;
    while (i < len - 1) {
        if (ret[i] >= 0 && ret[i] <= 9) {
            ret[i] += '0';
        } else if (ret[i] >= 0x0a && ret[i] <= 0x0f) {
            ret[i] = 'A' + (ret[i] - 0x0a);
        }
        i++;
    } 
out:
    return ret;
}

#if defined(TEST)

/* 
   Test consists of passing the 4 hex digits of a unicode string as
   argv[1] and verifying that the output displayed to stdout is the
   same. XXX do something better like with cunit.
*/

void usage(char *name);

int
main(int argc, char *argv[])
{
    char u[4];
    int len;
    char *p, *q;

    if (argc != 2) {
        usage(argv[0]);
        exit(1);
    }
    p = FromStrToUnicode(argv[1], &len);
    if (p != (char *) NULL) {
        q = FromUnicodeToStr(p);
        if (q != (char *) NULL) {
            printf("q is %s\n", q);
            free(q);
        } else {
            printf("%s: unable to malloc unicode str\n", __FUNCTION__);
        }
        free(p);
    } else {
        printf("%s: unable to malloc unicode bytestream\n", __FUNCTION__);
    }
}

void
usage(char *name)
{
    printf("usage: %s utfstring\n", name);
}
#endif
