#include <stdio.h>
#include <stdlib.h>

/* run this program using the console pauser or add your own getch, system("pause") or input loop */

int main(int argc, char *argv[]) {
	int result;
	int num1, num2;
	
	printf("���� one: ");
	scanf("%d", &num1);
	printf("���� two: ");
	scanf("%d", &num2);
	
	result=num1+num2;
	printf("%d + %d = %d \n", num1, num2, result);
	return 0;
}
