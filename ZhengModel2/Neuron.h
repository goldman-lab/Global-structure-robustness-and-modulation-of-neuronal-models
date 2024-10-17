//****CNeuron.h:  header file ****


#define m_Compart(a) ((CCompartment*)m_CompartArray[a])
#define ERROR_TOLERANCE (1.0e-10)

/////////////////////////////////////////////////////////////////////////////
//CNeuron class stuff

class CNeuron : public CObject
{
// Construction
	DECLARE_SERIAL (CNeuron)
public:
	CNeuron();
	

//Attributes
public:
	//double m_gTot; //sum of the individual channel conductances
	//double m_ITotal; //sum of all channel currents + injection (=-capacitive current) 
	CObArray m_CompartArray; //Array of compartments
	double** m_B; //matrix of conductances/capacitance -- diagonal elements are total
											  //compartmental conductances; off-diagonal=intercompart
	double** m_S; //change of basis matrix--columns are eigenvects of B
	double** m_SInverse; //change of basis matrix to eigencoords
	double m_lambda[_MAX_COMPARTS]; //eigenvalues of B
	double m_V[_MAX_COMPARTS]; //Array of voltages
	double m_VEigen[_MAX_COMPARTS]; //Array of voltages converted to eigencoords
	double m_D2[_MAX_COMPARTS]; //D = -I_inject + E.g/C for each compartment, equals m_D of compartment class minus I_inject
	double m_D2Eigen[_MAX_COMPARTS]; //Array of D's converted to eigencoords

//Operations
public:
	void ComputeMe(double* I_inject, double dt); //calls compartments

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CNeuron)
	//}}AFX_VIRTUAL
	void Serialize(CArchive& ar);

// Implementation
public:
	virtual ~CNeuron();

private:
};
