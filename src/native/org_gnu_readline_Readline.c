/**************************************************************************
 * gnu_readline_Readline.c -- implementation of the Java wrapper
 * of GNU readline.
 *
 * Java Wrapper Copyright (c) 1998-2001 by Bernhard Bablok (mail@bablokb.de)
 *
 * This program is free software; you can redistribute it and or modify
 * it under the terms of the GNU Library General Public License as published
 * by  the Free Software Foundation; either version 2 of the License or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful, but
 * WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU Library General Public License for more details.
 *
 * You should have received a copy of the GNU Library General Public License
 * along with this program; see the file COPYING.LIB.  If not, write to
 * the Free Software Foundation Inc., 59 Temple Place - Suite 330,
 * Boston, MA  02111-1307 USA
 **************************************************************************/

/*
 * This file implements the native method interface for class
 * gnu.readline.Readline
 *
 * $Revision: 1.3 $
 * $Author: Bablokb $
 */

#include "org_gnu_readline_Readline.h"

#include <readline/readline.h>
#include <readline/history.h>  
#include <string.h>
#include <errno.h>

#define BUF_LENGTH  1024

static char* utf2ucs(const char *utf8, char *ucs, size_t n);
static char* ucs2utf(const char *ucs, char *utf8, size_t n);

static jobject   jniObject;
static jmethodID jniMethodId;
static jclass    jniClass;
static JNIEnv*   jniEnv;

/* -------------------------------------------------------------------------- */
/* Initialize readline and history. Set application name.                     */
/* -------------------------------------------------------------------------- */

JNIEXPORT void JNICALL Java_org_gnu_readline_Readline_initReadline
                              (JNIEnv *env, jclass theClass, jstring jappName) {
   const char *appName;
   jboolean is_copy;

   appName = (*env)->GetStringUTFChars(env,jappName,&is_copy);
   if (appName && strlen(appName))
     rl_readline_name = strdup(appName);
   else
     rl_readline_name = strdup("JAVA");
   if (is_copy == JNI_TRUE)
    (*env)->ReleaseStringUTFChars(env, jappName, appName);
   using_history();
}


/* -------------------------------------------------------------------------- */
/* Prompt for input. "Main" readline function.          .                     */
/* -------------------------------------------------------------------------- */

JNIEXPORT jstring JNICALL Java_org_gnu_readline_Readline_readline
                               (JNIEnv *env, jclass theClass, jstring jprompt) {

  char buffer[BUF_LENGTH];
  const char *prompt;
  char *input;
  jboolean is_copy;

  /* retrieve prompt argument and convert to ucs ---------------------------- */

  prompt = (*env)->GetStringUTFChars(env,jprompt,&is_copy);
  if (!utf2ucs(prompt,buffer,BUF_LENGTH)) {
    jclass newExcCls;
    if (is_copy == JNI_TRUE)
      (*env)->ReleaseStringUTFChars(env, jprompt, prompt);
    newExcCls = (*env)->FindClass(env,"java/io/UnsupportedEncodingException");
    if (newExcCls != NULL)
      (*env)->ThrowNew(env,newExcCls,"");
    return NULL;
  }
  if (is_copy == JNI_TRUE)
    (*env)->ReleaseStringUTFChars(env, jprompt, prompt);

  /* use gnu-readline, convert string to utf8 ------------------------------- */

  input = readline(buffer);
  if (input == NULL) {
    jclass     newExcCls;
    newExcCls = (*env)->FindClass(env,"java/io/EOFException");
    if (newExcCls != NULL)
      (*env)->ThrowNew(env,newExcCls,"");
    return NULL;
  } else if (*input) {
    add_history(input);
    ucs2utf(input,buffer,BUF_LENGTH);
    return (*env)->NewStringUTF(env,buffer);
  } else
    return NULL;
}


/* -------------------------------------------------------------------------- */
/* Read keybindings from file.                                                */
/* -------------------------------------------------------------------------- */

