#pragma once

#include <stdint.h>

//*****************************************************************************
//******************** ������� ������� ������ � ������� ***********************
template <typename T>
void swapValues(T& a, T& b);                                                   // ������ ������� ��������

template <typename T>
bool ascending(T a, T b);                                                      // �������� ���������� �� �����������

template <typename T>
bool descending(T a, T b);                                                     // �������� ���������� �� ��������

template <typename T>
void swapValues(T& a, T& b)
{
	T c = a;
	a = b;
	b = c;
}

template <typename T>
bool ascending(T a, T b)
{
	return a > b;
}

template <typename T>
bool descending(T a, T b)
{
	return a < b;
}
//*****************************************************************************
//*****************************************************************************

//*****************************************************************************
//******************** ������� ������� ������ � ��������� *********************
template <typename T>
void copyArray(T* array, T* copy, uint16_t length);                            // �������� ����� �������

template <typename T>
void sortArray(T* array, uint16_t length, bool (*comparisonFcn)(T, T));        // ���������� �������

template <typename T>
double averageValue(T* array, uint16_t length);                                // ���������� ������������ �������� � �������

template <typename T>
double medianValue(T* array, uint16_t length);                                 // ���������� ���������� �������� � �������

template <typename T>
double filterValue(T* array, uint16_t length, double (*referenceFcn)(T* array, uint16_t length), uint16_t maxDiffPercent);
                                                                               // ���������� ���������������� ��������
template <typename T>
void copyArray(T* array, T* copy, uint16_t length)
{
	for (uint16_t currentIndex = 0; currentIndex < length; currentIndex++)
	{
		copy[currentIndex] = array[currentIndex];
	}
}

template <typename T>
void sortArray(T* array, uint16_t length, bool (*comparisonFcn)(T, T))
{
	for (uint16_t startIndex = 0; startIndex < length; ++startIndex)
	{
		uint16_t bestIndex = startIndex;
		for (uint16_t currentIndex = startIndex + 1; currentIndex < length; ++currentIndex)
		{
			if (comparisonFcn(array[bestIndex], array[currentIndex]))
				bestIndex = currentIndex;
		}
		swapValues(array[startIndex], array[bestIndex]);
	}
}

template <typename T>
double averageValue(T* array, uint16_t length)
{
	double average = 0;
	for (uint16_t currentIndex = 0; currentIndex < length; currentIndex++)
	{
		average += array[currentIndex];
	}
	return (average / length);
}

template <typename T>
double medianValue(T* array, uint16_t length)
{
	double diffValue = 0;
	double newDiffValue = 0;
	uint16_t currentIndex = length / 2;
	uint16_t medianIndex = currentIndex;
	if (length < 3)
	{
		return averageValue(array, length);
	}
	else
	{
		T* copy = new T[length];
		copyArray(array, copy, length);
		sortArray(copy, length, ascending);
		double result;
		while (currentIndex >= 0 && currentIndex < length)
		{
			double leftSum = 0;
			double rightSum = 0;
			for (uint16_t leftIndex = 0; leftIndex < currentIndex; leftIndex++)
			{
				leftSum += copy[currentIndex] - copy[leftIndex];
			}
			for (uint16_t rightIndex = currentIndex + 1; rightIndex < length; rightIndex++)
			{
				rightSum += copy[rightIndex] - copy[currentIndex];
			}
			newDiffValue = rightSum - leftSum;
			if (newDiffValue == 0)
			{
				result = copy[currentIndex];
				delete[] copy;
				return (result);
			}
			if (newDiffValue + diffValue == 0)
			{
				result = copy[medianIndex];
				delete[] copy;
				return (result);
			}
			if (newDiffValue < 0)
			{
				if (abs(newDiffValue) <= abs(diffValue) || diffValue == 0)
				{
					medianIndex = currentIndex;
					currentIndex--;
					diffValue = newDiffValue;
				}
				else
				{
					result = copy[medianIndex];
					delete[] copy;
					return (result);
				}
			}
			else
			{
				if (newDiffValue <= abs(diffValue) || diffValue == 0)
				{
					medianIndex = currentIndex;
					currentIndex++;
					diffValue = newDiffValue;
				}
				else
				{
					result = copy[medianIndex];
					delete[] copy;
					return (result);
				}
			}
		}
		delete[] copy;
		return 0;
	}
}

template <typename T>
double filterValue(T* array, uint16_t length, double (*referenceFcn)(T* array, uint16_t length), uint16_t maxDiffPercent)
{
	double summ = 0;
	uint16_t members = 0;
	double referenceData = referenceFcn(array, length);
	for (uint16_t currentIndex = 0; currentIndex < length; currentIndex++)
	{
		double diff = abs(array[currentIndex] - referenceData);
		if (diff < referenceData * maxDiffPercent / 100)
		{
			summ += array[currentIndex];
			members++;
		}
	}
	if (members != 0)
		return (summ / members);
	else
		return referenceData;
}
//*****************************************************************************
//*****************************************************************************

//*****************************************************************************
//********************* ��������� ����� ***************************************
template <typename T>
class RingBuffer {

private:

	uint16_t m_size = 0;
	uint16_t m_index = 0;
	bool m_full = false;
	T* m_array;

public:

	RingBuffer(uint16_t size)                                                  // �����������
	{
		m_size = size;
		m_array = new T[m_size];
	}

	~RingBuffer()                                                              // ����������
	{
		delete[] m_array;
	}

	RingBuffer(const RingBuffer& source)                                       // ����������� �����������
	{
		m_size = source.m_size;
		m_index = source.m_index;
		if (source.m_array)
		{
			m_array = new T[m_size];
			for (uint16_t i = 0; i < m_size; ++i)
				m_array[i] = source.m_array[i];
		}
		else
			m_array = 0;
	}

	RingBuffer& operator=(const RingBuffer& source)                            // ���������� ��������� =
	{
		if (this == &source)
			return *this;
		m_size = source.m_size;
		m_index = source.m_index;
		delete[] m_array;
		if (source.m_array)
		{
			m_array = new T[m_size];
			for (uint16_t i = 0; i < m_size; ++i)
				m_array[i] = source.m_array[i];
		}
		else
			m_array = 0;
		return *this;
	}

	T& operator[] (const uint16_t index)                                       // ���������� ��������� []
	{
		return m_array[index];
	}

	uint16_t getIndex()                                                       // ��������� ���������� ����������� ������� � �����
	{
		if (m_array)
		{
			if (m_full)
				return (m_size - 1);
			else
				return (m_index - 1);
		}
		else
			return 0;
	}

	uint16_t getSize()                                                         // ��������� ������� ������
	{
		return m_size;
	}

	bool full()                                                                // �������� �� ������������� �����
	{
		return m_full;
	}

	void push(T data)                                                          // ���������� �������� � �����
	{
		m_array[m_index] = data;
		if (m_index < m_size - 1)
		{
			m_index++;
		}
		else
		{
			m_index = 0;
			m_full = true;
		}
	}

	void clear()                                                               // ������� ������
	{
		m_index = 0;
		m_full = false;
	}

	double getAverage()                                                        // ��������� ������������ ��������
	{
		return averageValue(m_array, getIndex() + 1);
	}

	double getMedian()                                                         // ��������� ���������� ��������
	{
		return  medianValue(m_array, getIndex() + 1);
	}

	double getFiltered(double (*referenceFcn)(T* array, uint16_t length), uint8_t maxDiffPercent)
		                                                                       // ��������� ���������������� ��������
	{
		return(filterValue(m_array, getIndex() + 1, referenceFcn, maxDiffPercent));
	}
};
//*****************************************************************************
//*****************************************************************************

//*****************************************************************************
//********************* ������� ��������� ����� *******************************
template <typename T>
class DoubleBuffer
{

private:

	RingBuffer<T>* cashBuffer;
	RingBuffer<T>* stackBuffer;

public:

	DoubleBuffer(uint16_t stack_size, uint16_t cash_size)                      // �����������
	{
		cashBuffer = new RingBuffer<T>(cash_size);
		stackBuffer = new RingBuffer<T>(stack_size);
	}

	~DoubleBuffer()                                                            // ����������
	{
		delete cashBuffer;
		delete stackBuffer;
	}

	DoubleBuffer(const DoubleBuffer& source)                                   // ����������� �����������
	{
		cashBuffer = source.cashBuffer;
		stackBuffer = source.stackBuffer;
	}

	DoubleBuffer& operator=(const DoubleBuffer& source)                        // ���������� ��������� =
	{
		if (this == &source)
			return *this;
		cashBuffer = source.cashBuffer;
		stackBuffer = source.stackBuffer;
		return *this;
	}

	T& operator[] (const uint16_t index)                                       // ���������� ��������� []
	{
		return stackBuffer[index];
	}

	uint16_t getIndex()                                                        // ��������� ���������� ����������� ������� � �����
	{
		return stackBuffer->getIndex();
	}

	uint16_t getSize()                                                         // ��������� ������� ������
	{
		return stackBuffer->m_size;
	}

	bool full()                                                                // �������� �� ������������� �����
	{
		return stackBuffer->m_full();
	}

	void push(T data)                                                          // ���������� �������� � �����
	{
		cashBuffer->push(data);
		if (cashBuffer->full())
		{
			stackBuffer->push(cashBuffer->getAverage());
			cashBuffer->clear();
		}
	}

	void clear()                                                               // ������� ������
	{
		cashBuffer->clear();
		stackBuffer->clear();
	}

	double getAverage()                                                        // ��������� ������������ ��������
	{
		return stackBuffer->getAverage();
	}

	double getMedian()                                                         // ��������� ���������� ��������
	{
		return  stackBuffer->getMedian();
	}

	double getFiltered(double (*referenceFcn)(T* array, uint16_t length), uint8_t maxDiffPercent)
		                                                                       // ��������� ���������������� ��������
	{
		return stackBuffer->getFiltered(referenceFcn, maxDiffPercent);
	}
};
//*****************************************************************************
//*****************************************************************************