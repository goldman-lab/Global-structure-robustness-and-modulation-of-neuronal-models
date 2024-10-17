//Synapse.cpp : implementation file
// 

#include "stdafx.h"
#include "ZhengModelHeaders.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


//Global vars
	char SynName[_MAX_SYNAPSES][12] = {"Inhib", "Excit"};
/////////////////////////////////////////////////////////////////////////////
// CSynapse

IMPLEMENT_SERIAL(CSynapse, CObject, _VERSION_NUMBER)


CSynapse::CSynapse(int type, BOOL exists) //constructor
{
	m_type = type;  //type of Synapse, e.g. Inhib
	//m_name = CString(ChanName[m_type]);
	m_bExists = exists;
	switch (type) {
		case _Inhib:
			m_E = _Inhib_E;
			m_tau_decay = _Inhib_TAU_DECAY;
			m_gStep = _Inhib_GSTEP;
			m_g = 0.0; break;
		case _Excit:
			m_E = _Excit_E;
			m_tau_decay = _Excit_TAU_DECAY;
			m_gStep = _Excit_GSTEP;
			m_g = 0.0; break;
	}
}

CSynapse::~CSynapse()
{
}

void CSynapse::m_Update_g(double dt)
{
	m_g = m_g*exp(-dt/m_tau_decay);
}

void CSynapse::m_SpikeReceived()
{
	m_g += m_gStep;
}


double CSynapse::m_I(double V) { //in [uA/cm^2]
	return (m_g * (V - m_E));
}


/////////////////////////////////////////////////////////////////////////////
// CSynapse serialization

void CSynapse::Serialize(CArchive& ar)
{

	CObject::Serialize(ar);

	if (ar.IsStoring())
	{
		ar << m_type;
		//ar << m_name;
		ar << m_bExists;
		ar << m_E;
		ar << m_tau_decay;
		ar << m_gStep;
		ar << m_g;
	}	
	else
	{
		ar >> m_type;
		//ar >> m_name;
		ar >> m_bExists;
		ar >> m_E;
		ar >> m_tau_decay;
		ar >> m_gStep;
		ar >> m_g;
	}

}
