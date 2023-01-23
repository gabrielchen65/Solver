#include <exception>
#include <iostream>
#include <string>
using namespace std;

class parserException : public exception{
    const char* msg;
    public:
    parserException(const char* msg):msg(msg){};
    const char* what(){
        return msg;
    }
};