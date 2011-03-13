#include "MsAnimation.h"
#include "stdlib.h"
#include "stdio.h"


void _defaultStep(MsAnimation *animation)
{
	float step = (animation->to.f - animation->from.f) / animation->count;
	animation->from.f += step;
}

MsAnimation* ms_animation_create_default(float from, float to, int delayCount, int count)
{
	generic gFrom, gTo;
	gFrom.f = from;
	gTo.f = to;
	return ms_animation_create(gFrom, gTo, delayCount, count, _defaultStep);
}

MsAnimation* ms_animation_create(generic from, generic to, int delayCount, int count, void (*stepCallback)(MsAnimation*))
{
	MsAnimation *animation = (MsAnimation*) malloc(sizeof(MsAnimation));

	animation->from = from;
	animation->to = to;
    animation->delayCount = delayCount;
	animation->count = count;
	animation->performStep = (stepCallback == 0) ? _defaultStep : stepCallback;
    
    animation->rAngle = 0.0f;
    animation->rVector.x = 0.0f;
    animation->rVector.y = 0.0f;
    animation->rVector.z = 0.0f;

	return animation;
}

// NOTE:	don't forget to delete animation object after the last perform step has executed
int ms_animation_perform_step(MsAnimation *animation)
{
	if(animation == 0)
		return 0;

	if(animation->performStep == 0)
		return 0;

	if(animation->count <= 0)
		return 0;
    
    if(animation->delayCount > 0)
    {
        animation->delayCount --;
        return 1;
    }

	animation->performStep(animation);

	animation->count --;

	return (animation->count >= 0);
}


void ms_animation_free(MsAnimation **animation)
{
	free(*animation);
	*animation = 0;
}

void _lineStep(MsAnimation *anim)
{
	point *from = (point*)(anim->from.p);
	point *to = (point*)(anim->to.p);
	point step;
	step.x = (to->x - from->x) / anim->count;
	step.y = (to->y - from->y) / anim->count;

	from->x += step.x;
	from->y += step.y;
}


void ms_animation_unit_test()
{
	MsAnimation	*anim;

	point from;
	point to;
	int count = 10;

	from.x = 0;
	from.y = 0;

	to.x = 10;
	to.y = 5;

	anim = ms_animation_create(_ptog(&from), _ptog(&to), 0, count, _lineStep);

	do
	{
		printf("(%f, %f)\r\n", ((point*)anim->from.p)->x, ((point*)anim->from.p)->y);
	}
	while(ms_animation_perform_step(anim));

	ms_animation_free(&anim);
}