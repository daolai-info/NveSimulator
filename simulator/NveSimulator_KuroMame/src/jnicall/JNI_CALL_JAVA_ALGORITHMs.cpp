/*
 * JNICALLJAVA.cpp
 *
 *  Created on: 2016/11/14
 *      Author: zhangshanming
 */

#include "JNI_CALL_JAVA_ALGORITHMs.h"

using namespace std;

const string JNI_CALL_JAVA_ALGORITHMs::JAVA_CLASS_PATH =
		"-Djava.class.path=./jalgs/";
const string JNI_CALL_JAVA_ALGORITHMs::JAVA_JAR_FILE_PATH =
		"./jlibs/jgrapht-core-1.0.0.jar";

const string JNI_CALL_JAVA_ALGORITHMs::ALG_K_SHORTESTPATH_CLASSPATH =
		"org/graph/algorithms/shortestpath/kshortestpath/KShortestPath";

const string JNI_CALL_JAVA_ALGORITHMs::ALG_SHORTESTPATH_CLASSPATH =
		"org/graph/algorithms/shortestpath/common/Path";

const string JNI_CALL_JAVA_ALGORITHMs::ALG_LINK_CLASSPATH =
		"org/graph/algorithms/shortestpath/common/Link";

JNI_CALL_JAVA_ALGORITHMs::JNI_CALL_JAVA_ALGORITHMs() {
	string seprator = ":";
	string wholePath = JNI_CALL_JAVA_ALGORITHMs::JAVA_CLASS_PATH + seprator
			+ JNI_CALL_JAVA_ALGORITHMs::JAVA_JAR_FILE_PATH;
	cout << wholePath << endl;
	char *cstr = new char[wholePath.length() + 1];
	std::strcpy(cstr, wholePath.c_str());

	this->options[0].optionString = cstr;  // where to find java .class
	this->vm_args.version = JNI_VERSION_1_6;             // minimum Java version
	this->vm_args.nOptions = 1;                          // number of options
	this->vm_args.options = this->options;
	this->vm_args.ignoreUnrecognized = false; // invalid options make the JVM init fail

	//=============== load and initialize Java VM and JNI interface =============
	jint rc = JNI_CreateJavaVM(&this->jvm, (void**) &this->env, &this->vm_args); // YES !!
	if (rc != JNI_OK) {
		// TO DO: error processing...
		cerr << "ERROR: Load and Initialize Java VM and JNI Failure !!!"
				<< endl;
		exit(EXIT_FAILURE);
	}
	this->kshortestPath = NULL;
	this->kshortestPath_instance = NULL;
}

JNI_CALL_JAVA_ALGORITHMs::~JNI_CALL_JAVA_ALGORITHMs() {
	this->jvm->DestroyJavaVM();
	delete this->env;
	delete this->jvm;
}

void JNI_CALL_JAVA_ALGORITHMs::JNI_CALL_JAVA_Constructor_KShortestPath_Instance(
		const string fileName) {
	const char * class_file =
			JNI_CALL_JAVA_ALGORITHMs::ALG_K_SHORTESTPATH_CLASSPATH.c_str();
	this->kshortestPath = this->env->FindClass(class_file);
	if (this->kshortestPath == nullptr) {
		cout << class_file << endl;
		cerr << "ERROR: KShortestPath class not found !";
		exit(EXIT_FAILURE);
	}

	jmethodID constructor = this->env->GetMethodID(this->kshortestPath,
			"<init>", "(Ljava/lang/String;)V");
	if (constructor == nullptr) {
		cerr << "ERROR: The constructor method of KShortestPath  is not found !"
				<< endl;
		exit(EXIT_FAILURE);
	}

	jstring fname = this->env->NewStringUTF(fileName.c_str());
	this->kshortestPath_instance = this->env->NewObject(this->kshortestPath,
			constructor, fname);

	if (this->kshortestPath_instance == 0) {
		cerr << "ERROR: The instance initialization of KShortestPath  failure !"
				<< endl;
		exit(EXIT_FAILURE);
	}

}

