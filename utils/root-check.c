#include "RootCheck.h"

void init_result_file()
{
    if(creat("/data/data/root-check/add_apk.txt",0755) < 0) {
        LOGE("create file add_apk.txt failure:%s\n",strerror(errno));
        exit(-1);
    }
    else
        LOGD("create file add_apk.txt success\n");

    if(creat("/data/data/root-check/delete_apk.txt",0755) <0 ) {
        LOGE("create file delete_apk.txt failure:%s\n",strerror(errno));
        exit(-1);
    }
    else 
        LOGD("Create file delete_apk.txt success\n");

    if(creat("/data/data/root-check/modify_apk.txt",0755)<0) {
	    
        LOGE("create file modify_apk.txt failure:%s\n",strerror(errno));
	exit(-1);
    }
    else
        LOGD("create file modify_apk.txt success\n");

}

char *do_md5(FILE *file)
{
    MD5_CTX ctx;
    unsigned char md[MD5_DIGEST_LENGTH];
    int fd;
    int i;
    static unsigned char buf[BUFSIZE];
	 	
    fd = fileno(file);	
    MD5_Init(&ctx);
    for(;;) {
		
        i = read(fd,buf,BUFSIZE);
	if (i <= 0) break;
	    MD5_Update(&ctx,buf,(unsigned long)i);
		
    }
    MD5_Final( &(md[0]),&ctx );
    return print_md5(md);

}

char* print_md5(unsigned char *md)
{
	int i;
	 //allocate 16 bytes to store the md5 string
        char *s=(char*)calloc(16,sizeof(char*));
        if(!s)
            exit(-1);
	//printf("length of the temp array is:%ld\n",16*sizeof(char*));
	for (i=0;i<MD5_DIGEST_LENGTH;i++)
		sprintf(&s[i*2],"%02x",md[i]);
	return s;
	free(s);

}




int compare_str(char *src,char *dest)
{
	int ret;

	//utilize formal parameter "s" to compare with "str"and the position comes from index of "str" 
	ret = str_kmp_alg(src,dest);
	if (ret){
	   //printf("This file is not matched with original system\n");
	   return 1;	
	}else{
	     //printf("This file is matched with original system\n");
	     return 0;
	}
}

void get_next_R(char *t,int *next)
{
	unsigned int j=0;
	int k = -1;
	next[0]=-1;
 
	while(j<strlen(t)-1)
	{
 
	if(k==-1||t[j]==t[k])
	{
	    j++;
	    k++;
	    next[j]=k;
	}else
	     k=next[k];
	} 
}

/*function:define a function to compare each two strings with KMP algthoim
 *s:the original string to be compared
 *t:the input string arouse comparision
 *return:the location which points to the first exact index in the "s" when "t" is in it
*/ 
int str_kmp_alg(char *s, char *t)
{
	unsigned int i;
	int j = 0;
	int *next = (int*)calloc(strlen(t),sizeof(int));
 
	//call the get_next_R() function to locate index
	get_next_R(t,next);
 
	//printf("next[] = ");
	for(i=0;i<strlen(t);i++);
	   //printf("%d ",next[i]);
 
	i=0;
	while(i<strlen(s)) {
	    if(j==-1||s[i]==t[j])
	    {	
                i++;
		j++;
	    }
	   else
	        j = next[j];
 
       if(j==strlen(t))
	    return (i-strlen(t));
       }
 
       free(next);
 
       return -1;
}

int check_root_status()
{

        struct stat buff1;
        struct stat buff2;
	struct stat buff3;
        int  res1,res2,res3;
        res1 = stat("/data/data/root-check/add_apk.txt",&buff1);
        if(res1 == 0)
        {    
           LOGD("get record.txt stat success\n");
           LOGD("Size of add_apk.txt is:%ld\n" ,(long)buff1.st_size);
        }   
        if(errno)
        {   
            LOGE("add_apk.txt not exist,cannot get record.txt stat :%s\n",strerror(errno));
            exit(-1);
        }   

        res2 = stat("/data/data/root-check/delete_apk.txt",&buff2);
        if(res2 == 0)
        {   
            LOGD("get delete_apk.txt stat success\n");
            LOGD("Size of delete_apk.txt is:%ld\n", (long)buff2.st_size);
        }   
        if(errno)
        {   
            LOGE("delete_apk.txt not exist,cannot get delete_apk.txt stat:%s\n",strerror(errno));
            exit(-1);
        }   


	res3 =stat("/data/data/root-check/modify_apk.txt",&buff3);
	if(res3 == 0)
	{
		LOGD("get modify_apk.txt stat success\n");
		LOGD("Size of modify_apk.txt is:%ld\n",(long)buff3.st_size);
	}
	if(errno)
	{
		LOGE("modify_apk.txt not exist,cannot get modify_apk.txt stat:%s\n",strerror(errno));
		exit(-1);
	}

        if(buff1.st_size >0|| buff2.st_size||buff3.st_size) 
             return 1;

        else
             return 0;

}





