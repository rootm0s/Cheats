#include "Counter.h"

using namespace Utils;

Counter::Counter(void)
{
	Setup(0, 3);
}

Counter::Counter(int iStart, int iStop, int iIncreaseValue, int iDecreaseValue)
{
	Setup(iStart, iStop, iIncreaseValue, iDecreaseValue);
}

bool Counter::Finished(void)
{
	return m_bFinished;
}

int Counter::GetCurrentIndex(void)
{
	return m_iCurrentIndex;
}

void Counter::Increase(void)
{
	if (m_iCurrentIndex < m_iStop)
		m_iCurrentIndex += m_iIncreaseValue;

	else m_bFinished = true;
}

void Counter::Decrease(void)
{
	if (m_iCurrentIndex > m_iStart)
		m_iCurrentIndex -= m_iDecreaseValue;

	else m_bFinished = true;
}

void Counter::Start(void)
{
	if (m_bFinished)
		m_bStarted;

	m_bFinished = false;
}

void Counter::Stop(void)
{
	m_bFinished = true;
	m_bStarted = false;
	m_iCurrentIndex = m_iStart;
}

void Counter::SetStart(int iValue)
{
	m_iStart = iValue;
}

void Counter::SetStop(int iValue)
{
	m_iStop = iValue;
}

void Counter::SetIncrease(int iValue)
{
	m_iIncreaseValue = iValue;
}

void Counter::SetDecrease(int iValue)
{
	m_iDecreaseValue = iValue;
}

void Counter::Setup(int iStart, int iStop, int iIncreaseValue, int iDecreaseValue)
{
	SetStart(iStart);
	SetStop(iStop);
	m_iCurrentIndex = iStart;
	SetIncrease(iIncreaseValue);
	SetDecrease(iDecreaseValue);
}