#include "MsAnimationBundle.h"
#include "stdlib.h"
#include "stdio.h"

MsAnimationBundle* ms_animationbundle_create()
{
	MsAnimationBundle *bundle = (MsAnimationBundle*) malloc(sizeof(MsAnimationBundle));
	bundle->list = ms_list_create_empty();
	return bundle;
}

void ms_animationbundle_perform_step(MsAnimationBundle *bundle)
{
	MsAnimation *animation;
	MsListItem *item = bundle->list->first;
	MsListItem *itemToRemove;
	while(item != 0)
	{
		animation = (MsAnimation*)item->value.p;
		if(ms_animation_perform_step(animation) == 0)
		{
			itemToRemove = item;
			item = item->next;

			ms_animation_free(&animation);
			ms_list_remove_item(bundle->list, itemToRemove);
		}
		else
		{
			item = item->next;
		}
	}
}

void ms_animationbundle_free(MsAnimationBundle **bundle)
{
	MsAnimationBundle *b = *bundle;
	MsListItem *item;
	MsAnimation *animation;

	while(b->list->count > 0)
	{
		item = b->list->first;
		animation = (MsAnimation*)item->value.p;
		ms_animation_free(&animation);
		ms_list_remove_item(b->list, item);
	}

	ms_list_free(&b->list);

	free(b);
	*bundle = 0;
}

void _lineStep2(MsAnimation *anim)
{
	float *from = (float*)(anim->from.p);
	float *to = (float*)(anim->to.p);
	*from += (*to - *from) / anim->count;
}

void ms_animationbundle_unit_test()
{
	MsAnimationBundle *bundle = ms_animationbundle_create();

	float from1 = 0, from2 = 1;
	int times1 = 5, times2 = 10;
	float to1 = 2.5, to2 = 7;

	MsAnimation	*a1= ms_animation_create(_ptog(&from1), _ptog(&to1), 0, times1, _lineStep2),
		*a2 = ms_animation_create(_ptog(&from2), _ptog(&to2), 0, times2, _lineStep2);

	int i;

	ms_list_add_item(bundle->list, _ptog(a1));
	ms_list_add_item(bundle->list, _ptog(a2));

	for(i = 0; i < 15; i ++)
	{
		if(ms_list_contains(bundle->list, _ptog(a1)))
			printf("value1: %f\r\n", from1);

		if(ms_list_contains(bundle->list, _ptog(a2)))
			printf("value2: %f\r\n", from2);		

		ms_animationbundle_perform_step(bundle);
	}

	ms_animationbundle_free(&bundle);
}