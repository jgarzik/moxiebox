
#include <string>
#include <stdio.h>
#include "demo.h"

int main (int argc, char *argv[])
{
	SIM_DESC sd;
	sim_resume(sd, 0, 0);
	return 0;
}
