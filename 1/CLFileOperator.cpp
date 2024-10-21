#include <fcntl.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>

#include "CLFileOperator.h"

#define MAX_SIZE 265
#define BUFFER_SIZE 4096

using namespace std;

CLFileOperator::CLFileOperator(const char *pstrFileName)
{
    m_pFileName = new char[strlen(pstrFileName) + 1];
    strcpy(m_pFileName, pstrFileName);

    m_ReadBuffer = new char[BUFFER_SIZE];
    m_nUsedBytesForReadBuffer = 0;
    m_WriteBuffer = new char[BUFFER_SIZE];
    m_nUsedBytesForWriteBuffer = 0;

    m_bFlagForProcessExit = false;
    return;
};

CLStatus CLFileOperator::Open()
{
    m_Fd = open(m_pFileName, O_RDWR | O_CREAT | O_APPEND, S_IRUSR | S_IWUSR);

    if(m_Fd == -1)
        return CLStatus(-1, 0);

    return CLStatus(0, 0);
}

CLStatus CLFileOperator::Close()
{
    if(m_Fd == -1)
        return CLStatus(-1, 0);

    if(m_nUsedBytesForWriteBuffer != 0)
    {
        CLStatus s = FlushWriteBuffer();
        if(!s.IsSuccess())
            return CLStatus(-1, 0);
    }

    if(m_nUsedBytesForReadBuffer != 0)
    {
        CLStatus s = FlushReadBuffer();
        if(!s.IsSuccess())
            return CLStatus(-1, 0);
    }

    int r = close(m_Fd);
    if(r == -1)
        return CLStatus(-1, errno);

    return CLStatus(0, 0);
}

CLStatus CLFileOperator::Read()
{
    if(m_Fd == -1)
        return CLStatus(-1, 0);

    if(m_nUsedBytesForReadBuffer == 0)
    {
        ssize_t r = read(m_Fd, m_ReadBuffer, BUFFER_SIZE);
        if(r == -1)
            return CLStatus(-1, errno);

        m_nUsedBytesForReadBuffer = r;
    }else{
        // 缓冲区中还有数据，将数据读到后面
        char *p = m_ReadBuffer;
        while(p < m_ReadBuffer + m_nUsedBytesForReadBuffer)
            p++;
        ssize_t r = read(m_Fd, p, BUFFER_SIZE - m_nUsedBytesForReadBuffer);
        if(r == -1)
            return CLStatus(-1, errno);
        m_nUsedBytesForReadBuffer += r;
    }

    return CLStatus(0, 0);
}

CLStatus CLFileOperator::Write(const char* pstrMsg)
{
    if(m_Fd == -1)
        return CLStatus(-1, 0);

    if(m_nUsedBytesForWriteBuffer == BUFFER_SIZE)
    {
        CLStatus s = FlushWriteBuffer();
        if(!s.IsSuccess())
            return CLStatus(-1, 0);
    }

    unsigned int nleftroom = BUFFER_SIZE - m_nUsedBytesForWriteBuffer;
    unsigned int len_strmsg = strlen(pstrMsg);

    if(len_strmsg > nleftroom)
    {
        CLStatus s = FlushWriteBuffer();
        if(!s.IsSuccess())
            return CLStatus(-1, 0);

        if(len_strmsg > BUFFER_SIZE)
        {
            ssize_t r = write(m_Fd, pstrMsg, len_strmsg);
            if(r == -1)
                return CLStatus(-1, errno);
        }else{
            memcpy(m_WriteBuffer + m_nUsedBytesForWriteBuffer, pstrMsg, len_strmsg);
            m_nUsedBytesForWriteBuffer += len_strmsg;
        }
    }else{
        memcpy(m_WriteBuffer + m_nUsedBytesForWriteBuffer, pstrMsg, len_strmsg);
        m_nUsedBytesForWriteBuffer += len_strmsg;
    }

    return CLStatus(0, 0);
}

CLStatus CLFileOperator::FlushWriteBuffer()
{
    if(m_Fd == -1)
        return CLStatus(-1, 0);

    if(m_nUsedBytesForWriteBuffer == 0)
        return CLStatus(0, 0);

    ssize_t r = write(m_Fd, m_WriteBuffer, m_nUsedBytesForWriteBuffer);
    if(r == -1)
        return CLStatus(-1, errno);

    m_nUsedBytesForWriteBuffer = 0;

    return CLStatus(0, 0);
}

CLStatus CLFileOperator::FlushReadBuffer()
{
    if(m_Fd == -1)
        return CLStatus(-1, 0);

    if(m_nUsedBytesForReadBuffer == 0)
        return CLStatus(0, 0);

    printf("%s\n", m_ReadBuffer);
    m_nUsedBytesForReadBuffer = 0;

    return CLStatus(0, 0);
}

CLStatus CLFileOperator::Lseek()
{
    if(m_Fd == -1)
        return CLStatus(-1, 0);

    off_t r = lseek(m_Fd, 0, SEEK_SET);
    if(r == -1)
        return CLStatus(-1, errno);

    return CLStatus(0, 0);
}

CLStatus CLFileOperator::Flush()
{
    if(m_Fd == -1)
        return CLStatus(-1, 0);

    if(m_nUsedBytesForWriteBuffer != 0)
    {
        CLStatus s = FlushWriteBuffer();
        if(!s.IsSuccess())
            return CLStatus(-1, 0);
    }

    if(m_nUsedBytesForReadBuffer != 0)
    {
        CLStatus s = FlushReadBuffer();
        if(!s.IsSuccess())
            return CLStatus(-1, 0);
    }

    return CLStatus(0, 0);
}

CLFileOperator::~CLFileOperator()
{
    if(m_Fd != -1)
        close(m_Fd);

    if(m_pFileName != 0)
        delete[] m_pFileName;

    if(m_ReadBuffer != 0)
        delete[] m_ReadBuffer;

    if(m_WriteBuffer != 0)
        delete[] m_WriteBuffer;
}