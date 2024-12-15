#ifndef CLFileOperator_H
#define CLFileOperator_H

#include "CLStatus.h"

// CLFileOperator 类用于文件操作
class CLFileOperator
{
    public:
        // 打开文件
        CLStatus Open(const char *pstrFileName);
        // 读取文件
        CLStatus Read();
        // 写入文件
        CLStatus Write(const char *pstrMsg);
        // 关闭文件
        CLStatus Close();
        // 文件指针定位
        CLStatus Lseek();

        // 刷新缓冲区
        CLStatus Flush();

        // 获取 CLFileOperator 的单例实例
        static CLFileOperator* GetInstance();
        
        // 构造函数
        CLFileOperator();
        // 析构函数
        ~CLFileOperator();

    private:
        // 进程退出时的回调函数
        static void OnProcessExit();
        // 刷新读取缓冲区
        CLStatus FlushReadBuffer();
        // 刷新写入缓冲区
        CLStatus FlushWriteBuffer();

    private:
        // 文件描述符
        int m_Fd;

    private:
        // 文件名
        char *m_pFileName;
        // 读取缓冲区
        char *m_ReadBuffer;
        // 读取缓冲区已使用字节数
        unsigned int m_nUsedBytesForReadBuffer;
        // 写入缓冲区
        char *m_WriteBuffer;
        // 写入缓冲区已使用字节数
        unsigned int m_nUsedBytesForWriteBuffer;

    private:
        // 进程退出标志
        bool m_bFlagForProcessExit;

    private:
        // CLFileOperator 的单例实例
        static CLFileOperator *m_pFileOperator;
};

#endif