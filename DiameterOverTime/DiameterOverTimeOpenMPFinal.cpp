#include <iostream>
#include <list>
#include <vector>
#include <stdlib.h>
#include <time.h>
#include "graph.h"
#include <fstream>
#include <omp.h>
#include <sstream>
 #include <string>
#include <math.h>
#include <algorithm>
#include <time.h>
//#define N 8
using namespace std;

vector<pair <long long int,long> > txtime;


void BFS(long *CSR,long* depth,long* pos,/*long* pred,*/long src, long dest,long& maxDepth,long& farvertex,long& NvisitedVertices,long timeLimit,long* time)
{
    list<long> queue;
    depth[src]=0;
    queue.push_back(src);
    long count=0;
    while(!queue.empty())
    {
        long u=queue.front();
        //        std::cout<<"At Vertex: "<<u<<std::endl;
        queue.pop_front();
        long start=pos[u];
        long stop=pos[u+1];
        count++;
        //	if(count%1000000==0)
        //	{
        //		cout<<count<<" ";
        //	}
        //farvertex=u;
        for (long n=start;n<stop;n++)
        {
            //	cout<<"stop: "<<stop
            if (time[CSR[n]]< timeLimit)	// don't visit the transactions that has not occured yet
            {
            if (depth[CSR[n]]==-1)
            {
                depth[CSR[n]]=depth[u]+1;                
                //				pred[CSR[n]]=u;
                queue.push_back(CSR[n]);
                farvertex=CSR[n];
                maxDepth=depth[u]+1;
            }
            }
        }
    }
    NvisitedVertices=count;
    //	cout<<"Total Number of visited Vertices: "<<count<<endl;
    //~Initialising
    return;
}

long CalculateLongestPath(long src,long& dest,long *CSR,long* depth,long* pos,/*long* pred,*/vector <long>& Longestpath,long& NvisitedVertices, long timeLimit,long* time)
{
    long maxDepth=0;
    long farvertex=src;
    BFS(CSR,depth,pos,/*pred,*/src,dest,maxDepth,farvertex,NvisitedVertices,timeLimit,time);
    //get the path from the source to one of the farthest vertex
    long crawl=farvertex;
    dest=farvertex;

    // Longestpath.push_back(farvertex);//pushing the farthest vertex
    // while(pred[crawl]!=-1)
    // {
    //    Longestpath.push_back(pred[crawl]);
    //     crawl=pred[crawl];
    // }
    //    Longestpath.push_back(farvertex);//pushing the farthest vertex
    //~get the path from the source to one of the farthest vertex
    return maxDepth;

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

    //~assigning time to every transactions
    long AnalysisstartTime=1231006505;//Saturday, January 3, 2009 6:15:05 PM
    //     long end=1525484846;//Saturday, May 5, 2018 1:47:26 AM
    long end=1528206469;//Fri, 5 June 2018 13:47:49 GMT
    long Change=/*1232490969;*/(end-AnalysisstartTime)/50;
    long begin=AnalysisstartTime+Change;//After 2017 11-10 i.e. from Jan 18 2018 1492535706+Change;//After 2017 4 18 1308196800;//2011-06-16   /*1232490969;//1310688644;*///AnalysisstartTime+Change;//Friday, July 15, 2011 12:10:44 AM
     ofstream Diameter("Diameter50Div200times.dat");
	//ofstream Diameter("Diameter100Div.dat");
    while (begin<end)
    {
    double start= omp_get_wtime();
    timeLimit=begin;

    int Nsource=200;
    int M=100;
    int N=Nsource/M;
    int R=Nsource%M;
    long* srcarray=new long[Nsource];
    long visitedVertices=0;
    long ApproxDiameter=0;
    vector <long> LongestPaths;
    vector<long> srcID;
    vector<long> MaximumLength;
    vector<long> TempTransaction;
    clock_t s=clock();
    for (long i=0;i<txtime.size();i++)//pushing only the transactions that happened within a specific time
    {
        if (txtime[i].first<begin)
        {
            TempTransaction.push_back(txtime[i].second);
        }
        else break;
    }
    clock_t st=clock();
    cout<<"Time for finding the random numbers"<<(st-s)/(float)CLOCKS_PER_SEC<<endl;
    for (int j=0;j<Nsource;j++)
    {
        int size=TempTransaction.size();
        srcarray[j]=TempTransaction[rand()%(size-10)];//assigning starting source as transactions within specific period of time 
    }

#pragma omp parallel num_threads(M)
    {
        long* depth=new long[ginst->vert_count];
        //		long* Pred=new long[ginst->vert_count];

        long src=2;
        long dest=1;

        int beginning;
        int end;
        int threadID=omp_get_thread_num();
        beginning=threadID*N;
        if(threadID==M-1)
        {
            end=(threadID+1)*N+R;
        }
        else
        {
            end=(threadID+1)*N;
        }

        clock_t start=clock();

        while (beginning<end)
        {
            for (long i=0;i<ginst->vert_count;i++)
            {
                depth[i]=-1;
                //				Pred[i]=-1;
            }
            //     src= rand()%700000000+1;
          //  #pragma omp critical
          //  {
          //       cout<<beginning<<" ";     
          //  }
            src= srcarray[beginning];//rand()%20000+1;
            //        cout<<"Start From source vertex: "<<src<<endl;
            vector <long> LongestpathIn1BFS;
            long NvisitedVertices=0;
            long longestPathLength= CalculateLongestPath(src,dest,ginst->csr,depth,ginst->beg_pos/*,Pred*/,LongestpathIn1BFS,NvisitedVertices,timeLimit,time);
            //        cout<<"longest Path Length: "<<longestPathLength<<endl;

            #pragma omp critical
            {
    //            cout<<"Start From source vertex: "<<src<<endl;
    //            srcID.push_back(src);
    //            cout<<"longest Path Length: "<<longestPathLength<<endl;
    //            MaximumLength.push_back(longestPathLength);
    //            cout<<"Total Number of visited Vertices in this BFS: "<<NvisitedVertices<<endl;
                if (longestPathLength > ApproxDiameter)
                {
		    visitedVertices=NvisitedVertices;
                    ApproxDiameter=longestPathLength;
                }
               // LongestPaths.push_back(longestPathLength);
              //  cout<<"Maximum Diameter till BFS from:  "<<LongestPaths.size()<<" source is "<<ApproxDiameter<<endl;
            }
            clock_t stop=clock();
           // cout<<endl<<endl;
            beginning++;
    }
    delete[] depth;
    //		delete[] Pred;
    }
    delete[] srcarray;
    //cout<<"Outside OpenMP"<<endl;
    //ofstream Diameter("Diameter.dat");
    time_t time=(time_t)begin;
    struct tm ts=*localtime(&time);
    char buf[80];
    strftime(buf, sizeof(buf), "%Y-%m-%d", &ts);
    printf("%s\n", buf);
    cout<<"Diameter:"<<ApproxDiameter<<endl;

    Diameter<<buf<<" "<<ApproxDiameter<<" "<<visitedVertices<<endl;
  //  for (int i=0;i<LongestPaths.size();i++)
  //  {
  //      Diameter<<srcID[i]<<" "<<MaximumLength[i]<<" "<<LongestPaths[i]<<endl;
   // }
    begin+=Change;
    double stop= omp_get_wtime();
    cout<<"Time For finding the diameter "<<(stop-start)<<endl<<endl;
    }
//    double stop= omp_get_wtime();
    Diameter.close();
    delete[] time;
   // cout<<"Approximate Diameter"<<ApproxDiameter<<endl;

    //    BFS(CSR,depth,Pos,Pred,src,dest);


    return 0;
}