map<string, BasePath*> JNI_CALL_JAVA_ALGORITHMs::JNI_CALL_JAVA_Get_KShortestPaths(
		const string source, const string destination) {
	cout << " FuncInfo: JNI_CALL_JAVA_Get_KShortestPaths perform Start" << endl;
	jstring src = this->env->NewStringUTF(source.c_str());
	jstring dest = this->env->NewStringUTF(destination.c_str());

	jclass java_util_ArrayList = this->env->FindClass("java/util/ArrayList");
	jmethodID java_util_ArrayList_size = this->env->GetMethodID(
			java_util_ArrayList, "size", "()I");
	jmethodID java_util_ArrayList_get = this->env->GetMethodID(
			java_util_ArrayList, "get", "(I)Ljava/lang/Object;");

	jclass path_class = this->env->FindClass(
			JNI_CALL_JAVA_ALGORITHMs::ALG_SHORTESTPATH_CLASSPATH.c_str());
	jmethodID get_path_length = this->env->GetMethodID(path_class, "getLength",
			"()I");
	jmethodID get_node_info = this->env->GetMethodID(path_class, "getNodeInfo",
			"()Ljava/util/ArrayList;");
	jmethodID get_path_cost = this->env->GetMethodID(path_class, "getCost",
			"()D");

	jmethodID getKsPaths = this->env->GetMethodID(this->kshortestPath,
			"getKShortestPath",
			"(Ljava/lang/String;Ljava/lang/String;)Ljava/util/ArrayList;");
	if (getKsPaths == nullptr) {
		cerr << "ERROR: The getKShortestPath of KShortestPath  not found !"
				<< endl;
		exit(EXIT_FAILURE);
	}

	map<string, BasePath*> kPaths;
	string IdHead = BasePath::makePathId(source, destination);

	jobject resultPaths = this->env->CallObjectMethod(
			this->kshortestPath_instance, getKsPaths, src, dest);

	jint pSize = this->env->CallIntMethod(resultPaths,
			java_util_ArrayList_size);
	if (pSize == 0) {
		cout
				<< "INFO: The result of JNI_CALL_JAVA_Get_KShortestPaths  is zero !"
				<< endl;
		return kPaths;
	}

	string pathId = "";
	for (jint pIndex = 0; pIndex < pSize; ++pIndex) {
		jobject aPath = this->env->CallObjectMethod(resultPaths,
				java_util_ArrayList_get, pIndex);

		pathId = BasePath::makePathId(IdHead,
				Convert_Integer_to_STDString((int) pIndex));

		vector<string> pathInfo;
		jobject nodes = this->env->CallObjectMethod(aPath, get_node_info);
		jint nodeSize = this->env->CallIntMethod(nodes,
				java_util_ArrayList_size);
		for (jint nIndex = 0; nIndex < nodeSize; ++nIndex) {
			jstring aNode = (jstring) this->env->CallObjectMethod(nodes,
					java_util_ArrayList_get, nIndex);
			pathInfo.push_back(Convert_JString_to_STDString(aNode));
		}

		jint length = this->env->CallIntMethod(aPath, get_path_length);
		jdouble cost = this->env->CallDoubleMethod(aPath, get_path_cost);

		map<string, BasePath*>::iterator pIter = kPaths.begin();
		kPaths.insert(pIter,
				pair<string, BasePath*>(pathId,
						new BasePath(BasePath::makePathId(source, destination),
								source, destination, (double) cost,
								(int) length, pathInfo)));
		pathId = "";
	}

	cout << " FuncInfo: JNI_CALL_JAVA_Get_KShortestPaths perform End" << endl;
	return kPaths;
}