JNIEXPORT void JNICALL Java_org_gnu_readline_Readline_readInitFile
                            (JNIEnv *env, jclass theClass, jstring jfilename) {
  char buffer[BUF_LENGTH];
  const char *filename;
  jboolean is_copy;

  /* retrieve filename argument and convert to ucs -------------------------- */

  filename = (*env)->GetStringUTFChars(env,jfilename,&is_copy);
  if (!utf2ucs(filename,buffer,BUF_LENGTH)) {
    jclass newExcCls;
    if (is_copy == JNI_TRUE)
      (*env)->ReleaseStringUTFChars(env,jfilename,filename);
    newExcCls = (*env)->FindClass(env,"java/io/UnsupportedEncodingException");
    if (newExcCls != NULL)
      (*env)->ThrowNew(env,newExcCls,"");
    return;
  }
  if (is_copy == JNI_TRUE)
    (*env)->ReleaseStringUTFChars(env,jfilename,filename);

  /* pass to readline function ---------------------------------------------- */

  if (rl_read_init_file(buffer)) {
    jclass newExcCls;
    newExcCls = (*env)->FindClass(env,"java/io/IOException");
    if (newExcCls != NULL)
      (*env)->ThrowNew(env,newExcCls,strerror(errno));
    return;
  }
}


/* -------------------------------------------------------------------------- */
/* Read keybinding from string.                                               */
/* -------------------------------------------------------------------------- */

JNIEXPORT jboolean JNICALL Java_org_gnu_readline_Readline_parseAndBind
                                (JNIEnv *env, jclass theClass, jstring jline) {
  char buffer[BUF_LENGTH];
  const char *line;
  jboolean is_copy;

  /* retrieve line argument and convert to ucs -------------------------- */

  line = (*env)->GetStringUTFChars(env,jline,&is_copy);
  if (!utf2ucs(line,buffer,BUF_LENGTH)) {
    jclass newExcCls;
    if (is_copy == JNI_TRUE)
      (*env)->ReleaseStringUTFChars(env,jline,line);
    newExcCls = (*env)->FindClass(env,"java/io/UnsupportedEncodingException");
    if (newExcCls != NULL)
      (*env)->ThrowNew(env,newExcCls,"");
    return (jboolean) JNI_FALSE;
  }
  if (is_copy == JNI_TRUE)
    (*env)->ReleaseStringUTFChars(env,jline,line);

  /* pass to readline function ---------------------------------------------- */

  if (rl_parse_and_bind(buffer))
    return (jboolean) JNI_FALSE;
  else
    return (jboolean) JNI_TRUE;
}

/* -------------------------------------------------------------------------- */
/* Read history file                                                          */
/* -------------------------------------------------------------------------- */

JNIEXPORT void JNICALL Java_org_gnu_readline_Readline_readHistoryFile
                            (JNIEnv *env, jclass theClass, jstring jfilename) {
  char buffer[BUF_LENGTH];
  const char *filename;
  jboolean is_copy;
    
  /* retrieve filename argument and convert to ucs -------------------------- */

  filename = (*env)->GetStringUTFChars(env,jfilename,&is_copy);
  if (!utf2ucs(filename,buffer,BUF_LENGTH)) {
    jclass newExcCls;
    if (is_copy == JNI_TRUE)
      (*env)->ReleaseStringUTFChars(env,jfilename,filename);
    newExcCls = (*env)->FindClass(env,"java/io/UnsupportedEncodingException");
    if (newExcCls != NULL)
      (*env)->ThrowNew(env,newExcCls,"");
    return;
  }
  if (is_copy == JNI_TRUE)
    (*env)->ReleaseStringUTFChars(env,jfilename,filename);
  
  /* pass to history function ----------------------------------------------- */

  read_history(buffer);
}

/* -------------------------------------------------------------------------- */
/* Write history file                                                         */
/* -------------------------------------------------------------------------- */

JNIEXPORT void JNICALL Java_org_gnu_readline_Readline_writeHistoryFile
                            (JNIEnv *env, jclass theClass, jstring jfilename) {
  char buffer[BUF_LENGTH];
  const char *filename;
  jboolean is_copy;
  
  /* retrieve filename argument and convert to ucs -------------------------- */

  filename = (*env)->GetStringUTFChars(env,jfilename,&is_copy);
  if (!utf2ucs(filename,buffer,BUF_LENGTH)) {
    jclass newExcCls;
    if (is_copy == JNI_TRUE)
      (*env)->ReleaseStringUTFChars(env,jfilename,filename);
    newExcCls = (*env)->FindClass(env,"java/io/UnsupportedEncodingException");
    if (newExcCls != NULL)
      (*env)->ThrowNew(env,newExcCls,"");
    return;
  }
  if (is_copy == JNI_TRUE)
    (*env)->ReleaseStringUTFChars(env,jfilename,filename);
  
  /* pass to history function ----------------------------------------------- */

  write_history(buffer);
}


