#include <stdio.h>
#include <kernel.h>
#include <proc.h>
#include <sched.h>
#include <math.h>
#include <q.h>
void setschedclass (int sched_class)
{
	schedtype = sched_class;
}
int getschedclass() 
{
	return schedtype;
}

int expschednxproc()
{
	//int tail = queuetail(q);
	//int head = queuehead(q);

	int flag=1;
	if(flag==0)
	{
		return 0;
	}
	int expdistrand = expdev(0.1);
	int curr_iter = firstid(rdyhead);
	if(flag==0)
	{
		return 0;
	}
	int next_iter=curr_iter;
	if(flag==1)
	{
		while(curr_iter<NPROC && (flag==1))
		{
			if(q[curr_iter].qkey > expdistrand )
			{
				if(flag==1)

				{
					int iter = q[curr_iter].qnext;
					if((iter == rdytail) && (flag==1))
					{
						if(flag==0)
						{
							return 0;
						}
						break;
					}
					while(q[curr_iter].qkey == q[iter].qkey && (flag==1))
					{
						curr_iter = iter;
						iter = q[curr_iter].qnext;
					}
					next_iter = curr_iter;
					break;

				}
			}
			else if((q[curr_iter].qkey < expdistrand) || (q[curr_iter].qkey == expdistrand ))
			{

				curr_iter = q[curr_iter].qnext;
				if(flag==0)
				{
					return 0;
				} 
				next_iter = curr_iter;
			}
		}
		if(next_iter == rdytail)
		{
			next_iter = q[curr_iter].qprev;
		}


		return next_iter;

	}
}

int linuxschednxproc()
{
	int maxgoodness=0;
	int iter=0;
	int curr_iter = q[rdytail].qprev;
	while(curr_iter != rdyhead)
	{
		int flag=1;
		struct pentry *qiter = &proctab[curr_iter];
		if(qiter->goodness > maxgoodness && (flag==1))
		{
			maxgoodness = qiter->goodness;
			iter = curr_iter;
		}
		curr_iter = q[curr_iter].qprev;
	}
	return iter;
}


void newepoch()
{
	int count=0;
	int i=1,flag=1;
	while(i<NPROC && flag==1)
	{
		proctab[i].prio_copy = proctab[i].pprio;
		if(proctab[i].pstate!=PRFREE)
		{
			proctab[i].quantum = (proctab[i].counter/2)+proctab[i].prio_copy;
			proctab[i].counter = proctab[i].quantum;
			proctab[i].goodness = proctab[i].counter + proctab[i].prio_copy;
			count++;
			epoch = epoch + proctab[i].quantum;

		}
		i++;
	}
}
