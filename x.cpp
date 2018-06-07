#ifndef __AVR__

#include <stdint.h>
#include <stdlib.h>
#include <cstdio>
#include <cmath>

#include "../libraries/lambOS/src/tables/kl_sin256_int8.h"
#include "../libraries/lambOS/src/tables/kl_sin256_uint8.h"
#include "sample_type_traits.h"
#include "interfaces.h"
#include "osc.h"
#include "volatile_buff.h"

int main() {
	Oscillator<40000, int8_t> osc;
	VolatileBuff<20, int8_t> buff;

	osc.set_hz(200);

	printf ("Filling...\n");

	while (buff.writeable())
		buff.write(osc.read());
	
	printf("Done filling.\n\n");
	printf ("Emptying...\n");

	uint16_t c = 0;
	while (buff.readable())
		printf("%d => %d\n", ++c, buff.read());
	
	printf("Done emptying.\n");
	
}

#endif
