//C++ file
//***********************************************
//      Filename: boost_log.cc
//        Author: lgy - liguiyuan91@163.com
//   Description: 该日志可以支持多个文件，以及异步写，并且可以分等级写多个日志
//        Create: 2018-04-11 09:09:43
// Last Modified: 2018-04-11 09:09:43
//***********************************************
#include <string>
#include <iostream>
#include <boost/date_time/posix_time/posix_time.hpp>
#include <boost/log/support/date_time.hpp>
//#include <boost/log/common.hpp>
#include <boost/log/expressions.hpp>
#include <boost/log/expressions/keyword.hpp>

#include <boost/log/attributes.hpp>
#include <boost/log/attributes/timer.hpp>
#include <boost/log/sources/logger.hpp>
#include <boost/log/sources/severity_channel_logger.hpp>
#include <boost/log/sinks/async_frontend.hpp>
//#include <boost/log/sinks/text_file_backend.hpp>
#include <boost/log/sinks/text_multifile_backend.hpp>

#include <boost/log/utility/setup/file.hpp>
#include <boost/log/utility/setup/console.hpp>
#include <boost/log/utility/setup/common_attributes.hpp>
#include <boost/log/trivial.hpp>
#include <boost/log/attributes.hpp>

namespace logging = boost::log;
namespace attrs = boost::log::attributes;
namespace src = boost::log::sources;
namespace sinks = boost::log::sinks;
namespace expr = boost::log::expressions;
namespace keywords = boost::log::keywords;

enum SeverityLevel { Log_Info, Log_Notice, Log_Debug, Log_Warning, Log_Error, Log_Fatal };

BOOST_LOG_ATTRIBUTE_KEYWORD(log_severity, "Severity", SeverityLevel)
BOOST_LOG_ATTRIBUTE_KEYWORD(log_timestamp, "TimeStamp", boost::posix_time::ptime)
BOOST_LOG_ATTRIBUTE_KEYWORD(log_uptime, "Uptime", attrs::timer::value_type)
BOOST_LOG_ATTRIBUTE_KEYWORD(log_scope, "Scope", attrs::named_scope::value_type)
BOOST_LOG_ATTRIBUTE_KEYWORD(log_process, "ProcessId", attrs::current_process_id)
BOOST_LOG_ATTRIBUTE_KEYWORD(line_id, "LineID", unsigned int)

static boost::log::sources::severity_logger<SeverityLevel> b_log; 

#define LOG_DEBUG\
    BOOST_LOG_SEV(b_log, Log_Debug) << "[" <<__FILE__  << ":" << __LINE__ << "] " 
#define LOG_INFO\
    BOOST_LOG_SEV(b_log, Log_Info << "[" <<__FILE__  << ":" << __LINE__ << "] " 
#define LOG_ERROR\
    BOOST_LOG_SEV(b_log, Log_ERROR) << "[" <<__FILE__  << ":" << __LINE__ << "] " 
#define LOG_WARNING\
    BOOST_LOG_SEV(b_log, Log_Warning) << "[" <<__FILE__  << ":" << __LINE__ << "] " 
#define LOG_FATAL\
    BOOST_LOG_SEV(b_log, Log_Fatal) << "[" <<__FILE__  << ":" << __LINE__ << "] " 

void init(bool toconsole)
{
    logging::formatter formatter=
        expr::stream
        <<"<"<<log_severity<<">"
        << "[" << expr::format_date_time<boost::posix_time::ptime>("TimeStamp", "%Y-%m-%d,%H:%M:%S")<< "]" 
        //<<expr::if_(expr::has_attr(log_scope))
        //[
            //expr::stream<<"["<<expr::format_named_scope("Scope",keywords::format ="%f:%l", keywords::iteration = expr::reverse, keywords::depth = 2)<<"]"
        //]
         ////<< "[" << expr::format_named_scope(log_scope,keywords::format ="%f:%l")<<"]"
        //dayinxianchenghao
         //<< "[" << expr::attr<boost::log::attributes::current_thread_id::value_type>("ThreadID") << "]"
        <<expr::message; 

    logging::add_common_attributes();
    //BOOST_LOG_SCOPED_LOGGER_TAG(LogConfig::b_log, )
    //init sink
    typedef sinks::asynchronous_sink<sinks::text_multifile_backend> TextSinks; 
    boost::shared_ptr<sinks::text_multifile_backend> backend = boost::make_shared<sinks::text_multifile_backend>(); 
    backend->set_file_name_composer ( sinks::file::as_file_name_composer(expr::stream 
         <<"./logs" << "/" 
         << boost::log::aux::get_process_name() << "."
         << expr::attr<SeverityLevel>("Severity") << "." 
         << expr::format_date_time<boost::posix_time::ptime>("TimeStamp", "%Y-%m-%d.")
         << boost::log::aux::this_process::get_id().native_id()
         << ".log")); 
    boost::shared_ptr<TextSinks> file_sink(new TextSinks(backend)); 
    file_sink->set_filter(log_severity>=Log_Info);   //日志级别过滤

    file_sink->set_formatter(formatter);

    //logging::core::get()->add_global_attribute("Scope",attrs::named_scope());
    logging::core::get()->add_thread_attribute("Scope", attrs::named_scope()); 
    logging::core::get()->add_sink(file_sink);
    if(toconsole == true)
    {
        boost::shared_ptr< sinks::synchronous_sink<sinks::text_ostream_backend> > 
        console_sink=logging::add_console_log();
        console_sink->set_formatter(formatter);
        console_sink->set_filter(log_severity>=Log_Info);   //日志级别过滤
        logging::core::get()->add_sink(console_sink);
    }
    
}


int main()
{
    boost::log::sources::severity_logger<SeverityLevel> b_log; 
    init(true); 
    BOOST_LOG_SEV(b_log, Log_Info) << "[" <<__FILE__  << ":" << __LINE__ << "] "  << "hello!"; 
    return 0; 
}
