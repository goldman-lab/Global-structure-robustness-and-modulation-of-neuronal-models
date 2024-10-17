//***CChannel.h : header file ***

class CChannel : public CObject
{
// Construction
	DECLARE_SERIAL(CChannel)

public:
	CChannel() {}  //empty constructor for serialization
	CChannel(int type, BOOL exists);
	
//Attributes
public:
	int m_type; //the type of channel, e.g. I_Leak=0 (see GlobalDefs)
	CString m_name;  //text appearing in neuron page combo box
	BOOL m_bCaChannel; //TRUE if a Ca-admitting channel
	BOOL m_bExists;  //TRUE if exists, FALSE if been removed
	double m_E;		//reversal potential
	double m_gMax0; //initial max conductance
	double m_gMax;	//current max conductance
	int m_p;	//number of activation gates in channel -- rename p???
	int m_q;	//number of inactivation gates in channel -- rename q???
	double m_m;
	double m_h;
	double m_tau_gMax;  //time constant for gmax changing
	double m_SensorCoupling[_MAX_SENSORS]; //weights for computing error in gMax

//Operations
public:
	CString GetName(){return m_name;}  //returns name of channel
	void m_Update_m(double V, double Ca, double dt);
	void m_Update_h(double V, double dt);
	void m_Update_gMax(int compartment, double dt);
	void m_Update_E(double Ca); //uses Nernst equ. to update reversal pot'l of Ca channels
	double m_g();      //conductance of this channel
	double m_I(double V);    //current through this channel in [uA/(cm^2)]
	double m_TotalSensorError(int compartment); //Error message from sensor for this channel
	double m_m_inf(double V, double Ca);  //steady-state activation
	double m_h_inf(double V);	//steady-state inactivation
	double m_tau_m(double V);	//time constant for activation gate
	double m_tau_h(double V);  //time constant for inactivation gate

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CChannel)
	//}}AFX_VIRTUAL
	void Serialize(CArchive& ar);

// Implementation
public:
	virtual ~CChannel();

private:
};