//public java.util.ArrayList<org.graph.algorithms.shortestpath.common.Path> getKShortestPath(java.lang.String, java.lang.String, java.util.ArrayList<org.graph.algorithms.shortestpath.common.Link>);
//    descriptor: (Ljava/lang/String;Ljava/lang/String;Ljava/util/ArrayList;)Ljava/util/ArrayList;
map<string, BasePath*> JNI_CALL_JAVA_ALGORITHMs::JNI_CALL_JAVA_Get_KShortestPaths_With_GraphLinksInfo(
		const string source, const string destination,
		map<string, double> graphLinksInfo) {

	MessageSet::printInfoMessage(
			" FuncInfo: JNI_CALL_JAVA_Get_KShortestPaths_With_GraphLinksInfo perform Start.");

	jstring src = this->env->NewStringUTF(source.c_str());
	jstring dest = this->env->NewStringUTF(destination.c_str());

	jclass java_util_ArrayList = this->env->FindClass("java/util/ArrayList");
	if (java_util_ArrayList == nullptr) {
		cout << java_util_ArrayList << endl;
		cerr << "ERROR: java_util_ArrayList class not found !";
		exit(EXIT_FAILURE);
	}

	jmethodID java_util_ArrayList_size = this->env->GetMethodID(
			java_util_ArrayList, "size", "()I");
	if (java_util_ArrayList_size == nullptr) {
		cerr << "ERROR: The java_util_ArrayList_size  is not found !" << endl;
		exit(EXIT_FAILURE);
	}

	jmethodID java_util_ArrayList_get = this->env->GetMethodID(
			java_util_ArrayList, "get", "(I)Ljava/lang/Object;");
	if (java_util_ArrayList_get == nullptr) {
		cerr << "ERROR: The java_util_ArrayList_get is not found !" << endl;
		exit(EXIT_FAILURE);
	}

	jmethodID java_util_ArrayList_add = this->env->GetMethodID(
			java_util_ArrayList, "add", "(Ljava/lang/Object;)Z");
	if (java_util_ArrayList_add == nullptr) {
		cerr << "ERROR: The java_util_ArrayList_add is not found !" << endl;
		exit(EXIT_FAILURE);
	}

	jmethodID java_util_ArrayList_constructor = this->env->GetMethodID(
			java_util_ArrayList, "<init>", "()V");
	if (java_util_ArrayList_constructor == nullptr) {
		cerr << "ERROR: The java_util_ArrayList_constructor  is not found !"
				<< endl;
		exit(EXIT_FAILURE);
	}

	jclass link_class = this->env->FindClass(
			JNI_CALL_JAVA_ALGORITHMs::ALG_LINK_CLASSPATH.c_str());

	if (link_class == nullptr) {
		cout << link_class << endl;
		cerr << "ERROR: Link class not found !";
		exit(EXIT_FAILURE);
	}

	jobject j_LinksInfo_ArrayList__instance = this->env->NewObject(
			java_util_ArrayList, java_util_ArrayList_constructor);

	for (map<string, double>::iterator linkIter = graphLinksInfo.begin();
			linkIter != graphLinksInfo.end(); ++linkIter) {

		jmethodID link_constructor = this->env->GetMethodID(link_class,
				"<init>", "(Ljava/lang/String;Ljava/lang/String;D)V");
		if (link_constructor == nullptr) {
			cerr << "ERROR: The constructor method of Link  is not found !"
					<< endl;
			exit(EXIT_FAILURE);
		}

		// get the two virtual nodes of a virtual link
		vector<string> fromTo = CommonFunctions::stringSplit(linkIter->first,
				'_');
		string linkFrom = fromTo.at(0);
		string linkTo = fromTo.at(1);
		double cost = linkIter->second;

		jstring jLinkFrom = this->env->NewStringUTF(linkFrom.c_str());
		jstring jLinkTo = this->env->NewStringUTF(linkTo.c_str());
		jdouble jCost = (jdouble) cost;

		jobject link_instance = this->env->NewObject(link_class,
				link_constructor, jLinkFrom, jLinkTo, jCost);

		if (link_instance == 0) {
			cerr << "ERROR: The instance initialization of Link  failure !"
					<< endl;
			exit(EXIT_FAILURE);
		}

		jboolean resultFlag = this->env->CallBooleanMethod(
				j_LinksInfo_ArrayList__instance, java_util_ArrayList_add,
				link_instance);
		if (resultFlag == false) {
			cerr << "ERROR: The ArrayList's add function performs  failure !"
					<< endl;
			exit(EXIT_FAILURE);
		}
	}

	jclass path_class = this->env->FindClass(
			JNI_CALL_JAVA_ALGORITHMs::ALG_SHORTESTPATH_CLASSPATH.c_str());
	jmethodID get_path_length = this->env->GetMethodID(path_class, "getLength",
			"()I");
	jmethodID get_node_info = this->env->GetMethodID(path_class, "getNodeInfo",
			"()Ljava/util/ArrayList;");
	jmethodID get_path_cost = this->env->GetMethodID(path_class, "getCost",
			"()D");

	jmethodID getKsPaths =
			this->env->GetMethodID(this->kshortestPath,
					"getKShortestPathWithGraphLinksInfo",
					"(Ljava/lang/String;Ljava/lang/String;Ljava/util/ArrayList;)Ljava/util/ArrayList;");

	if (getKsPaths == nullptr) {
		cerr
				<< "ERROR: The getKShortestPathWithGraphLinksInfo of KShortestPath  not found !"
				<< endl;
		exit(EXIT_FAILURE);
	}

	map<string, BasePath*> kPaths;
	string IdHead = CommonFunctions::makePathId(source, destination);

	jobject resultPaths = this->env->CallObjectMethod(
			this->kshortestPath_instance, getKsPaths, src, dest,
			j_LinksInfo_ArrayList__instance);

	jint pSize = this->env->CallIntMethod(resultPaths,
			java_util_ArrayList_size);
	if (pSize == 0) {
		cout
				<< "INFO: The result of JNI_CALL_JAVA_Get_KShortestPaths  is zero !"
				<< endl;
		return kPaths;
	}

	string pathId = "";
	for (jint pIndex = 0; pIndex < pSize; ++pIndex) {
		jobject aPath = this->env->CallObjectMethod(resultPaths,
				java_util_ArrayList_get, pIndex);

		pathId = CommonFunctions::makePathId(IdHead,
				CommonFunctions::CoverIntegerToString((int) pIndex));

		vector<string> pathInfo;
		jobject nodes = this->env->CallObjectMethod(aPath, get_node_info);
		jint nodeSize = this->env->CallIntMethod(nodes,
				java_util_ArrayList_size);
		for (jint nIndex = 0; nIndex < nodeSize; ++nIndex) {
			jstring aNode = (jstring) this->env->CallObjectMethod(nodes,
					java_util_ArrayList_get, nIndex);
			pathInfo.push_back(Convert_JString_to_STDString(aNode));
		}

		jint hops = this->env->CallIntMethod(aPath, get_path_length);
		jdouble cost = this->env->CallDoubleMethod(aPath, get_path_cost);

		map<string, BasePath*>::iterator pIter = kPaths.begin();
		kPaths.insert(pIter,
				pair<string, BasePath*>(pathId,
						new BasePath(
								CommonFunctions::makePathId(source,
										destination), source, destination, 0.0,
								(double) cost, (int) hops, pathInfo)));
		pathId = "";
	}

	MessageSet::printInfoMessage(
			" FuncInfo: JNI_CALL_JAVA_Get_KShortestPaths_With_GraphLinksInfo perform End.");

	return kPaths;
}

