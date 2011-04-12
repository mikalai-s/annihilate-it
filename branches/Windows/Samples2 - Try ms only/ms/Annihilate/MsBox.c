#include "MsBox.h"
#include "stdlib.h"
#include "stdio.h"
#include "MsPalette.h"

MsBox* ms_box_create(float x, float y, float width, float height, int colorIndex)
{
	MsBox *boxToAnimate;
	MsBox *box = (MsBox*) malloc(sizeof(MsBox));
	box->location.x = x;
	box->location.y = y;
    box->location.z = 0;
	box->size.width = width;
	box->size.height = height;
	box->colorIndex = colorIndex;
    box->border.left = 0;
    box->border.top = 0;
    box->border.right = 0;
    box->border.bottom = 0;
	box->animations = ms_animationbundle_create();
    
    boxToAnimate = (MsBox*) malloc(sizeof(MsBox));
	boxToAnimate->location = box->location;
	boxToAnimate->size = box->size;
	boxToAnimate->colorIndex = colorIndex;
    boxToAnimate->border = box->border;
    box->boxToAnimate = boxToAnimate;
	return box;
}


void ms_box_free(MsBox **box)
{
	ms_animationbundle_free(&(*box)->animations);
    free((*box)->boxToAnimate);
	free(*box);
	*box = 0;
}

void ms_box_copy(MsBox *source, MsBox *target)
{
    target->location = source->location;
    target->size = source->size;
    target->border = source->border;
}

void ms_box_make_invisible(MsBox *box)
{    
	if(box == 0)
		return;

	box->colorIndex = MS_BOX_INVISIBLE;
}


int ms_box_is_visible(MsBox *box)
{
	return (box != 0 && box->colorIndex != MS_BOX_INVISIBLE);
}


void ms_box_unit_test()
{
	MsBox *box = ms_box_create(0,0,100,100,2);
	MsAnimation *anime;
	MsListItem *next;
	int i;

	ms_list_add_item(box->animations->list, _ptog(ms_animation_create_default(0, 2.5, 0, 5)));
	ms_list_add_item(box->animations->list, _ptog(ms_animation_create_default(1, 7, 0, 10)));

	for(i = 0; i < 100; i ++)
	{
		MsListItem *item = box->animations->list->first;
		while(item != 0)
		{
			next = item->next;

			anime = (MsAnimation*)item->value.p;
			
			if(next != 0)
				printf("%f\r\n", anime->from.f);
			else
				printf("  %f\r\n", anime->from.f);

			if(ms_animation_perform_step(anime) == 0)
			{
				ms_animation_free(&anime);
				ms_list_remove_item(box->animations->list, item);
			}

			item = next;
		}
	}

	ms_box_free(&box);
}