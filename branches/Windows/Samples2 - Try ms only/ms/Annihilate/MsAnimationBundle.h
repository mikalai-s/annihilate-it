#pragma once

#include "MsAnimation.h"
#include "MsList.h"
#include "MsCommon.h"
#include <list>

using namespace::std;

typedef std::list<MsAnimationBase*>	msAnimationList;
typedef msAnimationList::iterator	msAnimationIterator;

// represents set of animations each animation autoreleases when it's done
class MsAnimationBundle
{
	msAnimationList m_list;

	static void _lineStep2(MsAnimationBase *anim);

public:
	MsAnimationBundle();
	~MsAnimationBundle();

	void performStep();

	static void unitTest();
};