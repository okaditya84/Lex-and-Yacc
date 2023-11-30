
#include <stdio.h>

int main() {
  int num1, num2, sum, diff;
  float ratio;

  printf("Enter first number: ");
  scanf("%d", &num1);

  printf("Enter second number: ");
  scanf("%d", &num2);

  sum = num1 + num2;
  diff = num1 - num2;

  ratio = (float) sum / diff;

  printf("Sum of %d and %d is %d\n", num1, num2, sum);
  printf("Difference of %d and %d is %d\n", num1, num2, diff);
  printf("Ratio of sum and difference is %.2f\n", ratio);

  return 0;
}
