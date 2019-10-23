
#include "factory.h"

void* readFromQueue(){}
void* calculating(){}
void* learning(){}
void* writeToQueue(){}




int main (void)
{
  // int n = 4;
  // double x[4] = { 1970, 1980, 1990, 2000 };
  // double y[4] = {   12,   11,   14,   13 };
  // double w[4] = {  0.1,  0.2,  0.3,  0.4 };

  // Factory factory(1);

  // factory.temperature.updateData(x,y,w,n);

  // factory.storeData(1970,12,200,0.1);
  // factory.storeData(1980,11,300,0.2);
  // factory.storeData(1990,14,400,0.3);
  // factory.storeData(2000,13,500,0.5);
  // factory.fetchData();


  vector<Factory> factories;
  factories.reserve(6);
  for (int i=0; i<6;i++){
    factories.push_back(Factory(i+1));
  }
  factories[0].storeData(1970,12,200,0.1);
  factories[0].storeData(1980,11,300,0.2);
  factories[0].storeData(1990,14,400,0.3);
  factories[0].storeData(2000,13,500,0.5);
  factories[0].fetchData();




  // char temp_dummy[]="TEMPERATURE";
  // char pressure_dummy[]="PRESSURE";
  // factory.learn(temp_dummy);
  // cout<<"For Temperature"<<endl;
  // factory.estimate(1960,temp_dummy);
  // cout<<"For Pressure"<<endl;
  // factory.estimate(1960,pressure_dummy);

  char temp_dummy[]="TEMPERATURE";
  char pressure_dummy[]="PRESSURE";
  factories[0].learn(temp_dummy);
  cout<<"For Temperature"<<endl;
  factories[0].estimate(1960,temp_dummy);
  cout<<"For Pressure"<<endl;
  factories[0].estimate(1960,pressure_dummy);
  return 0;
}
