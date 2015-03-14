//do some initialization fot this double list
int init_double_list(duNode_t ** pstHead)
{
    int iRet = RET_SUCCESS;
    int iCir = 0;
    //init head node
    * pstHead = (duNode_t *)malloc(sizeof(duNode_t));
    (* pstHead)->pstPrior = NULL;
    (* pstHead)->pstNext = NULL;
    if ( !pstHead )
    {
        NO_MEMORY;
    }
    return iRet;
}

duNode_t * insert_double_list(duNode_t *pstCurrent,time_t unixtime,char *str )
{
 
 
 duNode_t * pstTmp = NULL;
 int iRet = RET_SUCCESS;
 //list initialization
 
  pstTmp = (duNode_t *)malloc(sizeof(duNode_t));
  if ( !pstTmp )
  {
   NO_MEMORY;
  }
  //assign to current node
  pstCurrent->data = (long)unixtime;
  memcpy(pstCurrent->str,str,256);
  LOGE("UNIX_TIME is:%d and pstCurrent->str is:%s\n",pstCurrent->data,pstCurrent->str);

  //alloc memory to next node
  pstCurrent->pstNext = pstTmp;
  pstTmp->pstPrior=pstCurrent;
  pstTmp->pstNext = NULL;
  
  LOGE("iRet is :%d error happens here :%s\n",iRet,strerror(errno));
  return pstTmp;
}

// output all the list info   
int show_double_list_to_file(duNode_t * pstHead,FILE*fp)
{
 struct tm *tm;
 char buff[50];
 int iRet = RET_SUCCESS;
 duNode_t * pstTmp = pstHead->pstNext;
 if ( NULL == pstHead )
 {
  LOGE("head node should not be NULL\n");
  iRet = RET_FAILED;
 }
 else
 {
  while ( pstTmp )
  {
   tm = localtime((time_t const *)&pstTmp->data);
   sprintf(buff," %04d-%02d-%02d %02d:%02d",tm->tm_year + 1900, tm->tm_mon + 1, tm->tm_mday, tm->tm_hour, tm->tm_min);
   fputs(buff,fp);
   fputs(pstTmp->str,fp);
   pstTmp = pstTmp->pstNext;
  }
 
 }
 return iRet;
}

//delete all the nodes including HEAD
int destroy_double_list(duNode_t * pstHead)
{
 duNode_t * pstTmp = NULL;   //Temp pointer for circle
 int iRet = RET_SUCCESS;
 if ( !pstHead )
 {
  LOGE("Error! pstHead is null\n");
  iRet = RET_FAILED;
 }
 else
 {
  while ( pstHead )  //Free  nodes
  {
   pstTmp = pstHead;
   pstHead = pstHead->pstNext;
   free(pstTmp);
  }
  pstHead = NULL;
 }
 return iRet;
}

duNode_t * partion(duNode_t * pstHead, duNode_t * pstLow, duNode_t * pstHigh)
{
 elemType iTmp = 0;
 elemType pivot = 0;
 char buf[256];
 if ( !pstHead )
 {
  printf("head node should not be NULL！\n");
  exit(1);
 }
 if ( !pstHead->pstNext )
 {
  return pstHead->pstNext;//just one element
 } 
 pivot = pstLow->data;
 while ( pstLow != pstHigh )
 {
  //from end to front
  while ( pstLow != pstHigh && pstHigh->data <= pivot )
  {
   pstHigh = pstHigh->pstPrior;
  }
  //swith high and low node
  iTmp = pstLow->data;
  pstLow->data = pstHigh->data;
  pstHigh->data = iTmp;
 
  memcpy(buf,pstLow->str,256);
  memcpy(pstLow->str,pstHigh->str,256);
  memcpy(pstHigh->str,buf,256);

  //from front to end
  while ( pstLow != pstHigh && pstLow->data >= pivot )
  {
   pstLow = pstLow->pstNext;
  }
  //siwth high and low node 
  iTmp = pstLow->data;
  pstLow->data = pstHigh->data;
  pstHigh->data = iTmp;
 
  memcpy(buf,pstLow->str,256);
  memcpy(pstLow->str,pstHigh->str,256);
  memcpy(pstHigh->str,buf,256);
 }
 return pstLow;
}
//quick sort alg
void quick_sort_for_double_list(duNode_t * pstHead, duNode_t * pstLow, duNode_t * pstHigh)
{
 duNode_t * pstTmp = NULL;

 pstTmp = partion(pstHead, pstLow, pstHigh);
 if ( pstLow != pstTmp )
 {
  quick_sort(pstHead, pstLow, pstTmp->pstPrior);
 }
 if ( pstHigh != pstTmp )
 {
  quick_sort(pstHead, pstTmp->pstNext, pstHigh);
 }
 
}
