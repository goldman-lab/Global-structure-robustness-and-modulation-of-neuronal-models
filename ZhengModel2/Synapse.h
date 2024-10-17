//***CSynapse.h : header file ***

class CSynapse : public CObject
{
// Construction
	DECLARE_SERIAL(CSynapse)

public:
	CSynapse() {}  //empty constructor for serialization
	CSynapse(int type, BOOL exists);
	
//Attributes
public:
	int m_type; //the type of Synapse, e.g. Inhib=0 (see GlobalDefs)
	//CString m_name;  //text appearing in neuron page combo box
	BOOL m_bExists;  //TRUE if exists, FALSE if been removed
	double m_E;		//reversal potential
	double m_tau_decay; //time constant for decay of synaptic conductance
	                    //currently use instantaneous rise
	double m_gStep;	//increase in conductance upon receiving a spike (currently no saturation)
	double m_g; //conductance of this Synapse

//Operations
public:
	//CString GetName(){return m_name;}  //returns name of Synapse
	void m_Update_g(double dt);
	void m_SpikeReceived();
	double m_I(double V);    //current through this Synapse in [uA/(cm^2)]
	
	
// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CSynapse)
	//}}AFX_VIRTUAL
	void Serialize(CArchive& ar);

// Implementation
public:
	virtual ~CSynapse();

private:
};