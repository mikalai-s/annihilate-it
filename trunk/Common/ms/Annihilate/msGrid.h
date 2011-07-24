#pragma once

#include "../msCommon.h"

template <class T>
class msGrid
{
public:
    GLint m_rowCount;
    GLint m_columnCount;
    T* m_items;

private: 
    GLint getIndexByYX(GLint y, GLint x)
    {
        return y * m_columnCount + x;
    }

public:
    msGrid(GLint rowCount, GLint colCount)
    {
        m_rowCount = rowCount;
        m_columnCount = colCount;
        m_items = (T*)calloc(rowCount * colCount, sizeof(T));
    }

    ~msGrid()
    {
        free(m_items);
    }

    int getRowCount()
    {
        return m_rowCount;
    }

    int getColCount()
    {
        return m_columnCount;
    }

    void setItem(GLint y, GLint x, T value)
    {
        if(y < m_rowCount && x < m_columnCount)
            m_items[getIndexByYX(y, x)] = value;
    }

    T getItem(GLint y, GLint x)
    {
        if(y >= 0 && y < m_rowCount && x >= 0 && x < m_columnCount)
            return m_items[getIndexByYX(y, x)];
        return 0;
    }

    void display()
    {
        for(GLint y = 0; y < m_rowCount; y ++)
        {
            for(GLint x = 0; x < m_columnCount; x ++)
                printf("%d ", m_items[getIndexByYX(y, x)]);
            printf("\r\n");
        }        
    }
};