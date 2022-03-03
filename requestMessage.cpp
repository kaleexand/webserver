#include "requestMessage.hpp"


void RequestMessage::parceRequest(std::string requestToParce)
{
	std::size_t pos1, pos2;
	std::string	headersToParce;

	pos1 = 0;
	pos2 = requestToParce.find(" ");
	this->method = requestToParce.substr(pos1, pos2 - pos1);

	pos1 = pos2 + 1;
	pos2 = requestToParce.find(" ", pos1);
	this->target = requestToParce.substr(pos1, pos2 - pos1);

	pos1 = pos2 + 1;
	pos2 = requestToParce.find("\r\n", pos1 + 1);
	this->protocolVersion = requestToParce.substr(pos1, pos2 - pos1);
	
	headersToParce = requestToParce.substr(pos2 + 2);
	this->parceHeaders(headersToParce);
}

void RequestMessage::parceHeaders(std::string headersToParce)
{
    std::string key, val;
    std::istringstream iss(headersToParce);

    while(std::getline(std::getline(iss, key, ':') >> std::ws, val))
        headers[key] = val.substr(0, val.find_first_of("\r\n"));
}
