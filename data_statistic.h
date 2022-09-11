#pragma once

#include <stdint.h>

//*****************************************************************************
//******************** Функции для работы с данными ***************************
template <typename T>
void swapValues(T& a, T& b);                                                    // Функция замены значений местами

template <typename T>
bool ascending(T a, T b);                                                       // Функция сравнения большего с меньшим

template <typename T>
bool descending(T a, T b);                                                      // Функция сравнения меньшего с большим

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
//******************** Функции для работы с массивами *************************
template <typename T, typename S>
void copyArray(T* array, T* copy, S length);                                    // Копирование массива

template <typename T, typename S>
void sortArray(T* array, S length, bool (*comparisonFcn)(T, T));                // Сортировка массива

template <typename T, typename S>
double averageValue(T* array, S length);                                        // Среднее арифметическое в массиве

template <typename T, typename S>
double medianValue(T* array, S length);                                         // Медианное значение в массиве

template <typename T, typename S>
double filterValue(T* array, S length, double (*referenceFcn)(T* array, S length), uint8_t maxDiffPercent);
                                                                                // Отфильтрованное значение с отбрасыванием наиболее отклоненных
template <typename T, typename S>
void copyArray(T* array, T* copy, S length)
{
	for (S currentIndex = 0; currentIndex < length; currentIndex++)
	{
		copy[currentIndex] = array[currentIndex];
	}
}

template <typename T, typename S>
void sortArray(T* array, S length, bool (*comparisonFcn)(T, T))
{
	for (S startIndex = 0; startIndex < length; ++startIndex)
	{
		S bestIndex = startIndex;
		for (S currentIndex = startIndex + 1; currentIndex < length; ++currentIndex)
		{
			if (comparisonFcn(array[bestIndex], array[currentIndex]))
				bestIndex = currentIndex;
		}
		swapValues(array[startIndex], array[bestIndex]);
	}
}

template <typename T, typename S>
double averageValue(T* array, S length)
{
	double average = 0;
	for (S currentIndex = 0; currentIndex < length; currentIndex++)
	{
		average += array[currentIndex];
	}
	return (average / length);
}

template <typename T, typename S>
double medianValue(T* array, S length)
{
	double diffValue = 0;
	double newDiffValue = 0;
	S currentIndex = length / 2;
	S medianIndex = currentIndex;
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
			for (S leftIndex = 0; leftIndex < currentIndex; leftIndex++)
			{
				leftSum += copy[currentIndex] - copy[leftIndex];
			}
			for (S rightIndex = currentIndex + 1; rightIndex < length; rightIndex++)
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

template <typename T, typename S>
double filterValue(T* array, S length, double (*referenceFcn)(T* array, S length), uint8_t maxDiffPercent)
{
	double summ = 0;
	S members = 0;
	double referenceData = referenceFcn(array, length);
	for (S currentIndex = 0; currentIndex < length; currentIndex++)
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
//********************* Кольцевой буффер **************************************
template <typename T, typename S>
class RingBuffer {

private:

	S m_size = 0;
	S m_index = 0;
	bool m_full = false;
	T* m_array;

public:

	RingBuffer(S size)                                                          // Конструктор
	{
		m_size = size;
		m_array = new T[m_size];
	}

	~RingBuffer()                                                               // Деструктор
	{
		delete[] m_array;
	}

	RingBuffer(const RingBuffer& source)                                        // Конструктор копирования
	{
		m_size = source.m_size;
		m_index = source.m_index;
		if (source.m_array)
		{
			m_array = new T[m_size];
			for (S i = 0; i < m_size; ++i)
				m_array[i] = source.m_array[i];
		}
		else
			m_array = 0;
	}

	RingBuffer& operator=(const RingBuffer& source)                             // Перегрузка оператора =
	{
		if (this == &source)
			return *this;
		m_size = source.m_size;
		m_index = source.m_index;
		delete[] m_array;
		if (source.m_array)
		{
			m_array = new T[m_size];
			for (S i = 0; i < m_size; ++i)
				m_array[i] = source.m_array[i];
		}
		else
			m_array = 0;
		return *this;
	}

	T& operator[] (const S index)                                               // Перегрузка оператора []
	{
		return m_array[index];
	}

	S getIndex()                                                                // Получение текущего индекса
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

	S getSize()                                                                 // Получение размера буффера
	{
		return m_size;
	}

	bool full()                                                                 // Проверка на заполненность
	{
		return m_full;
	}

	void push(T data)                                                           // Добавление данных
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

	void clear()                                                                // Очистка буффера
	{
		m_index = 0;
		m_full = false;
	}

	double getAverage()                                                         // Получение среднего арифметического значения
	{
		return averageValue(m_array, getIndex() + 1);
	}

	double getMedian()                                                          // Получение медианного значения
	{
		return  medianValue(m_array, getIndex() + 1);
	}

	double getFiltered(double (*referenceFcn)(T* array, S length), uint8_t maxDiffPercent)
		                                                                        // Получение отфильтрованного значения
	{
		return(filterValue<T, S>(m_array, getIndex() + 1, referenceFcn, maxDiffPercent));
	}
};
//*****************************************************************************
//*****************************************************************************

//*****************************************************************************
//********************* Двойной кольцевой буффер ******************************
template <typename T, typename Ss, typename Sc>
class DoubleBuffer
{

private:

	RingBuffer<T, Sc>* cashBuffer;
	RingBuffer<T, Ss>* stackBuffer;

public:

	DoubleBuffer(Ss stack_size, Sc cash_size)                                   // Конструктор
	{
		cashBuffer = new RingBuffer<T, Sc>(cash_size);
		stackBuffer = new RingBuffer<T, Ss>(stack_size);
	}

	~DoubleBuffer()                                                             // Деструктор
	{
		delete cashBuffer;
		delete stackBuffer;
	}

	DoubleBuffer(const DoubleBuffer& source)                                    // Конструктор копирования
	{
		cashBuffer = source.cashBuffer;
		stackBuffer = source.stackBuffer;
	}

	DoubleBuffer& operator=(const DoubleBuffer& source)                         // Перегрузка оператора =
	{
		if (this == &source)
			return *this;
		cashBuffer = source.cashBuffer;
		stackBuffer = source.stackBuffer;
		return *this;
	}

	T& operator[] (const Ss index)                                              // Перегрузка оператора []
	{
		return stackBuffer[index];
	}

	uint16_t getIndex()                                                         // Получение текущего индекса
	{
		return stackBuffer->getIndex();
	}

	uint16_t getSize()                                                          // Получение размера
	{
		return stackBuffer->m_size;
	}

	bool full()                                                                 // Проверка на заполненность
	{
		return stackBuffer->m_full();
	}

	void push(T data)                                                           // Добавление данных
	{
		cashBuffer->push(data);
		if (cashBuffer->full())
		{
			stackBuffer->push(cashBuffer->getAverage());
			cashBuffer->clear();
		}
	}

	void clear()                                                                // Очистка
	{
		cashBuffer->clear();
		stackBuffer->clear();
	}

	double getAverage()                                                         // Получение среднего арифметического значения
	{
		return stackBuffer->getAverage();
	}

	double getMedian()                                                          // Получение медианного значения
	{
		return  stackBuffer->getMedian();
	}

	double getFiltered(double (*referenceFcn)(T* array, Ss length), uint8_t maxDiffPercent)
		                                                                        // Получение отфильтрованного значения
	{
		return stackBuffer->getFiltered(referenceFcn, maxDiffPercent);
	}
};
//*****************************************************************************
//*****************************************************************************