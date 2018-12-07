#include <iostream>
#include <stdlib.h>
#include <sstream>
#include <fstream>
#include <string>
#include <vector>
#include <cstring>
#include <algorithm>
#include <time.h>
using namespace std;
template <typename T>

std::string ToString(T val)
{
    std::stringstream stream;
    stream << val;
    return stream.str();
}

int StringToInt(std::string str)
{
    return atoi(str.c_str());
}

//vector <string> TransactionID;// Faster to search in std::vectors
//ptrdiff_t pos = distance(Names.begin(), find(Names.begin(), Names.end(), old_name_));
size_t NAddresses;// save the addresses from the outputs in the transactions

 vector<pair <string,long> > blockInfo;
 vector<pair <string,string> > TransactionID; //<Transactionstring,Blockstring>
vector <string> Addresses; // again added on 20 June 2018 for duplicate address removal

long GetTransactionTime(string blockstring)
{
    size_t left=0;
    size_t right=blockInfo.size()-1;
    while (left<=right)
    {
        size_t mid= left+(right-left)/2;
        int value=strcmp((blockInfo[mid].first).c_str(),blockstring.c_str());
        if (value==0)
        {
            return blockInfo[mid].second;
        }
        else if (value <0)
        {
            left = mid +1;
        }
        else
        {
            right = mid -1;
        }

    }
    return -1;
}
size_t GetTransID(string transaction)
{
    size_t left =0;
    size_t right=TransactionID.size()-1;
    while (left<=right)
    {
        size_t mid= left+(right-left)/2;
        int value=strcmp((TransactionID[mid].first).c_str(),transaction.c_str());
        if (value==0)
        {
            return mid;
        }
        else if(value <0) //mid is less than the query string. It lies on right side
        {
            left=mid+1;
        }
        else
        {
            right=mid-1;
        }
    }
    return -1;// transaction not found in vector

}

size_t GetAddressID(string address)
{
    //    size_t pos=distance(Addresses.begin(), find(Addresses.begin(), Addresses.end(), address));
    //  if (pos > Addresses.size()) return -1; // Address not found
    size_t left=0;
    size_t right=Addresses.size()-1;
    while (left<=right)
    {
        size_t mid=left+(right-left)/2;
        int value=strcmp(Addresses[mid].c_str(),address.c_str());
        if(value==0)
        {
            return mid;
        }
        else if(value <0)
        {
            left=mid+1;
        }
        else
        {
            right=mid-1;
        }
    }
    return -1;
}


struct Inputs
{
    size_t IP_address;
    string IP_address_string;
    string BitcoinValue;

};

struct Outputs
{
    size_t OP_address;
    string OP_address_string;
    string BitcoinValue;
};

struct Transaction_Struct
{
    size_t TxID;
    string TxIDstring;
    vector <Inputs> Input;
    vector <Outputs> Output;
    string block;
};

Transaction_Struct* transactions;

string GetInputAddress(string Prev_Transaction,int Index,size_t transID)
{
    // size_t transID= GetTransID(Prev_Transaction);
    string address="";
    if (transID==-1)
    {
        cout<<"cannot find Previous transaction";
        return address;
    }
    address= transactions[transID].Output[Index].OP_address_string;
    return address;
}

//vector <vector<string>> Tx_out;

void ReadBlockFile()
{
 //    std:: ifstream Blk("/home/anil/Documents/RustyBlockchainParser/rusty-blockparser/DumpedCSV/blocks-0-136312.csv");
   std:: ifstream Blk("../RawData/blocks-0-526500.csv");
    std:: string line;
    std:: string blk_ID;
    std:: string height;
     std:: string version;
      std:: string blocksize;
       std:: string hashPrev;
        std:: string hashMerkleRoot;
        std:: string timeStr;
         while (getline(Blk,line))
         {
              std::stringstream linestream(line);
              getline(linestream,blk_ID,';');
              getline(linestream,height,';');
              getline(linestream,version,';');
              getline(linestream,blocksize,';');
              getline(linestream,hashPrev,';');
              getline(linestream,hashMerkleRoot,';');
              getline(linestream,timeStr,';');
  //              std::string::size_type sz;
                long time = std::atoll(timeStr.c_str());
//                VerticesMoney_temp.push_back(atoll(money.c_str()));
              blockInfo.push_back(make_pair(blk_ID,time) );
         }
          sort(blockInfo.begin(), blockInfo.end());
}

