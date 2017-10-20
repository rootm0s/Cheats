#ifndef __UTILS_COUNTER_H__
#define __UTILS_COUNTER_H__

#ifdef _MSC_VER
#pragma once
#endif

#include "main.h"

namespace Utils
{
	class Counter
	{
	private:
		bool	m_bStarted;
		bool	m_bFinished;
		int		m_iStart;
		int		m_iStop;
		int		m_iCurrentIndex;
		int		m_iIncreaseValue;
		int		m_iDecreaseValue;

	public:
		Counter(void);
		Counter(int iStart, int iStop, int iIncreaseValue = 1, int iDecreaseValue = 1);
		bool	Finished(void);
		int		GetCurrentIndex(void);
		void	Increase(void);
		void	Decrease(void);
		void	Start(void);
		void	Stop(void);
		void	SetStart(int iValue);
		void	SetStop(int iValue);
		void	SetIncrease(int iValue);
		void	SetDecrease(int iValue);
		void	Setup(int iStart, int iStop, int iIncreaseValue = 1, int iDecreaseValue = 1);
	};
}

#endif