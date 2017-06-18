/*
 * JNICALLJAVA.h
 *
 *  Created on: 2016/11/14
 *      Author: zhangshanming
 */

#ifndef JNI_CALL_JAVA_ALGORITHMS_H_
#define JNI_CALL_JAVA_ALGORITHMS_H_
#include <jni.h>
#include <jni_md.h>

#include <iostream>
#include <cstring>
#include <sstream>
#include <map>
#include "../graphs/BasePath.h"
#include "../com/MessageSet.h"
#include "../com/CommonFunctions.h"

using namespace std;

class JNI_CALL_JAVA_ALGORITHMs {

public:

	static const string JAVA_CLASS_PATH;
	static const string JAVA_JAR_FILE_PATH;
	static const string ALG_K_SHORTESTPATH_CLASSPATH;
	static const string ALG_SHORTESTPATH_CLASSPATH;
	static const string ALG_LINK_CLASSPATH;

	JNI_CALL_JAVA_ALGORITHMs();
	virtual ~JNI_CALL_JAVA_ALGORITHMs();

//	org.graph.algorithms.shortestpath.kshortestpath.KShortestPath(java.lang.String);
//	    descriptor: (Ljava/lang/String;)V
	void JNI_CALL_JAVA_Constructor_KShortestPath_Instance(
			const string fileName);

	//	  public java.util.ArrayList<org.graph.algorithms.shortestpath.common.Path> getKShortestPath(java.lang.String, java.lang.String);
	//	    descriptor: (Ljava/lang/String;Ljava/lang/String;)Ljava/util/ArrayList;
	map<string, BasePath*> JNI_CALL_JAVA_Get_KShortestPaths(const string source,
			const string destination);

	//public java.util.ArrayList<org.graph.algorithms.shortestpath.common.Path> getKShortestPath(java.lang.String, java.lang.String, java.util.ArrayList<org.graph.algorithms.shortestpath.common.Link>);
	//    descriptor: (Ljava/lang/String;Ljava/lang/String;Ljava/util/ArrayList;)Ljava/util/ArrayList;
	map<string, BasePath*> JNI_CALL_JAVA_Get_KShortestPaths_With_GraphLinksInfo(
			const string source, const string destination,
			map<string, double> graphLinksInfo);

	map<string, BasePath*> JNI_CALL_JAVA_Get_KShortestPaths_BaseOn_GraphLinksInfo_With_HopLimit(
			const string source, const string destination,
			map<string, double> graphLinksInfo, const int hopMinLimit,
			const int hopMaxLimit);

	//	  public java.util.ArrayList<org.graph.algorithms.shortestpath.common.Path> getKShortestPathWithKLimit(java.lang.String, java.lang.String, int);
	//	    descriptor: (Ljava/lang/String;Ljava/lang/String;I)Ljava/util/ArrayList;
	map<string, BasePath*> JNI_CALL_JAVA_Get_KShortestPathsWith_KLimit(
			const string source, const string destination, const int k);

	//	  public java.util.ArrayList<org.graph.algorithms.shortestpath.common.Path> getKShortestPathWithHopMinimumLimit(java.lang.String, java.lang.String, int);
	//	    descriptor: (Ljava/lang/String;Ljava/lang/String;I)Ljava/util/ArrayList;
	map<string, BasePath*> JNI_CALL_JAVA_Get_KShortestPathsWith_HopMinimumLimit(
			const string source, const string destination,
			const int minimumHop);

	//	  public java.util.ArrayList<org.graph.algorithms.shortestpath.common.Path> getKShortestPathWithHopMaximumLimit(java.lang.String, java.lang.String, int);
	//	    descriptor: (Ljava/lang/String;Ljava/lang/String;I)Ljava/util/ArrayList;
	map<string, BasePath*> JNI_CALL_JAVA_Get_KShortestPathsWith_HopMaximumLimit(
			const string source, const string destination,
			const int maximumHop);

	//	  public java.util.ArrayList<org.graph.algorithms.shortestpath.common.Path> getKShortestPathWithHopRangeLimit(java.lang.String, java.lang.String, int, int);
	//	    descriptor: (Ljava/lang/String;Ljava/lang/String;II)Ljava/util/ArrayList;
	map<string, BasePath*> JNI_CALL_JAVA_Get_KShortestPathsWith_HopRangeLimit(
			const string source, const string destination, const int minimumHop,
			const int maximumHop);

	string Convert_JString_to_STDString(jstring jstr);
	string Convert_Integer_to_STDString(int integer);

private:
	JavaVM *jvm;                    // Pointer to the JVM (Java Virtual Machine)
	JNIEnv *env;                    // Pointer to native interface
	JavaVMInitArgs vm_args;                        // Initialization arguments
	JavaVMOption options[1];       // A list of options to build a JVM from C++

	jclass kshortestPath;
	jobject kshortestPath_instance;
};

#endif /* JNI_CALL_JAVA_ALGORITHMS_H_ */
