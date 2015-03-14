#include <stdio.h>
#include <stdlib.h>
#include <dirent.h>
#include <errno.h>
#include <errno.h>
#include <string.h>
#include <limits.h>
#include <inttypes.h>
#include <openssl/md5.h>
#include <jni.h>
#include <android/log.h>
#include <unistd.h>
#include <sys/stat.h>
#include <time.h>
#include <grp.h>
#include <pwd.h>
#include <assert.h>

#define MAX 1024
#define BUFSIZE 1024*16

#define LOG_TAG "root-check"
#define LOGE(...) __android_log_print(ANDROID_LOG_ERROR,LOG_TAG,__VA_ARGS__)
#define LOGD(...) __android_log_print(ANDROID_LOG_DEBUG,LOG_TAG,__VA_ARGS__)

#define RET_SUCCESS   ( 1 ) 
#define RET_FAILED   ( 0 )  



//Error report when heap memory is not enough
#define NO_MEMORY   LOGE("Error! Not enough memory!\n");exit(-1)

//specify the class needs to be JNI mapping
#define JNIREG_CLASS "com/snailws/taskscompleted/MainActivity"

/*
#if !defined(_OSD_POSIX) && !defined(__DJGPP__)
int read(int, void *, unsigned int);
#endif
*/



typedef struct system_apk_path{
	char dir[256];
	char apk_md5_report[256];
}System_Root_Path;




/*********************************************************************
* for single list operations in showing information of deleting apk files
*/
//define a single list to store content with giving report
typedef struct Single_List{
	//each record can store 1024 bytes read from a line
        unsigned char record[1024];
        struct Stu *next;
}sqlist;

#define LEN sizeof(struct Single_List)
extern void insert_node(struct Single_List *head,struct Single_List *newNode);
extern int delete_node(sqlist *head,sqlist *currentNode)
extern struct Single_List *read_report_to_list(char *report_file);
extern void print_list_to_file(struct Single_List *head,FILE* fp);
extern void print_single_list(sqlist*);






/*****************************************************************************
* for double list operations in showing information of adding/modifying apk files.
*/
//utilize elemType to represent basical type
typedef int elemType;     

//define a structure node style for doule list
typedef struct Double_List
{
 	elemType data; // represent for unix time value 
 	char str[256]; // represent for apk name after checking 
 	struct Double_List * pstNext; 
 	struct Double_List * pstPrior;
}duNode_t;

extern int init_double_list(duNode_t ** pstHead);
extern duNode_t * insert_double_list(duNode_t *pstCurrent,time_t unixtime,char *str );
extern int show_double_list_to_file(duNode_t * pstHead,FILE* fp);
extern int destroy_double_list(duNode_t * pstHead);
extern duNode_t * partion(duNode_t * pstHead, duNode_t * pstLow, duNode_t * pstHigh);
extern void quick_sort_for_double_list(duNode_t * pstHead, duNode_t * pstLow, duNode_t * pstHigh);







/*********************************************************************************************
* for other external operations,which are main logical methods when dealing root check process.
*/
extern void core_match_process(char *root,char *report_file);
extern void init_result_file();
extern int check_root_status();

extern char *do_md5(FILE *file);
extern char* print_md5(unsigned char *md);


extern int compare_str(char *src,char *dest);
extern void get_next_R(char *t,int *next);
extern int str_kmp_alg(char *s, char *t);