map<string, BasePath*> JNI_CALL_JAVA_ALGORITHMs::JNI_CALL_JAVA_Get_KShortestPaths_BaseOn_GraphLinksInfo_With_HopLimit(
		const string source, const string destination,
		map<string, double> graphLinksInfo, const int minimumHop,
		const int maximumHop) {
	MessageSet::printInfoMessage(
			" FuncInfo: JNI_CALL_JAVA_Get_KShortestPaths_BaseOn_GraphLinksInfo_With_HopLimit perform Start.");

	jstring src = this->env->NewStringUTF(source.c_str());
	jstring dest = this->env->NewStringUTF(destination.c_str());

	jint maxHopLimit = (jint) maximumHop;
	jint minHopLimit = (jint) minimumHop;

	jclass java_util_ArrayList = this->env->FindClass("java/util/ArrayList");
	if (java_util_ArrayList == nullptr) {
		cout << java_util_ArrayList << endl;
		cerr << "ERROR: java_util_ArrayList class not found !";
		exit(EXIT_FAILURE);
	}

	jmethodID java_util_ArrayList_size = this->env->GetMethodID(
			java_util_ArrayList, "size", "()I");
	if (java_util_ArrayList_size == nullptr) {
		cerr << "ERROR: The java_util_ArrayList_size  is not found !" << endl;
		exit(EXIT_FAILURE);
	}

	jmethodID java_util_ArrayList_get = this->env->GetMethodID(
			java_util_ArrayList, "get", "(I)Ljava/lang/Object;");
	if (java_util_ArrayList_get == nullptr) {
		cerr << "ERROR: The java_util_ArrayList_get is not found !" << endl;
		exit(EXIT_FAILURE);
	}

	jmethodID java_util_ArrayList_add = this->env->GetMethodID(
			java_util_ArrayList, "add", "(Ljava/lang/Object;)Z");
	if (java_util_ArrayList_add == nullptr) {
		cerr << "ERROR: The java_util_ArrayList_add is not found !" << endl;
		exit(EXIT_FAILURE);
	}

	jmethodID java_util_ArrayList_constructor = this->env->GetMethodID(
			java_util_ArrayList, "<init>", "()V");
	if (java_util_ArrayList_constructor == nullptr) {
		cerr << "ERROR: The java_util_ArrayList_constructor  is not found !"
				<< endl;
		exit(EXIT_FAILURE);
	}

	jclass link_class = this->env->FindClass(
			JNI_CALL_JAVA_ALGORITHMs::ALG_LINK_CLASSPATH.c_str());

	if (link_class == nullptr) {
		cout << link_class << endl;
		cerr << "ERROR: Link class not found !";
		exit(EXIT_FAILURE);
	}

	jobject j_LinksInfo_ArrayList__instance = this->env->NewObject(
			java_util_ArrayList, java_util_ArrayList_constructor);

	for (map<string, double>::iterator linkIter = graphLinksInfo.begin();
			linkIter != graphLinksInfo.end(); ++linkIter) {

		jmethodID link_constructor = this->env->GetMethodID(link_class,
				"<init>", "(Ljava/lang/String;Ljava/lang/String;D)V");
		if (link_constructor == nullptr) {
			cerr << "ERROR: The constructor method of Link  is not found !"
					<< endl;
			exit(EXIT_FAILURE);
		}

		// get the two virtual nodes of a virtual link
		vector<string> fromTo = CommonFunctions::stringSplit(linkIter->first,
				'_');
		string linkFrom = fromTo.at(0);
		string linkTo = fromTo.at(1);
		double cost = linkIter->second;

		jstring jLinkFrom = this->env->NewStringUTF(linkFrom.c_str());
		jstring jLinkTo = this->env->NewStringUTF(linkTo.c_str());
		jdouble jCost = (jdouble) cost;

		jobject link_instance = this->env->NewObject(link_class,
				link_constructor, jLinkFrom, jLinkTo, jCost);

		if (link_instance == 0) {
			cerr << "ERROR: The instance initialization of Link  failure !"
					<< endl;
			exit(EXIT_FAILURE);
		}

		jboolean resultFlag = this->env->CallBooleanMethod(
				j_LinksInfo_ArrayList__instance, java_util_ArrayList_add,
				link_instance);
		if (resultFlag == false) {
			cerr << "ERROR: The ArrayList's add function performs  failure !"
					<< endl;
			exit(EXIT_FAILURE);
		}
	}

	jclass path_class = this->env->FindClass(
			JNI_CALL_JAVA_ALGORITHMs::ALG_SHORTESTPATH_CLASSPATH.c_str());
	jmethodID get_path_length = this->env->GetMethodID(path_class, "getLength",
			"()I");
	jmethodID get_node_info = this->env->GetMethodID(path_class, "getNodeInfo",
			"()Ljava/util/ArrayList;");
	jmethodID get_path_cost = this->env->GetMethodID(path_class, "getCost",
			"()D");

	jmethodID getKsPaths =
			this->env->GetMethodID(this->kshortestPath,
					"getKShortestPathBaseOnGraphLinksInfoWithHopLimit",
					"(Ljava/lang/String;Ljava/lang/String;Ljava/util/ArrayList;II)Ljava/util/ArrayList;");
	if (getKsPaths == nullptr) {
		cerr
				<< "ERROR: The getKShortestPathBaseOnGraphLinksInfoWithHopLimit of KShortestPath  not found !"
				<< endl;
		exit(EXIT_FAILURE);
	}

	map<string, BasePath*> kPaths;
	string IdHead = CommonFunctions::makePathId(source, destination);

	jobject resultPaths = this->env->CallObjectMethod(
			this->kshortestPath_instance, getKsPaths, src, dest,
			j_LinksInfo_ArrayList__instance, minHopLimit, maxHopLimit);

	jint pSize = this->env->CallIntMethod(resultPaths,
			java_util_ArrayList_size);
	if (pSize == 0) {
		cout
				<< "INFO: The result of JNI_CALL_JAVA_Get_KShortestPaths  is zero !"
				<< endl;
		return kPaths;
	}

	string pathId = "";
	for (jint pIndex = 0; pIndex < pSize; ++pIndex) {
		jobject aPath = this->env->CallObjectMethod(resultPaths,
				java_util_ArrayList_get, pIndex);

		pathId = CommonFunctions::makePathId(IdHead,
				CommonFunctions::CoverIntegerToString((int) pIndex));

		vector<string> pathInfo;
		jobject nodes = this->env->CallObjectMethod(aPath, get_node_info);
		jint nodeSize = this->env->CallIntMethod(nodes,
				java_util_ArrayList_size);
		for (jint nIndex = 0; nIndex < nodeSize; ++nIndex) {
			jstring aNode = (jstring) this->env->CallObjectMethod(nodes,
					java_util_ArrayList_get, nIndex);
			pathInfo.push_back(Convert_JString_to_STDString(aNode));
		}

		jint hops = this->env->CallIntMethod(aPath, get_path_length);
		jdouble cost = this->env->CallDoubleMethod(aPath, get_path_cost);

		map<string, BasePath*>::iterator pIter = kPaths.begin();
		kPaths.insert(pIter,
				pair<string, BasePath*>(pathId,
						new BasePath(
								CommonFunctions::makePathId(source,
										destination), source, destination, 0.0,
								(double) cost, (int) hops, pathInfo)));
		pathId = "";
	}

	MessageSet::printInfoMessage(
			" FuncInfo: JNI_CALL_JAVA_Get_KShortestPaths_BaseOn_GraphLinksInfo_With_HopLimit perform End.");

	return kPaths;
}

