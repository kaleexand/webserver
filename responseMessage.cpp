#include "responseMessage.hpp"
#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <string>
#include <fcntl.h>
#include <unistd.h>

#include <string>
#include <iostream>
#include <iomanip>
#include <cstdlib>

#include <map>
#include <queue>
#include <vector>

#include <sys/time.h>
#include <sys/stat.h>
#include <sys/wait.h>
#include <sys/select.h>
#include <arpa/inet.h>
#include "fstream"
#include <dirent.h>

#define RED "\33[1;31m"
#define GREEN "\33[1;32m"
#define YELLOW "\33[1;33m"
#define BLUE "\33[1;34m"
#define CIAN "\33[1;36m"
#define RESET "\33[0m"

ResponseMessage::ResponseMessage(){}

ResponseMessage::~ResponseMessage(){}

std::string ResponseMessage::PrepareResponse(RequestMessage &requestMessage,std::vector<server> &servers)
{
    std::string response;
    std::ifstream inf;
    std::string serverHost;
    int i = 0;
    for (std::vector<server>::const_iterator it = servers.begin(); it != servers.end(), i <= servers.size(); ++it, ++i)
    {
        serverHost = servers[i].host + ":" + std::to_string(servers[i].port);
        if (requestMessage.headers.find("Host")->second == serverHost)
        {
             if (requestMessage.method == "GET")
            {
                // if (requestMessage.target == "/autoindex"){
                //     autoindexGet(requestMessage, servers[i]);
                //     return (getResponse());
                // }

                PrepareGet(requestMessage, servers[i]);
                return (getResponse());
            }
        }
            


    }
}

void ResponseMessage::generateAutoindex(std::string itl, char *buffer){
    DIR *dir;
    struct dirent *current;
    dir = opendir(buffer);
    if (dir == NULL) {
        std::cerr << RED  << RED << "Error: could not open [" << buffer << "]" << RESET <<RESET << std::endl;
        exit(-1);
    }
	this->bodyResponse = "<html>\n<body>\n";
	this->bodyResponse += "<h1>Autoindex: on</h1>\n";
	while ((current = readdir(dir)) != nullptr)
    {
        if (current->d_name[0] != '.')
		{
			this->bodyResponse += "<a href=\""+ itl;
			    if (itl != "/")
                    this->bodyResponse += "/";
			this->bodyResponse += current->d_name;
			this->bodyResponse += "\">";
			this->bodyResponse += current->d_name;
			this->bodyResponse += "</a><br>\n";
		}
	}
    closedir(dir);
    this->answerNum = "200";
    this->contentType = "text/html";
	this->bodyResponse += "</body>\n</html>\n";
    this->headerResponse = "HTTP/1.1 " + this->answerNum + " OK\nContent-Length:" + to_string(bodyResponse.size()) +"\r\nContent-Type: " + this->contentType + "\r\n\r\n";
    this->fullResponse = this->headerResponse + this->bodyResponse;
    setResponse(this->fullResponse);
}

