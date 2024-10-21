# include<stdio.h>
# include<iostream>
# include<unistd.h>
# include<fcntl.h>

# include"CLFileOperator.h"

using namespace std;

int main(){
    CLFileOperator file = CLFileOperator("test.txt");
    file.Open();
    file.Write("Hello World!123");
    file.Read();
    file.Lseek();
    file.Close();
    return 0;
}