map<string, BasePath*> JNI_CALL_JAVA_ALGORITHMs::JNI_CALL_JAVA_Get_KShortestPathsWith_KLimit(
		const string source, const string destination, const int k) {
	cout
			<< " Funcation: JNI_CALL_JAVA_Get_KShortestPathsWith_KLimit perform Start"
			<< endl;
	jstring src = this->env->NewStringUTF(source.c_str());
	jstring dest = this->env->NewStringUTF(destination.c_str());
	jint kLimit = (jint) k;
	jclass java_util_ArrayList = this->env->FindClass("java/util/ArrayList");
	jmethodID java_util_ArrayList_size = this->env->GetMethodID(
			java_util_ArrayList, "size", "()I");
	jmethodID java_util_ArrayList_get = this->env->GetMethodID(
			java_util_ArrayList, "get", "(I)Ljava/lang/Object;");

	jclass path_class = this->env->FindClass(
			JNI_CALL_JAVA_ALGORITHMs::ALG_SHORTESTPATH_CLASSPATH.c_str());
	jmethodID get_path_length = this->env->GetMethodID(path_class, "getLength",
			"()I");
	jmethodID get_node_info = this->env->GetMethodID(path_class, "getNodeInfo",
			"()Ljava/util/ArrayList;");
	jmethodID get_path_cost = this->env->GetMethodID(path_class, "getCost",
			"()D");

	jmethodID getKsPaths = this->env->GetMethodID(this->kshortestPath,
			"getKShortestPathWithKLimit",
			"(Ljava/lang/String;Ljava/lang/String;I)Ljava/util/ArrayList;");
	if (getKsPaths == nullptr) {
		cerr << "ERROR: The getKShortestPath of KShortestPath  not found !"
				<< endl;
		exit(EXIT_FAILURE);
	}

	map<string, BasePath*> kPaths;
	string IdHead = BasePath::makePathId(source, destination);

	jobject resultPaths = this->env->CallObjectMethod(
			this->kshortestPath_instance, getKsPaths, src, dest, kLimit);

	jint pSize = this->env->CallIntMethod(resultPaths,
			java_util_ArrayList_size);

	if (pSize == 0) {
		cout
				<< "INFO: The result of JNI_CALL_JAVA_Get_KShortestPathsWith_KLimit  is zero !"
				<< endl;
		return kPaths;
	}

	string pathId = "";
	for (jint pIndex = 0; pIndex < pSize; ++pIndex) {
		jobject aPath = this->env->CallObjectMethod(resultPaths,
				java_util_ArrayList_get, pIndex);
		jobject nodes = this->env->CallObjectMethod(aPath, get_node_info);
		jint nodeSize = this->env->CallIntMethod(nodes,
				java_util_ArrayList_size);

		pathId = BasePath::makePathId(IdHead,
				Convert_Integer_to_STDString((int) pIndex));

		vector<string> pathInfo;

		for (jint nIndex = 0; nIndex < nodeSize; ++nIndex) {
			jstring aNode = (jstring) this->env->CallObjectMethod(nodes,
					java_util_ArrayList_get, nIndex);
			pathInfo.push_back(Convert_JString_to_STDString(aNode));
		}

		jint length = this->env->CallIntMethod(aPath, get_path_length);
		jdouble cost = this->env->CallDoubleMethod(aPath, get_path_cost);
		map<string, BasePath*>::iterator pIter = kPaths.begin();
		kPaths.insert(pIter,
				pair<string, BasePath*>(pathId,
						new BasePath(BasePath::makePathId(source, destination),
								source, destination, (double) cost,
								(int) length, pathInfo)));
		pathId = "";

	}

	cout
			<< " Funcation: JNI_CALL_JAVA_Get_KShortestPathsWith_KLimit perform End"
			<< endl;
	return kPaths;
}

