#include <stdio.h>
#include <stdlib.h>

//¹®Á¦ 4 3152
int main(int argc, char *argv[]) {
	int num1, num2;
	scanf("%d", &num1);
	scanf("%d", &num2);
	int result1 = num1/num2;
	int result2 = num1%num2;
	printf("%d, %d", result1,result2);
}

