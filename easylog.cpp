

#include "easylog.h"

/*****************************************************************
* Name: getEasylog
* Description: get Easylog
* param: -
* return: -
* author: 
* update: 
******************************************************************/
shared_ptr<EasyLog> LogFactory::getEasylog(string logID, string logpath)
{

	map<string, shared_ptr<EasyLog> >::iterator it;

	it = m_easylog_map.find(logID);

   //存在要设定的消息
    if(it != m_easylog_map.end())
    {
    	return it->second;
    }
    else
    {
    	string path;
    	if(!logpath.empty())
    	{
    		path = logpath + "/" + logID;
    	}

    	shared_ptr<EasyLog> loger(new EasyLog(path));

    	m_easylog_map.insert(std::pair<string, shared_ptr<EasyLog>>(logID, loger));

    	return loger;
    }
}

/*****************************************************************
* Name: EasyLog
* Description: construct function
* param: -
* return: -
* author: 
* update: 
******************************************************************/
EasyLog::EasyLog(string logfilepath)
{
	m_logfilename = createlogfile(logfilepath);
	m_record_level = INFO;
    is_output = true;
	// m_log_opt = (EASYLOG_TIME | EASYLOG_FUNC | EASYLOG_LINE);
}

/*****************************************************************
* Name: setRecordlevel
* Description: set the record log level
* param: 
*   - EASYLOG_LEVEL level : record log level
* return: -
* author: 
* update: 
******************************************************************/
void EasyLog::setRecordlevel(EASYLOG_LEVEL level)
{
	m_record_level = level;
}
/*****************************************************************
* Name: WriteLog
* Description: write log function
* param:
*   EASYLOG_LEVEL level: log level
*   arg... :  log data
* return: -
* author: 
* update: 
******************************************************************/

void EasyLog::WriteLog(EASYLOG_LEVEL level, const char * fmt, ...)
{
    va_list argptr;

    if((level > m_record_level) || (!is_output))
    {
    	return;
    }

    va_start(argptr, fmt);
    easylog_write_log(level, fmt, argptr);
	va_end(argptr);

	return;
}

/*****************************************************************
* Name: getStream
* Description: get the log file stream or std cout
* param: -
* return: -
* author: 
* update: 
******************************************************************/
std::ostream& EasyLog::getStream()
{
	//if log file is not empty, open the file
	if(!m_logfilename.empty())
	{
		// m_log_file.open(m_logfilename.c_str(), ios::app, 0);
        m_log_file.open(m_logfilename.c_str(), ios::app);

	}

	// return log stream
	return m_log_file.is_open() ? m_log_file : std::cout;
}

/*****************************************************************
* Name: closeStream
* Description: close the log file stream or std cout
* param: -
* return: -
* author: 
* update: 
******************************************************************/
void EasyLog::closeStream()
{

	// return log stream
	if(m_log_file.is_open())
	{
		m_log_file.close();
	}
}


/*****************************************************************
* Name: createlogfile
* Description: close the log file stream or std cout
* param: -
* return: -
* author: 
* update: 
******************************************************************/
string EasyLog::createlogfile(string logpath)
{
    string file_path = ""; 
    char filetime[32] = {0};  
    struct tm tm_time;  
    time_t t_log;  
    std::string log_time = ""; 
    try
    {
        file_path = logpath;

        if(access(file_path.c_str(), F_OK) != 0) 
        {
            if(mkdir(file_path.c_str(), 0777) == -1)
            {
                return "";            
            }
        }
      
        t_log = time(NULL);  
        localtime_r(&t_log, &tm_time);  
        strftime(filetime, sizeof(filetime), "%Y%m%d%H%M%S", &tm_time); //日志的时间  
        
        log_time.assign(filetime, 0, 8);    

        file_path = file_path + "/" + log_time + ".log";
    }
    catch(exception& e)
    {
        cout<<"GetLogFilePath function occurs some exceptions."<<e.what()<<endl;
        file_path = "";        
    }
    catch(...)
    {
        cout<<"GetLogFilePath function occurs some exceptions."<<endl;
        file_path = "";
    }   

    return file_path;
}

/*****************************************************************
* Name: easylog_write_log
* Description: close the log file stream or std cout
* param: -
* return: -
* author: 
* update: 
******************************************************************/
void EasyLog::easylog_write_log(EASYLOG_LEVEL level, const char * fmt, va_list arg_list)
{

    m_lock.lock();

	std::ostream &log_stream = getStream();

	char content[LOG_CONTENT_SIZE] = {0}; 
	//write time to log
	log_stream << getTime_r().c_str();

    switch(level)
    {
        case ERROR:
           log_stream << "[ERROR] ";
           break;
        case WARN:
           log_stream << "[WARN] ";
            break;
        case INFO:
           log_stream << "[INFO] ";
           break;
        case DEBUG:
           log_stream << "[DEBUG] ";
           break;
        case TRACE:
           log_stream << "[TRACE] ";
           break;
        default:
           log_stream << "[NONE] ";
            break;
    }
	//write  content to log
	vsnprintf(content, LOG_CONTENT_SIZE, fmt, arg_list);

	log_stream << content << endl;

	closeStream();

	m_lock.unlock();
}

/*****************************************************************
* Name: getTime
* Description: get the time
* param: -
* return: -
* author: 
* update: 
******************************************************************/
string EasyLog::getTime_r()
{
    char logtime[32] = {0};  
    struct tm tm_time;  
    time_t t_log;  

    t_log = time(NULL);  
    localtime_r(&t_log, &tm_time);  
    strftime(logtime, sizeof(logtime), "[%Y-%m-%d %H:%M:%S] ", &tm_time); //日志的时间  

    return logtime;
}

/*****************************************************************
* Name: ShutdownLog
* Description: stop output log
* param: -
* return: -
* author: 
* update: 
******************************************************************/
void EasyLog::ShutdownLog()
{
    is_output = false;
}