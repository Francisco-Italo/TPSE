#include <stdio.h>
#define RECIPROCAL(number) (1.0 / (number))

int main(){
    float counter; /* Counter for our table */

    for (counter = 1.0; counter < 10.0;counter += 1.0) printf("1/%.1f = %.3f\n", counter, RECIPROCAL(counter));

    return (0);
}