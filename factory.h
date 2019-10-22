#ifndef FACTORY_H
#define FACTORY_H

#include <stdio.h>
#include <iostream>
#include <string.h>
#include <gsl/gsl_fit.h>
#include <vector>
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

    void updateData(double temps[], double variable[], double prec[], double n){
      for (int i=0;i<n;i++){
        this->data.push_back(variable[i]);
        this->temps.push_back(temps[i]);
        this->precision.push_back(prec[i]);
        this->size ++;
        cout<<variable[i]<<"\t"<<temps[i]<<"\t"<<prec[i]<<"\t"<<size<<endl;
      }
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
  Parameter temperature , pressure, humidity;
  Factory(int number){
    this->id = number;
  }

  void estimate(double temp,Parameter target)
  {
    gsl_fit_linear_est(temp,
       (target.c0),(target.c1),
       (target.cov00),(target.cov01),(target.cov11),
       &(target.est),&(target.est_err));

     printf ("# Fit: %g %g\n", temp,(target.est) );
     printf ("# High : %g %g\n", temp, (target.est) + (target.est_err));
     printf ("# Low : %g %g\n", temp, (target.est) - (target.est_err));
  }

  void learn(char *target){
    char temperature_dummy [] = "TEMPERATURE";
    char pressure_dummy [] = "PRESSURE";
    char humidity_dummy [] = "HUMIDITY";

      if(!strcmp(target,temperature_dummy)){
        double size = this->temperature.giveSize();
        double* data = (double *)malloc(size*sizeof (double));
        double* temps = (double *)malloc(size*sizeof (double));
        double* precision = (double *)malloc(size*sizeof (double));

        data = this->temperature.giveData();
        temps = this->temperature.giveTemps();
        precision = this->temperature.givePrecision();

        gsl_fit_wlinear (temps, 1, precision, 1, data, 1, size,
                         &(this->temperature.c0), &(this->temperature.c1),
                         &(this->temperature.cov00), &(this->temperature.cov01), &(this->temperature.cov11),
                         &(this->temperature.chisq));

         printf ("# best fit: Y = %g + %g X\n", (this->temperature.c0), (this->temperature.c1));
         printf ("# covariance matrix:\n");
         printf ("# [ %g, %g\n#   %g, %g]\n",
                 (this->temperature.cov00), (this->temperature.cov01), (this->temperature.cov01), (this->temperature.cov11));
         printf ("# chisq = %g\n", (this->temperature.chisq));
       }
       if(!strcmp(target,pressure_dummy)){
         double size = this->pressure.giveSize();
         double* data = (double *)malloc(size*sizeof (double));
         double* temps = (double *)malloc(size*sizeof (double));
         double* precision = (double *)malloc(size*sizeof (double));

         data = this->pressure.giveData();
         temps = this->pressure.giveTemps();
         precision = this->pressure.givePrecision();

         gsl_fit_wlinear (temps, 1, precision, 1, data, 1, size,
                          &(this->pressure.c0), &(this->pressure.c1),
                          &(this->pressure.cov00), &(this->pressure.cov01), &(this->pressure.cov11),
                          &(this->pressure.chisq));

          printf ("# best fit: Y = %g + %g X\n", (this->pressure.c0), (this->pressure.c1));
          printf ("# covariance matrix:\n");
          printf ("# [ %g, %g\n#   %g, %g]\n",
                  (this->pressure.cov00), (this->pressure.cov01), (this->pressure.cov01), (this->pressure.cov11));
          printf ("# chisq = %g\n", (this->pressure.chisq));
        }
        if(!strcmp(target,humidity_dummy)){
          double size = this->pressure.giveSize();
          double* data = (double *)malloc(size*sizeof (double));
          double* temps = (double *)malloc(size*sizeof (double));
          double* precision = (double *)malloc(size*sizeof (double));

          data = this->humidity.giveData();
          temps = this->humidity.giveTemps();
          precision = this->humidity.givePrecision();

          gsl_fit_wlinear (temps, 1, precision, 1, data, 1, size,
                           &(this->humidity.c0), &(this->humidity.c1),
                           &(this->humidity.cov00), &(this->humidity.cov01), &(this->humidity.cov11),
                           &(this->humidity.chisq));

           printf ("# best fit: Y = %g + %g X\n", (this->humidity.c0), (this->humidity.c1));
           printf ("# covariance matrix:\n");
           printf ("# [ %g, %g\n#   %g, %g]\n",
                   (this->humidity.cov00), (this->humidity.cov01), (this->humidity.cov01), (this->humidity.cov11));
           printf ("# chisq = %g\n", (this->humidity.chisq));
         }

  }
};
#endif
