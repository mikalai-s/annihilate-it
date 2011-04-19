#pragma once

#include "msAnimation.h"
#include "msAnimationContext.h"
#include "../MsCommon.h"
#include <list>

using namespace::std;

typedef std::list<msAnimation*>	msAnimationList;
typedef msAnimationList::iterator	msAnimationIterator;

// represents set of animations each animation autoreleases when it's done
class msAnimationBundle
{
public:
	msAnimationList m_list;

	static void _lineStep2(msAnimationContext *context);

public:
	msAnimationBundle();
	~msAnimationBundle();

	void performStep();

	static void unitTest();
};