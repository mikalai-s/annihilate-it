#include "MsAnimationBundle.h"
#include "stdlib.h"
#include "stdio.h"

MsAnimationBundle::MsAnimationBundle()
{
}

void MsAnimationBundle::performStep()
{
	for(msAnimationIterator i = m_list.begin(); i != m_list.end(); i ++)
	{
		MsAnimationBase *animation = *i;
		if(animation->performStep() == 0)
		{
			m_list.remove(animation);
			delete animation;
		}
	}
}

MsAnimationBundle::~MsAnimationBundle()
{
	while(m_list.size() > 0)
	{
		MsAnimationBase *a = m_list.front();
		m_list.remove(a);
		delete a;
	}
}

void MsAnimationBundle::_lineStep2(MsAnimationBase *anim)
{
	MsAnimation<float*> *a = (MsAnimation<float*>*)anim;
	float *from = a->m_from;
	float *to = a->m_to;
	*from += (*to - *from) / anim->m_count;
}

void MsAnimationBundle::unitTest()
{
	MsAnimationBundle bundle;
MsAnimation<int> ia(0, 0, 0, 0, 0);


/*
	float from1 = 0, from2 = 1;
	int times1 = 5, times2 = 10;
	float to1 = 2.5, to2 = 7;

	MsAnimation<float*>	*a1 = new MsAnimation<float*>(&from1, &to1, 0, times1, _lineStep2);
	MsAnimation<float*>	*a2 = new MsAnimation<float*>(&from2, &to2, 0, times2, _lineStep2);

	bundle.m_list.push_back(a1);
	bundle.m_list.push_back(a2);

	for(int i = 0; i < 15; i ++)
	{
		int j = 0;
		for(msAnimationIterator a = bundle.m_list.begin(); a != bundle.m_list.end(); j ++, a ++)
		{
			MsAnimation<float*> *animation = (MsAnimation<float*>*) *a;
			printf("value%d: %f\r\n", j, *animation->m_from);			
		}

		bundle.performStep();
	}*/
}