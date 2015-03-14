#include "RootCheck.h"

sqlist *read_report_to_list(char *report_file)
{
    FILE *fp=NULL;
    sqlist *f=NULL;
    sqlist *head=NULL;
    char str[1024];

    if((fp=fopen(report_file,"r+")) == NULL)
        {
                LOGE("open %s faild,please check!!\n");
                return head;    //now return failed because head is NULL
        }

        if( fgets( str , sizeof(str) , fp ) == NULL )
        {
                LOGE("can not read data from file！\n");
                fclose(fp);
                return head;    //now return failed beacause head is NULL
        }
	
        //allocate heap memory to store each record from opened file
        head=(sqlist*)malloc(LEN);
	
        if(head==NULL)
        {
                LOGE("allocate memory failed!\n");
                fclose(fp);
                exit(0);
        }
        head->next=NULL;
	//printf("%s\n",str);
	/*	
	unsigned char buffer[88];
	sscanf(str,"%[^\n]",buffer);
	printf("sscanf input is:%s\n",buffer);*/
        sscanf(str,"%[^\n]",head->record);

	
	//read data by line to line 
        while (fgets( str , sizeof(str) , fp ) != NULL)
        {
                f=(sqlist*)malloc(LEN);
                f->next=NULL;
                sscanf(str,"%[^\n]",f->record) ;
		//printf("%s %s\n",f->file_name,f->md5);
		//insert data into the list that has been created
                insertNode(head,f);
        }
	
        fclose(fp);
        return head;
}

void insert_node(sqlist *head,sqlist *newNode)
{
	
        sqlist *f;
        f=head;
        while(f->next!=NULL)
        {
                f=f->next;
        }

        f->next=newNode;
        newNode->next=NULL;
}

int delete_node(sqlist *head,sqlist *currentNode)
{
	if( head == NULL || currentNode == NULL )
		return -1;
	//if  currentNode locates in the middle list
	if( currentNode->next != NULL )
	{
	    sqlist* q=currentNode->next;
	    strcpy(currentNode->record,q->record);
	    currentNode->next=q->next;
	    free(q);
	    q=NULL;
	}
	//if  currentNode locates in head
	else if(head == currentNode)
	{
	    free(currentNode);
	    currentNode=NULL;
	    head=NULL;

	}
	//if currentNode locates in the last list
	else
	{
	    sqlist* p= head;
	    while(p->next != currentNode)
		p=p->next;
	    p->next=NULL;
	    free(currentNode);
	    currentNode=NULL;
	}	
}
void print_list_to_file(sqlist *head,FILE* fp)
{
        sqlist *f;
        f=head;
        if(f!=NULL)
        {
                do
                {
                //printf("%s\n",f->record);
		fputs(f->record,fp);		
                fputs("\n",fp);
                f=f->next;
                }while(f!=NULL);
        }
	
}

void print_single_list(sqlist* head)
{
	sqlist *f;
	f = head;
	if(f!=NULL)
	{
		do{
		 LOGE("f->record is:%s\n",f->record);
		 f=f->next;
		}while(f!=NULL);
	}

}
