//
//  stringTest.cpp
//  TestCDemo
//
//  Created by willwang on 2019/3/8.
//  Copyright © 2019年 com.huifu. All rights reserved.
//

#include "stringTest.hpp"
#include <iostream>
#include <string>
#include <ctime>
#include <fstream>
#include <vector>
#include <unistd.h>
#include <cstring>
#include <sys/stat.h>
#include <pthread.h>
using namespace std;

void getContent();
void getContent1();
string getAge(string rowdata);
void writeToFile(string rowdata,string fileName);
const char * split = ",";
//定位整行位置

bool spliteFile(string filename,int piece);

//文件大小
int fileSize(string filename);

int main(int argc, const char * argv[]) {
    
    clock_t start = clock();
//    getAge("120101199706212213,李八,22,男,司机,13320800027,13320800027@123.com,2000-04-26,和平区,96613.22");
//    getContent1();
    
    spliteFile("./data_2000w.csv", 4);
//    int a = locateFile(1101);
//    cout << "\n\n aaaa= "<< a <<endl;
    
    clock_t end = clock();
    cout << "\n\ntime : " << ((double)end - start) / CLOCKS_PER_SEC << "s\n";
    
    return 0;
}

void getContent1(){
    ifstream infile;
    //    infile.open("./test.csv");
    //    infile.open("./data_2000w.csv");
    string test = "./data_2000w.csv";
    infile.open(test, std::ios::binary);
    //    infile.open("./test.csv", std::ios::binary);
    
    string *pointArr[2000];
    
//    infile.seekg(100000);
    int n=0;
//    string arr2[200][1000];
//    string arr3[200000];
    for (int i=0; i<20; i++) {
        string stringArr[10000];
        string str = "";
        string age = "";
        string rowdata;
        getline(infile, rowdata);
        str = str + rowdata + "\n";
        for (int j=1; j<10000 && getline(infile, rowdata); j++) {
            string age = getAge(rowdata);
            
            if (getAge(rowdata) == "32") {
                str = str +rowdata +"\n";
            }
            
//            cout << "\nppppp:"<<rowdata;
        }
        string fileName = "./"+age+".csv";
        writeToFile(str,fileName);
//        pointArr[i]= &stringArr[0];
       //1遍历所有数据，按年龄分文件（按年龄命名），一个年龄一个文件，同时用地区map记录地区最小注册时间<地区，注册时间>
        //按年龄顺序读取文件，分别写入地区命名的文件，
        //排序地区map，按注册时间
        //按排序后的map顺序合并地区文件
        
        
        //2,遍历所有数据，按地区分文件（按地区命名）同时用地区map记录地区最小注册时间<地区，注册时间>
        //遍历地区文件，将数据排序，？？？？，回写数据（数据过大如何排序）
        //排序地区map，按注册时间
        //按排序后的map顺序合并地区文件
        
        //3,
        //拆分文件
        //读入内存，按地区分区，按年龄排序，找出最小注册时间
        //排序地区map，按注册时间
        //按排序后的map顺序合并地区文件
        
        
    }
    
    cout << "\nnnnnn:"<<n;
//    for (int m=0; m<10; m++) {
//        cout << "\nmmmmm:"<<pointArr[m][m+1];
//    }
    
    
    infile.close();
}

void readFile(string filename){
    ifstream infile;
    infile.open("./data_2000w.csv", std::ios::binary);
    string rowdata;
    while (getline(infile, rowdata)) {
        cout<<"\n"<<rowdata;
    }
    infile.close();
    
}

void writeToFile(string rowdata,string fileName){
    
    ofstream outfile;
    outfile.open(fileName,ios::app);
    outfile << rowdata <<"\n"<<endl;
    outfile.close();
}

string getAge(string rowdata){
    char temp[rowdata.length()] ;
    strcpy(temp, rowdata.c_str());
    char * p = strtok (temp, split);
    int n=0;
    while (p!=NULL) {
        if (n==2) {
            
            return p;
        }else{
            p = strtok (NULL, split);
            n++;
        }
        
    }
    string age = "";
    return age;
}




