/*
#include "MsGrid.h"
#include "stdlib.h"
#include "stdio.h"

template <class T>
msGrid<T>::msGrid(GLuint rowCount, GLuint colCount)
{
	m_rowCount = rowCount;
	m_columnCount = colCount;
	m_items = (T*)calloc(rowCount * colCount, sizeof(T));
}

template <class T>
msGrid<T>::~msGrid()
{
	free(m_items);
}

template <class T>
GLuint msGrid<T>::getIndexByYX(GLuint y, GLuint x)
{
	return y * m_columnCount + x;
}

template <class T>
void msGrid<T>::setItem(GLuint y, GLuint x, T value)
{
	if(y < m_rowCount && x < m_columnCount)
		m_items[getIndexByYX(y, x)] = value;
}

template <class T>
T msGrid<T>::getItem(GLuint y, GLuint x)
{
	if(y >= 0 && y < m_rowCount && x >= 0 && x < m_columnCount)
		return m_items[getIndexByYX(y, x)];
	return 0;
}

template <class T>
void msGrid<T>::display()
{
	for(GLuint y = 0; y < m_rowCount; y ++)
	{
		for(GLuint x = 0; x < m_columnCount; x ++)
			printf("%d ", m_items[getIndexByYX(y, x)]);
		printf("\r\n");
	}		
}

*/