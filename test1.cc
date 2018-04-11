//C++ file
//***********************************************
//      Filename: test1.cc
//        Author: lgy - liguiyuan91@163.com
//   Description: ---
//        Create: 2018-04-04 09:30:19
// Last Modified: 2018-04-04 09:30:19
//***********************************************

#include <boost/log/core.hpp>
#include <boost/log/trivial.hpp>
#include <boost/log/expressions.hpp>
#include <boost/log/sinks/text_file_backend.hpp>
#include <boost/log/utility/setup/file.hpp>
#include <boost/log/utility/setup/common_attributes.hpp>
#include <boost/log/utility/setup/console.hpp>
#include <boost/log/sources/severity_logger.hpp>
#include <boost/log/sources/record_ostream.hpp>
#include <boost/log/sources/logger.hpp>

namespace logging = boost::log;
namespace src = boost::log::sources;
namespace sinks = boost::log::sinks;
namespace keywords = boost::log::keywords;

namespace logging=boost::log;
namespace src = boost::log::sources;
namespace sinks = boost::log::sinks;
namespace keywords = boost::log::keywords;

void init1()
{
    logging::add_file_log("sample.log");
    logging::core::get()->set_filter(logging::trivial::severity>=logging::trivial::info);
    logging::add_console_log(std::clog, keywords::format = "%TimeStamp%: %Message%");
}

int main(int argc, char* argv[])
{
    init1();
    src::logger lg; 
    BOOST_LOG_TRIVIAL(trace)<<"A trace severity message";
    BOOST_LOG_TRIVIAL(debug) << "A debug severity message";
    BOOST_LOG_TRIVIAL(info) << "An informational severity message";
    BOOST_LOG_TRIVIAL(warning) << "A warning severity message";
    BOOST_LOG_TRIVIAL(error) << "An error severity message";
    BOOST_LOG_TRIVIAL(fatal) << "A fatal severity message";
    BOOST_LOG(lg) << "hello world!"; 
    return 0;
}
