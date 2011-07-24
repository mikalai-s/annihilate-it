#include "stdlib.h"
#include "stdio.h"
#include "time.h"

#include "msBoxGrid.h"
#include "msGrid.h"

void msBoxGrid::_borderInvertion( msAnimationContext *c )
{
    msBorderAnimationContext *context = (msBorderAnimationContext*)c;
    context->updateColor();
}

msBoxGrid::msBoxGrid(msPalette *palette, GLint *pattern, GLint numRows, GLint numCols, GLfloat gridHeight, GLfloat gridWidth) : msGrid<msBox*>(numRows, numCols)
{
    init(palette, pattern, numRows, numCols, gridHeight, gridWidth);
}

msBoxGrid::msBoxGrid(msPalette *palette, GLint numColors, GLint numRows, GLint numCols, GLfloat gridHeight, GLfloat gridWidth) : msGrid<msBox*>(numRows, numCols)
{   
    GLint *pattern = _generate_random_pattern(numRows, numCols, numColors);
    init(palette, pattern, numRows, numCols, gridHeight, gridWidth);
    free(pattern);
}



void msBoxGrid::init(msPalette *palette, GLint *pattern, GLint numRows, GLint numCols, GLfloat gridHeight, GLfloat gridWidth)
{
    m_lastKnownDirection = MS_BOX_SHIFT_DOWN;
    
    float width = gridWidth / numCols, height = gridHeight / numRows;
    float curx = 0, cury = 0;

    m_palette = palette;

    m_coordinateGrid = new msGrid<msPoint3f*>(numRows, numCols);

    m_boxVertexData = (msBoxData*)malloc(sizeof(msBoxData) * numRows * numCols);

    size.width = gridWidth;
    size.height = gridHeight;

    for(GLint y = 0; y < numRows; y ++)
    {
        curx = 0;
        for(GLint x = 0; x < numCols; x ++)
        {
            msBoxData* verticesData = &m_boxVertexData[y * numCols + x];
            verticesData->vertices[0] = msPoint3f(curx, cury, 0);
            verticesData->vertices[1] = msPoint3f(curx + width, cury, 0);
            verticesData->vertices[2] = msPoint3f(curx, cury + height, 0);
            verticesData->vertices[3] = msPoint3f(curx + width, cury + height, 0);

            verticesData->frontFace.colorIndex = pattern[y * numCols + x];
            verticesData->backFace.colorIndex = 0;

            verticesData->frontFace.colorDisturbance.a = 1.0f;
            verticesData->frontFace.colorDisturbance.r = 1.0f;
            verticesData->frontFace.colorDisturbance.g = 1.0f;
            verticesData->frontFace.colorDisturbance.b = 1.0f;
            verticesData->backFace.colorDisturbance.a = 1.0f;
            verticesData->backFace.colorDisturbance.r = 1.0f;
            verticesData->backFace.colorDisturbance.g = 1.0f;
            verticesData->backFace.colorDisturbance.b = 1.0f;

            verticesData->angle = 0.0f;
            verticesData->angleVector = msPoint3f(0.0f, 1.0f, 0.0f);

            msPoint3f* originalCoords = (msPoint3f*)malloc(sizeof(verticesData->vertices));
            memcpy(originalCoords, verticesData->vertices, sizeof(verticesData->vertices));

            m_coordinateGrid->setItem(y, x, originalCoords);

            msBox *box = new msBox(verticesData);
           /* if(palette != 0)
                box->m_border->color = *palette->getColor(0);*/
            setItem(y, x, box);

            curx += width;
        }

        cury += height;
    }

    _refreshBorders();
}


GLint* msBoxGrid::_generate_random_pattern(GLint numRows, GLint numCols, GLint numColors)
{
    int *pattern = (int*)malloc(sizeof(int) * numRows * numCols);
   
    srand((unsigned int)time(0));
    
    for(GLint y = 0; y < numRows; y ++)
        for(GLint x = 0; x < numCols; x ++)
            pattern[ y * numCols + x] = rand() % numColors + 1;
    
    return pattern;
}



msBoxGrid::~msBoxGrid()
{
    for(GLint y = 0; y < m_rowCount; y ++)
    {
        for(GLint x = 0; x < m_columnCount; x ++)
        {
            delete getItem(y, x);

            delete m_coordinateGrid->getItem(y, x);
        }
    }

    delete m_coordinateGrid;

    free(m_boxVertexData);
}



