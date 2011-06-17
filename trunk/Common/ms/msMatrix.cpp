#include "msMatrix.h"

#include <memory.h>


msMatrix::msMatrix(void)
{
}


msMatrix::~msMatrix(void)
{
}


void msMatrix::multiply(msMatrix *m)
{
	msMatrix tmp;
	for (int i = 0; i < 4; i ++)
	{
		tmp.m_value[i][0] =	
			(this->m_value[i][0] * m->m_value[0][0]) + (this->m_value[i][1] * m->m_value[1][0]) + (this->m_value[i][2] * m->m_value[2][0]) + (this->m_value[i][3] * m->m_value[3][0]) ;
		tmp.m_value[i][1] =	
			(this->m_value[i][0] * m->m_value[0][1]) + (this->m_value[i][1] * m->m_value[1][1]) + (this->m_value[i][2] * m->m_value[2][1]) + (this->m_value[i][3] * m->m_value[3][1]) ;
		tmp.m_value[i][2] =	
			(this->m_value[i][0] * m->m_value[0][2]) + (this->m_value[i][1] * m->m_value[1][2]) + (this->m_value[i][2] * m->m_value[2][2]) + (this->m_value[i][3] * m->m_value[3][2]) ;
		tmp.m_value[i][3] =	
			(this->m_value[i][0] * m->m_value[0][3]) + (this->m_value[i][1] * m->m_value[1][3]) + (this->m_value[i][2] * m->m_value[2][3]) + (this->m_value[i][3] * m->m_value[3][3]) ;
	}
	memcpy(this->m_value, tmp.m_value, sizeof(float) * 16);
}


msMatrix msMatrix::identity()
{
	msMatrix identity;
	memset(identity.m_value, 0, sizeof(msMatrix));
	identity.m_value[0][0] = 1.0f;
	identity.m_value[1][1] = 1.0f;
	identity.m_value[2][2] = 1.0f;
	identity.m_value[3][3] = 1.0f;
	return identity;
}