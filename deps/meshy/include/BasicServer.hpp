#pragma once

#include "socket.h"
#include <functional>

namespace meshy {
	
	class IStream;

	template<typename ConnectionType>
	class BasicServer : public Socket {
	public:
		typedef std::function<void(IStream*)> ConnectIndicationHandler;
		typedef std::function<void(IStream*)> DisconnectIndicationHandler;

		int32_t bind(std::string const& host, uint16_t port) {
			NativeSocket listenfd = socket(AF_INET, SOCK_STREAM, 0);
			if (listenfd < 0)
			{
				TRACE_ERROR("Create socket failed!");
				exit(1);
			}
			int32_t option = 1;
			setsockopt(listenfd, SOL_SOCKET, SO_REUSEADDR, (char*)&option, sizeof(option));
			NativeSocketAddress srvAddr;
			inet_pton(AF_INET, host.c_str(), &srvAddr.sin_addr);
			srvAddr.sin_family = AF_INET;
			srvAddr.sin_port = htons(port);
			int32_t errorCode = ::bind(listenfd, (sockaddr*)&srvAddr, sizeof(sockaddr));
			if (SOCKET_ERROR == errorCode)
			{
				TRACE_ERROR("Bind failed. Error: " + GetLastError());
				assert(false);
				return errorCode;
			}
			SetNativeSocket(listenfd);
			return 0;
		}

		virtual int32_t listen(std::string const& host, uint16_t port, int backlog) = 0;
		virtual void OnConnectIndication(ConnectIndicationHandler handler) = 0;
		virtual void OnDisconnectIndication(DisconnectIndicationHandler handler) = 0;

		virtual ConnectionType accept() = 0;
	};
}
