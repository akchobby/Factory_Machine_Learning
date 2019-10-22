
#include "factory.h"
using namespace std;




int main (void)
{
  int n = 4;
  double x[4] = { 1970, 1980, 1990, 2000 };
  double y[4] = {   12,   11,   14,   13 };
  double w[4] = {  0.1,  0.2,  0.3,  0.4 };

  Factory factory(1);
  factory.temperature.updateData(x,y,w,n);

  char temp_dummy[]="TEMPERATURE";
  factory.learn(temp_dummy);
  cout<<factory.temperature.c0<<endl;
  cout<<"For Temperature"<<endl;
  factory.estimate(1960,factory.temperature);
  cout<<"For Pressure"<<endl;
  factory.estimate(1960,factory.pressure);
  return 0;
}
