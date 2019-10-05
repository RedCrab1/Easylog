#ifndef __EASYLOG__
#define __EASYLOG__

#include <iostream>
#include <memory>
#include <thread>
#include <mutex>
#include <fstream>
#include <map>
#include <stdarg.h>
#include <unistd.h>
#include <sys/stat.h> 

using namespace std;

// FOR SINGLETON
#define DISALLOW_COPY_AND_ASSIGN(classname) \
 private:                                   \
  classname(const classname &);             \
  classname &operator=(const classname &);

#define DISALLOW_IMPLICIT_CONSTRUCTORS(classname) \
 private:                                         \
  classname() {}                                  \
  DISALLOW_COPY_AND_ASSIGN(classname);

#define DECLARE_SINGLETON(classname)        \
 public:                                    \
  static classname *instance() {            \
    static classname instance;              \
    return &instance;                       \
  }                                         \
  DISALLOW_IMPLICIT_CONSTRUCTORS(classname) \
 private:

enum EASYLOG_LEVEL
{
	ERROR = 0,		//此信息输出后，主体系统核心模块不能正常工作，需要修复才能正常工作。
	WARN,			//此信息输出后，系统一般模块存在问题，不影响系统运行。
	INFO,			//此信息输出后，主要是记录系统运行状态等关联信息。
	DEBUG,			//最细粒度的输出，除却上面各种情况外,调试时你希望输出的相关信息，都可以在这里输出
	TRACE			//最细粒度的输出，除却上面各种情况后，你希望输出的相关信息，都可以在这里输出。
};

// #define EASYLOG_TIME    (1 << 0)
// #define EASYLOG_FILE    (1 << 1)
// #define EASYLOG_LINE    (1 << 2)
// #define EASYLOG_FUNC    (1 << 3)

#define LOG_CONTENT_SIZE (1024)

// extern class EasyLog;




class EasyLog
{
public:
	EasyLog(string logfile);
	~EasyLog(){};

public:
	void WriteLog(EASYLOG_LEVEL level, const char * fmt, ...);
	void setRecordlevel(EASYLOG_LEVEL level);
	void setlog_opt(int log_opt);
	void ShutdownLog();
private:
	void easylog_write_log(EASYLOG_LEVEL level, const char * fmt, va_list arg_list);
	
	std::ostream& getStream();
	void closeStream();
	string createlogfile(string logpath);
    string getTime_r();


private:
	string m_logfilename;
	EASYLOG_LEVEL m_record_level;
	ofstream m_log_file;
	int  m_log_opt;
	std::mutex m_lock;
	bool is_output;
};

class LogFactory
{
	DECLARE_SINGLETON(LogFactory);

public:
	shared_ptr<EasyLog> getEasylog(string logID, string logfile = "");

private:
	map<string, shared_ptr<EasyLog> > m_easylog_map; 
};

#endif //__EASYLOG__