map<string, BasePath*> JNI_CALL_JAVA_ALGORITHMs::JNI_CALL_JAVA_Get_KShortestPathsWith_HopMinimumLimit(
		const string source, const string destination, const int minimumHop) {
	cout
			<< " Funcation: JNI_CALL_JAVA_Get_KShortestPathsWith_HopMinimumLimit perform Start"
			<< endl;
	jstring src = this->env->NewStringUTF(source.c_str());
	jstring dest = this->env->NewStringUTF(destination.c_str());
	jint minHopLimit = (jint) minimumHop;

	jclass java_util_ArrayList = this->env->FindClass("java/util/ArrayList");
	jmethodID java_util_ArrayList_size = this->env->GetMethodID(
			java_util_ArrayList, "size", "()I");
	jmethodID java_util_ArrayList_get = this->env->GetMethodID(
			java_util_ArrayList, "get", "(I)Ljava/lang/Object;");

	jclass path_class = this->env->FindClass(
			JNI_CALL_JAVA_ALGORITHMs::ALG_SHORTESTPATH_CLASSPATH.c_str());
	jmethodID get_path_length = this->env->GetMethodID(path_class, "getLength",
			"()I");
	jmethodID get_node_info = this->env->GetMethodID(path_class, "getNodeInfo",
			"()Ljava/util/ArrayList;");
	jmethodID get_path_cost = this->env->GetMethodID(path_class, "getCost",
			"()D");

	jmethodID getKsPaths = this->env->GetMethodID(this->kshortestPath,
			"getKShortestPathWithHopMinimumLimit",
			"(Ljava/lang/String;Ljava/lang/String;I)Ljava/util/ArrayList;");
	if (getKsPaths == nullptr) {
		cerr << "ERROR: The getKShortestPath of KShortestPath  not found !"
				<< endl;
		exit(EXIT_FAILURE);
	}

	map<string, BasePath*> kPaths;
	string IdHead = BasePath::makePathId(source, destination);

	jobject resultPaths = this->env->CallObjectMethod(
			this->kshortestPath_instance, getKsPaths, src, dest, minHopLimit);
	cout << "--------" << endl;
	jint pSize = this->env->CallIntMethod(resultPaths,
			java_util_ArrayList_size);

	if (pSize == 0) {
		cout
				<< "INFO: The result of JNI_CALL_JAVA_Get_KShortestPaths  is zero !"
				<< endl;
		return kPaths;
	}

	string pathId = "";
	for (jint pIndex = 0; pIndex < pSize; ++pIndex) {
		jobject aPath = this->env->CallObjectMethod(resultPaths,
				java_util_ArrayList_get, pIndex);
		jobject nodes = this->env->CallObjectMethod(aPath, get_node_info);
		jint nodeSize = this->env->CallIntMethod(nodes,
				java_util_ArrayList_size);

		pathId = BasePath::makePathId(IdHead,
				Convert_Integer_to_STDString((int) pIndex));

		vector<string> pathInfo;

		cout << "nodes : " << endl;
		for (jint nIndex = 0; nIndex < nodeSize; ++nIndex) {
			jstring aNode = (jstring) this->env->CallObjectMethod(nodes,
					java_util_ArrayList_get, nIndex);

			pathInfo.push_back(Convert_JString_to_STDString(aNode));
			cout << aNode << " ";
		}

		jint length = this->env->CallIntMethod(aPath, get_path_length);
		jdouble cost = this->env->CallDoubleMethod(aPath, get_path_cost);

		map<string, BasePath*>::iterator pIter = kPaths.begin();
		kPaths.insert(pIter,
				pair<string, BasePath*>(pathId,
						new BasePath(BasePath::makePathId(source, destination),
								source, destination, (double) cost,
								(int) length, pathInfo)));
		cout << ";  length :" << length << "; cost: " << cost << endl;
		pathId = "";
	}

	cout
			<< " Funcation: JNI_CALL_JAVA_Get_KShortestPathsWith_HopMinimumLimit perform End"
			<< endl;
	return kPaths;
}

