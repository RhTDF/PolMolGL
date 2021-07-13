#include "mathfuncs.h"

MathFuncs::MathFuncs()
{

}

double MathFuncs::normalize(double rmin, double rmax, double tmin, double tmax, double value)
{
    return tmin + (tmax-tmin)*(value-rmin)/(rmax-rmin);
}
