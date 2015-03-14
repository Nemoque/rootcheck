//初始化双向链表
int init_double_list(duNode_t ** pstHead)
{
    int iRet = RET_SUCCESS;
    int iCir = 0;
    //初始化头节点
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
 //链表初始化
 
  pstTmp = (duNode_t *)malloc(sizeof(duNode_t));
  if ( !pstTmp )
  {
   NO_MEMORY;
  }
  //赋值给当前的节点
  pstCurrent->data = (long)unixtime;
  memcpy(pstCurrent->str,str,256);
  LOGE("UNIX_TIME is:%d and pstCurrent->str is:%s\n",pstCurrent->data,pstCurrent->str);

  //当前节点指向下一个开辟的空间
  pstCurrent->pstNext = pstTmp;
  pstTmp->pstPrior=pstCurrent;
  pstTmp->pstNext = NULL;
  
  LOGE("iRet is :%d error happens here :%s\n",iRet,strerror(errno));
  return pstTmp;
}

// 打印链表 链表的data元素是可打印的整形    
int show_double_list_to_file(duNode_t * pstHead,FILE*fp)
{
 struct tm *tm;
 char buff[50];
 int iRet = RET_SUCCESS;
 duNode_t * pstTmp = pstHead->pstNext;
 if ( NULL == pstHead )
 {
  LOGE("链表的头节点是空\n");
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

/* 删除包括头节点在内的所有的节点. 07/04/28  */
int destroy_double_list(duNode_t * pstHead)
{
 duNode_t * pstTmp = NULL;   /* Temp pointer for circle  */
 int iRet = RET_SUCCESS;
 if ( !pstHead )
 {
  LOGE("Error! pstHead is null\n");
  iRet = RET_FAILED;
 }
 else
 {
  while ( pstHead )  /* Free  nodes      */
  {
   pstTmp = pstHead;
   pstHead = pstHead->pstNext;
   free(pstTmp);
  }
  pstHead = NULL;
 }
 return iRet;
}/* End of destroyList----------------------------------------------*/

//正确的快速排序 2007/05/09
/*
 一趟快速排序的具体做法是：附设两个指针low和high(即第一个和最后一个指针),
 他们的初值分别为low和high设枢轴(一般为low的值pivot)记录的关键字
 (即本例子中的整形data)为pivot，则首先从high所指位置
 起向前搜索到第一个关键字小于pivot的记录和枢轴记录交换，然后从low所指位置起
 向后搜索，找到第一个关键字大于pivot的记录和枢轴记录相互交换，重复这两步直
 至low = high为止。
*/
duNode_t * partion(duNode_t * pstHead, duNode_t * pstLow, duNode_t * pstHigh)
{
 elemType iTmp = 0;
 elemType pivot = 0;
 char buf[256];
 if ( !pstHead )
 {
  printf("错误，头节点为空！\n");
  exit(1);
 }
 if ( !pstHead->pstNext )
 {
  return pstHead->pstNext;//就一个元素
 } 
 pivot = pstLow->data;
 while ( pstLow != pstHigh )
 {
  //从后面往前换
  while ( pstLow != pstHigh && pstHigh->data <= pivot )
  {
   pstHigh = pstHigh->pstPrior;
  }
  //交换high low
  iTmp = pstLow->data;
  pstLow->data = pstHigh->data;
  pstHigh->data = iTmp;
 
  memcpy(buf,pstLow->str,256);
  memcpy(pstLow->str,pstHigh->str,256);
  memcpy(pstHigh->str,buf,256);

  //从前往后换
  while ( pstLow != pstHigh && pstLow->data >= pivot )
  {
   pstLow = pstLow->pstNext;
  }
  //交换high low
  iTmp = pstLow->data;
  pstLow->data = pstHigh->data;
  pstHigh->data = iTmp;
 
  memcpy(buf,pstLow->str,256);
  memcpy(pstLow->str,pstHigh->str,256);
  memcpy(pstHigh->str,buf,256);
 }
 return pstLow;
}
//快排
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
