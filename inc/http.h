#ifndef HTTP_H
#define HTTP_H
#include <iostream>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
#include <vector>
#include <fstream>
#include <iostream>
#include <sstream> //std::stringstream

struct HttpHeader
{
	std::string action;
	std::string resource_path;
	std::string http_version;
	std::string response_content_path;
};

struct HttpResponse
{
	int status;
};

class Http
{
  public:
	Http(int port, const char * public_dir="./");
	int initialize();
	int wait_request();
	int parse_request(std::string & req);
	ssize_t send_response(int sock);
  private:
	int port_;
	int address_;
	std::string public_dir_;
	int connection_sock_;
	HttpHeader header_;
	HttpResponse response_;
	void route();

	int is_valid_header();
};
#endif
