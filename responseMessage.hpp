#ifndef RESPONSEMESSAGE_HPP
#define RESPONSEMESSAGE_HPP 
#include "server.hpp"
#include "parser.hpp"
#include "requestMessage.hpp"
#include <iostream>


enum 
{
    GET,
    POST,
    DELETE
};



class ResponseMessage
{
    private:
        std::string bodyResponse;
        std::string headerResponse;
        std::string fullResponse;
        std::string answerNum;
        std::string contentType;
        parser parser;
    public:
        std::string PrepareResponse(RequestMessage &requestMessage,std::vector<server> &servers);
        void        setResponse(std::string &resp);
        std::string getResponse();
        void CheckLocation();
        void PrepareGet(RequestMessage &requestMessage, server &servers);
        std::string getAnswerNum(void);
        std::string to_string(int n);
        ResponseMessage();
        ~ResponseMessage();
        

};

#endif
