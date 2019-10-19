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
