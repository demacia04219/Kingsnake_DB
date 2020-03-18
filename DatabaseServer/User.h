#ifndef	__USER_H__
#define	__USER_H__

#include <list>
#include <array>
#include "IOCPClient.h"

class User : public IOCPClient
{
public:
	static int userAcceptCount;
	int index;
	User();
	~User();

	virtual void recvProcess()			override;
	virtual void acceptProcess()		override;
	virtual void disconnectProcess()	override;
};

#endif