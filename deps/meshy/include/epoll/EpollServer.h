
#pragma once

#include "../BasicServer.hpp"
#include "EpollConnection.h"

namespace meshy{
	class EpollServer : public BasicServer<EpollConnectionPtr> {
	public:
		virtual int32_t listen(std::string const& host, uint16_t port, int backlog)override;
	
		EpollConnectionPtr accept();
	};	
}

