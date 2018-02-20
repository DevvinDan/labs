#include <stdio.h>

void main(){
	char str[50] = "string";
	sprintf(str, "Adding: %s", str);
	printf("%s\n", str);

}
