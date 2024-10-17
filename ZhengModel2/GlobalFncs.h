//****GlobalFncs.h****//

CZhengModelDoc* GetDoc();
CMDIChildWnd* GetViewWnd();
CNeuron* GetNeuron();
CCompartment* GetCompart(int type);
CChannel* GetChan(int comp, int chan);
CSynapse* GetSyn(int comp, int syn);
CSensor* GetSensor(int comp, int s);
CRunPage* GetRunPage();
CNeuronPage* GetNeuronPage();
CDisplayPage* GetDisplayPage();
CPlotsDialog* GetPlotsDialog();