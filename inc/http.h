#ifndef HTTP_H
#define HTTP_H
#include <iostream>

struct HttpHeader
{
	int action;
};

class Http
{
  public:
	Http();
	ssize_t listen();
	ssize_t close();
	ssize_t connect();
	ssize_t parse_request();
	ssize_t send_response();
  private:
	int port_;
	int address_;
	HttpHeader header_;
};
#endif
