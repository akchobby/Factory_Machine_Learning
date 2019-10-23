#include <netdb.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include "factory.h"

#define MAX 80
#define PORT 8081
#define SA struct sockaddr


void func(int sockfd)
{
	char buff[MAX];
  char request_c[20];
	int id;
  string command_s,id_s,time_s,request_s,temperature_s,pressure_s,humidity_s,message;
  double est,time,temperature,pressure,humidity;
  vector <string> results;
  vector <Factory> factories;
  factories.reserve(6);

  for (;;) {
		bzero(buff, sizeof(buff));
    results = {};
    read(sockfd, buff, sizeof(buff));

    boost::split(results, buff, [](char c){return c == ',';});

    command_s = "";
    id_s = "";

    command_s.append(results.at(0));
    id_s.append(results.at(1));

    id = stoi(id_s);

    switch (results.size()) {
      case 2: //create a new instance of Factory
        factories.push_back(Factory(id));
      case 4: //send estimation
          time_s = "";
          time_s.append(results.at(2));
          time = atof(time_s.c_str());

          request_s = "";
          request_s.append(results.at(3));
          bzero(request_c, sizeof(buff));
          strcpy(request_c,request_s.c_str());

          factories[id-1].fetchData();
          factories[id-1].learn(request_c);
          est = factories[id-1].estimate(time,request_c);

          message = "";
          message.append("est,");
          message.append(id_s+",");
          message.append(time_s+",");
          message.append(to_string(est));
          bzero(buff, sizeof(buff));
          strcpy(buff,message.c_str());

          write(sockfd, buff, sizeof(buff));
          factories[id-1].temperature.cleanVectors();
          factories[id-1].pressure.cleanVectors();
          factories[id-1].humidity.cleanVectors();
      case 6: //receive data
        time_s = "";
        temperature_s = "";
        pressure_s = "";
        humidity_s = "";

        time_s.append(results.at(2));
        temperature_s.append(results.at(3));
        pressure_s.append(results.at(4));
        humidity_s.append(results.at(5));

        time = atof(time_s.c_str());
        temperature = atof(temperature_s.c_str());
        pressure = atof(pressure_s.c_str());
        humidity = atof(humidity_s.c_str());

        factories[id-1].storeData(time,temperature,pressure,humidity);

    }
	}
}




int main (void)
{
  int sockfd;
	struct sockaddr_in servaddr;

	// socket create and varification
	sockfd = socket(AF_INET, SOCK_STREAM, 0);
	if (sockfd == -1) {
		printf("socket creation failed...\n");
		exit(0);
	}
	else
		printf("Socket successfully created..\n");
	bzero(&servaddr, sizeof(servaddr));

	// assign IP, PORT
	servaddr.sin_family = AF_INET;
	servaddr.sin_addr.s_addr = inet_addr("127.0.0.1");
	servaddr.sin_port = htons(PORT);

	// connect the client socket to server socket
	if (connect(sockfd, (SA*)&servaddr, sizeof(servaddr)) != 0) {
		printf("connection with local server failed...\n");
		exit(0);
	}
	else
		printf("connected to local server..\n");

  // function for chat
  func(sockfd);

  // close the socket
  close(sockfd);


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


  // vector<Factory> factories;
  // factories.reserve(6);
  // for (int i=0; i<6;i++){
  //   factories.push_back(Factory(i+1));
  // }
  // factories[0].storeData(1970,12,200,0.1);
  // factories[0].storeData(1980,11,300,0.2);
  // factories[0].storeData(1990,14,400,0.3);
  // factories[0].storeData(2000,13,500,0.5);
  // factories[0].fetchData();




  // char temp_dummy[]="TEMPERATURE";
  // char pressure_dummy[]="PRESSURE";
  // factory.learn(temp_dummy);
  // cout<<"For Temperature"<<endl;
  // factory.estimate(1960,temp_dummy);
  // cout<<"For Pressure"<<endl;
  // factory.estimate(1960,pressure_dummy);

  // char temp_dummy[]="TEMPERATURE";
  // char pressure_dummy[]="PRESSURE";
  // factories[0].learn(temp_dummy);
  // cout<<"For Temperature"<<endl;
  // factories[0].estimate(1960,temp_dummy);
  // cout<<"For Pressure"<<endl;
  // factories[0].estimate(1960,pressure_dummy);
  // return 0;
}