void msBoxGrid::display()
{
    for(GLint y = 0; y < m_rowCount; y ++)
    {
        for(GLint x = 0; x < m_columnCount; x ++)
        {
            msBox *box = getItem(y, x);
            if(box->isVisible())
                printf("%d:(%3.0f,%3.0f)  ", box->getColorIndex(), box->m_location->y, box->m_location->x);
            else
                printf("      -      ");
        }
        printf("\r\n");
    }
    printf("\r\n");
}

void msBoxGrid::display2()
{
    for(GLint y = 0; y < m_rowCount; y ++)
    {
        for(GLint x = 0; x < m_columnCount; x ++)
        {
            msBox *box = getItem(y, x);
            if(box->isVisible())
                printf("%d", box->getColorIndex());
            else
                printf(" ");
        }
        printf("\r\n");
    }
    printf("\r\n");
}

msAnimationBundle* msBoxGrid::getAnimations()
{
    return &m_animations;
}

void msBoxGrid::doBoxesFallingCallback(msAnimationContext *c)
{
    msKeyValueAnimationContext<msBoxGrid*, int> *context = (msKeyValueAnimationContext<msBoxGrid*, int>*)c;
    msBoxGrid *boxGrid = context->getKey();
    int direction = context->getValue();
    boxGrid->shiftPendentBoxes(direction);
}

void msBoxGrid::_animateBoxHiding(msBoxExplMap &boxesMap)
{
    int maxOffset = 0;
    int offset = 0;
    int offsetStep = 2;
    for(msBoxExplIterator i = boxesMap.begin(); i != boxesMap.end(); i ++)
    {
        msBox *box = (*i).first;
        int index = (*i).second;

        int o = offset + offsetStep * index;

        box->hide(o);

        if(o > maxOffset)
            maxOffset = o;
    }

    msKeyValueAnimationContext<msBoxGrid*, int> *c = new msKeyValueAnimationContext<msBoxGrid*, int>(this, m_lastKnownDirection);
    msAnimation *a = new msAnimation(maxOffset + 15, 1, c, doBoxesFallingCallback);
    m_animations.add(a);
}


int contains(msBoxExplMap &removedBoxes, msBox *box)
{
    msBoxExplIterator b = removedBoxes.find(box);
    if(b == removedBoxes.end())
        return -1;

    return (*b).second;
}


void msBoxGrid::_removeSimilarBoxes(GLint y, GLint x, GLint c, msBoxExplMap &removedBoxes, GLint level)
{
    // out of range
    if(x < 0 || y < 0 || x >= m_columnCount || y >= m_rowCount)
        return;
  
    msBox *box = getItem(y, x);
    if(!box->isVisible())
    {
        return;
    }
    else
    {
        // if the neighbor has different color
        if(box->getColorIndex() != c)
            return;
    }

     // check, probably the same box is trying to be exploded from another recursion call, so, we need to check its level
    // and if it's less than already stored then we use it
    int l = contains(removedBoxes, box);
    if(l != -1 && l <= level)
        return;

    if(l == -1 || level < l)
        removedBoxes[box] = level;
       
    _removeSimilarBoxes(y, x - 1, c, removedBoxes, level + 1);
    _removeSimilarBoxes(y - 1, x, c, removedBoxes, level + 1);
    _removeSimilarBoxes(y, x + 1, c, removedBoxes, level + 1);
    _removeSimilarBoxes(y + 1, x, c, removedBoxes, level + 1);
}

void msBoxGrid::removeSimilarItems(GLint y, GLint x)
{
    msBox *box = getItem(y, x);
    if(box->isVisible())
    {
       // if(_ms_boxgrid_has_similar_neighbour(y, x, box->getColorIndex()))
        {
            box->wave(MS_WAVE_DELAY);

            msBoxExplMap removedBoxes;

            // clear undo buffer
            _lastHiddenBoxes.clear();

            _removeSimilarBoxes(y, x, box->getColorIndex(), removedBoxes, 0);

            // fill undo buffer
            for(msBoxExplIterator i = removedBoxes.begin(); i != removedBoxes.end(); i ++)
            {
                _lastHiddenBoxes.push_back(msHideAction((*i).first));
            }
        
            _animateBoxHiding(removedBoxes);
        }
    }
}


void msBoxGrid::_exchangeBoxes(msGrid<msBox*> *grid, GLint y1, GLint x1, GLint y2, GLint x2)
{
    msBox *box1 = grid->getItem(y1, x1);
    msBox *box2 = grid->getItem(y2, x2);

    grid->setItem(y1, x1, box2);
    grid->setItem(y2, x2, box1);
}


