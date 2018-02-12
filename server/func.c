
int u2c(unsigned short port, char *ch, int len)
{
	unsigned short tmp_port;

	do {
		tmp_port = port % 10;
		port = port / 10;
		switch (tmp_port) {
		case 1:
			*ch = '1';
			break;
		case 2:
			*ch = '2';
			break;
		case 3:
			*ch = '3';
			break;
		case 4:
			*ch = '4';
			break;
		case 5:
			*ch = '5';
			break;
		case 6:
			*ch = '6';
			break;
		case 7:
			*ch = '7';
			break;
		case 8:
			*ch = '8';
			break;
		case 9:
			*ch = '9';
			break;
		case 0:
			*ch = '0';
			break;
		}
		ch++;
	} while(port);
	*ch = '\0';
	return 0;
}
