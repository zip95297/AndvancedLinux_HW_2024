#include "CLStatus.h"

// 构造函数，初始化返回码和错误码
CLStatus::CLStatus(long lReturnCode, long lErrorCode) : m_clReturnCode(m_lReturnCode), m_clErrorCode(m_lErrorCode)
{
	// 将传入的返回码和错误码赋值给成员变量
	m_lReturnCode = lReturnCode;
	m_lErrorCode = lErrorCode;
}

// 析构函数
CLStatus::~CLStatus()
{
}

// 拷贝构造函数，初始化返回码和错误码
CLStatus::CLStatus(const CLStatus& s) : m_clReturnCode(m_lReturnCode), m_clErrorCode(m_lErrorCode)
{
	// 将传入对象的返回码和错误码赋值给成员变量
	m_lReturnCode = s.m_lReturnCode;
	m_lErrorCode = s.m_lErrorCode;
}

// 判断操作是否成功
bool CLStatus::IsSuccess()
{
	// 如果返回码大于等于0，则表示成功
	if(m_clReturnCode >= 0)
		return true;
	else
		return false;
}