void ReadTXFile()
{
    //    std:: ifstream Tx("./RawData/transactions-0-526500.csv");
//    std:: ifstream Tx("/home/anil/Documents/RustyBlockchainParser/rusty-blockparser/DumpedCSV/transactions-0-136312.csv");
std:: ifstream Tx("../RawData/transactions-0-526500.csv");
    std:: string line;
    std::string blk;
    while (getline(Tx,line))
    {
        std::stringstream linestream(line);
        std::string Tx_id;
        getline(linestream,Tx_id,';');      
//        TransactionID.push_back(Tx_id);
        getline(linestream,blk,';');
        TransactionID.push_back(make_pair(Tx_id,blk) );
    }

    sort(TransactionID.begin(), TransactionID.end());
    transactions=new Transaction_Struct[TransactionID.size()];
    // for (int i=0;i<TransactionID.size();i++)
    // {
    //     cout<<TransactionID[i]<<endl;
    // }
}

void ReadTX_OutFile()
{
    //    std::ifstream TX_O("./RawData/tx_out-0-526500.csv");
//    std::ifstream TX_O("/home/anil/Documents/RustyBlockchainParser/rusty-blockparser/DumpedCSV/tx_out-0-136312.csv");
std::ifstream TX_O("../RawData/tx_out-0-526500.csv");
    std::string line;
    std::string Tx_id;
    std::string IndexOut;
    std::string value;
    std::string ScriptPubKey;
    std::string address;
    size_t count=0;
    vector <string> TempAddresses;
    while (getline(TX_O,line))
    {
        std::stringstream linestream(line);
        getline(linestream,Tx_id,';');
        getline(linestream,IndexOut,';');    
        getline(linestream,value,';');
        getline(linestream,ScriptPubKey,';');
        getline(linestream,address,';');
        TempAddresses.push_back(address);

        size_t transID=GetTransID(Tx_id);
        // cout<<transID<<" ";
        Outputs temp_output;
        temp_output.OP_address_string=address;
        temp_output.OP_address=0;// intially don't know the ID. Will be known only after sorting Addresses
        temp_output.BitcoinValue=value;
        if (transID==-1) 
        {
            cout <<"No Transaction found while reading the output file"<<endl;
            return;
        }
        transactions[transID].Output.push_back(temp_output); // pushing the output value in transactions
        transactions[transID].TxIDstring=Tx_id;
        count++;

    }
    //    NAddresses=count;
    sort(TempAddresses.begin(), TempAddresses.end());
    cout<<"Start Removing Duplicate Addresses"<<endl;
    clock_t start=clock();
    Addresses.push_back(TempAddresses[0]);
    for (size_t i =1;i<TempAddresses.size();i++)
    {
        if (TempAddresses[i]!=TempAddresses[i-1])
        {
            Addresses.push_back(TempAddresses[i]);//Addresses will have addresses in sorted order with duplicates removed
        }
    }
    clock_t stop=clock();
    cout<<"Finished Removing Duplicate Addresses"<<endl;
    cout<<"Time For Removing Duplicate Addresses= "<<((float)(stop-start)/CLOCKS_PER_SEC)<<endl;
    NAddresses=Addresses.size();
}

void ReadTX_InFile()
{
    //    std::ifstream TX_I("./RawData/tx_in-0-526500.csv");
   // std::ifstream TX_I("/home/anil/Documents/RustyBlockchainParser/rusty-blockparser/DumpedCSV/tx_in-0-136312.csv");
std::ifstream TX_I("../RawData/tx_in-0-526500.csv");
    std::string line;
    std::string Tx_id;
    std::string hashPrevOut;//transaction ID of previous hash
    std::string IndexPrevOut;//index of previous output address in the previous transaction
    std::string ScriptSig;
    std::string sequence;
    //    size_t count=0;

    while (getline(TX_I,line))
    {
        std::stringstream linestream(line);
        getline(linestream,Tx_id,';');
        getline(linestream,hashPrevOut,';');    //Previous Transaction's ID containing this money
        getline(linestream,IndexPrevOut,';');
        getline(linestream,ScriptSig,';');
        getline(linestream,sequence,';');
        //  string address=GetInputAddress(hashPrevOut,IndexPrevOut); 
        string address="";  
        bool miner =false;
        if(hashPrevOut=="0000000000000000000000000000000000000000000000000000000000000000") 
        {
            miner=true;
        }
        size_t transID=GetTransID(Tx_id);
        size_t PrevtransID;
        if(!miner)
        {
            PrevtransID=GetTransID(hashPrevOut);
        }
        int INTPrevIndex=StringToInt(IndexPrevOut);
        if (transID==-1)
        {
            cout <<"No Transaction found while reading the input file"<<endl;
            return;
        }
        if (miner)
        {
            //no source for the money. That means it is newly generated from a miner
            address="-1";
            INTPrevIndex=0;
            //          cout<<"No Source Input: Miner Detected!"<<endl;
        }
        else
        {
            //            cout<<"Source Detected"<<endl;
            //            address=GetInputAddress(hashPrevOut,INTPrevIndex,PrevtransID); 
            address= transactions[PrevtransID].Output[INTPrevIndex].OP_address_string;
        }
        Inputs temp_input;

        temp_input.IP_address_string=address;
        //         size_t vertexNumber
        if (miner)
        {
            temp_input.IP_address=-1;
        }
        else
        {
            //                vertexNumber=GetAddressID(address);
            temp_input.IP_address=GetAddressID(address);
            //            temp_input.IP_address=transactions[PrevtransID].Output[INTPrevIndex].OP_address;
        }
        temp_input.IP_address_string=address;
        // cout<<transID<<" "<<INTPrevIndex<<" "<<Tx_id<<endl;
        // cout<<"Previous Transaction: "<<hashPrevOut<<" "<<"address: "<<address<<endl;
        try
        {
            if (miner)
            {
                temp_input.BitcoinValue= "5000000000";// 50 Bitcoins Considered for the begnning //9 June 2018 there is 12.5 reward for creating a new block to miner
            }       
            else
            {
                // if (transactions[PrevtransID].Output.size()+1<INTPrevIndex)
                {
                    temp_input.BitcoinValue=transactions[PrevtransID].Output[INTPrevIndex].BitcoinValue;
                }
                // else
                {
                    //   cout<<"Invalid Address Indexing detected"<<endl;
                    // temp_input.BitcoinValue="0";
                }
            }
        }
        catch(std::exception& e)
        {
            cout<<"Exception! at "<<"TransID: "<<transID<<"INTPrevIndex: "<<INTPrevIndex<<"Exception: "<<e.what()<<endl;
            return;
        }

        transactions[transID].Input.push_back(temp_input); // pushing the output value in transactions
        miner=false;
    }
}