map<string, BasePath*> JNI_CALL_JAVA_ALGORITHMs::JNI_CALL_JAVA_Get_KShortestPathsWith_HopMaximumLimit(
		const string source, const string destination, const int maximumHop) {

	cout
			<< " Funcation: JNI_CALL_JAVA_Get_KShortestPathsWith_HopMaximumLimit perform Start"
			<< endl;
	jstring src = this->env->NewStringUTF(source.c_str());
	jstring dest = this->env->NewStringUTF(destination.c_str());
	jint maxHopLimit = (jint) maximumHop;

	jclass java_util_ArrayList = this->env->FindClass("java/util/ArrayList");
	jmethodID java_util_ArrayList_size = this->env->GetMethodID(
			java_util_ArrayList, "size", "()I");
	jmethodID java_util_ArrayList_get = this->env->GetMethodID(
			java_util_ArrayList, "get", "(I)Ljava/lang/Object;");

	jclass path_class = this->env->FindClass(
			JNI_CALL_JAVA_ALGORITHMs::ALG_SHORTESTPATH_CLASSPATH.c_str());
	jmethodID get_path_length = this->env->GetMethodID(path_class, "getLength",
			"()I");
	jmethodID get_node_info = this->env->GetMethodID(path_class, "getNodeInfo",
			"()Ljava/util/ArrayList;");
	jmethodID get_path_cost = this->env->GetMethodID(path_class, "getCost",
			"()D");

	jmethodID getKsPaths = this->env->GetMethodID(this->kshortestPath,
			"getKShortestPathWithHopMaximumLimit",
			"(Ljava/lang/String;Ljava/lang/String;I)Ljava/util/ArrayList;");
	if (getKsPaths == nullptr) {
		cerr << "ERROR: The getKShortestPath of KShortestPath  not found !"
				<< endl;
		exit(EXIT_FAILURE);
	}

	map<string, BasePath*> kPaths;
	string IdHead = BasePath::makePathId(source, destination);

	jobject resultPaths = this->env->CallObjectMethod(
			this->kshortestPath_instance, getKsPaths, src, dest, maxHopLimit);
	cout << "--------" << endl;
	jint pSize = this->env->CallIntMethod(resultPaths,
			java_util_ArrayList_size);

	if (pSize == 0) {
		cout
				<< "INFO: The result of JNI_CALL_JAVA_Get_KShortestPaths  is zero !"
				<< endl;
		return kPaths;
	}

	string pathId = "";
	for (jint pIndex = 0; pIndex < pSize; ++pIndex) {
		jobject aPath = this->env->CallObjectMethod(resultPaths,
				java_util_ArrayList_get, pIndex);
		jobject nodes = this->env->CallObjectMethod(aPath, get_node_info);
		jint nodeSize = this->env->CallIntMethod(nodes,
				java_util_ArrayList_size);

		pathId = BasePath::makePathId(IdHead,
				Convert_Integer_to_STDString((int) pIndex));

		vector<string> pathInfo;

		cout << "nodes : " << endl;
		for (jint nIndex = 0; nIndex < nodeSize; ++nIndex) {
			jstring aNode = (jstring) this->env->CallObjectMethod(nodes,
					java_util_ArrayList_get, nIndex);

			pathInfo.push_back(Convert_JString_to_STDString(aNode));
			cout << aNode << " ";
		}

		jint length = this->env->CallIntMethod(aPath, get_path_length);
		jdouble cost = this->env->CallDoubleMethod(aPath, get_path_cost);

		map<string, BasePath*>::iterator pIter = kPaths.begin();
		kPaths.insert(pIter,
				pair<string, BasePath*>(pathId,
						new BasePath(BasePath::makePathId(source, destination),
								source, destination, (double) cost,
								(int) length, pathInfo)));
		cout << ";  length :" << length << "; cost: " << cost << endl;
		pathId = "";
	}

	cout
			<< " Funcation: JNI_CALL_JAVA_Get_KShortestPathsWith_HopMaximumLimit perform End"
			<< endl;
	return kPaths;
}

