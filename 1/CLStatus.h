#ifndef CLSTATUS_H
#define CLSTATUS_H

/*
用于保存函数的处理结果
*/
class CLStatus
{
public:
	/*
	构造函数
	lReturnCode >=0表示成功，否则失败
	lErrorCode 表示错误代码
	*/
	CLStatus(long lReturnCode, long lErrorCode);

	/*
	拷贝构造函数
	*/
	CLStatus(const CLStatus& s);

	/*
	析构函数
	*/
	virtual ~CLStatus();

public:
	/*
	判断操作是否成功
	返回值为true表示成功，false表示失败
	*/
	bool IsSuccess();

public:
	// 返回代码，引用类型
	const long& m_clReturnCode;

	// 错误代码，引用类型
	const long& m_clErrorCode;

private:
	// 返回代码，实际存储值
	long m_lReturnCode;

	// 错误代码，实际存储值
	long m_lErrorCode;
};

#endif
