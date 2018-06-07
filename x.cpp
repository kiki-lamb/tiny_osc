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

int main() {
	Oscillator<40000, int8_t> osc;
	
	osc.set_hz(200);
	
	for(size_t x = 0; x < 200; x++)
		printf("%d\n", osc.read());
	//printf("%d\n", osc.read());
	
}

#endif