map<string, BasePath*> JNI_CALL_JAVA_ALGORITHMs::JNI_CALL_JAVA_Get_KShortestPathsWith_HopRangeLimit(
		const string source, const string destination, const int minimumHop,
		const int maximumHop) {

	MessageSet::printInfoMessage(
			" Funcation: JNI_CALL_JAVA_Get_KShortestPathsWith_HopRangeLimit perform Start ");

	jstring src = this->env->NewStringUTF(source.c_str());
	jstring dest = this->env->NewStringUTF(destination.c_str());
	jint maxHopLimit = (jint) maximumHop;
	jint minHopLimit = (jint) minimumHop;

	jclass java_util_ArrayList = this->env->FindClass("java/util/ArrayList");
	jmethodID java_util_ArrayList_size = this->env->GetMethodID(
			java_util_ArrayList, "size", "()I");
	jmethodID java_util_ArrayList_get = this->env->GetMethodID(
			java_util_ArrayList, "get", "(I)Ljava/lang/Object;");

	jclass path_class = this->env->FindClass(
			JNI_CALL_JAVA_ALGORITHMs::ALG_SHORTESTPATH_CLASSPATH.c_str());
	jmethodID get_path_length = this->env->GetMethodID(path_class, "getLength",
			"()I");
	jmethodID get_node_info = this->env->GetMethodID(path_class, "getNodeInfo",
			"()Ljava/util/ArrayList;");
	jmethodID get_path_cost = this->env->GetMethodID(path_class, "getCost",
			"()D");

	jmethodID getKsPaths = this->env->GetMethodID(this->kshortestPath,
			"getKShortestPathWithHopRangeLimit",
			"(Ljava/lang/String;Ljava/lang/String;II)Ljava/util/ArrayList;");
	if (getKsPaths == nullptr) {
		cerr << "ERROR: The getKShortestPath of KShortestPath  not found !"
				<< endl;
		MessageSet::printErrorMessage(
				"ERROR: The getKShortestPath of KShortestPath  not found !");

		exit(EXIT_FAILURE);
	}

	map<string, BasePath*> kPaths;
	string IdHead = BasePath::makePathId(source, destination);

	jobject resultPaths = this->env->CallObjectMethod(
			this->kshortestPath_instance, getKsPaths, src, dest, minHopLimit,
			maxHopLimit);
	cout << "--------" << endl;
	jint pSize = this->env->CallIntMethod(resultPaths,
			java_util_ArrayList_size);

	if (pSize == 0) {
		MessageSet::printWarningMessage(
				"INFO: The result of JNI_CALL_JAVA_Get_KShortestPaths  is zero !");

		return kPaths;
	}

	string pathId = "";
	for (jint pIndex = 0; pIndex < pSize; ++pIndex) {
		jobject aPath = this->env->CallObjectMethod(resultPaths,
				java_util_ArrayList_get, pIndex);
		jobject nodes = this->env->CallObjectMethod(aPath, get_node_info);
		jint nodeSize = this->env->CallIntMethod(nodes,
				java_util_ArrayList_size);
		//cout << "nodes : " << endl;

		pathId = BasePath::makePathId(IdHead,
				Convert_Integer_to_STDString((int) pIndex));

		vector<string> pathInfo;

		for (jint nIndex = 0; nIndex < nodeSize; ++nIndex) {
			jstring aNode = (jstring) this->env->CallObjectMethod(nodes,
					java_util_ArrayList_get, nIndex);
			//cout << aNode << " ";
			pathInfo.push_back(Convert_JString_to_STDString(aNode));
		}

		jint length = this->env->CallIntMethod(aPath, get_path_length);
		jdouble cost = this->env->CallDoubleMethod(aPath, get_path_cost);
		map<string, BasePath*>::iterator pIter = kPaths.begin();
		kPaths.insert(pIter,
				pair<string, BasePath*>(pathId,
						new BasePath(BasePath::makePathId(source, destination),
								source, destination, (double) cost,
								(int) length, pathInfo)));
		//cout << ";  length :" << length << "; cost: " << cost << endl;
		pathId = "";
	}

	MessageSet::printInfoMessage(
			" Funcation: JNI_CALL_JAVA_Get_KShortestPathsWith_HopRangeLimit perform End. ");

	return kPaths;
}

string JNI_CALL_JAVA_ALGORITHMs::Convert_JString_to_STDString(jstring jstr) {
	const char* cstr = this->env->GetStringUTFChars(jstr, 0);
	char *str = strdup(cstr);
	this->env->ReleaseStringUTFChars(jstr, cstr);

	stringstream sstr;
	sstr << str;
	return sstr.str();
}

string JNI_CALL_JAVA_ALGORITHMs::Convert_Integer_to_STDString(int integer) {
	stringstream ss;
	ss << integer;
	return ss.str();
}