void WriteEdgeList()
{
    //    ofstream IpTx("./Edges/Input-Transaction.dat");
    //    ofstream TxOp("./Edges/Transaction-Output.dat");
    //    ofstream IpOp("./Edges/Input-Output.dat");
    //  ofstream VMap("./Edges/VertexMap.dat");
  //    ofstream VMap("/home/anil/Documents/RustyBlockchainParser/rusty-blockparser/DumpedCSV/Edges/Final/VertexMap.dat");
   ofstream VMap("VertexMap.dat");
//   ofstream VMap_Money("/home/anil/Documents/RustyBlockchainParser/rusty-blockparser/DumpedCSV/Edges/Final/MoneyPerVertex.dat");
    // ofstream AdTx("./Edges/Address-Transaction.dat");
 //    ofstream AdTx("/home/anil/Documents/RustyBlockchainParser/rusty-blockparser/DumpedCSV/Edges/Final/Address-Transaction.dat");
   ofstream AdTx("Address-Transaction.dat");
    // ofstream TxT("/home/anil/Documents/RustyBlockchainParser/rusty-blockparser/DumpedCSV/Edges/Final/TransactionV-Time.dat");
	ofstream TxT("TransactionV-Time.dat");
//    ofstream AdTxImage("/home/anil/Documents/graphviz/Address-Transaction.dot");
//    AdTxImage<<"digraph hello{"<<endl;
    size_t* Vertices_Address=new size_t[NAddresses];
    size_t* Vertices_Transaction=new size_t[TransactionID.size()];
  //  size_t* Vertices_Money=new size_t[NAddresses+TransactionID.size()];
    for (size_t i=0;i<NAddresses;i++)
    {
//        Vertices_Money[i]=0;
        Vertices_Address[i]=-1;// Vertex not assigned
        if(i<TransactionID.size())
        {
            Vertices_Transaction[i]=-1;//vertex not assigned
        }
    }

    size_t count=0;
    size_t IPVertex;
    size_t OPVertex;
    size_t TVertex;
    for (size_t t=0;t<TransactionID.size();t++)
    {
        if (TransactionID.size()%10000==0)
        {
            cout<<"T:"<<t<<" ";
        }
        size_t T_ID=t;//GetAddressID(transactions[t].Output[o].OP_address_string);
        if(Vertices_Transaction[T_ID] ==-1)
        {
            count++;
            TVertex=count;
            Vertices_Transaction[T_ID]=count;
            VMap<<TVertex<<" "<<"T"<<" "<<transactions[t].TxIDstring<<endl;
            string blockstring=TransactionID[t].second;
            long time=GetTransactionTime(blockstring);
            TxT<<TVertex<<" "<<time<<endl;
            //VMap_Money<<TVertex<<" "<<0<<endl;
//            Vertices_Money[TVertex]=0;
        }
        else
        {
            TVertex=Vertices_Transaction[T_ID];
        }
   //     Vertices_Money[TVertex]+=0;// Assigning all the Transaction Addresses money as 0 right now. Can be assigned Sum(O/p)-Sum(i/p)--> going to the miner later
        double sum=0;
        for (int l=0;l<transactions[t].Input.size();l++)//To find sum of input in the current transaction
        {
            sum+=atof((transactions[t].Input[l].BitcoinValue).c_str());
        }

        for (int o=0;o<transactions[t].Output.size();o++)
        {
            size_t O_Address_ID=GetAddressID(transactions[t].Output[o].OP_address_string);// Position of address in sorted array address
            if(Vertices_Address[O_Address_ID] ==-1)
            {
                count++;
                OPVertex=count;
                Vertices_Address[O_Address_ID]=count;
                VMap<<OPVertex<<" "<<"A"<<" "<<transactions[t].Output[o].OP_address_string<<endl;
            }
            else
            {
                OPVertex=Vertices_Address[O_Address_ID];
            }
//            cout<<t<<" ";
          //  if (t==468657)
        //    {
      //          int a =5;
    //        }
  //          Vertices_Money[OPVertex]+=atof((transactions[t].Output[o].BitcoinValue).c_str());
            AdTx<<TVertex<<" "<<OPVertex<<" "<<"O"<<" "<<transactions[t].Output[o].BitcoinValue<<endl;
          //  AdTxImage<<TVertex<<"->"<<OPVertex<<";"<<endl;

            for (int i=0;i<transactions[t].Input.size();i++)
            {
                size_t I_Address_ID=transactions[t].Input[i].IP_address;// Position of address in sorted array address
                if(transactions[t].Input[i].IP_address_string!="-1")
                {
                    if (Vertices_Address[I_Address_ID] ==-1)
                    {
                        count++;
                        IPVertex=count;
                        Vertices_Address[I_Address_ID]=count;
                        VMap<<IPVertex<<" "<<"A"<<" "<<transactions[t].Input[i].IP_address_string<<endl;
                    }
                    else
                    {
                        IPVertex=Vertices_Address[I_Address_ID]; // The vertex in the file with value -1 means an input address for a miner
                    }
                    //            IpTx<<IPVertex<<" "<<TVertex<<" "<<transactions[t].Input[i].BitcoinValue<<endl;// Don't write input address-transaction for miner as miner don't have input address
                    if (o==0)
                    {
 //                          AdTx<<IPVertex<<" "<<TVertex/*<<" "<<transactions[t].Input[i].BitcoinValue*/<<endl;
                         AdTx<<TVertex<<" "<<IPVertex<<" "<<"I"<<" "<<transactions[t].Input[i].BitcoinValue<<endl;
//                           Vertices_Money[IPVertex]-=atof((transactions[t].Input[i].BitcoinValue).c_str());
        //                AdTxImage<<IPVertex<<"->"<<TVertex<<";"<<endl;
                    }
                }
                else
                {
                    IPVertex=0;
                }
            }
        }

    }
  //  AdTxImage<<"}";
    //AdTxImage.close();
//}
// for (size_t i=0;i<NAddresses+TransactionID.size();i++)
//{
//    VMap_Money<<i<<" "<<Vertices_Money[i]<<endl;
//}
delete[] Vertices_Address;
delete[] Vertices_Transaction;
}

