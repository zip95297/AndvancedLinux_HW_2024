#include <fcntl.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>

#include "CLFileOperator.h"

#define MAX_SIZE 265
#define BUFFER_SIZE 4096

// 构造函数，初始化成员变量
CLFileOperator::CLFileOperator()
{
    m_ReadBuffer = new char[BUFFER_SIZE]; // 分配读取缓冲区
    m_nUsedBytesForReadBuffer = 0; // 初始化读取缓冲区已用字节数
    m_WriteBuffer = new char[BUFFER_SIZE]; // 分配写入缓冲区
    m_nUsedBytesForWriteBuffer = 0; // 初始化写入缓冲区已用字节数
    m_pFileName = nullptr; // 初始化文件名指针

    m_bFlagForProcessExit = false; // 初始化进程退出标志

    return;
}

// 打开文件
CLStatus CLFileOperator::Open(const char *pstrFileName)
{
    if(m_pFileName)
    {
        printf("Error: FileOperator is occupied. Please close the File already opened.\n");
        return CLStatus(-1, 0);
    }

    m_pFileName = new char[strlen(pstrFileName) + 1]; // 分配文件名内存
    strcpy(m_pFileName, pstrFileName); // 复制文件名

    m_Fd = open(m_pFileName, O_RDWR | O_CREAT | O_APPEND, S_IRUSR | S_IWUSR); // 打开文件

    if(m_Fd == -1)
        return CLStatus(-1, 0);

    return CLStatus(0, 0);
}

// 关闭文件
CLStatus CLFileOperator::Close()
{
    m_pFileName = nullptr; // 清空文件名指针

    if(m_Fd == -1)
        return CLStatus(-1, 0);

    if(m_nUsedBytesForWriteBuffer != 0)
    {
        CLStatus s = FlushWriteBuffer(); // 刷新写入缓冲区
        if(!s.IsSuccess())
            return CLStatus(-1, 0);
    }

    if(m_nUsedBytesForReadBuffer != 0)
    {
        CLStatus s = FlushReadBuffer(); // 刷新读取缓冲区
        if(!s.IsSuccess())
            return CLStatus(-1, 0);
    }

    int r = close(m_Fd); // 关闭文件描述符
    if(r == -1)
        return CLStatus(-1, errno);

    return CLStatus(0, 0);
}

// 读取文件
CLStatus CLFileOperator::Read()
{
    if(m_Fd == -1)
        return CLStatus(-1, 0);

    if(m_nUsedBytesForReadBuffer == 0)
    {
        ssize_t r = read(m_Fd, m_ReadBuffer, BUFFER_SIZE); // 读取文件内容到缓冲区
        if(r == -1)
            return CLStatus(-1, errno);

        m_nUsedBytesForReadBuffer = r; // 更新读取缓冲区已用字节数
    }else{
        // 缓冲区中还有数据，将数据读到后面
        char *p = m_ReadBuffer;
        while(p < m_ReadBuffer + m_nUsedBytesForReadBuffer)
            p++;
        ssize_t r = read(m_Fd, p, BUFFER_SIZE - m_nUsedBytesForReadBuffer); // 继续读取文件内容到缓冲区
        if(r == -1)
            return CLStatus(-1, errno);
        m_nUsedBytesForReadBuffer += r; // 更新读取缓冲区已用字节数
    }

    return CLStatus(0, 0);
}

// 写入文件
CLStatus CLFileOperator::Write(const char* pstrMsg)
{
    if(m_Fd == -1)
        return CLStatus(-1, 0);

    if(m_nUsedBytesForWriteBuffer == BUFFER_SIZE)
    {
        CLStatus s = FlushWriteBuffer(); // 刷新写入缓冲区
        if(!s.IsSuccess())
            return CLStatus(-1, 0);
    }

    unsigned int nleftroom = BUFFER_SIZE - m_nUsedBytesForWriteBuffer; // 计算写入缓冲区剩余空间
    unsigned int len_strmsg = strlen(pstrMsg); // 获取待写入字符串长度

    if(len_strmsg > nleftroom)
    {
        CLStatus s = FlushWriteBuffer(); // 刷新写入缓冲区
        if(!s.IsSuccess())
            return CLStatus(-1, 0);

        if(len_strmsg > BUFFER_SIZE)
        {
            ssize_t r = write(m_Fd, pstrMsg, len_strmsg); // 直接写入文件
            if(r == -1)
                return CLStatus(-1, errno);
        }else{
            memcpy(m_WriteBuffer + m_nUsedBytesForWriteBuffer, pstrMsg, len_strmsg); // 复制数据到写入缓冲区
            m_nUsedBytesForWriteBuffer += len_strmsg; // 更新写入缓冲区已用字节数
        }
    }else{
        memcpy(m_WriteBuffer + m_nUsedBytesForWriteBuffer, pstrMsg, len_strmsg); // 复制数据到写入缓冲区
        m_nUsedBytesForWriteBuffer += len_strmsg; // 更新写入缓冲区已用字节数
    }

    return CLStatus(0, 0);
}

