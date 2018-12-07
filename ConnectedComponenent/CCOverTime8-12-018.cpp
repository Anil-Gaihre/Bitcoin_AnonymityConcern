#include <iostream>
#include <list>
#include <vector>
#include <stdlib.h>
#include "graph.h"
#include <time.h>
#include <string>
#include <fstream>
#include <sstream>
#include <algorithm>
//#define N 8
using namespace std;
int Max;

vector<pair <long long int,long> > txtime;

void BFS(long *CSR,int* depth,long* pos,long src,int& maxDepth,long& count,long& verticesinCC,long timeLimit,long* time)
{
    list<long> queue;
    depth[src]=0;
    queue.push_back(src);
//    long count=0;
    while(!queue.empty())
    {
        long u=queue.front();
	count++;
	verticesinCC++;
	if (count%1000000==0)
	{
		cout<<count<<" ";
	}
    //    std::cout<<"At Vertex: "<<u<<std::endl;
        queue.pop_front();
        long start=pos[u];
        long stop=pos[u+1];
        for (long n=start;n<stop;n++)
        {
	 if(time[CSR[n]]< timeLimit) 
		{       // don't visit the transactions that has not occured yet
            if (depth[CSR[n]]==-1)
            {
                depth[CSR[n]]=depth[u]+1;                
               // pred[CSR[n]]=u;
                queue.push_back(CSR[n]);
                maxDepth=depth[u]+1;
            }
       		 }
	}
    }
    //cout<<endl<<endl;
    //~Initialising
    return;
}

int ConnectedComponents(long *CSR,int* depth,long* pos,long NVertex,vector <int>& ApproxDiameters,vector<int>& NVertices,vector<long>& OneVertex, long timeLimit,long* time)
{
	Max=0;
    int components=0;
    long count=0;
    for(long i=0;i<NVertex;i++)
    {
	//the time stamp for the address vertices are assigned as zero. We can use it to detect address vertices
	if(time[i]!=0)//don't start from an address vertex as there is no time stamp for the addresses. They will be visited and included into connected components when we start reach the transaction linked to it
{
	long verticesinCC=0;
	if (time[i]< timeLimit) // don't start from the transactions that has not occured yet
	{
        if (depth[i]==-1) 
        {
            int maxDepth=0;
            components++;
	    OneVertex.push_back(i);
            BFS(CSR,depth,pos,i,maxDepth,count,verticesinCC,timeLimit,time);
		if (maxDepth > Max) Max=maxDepth;
            ApproxDiameters.push_back(maxDepth);
	    NVertices.push_back(verticesinCC);
        }
	}
    }
}

    cout<<"Number of visited vertices: "<<count<<endl;

    return components;
}