void msBoxGrid::_moveBox(msMoveAction move)
{
    msBox *boxFrom = getItem(move.from.y, move.from.x);
    msBox *boxTo = getItem(move.to.y, move.to.x);
    
    setItem(move.from.y, move.from.x, boxTo);
    setItem(move.to.y, move.to.x, boxFrom);

    boxFrom->fall(0, move.direction, m_coordinateGrid->getItem(move.to.y, move.to.x));

    boxTo->getVerticesData()->copyVertices(boxFrom->getVerticesData());
}

void msBoxGrid::_moveBackBox( msMoveAction move )
{
    msBox *boxFrom = getItem(move.from.y, move.from.x);
    msBox *boxTo = getItem(move.to.y, move.to.x);

    setItem(move.from.y, move.from.x, boxTo);
    setItem(move.to.y, move.to.x, boxFrom);

    boxFrom->unfall(0, move.direction, m_coordinateGrid->getItem(move.to.y, move.to.x));

    boxTo->getVerticesData()->copyVertices(boxFrom->getVerticesData());
}

void msBoxGrid::_shiftDown(msGrid<msBox*> *grid, msMoveActionList *moves)
{
    for(GLint x = 0; x < m_columnCount; x ++)
    {
        GLint y = m_rowCount - 1;
        while(y >= -1)
        {
            if(!grid->getItem(y, x)->isVisible())
            {
                // y < 0 when there are no empty boxes
                if(y < 0)
                    break;

                GLint yy = y - 1; // note: dependent on direction
                while(yy >= -1 && !grid->getItem(yy, x)->isVisible())
                    yy --;

                // yy < -1 when there are no items to shift
                if(yy <= -1)
                    break;
                
                // shift found box
                moves->push_back(msMoveAction(x, yy, x, y, MS_BOX_SHIFT_DOWN));
                
                _exchangeBoxes(grid, yy, x, y, x);
            }
            
            y --;
        }
    }
}


void msBoxGrid::_shiftTop(msGrid<msBox*> *grid, msMoveActionList *moves)
{  
    for(GLint x = 0; x < m_columnCount; x ++)
    {
        GLint y = 0;
        while(y < m_rowCount)
        {
            if(!grid->getItem(y, x)->isVisible())
            {
                // y < 0 when there are no empty boxes
                if(y >= m_rowCount)
                    break;

                GLint yy = y + 1; // note: dependent on direction
                while(yy <= m_rowCount && !grid->getItem(yy, x)->isVisible())
                    yy ++;

                // yy < -1 when there are no items to shift
                if(yy > m_rowCount - 1)
                    break;

                // shift found box
                moves->push_back(msMoveAction(x, yy, x, y, MS_BOX_SHIFT_TOP));
                _exchangeBoxes(grid, yy, x, y, x);
            }
            
            y ++;
        }
    }
}


void msBoxGrid::_shiftRight(msGrid<msBox*> *grid, msMoveActionList *moves)
{
    for(GLint y = 0; y < m_rowCount; y ++)
    {
        GLint x = m_columnCount - 1;
        while(x >= -1)
        {
            if(!grid->getItem(y, x)->isVisible())
            {
                // y < 0 when there are no empty boxes
                if(x < 0)
                    break;

                GLint xx = x - 1; // note: dependent on direction
                while(xx >= -1 && !grid->getItem(y, xx)->isVisible())
                    xx --;

                // yy < -1 when there are no items to shift
                if(xx <= -1)
                    break;

                // shift found box
                moves->push_back(msMoveAction(xx, y, x, y, MS_BOX_SHIFT_RIGHT));
                _exchangeBoxes(grid, y, xx, y, x);
            }
            
            x --;
        }
    }
}


void msBoxGrid::_shiftLeft(msGrid<msBox*> *grid, msMoveActionList *moves)
{
    for(GLint y = 0; y < m_rowCount; y ++)
    {
        GLint x = 0;
        while(x < m_columnCount)
        {
            if(!grid->getItem(y, x)->isVisible())
            {
                // y < 0 when there are no empty boxes
                if(x >= m_columnCount)
                    break;

                GLint xx = x + 1; // note: dependent on direction
                while(xx <= m_columnCount && !grid->getItem(y, xx)->isVisible())
                    xx ++;

                // yy < -1 when there are no items to shift
                if(xx > m_columnCount - 1)
                    break;

                // shift found box
                moves->push_back(msMoveAction(xx, y, x, y, MS_BOX_SHIFT_LEFT));
                _exchangeBoxes(grid, y, xx, y, x);
            }
            
            x ++;
        }
    }
}


