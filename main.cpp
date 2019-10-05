#include "easylog.h"
#include <string>
using namespace std;

class SampleA
{
public:
	SampleA(string logId, string logfile)
	{
		logger = LogFactory::instance()->getEasylog(logId, logfile);
		logger->setRecordlevel(INFO);
	}

	void testfunc(string str)
	{
		logger->WriteLog(ERROR, "this is a test error log: %s", str.c_str());
		logger->WriteLog(INFO, "this is a test INFO log: %s", str.c_str());
		logger->WriteLog(DEBUG, "this is a test DEBUG log: %s", str.c_str());
	}

private:
	shared_ptr<EasyLog> logger;
};

class SampleB
{
public:
	SampleB(string logId, string logdir)
	{
		logger = LogFactory::instance()->getEasylog(logId, logdir);
		logger->setRecordlevel(INFO);
	}

	void testfunc(string str)
	{
		logger->WriteLog(ERROR, "this is a test error log: %s", str.c_str());
		logger->WriteLog(INFO, "this is a test INFO log: %s", str.c_str());
		logger->WriteLog(DEBUG, "this is a test DEBUG log: %s", str.c_str());
	}

private:
	shared_ptr<EasyLog> logger;
};


int main()
{
	//创建一个log文件，id为"Sample1", logid相同时会写入同一个log文件．
	SampleA  a1("Sample1", "./");
	//写入sample1.log中
	a1.testfunc("a1");

	//创建一个对象a2，logid仍然为"Sample1", 该对象的log会写入同一个文件.
	SampleA  a2("Sample1", "./");
	a2.testfunc("a2");

	//创建对象a3, logid仍然为"Sample1", 但是log文件不指定，此时仍会写入同一个文件
	SampleA  a3("Sample1", " ");
	a3.testfunc("a3");

	//使用SampleB创建对象b1, 仍指定logID为＂Sample1＂, log会写入同一个文件
	SampleB b1("Sample1", " ");
	b1.testfunc("b1");

	//写log与id有关，id相同会写入同一个文件，如果不同会写入不同的文件
	SampleB  b2("Sample2", "./");
	b2.testfunc("b2");

	//当第一次使用一个新的id, 并不指定文件路径时，直接输出在屏幕上
	SampleB  b3("Sample3", " ");
	b3.testfunc("b3");



	return 0;
}