void ResponseMessage::PrepareGet(RequestMessage &requestMessage, server &servers)
{
    setTypes();
    for (std::vector<location>::const_iterator it = servers.locations.begin(); 
    it != servers.locations.end(); ++it){
        std::cout << "KALEXAND: "<< it->locationURI<<std::endl;
        char buf1[255];
        strcpy(buf1,requestMessage.target.c_str());
        if (strstr(buf1, "/autoindex") != nullptr){
            char root_now[53];
            strcpy(root_now,servers.locations[0].root.c_str());
            char *path = root_now;
            generateAutoindex(it->locationURI, path);
        }
        else if (strstr(buf1, "/auth/") != nullptr){
            std::string buf2 = servers.locations[0].root;
            buf2 += requestMessage.target.erase(0,5);
            int buffer_size = buf2.size();
            char buf3[buffer_size];
            strcpy(buf3,buf2.c_str());
            generateAutoindex(it->locationURI, buf3);
        }
        else if (requestMessage.target == it->locationURI)
        {   
            std::string buf;
            std::ifstream inf;
            std::string path;
            inf.open(it->root);
            if (inf)
            {
                getline (inf, buf, '\0');
                inf.close();
            }
            else
            {
                inf.open("/Users/kalexand/Desktop/webserver-master/my_webserver/www/custom_404.html");
                this->answerNum = "404";
                getline (inf, buf, '\0');
                inf.close();
                return ;
            }
            this->answerNum = "200";
            this->contentType = "text/html";
            this->headerResponse = "HTTP/1.1 " + this->answerNum + " OK\nContent-Length:" + to_string(buf.size()) + "\r\nContent-Type: " + this->contentType + "\r\n\r\n";
            this->bodyResponse = buf;
            this->fullResponse = this->headerResponse + this->bodyResponse;
            setResponse(this->fullResponse);
        }
        else if (requestMessage.target == "/stylesheet.css")
        {
            
            std::string buf;
            std::ifstream inf;
            inf.open("/Users/kalexand/Desktop/webserver_copy/www/stylesheet.css");
            getline (inf, buf, '\0');
            inf.close();
            this->answerNum = "200";
            this->contentType = "text/css";
            this->headerResponse = "HTTP/1.1 " + this->answerNum + " OK\nContent-Length:" + to_string(buf.size()) + "\r\nContent-Type: " + this->contentType + "\r\n\r\n";
            buf = this->headerResponse + buf;
            setResponse(buf);
        }
         else if (strstr(buf1, "/errors/") != nullptr){
            std::string buf;
            // std::ifstream inf;
            // std::string path;
            // std::ios_base::openmode mode = std::fstream::in;
            std::fstream						file;
            file.open("YoupiBanane/errors/404.png", std::fstream::in);
            while (!file.eof()) {
		        std::string	buf;
		        getline(file, buf);
		        // this->ask += buf + "\n";
	        }
            this->contentType = "image/png";
            // this->headerResponse = "HTTP/1.1 " + this->answerNum + " OK\nContent-Length:" + to_string(49975) + + "\r\nContent-Location: /errors/404.png"+ "\r\nLocation: /errors/404.png"+"\r\nContent-Type: " + this->contentType + "\r\n\r\n";
                        this->headerResponse = "HTTP/1.1 " + this->answerNum + " OK\nContent-Length:" + to_string(49975) + + "\r\nContent-Location: /errors/404.png"+ "\r\nLocation: /errors/404.png"+"\r\nContent-Type: " + this->contentType + "\r\n\r\n";
            this->bodyResponse = buf;
            this->fullResponse = this->headerResponse + this->bodyResponse;
            setResponse(this->fullResponse);
            // this->answerNum = "200";
            // this->headerResponse = "HTTP/1.1 " + this->answerNum + " OK\nContent-Length:" + to_string(1041006) + "\r\nContent-Type: " + this->contentType + "\r\n\r\n";
            // this->bodyResponse = buf;
            // this->fullResponse = this->headerResponse + this->bodyResponse;
            // setResponse(buf);

            std::cout << "KALEXAND tsrget: "<< requestMessage.target<<std::endl;

        }
    }
}

void ResponseMessage::setTypes(){
    this->setType("html", "text/html");
	this->setType("htm", "text/html");
	this->setType("css", "text/css");
	this->setType("js", "text/javascript");
	this->setType("xml", "text/xml");
	this->setType("gif", "image/gif");
	this->setType("jpg", "image/jpeg");
	this->setType("jpeg", "image/jpeg");
	this->setType("png", "image/png");
	this->setType("bmp", "image/bmp");
	this->setType("ico", "image/x-icon");
}

std::string ResponseMessage::getAnswerNum(void)
{
    return (this->answerNum);
}


void        ResponseMessage::setResponse(std::string &resp)
{
    this->fullResponse = resp;
}

 std::string ResponseMessage::getResponse()
 {
     return (this->fullResponse);
 }

 std::string ResponseMessage::to_string(int n)
{
    char buf[40];
    sprintf(buf,"%d",n);
    return buf;
}

void    ResponseMessage::setType(std::string type, std::string standart) {
	this->types[type] = standart;
}