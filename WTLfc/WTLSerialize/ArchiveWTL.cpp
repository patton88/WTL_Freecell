// ArchiveWTL.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include "Archive.h"
#include "Driver.h"
#include "Mytests.h"

CAppModule _Module;

int main(int argc, char* argv[])
{
	wstring strFile = L"demo.txt";
	
	// Storing
	FILE* pFile1 = _wfopen(strFile.c_str(), L"wb");
	CXArchive ar1(pFile1, CXArchive::store);

	int n = 6;
	CTvec<> ctvec1;	// 模板参数有默认值也需要加上尖括号<>
	for (int i = 0; i < n; i++)
		ctvec1.m_tVec.push_back(i * 2);
	dispt(ctvec1.m_tVec);
	ctvec1.Serialize(ar1);

	ar1.Close();

	// Loading
	FILE* pFile2 = _wfopen(strFile.c_str(), L"rb");
	CXArchive ar2(pFile2, CXArchive::load);

	CTvec<> ctvec2;	// 模板参数有默认值也需要加上尖括号<>
	ctvec2.Serialize(ar2);
	dispt(ctvec2.m_tVec);

	ar2.Close();

	system("pause");
	return 0;
}

