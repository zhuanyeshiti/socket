#include <stdio.h>

unsigned short c2u(unsigned short port, char *ch, int len)
{
	unsigned short tmp = 0;
	int mul = 1;
	do {
		switch (*ch) {
		case '1':
			tmp += 1 * mul;
			break;
		case '2':
			tmp += 2 * mul;
			break;
		case '3':
			tmp += 3 * mul;
			break;
		case '4':
			tmp += 4 * mul;
			break;
		case '5':
			tmp += 5 * mul;
			break;
		case '6':
			tmp += 6 * mul;
			break;
		case '7':
			tmp += 7 * mul;
			break;
		case '8':
			tmp += 8 * mul;
			break;
		case '9':
			tmp += 9 * mul;
			break;
		case '0':
			tmp += 0 * mul;
			break;
		}
		mul *= 10;
		ch++;
	} while(*ch != '\0');
	printf("%u\n", tmp);
	return tmp;
}
