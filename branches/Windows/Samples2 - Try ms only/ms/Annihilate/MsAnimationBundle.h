#pragma once

#include "msAnimationBase.h"
#include "MsCommon.h"
#include <list>

using namespace::std;

typedef std::list<msAnimationBase*>	msAnimationList;
typedef msAnimationList::iterator	msAnimationIterator;

// represents set of animations each animation autoreleases when it's done
class msAnimationBundle
{
public:
	msAnimationList m_list;

	static void _lineStep2(msAnimationBase *anim);

public:
	msAnimationBundle();
	~msAnimationBundle();

	void performStep();

	static void unitTest();
};