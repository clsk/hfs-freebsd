/*
 * Taken from Darwin fsck_hfs.  The Apple Public Source License applies.
 */

#include <sys/types.h>

#include "hfs_endian.h"

/*
 * utf_encodestr
 *
 * Encode a UCS-2 (Unicode) string to UTF-8
 */
int utf_encodestr(ucsp, ucslen, utf8p, utf8len)
	const u_int16_t * ucsp;
	size_t ucslen;
	u_int8_t * utf8p;
	size_t * utf8len;
{
	u_int8_t * bufstart;
	u_int16_t ucs_ch;
	int charcnt;
	
	bufstart = utf8p;
	charcnt = ucslen / 2;

	while (charcnt-- > 0) {
		ucs_ch = SWAP_BE16(*ucsp++);

		if (ucs_ch < 0x0080) {
			if (ucs_ch == '\0')
				continue;	/* skip over embedded NULLs */

			*utf8p++ = ucs_ch;
		} else if (ucs_ch < 0x800) {
			*utf8p++ = (ucs_ch >> 6)   | 0xc0;
			*utf8p++ = (ucs_ch & 0x3f) | 0x80;
		} else {	
			*utf8p++ = (ucs_ch >> 12)         | 0xe0;
			*utf8p++ = ((ucs_ch >> 6) & 0x3f) | 0x80;
			*utf8p++ = ((ucs_ch) & 0x3f)      | 0x80;
		}
	}
	
	*utf8len = utf8p - bufstart;

	return (0);
}


/*
 * utf_decodestr
 *
 * Decode a UTF-8 string back to UCS-2 (Unicode)
 */
int
utf_decodestr(utf8p, utf8len, ucsp, ucslen)
	const u_int8_t* utf8p;
	size_t utf8len;
	u_int16_t* ucsp;
	size_t *ucslen;
{
	u_int16_t* bufstart;
	u_int16_t ucs_ch;
	u_int8_t byte;

	bufstart = ucsp;

	while (utf8len-- > 0 && (byte = *utf8p++) != '\0') {
		/* check for ascii */
		if (byte < 0x80) {
			*ucsp++ = SWAP_BE16((u_int16_t)byte);
			continue;
		}

		switch (byte & 0xf0) {
		/*  2 byte sequence*/
		case 0xc0:
		case 0xd0:
			/* extract bits 6 - 10 from first byte */
			ucs_ch = (byte & 0x1F) << 6;  
			if (ucs_ch < 0x0080)
				return (-1); /* seq not minimal */
			break;
		/* 3 byte sequence*/
		case 0xe0:
			/* extract bits 12 - 15 from first byte */
			ucs_ch = (byte & 0x0F) << 6;

			/* extract bits 6 - 11 from second byte */
			if (((byte = *utf8p++) & 0xc0) != 0x80)
				return (-1);

			utf8len--;
			ucs_ch += (byte & 0x3F);
			ucs_ch <<= 6;
			if (ucs_ch < 0x0800)
				return (-1); /* seq not minimal */
			break;
		default:
			return (-1);
		}

		/* extract bits 0 - 5 from final byte */
		if (((byte = *utf8p++) & 0xc0) != 0x80)
			return (-1);

		utf8len--;
		ucs_ch += (byte & 0x3F);  
		*ucsp++ = SWAP_BE16(ucs_ch);
	}

	*ucslen = (u_int8_t*)ucsp - (u_int8_t*)bufstart;

	return (0);
}
