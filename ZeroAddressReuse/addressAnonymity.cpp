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

typedef long index_t;
typedef long data_t;

using namespace std;

vector<pair <long long int,long> > txtime;
int cmp(const void* a, const void* b)
{
    if ((*(long long int*)a - *(long long int*)b) > 0) return 1;
    else if ((*(long long int*)a - *(long long int*)b) == 0) return 0;
    else
        return -1;
}


long long int GetWeightThrehold(long long int* VerticesMoney,long num_element,long TotalVertices,long& Nonzeroaddressvertices,double& TopRichTotalBTC,double& PoorTotalBTC,long& ZeroMoneyAddresses,long& NAddresses)
{
    long long int* sampled_top=new long long int[num_element];
    long long int weightThreshold=0;
//   cout<<"Number of vertices:"<<num_element<<endl; //Its giving wrong number
	ZeroMoneyAddresses=0;
   vector <long long int> Money;
    NAddresses=0;
   clock_t startArrange=clock();
   for(long i = 0;i < TotalVertices; i ++) 
    {   
        //if (VerticesMoney[i]!=-1)
        if (VerticesMoney[i]>0)// Only consdering the addresses that have balance greater than 0 to compute the threshold
        {
            Money.push_back(VerticesMoney[i]);
	    NAddresses++;
        }
	else if(VerticesMoney[i]==0)
	{
		ZeroMoneyAddresses++;
		 NAddresses++;
	}
    }
   clock_t stopArrange=clock();
//   cout<<"Time for arranging array"<<((float)(stopArrange-startArrange)/CLOCKS_PER_SEC)<<endl;
   clock_t startSort=clock();
//   cout<<"Sorting started"<<endl;
   sort(Money.begin(),Money.end());
 //  cout<<"Sorting done"<<endl;
   clock_t stopSort=clock();
 //  cout<<"Time for sorting"<<((float)(stopSort-startSort)/CLOCKS_PER_SEC)<<endl;
    Nonzeroaddressvertices=Money.size();  
    cout<<"Number of vertices till now with non zero balance:"<<Money.size()<<endl;
  
//    cout<<"90th percentile position of the money of non zero addresses:"<<9*Money.size()/10<<endl;
	long long int threshold=3*Money.size()/4;    
    cout<<"Third quartile position of the money of non zero addresses:"<<threshold<<endl;
    for (long i=0;i<Money.size();i++)
	{
		if (Money[i]<=Money[threshold])
		{	
			PoorTotalBTC+=Money[i]/(double)100000000;
		}
		else
		{
			TopRichTotalBTC+=Money[i]/(double)100000000;
		}
	}
  // cout<<"Threshold money For Coming Transactions:"<<Money[9*Money.size()/10]<<endl;

//   for (long i=0;i<Money.size();i++)
 //  {
 //      cout<<Money[i]<<" ";
 //  }
   return Money[threshold];

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

    //template <file_vertex_t, file_index_t, file_weight_t
    //new_vertex_t, new_index_t, new_weight_t>
    graph<long, long, /*int*/long,char, long, long,/* char*/long,char>
        *ginst = new graph
        <long, long, /*int*/long,char, long, long, /*char*/long,char>
        (beg_file,csr_file,weight_file,addressMap_file);

    //**
    //You can implement your single threaded graph algorithm here.
    //like BFS, SSSP, PageRank and etc.

    //long long int* VerticesMoney=new long long int[ginst->vert_count];
    //   long long int* VerticesMoney_temp=new long long int[ginst->vert_count];
    //vector <long long int> VerticesMoney_temp;
    //cout<<"reading vertices accumulated money"<<endl;

    ifstream Tx_t(Tx_Time_file);
    string line;
    string vertexstr;
    long int vertex;
    string timestr;
    long long int time;
    long count=1;
    // VerticesMoney[0]=0;
    // VerticesMoney_temp.push_back(0);

    while (getline(Tx_t,line))
    {
        std::stringstream linestream(line);
        getline(linestream,vertexstr,' ');
        getline(linestream,timestr,' ');
        time=atoll(timestr.c_str());
        vertex=atol(vertexstr.c_str());
        txtime.push_back(make_pair(time,vertex) );
        //        VerticesMoney[count]=atoll(time.c_str());

        //cout<<VerticesMoney_temp[count]<<endl;
    }
    Tx_t.close();
    //Sorting the Transactions on the basis of time
    cout<<"Sorting the transactions on the basis of time Started"<<endl;
    sort(txtime.begin(), txtime.end());
    cout<<"Finished sorting the transactions"<<endl;


    //    cout<<"Reading VertexMap file"<<endl;
    //    char* VertexType=new char[ginst->vert_count];
    //    ifstream VMap(vmap_file);
    //    string vertex;
    //    string type;
    //    string value;
    //    count=1;// the vertices in the VMap start from 1
    //    string line;
    //   VertexType[0]='N'; 
    //    while (getline(VMap,line))
    //    {
    //        std::stringstream linestream(line);

    // VMap >> vertex;
    //        getline(linestream,vertex,' ');
    //        getline(linestream,type,' ');
    //        getline(linestream,value,' ');
    //        VertexType[count]=type[0];
    //  VMap >> VertexType[count];
    // VMap >> value;
    //        cout<<VertexType[count]<<" "<<value<<endl;
    //        count++;
    //    }
    //    VMap.close();
    //    cout<<"Vertex Map Reading Done"<<endl;
    //    int k=0;

    long interactpoorgroup=0;
    long interactrichgroup=0;
    long long int weightThreshold=0;

    long richrichinteraction=0;
    long temprichrichinteraction=0;

    long richpoorinteraction=0;
    long temprichpoorinteraction=0;

    long Onlyrichrichinteraction=0;
    long tempOnlyrichrichinteraction=0;

    long Onlypoorpoorinteraction=0;
    long tempOnlypoorpoorinteraction=0;

    long Onlyrichtopoor=0;
    long tempOnlyrichtopoor=0;

    long Onlypoortorich=0;
    long tempOnlypoortorich=0;

    long richandpoor=0;
    long temprichandpoor=0;

    double BTCPoortoRich=0;
    double tempBTCPoortoRich=0;

    double BTCRichtoPoor=0;
    double tempBTCRichtoPoor=0;

    double BTCPoortoPoor=0;
    double tempBTCPoortoPoor=0;

    double BTCRichtoRich=0;
    double tempBTCRichtoRich=0;

	long tempRaddressTo0=0;
	long tempPaddressTo0=0;
	long tempseriouslyconcerned=0;
	

    long long int BTCTransactionCharge=0;
    long long int tempBTCTransactionCharge=0;

	long tempNRI1=0;
	long tempNPI1=0;

	long tempZeroAddressesUsed=0;//to receive money
	long tempmoderatelyconcerned=0;
	long templeastconcerned=0;
//	long tempmoderatelyconcerned=0;
//	long templeastconcerned=0;

	long tempOldAddressesUsedToreceiveMoney=0;//to receive money
	long tempNewAddressesUsedToreceiveMoney=0;//new address to receive money
    long long int* VerticesMoney=new long long int[ginst->vert_count];
	int* indegree=new int[ginst->vert_count];
    for(long i=0;i<ginst->vert_count;i++)
    {
        VerticesMoney[i]=-1;
	indegree[i]=0;
    }
 
    std::string pathstring=csr_file;
    int pos=pathstring.find_last_of("/\\");
    string path=pathstring.substr(0,pos); 
    string pathtoRichPoor=path+"//RichPoorInteraction.dat";
    string pathtoRichRich=path+"//OnlyRichInteraction.dat";
    string pathtoPoorPoor=path+"//OnlyPoorInteraction.dat";
    string pathtoInteractions=path+"//Interactions.dat";
    string pathtoInteractionsRichgetsRicher=path+"//InteractionsRichgetsRicher.dat";
    string pathtoBTCRichPoor=path+"//BTCRichPoor.dat";
    string pathtoAddressZeroNonZero=path+"//ZeroNonZero.dat";
    string pathtoTopAccumulationMoney=path+"//TopAccumulationMoney.dat";
    string pathtoZeroMoneyAddresses=path+"//ZeroMoneyaddresses.dat";
    string pathtoNonZeroMoneyAddresses=path+"//NonZeroMoneyaddresses.dat";
    string pathtoNewlyIntroducedAddresses=path+"//NewlyIntroducedaddresses.dat";
    string pathtoNewTransactionNumber=path+"//RecentNumberofTrasaction.dat";
    string pathtoOldZeroMoneyAddressUsed=path+"//OldZeroMoneyAddressUsed.dat";
    string pathtohybrid=path+"//New-hybrid-date-NRecentNewAddress-NRecentTx-NPresentZeroAddressTillNow-NPresentNonZeromoneyaddress-NRecentOldMoneyUsed_fromlasttransaction-OldAddressesUsedToreceiveMoneyfrom_lastTc.dat";
    string pathtoAnonymityconcernedaddresses=path+"//AnonymityConcerned-Date-NRichAddressto0-NPaddressesto0-Indeg_1-Indeg_2_5-Inde_g5.dat";
     string pathtoAnonymityRichPoorIndeg=path+"//AnonymityConcerned-Date-NRichAddressto0_Indegree1-NPaddressesto0_Indeg_1.dat";

 
    ofstream RichPoor(pathtoRichPoor.c_str());
    ofstream RichRich(pathtoRichRich.c_str());
    ofstream PoorPoor(pathtoPoorPoor.c_str());
    ofstream Interactions(pathtoInteractions.c_str());
    ofstream InteractionsRichgetsRicher(pathtoInteractionsRichgetsRicher.c_str());
    ofstream BTCRichPoor(pathtoBTCRichPoor.c_str());
    ofstream ZeroNonZero(pathtoAddressZeroNonZero.c_str());
    ofstream TopAccumulationMoney(pathtoTopAccumulationMoney.c_str());
    ofstream ZeroMoney(pathtoZeroMoneyAddresses.c_str());
    ofstream NonZeroMoney(pathtoNonZeroMoneyAddresses.c_str());
    ofstream NewlyIntroducedAddresses(pathtoNewlyIntroducedAddresses.c_str());
    ofstream NewlydoneTransactionNumber(pathtoNewTransactionNumber.c_str());
    ofstream OldZeroMoneyAddressUsed(pathtoOldZeroMoneyAddressUsed.c_str());
    ofstream Hybrid(pathtohybrid.c_str());
	ofstream Anonymityconcernedaddresses(pathtoAnonymityconcernedaddresses.c_str());
ofstream AnonymityconcernedRichPoorAddress(pathtoAnonymityRichPoorIndeg.c_str());

    long long int OldTransactionTime=0;
    bool DumpedOldInteractionValues=false;
     std::set<long> Address_set;//for number of unique addrreses
     double SumOfAllBitcoins=0;
     long AnalysisstartTime=1231006505;//Saturday, January 3, 2009 6:15:05 PM
//     long end=1525484846;//Saturday, May 5, 2018 1:47:26 AM
     long end=1528206469;//Fri, 5 June 2018 13:47:49 GMT
     long Change=(end-AnalysisstartTime)/50;//1296000;//15 days change //(end-AnalysisstartTime)/50;
     long begin=AnalysisstartTime+Change;
     long Naddresses=0;
     clock_t start=clock();
     long LastTransaction=0;
     long PrevAddressNumber=0;
    for (long i=0;i<txtime.size();i++)//The transactions are sorted according to the time, so Just read them from 0 position
    {
        long richcount=0;
        long poorcount=0;
        long NPI=0;
        long NRI=0;
        long NRO=0;
        long NPO=0;
        long transactionvertex=txtime[i].second;
        long Ninputs=0;
        long Noutputs=0;
        if (OldTransactionTime!=txtime[i].first)
        {
            if (OldTransactionTime!=0)
            {
                //Dumping Interaction values
                RichPoor<<txtime[i].first<<" "<<richpoorinteraction<<endl;
                RichRich<<txtime[i].first<<" "<<Onlyrichrichinteraction<<endl;
                PoorPoor<<txtime[i].first<<" "<<Onlypoorpoorinteraction<<endl;
//                weightThreshold=SumOfAllBitcoins/Address_set.size(); // Changing the Threshold(mean) to measure the richness and poorness with different blocks(or time)
            }
            OldTransactionTime=txtime[i].first;
            if(txtime[i].first > begin)
            {
                cout<<"At time:"<<txtime[i].first<<endl;
                
                time_t time=(time_t)txtime[i].first;
                struct tm ts=*localtime(&time);
                char buf[80];

//                strftime(buf, sizeof(buf), "%a %Y-%m-%d %H:%M:%S %Z", &ts);
                strftime(buf, sizeof(buf), "%Y-%m-%d", &ts);
                printf("%s\n", buf);
                
                time=(time_t)AnalysisstartTime;
                ts=*localtime(&time);
                char bufStartAnalysis[80];
//                strftime(bufStartAnalysis, sizeof(bufStartAnalysis), "%a %Y-%m-%d %H:%M:%S %Z", &ts);
                strftime(bufStartAnalysis, sizeof(bufStartAnalysis), "%Y-%m-%d", &ts);
                Interactions<<bufStartAnalysis<<"-to-"<<buf<<"-Th:"<<weightThreshold<<" ";//threshold till now
	        	InteractionsRichgetsRicher<<bufStartAnalysis<<"-to-"<<buf<<"-Th:"<<weightThreshold<<" ";//threshold till now
                BTCRichPoor<<bufStartAnalysis<<"-to-"<<buf<<"-Th:"<<weightThreshold<<"-Tr:"<<i-LastTransaction<<" ";//threshold till now
		ZeroNonZero<<bufStartAnalysis<<"-to-"<<buf<<"T-Address"<<Address_set.size()<<" ";
		long Nonzeroaddressvertices=0;
		double TopRichTotalBTC=0;
		double PoorTotalBTC=0;
		long ZeroMoneyAddresses=0;
		long NAddresses=0;
		Anonymityconcernedaddresses<<bufStartAnalysis<<"To"<<buf<<"-Th:"<<weightThreshold<<" "<<tempRaddressTo0<<" "<<tempPaddressTo0<<" "<<tempseriouslyconcerned<<" "<<tempmoderatelyconcerned<<" "<<templeastconcerned<<endl;

		 AnonymityconcernedRichPoorAddress<<bufStartAnalysis<<"To"<<buf<<"-Th:"<<weightThreshold<<" "<<tempNRI1<<" "<<tempNPI1<<endl;
		tempNPI1=0;tempNRI1=0;

                weightThreshold = GetWeightThrehold(VerticesMoney, Address_set.size(),ginst->vert_count,Nonzeroaddressvertices,TopRichTotalBTC,PoorTotalBTC,ZeroMoneyAddresses,NAddresses);



		tempRaddressTo0=0;tempPaddressTo0=0;tempseriouslyconcerned=0;tempmoderatelyconcerned=0;templeastconcerned=0;
		 OldZeroMoneyAddressUsed<<buf<<" "<<tempZeroAddressesUsed<<endl;
//		tempZeroAddressesUsed=0;
		NewlyIntroducedAddresses<<buf<<" "<<NAddresses-PrevAddressNumber<<endl;		
		NewlydoneTransactionNumber<<buf<<" "<<i-LastTransaction<<endl;

			Hybrid<<buf<<" "<<tempNewAddressesUsedToreceiveMoney<<" "<<i-LastTransaction<<" "<<ZeroMoneyAddresses<<" "<<Nonzeroaddressvertices<<" "<<tempZeroAddressesUsed<<" "<<tempOldAddressesUsedToreceiveMoney<<endl;
//	Hybrid<<buf<<" "<<NAddresses-PrevAddressNumber<<" "<<i-LastTransaction<<" "<<ZeroMoneyAddresses<<" "<<Nonzeroaddressvertices<<" "<<tempZeroAddressesUsed<<" "<<tempOldAddressesUsedToreceiveMoney<<endl;
		tempOldAddressesUsedToreceiveMoney=0;tempNewAddressesUsedToreceiveMoney=0;
		tempZeroAddressesUsed=0;

		PrevAddressNumber=NAddresses;
		ZeroMoney<<buf<<" "<<ZeroMoneyAddresses<<endl;
                NonZeroMoney<<buf<<" "<<Nonzeroaddressvertices<<endl;
		ZeroNonZero<<Address_set.size()-Nonzeroaddressvertices<<" "<<Nonzeroaddressvertices<<endl;
                TopAccumulationMoney<<bufStartAnalysis<<"-to-"<<buf<<"T-BitcoinsGenerated"<<SumOfAllBitcoins<<" "<<TopRichTotalBTC<<" "<<PoorTotalBTC<<endl;
                AnalysisstartTime=begin;
                begin+=Change;

                cout<<"Results of last: "<<i-LastTransaction<<" Transactions"<<endl;
                LastTransaction=i;
                cout<<"Rich and poor address interaction in from last time to this time= "<<temprichpoorinteraction<<endl;
		        cout<<"Only Rich to Poor: "<<tempOnlyrichtopoor<<endl;
                Interactions<<temprichpoorinteraction<<" ";
                
		InteractionsRichgetsRicher<<tempOnlyrichtopoor<<" ";
		tempOnlyrichtopoor=0;
                temprichpoorinteraction=0;// resetting to 0 for next interval calculation
                cout<<"Only Rich addresses interaction from last time to this time= "<<tempOnlyrichrichinteraction<<endl;
		 cout<<"Only Poor to Rich: "<<tempOnlypoortorich<<endl;
                Interactions<<tempOnlyrichrichinteraction<<" ";
		InteractionsRichgetsRicher<<tempOnlypoortorich<<" ";
		tempOnlypoortorich=0;
                tempOnlyrichrichinteraction=0;
                cout<<"Only poor addresses interaction from last time to this time= "<<tempOnlypoorpoorinteraction<<endl;            
		cout<<" Rich-> rich , poor -> poor ,rich and poor --> rich or poor,rich or poor --> rich and poor: "<<temprichandpoor<<endl;
                Interactions<<tempOnlypoorpoorinteraction<<endl;
		InteractionsRichgetsRicher<<temprichandpoor<<endl;
		temprichandpoor=0;
                tempOnlypoorpoorinteraction=0;
                cout<<"Threshold money For Coming Transactions:"<<weightThreshold<<endl;
                cout<<endl<<endl;
                BTCRichPoor<<tempBTCPoortoRich<<" ";
                tempBTCPoortoRich=0;
                BTCRichPoor<<tempBTCRichtoPoor<<" ";
                tempBTCRichtoPoor=0;
                BTCRichPoor<<tempBTCPoortoPoor<<" ";
                tempBTCPoortoPoor=0;
                BTCRichPoor<<tempBTCRichtoRich<<" ";
                tempBTCRichtoRich=0;
                BTCRichPoor<<(double)tempBTCTransactionCharge/100000000<<endl;
                tempBTCTransactionCharge=0;               

            }
        }

        long long int outputsum=0;
        long long int sumIp=0;
        long long int sumPoor=0;
        long long int sumOp=0;
        long long int sumRich=0;
        int Nip=0;
//        double transactionCharge=0;

        for  (long j=ginst->beg_pos[transactionvertex];j<ginst->beg_pos[transactionvertex+1];j++)//Calculating the sum of the input and the sum of the output
        {
            if (ginst->addressMap[j]=='I')
            {
                Nip++;
                sumIp+=ginst->weight[j]/*(double)100000000*/;
                if (VerticesMoney[ginst->csr[j]] >= weightThreshold)//When address is rich
                {
                    sumRich+=ginst->weight[j]/*(double)100000000*/;
                }
                else
                {
                    sumPoor+=ginst->weight[j]/*(double)100000000*/;
                }
            }
            else// For finding out the transction charge
            {
                sumOp+=ginst->weight[j]/*(double)100000000*/;
            }
        }
        
        if (Nip!=0)//Only consder the traansaction that are not mining transactions
        {
             BTCTransactionCharge+=(sumIp-sumOp);
             tempBTCTransactionCharge+=(sumIp-sumOp);
        }
//string pathtoAnonymityRichPoorIndeg=path+"//Anony
        for (long j=ginst->beg_pos[transactionvertex];j<ginst->beg_pos[transactionvertex+1];j++)
        {            
            if (VerticesMoney[ginst->csr[j]] >= weightThreshold) 
            {
                richcount++;
            }
            else 
            {
                poorcount++;
            }

            if (ginst->addressMap[j]=='I')//Condition where the address is the Input to the transaction
            {
		bool richaddress=false;//To check the shifting of money from one address to another address
                //reduce the money from the bitcoin address
                if ( VerticesMoney[ginst->csr[j]]==-1)  
                {
                    VerticesMoney[ginst->csr[j]]=0;
                    Naddresses++;
                }
		else if ( VerticesMoney[ginst->csr[j]]==0)// Condition of reusing the old address with zero balance
		{
			 tempZeroAddressesUsed++;
		}
                if (VerticesMoney[ginst->csr[j]] >= weightThreshold)
                {
                    NRI++;
		    richaddress=true;
                }
                else
                {
                    NPI++;
		    richaddress=false;
                }
		
                VerticesMoney[ginst->csr[j]]-=ginst->weight[j];
		if(VerticesMoney[ginst->csr[j]]==0)
		{
			//switchtonewaddress++;
			if (richaddress) 
			{
				tempRaddressTo0++;
			}
			else
			{
				tempPaddressTo0++;
			}
			if (indegree[ginst->csr[j]]==1)
			{
				if (richaddress) tempNRI1++;
				else tempNPI1++;

				tempseriouslyconcerned++;
			}
			else if ((indegree[ginst->csr[j]]>1)&& (indegree[ginst->csr[j]]<5))
			{
				tempmoderatelyconcerned++;
			}
			else
			{
				templeastconcerned++;
			}
		}
           Ninputs++;
	    }
            else
            {
		indegree[ginst->csr[j]]++;
                if (Ninputs!=0)
                {
                 if (VerticesMoney[ginst->csr[j]] >= weightThreshold)//when the address is rich
                 {
                    BTCPoortoRich+=(sumPoor/(double)sumIp)*(double)ginst->weight[j]/100000000;
                    tempBTCPoortoRich+=(sumPoor/(double)sumIp)*(double)ginst->weight[j]/100000000;
                    BTCRichtoRich+=(sumRich/(double)sumIp)*(double)ginst->weight[j]/100000000;
                    tempBTCRichtoRich+=(sumRich/(double)sumIp)*(double)ginst->weight[j]/100000000;
                 }
		
                 else//when the address is poor
                 {
                     BTCPoortoPoor+=(sumPoor/(double)sumIp)*(double)ginst->weight[j]/100000000;
                     tempBTCPoortoPoor+=(sumPoor/(double)sumIp)*(double)ginst->weight[j]/100000000;
                     BTCRichtoPoor+=(sumRich/(double)sumIp)*(double)ginst->weight[j]/100000000;
                     tempBTCRichtoPoor+=(sumRich/(double)sumIp)*(double)ginst->weight[j]/100000000;
                 }
                }
                
                Address_set.insert(ginst->csr[j]);//Pushing the output adddress to the set where the duplicate items are not saved. So no chance of duplicate address counting
                 if ( VerticesMoney[ginst->csr[j]]==-1) 
                 {
                     VerticesMoney[ginst->csr[j]]=0;
			tempNewAddressesUsedToreceiveMoney++;
                     Naddresses++;
                 }
		 else if ( VerticesMoney[ginst->csr[j]]==0)// Condition of reusing the old address with zero balance
		{
			tempZeroAddressesUsed++;
			tempOldAddressesUsedToreceiveMoney++;
		}	
		else if (VerticesMoney[ginst->csr[j]] >0 )
		{
			tempOldAddressesUsedToreceiveMoney++;
		}
                 if (VerticesMoney[ginst->csr[j]] >= weightThreshold)
                 {
                     NRO++;
                 }
                 else
                 {
                     NPO++;
                 }
                VerticesMoney[ginst->csr[j]]+=ginst->weight[j];
                outputsum+=ginst->weight[j];
                Noutputs++;
            }
        }
        if (Ninputs==0)//Condition of mining transaction(The number of input addresses is zero). Add the Bitcoin value to the total sum
        {
            SumOfAllBitcoins+=outputsum/(double)100000000;//summing up the total generated bitcoins by converting the satoshi to bitcoin
        }
        if (Ninputs >0 && Noutputs >0)//Not considering the transactions with no inputs (Mining transactions)
        {
		if(((NPI==0)&& (NRI > 0)) && ((NRO==0) && (NPO >0)))
		{
				Onlyrichtopoor++;
				tempOnlyrichtopoor++;
		}
		else if (((NRI==0)&& (NPI > 0)) && ((NPO==0) && (NRO >0)))
		{
			Onlypoortorich++;
			tempOnlypoortorich++;
			
		}
		else// Rich-> rich , poor -> poor ,rich and poor --> rich or poor,rich or poor --> rich and poor
		{
			richandpoor++;
			temprichandpoor++;
		}
            if(((NPI==0)&& (NRI > 0)) && ((NPO==0) && (NRO >0)))//Only Rich Rich interation case
            {
                Onlyrichrichinteraction++;
                tempOnlyrichrichinteraction++;
            }
            else if (((NRI==0)&& (NPI > 0)) && ((NRO==0) && (NPO >0)))//only Poor Poor interaction case
            {
                Onlypoorpoorinteraction++;
                tempOnlypoorpoorinteraction++;
            }
            else // Transaction that has at least 1 pair of different category of address in the input and output
            {
                 richpoorinteraction++;
                 temprichpoorinteraction++;
            }
//            if (poorcount >0 && richcount>0)
//            {
//                richpoorinteraction++;
//                temprichpoorinteraction++;
//            }
//            if (richcount >1)
//            {
//                richrichinteraction++;
//                temprichrichinteraction++;
//            }
//            if (poorcount==0 && richcount >0)
//            {
//                Onlyrichrichinteraction++;
//                tempOnlyrichrichinteraction++;
//            }
//            if (richcount==0 && poorcount >0)
//            {
//                Onlypoorpoorinteraction++;
//                tempOnlypoorpoorinteraction++;
//            }
        }
        //Calculating Vertices money
    }
    clock_t stop=clock();

    RichPoor.close();
    RichRich.close();
    PoorPoor.close();
    Interactions.close();
    InteractionsRichgetsRicher.close();
    BTCRichPoor.close();
    ZeroNonZero.close();
    TopAccumulationMoney.close();
    ZeroMoney.close();
    NonZeroMoney.close();
    NewlyIntroducedAddresses.close();
    NewlydoneTransactionNumber.close();
    OldZeroMoneyAddressUsed.close();
    Hybrid.close();
	AnonymityconcernedRichPoorAddress.close();
    cout<<endl<<endl<<endl<<"Final Result"<<endl;
    cout<<"Number of transactions involving rich address and other pooraddresses= "<<richpoorinteraction<<endl;
    //    cout<<"Number of transactions involving at least 2 top rich address= "<<richrichinteraction<<endl;
    cout<<"Number of transactions involving only rich address= "<<Onlyrichrichinteraction<<endl;
    cout<<"Number of transactions involving only poor address= "<<Onlypoorpoorinteraction<<endl<<endl;
    cout<<"Time For Calculating Rich poor interactions= "<<((float)(stop-start)/CLOCKS_PER_SEC)<<endl<<endl;

  //  cout<<"Writing the accumulated money for all the addresses(No money for transaction vertices)"<<endl;
//    path=path+"//VerticesMoney_Address-Satoshi.dat";
    //    const char* moneypath=path+"\\VerticesMoney.dat";
    vector<pair <long,long long int> > AddressMoney;
    //    ofstream Moneyfile(path.c_str());
    long NonZero_Zerogreaterthan5=0;    
    long ZeroLessThan5=0;
    for(long i=0;i<ginst->vert_count;i++)
    {
        //      Moneyfile<<i<<" "<<VerticesMoney[i]<<endl;
        if (VerticesMoney[i]!=0)
        {
	    if (indegree[i] > 5)
		{
			NonZero_Zerogreaterthan5++;
		}
            AddressMoney.push_back(make_pair(VerticesMoney[i],i) );
        }
	else if (VerticesMoney[i]==0)//Addresses with zero balance
	{
		if (indegree[i] > 5)
		{
			NonZero_Zerogreaterthan5++;
		}
		else
		{
			ZeroLessThan5++;	
		}
	}
    }

    cout<<"The number of addresses with zero or non zero balance and in-degree greater than 5 till June6 is:"<<NonZero_Zerogreaterthan5<<endl;
    cout<<"The number of addresses with zero balance and in-degree less than 5 till June6 is:"<<ZeroLessThan5<<endl;
string pathtoTotalAddressNumberwithIndegreeZerobalanceinfo=path+"//totalAddresswithZero_InLT_5.dat";
ofstream AddresswithZero_InLT_5(pathtoTotalAddressNumberwithIndegreeZerobalanceinfo.c_str());
AddresswithZero_InLT_5<<"The number of addresses with zero or non zero balance and in-degree greater than 5 till June6 is:"<<NonZero_Zerogreaterthan5<<endl;
AddresswithZero_InLT_5<<"The number of addresses with zero balance and in-degree less than 5 till June6 is:"<<ZeroLessThan5<<endl;
AddresswithZero_InLT_5.close();
    //Moneyfile.close();
  //  cout<<"Finished writing the accumulated money for all the addresses : VerticesMoney.dat"<<endl;
    cout<<" Sorting the addresses with non zero balance"<<endl;
    sort(AddressMoney.begin(), AddressMoney.end());
    cout<<"Dumping the sorted money with non zero balance: AddressVertex SatoshiValue"<<endl;
    string path2=pathstring.substr(0,pos)+"//VerticesMoney.dat_Address-Satoshi";
    ofstream Moneyfile(path2.c_str());
    for(long i=0;i<AddressMoney.size();i++)
    {
        Moneyfile<<AddressMoney[i].second<<" "<<AddressMoney[i].first<<endl;
    }
    cout<<"Dumping the sorted money with non zero balance: Done!"<<endl;

    //Sorting the money 

    // VMoney.close();
    return 0;	
}
