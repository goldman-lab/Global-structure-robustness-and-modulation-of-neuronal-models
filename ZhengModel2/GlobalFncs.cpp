//****GlobalFncs.cpp****//
//Global functions to make moving between view/doc/dialogs easier

#include "StdAfx.h"
#include "ZhengModelHeaders.h"

//Returns pointer to the document to ease the back and forth from dialogs, views, etc.
CZhengModelDoc* GetDoc() {
	CMainFrame* pWnd = (CMainFrame*)AfxGetMainWnd();
	CMDIChildWnd* pChild = (CMDIChildWnd*)(pWnd->MDIGetActive());
	CZhengModelView* pView = (CZhengModelView*)(pChild->GetActiveView());
	return ((CZhengModelDoc*)(pView->GetDocument()));
	
}

//Returns pointer to the view window
CMDIChildWnd* GetViewWnd() {
	CMainFrame* pWnd = (CMainFrame*)AfxGetMainWnd();
	return ((CMDIChildWnd*)(pWnd->MDIGetActive()));
}

//Returns pointer to the neuron
CNeuron* GetNeuron() { 
	return (GetDoc()->m_pTheNeuron);
}

CCompartment* GetCompart(int type) {
	return ((CCompartment*) (GetNeuron()->m_CompartArray[type]));

}

//Returns pointer to the channel
CChannel* GetChan(int comp, int chan) {
	return ((CChannel*) (GetCompart(comp)->m_ChanArray[chan]));
}

//Returns pointer to the channel
CSynapse* GetSyn(int comp, int syn) {
	return ((CSynapse*) (GetCompart(comp)->m_SynArray[syn]));
}

//Returns pointer to the sensor
CSensor* GetSensor(int comp, int s) {
	return ((CSensor*) (GetCompart(comp)->m_SensorArray[s]));
}


//Returns pointer to the run page
CRunPage* GetRunPage() {
	CMainFrame* pWnd = (CMainFrame*)AfxGetMainWnd();
	return (pWnd->m_pCtrlSheet->m_pRunPage);
}

//Returns pointer to the neuron page
CNeuronPage* GetNeuronPage() {
	CMainFrame* pWnd = (CMainFrame*)AfxGetMainWnd();
	return (pWnd->m_pCtrlSheet->m_pNeuronPage);
}

//Returns pointer to the display page
CDisplayPage* GetDisplayPage() {
	CMainFrame* pWnd = (CMainFrame*)AfxGetMainWnd();
	return (pWnd->m_pCtrlSheet->m_pDisplayPage);
}

//Returns pointer to the plots dialog page
CPlotsDialog* GetPlotsDialog() {
	CMainFrame* pWnd = (CMainFrame*)AfxGetMainWnd();
	return (pWnd->m_pPlotsDialog);
}





