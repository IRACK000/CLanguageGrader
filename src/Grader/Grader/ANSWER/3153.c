/*
 * �Է� ���� �� ���� ���� num1, num2, num3�� ������� ���� ������ ����� ����ϴ� ���α׷��� �ۼ��غ���.
 * (num1-num2)*(num2+num3)*(num3%num1)
 */
#include <stdio.h>


int main(int argc, char * argv[]) {
    int num1, num2, num3;

    scanf("%d %d %d", &num1, &num2, &num3);
    printf("(num1-num2)*(num2+num3)*(num3%%num1) = %d\n", (num1-num2)*(num2+num3)*(num3%num1));

    return 0;
}
