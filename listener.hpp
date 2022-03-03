#pragma once
#ifndef __LISTENER_HPP
#define __LISTENER_HPP

#include <cstring>
#include <cstdio>
#include <cstdlib>
#include <iostream>
#include <cerrno>
#include <map>

#include <sys/socket.h>
#include <arpa/inet.h>
#include <poll.h>
#include <fcntl.h>
#include <unistd.h>

#include "server.hpp"
#include "requestMessage.hpp"
#include "responseMessage.hpp"

#define MAX_CLIENTS 10
#define BUF_SIZE 500

class Listener 
{
private:
	
public:
	std::vector<server> servers;
	struct pollfd 		fds[MAX_CLIENTS + 1];
	int 				clientsNumb;
	int					serversNumb;
	RequestMessage		requestMessage;
	ResponseMessage		responseMessage;
	std::map<int, std::string>	responseMap;
	std::map<int, std::string>	bodyMap;

	int 	createSocket(server instance);
	int		listenPoll(void);
	void	acceptConnection(int i);
	void	receiveFromClient(int i);
	int 	sendAllData(int i);
	std::string to_string(int n);
};

#endif
