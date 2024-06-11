#include "hinhchunhat.h"
#include <math.h>

#define ALPHA 0.05
#define NMAX 100
#define EPSILON 1e-6
const double deltat = 0.0001;

double findf(double x, double width, double length)
{
	return x * (width - 2 * x) * (length - 2 * x);
}

double finddf(double x, double width, double length)
{
	return (findf(x + deltat, width, length) - findf(x - deltat, width, length)) / (2 * deltat);
}

double findx(double width, double length, double x, double Vmax)
{
	x = 0;
	double y = findf(x, width, length), dy, y0;
	int n = 0;
	while (n < NMAX)
	{
		y0 = y;
		dy = finddf(x, width, length);
		if (dy >= 0)
			x = x + ALPHA * dy;
		else
			x = x - ALPHA * dy;
		y = findf(x, width, length);
		n++;
		if (fabs(y - y0) < EPSILON)
			break;
	}
	return x;
}

void Ketqua(double width, double length, double &x, double &Vmax)
{
	x = findx(width, length, x, Vmax);
	Vmax = findf(x, width, length);
}
