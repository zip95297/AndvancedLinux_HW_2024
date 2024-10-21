#ifndef CLFileOperator_H
#define CLFileOperator_H

#include "CLStatus.h"

class CLFileOperator
{
    public:
        CLStatus Open(const char *pstrFileName);
        CLStatus Read();
        CLStatus Write(const char *pstrMsg);
        CLStatus Close();
        CLStatus Lseek();

        CLStatus Flush();

        static CLFileOperator* GetInstance();
        
        CLFileOperator();
        ~CLFileOperator();

    private:
        static void OnProcessExit();
        CLStatus FlushReadBuffer();
        CLStatus FlushWriteBuffer();

    private:
        int m_Fd;

    private:
        char *m_pFileName;
        char *m_ReadBuffer;
        unsigned int m_nUsedBytesForReadBuffer;
        char *m_WriteBuffer;
        unsigned int m_nUsedBytesForWriteBuffer;

    private:
        bool m_bFlagForProcessExit;

    private:
        static CLFileOperator *m_pFileOperator;
};

#endif