int main()
{
    clock_t Prestart=clock();
    cout<<"Start Reading Block File"<<endl;
    ReadBlockFile();
    cout<<"Finished Reading Block File"<<endl;
    clock_t start=clock();
    cout<<"Time For Reading Block File= "<<((float)(start-Prestart)/CLOCKS_PER_SEC)<<endl;
    cout<<"Start Reading Transaction File"<<endl;
    ReadTXFile();
    cout<<"Finished Reading Transaction File"<<endl;
    clock_t stop=clock();
    cout<<"Time For Reading Transaction File= "<<((float)(stop-start)/CLOCKS_PER_SEC)<<endl;

    cout<<"Start Reading OutAddress File"<<endl;
    ReadTX_OutFile();
    cout<<"Finished Reading OutAddress File"<<endl;
    clock_t stop1=clock();
    cout<<"Time For Reading OutAddress File= "<<((float)(stop1-stop)/CLOCKS_PER_SEC)<<endl;

    cout<<"Start Reading InAddress File"<<endl;
    ReadTX_InFile();
    cout<<"Finished Reading InAddress File"<<endl;
    clock_t stop2=clock();
     cout<<"Time For Reading InAddress File= "<<((float)(stop2-stop1)/CLOCKS_PER_SEC)<<endl;

    cout<<"Start Writing Edge Files"<<endl;
    WriteEdgeList();
    cout<<"Finished Reading Edge Files"<<endl;
    clock_t stop3=clock();
    cout<<"Time For Reading InAddress File= "<<((float)(stop3-stop2)/CLOCKS_PER_SEC)<<endl;
    
    cout<<"Total Time for Edge generation "<<((float)(stop3-start)/CLOCKS_PER_SEC)<<endl;
    return 0;
}
