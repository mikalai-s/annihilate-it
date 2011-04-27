#pragma once

#include "../msGL.h"
#include "msBox.h"

class msBoxAnimation : public msBox
{
    // animations related
	msAnimationBundle *m_animations;

    msBox *m_box;

    GLboolean m_requiresExplosion;

    GLboolean m_requiresWave;

    GLboolean m_requiresWaveInit;

private:
    static void _linearFalling(msAnimationContext *c);

	static void _linearFalling2(msAnimationContext *c);

    static void _hiding1(msAnimationContext *c);

    static void setFlagCallback(msAnimationContext *c);   

    void setFlag(GLint delay, GLboolean *flag, GLboolean value);

public:
    msBoxAnimation(msBox *box);
    ~msBoxAnimation();

    msAnimationBundle *getAnimations();

    void fall(GLint delay, GLint direction);

    void hide(GLint delay);

    void wave(GLint delay);

    GLboolean getRequiresExplosion();

    GLboolean getRequiresWave();

    GLboolean getRequiresWaveInit();
};
