# include<stdio.h>
# include<iostream>
# include<unistd.h>
# include<fcntl.h>

# include"CLFileOperator.h"

using namespace std;

int main(){
    CLFileOperator fileOperator = CLFileOperator();
    fileOperator.Open("test1.txt");
    fileOperator.Write("Hello World! Writing in text1\n");
    fileOperator.Open("test2.txt");
    fileOperator.Read();
    fileOperator.Close();
    fileOperator.Open("test2.txt");
    fileOperator.Write("Hello World! Writing in text2\n");
    fileOperator.Read();
    fileOperator.Close();
    
    return 0;
}