#include <stdio.h>
#include <stdlib.h>

/* run this program using the console pauser or add your own getch, system("pause") or input loop */

//¹®Á¦ 5 3153
int main(int argc, char *argv[]) {
	int num1, num2, num3;
	scanf("%d", &num1);
	scanf("%d", &num2);
	scanf("%d", &num3);
	int result = (num1-num2)*(num1+num3)*(num3%num1);
	printf("%d", result);
}
