#pragma once

#include "../msGL.h"
#include "msBox.h"

class msBoxAnimation : public msBox
{
    // animations related
	msAnimationBundle *m_animations;

    msBox *m_box;

    GLboolean m_requiresExplosion;

private:
    static void _linearFalling(msAnimationContext *c);

	static void _linearFalling2(msAnimationContext *c);

    static void _hiding1(msAnimationContext *c);

    static void _resetRequiresAnimation(msAnimationContext *c);

public:
    msBoxAnimation(msBox *box);
    ~msBoxAnimation();

    msAnimationBundle *getAnimations();

    void fall(GLint delay, GLint direction);

    void hide(GLint delay);

    GLboolean getRequiresExplosion();

    void resetRequiresExplosion();
};
