#ifndef LOGGING_H
#define LOGGING_H

#define BOOST_LOG_DYN_LINK 1
#include <iostream>


//#define TRACE(msg)            BOOST_LOG_TRIVIAL(trace)<< msg
//#define INFO(msg)             BOOST_LOG_TRIVIAL(info) << msg
//#define ERR(msg)              BOOST_LOG_TRIVIAL(error)<< msg
#define TRACE(msg)            std::cout << msg <<std::endl
#define INFO(msg)             std::cout << msg <<std::endl
#define ERR(msg)              std::cout << msg <<std::endl

void setLoggingForDaemon();

#endif // LOGGING_H

