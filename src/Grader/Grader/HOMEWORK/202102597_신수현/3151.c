#include <stdio.h>


int main(int argc, char * argv[]) {
    int result, num1, num2;

    printf("���� one: ");
    scanf("%d", &num1);
    printf("���� two: ");
    scanf("%d", &num2);

    result = num1 + num2;
    printf("%d + %d = %d\n", num1, num2, result);

    return 0;
}
