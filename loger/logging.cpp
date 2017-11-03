#include "logging.h"

void setLoggingForDaemon(){
    boost::log::add_file_log("./checker.log");
    boost::log::core::get()->set_filter(
                boost::log::trivial::severity >=boost::log::trivial::info
            );
}
