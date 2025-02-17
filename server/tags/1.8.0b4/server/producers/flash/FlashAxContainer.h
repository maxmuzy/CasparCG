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
 
#ifndef _FLASHAXCONTAINER_H__
#define _FLASHAXCONTAINER_H__

#pragma once

#include <atlbase.h>
#include <atlcom.h>
#include <atlhost.h>


#include <ocmm.h>
#include "..\..\frame\Frame.h"
#include <vector>

#include "axflash.h"
//#import "progid:ShockwaveFlash.ShockwaveFlash.9" no_namespace, named_guids

namespace caspar {

class FlashProducer;

namespace flash {

class TimerHelper;
struct DirtyRect {
	DirtyRect(LONG l, LONG t, LONG r, LONG b, bool e) : bErase(e), bWhole(false) { 
		rect.left = l;
		rect.top = t;
		rect.right = r;
		rect.bottom = b; 
	}
	DirtyRect(const RECT& rc, bool e) : bErase(e), bWhole(false)  {
		rect.left = rc.left;
		rect.top = rc.top;
		rect.right = rc.right;
		rect.bottom = rc.bottom; 
	}
	explicit DirtyRect(bool b) : bWhole(b) {}

	RECT	rect;
	bool	bErase;
	bool	bWhole;
};

extern _ATL_FUNC_INFO fnInfoFlashCallEvent;
extern _ATL_FUNC_INFO fnInfoReadyStateChangeEvent;

class ATL_NO_VTABLE FlashAxContainer : 
		public ATL::CComCoClass<FlashAxContainer , &CLSID_NULL>,
		public ATL::CComObjectRootEx<ATL::CComMultiThreadModel>,
		public IOleClientSite,
		public IOleContainer,
		public IOleControlSite,
		public IOleInPlaceSiteWindowless,
		public IObjectWithSiteImpl<FlashAxContainer>,
		public IServiceProvider,
		public IAdviseSink,
		public ITimerService,
		public ITimer,
		public IDispatchImpl<IDispatch>,
		public IDispEventSimpleImpl<0, FlashAxContainer, &DIID__IShockwaveFlashEvents>
{

public:

	FlashAxContainer();
	virtual ~FlashAxContainer();

	DECLARE_NO_REGISTRY()
	DECLARE_POLY_AGGREGATABLE(FlashAxContainer)
	DECLARE_GET_CONTROLLING_UNKNOWN()

	BEGIN_COM_MAP(FlashAxContainer)
		COM_INTERFACE_ENTRY(IDispatch)
		COM_INTERFACE_ENTRY(IOleClientSite)
		COM_INTERFACE_ENTRY(IObjectWithSite)
		COM_INTERFACE_ENTRY(IOleControlSite)
		COM_INTERFACE_ENTRY(IOleContainer)

		COM_INTERFACE_ENTRY(IOleInPlaceSiteWindowless)
		COM_INTERFACE_ENTRY(IOleInPlaceSiteEx)
		COM_INTERFACE_ENTRY(IOleInPlaceSite)
		COM_INTERFACE_ENTRY(IOleWindow)

		COM_INTERFACE_ENTRY(IServiceProvider)

		COM_INTERFACE_ENTRY(IAdviseSink)

		COM_INTERFACE_ENTRY(ITimerService)

		COM_INTERFACE_ENTRY(ITimer)
	END_COM_MAP()

	BEGIN_SINK_MAP(FlashAxContainer)
		SINK_ENTRY_INFO(0, DIID__IShockwaveFlashEvents, 0xc5, OnFlashCall, &fnInfoFlashCallEvent)
		SINK_ENTRY_INFO(0, DIID__IShockwaveFlashEvents, 0xfffffd9f, OnReadyStateChange, &fnInfoReadyStateChangeEvent)
	END_SINK_MAP()

	void STDMETHODCALLTYPE OnFlashCall(BSTR request);
	void STDMETHODCALLTYPE OnReadyStateChange(long newState);

// IObjectWithSite
	STDMETHOD(SetSite)(IUnknown* pUnkSite);

// IOleClientSite
	STDMETHOD(SaveObject)();
	STDMETHOD(GetMoniker)(DWORD dwAssign, DWORD dwWhichMoniker, IMoniker** ppmk);
	STDMETHOD(GetContainer)(IOleContainer** ppContainer);
	STDMETHOD(ShowObject)();
	STDMETHOD(OnShowWindow)(BOOL fShow);
	STDMETHOD(RequestNewObjectLayout)();

// IOleInPlaceSite
	STDMETHOD(GetWindow)(HWND* pHwnd);
	STDMETHOD(ContextSensitiveHelp)(BOOL fEnterMode);
	STDMETHOD(CanInPlaceActivate)();
	STDMETHOD(OnInPlaceActivate)();
	STDMETHOD(OnInPlaceDeactivate)();
	STDMETHOD(OnUIActivate)();
	STDMETHOD(OnUIDeactivate)(BOOL fUndoable);
	STDMETHOD(GetWindowContext)(IOleInPlaceFrame** ppFrame, IOleInPlaceUIWindow** ppDoc, LPRECT lprcPosRect, LPRECT lprcClipRect, LPOLEINPLACEFRAMEINFO pFrameInfo);
	STDMETHOD(Scroll)(SIZE scrollExtant);
	STDMETHOD(DiscardUndoState)();
	STDMETHOD(DeactivateAndUndo)();
	STDMETHOD(OnPosRectChange)(LPCRECT lprcPosRect);

// IOleInPlaceSiteEx
	STDMETHOD(OnInPlaceActivateEx)(BOOL* pfNoRedraw, DWORD dwFlags);
	STDMETHOD(OnInPlaceDeactivateEx)(BOOL fNoRedraw);
	STDMETHOD(RequestUIActivate)();

// IOleInPlaceSiteWindowless
	STDMETHOD(CanWindowlessActivate)();
	STDMETHOD(GetCapture)();
	STDMETHOD(SetCapture)(BOOL fCapture);
	STDMETHOD(GetFocus)();
	STDMETHOD(SetFocus)(BOOL fGotFocus);
	STDMETHOD(GetDC)(LPCRECT pRect, DWORD grfFlags, HDC* phDC);
	STDMETHOD(ReleaseDC)(HDC hDC);
	STDMETHOD(InvalidateRect)(LPCRECT pRect, BOOL fErase);
	STDMETHOD(InvalidateRgn)(HRGN hRGN, BOOL fErase);
	STDMETHOD(ScrollRect)(INT dx, INT dy, LPCRECT pRectScroll, LPCRECT pRectClip);
	STDMETHOD(AdjustRect)(LPRECT prc);
	STDMETHOD(OnDefWindowMessage)(UINT msg, WPARAM wParam, LPARAM lParam, LRESULT* plResult);

// IOleControlSite
	STDMETHOD(OnControlInfoChanged)();
	STDMETHOD(LockInPlaceActive)(BOOL fLock);
	STDMETHOD(GetExtendedControl)(IDispatch** ppDisp);
	STDMETHOD(TransformCoords)(POINTL* pPtlHimetric, POINTF* pPtfContainer, DWORD dwFlags);
	STDMETHOD(TranslateAccelerator)(LPMSG lpMsg, DWORD grfModifiers);
	STDMETHOD(OnFocus)(BOOL fGotFocus);
	STDMETHOD(ShowPropertyFrame)();

// IAdviseSink
	STDMETHOD_(void, OnDataChange)(FORMATETC* pFormatetc, STGMEDIUM* pStgmed);
	STDMETHOD_(void, OnViewChange)(DWORD dwAspect, LONG lindex);
	STDMETHOD_(void, OnRename)(IMoniker* pmk);
	STDMETHOD_(void, OnSave)();
	STDMETHOD_(void, OnClose)();

// IServiceProvider
	STDMETHOD(QueryService)( REFGUID rsid, REFIID riid, void** ppvObj);

// IOleContainer
	STDMETHOD(ParseDisplayName)(IBindCtx*, LPOLESTR, ULONG*, IMoniker**)
	{
		ATLTRACENOTIMPL(_T("IOleContainer::ParseDisplayName"));
	}
	STDMETHOD(EnumObjects)(DWORD, IEnumUnknown** ppenum)
	{
		if (ppenum == NULL)
			return E_POINTER;
		*ppenum = NULL;
		typedef CComObject<CComEnum<IEnumUnknown, &__uuidof(IEnumUnknown), IUnknown*, _CopyInterface<IUnknown> > > enumunk;
		enumunk* p = NULL;
		ATLTRY(p = new enumunk);
		if(p == NULL)
			return E_OUTOFMEMORY;
		IUnknown* pTemp = m_spUnknown;
		// There is always only one object.
		HRESULT hRes = p->Init(reinterpret_cast<IUnknown**>(&pTemp), reinterpret_cast<IUnknown**>(&pTemp + 1), GetControllingUnknown(), AtlFlagCopy);
		if (SUCCEEDED(hRes))
			hRes = p->QueryInterface(__uuidof(IEnumUnknown), (void**)ppenum);
		if (FAILED(hRes))
			delete p;
		return hRes;
	}
	STDMETHOD(LockContainer)(BOOL)
	{
		ATLTRACENOTIMPL(_T("IOleContainer::LockContainer"));
	}

//ITimerService
	STDMETHOD(CreateTimer)(ITimer *pReferenceTimer, ITimer **ppNewTimer);
	STDMETHOD(GetNamedTimer)(REFGUID rguidName, ITimer **ppTimer);
	STDMETHOD(SetNamedTimerReference)(REFGUID rguidName, ITimer *pReferenceTimer);

//ITimer
	STDMETHOD(Advise)(VARIANT vtimeMin, VARIANT vtimeMax, VARIANT vtimeInterval, DWORD dwFlags, ITimerSink *pTimerSink, DWORD *pdwCookie);
	STDMETHOD(Unadvise)(DWORD dwCookie);
	STDMETHOD(Freeze)(BOOL fFreeze);
	STDMETHOD(GetTime)(VARIANT *pvtime);
	int GetFPS();

	HRESULT CreateAxControl();
	void DestroyAxControl();
	HRESULT QueryControl(REFIID iid, void** ppUnk);

	template <class Q>
	HRESULT QueryControl(Q** ppUnk)
	{
		return QueryControl(__uuidof(Q), (void**)ppUnk);
	}

//	static ATL::CComObject<FlashAxContainer>* CreateInstance();

	bool DrawControl(HDC targetDC);

	TimerHelper* pTimerHelper;
	volatile bool bInvalidRect_;
	volatile bool bCallSuccessful_;
	volatile bool bReadyToRender_;
	volatile bool bIsEmpty_;
	volatile bool bHasNewTiming_;
	FlashProducer* pFlashProducer_;
	std::vector<DirtyRect> bDirtyRects_;

	void SetFormat(const caspar::FrameFormatDescription&);
	bool IsReadyToRender() const;
	void EnterFullscreen();

	static bool CheckForFlashSupport();
private:
	static CComBSTR flashGUID_;

//	state
	bool		bUIActive_;
	bool		bInPlaceActive_;
	unsigned long		bHaveFocus_ : 1;
	unsigned long		bCapture_ : 1;

	DWORD m_dwOleObject;
	DWORD m_dwMiscStatus;
	SIZEL m_hmSize;
	SIZEL m_pxSize;
	RECT m_rcPos;

	ATL::CComPtr<IUnknown> m_spUnknown;
	ATL::CComPtr<IOleObject> m_spServices;
	ATL::CComPtr<IOleObject> m_spOleObject;
	ATL::CComPtr<IViewObjectEx> m_spViewObject;
	ATL::CComPtr<IOleInPlaceObjectWindowless> m_spInPlaceObjectWindowless;

//	ATL::CComPtr<ATL::CComObject<MyMoniker> > m_spMyMoniker;
};

}	//namespace flash
}	//namespace caspar

#endif	//_FLASHAXCONTAINER_H__