#include <http.h>

Http::Http(int port, const char * public_dir)
:port_(port),
public_dir_(public_dir)
{
}

int Http::initialize()
{
	struct sockaddr_in addr;
	connection_sock_ = socket(AF_INET, SOCK_STREAM, 0);
	addr.sin_family = AF_INET;
	addr.sin_port = htons(port_);
	addr.sin_addr.s_addr = INADDR_ANY;
	bind(connection_sock_, (struct sockaddr *)&addr, sizeof(addr));
	return (0);
}

int Http::wait_request()
{
	int len;
	int sock;
	char buf[256];
	int n;
	struct sockaddr_in client;

	while (1)
	{
		std::string req;

		listen(connection_sock_, 5);
		len = sizeof(client);
		sock = accept(connection_sock_, (struct sockaddr *)&client, (socklen_t *)&len);
		if(sock < 0)
		{
			std::cerr << "Error: could not accept." << std::endl;
			return (0);
		}
		do
		{
			n = read(sock, buf, sizeof(buf));
			req += buf;
		} while (n == sizeof(buf));
		if (parse_request(req) != -1)
		{
			std::cout << "Method: " << header_.action << std::endl;
			std::cout << "PATH: " << header_.resource_path << std::endl;
			std::cout << "VERSION: " << header_.http_version << std::endl;
			if (send_response(sock) < 0)
			{
				//error
				std::cerr <<  "Error: sending response" << std::endl;
			}
		}
	}
	close(sock);
	close(connection_sock_);
}

std::vector<std::string> split_one_line(std::string & str, char del)
{
	std::vector<std::string> ret;
	int start(0);
	for (int current = 0; current < str.size(); ++current)
	{
		if (str[current] == '\n' || str[current] == del)
		{
			ret.push_back(str.substr(start, current - start));
			start = current + 1;
		}
		if (str[current] == '\n')
			break ;
	}
	return ret;
}

int Http::parse_request(std::string & req)
{
	auto ret = split_one_line(req, ' ');
	if (ret.size() != 3)
		return (-1);
	header_.action = ret[0];
	header_.resource_path = ret[1];
	header_.http_version = ret[2];
	if (!is_valid_header())
		return (-1);

	route();
	return (0);
}

void Http::route()
{
	if (header_.resource_path[header_.resource_path.size() - 1] == '/')
	{
		// Todo
		// find path
		// if found then status should be 200
		// else status should be 404
		// use filesystem to identify the path.
		header_.response_content_path = public_dir_ + header_.resource_path + "index.html";
		response_.status = 200;
	}
	else
	{
		response_.status = 404;
	}
}

ssize_t Http::send_response(int sock)
{
	std::string res;

	// Todo
	// if file exist
	// then set content and status as 200

	// Todo
	// apply dictionary for generating reponse header
	// then iterate all keys to generate response header
	if (response_.status == 200)
	{
		std::ifstream ifs (header_.response_content_path);
		std::cout << "file : " << header_.response_content_path << std::endl;
		std::string content;
		getline (ifs, content, (char) ifs.eof());

		res = "HTTP/1.1 200 OK\r\n";
		res += "Connection: Keep-Alive\r\n";
		res += "Content-Length: ";
		res += std::to_string(content.size());
		res += "\r\n";
		res += "Content-Type: text/html\r\n";
		res += "\r\n";
		res += content;
	}
	else if (response_.status == 404)
	{
		std::string content;
		content += "<!doctype html><html><head><meta charset=\"utf-8\"><meta name=\"viewport\" content=\"width=device-width, initial-scale=1\"><title>Not Found</title></head><body><h1>404 Not Found!</h1></body></html>\r\n";

		res = "HTTP/1.1 404 Not Found\r\n";
		res += "Connection: Keep-Alive\r\n";
		res += "Content-Length: ";
		res += std::to_string(content.size());
		res += "\r\n";
		res += "Content-Type: text/html\r\n";
		res += "\r\n";
		res += content;
	}
	ssize_t ret = write(sock, res.c_str(), res.size());
	return (0);
}

int Http::is_valid_header()
{
	// Todo
	// check action (GET or POST)
	// check path
	// check version
	return (1);
}
