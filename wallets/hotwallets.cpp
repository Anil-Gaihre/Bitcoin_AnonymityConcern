#include <iostream>
#include "graph.h"
#include <fstream>
#include <sstream>
#include <string>
#include <algorithm>
#include <vector>
#include <set>
#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include <assert.h>
#include <math.h>
#include <time.h>
#include <list>
using namespace std;

vector<pair <long long int,long> > txtime;
void BFS(long *CSR,long* depth,long* pos,long src,long long int* VerticesMoney,vector<long>& PotColdWallet)
{
	list<long> queue;
	depth[src]=0;
	queue.push_back(src);
	long count=0;
	while(!queue.empty())
	{
		long u=queue.front();
		queue.pop_front();
		long start=pos[u];
		long stop=pos[u+1];

		if (VerticesMoney[u] >= 1000000000000) // greater than 10000 BTC
		{	
			//Cold address condition
			PotColdWallet.push_back(u);

		}
		count++;
		//		if(count%1000000==0)
		//		{
		//			cout<<count<<" ";
		//		}
		for (long n=start;n<stop;n++)
		{

			if (depth[u] <2)
			{
				if (depth[CSR[n]]==-1)
				{
					if (CSR[n]==215392)
					{
						cout<<"The Transaction with 215392 is:"<<u<<endl;	
					}
					if (CSR[n]==215394)
					{
						cout<<"The Transaction with 215394 is:"<<u<<endl;	
					}
					if (CSR[n]==215395)
					{
						cout<<"The Transaction with 215395 is:"<<u<<endl;	
					}	
					if (CSR[n]==215464)
					{
						cout<<"The Transaction with 215464 is:"<<u<<endl;	
					}
					depth[CSR[n]]=depth[u]+1;                
					//	pred[CSR[n]]=u;
					queue.push_back(CSR[n]);
					//	farvertex=CSR[n];
					//	maxDepth=depth[u]+1;
				}
			}
		}
	}
	cout<<"Total Number of visited Vertices: "<<count<<endl;
	return;
}

