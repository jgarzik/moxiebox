#ifndef __DEMO_H__
#define __DEMO_H__

class SIM_DESC {
public:
	int dummy;
};

extern void
sim_resume (SIM_DESC& sd, int step, int siggnal);

#endif // __DEMO_H__