// 刷新写入缓冲区
CLStatus CLFileOperator::FlushWriteBuffer()
{
    if(m_Fd == -1)
        return CLStatus(-1, 0);

    if(m_nUsedBytesForWriteBuffer == 0)
        return CLStatus(0, 0);

    ssize_t r = write(m_Fd, m_WriteBuffer, m_nUsedBytesForWriteBuffer); // 将缓冲区内容写入文件
    if(r == -1)
        return CLStatus(-1, errno);

    m_nUsedBytesForWriteBuffer = 0; // 重置写入缓冲区已用字节数

    return CLStatus(0, 0);
}

// 刷新读取缓冲区
CLStatus CLFileOperator::FlushReadBuffer()
{
    if(m_Fd == -1)
        return CLStatus(-1, 0);

    if(m_nUsedBytesForReadBuffer == 0)
        return CLStatus(0, 0);

    printf("%s\n", m_ReadBuffer); // 打印读取缓冲区内容
    m_nUsedBytesForReadBuffer = 0; // 重置读取缓冲区已用字节数

    return CLStatus(0, 0);
}

// 文件指针定位到文件开头
CLStatus CLFileOperator::Lseek()
{
    if(m_Fd == -1)
        return CLStatus(-1, 0);

    off_t r = lseek(m_Fd, 0, SEEK_SET); // 定位文件指针到文件开头
    if(r == -1)
        return CLStatus(-1, errno);

    return CLStatus(0, 0);
}

// 刷新缓冲区
CLStatus CLFileOperator::Flush()
{
    if(m_Fd == -1)
        return CLStatus(-1, 0);

    if(m_nUsedBytesForWriteBuffer != 0)
    {
        CLStatus s = FlushWriteBuffer(); // 刷新写入缓冲区
        if(!s.IsSuccess())
            return CLStatus(-1, 0);
    }

    if(m_nUsedBytesForReadBuffer != 0)
    {
        CLStatus s = FlushReadBuffer(); // 刷新读取缓冲区
        if(!s.IsSuccess())
            return CLStatus(-1, 0);
    }

    return CLStatus(0, 0);
}

// 析构函数，释放资源
CLFileOperator::~CLFileOperator()
{
    if(m_Fd != -1)
        close(m_Fd); // 关闭文件描述符

    if(m_pFileName != 0)
        delete[] m_pFileName; // 释放文件名内存

    if(m_ReadBuffer != 0)
        delete[] m_ReadBuffer; // 释放读取缓冲区内存

    if(m_WriteBuffer != 0)
        delete[] m_WriteBuffer; // 释放写入缓冲区内存
}

// 获取单例实例
CLFileOperator* CLFileOperator::GetInstance()
{
    if(m_pFileOperator == nullptr)
    {
        m_pFileOperator = new CLFileOperator; // 创建单例实例

        if(atexit(CLFileOperator::OnProcessExit) != 0)
        {
            if(m_pFileOperator != 0)
            {
                m_pFileOperator->m_bFlagForProcessExit = true;
                printf("In CLFileOperator::GetInstance(), atexit binding error\n");
            }
        }
    }

    return m_pFileOperator;
}

// 进程退出时调用的函数
void CLFileOperator::OnProcessExit()
{
    CLFileOperator *pFileOperator = CLFileOperator::GetInstance();
    if(pFileOperator != nullptr)
    {
        pFileOperator->Flush(); // 刷新缓冲区
        pFileOperator->m_bFlagForProcessExit = true; // 设置进程退出标志
    }
}

// 初始化静态成员变量
CLFileOperator* CLFileOperator::m_pFileOperator = nullptr;