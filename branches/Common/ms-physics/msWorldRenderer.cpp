#include "msWorldRenderer.h"

msWorldRenderer::msWorldRenderer(msShaderProgram *program)
{
    m_program = program;
}

msWorldRenderer::~msWorldRenderer(void)
{
}

GLfloat colors[1000][4];

void msWorldRenderer::DrawSolidPolygon(const b2Vec2* vertices, int32 vertexCount, const b2Color& color)
{

    for(int i = 0; i < vertexCount; i ++)
    {
        colors[i][0] = 1.0;
        colors[i][1] = 1.0;
        colors[i][2] = 1.0;
        colors[i][3] = 1.0;
    }

    m_program->getAttribute("position")->setPointerAndEnable( 2, GL_FLOAT, 0, 0, vertices );
    m_program->getAttribute("color")->setPointerAndEnable( 4, GL_FLOAT, 0, 0, colors );

    glDrawArrays(GL_LINE_LOOP, 0, vertexCount);

/*


    glColor4f(0.5f * color.r, 0.5f * color.g, 0.5f * color.b, 0.5f);
    glBegin(GL_TRIANGLE_FAN);
    for (int32 i = 0; i < vertexCount; ++i)
    {
        glVertex2f(vertices[i].x, vertices[i].y);
    }
    glEnd();
    glDisable(GL_BLEND);

    glColor4f(color.r, color.g, color.b, 1.0f);
    glBegin(GL_LINE_LOOP);
    for (int32 i = 0; i < vertexCount; ++i)
    {
        glVertex2f(vertices[i].x, vertices[i].y);
    }
    glEnd();*/
}


void msWorldRenderer::DrawPolygon(const b2Vec2* vertices, int32 vertexCount, const b2Color& color)
{

}

/// Draw a circle.
void msWorldRenderer::DrawCircle(const b2Vec2& center, float32 radius, const b2Color& color)
{

}

/// Draw a solid circle.
void msWorldRenderer::DrawSolidCircle(const b2Vec2& center, float32 radius, const b2Vec2& axis, const b2Color& color)
{

}

/// Draw a line segment.
void msWorldRenderer::DrawSegment(const b2Vec2& p1, const b2Vec2& p2, const b2Color& color)
{

}

/// Draw a transform. Choose your own length scale.
/// @param xf a transform.
void msWorldRenderer::DrawTransform(const b2Transform& xf)
{

}
