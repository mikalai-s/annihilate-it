#include "MsBox.h"
#include "stdlib.h"
#include "stdio.h"
#include "MsPalette.h"

MsBox::MsBox()
{

}

MsBox::MsBox(float x, float y, float width, float height, int colorIndex)
{	
	m_location.x = x;
	m_location.y = y;
    m_location.z = 0;
	m_size.width = width;
	m_size.height = height;
	m_colorIndex = colorIndex;
    m_border.left = 0;
    m_border.top = 0;
    m_border.right = 0;
    m_border.bottom = 0;
	m_animations = new MsAnimationBundle();
    
	MsBox *boxToAnimate = new MsBox();
	boxToAnimate->m_location = m_location;
	boxToAnimate->m_size = m_size;
	boxToAnimate->m_colorIndex = m_colorIndex;
    boxToAnimate->m_border = m_border;
    m_boxToAnimate = boxToAnimate;
}


MsBox::~MsBox()
{
	delete m_animations;
	delete m_boxToAnimate;
}

MsBox* MsBox::copy(MsBox *target)
{
    target->m_location = m_location;
    target->m_size = m_size;
    target->m_border = m_border;
	return target;
}

void MsBox::makeInvisible()
{
	m_colorIndex = MS_BOX_INVISIBLE;
}


GLboolean MsBox::isVisible()
{
	return (m_colorIndex != MS_BOX_INVISIBLE);
}

/*
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
}*/