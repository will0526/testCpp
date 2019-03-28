//
//  splitFile.cpp
//  TestCDemo
//
//  Created by willwang on 2019/3/25.
//  Copyright © 2019年 com.huifu. All rights reserved.
//

#include "CustomJNI.h"
#include "splitFile.hpp"
#include <fstream>
#include <iostream>
#include <string>
#include <stdio.h>

using namespace std;


int locateFile(int local, ifstream &input);
//bool spliteFile(string filename,int piece);
const int kBlockSize = 1024*1024*50;
void copy_file(ifstream &input, ofstream &output, size_t input_size);
bool spliteFile(string filepath, int piece);

JNIEXPORT jboolean JNICALL Java_CustomJNI_splitFile(JNIEnv * env, jobject obj, jstring jfilepath, jint piece){
    
//    bool isSuccess = spliteFile(&filepath, &piece);
    
    const char* str;
    str = env->GetStringUTFChars(jfilepath,JNI_FALSE);
    if (str == NULL) {
        return false;
    }
    string filePath = str;

    spliteFile(filePath, piece);
    return true;
}




/**
 切分文件
 
 @param filepath 文件路径
 @param piece 份数
 @return 返回文件拆分是否成功
 */
bool spliteFile(string filepath, int piece){
    
//    string filepath = &jfilepath;
    
    if (filepath.length()==0 ) {
        return false;
    }
    
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
