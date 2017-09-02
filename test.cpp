#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <iostream>
#include "BigInteger.h"

using std::cout;
using std::endl;


int main(int argc , char *argv[]) {

    BigInteger bigint = 665;

    bigint.printHexadecimal();

    bigint.exp(3354);

    bigint.printHexadecimal();

	return EXIT_SUCCESS;
}



