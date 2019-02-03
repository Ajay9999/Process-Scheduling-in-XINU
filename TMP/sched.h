#define EXPDISTSCHED 1
#define LINUXSCHED 2
#define DEFAULTSCHED 3

extern int schedtype;
extern void setschedclass(int sched_class);
extern int getschedclass();
extern int expschednxproc();
extern int linuxschednxproc();
extern void newepoch();
