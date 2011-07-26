#pragma once

#include "../msCommon.h"

template <class T>
class msGrid
{
public:
    GLint rowCount;
    GLint columnCount;
    T* items;

private: 
    GLint getIndexByYX(GLint y, GLint x)
    {
        return y * this->columnCount + x;
    }

public:
    msGrid(GLint rowCount, GLint colCount)
    {
        this->rowCount = rowCount;
        this->columnCount = colCount;
        this->items = (T*)calloc(rowCount * colCount, sizeof(T));
    }

    ~msGrid()
    {
        free(this->items);
    }

    int getRowCount()
    {
        return this->rowCount;
    }

    int getColCount()
    {
        return this->columnCount;
    }

    void setItem(GLint y, GLint x, T value)
    {
        if(y < this->rowCount && x < this->columnCount)
            this->items[getIndexByYX(y, x)] = value;
    }

    T getItem(GLint y, GLint x)
    {
        if(y >= 0 && y < this->rowCount && x >= 0 && x < this->columnCount)
            return this->items[getIndexByYX(y, x)];
        return 0;
    }

    void display()
    {
        for(GLint y = 0; y < this->rowCount; y ++)
        {
            for(GLint x = 0; x < this->columnCount; x ++)
                printf("%d ", this->items[getIndexByYX(y, x)]);
            printf("\r\n");
        }        
    }
};