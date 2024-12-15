# include<stdio.h> 
# include<iostream> 
# include<unistd.h> 
# include<fcntl.h> 

# include"CLFileOperator.h" // 自定义文件操作类的头文件

using namespace std; // 使用标准命名空间

int main(){
    // 创建CLFileOperator类的实例
    CLFileOperator fileOperator = CLFileOperator();
    
    // 打开名为"test1.txt"的文件
    fileOperator.Open("test1.txt");
    
    // 向"test1.txt"文件中写入字符串
    fileOperator.Write("Hello World! Writing in text1\n");
    
    // 打开名为"test2.txt"的文件
    fileOperator.Open("test2.txt");
    
    // 从"test2.txt"文件中读取内容
    fileOperator.Read();
    
    // 关闭当前打开的文件
    fileOperator.Close();
    
    // 再次打开名为"test2.txt"的文件
    fileOperator.Open("test2.txt");
    
    // 向"test2.txt"文件中写入字符串
    fileOperator.Write("Hello World! Writing in text2\n");
    
    // 从"test2.txt"文件中读取内容
    fileOperator.Read();
    
    // 关闭当前打开的文件
    fileOperator.Close();
    
    return 0; // 程序正常结束
}