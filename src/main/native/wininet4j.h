#include <windows.h>

jint throwNoClassDefError( JNIEnv *, char *);

jint throwIOException( JNIEnv *, char *);

static CHAR* getLastErrorText(CHAR *, ULONG);
