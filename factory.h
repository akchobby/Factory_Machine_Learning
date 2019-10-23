#ifndef FACTORY_H
#define FACTORY_H

#include <stdio.h>
#include <iostream>
#include <fstream>
#include <string.h>
#include <gsl/gsl_fit.h>
#include <vector>
#include <boost/algorithm/string.hpp>
using namespace std;

class Parameter{
  private:
    vector<double> data;
    vector<double> temps;
    vector<double> precision;
    double size;
  public:
    double est;
    double est_err;
    double c0,c1, cov00,cov01, cov11, chisq;
    Parameter(){
      this->est = 0;
      this->est_err = 0;
      this->c0 = 0;
      this->c1 = 0;
      this->cov00 = 0;
      this->cov01 = 0;
      this->cov11 = 0;
      this->chisq = 0;
      this->data = {};
      this->temps = {};
      this->precision = {};
      this->size = 0;
    }
    void cleanVectors(){
      this->data = {};
      this->temps = {};
      this->precision = {};
      this->size = 0;
    }

    void updateData(double temps[], double variable[], double prec[], double n){
      for (int i=0;i<n;i++){
        this->data.push_back(variable[i]);
        this->temps.push_back(temps[i]);
        this->precision.push_back(prec[i]);
        this->size ++;
      }
    }
    void updateData(double temp,double variable,double prec){
      this->data.push_back(variable);
      this->temps.push_back(temp);
      this->precision.push_back(prec);
      this->size ++;
    }
    double* giveData(){
      double *result = (double *)malloc(this->size*sizeof(double));
      for (int i=0; i< this->size; i++){
        *(result+i)=this->data[i];
      }
      return result;
    }
    double* giveTemps(){
      double *result = (double *)malloc(this->size*sizeof(double));
      for (int i=0; i< this->size; i++){
        *(result+i)=this->temps[i];
      }
      return result;
    }
    double* givePrecision(){
      double *result = (double *)malloc(this->size*sizeof(double));
      for (int i=0; i< this->size; i++){
        *(result+i)=this->precision[i];
      }
      return result;
    }
    double giveSize(){
      return this->size;
    }
};

class Factory{
public:
  int id;
  string file_address;
  Parameter temperature , pressure, humidity;
  Factory(int number){
    this->id = number;
    string id_s = to_string(id);
    this->file_address = "Fact_"+id_s+".txt";
    ofstream outfile (file_address);
    outfile << "Logs for Factory: "+id_s << endl;
    outfile.close();
  }
  void storeData(double TIME, double TEMPERATURE,double PRESSURE,double HUMIDITY){
    ofstream outfile;
    outfile.open(this->file_address, ios_base::app);
    outfile << to_string(TIME)<<","<<to_string(TEMPERATURE)<<","<<to_string(PRESSURE)<<","<<to_string(HUMIDITY)<<endl;
    outfile.close();
  }
  void fetchData(){
    string line;
    ifstream myfile(this->file_address);
    vector <string> results;
    double time,t,p,h;
    string time_s,t_s,p_s,h_s;

    if(myfile.is_open()){
      getline(myfile,line);
      while( getline(myfile,line)){
        results = {};
        time_s = "";
        t_s = "";
        p_s = "";
        h_s = "";

        boost::split(results, line, [](char c){return c == ',';});

        time_s.append(results.at(0));
        t_s.append(results.at(1));
        p_s.append(results.at(2));
        h_s.append(results.at(3));
        cout<<time_s<<"\t"<<t_s<<"\t"<<p_s<<"\t"<<h_s<<endl;
        time = atof(time_s.c_str());
        t = atof(t_s.c_str());
        p = atof(p_s.c_str());
        h = atof(h_s.c_str());

        this->temperature.updateData(time,t,0.1);
        this->pressure.updateData(time,p,0.1);
        this->humidity.updateData(time,h,0.1);
      }
      myfile.close();
    }
  }
  void estimate(double temp,char* command){
    Parameter *target;
    char temperature_dummy [] = "TEMPERATURE";
    char pressure_dummy [] = "PRESSURE";
    char humidity_dummy [] = "HUMIDITY";
    if (!strcmp(command,temperature_dummy)) {
      target = &(this->temperature);
    }
    if (!strcmp(command,pressure_dummy)) {
      target = &(this->pressure);
    }
    if (!strcmp(command,humidity_dummy)) {
      target = &(this->humidity);
    }

      gsl_fit_linear_est(temp,
         (target->c0),(target->c1),
         (target->cov00),(target->cov01),(target->cov11),
         &(target->est),&(target->est_err));

       printf ("# Fit: %g %g\n", temp,(target->est) );
       printf ("# High : %g %g\n", temp, (target->est) + (target->est_err));
       printf ("# Low : %g %g\n", temp, (target->est) - (target->est_err));
  }
  void learn(char *command){
    Parameter *target;
    char temperature_dummy [] = "TEMPERATURE";
    char pressure_dummy [] = "PRESSURE";
    char humidity_dummy [] = "HUMIDITY";
    if (!strcmp(command,temperature_dummy)) {
      target = &(this->temperature);
    }
    if (!strcmp(command,pressure_dummy)) {
      target = &(this->pressure);
    }
    if (!strcmp(command,humidity_dummy)) {
      target = &(this->humidity);
    }
     double size = target->giveSize();
     double* data = (double *)malloc(size*sizeof (double));
     double* temps = (double *)malloc(size*sizeof (double));
     double* precision = (double *)malloc(size*sizeof (double));

     data = target->giveData();
     temps = target->giveTemps();
     precision = target->givePrecision();

     gsl_fit_wlinear (temps, 1, precision, 1, data, 1, size,
                      &(target->c0), &(target->c1),
                      &(target->cov00), &(target->cov01), &(target->cov11),
                      &(target->chisq));

      printf ("# best fit: Y = %g + %g X\n", (target->c0), (target->c1));
      printf ("# covariance matrix:\n");
      printf ("# [ %g, %g\n#   %g, %g]\n",
              (target->cov00), (target->cov01), (target->cov01), (target->cov11));
      printf ("# chisq = %g\n", (target->chisq));

  }
};
#endif
