// shortest.cpp : Defines the entry point for the console application.
//

#include <iostream>
#include <stdio.h>
#include <string.h>
#include <string>
#include <fstream>
//openmpi library
//#include <mpi.h>
#include <omp.h>
using namespace std;
//declaration matrix, node number data, flag data
int **mat, node;
bool *flag;
int min=100;
string short_path="";
int path_find(int k,int length, string path) {
	//exit for function
	if(k>node-1) return 0;
	//get result and print possible method.
	if (k == node-1) {
		cout<<path<<"  weight: "<<length<<endl;
		if (min > length) {
			min=length;
			short_path=path;
		}
	}
	//using matrix, gettting possible path and make
	if (k < node-1) {
	//parallel processing sub
	#pragma omp parallel for
		for (int i = 0;i < node;i++) {
			if (mat[k][i] != 0) {
				//the point that i have passed checking..
				if (flag[i] == false) {
					flag[i]=true;
					char no[3];
					_itoa_s(i+1,no,10);
					string sno="";
					for (int i = 0; i < strlen(no); i++) {
						sno += no[i];
					}
					//parallel search for path in graph
					path_find(i,length+mat[k][i],path+sno+" ");
					flag[i]=false;
				}
			}
		}
	}
}
int main(int argc, char *argv[])
{
	int comm_sz;
	int my_rank;
	
	fstream fp;
	fp.open("input.txt",ios::in);
	//read matrix data from input file
	fp>>node;
	mat=(int**)malloc(sizeof(int*)*node);
	flag=(bool*)malloc(sizeof(bool)*node);
	for (int i = 0;i < node;i++) {
		mat[i]=(int*)malloc(sizeof(int)*node);
	}
	for (int i = 0;i < node;i++) {
		flag[i]=false;
		for (int j = 0;j < node;j++) {
			fp>>mat[i][j];
		}
	}
	flag[0]=true;
	path_find(0,0,"1 ");
	cout<<"----------------------------"<<endl;
	cout<<"shortest path : "<<short_path<<" :  weight "<<min<<endl;
    return 0;
}

