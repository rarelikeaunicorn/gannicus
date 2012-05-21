#include "move.h"

special::special(const char * n)
{
	build(n);
	init();
}

bool special::check(bool pos[5], bool neg[5], int t, int f, int* resource)
{
	for(int i = 0; i < 5; i++){
		if(button[i] == 1){
			if(!pos[i] && !neg[i]) return 0;
		}
	}
	if(t > tolerance) return 0;
	if(f > activation) return 0;
	if(resource[0] < cost) return 0;
	return 1;
}

super::super(const char * n)
{
	build(n);
	init();
}

bool super::check(bool pos[5], bool neg[5], int t, int f, int* resource)
{
	return special::check(pos, neg, t, f, resource);
}

void super::execute(move * last, int *& resource)
{
	printf("%i, %i\n", resource[0], cost);
	resource[0] -= 100;
	resource[0] -= cost;
	last->init();
}