void core_match_process(char *root,char *report_file)
{
	DIR *dir=NULL;
	struct dirent *ptr = NULL;
	int count=0;
	struct stat st;
	struct tm *tm = NULL;
	MD5_CTX ctx;
	unsigned char md[16];
	int i;

	duNode_t * pstHead_add = NULL;
	duNode_t * pstHead_modify = NULL;
	
	init_double_list(&pstHead_add);//init two double list
	init_double_list(&pstHead_modify);

	//the temp nodes come below would be used for travsering in the list 
	duNode_t * temp_duNode_add_Current=pstHead_add;
	duNode_t * temp_duNode_modify_Current=pstHead_modify;

	duNode_t * temp_duNode_add= NULL;
	duNode_t * temp_duNode_modify= NULL;
        
	FILE *filename = NULL;//for md5 calculating 
	FILE *fp_add = NULL;//for add_apk.txt
	FILE *fp_delete = NULL;//for delete_apk.txt
	FILE *fp_modify = NULL;//for modify_apk.txt	

	char abs_path[256];//for calculating absolute path of each current file in the giving dir
	char *md5_str=(char*)calloc(16,sizeof(char*));	
	char record[1024];//

	
	int res=-1;//utilize "res" to return result of core comparision 
	uint32_t flag = 0;//utliize "flag" to detemine the behavior of match logic

	int ret;

	init_result_file();
	fp_add = fopen("/data/data/root-check/add_apk.txt","r+");
        if (fp_add == NULL) {	
	    LOGE("fp_add cannot be open with error:%s\n",strerror(errno));
	    exit(-1);
	}

	fp_delete = fopen("/data/data/root-check/delete_apk.txt","r+");
        if (fp_delete == NULL) {		 	
	    LOGE("fp_delete cannot be open with error:%s\n",strerror(errno));
	    exit(-1);
	}

	fp_modify = fopen("/data/data/root-check/modify_apk.txt","r+");
	if (fp_modify == NULL) {
	    LOGE("fp_modify cannot be open with error:%s\n",strerror(errno));
	    exit(-1);
	}

	//begin to read file specified and needs comparision 
	sqlist *head = NULL;
	sqlist *temp_list = NULL;//
	sqlist *target_node = NULL;//
         
        head = read_report_to_list(report_file);
        if (head == NULL)
        {
	    LOGE("no words in single list\n");
	    exit(-1);
        }

	
	//begin to scan the given directory and fetch each info as filename,md5 value...
 	dir = opendir(root);
	if (dir == NULL) {
	    LOGE("fail to open system apk path\n");
   	    exit(1);
	}
	errno = 0;

	while ((ptr = readdir(dir)) != NULL) {
	    if (strcmp(ptr->d_name,".") == 0 || strcmp(ptr->d_name,"..") == 0)
		continue;
	    realpath(root,abs_path);//get the real path of giving system apk directory
	    strcat(abs_path,"/");
	    strcat(abs_path,ptr->d_name);
	    if (stat(abs_path,&st) < 0)
	    {		
		LOGE("Cannot fetch stat of this file\n");
		exit(-1);
	    }
		
	    filename = fopen(abs_path,"r");
	    if (filename == NULL) {
		LOGE("%s cannot be opened with filestream:%s\n",ptr->d_name,strerror(errno));	
		exit(1);
	    }

	//begin to calculate each file's md5 value
	    if (md5_str == NULL)
		exit(-1);
	    md5_str=do_md5(filename);
	   

	//begin to organize a string recode with comparision against an advanced report
  	    strcat(record,ptr->d_name);
	    strcat(record," ");
	    strcat(record,md5_str);
	    fclose(filename);    
  	  
	//begin to compare this record with a line content from the specified file	
	   if ( head != NULL )
	   {  
	      temp_list = head;
	      while(temp_list != NULL)
              {
		  flag = 0;
		  LOGD("head->record and record are:%s\t%s\n",temp_list->record,record);
		  if(temp_list->record){
	          	res = compare_str(temp_list->record,record);
		  	
		  }
		  else {
			LOGE("there is no element now\n");
			break;
			}
		    
		 if ( res == 0 )
	         {   
		      flag = 1;
		      break;		   
		 }
		if (res ==1 && strstr(temp_list->record,ptr->d_name)!=NULL)
		{	
			flag = 2;
			break;
		}
		
		//move to next node to compare
	         temp_list = temp_list->next;

	      }
	   }

	   if (res == 1 && flag == 0)
	   {  
		temp_duNode_add=insert_DuList(temp_duNode_add_Current,st.st_ctime,ptr->d_name);
		if(temp_duNode_add==NULL)
			LOGE("insert add_apk node to du list error\n");
	       flag=0;	
	   }
	   if (res==1 && flag==2)
	   {
		temp_duNode_modify=insert_DuList(temp_duNode_modify_Current,st.st_mtime,ptr->d_name);
		if(temp_duNode_modify==NULL)
			LOGE("Insert modify_apk node to du list error\n");
		flag=0;
	   }
	            
	   if (res == 0 && flag == 1)
	   {
	   	if (delete_node(head,temp_list) == 0 )//delete node in single list which is matched with the report  
	        	LOGE("\ndelete target_node success\n");
	     	flag = 0;
	   }
  	    LOGD("\n");
	    memset(record,0,sizeof(record)); 

	    //update the current double node
	    temp_duNode_add_Current=temp_duNode_add;
	    temp_duNode_modify_Current=temp_duNode_modify; 
	    
	}

	//restore record into another file and delete each node in the list
	print_list_to_file(head,fp_delete);
	show_double_list_to_file(pstHead_add,fp_add);
	show_double_list_to_file(pstHead_modify,fp_modify);

	if (errno != 0){
	    LOGE("fail to read dir\n");
	    exit(1);
	}
	
	destroy_double_list(pstHead_add);
	destroy_double_list(pstHead_modify);
	closedir(dir);
	fclose(fp_add);
	fclose(fp_delete);
	fclose(fp_modify);
	free(md5_str);
}


