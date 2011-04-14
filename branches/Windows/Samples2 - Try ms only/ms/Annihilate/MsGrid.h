#pragma once

#include "MsCommon.h"

template <class T>
class msGrid
{
public:
	GLuint m_rowCount;
	GLuint m_columnCount;
	T* m_items;

	GLuint getIndexByYX(GLuint y, GLuint x)
	{
		return y * m_columnCount + x;
	}

public:
	msGrid(GLuint rowCount, GLuint colCount)
	{
		m_rowCount = rowCount;
		m_columnCount = colCount;
		m_items = (T*)calloc(rowCount * colCount, sizeof(T));
	}

	~msGrid()
	{
		free(m_items);
	}

	void setItem(GLuint y, GLuint x, T value)
	{
		if(y < m_rowCount && x < m_columnCount)
			m_items[getIndexByYX(y, x)] = value;
	}

	T getItem(GLuint y, GLuint x)
	{
		if(y >= 0 && y < m_rowCount && x >= 0 && x < m_columnCount)
			return m_items[getIndexByYX(y, x)];
		return 0;
	}

	void display()
	{
		for(GLuint y = 0; y < m_rowCount; y ++)
		{
			for(GLuint x = 0; x < m_columnCount; x ++)
				printf("%d ", m_items[getIndexByYX(y, x)]);
			printf("\r\n");
		}		
	}
};