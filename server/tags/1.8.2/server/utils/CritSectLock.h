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
 
#ifndef	_CASPAR_CRITSECTLOCK_H__
#define _CASPAR_CRITSECTLOCK_H__

#pragma once

namespace caspar {
namespace utils {

class CritSectLock
{
public:
	CritSectLock(CRITICAL_SECTION* pCritSect) : pCriticalSection_(pCritSect)
	{
		EnterCriticalSection(pCriticalSection_);
	}
	~CritSectLock()
	{
		LeaveCriticalSection(pCriticalSection_);
	}

private:
	CRITICAL_SECTION* pCriticalSection_;
};

}	//namespace utils
}	//namespace caspar

#endif	//_CASPAR_CRITSECTLOCK_H__