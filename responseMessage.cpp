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


void ResponseMessage::PrepareGet(RequestMessage &requestMessage, server &servers)
{
    for (std::vector<location>::const_iterator it = servers.locations.begin(); 
    it != servers.locations.end(); ++it)
        if (requestMessage.target == it->locationURI)
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
                inf.open("/Users/kalexand/Desktop/webserver_copy/www/custom_404.html");
                this->answerNum = "404";
                getline (inf, buf, '\0');
                inf.close();
                return ;
            }
            // char *buf1 = strdup(requestMessage.target);
            char buf1[255];
            strcpy(buf1,requestMessage.target.c_str());
            if (strstr(buf1, "/autoindex") != nullptr){
                
                
                DIR *dir;
                struct dirent *current;

	            // close(client.read_fd);
	            // client.read_fd = -1;
	            dir = opendir(buf1);
	            this->bodyResponse = "<html>\n<body>\n";
	            this->bodyResponse += "<h1>Autoindex: on</h1>\n";
	            while ((current = readdir(dir)) != nullptr)
	            {
		            if (current->d_name[0] != '.')
		            {
			            this->bodyResponse += "<a href=\""+ it->locationURI;
			                if (it->locationURI != "/")
				                this->bodyResponse += "/";
			            this->bodyResponse += current->d_name;
			            this->bodyResponse += "\">";
			            this->bodyResponse += current->d_name;
			            this->bodyResponse += "</a><br>\n";
		            }
	            }   
	            closedir(dir);
	            this->bodyResponse += "</body>\n</html>\n";
                this->answerNum = "200";
                this->contentType = "text/html";
                this->headerResponse = "HTTP/1.1 " + this->answerNum + " OK\nContent-Length:" + to_string(this->bodyResponse.size()) + "\r\nContent-Type: " + this->contentType + "\r\n\r\n";
                this->fullResponse = this->headerResponse + this->bodyResponse;
                setResponse(this->fullResponse);
            }
            else{
            this->answerNum = "200";
            this->contentType = "text/html";
            this->headerResponse = "HTTP/1.1 " + this->answerNum + " OK\nContent-Length:" + to_string(buf.size()) + "\r\nContent-Type: " + this->contentType + "\r\n\r\n";
            this->bodyResponse = buf;
            this->fullResponse = this->headerResponse + this->bodyResponse;
            setResponse(this->fullResponse);
            }
        }
        if (requestMessage.target == "/stylesheet.css")
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