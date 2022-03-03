
#ifndef REQUESTMESSAGE_HPP
#define REQUESTMESSAGE_HPP
#include <map>
#include <string>
#include <istream>
#include <sstream>


class RequestMessage
{
private:
	void parceHeaders(std::string headersToParce);

public:
	std::string method;
	std::string target;
	std::string	protocolVersion;
	std::map<std::string, std::string>	headers;


	void parceRequest(std::string requestToParce);
};

#endif
