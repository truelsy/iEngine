/*
 * Session.h
 *
 *  Created on: 2016. 2. 17.
 *      Author: mega
 */

#ifndef USER_H_
#define USER_H_

#include <stddef.h>
#include <glog/logging.h>
#include <NetworkV2/Static.h>

namespace Server {

class User : public ::Network::Session::Observer, public std::enable_shared_from_this<User>
{
public:
	User();
	virtual ~User();

	void	Initialize() {};
	void	Finalize() {};

	std::shared_ptr<User> CAPTURE() { return shared_from_this(); }

	void	SetSession(std::shared_ptr<::Network::Session> session) { _pSession = session; }

	virtual void OnOpen();
	virtual void OnClose();
	virtual void OnMessage();

private:
	std::shared_ptr<::Network::Session> _pSession;
};

} /*Server*/


#endif /* USER_H_ */
