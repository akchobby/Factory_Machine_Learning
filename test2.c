#include <stdio.h>
#include <gsl/gsl_fit.h>

struct parameter{
  double est;
  double c0,c1, cov00,cov01, cov11, chisq;
};

struct factory{
  int No;
  struct parameter *temperature , *pressure, *humidity;
  void (*learn) (struct parameter* temperature,struct parameter* pressure,struct parameter* humidity);
  void (*estimate) (int time, struct parameter *temperature,struct parameter *pressure,struct parameter *humidity);
};

void learn(double variable[],double temps[],double precision[], int size){
  double c0, c1, cov00, cov01, cov11, chisq;

  gsl_fit_wlinear (temps, 1, precision, 1, variable, 1, size,
                   &c0, &c1, &cov00, &cov01, &cov11,
                   &chisq);

   printf ("# best fit: Y = %g + %g X\n", c0, c1);
   printf ("# covariance matrix:\n");
   printf ("# [ %g, %g\n#   %g, %g]\n",
           cov00, cov01, cov01, cov11);
   printf ("# chisq = %g\n", chisq);

   for (int i = 0; i < size; i++)
     printf ("data: %g %g %g\n",
                    temps[i], variable[i], 1/sqrt(precision[i]));

   printf ("\n");

}

void estimate(double temp, struct parameter *temperature,struct parameter *pressure,struct parameter *humidity)
{

}
int main (void)
{
  int n = 4;
  double x[4] = { 1970, 1980, 1990, 2000 };
  double y[4] = {   12,   11,   14,   13 };
  double w[4] = {  0.1,  0.2,  0.3,  0.4 };


  learn(y,x,w,n);

  // for (i = -30; i < 130; i++)
  //   {
  //     double xf = x[0] + (i/100.0) * (x[n-1] - x[0]);
  //     double yf, yf_err;
  //
  //     gsl_fit_linear_est (xf,
  //                         c0, c1,
  //                         cov00, cov01, cov11,
  //                         &yf, &yf_err);
  //
  //     printf ("fit: %g %g\n", xf, yf);
  //     printf ("hi : %g %g\n", xf, yf + yf_err);
  //     printf ("lo : %g %g\n", xf, yf - yf_err);
  //   }
  return 0;
}
