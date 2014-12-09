#include "com_githib_bedrin_wininet4j_WinInetHttpConnection.h"
#include "wininet4j.h"

#include <windows.h>
#include <wininet.h>

#pragma comment( lib, "wininet" )

/*
 * Class:     com_githib_bedrin_wininet4j_WinInetHttpConnection
 * Method:    initImpl
 * Signature: ()V
 */
JNIEXPORT void JNICALL Java_com_githib_bedrin_wininet4j_WinInetHttpConnection_initImpl
(JNIEnv *env, jclass obj) {
	
	HINTERNET hInternet;

	jclass winInetHttpConnectionClass;

	jfieldID hInternetField;

	hInternet = InternetOpen(
            TEXT("WinInet Test"),
            INTERNET_OPEN_TYPE_PRECONFIG,
            NULL,NULL,
            0);

	winInetHttpConnectionClass = (*env)->FindClass( env, "com/githib/bedrin/wininet4j/WinInetHttpConnection" );
	hInternetField = (*env)->GetStaticFieldID( env, winInetHttpConnectionClass, "hInternet", "J" );

	(*env)->SetStaticLongField( env, NULL, hInternetField, hInternet);

}

/*
 * Class:     com_githib_bedrin_wininet4j_WinInetHttpConnection
 * Method:    connectImpl
 * Signature: ()V
 */
JNIEXPORT void JNICALL Java_com_githib_bedrin_wininet4j_WinInetHttpConnection_connectImpl
(JNIEnv *env, jobject obj) {

	HINTERNET hInternet, hConnect;

	jclass urlClass, urlConnectionClass, winInetHttpConnectionClass;
	jmethodID getHostMethodId;
	jfieldID urlField, hInternetField, hConnectField;
	jobject url, host;

	const char * c_host;

	CHAR errorMessageBuff[256];
	CHAR* errorMessage;

	winInetHttpConnectionClass = (*env)->GetObjectClass( env, obj );
	urlConnectionClass = (*env)->FindClass( env, "java/net/URLConnection" );
	urlClass = (*env)->FindClass(env, "java/net/URL");

	hInternetField = (*env)->GetStaticFieldID( env, winInetHttpConnectionClass, "hInternet", "J" );

    urlField = (*env)->GetFieldID( env, winInetHttpConnectionClass, "url", "Ljava/net/URL;" );
	hConnectField = (*env)->GetFieldID( env, winInetHttpConnectionClass, "hConnect", "J" );

	getHostMethodId = (*env)->GetMethodID( env, urlClass, "getHost", "()Ljava/lang/String;" );

	url = (*env)->GetObjectField( env, obj, urlField);
	host = (*env)->CallObjectMethod( env, url, getHostMethodId);

	c_host = (*env)->GetStringUTFChars(env, host, 0);

	hInternet = (*env)->GetStaticLongField( env, obj, hInternetField);

	hConnect = InternetConnect(
				hInternet,
				c_host,
				INTERNET_DEFAULT_HTTP_PORT,
				NULL,NULL,
				INTERNET_SERVICE_HTTP,
				0,
				1u);

	if (NULL != hConnect) {
		(*env)->SetLongField( env, obj, hConnectField, hConnect);
	} else {
		errorMessage = getLastErrorText(errorMessageBuff,sizeof(errorMessageBuff));
		throwIOException(env, errorMessage);
	}

	(*env)->ReleaseStringUTFChars( env, host, c_host);

}

/*
 * Class:     com_githib_bedrin_wininet4j_WinInetHttpConnection
 * Method:    openRequestImpl
 * Signature: ()J
 */
JNIEXPORT jlong JNICALL Java_com_githib_bedrin_wininet4j_WinInetHttpConnection_openRequestImpl
(JNIEnv * env, jobject obj) {

	HINTERNET hConnect;

	jclass winInetHttpConnectionClass;
	jfieldID hConnectField;

	CHAR errorMessageBuff[256];
	CHAR* errorMessage;

	winInetHttpConnectionClass = (*env)->GetObjectClass( env, obj );
	hConnectField = (*env)->GetFieldID( env, winInetHttpConnectionClass, "hConnect", "J" );
	hConnect = (*env)->GetLongField( env, obj, hConnectField);

	return (jlong) HttpOpenRequest(
					hConnect,
					TEXT("GET"),
					TEXT("/"),
					NULL,
					NULL,
					0,	
					INTERNET_FLAG_KEEP_CONNECTION,
					1);

	// TODO throw exception

}

/*
 * Class:     com_githib_bedrin_wininet4j_WinInetHttpConnection
 * Method:    sendRequestImpl
 * Signature: (J)Z
 */
JNIEXPORT jboolean JNICALL Java_com_githib_bedrin_wininet4j_WinInetHttpConnection_sendRequestImpl
(JNIEnv * env, jobject obj, jlong hRequest) {
	return HttpSendRequest(hRequest, NULL,0, NULL,0);
}


jint throwNoClassDefError( JNIEnv *env, char *message )
{
    jclass exClass;
    char *className = "java/lang/NoClassDefFoundError";

    exClass = (*env)->FindClass( env, className);
    if (exClass == NULL) {
        return throwNoClassDefError( env, className );
    }

    return (*env)->ThrowNew( env, exClass, message );
}

jint throwIOException( JNIEnv *env, char *message )
{
    jclass exClass;
    char *className = "java/io/IOException" ;

    exClass = (*env)->FindClass( env, className );
    if ( exClass == NULL ) {
        return throwNoClassDefError( env, className );
    }

    return (*env)->ThrowNew( env, exClass, message );
}

// usage
//     CHAR msgText[256];
//     getLastErrorText(msgText,sizeof(msgText));
static CHAR *                      //   return error message
getLastErrorText(                  // converts "Lasr Error" code into text
CHAR *pBuf,                        //   message buffer
ULONG bufSize)                     //   buffer size
{
     DWORD retSize;
     LPTSTR pTemp=NULL;

     if (bufSize < 16) {
          if (bufSize > 0) {
               pBuf[0]='\0';
          }
          return(pBuf);
     }
     retSize=FormatMessage(FORMAT_MESSAGE_ALLOCATE_BUFFER|
                           FORMAT_MESSAGE_FROM_SYSTEM|
                           FORMAT_MESSAGE_ARGUMENT_ARRAY,
                           NULL,
                           GetLastError(),
                           MAKELANGID(LANG_ENGLISH, SUBLANG_ENGLISH_US),
                           (LPTSTR)&pTemp,
                           0,
                           NULL );
     if (!retSize || pTemp == NULL) {
          pBuf[0]='\0';
     }
     else {
          pTemp[strlen(pTemp)-2]='\0'; //remove cr and newline character
          sprintf(pBuf,"%0.*s (0x%x)",bufSize-16,pTemp,GetLastError());
          LocalFree((HLOCAL)pTemp);
     }
     return(pBuf);
}