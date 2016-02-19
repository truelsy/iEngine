/*
 * Checker.h
 *
 *  Created on: 2016. 2. 19.
 *      Author: mega
 */

#ifndef CHECKER_H_
#define CHECKER_H_

#include <glog/logging.h>
#include <Timer/TimerHandler.h>
#include <NetworkV2/Static.h>

namespace Server {

class Checker
{
public:
	enum class TimerID {
		__NONE__,
		POOL_CHECK,
		TEST_TIMER,
		__MAX__,
	};

public:
	Checker()
	{
		_poolCheckTimer = std::make_shared<::Timer::TimerHandler>();
		_poolCheckTimer->SetTimer(1000 * 10, (int)TimerID::POOL_CHECK, std::bind(&Checker::OnTime, this, std::placeholders::_1, std::placeholders::_2));

		_testTimer = std::make_shared<::Timer::TimerHandler>();
		_testTimer->SetTimer(1000 * 1, (int)TimerID::TEST_TIMER, std::bind(&Checker::OnTime, this, std::placeholders::_1, std::placeholders::_2));
	};

	~Checker()
	{
	};

	void OnTime(const boost::system::error_code & ec, int timerId)
	{
		switch (timerId)
		{
		case (int)TimerID::POOL_CHECK:
			{
				int iSessionPoolCount = ::Network::Static::Instance().GETSESSIONPOOL()->GetPoolSize();
				int iReadPacketPoolCount = ::Network::Static::Instance().GETPACKETPOOL_READ()->GetPoolSize();
				int iSendPacketPoolCount = ::Network::Static::Instance().GETPACKETPOOL_SEND()->GetPoolSize();

				LOG(INFO) << __FUNCTION__ << " : [Pool Check] Session(" << iSessionPoolCount << ") ReadPacket(" << iReadPacketPoolCount << ") SendPacket(" << iSendPacketPoolCount << ")";

				// Timer 재등록
				_poolCheckTimer->ResumeTimer();
			}
			break;
		case (int)TimerID::TEST_TIMER:
			{
				LOG(INFO) << __FUNCTION__ << " : [Test Timer]";

				// Timer 재등록
				//_testTimer->ResumeTimer();
				_testTimer->KillTimer();
			}
			break;
		}
	}

private:
	std::shared_ptr<::Timer::TimerHandler> _poolCheckTimer;
	std::shared_ptr<::Timer::TimerHandler> _testTimer;
};

} /*Server*/


#endif /* CHECKER_H_ */
