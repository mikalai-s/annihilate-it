// AnnihilateIt.cpp : Defines the entry point for the console application.
//


#define _CRTDBG_MAP_ALLOC
#include <stdlib.h>
#include <crtdbg.h>

#include "Common\MsAnimation.h"
#include "Common\MsAnimationBundle.h"
#include "Common\MsBox.h"
#include "Common\MsBoxGrid.h"
#include "Common\MsCommon.h"
#include "Common\MsGrid.h"
#include "Common\MsList.h"
#include "Common\MsPalette.h"



int main(int argc, char* argv[])
{
	ms_animation_unit_test();
	ms_animationbundle_unit_test();
	ms_box_unit_test();
	ms_boxgrid_unit_test();
	ms_list_unit_test();

	_CrtDumpMemoryLeaks();

	return 0;
}