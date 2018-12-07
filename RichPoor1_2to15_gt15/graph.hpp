#include "graph.h"
#include <unistd.h>

template<
typename file_vert_t, typename file_index_t, typename file_weight_t,typename file_addressMap_t,
typename new_vert_t, typename new_index_t, typename new_weight_t, typename new_addressMap_t>
graph<file_vert_t,file_index_t, file_weight_t,file_addressMap_t,
new_vert_t,new_index_t,new_weight_t,new_addressMap_t>
::graph(
		const char *beg_file,
		const char *csr_file,
		const char *weight_file,
         const char *addressMap_file)
{
	double tm=wtime();
	FILE *file=NULL;
	file_index_t ret;
	
	vert_count=fsize(beg_file)/sizeof(file_index_t) - 1;
	edge_count=fsize(csr_file)/sizeof(file_vert_t);
	
	file=fopen(beg_file, "rb");
	if(file!=NULL)
	{
		file_index_t *tmp_beg_pos=NULL;

		if(posix_memalign((void **)&tmp_beg_pos, getpagesize(),
					sizeof(file_index_t)*(vert_count+1)))
			perror("posix_memalign");

		ret=fread(tmp_beg_pos, sizeof(file_index_t), 
				vert_count+1, file);
		assert(ret==vert_count+1);
		fclose(file);
		edge_count=tmp_beg_pos[vert_count];
		std::cout<<"Expected edge count: "<<tmp_beg_pos[vert_count]<<"\n";

        assert(tmp_beg_pos[vert_count]>0);

		//converting to new type when different 
		if(sizeof(file_index_t)!=sizeof(new_index_t))
		{
			if(posix_memalign((void **)&beg_pos, getpagesize(),
					sizeof(new_index_t)*(vert_count+1)))
			perror("posix_memalign");
			for(new_index_t i=0;i<vert_count+1;++i)
				beg_pos[i]=(new_index_t)tmp_beg_pos[i];
			delete[] tmp_beg_pos;
		}else{beg_pos=(new_index_t*)tmp_beg_pos;}
	}else std::cout<<"beg file cannot open\n";

		file=fopen(csr_file, "rb");
	if(file!=NULL)
	{
		file_vert_t *tmp_csr = NULL;
		if(posix_memalign((void **)&tmp_csr,getpagesize(),
						sizeof(file_vert_t)*edge_count))
			perror("posix_memalign");
		
		ret=fread(tmp_csr, sizeof(file_vert_t), edge_count, file);
        std::cout<<ret<<" "<<edge_count<<"\n";
		assert(ret==edge_count);
		assert(ret==beg_pos[vert_count]);
		fclose(file);
			
		if(sizeof(file_vert_t)!=sizeof(new_vert_t))
		{
			if(posix_memalign((void **)&csr,getpagesize(),
						sizeof(new_vert_t)*edge_count))
				perror("posix_memalign");
			for(new_index_t i=0;i<edge_count;++i)
				csr[i]=(new_vert_t)tmp_csr[i];
			delete[] tmp_csr;
		}else csr=(new_vert_t*)tmp_csr;

	}else std::cout<<"CSR file cannot open\n";

//file=fopen(weight_file, "rb");
//	if(file!=NULL)
//	{
//		file_vert_t *tmp_weight = NULL;
//		if(posix_memalign((void **)&tmp_weight,getpagesize(),
//						sizeof(file_vert_t)*edge_count))
//			perror("posix_memalign");
//		
//		ret=fread(tmp_weight, sizeof(file_vert_t), edge_count, file);
  //      std::cout<<ret<<" "<<edge_count<<"\n";
//		assert(ret==edge_count);
//		assert(ret==beg_pos[vert_count]);
//		fclose(file);
//			
//		if(sizeof(file_vert_t)!=sizeof(new_vert_t))
//		{
//			if(posix_memalign((void **)&weight,getpagesize(),
//						sizeof(new_vert_t)*edge_count))
//				perror("posix_memalign");
//			for(new_index_t i=0;i<edge_count;++i)
//				weight[i]=(new_vert_t)tmp_weight[i];
//			delete[] tmp_weight;
//		}else weight=(new_vert_t*)tmp_weight;
//
//	}else std::cout<<"Weight file cannot open\n";


	file=fopen(addressMap_file, "rb");
	if(file!=NULL)
	{
		char *tmp_addressMap = NULL;
		if(posix_memalign((void **)&tmp_addressMap,getpagesize(),sizeof(file_addressMap_t)*edge_count))
			perror("posix_memalign");
//        char* tmp_addressMap=new char[edge_count];
		
		ret=fread(tmp_addressMap, sizeof(file_addressMap_t), edge_count, file);
//        for (int i=0;i<5;i++)
 //       {
 //           std::cout<<tmp_addressMap[i]<<" ";
 //       }
		assert(ret==edge_count);
		fclose(file);
 //       std::cout<<"size of file_addressMap_t:"<<sizeof(file_addressMap_t)<<" "<<"size of new_addressMap_t"<<sizeof(new_addressMap_t)<<std::endl;	
		if(sizeof(file_addressMap_t)!=sizeof(new_addressMap_t))
		{
            std::cout<<"Right Loop\n";
			if(posix_memalign((void **)&addressMap,getpagesize(),
						sizeof(new_addressMap_t)*edge_count))
				perror("posix_memalign");
			for(new_index_t i=0;i<edge_count;++i)
            {
                std::cout<<tmp_addressMap[i]<<" ";
				addressMap[i]=(new_addressMap_t)tmp_addressMap[i];
            }
			delete[] tmp_addressMap;
		}else
        {
            addressMap=(new_addressMap_t*)tmp_addressMap;
            for (int m=0;m<5;m++)
            {
                std::cout<<addressMap[m]<<" ";
            }
         
        }
  //      delete[] tmp_addressMap;
	}
	else std::cout<<"addressMap file cannot open\n";
	
    
    file=fopen(weight_file, "rb");
	if(file!=NULL)
	{
		file_weight_t *tmp_weight = NULL;
		if(posix_memalign((void **)&tmp_weight,getpagesize(),
					sizeof(file_weight_t)*edge_count))
			perror("posix_memalign");
		
		ret=fread(tmp_weight, sizeof(file_weight_t), edge_count, file);
		assert(ret==edge_count);
		fclose(file);
        std::cout<<"size of file_weight_t:"<<sizeof(file_weight_t)<<" "<<"size of new_weight_t"<<sizeof(new_weight_t)<<std::endl;	
		if(sizeof(file_weight_t)!=sizeof(new_weight_t))
		{
            std::cout<<"In weight right loop"<<std::endl;
			if(posix_memalign((void **)&weight,getpagesize(),
						sizeof(new_weight_t)*edge_count))
				perror("posix_memalign");
			for(new_index_t i=0;i<edge_count;++i)
				weight[i]=(new_weight_t)tmp_weight[i];
			delete[] tmp_weight;
		}else 
        {
            weight=(new_weight_t*)tmp_weight;
           // for (int m=0;m<5;m++)
           // {
           //     std::cout<<tmp_weight[m]<<" ";
           // }
         //   std::cout<<"Weight In Wrong loop "<<std::endl;
        }
	}
	else std::cout<<"Weight file cannot open\n";

	std::cout<<"Graph load (success): "<<vert_count<<" verts, "
		<<edge_count<<" edges "<<wtime()-tm<<" second(s)\n";
}