void getContent(){
    cout << "\ncontent : ";
    ifstream infile;
    //    infile.open("./test.csv");
    //    infile.open("./data_2000w.csv");
    infile.open("./data_2000w.csv", std::ios::binary);
    //    infile.open("./test.csv", std::ios::binary);
    
    vector<char> buf(infile.seekg(0, std::ios::end).tellg());
    infile.seekg(0, std::ios::beg).read(&buf[0], static_cast<std::streamsize>(buf.size()));
    
    infile.close();
    
    vector<char>::iterator it;
    string *pointArr[2000];
    it = buf.begin();
    int m=0;
    for (int i=0; i<2000; i++) {
        string stringArr[10000];
        
        for (int j=0; j<10000; j++) {
            
            string rowdata="";
            for (; ('\n' != *it) && it!=buf.end(); it++) {
                rowdata = rowdata + *it;
                
            }
            it++;
//            cout << "\nrowdata:"<< rowdata;
            stringArr[j]=rowdata;
            m++;
            
        }
        
        pointArr[i]= &stringArr[0];
        
    }
    cout << "/nmmmm"<< m;
    
    
}


int locateFile(int local, ifstream &input);
bool spliteFile(string filename,int piece);
const int kBlockSize = 1024*1024*50;
void copy_file(ifstream &input, ofstream &output, size_t input_size);


/**
 切分文件

 @param filepath 文件路径
 @param piece 份数
 @return 返回文件拆分是否成功
 */
bool spliteFile(string filepath,int piece){
    
    if (filepath.length()==0 ) {
        return false;
    }
    
    int *p = new int[3];
    ifstream infile;
    infile.open(filepath,ios::in);
    
    infile.seekg(0,ios::end);
    int fileSize = (int)infile.tellg();
    int pieceSize = fileSize/piece;
    cout<<"\nsize..."<<fileSize;
    int startLocal = 0;
    for (int i=0; i< piece && startLocal<fileSize ; i++) {
        //读取的结尾位置
        int end = startLocal + pieceSize;
        
        if (end > fileSize) {
            end = fileSize;
        }else{
            end = locateFile(end, infile);
        }
        
        int readsize = end - startLocal;
        
        ofstream outfile;
        string cfile = "./children"+ to_string(i) + ".csv";
        outfile.open(cfile, ios::out | ios::trunc);
        
        infile.seekg(startLocal);
        copy_file(infile, outfile, readsize);
        
        outfile.close();
        startLocal = end+1;
    }
    
    infile.close();
    
    return true;
}


/**
 定位文件内，正确换行位置

 @param local 开始定位位置
 @param input 输入文件流
 @return 本行结束的位置
 */
int locateFile(int local, ifstream &input){
    int reallocal = local;
    input.seekg(0);
    input.seekg(reallocal);
    string row="";
    getline(input,row);
    reallocal += row.length();
    return reallocal;
}

/**
 从input流中读取size(默认大小kBlockSize)大小的字节到data里面

 @param data 读取数据
 @param input 输入文件流
 @param size 读取大小，默认为kBlockSize
 */
inline void read_file_in_block(char* data, ifstream &input, int size=kBlockSize) {
    input.read(data, size);
}

/**
 从data中将size(默认大小kBlockSize)大小的字节写入到output流

 @param data 写入
 @param output 输出文件流
 @param size 写入大小，默认为kBlockSize
 */
inline void write_file_in_block(char* data, ofstream &output, int size=kBlockSize) {
    output.write(data, size);
}


/**
 按块copy文件内容

 @param input 输入文件流
 @param output 输出文件流
 @param input_size copy块大小
 */
void copy_file(ifstream &input, ofstream &output, size_t input_size) {
    char* data = new char[kBlockSize];
    
    for (size_t block = 0; block < input_size / kBlockSize; block++) {
        read_file_in_block(data, input);
        write_file_in_block(data, output);
    }
    
    // 读取剩余的字节
    size_t left_size = input_size % kBlockSize;
    if (left_size != 0) {
        read_file_in_block(data, input, left_size);
        write_file_in_block(data, output, left_size);
    }
    
    delete [] data;
    data = nullptr;
}