int main(int args, char **argv)
{
    std::cout<<"Input: ./exe beg csr weight addressMap Tx-Time\n";
    if(args!=6){std::cout<<"Wrong input\n"; return -1;}

    const char *beg_file=argv[1];
    const char *csr_file=argv[2];
    const char *weight_file=argv[3];
    const char *addressMap_file=argv[4];
    const char *Tx_Time_file=argv[5];
    graph<long, long, long,char, long, long,long,char>
        *ginst = new graph
        <long, long, long,char, long, long, long,char>
        (beg_file,csr_file,weight_file,addressMap_file);


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
    cout<<"Initialising time for every vertices"<<endl;
    for (long i=0;i<ginst->vert_count;i++)
    {
        time[i]=0;
    }
    cout<<"Assiging time to the transactions"<<endl;
    for (long i=0;i<txtime.size();i++)
    {
        time[txtime[i].second]=txtime[i].first;
    }
    cout<<"Finished assigning time to the transactions"<<endl;

    long AnalysisstartTime=1231006505;//Saturday, January 3, 2009 6:15:05 PM
    long end=1528430400;//June 8 2018 1528206469;//Tuesday, June 5, 2018 1:47:49 PM GMT
     long Change=/*1232490969;*/(end-AnalysisstartTime)/50;
	 long begin=AnalysisstartTime+Change;

std::string pathstring=csr_file;
int pos=pathstring.find_last_of("/\\");
 string path=pathstring.substr(0,pos);
string pathdiaVertex=path+"//diaVertex.dat";
string pathtoCCInfo=path+"//ConnectedComponentInfo.dat";
ofstream CCInfo(pathtoCCInfo.c_str());
ofstream diaVertex(pathdiaVertex.c_str());
ofstream NumberOfCC("CCovertime.dat");
    // int* depth=new int[5];
    int* depth=new int[ginst->vert_count];

    long src=2;
    long dest=1;
    long ApproxDiameter=0;
   
    //for (long i=0;i<ginst->vert_count;i++)
    long Nvertex=ginst->vert_count;

    while(begin<=end)
{
    for (long i=0;i<Nvertex;i++)
    {
        depth[i]=-1;
    }
//    long beg_pos[]={0,1,3,4,5,6};
  //  long csr[]={1,0,2,1,4,3};
    vector<int> ApproxDiameters;//updated by a single BFS
    vector<int>	NVertices;
    vector<long> OneVertex;
    timeLimit=begin;//Friday, June 8, 2018 12:27:04 AM  end;//begin;
    clock_t start=clock();
    int components= ConnectedComponents(ginst->csr,depth,ginst->beg_pos,Nvertex,ApproxDiameters,NVertices,OneVertex,timeLimit,time);
    clock_t stop=clock();
    
//    int components= ConnectedComponents(src,ginst->csr,depth,ginst->beg_pos);

    cout<<"Number of Connected Components: "<<components<<endl;
    CCInfo<<"Number of Connected Components: "<<components<<endl;
    cout<<"Their diameters are: (Ran BFS only once. so it is approximattion.)"<<endl;
    CCInfo<<"Their diameters are: (Ran BFS only once. so it is approximattion.)"<<endl;
    int sum=0;
    int minDiameter=0;
    int maxDiameter=0;
    int secondBiggestDiameter=0;
    int* dia=new int[Max+1];
    for (int i=0;i<=Max;i++)
	{
		dia[i]=0;
	}
	
    for (int i=0;i<ApproxDiameters.size();i++)
    {
	dia[ApproxDiameters[i]]++;

	diaVertex<<ApproxDiameters[i]<<" "<<NVertices[i]<<endl;

        if (ApproxDiameters[i]<minDiameter)
        {
            minDiameter=ApproxDiameters[i];
        }
        if (ApproxDiameters[i]>maxDiameter)
        {
            maxDiameter=ApproxDiameters[i];
        }
        if (ApproxDiameters[i]<maxDiameter)
        {
            if (ApproxDiameters[i]>secondBiggestDiameter)
            {
                secondBiggestDiameter=ApproxDiameters[i];
            }
        }
        sum+=ApproxDiameters[i];
//        cout<<ApproxDiameters[i]<<" ";
    }
    cout<<"Max value"<<Max<<endl;
    for (int i=0;i<=Max;i++)
	{
		CCInfo<<i<<" "<<dia[i]<<" "<<OneVertex[i]<<endl;
	}
    delete[] dia;
    cout<<"Maximum Diameter: "<<maxDiameter<<endl;
    CCInfo<<"Maximum Diameter: "<<maxDiameter<<endl;
    cout<<"Minimum Diameter: "<<minDiameter<<endl;
    CCInfo<<"Minimum Diameter: "<<minDiameter<<endl;
    cout<<"Second Biggest Diameter: "<<secondBiggestDiameter<<endl;
    CCInfo<<"Second Biggest Diameter: "<<secondBiggestDiameter<<endl;
//    cout<<endl;
    cout<<"Time to detect Connected Components: "<<((float)(stop-start)/CLOCKS_PER_SEC)<<endl;
    time_t time=(time_t)begin;
    struct tm ts=*localtime(&time);
     char buf[80];
     strftime(buf, sizeof(buf), "%Y-%m-%d", &ts);
     printf("%s\n", buf);
     cout<<"Number of Connected Components on"<<endl;
     cout<<buf<<" "<<components<<endl;// visitedVertices<<endl;
     NumberOfCC<<buf<<" "<<components<<endl;//<<" "<<visitedVertices<<endl;
     begin=begin+Change;
     cout<<endl<<endl;
}
    CCInfo.close();
    diaVertex.close();
    NumberOfCC.close();

    delete[] depth;
    //delete[] dia;
    return 0;
}

