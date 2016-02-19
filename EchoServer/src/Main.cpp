#include <iostream>
#include <mutex>
#include <condition_variable>

#include <glog/logging.h>

#include <NetworkV2/Static.h>

#include "User.h"
#include "Checker.h"
#include "Static.h"

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

void OnAcceptEvent(std::shared_ptr<::Network::Session> pSession, unsigned short port)
{
	std::shared_ptr<::Server::User> pUser = ::Server::Static::Instance().GETUSERPOOL()->NEW();

	pUser->SetSession(pSession->CAPTURE());
	pSession->SetObserver(pUser->CAPTURE());
}

int main()
{
	google::InitGoogleLogging("LOG");

	google::SetLogDestination(google::GLOG_INFO, "Logs/INFO_");
	google::SetLogDestination(google::GLOG_WARNING, "Logs/WARNING_");
	google::SetLogDestination(google::GLOG_ERROR, "Logs/ERROR_");

	google::SetStderrLogging(google::INFO);

	SetSignal();

	int threadCount = 0;
	int sessionCount = 5000;
	int sendPacketCount = 10000;
	int readPacketCount = 10000;
	::Network::Static	clsNetwork(threadCount, sendPacketCount, readPacketCount, sessionCount);
	clsNetwork.SetAcceptEventFunc(std::bind(OnAcceptEvent, std::placeholders::_1, std::placeholders::_2));
	clsNetwork.Listen(54486);

	::Server::Static	clsStatic(sessionCount);
	::Server::Checker	clsChecker;

	LOG(INFO) << __PRETTY_FUNCTION__ << " : SERVER START";

	std::mutex	__mutex__;
	std::unique_lock<std::mutex> lock(__mutex__);
	m_Condition.wait(lock);

	google::ShutdownGoogleLogging();
}