void msBoxGrid::shiftPendentBoxes(GLint direction)
{
    // create mirror grid that is going to be used for shifting investigation
    msGrid<msBox*> mirrorGrid(m_rowCount, m_columnCount);
    for(int y = 0; y < m_rowCount; y ++)
        for(int x = 0; x < m_columnCount; x ++)
            mirrorGrid.setItem(y, x, getItem(y, x));

    // clear undo storage
    _lastMovedBoxes.clear();

    // collect movement data
    if(direction == MS_BOX_SHIFT_LEFT)
        _shiftLeft(&mirrorGrid, &_lastMovedBoxes);
    else if(direction == MS_BOX_SHIFT_TOP)
        _shiftTop(&mirrorGrid, &_lastMovedBoxes);
    else if(direction == MS_BOX_SHIFT_RIGHT)
        _shiftRight(&mirrorGrid, &_lastMovedBoxes);
    else 
        _shiftDown(&mirrorGrid, &_lastMovedBoxes);

    // iterate through all movements and apply them
    for(msMoveActionIterator i = _lastMovedBoxes.begin(); i != _lastMovedBoxes.end(); i ++)
        _moveBox(*i);
    
    // refresh
    _refreshBorders();
}

// returns 1 if two boxes are with the same color
// todo: check - it can be private
int msBoxGrid::_checkNeighbours(GLint y1, GLint x1, GLint y2, GLint x2)
{
    msBox *box1 = getItem(y1, x1);
    msBox *box2 = getItem(y2, x2);
    
    if(box1 == 0 && box2 == 0)
        return 1;
    
    if(box1 == 0 || box2 == 0)
        return 0;
    
    return box1->getColorIndex() == box2->getColorIndex();
}


void msBoxGrid::unitTest()
{
    /*GLint pattern_vert[] = 
    {
        -1,-1,-1,-1,-1,
        -1,-1,-1,-1, 1,
        -1,-1,-1, 1,-1,
        -1,-1,-1, 1, 1,
        -1,-1, 1,-1,-1,
        -1,-1, 1,-1, 1,
        -1,-1, 1, 1,-1,
        -1,-1, 1, 1, 1,
        -1, 1,-1,-1,-1,
        -1, 1,-1,-1, 1,
        -1, 1,-1, 1,-1,
        -1, 1,-1, 1, 1,
        -1, 1, 1,-1,-1,
        -1, 1, 1,-1, 1,
        -1, 1, 1, 1,-1,
        -1, 1, 1, 1, 1,
         1,-1,-1,-1,-1,
         1,-1,-1,-1, 1,
         1,-1,-1, 1,-1,
         1,-1,-1, 1, 1,
         1,-1, 1,-1,-1,
         1,-1, 1,-1, 1,
         1,-1, 1, 1,-1,
         1,-1, 1, 1, 1,
         1, 1,-1,-1,-1,
         1, 1,-1,-1, 1,
         1, 1,-1, 1,-1,
         1, 1,-1, 1, 1,
         1, 1, 1,-1,-1,
         1, 1, 1,-1, 1,
         1, 1, 1, 1,-1,
         1, 1, 1, 1, 1        
    };*/
    GLint pattern_horiz[] = 
    {
        3,2,1, 
        1,2,3,
        1,2,3,
    };
    //msBoxGrid grid(0, pattern_vert, 32, 5, 100.0, 100.0);
    msBoxGrid grid(0, pattern_horiz, 3, 3, 100.0, 100.0);
    grid.display2();

    grid.getItem(0, 0)->m_visible = false;
    grid.display2();

    grid.shiftPendentBoxes(MS_BOX_SHIFT_TOP);
    grid.display2();

    /*
    ms_boxgrid_shift_pendent_boxes(grid, 1);
    ms_boxgrid_display(grid);

    ms_boxgrid_shift_pendent_boxes(grid, 2);
    ms_boxgrid_display(grid);

    ms_boxgrid_shift_pendent_boxes(grid, 3);
    ms_boxgrid_display(grid);

    

    //ms_boxgrid_remove_similar_items(grid, 2, 2);
    //ms_boxgrid_display(grid);
    //ms_boxgrid_remove_similar_items(grid, 2, 0);
    //ms_boxgrid_display(grid);
    //ms_boxgrid_remove_similar_items(grid, 1, 1);
    //ms_boxgrid_display(grid);
*/
}


void msBoxGrid::removeSimilarItemsAtPoint( msPoint3f screenPoint )
{
    msPoint3f point;
    point.x = screenPoint.x * this->size.width;
    point.y = screenPoint.y * this->size.height;

    for(int y = 0; y < m_rowCount; y ++)
    {
        for(int x = 0; x < m_columnCount; x ++)
        {
            msBox *box = getItem(y, x);

            if(box->getVerticesData()->isInside(&point))
            {
                removeSimilarItems(y, x);
                return;
            }
        }
    }
}

