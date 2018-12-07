#include <iostream>
#include "graph.h"
#include <fstream>
#include <sstream>
#include <string>
#include <algorithm>
#include <vector>
#include <set>
#include <stdlib.h>
#include <stdint.h>
#include <assert.h>
#include <math.h>
#include <time.h>
#include <list>
using namespace std;
vector<pair <long long int,long> > txtime;


int main(int args, char **argv)
{
    std::cout<<"Input:UnDirectedGraphRichPoorDataset ./exe beg csr weight addressMap Tx-Time\n";
    if(args!=6){std::cout<<"Wrong input\n"; return -1;}

    const char *beg_file=argv[1];
    const char *csr_file=argv[2];
    const char *weight_file=argv[3];
    const char *addressMap_file=argv[4];
    const char *Tx_Time_file=argv[5];

    //template <file_vertex_t, file_index_t, file_weight_t
    //new_vertex_t, new_index_t, new_weight_t>
    graph<long, long, /*int*/long,char, long, long,/* char*/long,char>
        *ginst = new graph
        <long, long, /*int*/long,char, long, long, /*char*/long,char>
        (beg_file,csr_file,weight_file,addressMap_file);

    //**
    //You can implement your single threaded graph algorithm here.
    //like BFS, SSSP, PageRank and etc.


    ifstream Tx_t(Tx_Time_file);
    string line;
    string vertexstr;
    long int vertex;
    string timestr;
    long long int temptime;
    long count=1;

    while (getline(Tx_t,line))
    {
        std::stringstream linestream(line);
        getline(linestream,vertexstr,' ');
        getline(linestream,timestr,' ');
        temptime=atoll(timestr.c_str());
        vertex=atol(vertexstr.c_str());
        txtime.push_back(make_pair(temptime,vertex) );
    }
    Tx_t.close();

    cout<<"Sorting the transactions on the basis of time Started"<<endl;
    sort(txtime.begin(), txtime.end());
    cout<<"Finished sorting the transactions"<<endl;
    long timeLimit;
    long * time=new long [ginst->vert_count];
    long long int* VerticesMoney=new long long int[ginst->vert_count];
    cout<<"Initialising time for every vertices"<<endl;
    for (long i=0;i<ginst->vert_count;i++)
    {
	VerticesMoney[i]=0;
        time[i]=1533183858;// 1 Aug 2018 by default to all vertices
    }
    cout<<"Assiging time to the transactions"<<endl;
    for (long i=0;i<txtime.size();i++)
    {
               time[txtime[i].second]=0;//Assiging time to all the transactions as 0. This will differentiate transactions from addresses
    }
    cout<<"Finished assigning time to the transactions"<<endl;
    cout<<"assigning time to the addresses "<<endl;
//    long long int* VerticesMoney=new long long int[ginst->vert_count];
    for (long i=0;i<txtime.size();i++)
    {
        long transactionvertex=txtime[i].second;
        long beg = ginst->beg_pos[transactionvertex];
        long end = ginst->beg_pos[transactionvertex+1];
        for(long j = beg; j < end; j++)
        {
            long k=ginst->csr[j];// The neighbouring address
            if (time[k] > txtime[i].first) 
            {
                time[k] = txtime[i].first;
            }
	    if (ginst->addressMap[j]=='I')
	    {
  		VerticesMoney[ginst->csr[j]]-=ginst->weight[j];
      	    }
	    else
	    {
  		VerticesMoney[ginst->csr[j]]+=ginst->weight[j];  
	    }

        }
    }
    cout<<"Finished assigning time to the addresses"<<endl;

    //Checking the outdegree of the addresses  
    vector<pair <long,long> > timeAddress;
    for (long i=1;i<ginst->vert_count;i++)//the bitcoin graph vertices start from 1. 0 is added by the csr graph_generator. So we skip it
    {
        if (time[i]!=0)// if the vertex is address
        {
           
        
        long beg = ginst->beg_pos[i];
        long end = ginst->beg_pos[i+1];
        bool spent=false;
        for(long j = beg; j < end; j++)
        {
            if (ginst->addressMap[j]=='I')
            {
                spent=true;
                break;
            }
        }
        if (!spent)
        {
	   if (VerticesMoney[i]!=0)//Putting some conditions for the addresses that are transafered 0 balance in a trasnaction. Saw some of theses conditions also
	{
            timeAddress.push_back(make_pair(time[i],i) );
	}

        }
	}
    }
    cout<<"Finished finding the unspent money"<<endl;
    sort(timeAddress.begin(), timeAddress.end()); 
    cout<<"Sorting of the unspent addresses on the basis of time done"<<endl;
    ofstream timefile("StockAddressesOvertime.dat");
    ofstream AddressTimeFile("AddressTimeFile.dat");
    long AnalysisstartTime=1231006505;//Saturday, January 3, 2009 6:15:05 PM
    long end=1528206469;//Fri, 5 June 2018 13:47:49 GMT
    long Change=(end-AnalysisstartTime)/50;
    long begin=AnalysisstartTime+Change;
   long tempNaddresses=0; 
	long NaddressesCumulateUnspent=0;
    for (long i=0;i<timeAddress.size();)
    {
//        long tempNaddresses=0;
        if (timeAddress[i].first > begin)
        {
            time_t time=(time_t)begin;
            struct tm ts=*localtime(&time);
            char buf[80];
            strftime(buf, sizeof(buf), "%Y-%m-%d", &ts);
           // printf("%s\n", buf);

            timefile<<buf<<" "<<tempNaddresses<<" "<< NaddressesCumulateUnspent<<endl;
            begin+=Change;
            tempNaddresses=0;// resetting the number of addresses

        }
        else
        {
		NaddressesCumulateUnspent++;
             tempNaddresses++;
             i++;
           
            time_t time=(time_t)timeAddress[i].first;
            struct tm ts=*localtime(&time);
            char buf[80];
            strftime(buf, sizeof(buf), "%Y-%m-%d", &ts);
            AddressTimeFile<<timeAddress[i].second<<" "<<buf<<endl;
            //printf("%s\n", buf);
        }

    }
    AddressTimeFile.close();
    timefile.close();

    cout<<"Total Unspent Addresses:"<< timeAddress.size()<<endl;

    return 0;	
}
