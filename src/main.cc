#include <http.h>

int main(int argc, char const* argv[])
{
	Http server(12345, "../public/");
	server.initialize();
	server.wait_request();
	return 0;
}
