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

int main()
{
	google::SetLogDestination(google::GLOG_INFO, "/home/mega/workspace_kor/TestServer/Logs");
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

	std::mutex	__mutex__;
	std::unique_lock<std::mutex> lock(__mutex__);
	m_Condition.wait(lock);

	clsDispatch.Finalize();
}
