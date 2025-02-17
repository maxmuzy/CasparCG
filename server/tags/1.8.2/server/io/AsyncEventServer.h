/*
* copyright (c) 2010 Sveriges Television AB <info@casparcg.com>
*
*  This file is part of CasparCG.
*
*    CasparCG is free software: you can redistribute it and/or modify
*    it under the terms of the GNU General Public License as published by
*    the Free Software Foundation, either version 3 of the License, or
*    (at your option) any later version.
*
*    CasparCG is distributed in the hope that it will be useful,
*    but WITHOUT ANY WARRANTY; without even the implied warranty of
*    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
*    GNU General Public License for more details.

*    You should have received a copy of the GNU General Public License
*    along with CasparCG.  If not, see <http://www.gnu.org/licenses/>.
*
*/
 
// AsyncEventServer.h: interface for the AsyncServer class.
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_ASYNCEVENTSERVER_H__0BFA29CB_BE4C_46A0_9CAE_E233ED27A8EC__INCLUDED_)
#define AFX_ASYNCEVENTSERVER_H__0BFA29CB_BE4C_46A0_9CAE_E233ED27A8EC__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <string>
#include <map>
#include <vector>
#include <functional>

#include "..\utils\thread.h"
#include "..\utils\lockable.h"

#include "ProtocolStrategy.h"
#include "..\controller.h"
#include "SocketInfo.h"

namespace caspar {
namespace IO {

typedef std::function<void(caspar::IO::SocketInfoPtr)> ClientDisconnectEvent;

class AsyncEventServer : public utils::IRunnable, public caspar::IController
{
	static long instanceCount_;

	AsyncEventServer();
	AsyncEventServer(const AsyncEventServer&);
	AsyncEventServer& operator=(const AsyncEventServer&);

public:
	explicit AsyncEventServer(int port);
	~AsyncEventServer();

	bool Start();
	void SetProtocolStrategy(ProtocolStrategyPtr pPS) {
		pProtocolStrategy_ = pPS;
	}

	void Stop();

	void SetClientDisconnectHandler(ClientDisconnectEvent handler);
	
private:
	utils::Thread	listenThread_;
	void Run(HANDLE stopEvent);
	bool OnUnhandledException(const std::exception&) throw();

	bool OnAccept(SocketInfoPtr&);
	bool OnRead(SocketInfoPtr&);
	void OnWrite(SocketInfoPtr&);
	void OnClose(SocketInfoPtr&);
	void OnError(HANDLE waitEvent, int errorCode);

	SocketInfoPtr		pListenSocketInfo_;
	ProtocolStrategyPtr	pProtocolStrategy_;
	int					port_;

	friend class SocketInfo;
	void DoSend(SocketInfo&);
	void DisconnectClient(SocketInfo&);

	void LogSocketError(const TCHAR* pStr, int socketError = 0);

	class SocketInfoCollection : private utils::LockableObject
	{
		SocketInfoCollection(const SocketInfoCollection&);
		SocketInfoCollection& operator=(const SocketInfoCollection&);

		typedef std::map<HANDLE, SocketInfoPtr> SocketInfoMap;
		typedef std::vector<HANDLE> HandleVector;

	public:
		SocketInfoCollection();
		~SocketInfoCollection();

		bool AddSocketInfo(SocketInfoPtr& pSocketInfo);
		void RemoveSocketInfo(SocketInfoPtr& pSocketInfo);
		void RemoveSocketInfo(HANDLE);
		void CopyCollectionToArray(HANDLE*, int maxCount);

		bool FindSocketInfo(HANDLE, SocketInfoPtr& pResult);

		bool IsDirty() {
			return bDirty_;
		}
		void ClearDirty() {
			bDirty_ = false;
		}

		std::size_t Size() {
			return waitEvents_.size();
		}
		void Clear();

		ClientDisconnectEvent onSocketInfoRemoved;

	private:
		SocketInfoMap socketInfoMap_;
		HandleVector waitEvents_;
		bool bDirty_;
	};
	SocketInfoCollection socketInfoCollection_;
};
typedef std::tr1::shared_ptr<AsyncEventServer> AsyncEventServerPtr;

}	//namespace IO
}	//namespace caspar

#endif // !defined(AFX_ASYNCEVENTSERVER_H__0BFA29CB_BE4C_46A0_9CAE_E233ED27A8EC__INCLUDED_)
