//****CCompartment.h:  header file ****

#define m_Chan(a) ((CChannel*)m_ChanArray[a])
#define m_Syn(a)  ((CSynapse*)m_SynArray[a])
#define m_Sens(a) ((CSensor*)m_SensorArray[a])

/////////////////////////////////////////////////////////////////////////////
// CCompartment class

class CCompartment : public CObject 
{
// Construction
	DECLARE_SERIAL (CCompartment)
public:
	CCompartment() {}  //empty constructor for serialization
	CCompartment(int type);
	

//Attributes
public:
	int m_type; //identifies which compartment we're in
	double m_Area; //Area of compartment in [cm^2]
	double m_V0; //initial V -- read in from interface
	double m_V;	//Voltage in mV
	double m_Ca0; //initial Ca -- read in from interface
	double m_Ca;  //Calcium concentration in uM
	double m_C;  //Capacitance in uF/(cm^2)
	double m_tau_eff; //for use in computing ave sensor values over this time: =gmax_tau/min[sensor target values]
	double m_gAxial[_MAX_COMPARTS]; //conductances in mS/(this compart's cm^2) coming in from each other compart (0 from self)  
	double m_nA_to_uMCa; //conversion from uM of Ca to nA Ca current
	CObArray m_ChanArray;  //Array of channels
	CObArray m_SynArray;   //Array of synapse types
	CObArray m_SensorArray; //Array of Ca sensors on different time scales

//Operations
public:
	double m_D(); //g.E/C where dot product is over channels
	double m_gMemTot(); //sum of the channel conductances
	double m_gAxialTot(); //Axial conductance, in uF/(cm^2 surf area) units
	double m_ICa(); //total Ca current, i.e. current through CaT and CaP channels, in [nA]
	double m_ITotal(double IInject); //sum of all channel currents + injection (=-capacitive current)
	void ComputeMe(double dt);	//returns its voltage

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CCompartment)
	//}}AFX_VIRTUAL
	void Serialize(CArchive& ar);

// Implementation
public:
	virtual ~CCompartment();

private:
};