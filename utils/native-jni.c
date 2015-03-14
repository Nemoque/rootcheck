#include "RootCheck.h"


/**
* begin to utilize JNI funciton and recall methods which are in Java world.
*/
JNIEXPORT void native_Root_Check(JNIEnv *env, jobject thiz)
{
	LOGD("JAVA attempts to call native C function:RootCheck\n");
	int i=0;
	System_Root_Path[2];
	strcpy(System_Root_Path[0].dir,"/system/app/");
	strcpy(System_Root_Path[0].apk_md5_report,"/data/data/root-check/report-apk1");
	strcpy(System_Root_Path[1].dir,"/system/priv-app/");
	strcpy(System_Root_Path[1].apk_md5_report,"/data/data/root-check/report-apk2");

	for (;i<2;i++) {
	    LOGD("Root-check function attempts to match :%s\n",System_Root_Path[i].dir);
	    core_match_process(System_Root_Path[i].dir,System_Root_Path[i].apk_md5_report);	
	}

	LOGD("Call back method of setFlags() in JAVA class\n");
	jclass cls = (*env)->GetObjectClass(env,thiz);
	jmethodID method1 = (*env)->GetMethodID(env,cls,"setFlags","()V");
	(*env)->CallVoidMethod(env,thiz,method1);

	LOGD("JAVA call native function Rootcheck success\n");

}


JNIEXPORT void native_Root_Status(JNIEnv *env, jobject thiz)
{
	LOGE("JAVA to call native C function RootStatus\n");    
	jclass cls = (*env)->GetObjectClass(env,thiz);
        jmethodID method2 = (*env)->GetMethodID(env,cls,"setMessage","(I)V");
	LOGD("Check if setMessage funciton call-status:%s\n",strerror(errno));

        if (check_root_status() == 1)
           (*env)->CallVoidMethod(env,thiz,method2,1);
        else
            (*env)->CallVoidMethod(env,thiz,method2,0);
	LOGD("JAVA call native function RootStatus success\n");

}



/**
* Table of methods associated with a single class.
*/
static JNINativeMethod gMethods[] = {

	//binding real methods in Java world with native functions defined in this file
	{ "RootCheck", "()V;", (void*)native_Root_Check },
	{ "RootStatus", "()V;", (void*)native_Root_Status },
};

/*
* Register several native methods for one class.
*/
static int registerNativeMethods(JNIEnv* env, const char* className,
        JNINativeMethod* gMethods, int numMethods)
{
	jclass clazz;
	clazz = (*env)->FindClass(env, className);
	if (clazz == NULL) {
	    return JNI_FALSE;
	}
	if ((*env)->RegisterNatives(env, clazz, gMethods, numMethods) < 0) {
	    return JNI_FALSE;
	}

	return JNI_TRUE;
}


/*
* Register native methods for all classes we know about.
*/
static int registerNatives(JNIEnv* env)
{
	if (!registerNativeMethods(env, JNIREG_CLASS, gMethods, 
                                 sizeof(gMethods) / sizeof(gMethods[0])))
	     return JNI_FALSE;

	return JNI_TRUE;
}

/*
* Set some test stuff up.
*
* Returns the JNI version on success, -1 on failure.
*/
JNIEXPORT jint JNICALL JNI_OnLoad(JavaVM* vm, void* reserved)
{
	JNIEnv* env = NULL;
	jint result = -1;

	if ((*vm)->GetEnv(vm, (void**) &env, JNI_VERSION_1_4) != JNI_OK) {
	    return -1;
	}
	assert(env != NULL);

	if (!registerNatives(env)) {
	    return -1;
	}
	/* success -- return valid version number */
	result = JNI_VERSION_1_4;

	return result;
}
