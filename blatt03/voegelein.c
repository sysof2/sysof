#include <stdio.h>

int main(){
	#define AUTOR
	#include "macros.h"
	#undef AUTOR

	#define TITEL
	#include "macros.h"
	#undef TITEL

	#define S1
	#include "strophe.h"
	#undef S1

	#define Z1
	#include "macros.h"
	#undef Z2

	#define S2
	#include "strophe.h"
	#undef S2

	#define Z1
	#include "macros.h"
	#undef Z1

	#define S3
	#include "strophe.h"
	#undef S3

	#define Z1
	#include "macros.h"
	#undef Z1

	return 0;
}
