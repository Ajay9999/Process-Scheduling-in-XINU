//#include<conf.h>
//#include<kernel.h>
//#include<proc.h>
#include<stdio.h>
#include<math.h>
#define RAND_MAX 077777

double pow(double base, double exponent)
{
	double value = 1;
	while(exponent>0)
	{
		value = value*base;
		exponent--;

	}
	return value;
}

//int main()
//{
//kprintf("%f",pow(2,3));
//return 0;
//}

double log(double x)
{
	int limit = 20;
	double result = 0;
	while(limit!=0)
	{
		result = result + (-1 * (pow(x,limit)/limit));
		limit--;
	}
	return result;

}

double expdev(double lambda)
{
	double dummy;
	do
		dummy= (double) rand() / RAND_MAX;
	while (dummy == 0.0);
	return -log(dummy) / lambda;
}