/* -------------------------------------------------------------------------- */
/* Completer function, visible to the readline library                        */
/* -------------------------------------------------------------------------- */

char *java_completer(char *text, int state) {
  jstring jtext;
  jstring completion;
  char *line;
  jboolean is_copy;
  
  jtext = (*jniEnv)->NewStringUTF(jniEnv,text);

  if (jniMethodId == 0) {
    return;
  }

  completion = (*jniEnv)->CallObjectMethod(jniEnv, jniObject,
						 jniMethodId, jtext, state);
  if (!completion) {
    return ((char *)NULL);
  }

  line = (*jniEnv)->GetStringUTFChars(jniEnv,completion,&is_copy);
  return line;
}
  
/* -------------------------------------------------------------------------- */
/* Install completer object                                                   */
/* -------------------------------------------------------------------------- */

JNIEXPORT void JNICALL Java_org_gnu_readline_Readline_installCompleter
                                      (JNIEnv *env, jclass class, jobject obj) {
  
/*   completer_cls = (*env)->GetObjectClass(env, compl); */
/*   completer_cls = (*env)->NewGlobalRef(env, completer_cls); */

/*   completer = (*env)->GetMethodID(env, completer_cls, */
/* 			      "completer", */
/* 			      "(Ljava/lang/String;I)Ljava/lang/String;"); */

/*  (*env)->CallVoidMethod(env, completer_cls, completer, "test", 1); */
  jniEnv    = env;
  jniObject = obj;
  jniClass = (*jniEnv)->GetObjectClass(jniEnv, jniObject);
  jniClass = (*env)->NewGlobalRef(env, jniClass);
  jniObject = (*env)->NewGlobalRef(env, jniObject);
  jniMethodId = (*jniEnv)->GetMethodID(jniEnv, jniClass, "completer",
				    "(Ljava/lang/String;I)Ljava/lang/String;");
  if (jniMethodId == 0) {
    return;
  }
  rl_completion_entry_function = (Function*)java_completer;
}

/* -------------------------------------------------------------------------- */
/* Convert utf8-string to ucs1-string                   .                     */
/* -------------------------------------------------------------------------- */

char* utf2ucs(const char *utf8, char *ucs, size_t n) {
  const char *pin;
  char *pout;
  unsigned char current, next;
  int i;

  for (i=0,pin=utf8,pout=ucs; i<n && *pin; i++,pin++,pout++) {
    current = *pin;
    if (current >= 0xE0) {                   /* we support only two-byte utf8 */
      return NULL;
    } else if ((current & 0x80) == 0)        /* one-byte utf8                 */
      *pout = current;
    else {                                   /* two-byte utf8                 */
      next = *(++pin);
      if (next >= 0xC0) {                    /* illegal coding                */
	return NULL;
      }
      *pout = ((current & 3) << 6) +         /* first two bits of first byte  */
	(next & 63);                         /* last six bits of second byte  */
    }
  }
  if (i<n)
    *pout = '\0';
  return ucs;
}

/* -------------------------------------------------------------------------- */
/* Convert ucs1-string to utf8-string                   .                     */
/* -------------------------------------------------------------------------- */

char* ucs2utf(const char *ucs, char *utf8, size_t n) {
  const char *pin;
  char *pout;
  unsigned char current;
  int i;

  for (i=0,pin=ucs,pout=utf8; i<n && *pin; i++,pin++,pout++) {
    current = *pin;
    if (current < 0x80)                      /* one-byte utf8                 */
      *pout = current;
    else {                                   /* two-byte utf8                 */
      *pout = 0xC0 + (current>>6);           /* first two bits                */
      pout++, i++;                           /* examine second byte           */
      if (i>=n) {                            /* cannot convert last byte      */
	*(--pout) = '\0';
	return utf8;
      }
      *pout = 0x80 + (current & 63);         /* last six bits                 */
    }
  }
  if (i<n)
    *pout = '\0';
  return utf8;
}
