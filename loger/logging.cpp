#include "logging.h"
#include <boost/log/core.hpp>
#include <boost/log/trivial.hpp>
#include <boost/log/expressions.hpp>
#include <boost/log/utility/setup/file.hpp>

void setLoggingForDaemon(){
    boost::log::add_file_log("./checker.log");
//    boost::log::core::get()->set_filter(
//                boost::log::trivial::severity >=boost::log::trivial::info
//            );
}
