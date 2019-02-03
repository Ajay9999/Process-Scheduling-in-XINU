#include <conf.h>
#include <kernel.h>
#include <proc.h>
#include <q.h>
#include <sched.h>
#include <math.h>
unsigned long currSP;	
extern int ctxsw(int, int, int, int);

int schedtype = 0;
int epoch =0;
int resched()
{


	int schedtype = getschedclass();

	if(schedtype == EXPDISTSCHED)
	{


		register struct    pentry    *oldptr;    
		register struct    pentry    *newptr;    



		oldptr= &proctab[currpid];
		if (oldptr->pstate == PRCURR) {

			oldptr->pstate = PRREADY;
			insert(currpid,rdyhead,oldptr->pprio);
		}

		int id = expschednxproc();
		dequeue(id);
		newptr = &proctab[ (currpid = id) ];
		newptr->pstate = PRCURR;       

#ifdef    RTCLOCK
		preempt = QUANTUM;        
#endif

		ctxsw((int)&oldptr->pesp, (int)oldptr->pirmask, (int)&newptr->pesp, (int)newptr->pirmask);


		return OK;

	}

	else if(schedtype == LINUXSCHED)
	{
		register struct	pentry	*optr;	
		register struct	pentry	*nptr;	

		optr=&proctab[currpid];
		int flag=1;
		if(flag==1)
		{
			epoch  -=(proctab[currpid].quantum-preempt);
		}


		if (proctab[currpid].pstate == PRCURR) {
			proctab[currpid].pstate = PRREADY;
			insert(currpid,rdyhead,optr->pprio);
		}


		if(epoch==0 && flag==1)
		{
			 newepoch();

		}		





		else if(epoch!=0 && flag==1)
		{

			proctab[currpid].goodness = proctab[currpid].goodness-(proctab[currpid].quantum-preempt);


			if(preempt>=0)
			{
				proctab[currpid].quantum=preempt;
				flag=1;
			}
			else if(preempt<0)
			{
				proctab[currpid].quantum=0;
			}

			proctab[currpid].counter = proctab[currpid].quantum;

			if(proctab[currpid].counter ==0)
			{
				proctab[currpid].goodness=0;
			}

			/*		if(currpid == NULLPROC)
					{
					proctab[currpid].goodness = 0;
					proctab[currpid].quantum = 0;
					proctab[currpid].counter = 0;
					}*/
		}
		nptr=&proctab[linuxschednxproc()];
		nptr=&proctab[currpid=dequeue(linuxschednxproc())];
		nptr->pstate=PRCURR;
		// flag=1;
#ifdef  RTCLOCK
		preempt = nptr->counter;              
#endif
		ctxsw((int)&optr->pesp, (int)optr->pirmask, (int)&nptr->pesp, (int)nptr->pirmask);


		return OK;			



	}

	else
	{
		register struct	pentry	*optr;	
		register struct	pentry	*nptr;

		optr= &proctab[currpid];
		if (optr->pstate == PRCURR) {

			optr->pstate = PRREADY;
			insert(currpid,rdyhead,optr->pprio);
		}

		if ( ( (optr= &proctab[currpid])->pstate == PRCURR) &&
				(lastkey(rdytail)<optr->pprio)) {
			return(OK);
		}



		if (optr->pstate == PRCURR) {
			optr->pstate = PRREADY;
			insert(currpid,rdyhead,optr->pprio);
		}



		nptr = &proctab[ (currpid = getlast(rdytail)) ];
		nptr->pstate = PRCURR;		
#ifdef	RTCLOCK
		preempt = QUANTUM;		
#endif

		ctxsw((int)&optr->pesp, (int)optr->pirmask, (int)&nptr->pesp, (int)nptr->pirmask);
		return OK;
	}
}




