//Neuron.cpp : implementation file
// 

#include "stdafx.h"
#include "ZhengModelHeaders.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CNeuron

IMPLEMENT_SERIAL(CNeuron, CObject, _VERSION_NUMBER)

CNeuron::CNeuron() //constructor
{
	//allocate size of matrices
	m_B = matrix(0, _MAX_COMPARTS - 1, 0, _MAX_COMPARTS - 1);
	m_S = matrix(0, _MAX_COMPARTS - 1, 0, _MAX_COMPARTS - 1);
	m_SInverse = matrix(0, _MAX_COMPARTS - 1, 0, _MAX_COMPARTS - 1);

	//init member vars
	m_CompartArray.SetSize(0, _MAX_COMPARTS);
	for (int i = 0; i < _MAX_COMPARTS; i++) 
		m_CompartArray.Add(new CCompartment(i));
	for (i = 0; i < _MAX_COMPARTS; i++) {
		m_lambda[i] = 0.0;
		m_V[i] = 0.0;
		m_VEigen[i] = 0.0;
		m_D2[i] = 0.0;
		m_D2Eigen[i] = 0.0;
		for (int j = 0; j < _MAX_COMPARTS; j++) {
			m_B[i][j] = 0.0;
			m_S[i][j] = 0.0;
			m_SInverse[i][j] = 0.0;
		}
	}
	
}

CNeuron::~CNeuron()
{
	free_matrix(m_B, 0, _MAX_COMPARTS - 1, 0, _MAX_COMPARTS - 1);
	free_matrix(m_S, 0, _MAX_COMPARTS - 1, 0, _MAX_COMPARTS - 1);
	free_matrix(m_SInverse, 0, _MAX_COMPARTS - 1, 0, _MAX_COMPARTS - 1);

	for (int i=0; i<_MAX_COMPARTS; i++) {
		delete m_CompartArray[i];
	}
	m_CompartArray.RemoveAll();
}

void CNeuron::ComputeMe(double* I_Inject, double dt)
{

	//COMPUTE COMPARTMENTS' VOLTAGES
	//Assemble B matrix
	for (int i = 0; i < _MAX_COMPARTS; i++) {
		for (int j = 0; j < _MAX_COMPARTS; j++) {
			m_B[i][j] = -(m_Compart(i)->m_gAxialTot())/(m_Compart(i)->m_C);
			if (i == j) {//Add membrane(includes synaptic) conductance to diagonal elements
				m_B[i][j] *= -1.0;
				m_B[i][j] += (m_Compart(i)->m_gMemTot())/(m_Compart(i)->m_C);
			}
		}
	}
	//Compute eigenvals and change of basis matrix of B
	Eigensystem(m_S, m_lambda, m_B);
	//Compute change of basis to eigencoords, S^-1
	Inverse(m_SInverse, m_S);
	//Convert voltages and D's(=g.E/C for each compart) into Eigenbasis, propagate, convert back
	for (i = 0; i < _MAX_COMPARTS; i++) {
		m_V[i] = m_Compart(i)->m_V;
		m_D2[i] = (I_Inject[i] + m_Compart(i)->m_D()); //D2 includes injection
	}
	MatrixMult(m_VEigen, m_SInverse, m_V, _MAX_COMPARTS, _MAX_COMPARTS);
	MatrixMult(m_D2Eigen, m_SInverse, m_D2, _MAX_COMPARTS, _MAX_COMPARTS);
	for (i = 0; i < _MAX_COMPARTS; i++) {
		if (fabs(m_lambda[i]*dt) < ERROR_TOLERANCE) 
			m_VEigen[i] = (m_D2Eigen[i]*dt + m_VEigen[i]);
		else
			m_VEigen[i] = ((m_D2Eigen[i]*(1. - exp(-m_lambda[i]*dt))/m_lambda[i]) + m_VEigen[i]*exp(-m_lambda[i]*dt));
	}
	MatrixMult(m_V, m_S, m_VEigen, _MAX_COMPARTS, _MAX_COMPARTS);
	for (i = 0; i < _MAX_COMPARTS; i++) {
		m_Compart(i)->m_V = m_V[i];
	}

	//COMPUTE COMPARTMENTS' ACTIVATION/INACTIVATIONS/Ca
	for (i = 0; i < _MAX_COMPARTS; i++) {
		m_Compart(i)->ComputeMe(dt);
	}

}

/////////////////////////////////////////////////////////////////////////////
// CNeuron serialization

void CNeuron::Serialize(CArchive& ar)
{
	CObject::Serialize(ar);
	if (ar.IsStoring())
	{
		for (int i = 0; i < _MAX_COMPARTS; i++) {
			ar << m_lambda[i];
			ar << m_V[i];
			ar << m_VEigen[i];
			ar << m_D2[i];
			ar << m_D2Eigen[i];
			for (int j = 0; j < _MAX_COMPARTS; j++) {
				ar << m_B[i][j];
				ar << m_S[i][j];
				ar << m_SInverse[i][j];
			}
		}
	}	
	else
	{
		for (int i = 0; i < _MAX_COMPARTS; i++) {
			ar >> m_lambda[i];
			ar >> m_V[i];
			ar >> m_VEigen[i];
			ar >> m_D2[i];
			ar >> m_D2Eigen[i];
			for (int j = 0; j < _MAX_COMPARTS; j++) {
				ar >> m_B[i][j];
				ar >> m_S[i][j];
				ar >> m_SInverse[i][j];
			}
		}
	}
	m_CompartArray.Serialize(ar);
	int temp = 0;
}