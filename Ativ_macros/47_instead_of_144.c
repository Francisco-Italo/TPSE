#include <stdio.h>

#define FIRST_PART 7
#define LAST_PART 5
#define ALL_PARTS_WITHOUT_PARENTHESIS FIRST_PART + LAST_PART
#define ALL_PARTS_BETWEEN_PARENTHESIS (FIRST_PART + LAST_PART)

int main() {
    printf("%d\n%d\n%d\n", FIRST_PART, LAST_PART, FIRST_PART+LAST_PART);

    printf("The square of all the parts (without parenthesis) is %d\n", ALL_PARTS_WITHOUT_PARENTHESIS * ALL_PARTS_WITHOUT_PARENTHESIS);

    printf("The square of all the parts (with parenthesis) is %d\n", ALL_PARTS_BETWEEN_PARENTHESIS * ALL_PARTS_BETWEEN_PARENTHESIS); 
    
    return (0);
}