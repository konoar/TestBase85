/****************************************************************
 *
 * main.c
 *   copyright 2019.07.11 konoar
 *
 ****************************************************************/
#include <stdio.h>
#include <string.h>

#define KS_BASE85_OK 0
#define KS_BASE85_NG 1
#define KS_BASE85_LL 10 // LineLength

const unsigned char LOOKUP[] =
{
	0x21, 0x22, 0x23, 0x24, 0x25, 0x26, 0x27, 0x28, // !"#$%&'(
	0x29, 0x2A, 0x2B, 0x2C, 0x2D, 0x2E, 0x2F, 0x30, // )*+,-./0
	0x31, 0x32, 0x33, 0x34, 0x35, 0x36, 0x37, 0x38, // 12345678
	0x39, 0x3A, 0x3B, 0x3C, 0x3D, 0x3E, 0x3F, 0x40, // 9:;<=>?@
	0x41, 0x42, 0x43, 0x44, 0x45, 0x46, 0x47, 0x48, // ABCDEFGH
	0x49, 0x4A, 0x4B, 0x4C, 0x4D, 0x4E, 0x4F, 0x50, // IJKLMNOP
	0x51, 0x52, 0x53, 0x54, 0x55, 0x56, 0x57, 0x58, // QRSTUVWX
	0x59, 0x5A, 0x5B, 0x5C, 0x5D, 0x5E, 0x5F, 0x60, // YZ[/]^_`
	0x61, 0x62, 0x63, 0x64, 0x65, 0x66, 0x67, 0x68, // abcdefgh
	0x69, 0x6A, 0x6B, 0x6C, 0x6D, 0x6E, 0x6F, 0x70, // ijklmnop
	0x71, 0x72, 0x73, 0x74, 0x75                    // qrstu
};

int calc(const char c1, const char c2, const char c3, const char c4, char *a85)
{

	#define KS_SHIFT(_a_,_b_) (((unsigned int)_a_) << _b_)

	unsigned int val;

	if (0 == (c1 | c2 | c3 | c4)) {
		return KS_BASE85_NG;
	}

	val =  KS_SHIFT(c1, 24) | KS_SHIFT(c2, 16) | KS_SHIFT(c3, 8) | KS_SHIFT(c4, 0);

	for (int idx = 4; idx >= 0; idx--) {
		a85[idx] = LOOKUP[val % 85];
		val /= 85;
	}

	return KS_BASE85_OK;

	#undef KS_SHIFT

}

int main(int argc, const char *argv[])
{

	unsigned int len, cnt;
	char buff[5];

	void myput(const char c)
	{
		putc(c, stdout);

		if (++cnt % KS_BASE85_LL == 0) {
			putc('\n', stdout);
		}
	}

	if (argc < 2) {
		return 0;
	}

	len = strlen(argv[1]);
	cnt = 0;

	for (int idx = 0; idx < (len >> 2); idx++) {
		if (KS_BASE85_OK == calc(
			argv[1][idx * 4], argv[1][idx * 4 + 1], argv[1][idx * 4 + 2], argv[1][idx * 4 + 3], buff)) {
			myput(buff[0]);
			myput(buff[1]);
			myput(buff[2]);
			myput(buff[3]);
			myput(buff[4]);
		} else {
			myput('z');
		}
	}

	if (len % 4) {
		if (KS_BASE85_OK == calc(argv[1][len & 0xFFFFFFFC],
		         (len % 4 > 1) ? argv[1][len & 0xFFFFFFFC + 1] : 0,
		         (len % 4 > 2) ? argv[1][len & 0xFFFFFFFC + 2] : 0,
		         (len % 4 > 3) ? argv[1][len & 0xFFFFFFFC + 3] : 0, buff)) {
			myput(buff[0]);
			myput(buff[1]);
			if (len % 4 > 1) myput(buff[2]);
			if (len % 4 > 2) myput(buff[3]);
			if (len % 4 > 3) myput(buff[4]);
		} else {
			myput('z');
		}
	}

	if (cnt % KS_BASE85_LL) {
		putc('\n', stdout);
	}

	return 0;

}

