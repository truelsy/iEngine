#include <iostream>
#include <mutex>
#include <condition_variable>

#include <glog/logging.h>

#include "User.h"
#include "Checker.h"
#include "Static.h"
#include "NState/Static.h"
#include <Network/Static.h>
#include <Dispatch/Static.h>
#include <Memcache/Static.h>

std::condition_variable m_Condition;

void SignalHandler(int sig)
{
	m_Condition.notify_one();
}

void SetSignal()
{
	struct sigaction sa;
	sigemptyset(&sa.sa_mask);
	sa.sa_flags = 0;
	sa.sa_handler = SignalHandler;
	sigaction(SIGHUP, &sa, NULL);
	sigaction(SIGTERM, &sa, NULL);
	sigaction(SIGINT, &sa, NULL);

	// ignore SIGPIPE
	sigemptyset(&sa.sa_mask);
	sa.sa_flags = 0;
	sa.sa_handler = SIG_IGN;
	sigaction(SIGPIPE, &sa, NULL);
}

//void Test_Memcached()
//{
//	// SET TEST
//	{
//		::Memcache::Command::Data	clsData;
//		clsData._key		= "KEY_12345";
//		clsData._eAction	= ::Memcache::Command::SET;
//
//		::Memcache::Command::Auto clsAuto(::Memcache::GETCOMMANDPOOL());
//		clsAuto->SetData(clsData);
//
//		clsAuto->GetResultSet().Init(1, 5);
//
//		bool vBool = true;
//		int  vInt = 4486;
//		double vDouble = 123.456;
//		std::string vString = "MyString";
//		uint16_t vUint16 = 12;
//
//		clsAuto->GetResultSet() << vBool << vInt << vDouble << vString.c_str() << vUint16;
//
//		::Memcache::GETSESSION()->Execute(clsAuto);
//		std::cout << "Memcached Set Result : " << clsAuto->GetReturnCode() << std::endl;
//		std::cout << "Memcached Set String : " << clsAuto->GetErrorString() << std::endl;
//	}
//
//	// GET TEST
//	{
//		::Memcache::Command::Data clsData;
//		clsData._key		= "KEY_12345";
//		clsData._eAction	= ::Memcache::Command::GET;
//
//		::Memcache::Command::Auto clsAuto(::Memcache::GETCOMMANDPOOL());
//		clsAuto->SetData(clsData);
//
//		bool bRet = ::Memcache::GETSESSION()->Execute(clsAuto);
//		std::cout << "bRet : " << bRet << std::endl;
//
//		bool vBool;
//		int  vInt;
//		double vDouble;
//		std::string vString;
//		uint16_t vUint16;
//
//		while (clsAuto->GetResultSet().MoreRecord())
//		{
//			vBool	= atoi(clsAuto->GetResultSet().ROW[0]);
//			vInt	= atoi(clsAuto->GetResultSet().ROW[1]);
//			vDouble	= atof(clsAuto->GetResultSet().ROW[2]);
//			vString	= clsAuto->GetResultSet().ROW[3];
//			vUint16	= atoi(clsAuto->GetResultSet().ROW[4]);
//		}
//
//		std::cout << "Memcached Get Result : " << clsAuto->GetReturnCode() << std::endl;
//		std::cout << "Memcached Get String : " << clsAuto->GetErrorString() << std::endl;
//		std::cout << "Memcached Get Bool : " << vBool << std::endl;
//		std::cout << "Memcached Get Int : " << vInt << std::endl;
//		std::cout << "Memcached Get Double : " << vDouble << std::endl;
//		std::cout << "Memcached Get String : " << vString << std::endl;
//		std::cout << "Memcached Get Uint16 : " << vUint16 << std::endl;
//	}
//}

int main()
{
	google::SetLogDestination(google::GLOG_INFO, "../Logs");
	google::InitGoogleLogging("Log");

	SetSignal();

	::ChatServer::Checker			clsChecker;
	::ChatServer::Static			clsStatic;
	::ChatServer::NState::Static	clsStateStatic;

	::Dispatch::Static			clsDispatch;
	clsDispatch.Initialize();

	int nThreadCount = 2;
	int nSendPacketCount = 10000;
	int nReadPacketCount = 10000;
	::BoostAsioNetwork::Static	clsNetwork(nThreadCount, nSendPacketCount, nReadPacketCount);

	clsNetwork.SetListener(::Dispatch::GETHANDLER());


	for (int i = 0; i < 1024; i++)
	{
		ChatServer::User * pUser = new ChatServer::User;
		clsNetwork.GETSESSIONPOOL()->POST(pUser);
	}

	clsNetwork.Start();

//	::Memcache::Static	clsMemcache;
//	bool bMemcached = ::Memcache::GETSESSION()->Initialize("127.0.0.1:11211");
//	std::cout << "Memcached Connect : " << bMemcached << std::endl;
//
//	Test_Memcached();

	std::mutex	__mutex__;
	std::unique_lock<std::mutex> lock(__mutex__);
	m_Condition.wait(lock);

	clsDispatch.Finalize();
}
