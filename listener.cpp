#include "listener.hpp"
#include "responseMessage.hpp"
#include <string>

int Listener::createSocket(server instance)
{
	int         socketfd;
    sockaddr_in serv_addr;

    std::memset(&serv_addr, '0', sizeof(serv_addr));
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_addr.s_addr = inet_addr(instance.host.c_str());
    serv_addr.sin_port = htons(instance.port);
    if (serv_addr.sin_addr.s_addr == -1)
        perror("inet_addr");

    socketfd = socket(PF_INET, SOCK_STREAM, 0);
    if (socketfd == -1)
    {
        std::cerr << strerror(errno) << std::endl;
        std::exit(1);
    }
 
    int yes = 1;
    int setsockoptResult = setsockopt(socketfd, SOL_SOCKET, SO_REUSEADDR, &yes, sizeof(int));
    if ( setsockoptResult == -1)
    {
        std::cerr << strerror(errno) << std::endl;
        std::exit(1);
    }

    int bindResult = bind(socketfd, (struct sockaddr*)&serv_addr, sizeof(serv_addr));
    if (bindResult == -1)
    {
        std::cerr << strerror(errno) << std::endl;
        std::exit(1);
    }

    int listenResult = listen(socketfd, 10);
    if (listenResult == -1)
    {
        std::cerr << strerror(errno) << std::endl;
        std::exit(1);
    }

    return (socketfd);
}

void	Listener::acceptConnection(int i)
{
    int     clientfd;
    int     n = serversNumb;

    clientfd = accept(fds[i].fd, (struct sockaddr*)NULL, NULL);
 //   fcntl(clientfd, F_SETFL, O_NONBLOCK);
    fcntl(clientfd, F_SETFL, fcntl(fds[i].fd, F_GETFL) | O_NONBLOCK);
    
    while (n < MAX_CLIENTS)
    {
        if (fds[n].fd == 0)
        {
            fds[n].fd = clientfd;
            fds[n].events = POLLIN;
            clientsNumb++;
            break;
        }
        n++;
    }
}

void Listener::receiveFromClient(int i)
{
    int         inputMsgSize = 1;
    char        sendBuff[BUF_SIZE];
    std::string incomeMessage;
    std::string fromBuffer;
    std::string fromBufferFull;
    std::size_t pos;
    ResponseMessage response; 

    std::memset(sendBuff, '0', sizeof(sendBuff));

    std::cout << "Start receiving ..." << std::endl;            
    while (inputMsgSize > 0)
    {
        inputMsgSize = recv(fds[i].fd, sendBuff , BUF_SIZE, 0);
        if (inputMsgSize <= 0)
            break ;
        std::cout << sendBuff;
        sendBuff[inputMsgSize] = 0;
        fromBuffer = sendBuff;
        fromBufferFull += fromBuffer;
        pos = fromBuffer.find("\r\n\r\n");
        if (pos != std::string::npos)
            break ;
    }
    
    if (inputMsgSize <= 0)
    {
        close(fds[i].fd); ///??? can we do it, close isnt in an allowed list
        fds[i].fd = 0;
        i++;
        return ;
    }
    pos = fromBufferFull.find("\r\n\r\n");
    incomeMessage = fromBufferFull.substr(0, pos);
    std::string rest = fromBufferFull.substr(pos);

    std::cout << std::endl << "requestMessage before parcing = " << std::endl 
                << incomeMessage << std::endl;   
    this->requestMessage.parceRequest(incomeMessage);

    std::cout << std::endl << "requestMessage after parcing = " << std::endl;
    for (std::map<std::string, std::string>::const_iterator it = requestMessage.headers.begin(); 
        it != requestMessage.headers.end(); ++it)		
        std::cout << "key = \"" << (*it).first << "\" value = \"" << (*it).second << "\""<< std::endl;

    std::cout << "requestMessage.method = |" << requestMessage.method
    << "| requestMessage.target = |" << requestMessage.target
    << "| requestMessage.protocolVersion = |" << requestMessage.protocolVersion << "|" << std::endl;

    
    responseMap[i] = response.PrepareResponse(requestMessage, servers);
    fds[i].events = POLLIN | POLLOUT;
}

int Listener::sendAllData(int i)
{
    int len = this->responseMap[i].size();
    char* message = (char*) this->responseMap[i].c_str();
    
    int bytesSend = 0;
    int bytesLeft = len;
    int n;
    std::cout << message << std::endl;
    while(bytesSend < len)
    {
        n = send(fds[i].fd, message + bytesSend, bytesLeft, 0);
        if (n == -1)
            break;
        bytesSend += n;
        bytesLeft -= n;
    }
    len = bytesSend; // здесь количество действительно посланных байт
    
    fds[i].events = POLLIN;

    if (n == -1)
        return (-1);
    return 0;
}

int	Listener::listenPoll(void)
{
    int socketfd;
    int i = 0;

    clientsNumb = 0;
    serversNumb = servers.size();
   
    std::memset(fds, 0, (MAX_CLIENTS + 1) * sizeof(struct pollfd));

    for (std::vector<server>::const_iterator it = this->servers.begin(); 
		it != this->servers.end(); ++it, ++i)		
	{
        socketfd = this->createSocket(servers[i]);
        fds[i].fd = socketfd;
        fds[i].events = POLLIN | POLLOUT;
    }

    while(1) 
	{
        i = 0;
        int pollResult = poll(fds, clientsNumb + serversNumb, -1);
        if (pollResult > 0)
        {
            std::cout << "start poll" << std::endl;
            while (i < MAX_CLIENTS)
            {
                if ((fds[i].revents & POLLIN) && (i < serversNumb))
                    acceptConnection(i);
                if ((fds[i].revents & POLLIN) && (i >= serversNumb))
                    receiveFromClient(i);      
                if (fds[i].revents & POLLOUT)
                    sendAllData(i);
                i++;             
            } 
        }
    }
	 return (0);
}


std::string Listener::to_string(int n)
{
    char buf[40];
    sprintf(buf,"%d",n);
    return buf;
}