struct AddressParameters
{
	int degree;
	int indegree;
	int outdegree;
	long long int VerticesMoney;
	long long int TotalInVertMoney;
	long ID;
};
int mycompareqsort(const void* a, const void* b)
{
	struct AddressParameters* elem1=(struct AddressParameters*) a;
	struct AddressParameters* elem2=(struct AddressParameters*) b;
	if (elem1->TotalInVertMoney < elem2->TotalInVertMoney)
	{
		return 1;
	}
	else if (elem1->TotalInVertMoney > elem2->TotalInVertMoney)
	{
		return -1;	
	}
	else
	{
		return 0;
	}

}
bool ComparebyTotalInMoney(const AddressParameters& a,const AddressParameters& b)
{
	return a.TotalInVertMoney > b.TotalInVertMoney;//Descending order of total In Money
}

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
	cout<<"Initialising time for every vertices"<<endl;

	long long int* TotalInVertMoney=new long long int [ginst->vert_count];
	long long int* VerticesMoney=new long long int[ginst->vert_count]; 
	int* degree= new int[ginst->vert_count];
	int* indegree=new int[ginst->vert_count];
	int* outdegree=new int[ginst->vert_count];
	//	vector	<AddressParameters> addresses;
	AddressParameters* addresses=new AddressParameters[ginst->vert_count];
	for (long i=0;i<ginst->vert_count;i++)
	{
		degree[i]=0;
		indegree[i]=0;
		outdegree[i]=0;
		TotalInVertMoney[i]=0;
		VerticesMoney[i]=0;
		time[i]=1533183858;// 1 Aug 2018 by default to all vertices
	}
	//	cout<<"Assiging time to the transactions"<<endl;
	//	for (long i=0;i<txtime.size();i++)
	//	{
	//		time[txtime[i].second]=0;//Assiging time to all the transactions as 0. This will differentiate transactions from addresses
	//	}
	//	cout<<"Finished assigning time to the transactions"<<endl;
	cout<<"Calculating the accumulated vertices money "<<endl;
	for (long i=0;i<txtime.size();i++)
	{
		long transactionvertex=txtime[i].second;
		long beg = ginst->beg_pos[transactionvertex];
		long end = ginst->beg_pos[transactionvertex+1];
		set<long> inputaddresses;
		//set<long> 
		for(long j = beg; j < end; j++)
		{
			degree[ginst->csr[j]]++;
			if (ginst->addressMap[j]=='I')//address is used as an input
			{
				outdegree[ginst->csr[j]]++;
				VerticesMoney[ginst->csr[j]]-=ginst->weight[j];
				inputaddresses.insert(ginst->csr[j]);
			}
			else
			{
				indegree[ginst->csr[j]]++;
				VerticesMoney[ginst->csr[j]]+=ginst->weight[j];
				const bool is_in= (inputaddresses.find(ginst->csr[j]) != inputaddresses.end());
				if (!is_in)
					TotalInVertMoney[ginst->csr[j]]+=ginst->weight[j];
			}
		}
	}
	cout<<"Fininshed calculating money Flow"<<endl;
	cout<<"Pushing the money flow info in struct"<<endl;

	for (long i=0;i<ginst->vert_count;i++)
	{
		AddressParameters temp;
		temp.degree=degree[i];
		temp.indegree=indegree[i];
		temp.outdegree=outdegree[i];
		temp.VerticesMoney=VerticesMoney[i];
		temp.TotalInVertMoney=TotalInVertMoney[i];
		temp.ID=i;
		//		addresses.push_back(temp);
		addresses[i].degree=degree[i];
		addresses[i].indegree=indegree[i];
		addresses[i].outdegree=outdegree[i];
		addresses[i].VerticesMoney=VerticesMoney[i];
		addresses[i].TotalInVertMoney=TotalInVertMoney[i];	
		addresses[i].ID=i;
	}
	cout<<"Sorting the addresses with money info"<<endl;
	//	sort(addresses.begin(),addresses.end(),ComparebyTotalInMoney);
	ofstream hotwallet("hotwalletFinal.dat");
	//	ofstream hotwalletaddresses("hotwalletaddresses.dat"<<endl;
	qsort (addresses,ginst->vert_count,sizeof(AddressParameters),mycompareqsort);
	int countwrite=0;
	//	cout<<"Number of addresses filtered:"<<addresses.size()<<endl;
	long long int threshold=1000000000;//less than 10 BTC
	long* hotwallets=new long[70];
	for (long i=0;i<ginst->vert_count/*addresses.size()*/;i++)
	{
		long long int tempVMoney=addresses[i].VerticesMoney;
		if (tempVMoney <= threshold)// less than 10 BTC
		{
			//	cout<<"Vertex Money"<<addresses[i].VerticesMoney<<endl;
			if (addresses[i].degree> 50000)
			{
				hotwallet<<addresses[i].ID<<" "<<addresses[i].TotalInVertMoney<<" "<<addresses[i].VerticesMoney<<" "<<addresses[i].indegree<<" "<<addresses[i].outdegree<<" "<<addresses[i].degree<<endl;
				//				if (countwrite < 1000) hotwalletaddresses<<addresses[i].ID<<" ";
				//		countwrite++;
				if (countwrite< 70) 
				{
					hotwallets[countwrite]=addresses[i].ID;
				}
				countwrite++;
			}
		}
		if (countwrite >80) break;
	}
	hotwallet.close();
	//	hotwalletaddresses.close();
	cout<<"Starting BFS for every vertices"<<endl;
	ofstream ColdWallets("ColdWallets2hop.dat");
	for (int i=0;i<70 ;i++)
	{
		vector<long> PotColdWallet;
		long src=hotwallets[i];
		long* depth=new long[ginst->vert_count];
		cout<<"BFS from Source:"<<src<<endl;
		//	memset(depth,-1,(ginst->vert_count)*sizeof(long));
		for (long k=0;k<ginst->vert_count;k++)
		{
			depth[k]=-1;		
		}
		BFS(ginst->csr,depth,ginst->beg_pos,src,VerticesMoney, PotColdWallet);
		ColdWallets<<"Possible ColdWallets of:"<<src<<endl;
		for (long m=0;m<PotColdWallet.size();m++)
		{
			ColdWallets<<PotColdWallet[m]<<"("<<VerticesMoney[PotColdWallet[m]]<<")"<<" "<<degree[PotColdWallet[m]]<<"   ";
		}
		cout<<endl<<endl;
	}
	ColdWallets.close();
	return 0;	
}