msBoxFaceData* _frontFaceResolver(msBox *box)
{
    return &box->getVerticesData()->frontFace;
}

msBoxFaceData* _backFaceResolver(msBox *box)
{
    return &box->getVerticesData()->backFace;
}

// updates links between boxes. Boxes have links to each other if they are neighbors and have the same color
void msBoxGrid::_refreshBorders()
{
    for(int y = 0; y < m_rowCount; y ++)
    {
        for(int x = 0; x < m_columnCount; x ++)
        {
            _refreshBoxFaceBorders(y, x, _frontFaceResolver);
            _refreshBoxFaceBorders(y, x, _backFaceResolver);
        }
    }
}


void msBoxGrid::_refreshBoxFaceBorders(int y, int x, msBoxFaceData* (*faceResolver)(msBox *))
{
    msBoxFaceData *face = faceResolver(getItem(y, x));

    // the following variables reflects neighbors with the same color
    bool left = _checkBoxColor(y, x - 1, face->getColorIndex(), faceResolver);
    bool leftTop = _checkBoxColor(y - 1, x - 1, face->getColorIndex(), faceResolver);
    bool top = _checkBoxColor(y - 1, x, face->getColorIndex(), faceResolver);
    bool topRight = _checkBoxColor(y - 1, x + 1, face->getColorIndex(), faceResolver);
    bool right = _checkBoxColor(y, x + 1, face->getColorIndex(), faceResolver);
    bool rightBottom = _checkBoxColor(y + 1, x + 1, face->getColorIndex(), faceResolver);
    bool bottom = _checkBoxColor(y + 1, x, face->getColorIndex(), faceResolver);
    bool bottomLeft = _checkBoxColor(y + 1, x - 1, face->getColorIndex(), faceResolver);

    face->hasBorder[0] = !left;
    face->hasBorder[1] = !top;
    face->hasBorder[2] = !right;
    face->hasBorder[3] = !bottom;

    face->hasCornerBorder[0] = left & top & !leftTop;
    face->hasCornerBorder[1] = top & right & !topRight;        
    face->hasCornerBorder[2] = right & bottom & !rightBottom;
    face->hasCornerBorder[3] = bottom & left & !bottomLeft;
}


// checks whether a box with specified coordinates has given color
int msBoxGrid::_checkBoxColor(GLint y, GLint x, GLint colorIndex, msBoxFaceData* (*faceResolver)(msBox *))
{
    if(x < 0 || x >= m_columnCount || y < 0 || y >= m_rowCount)
        return 0;

    msBox *neigbor = getItem(y, x);
    if(!neigbor->isVisible())
        return 0;

    return faceResolver(neigbor)->getColorIndex() == colorIndex;
}


void msBoxGrid::undo()
{
    // iterate through all movements and apply them
    _lastMovedBoxes.reverse();
    for(msMoveActionIterator i = _lastMovedBoxes.begin(); i != _lastMovedBoxes.end(); i ++)
    {
        msMoveAction action = *i;
        _moveBackBox(action.invert());
    }

    for(msHideActionIterator i = _lastHiddenBoxes.begin(); i != _lastHiddenBoxes.end(); i ++)
    {
        msHideAction action = *i;
        action.box->show(0);
    }

    // clear undo buffer
    _lastMovedBoxes.clear();
    _lastHiddenBoxes.clear();

    // refresh
    _refreshBorders();
}

    void _rotationStep(msAnimationContext *c)
    {
        msKeyValueAnimationContext<float*, float> *context = (msKeyValueAnimationContext<float*, float>*)c;

        float *angle = context->getKey();

        *angle += (context->getValue() - *angle) / context->getAnimation()->getCount();
    }

void msBoxGrid::show()
{
    for(int y = 0; y < m_rowCount; y ++)
    {
        for(int x = 0; x < m_columnCount; x ++)
        {
            msBox *box = getItem(y, x);

            // don't rotate if the back color is the same as front one
            if(box->getVerticesData()->frontFace.getColorIndex() == box->getVerticesData()->backFace.getColorIndex())
                continue;

            box->m_verticesData->angle = 180.0 * 3.1415926f / 180.0f;

            msKeyValueAnimationContext<float*, float> *context = new msKeyValueAnimationContext<float*, float>(&box->m_verticesData->angle, 0.0f);
            msAnimation *rotation = new msAnimation(((y + x))  * (double)3 * (double)rand() / (double)RAND_MAX, 15, context, _rotationStep);
            box->getAnimations()->add(rotation);
        }
    }
}

void msBoxGrid::hide()
{
    
}


