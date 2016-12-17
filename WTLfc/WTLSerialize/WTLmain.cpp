// ArchiveWTL.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include "File.h"
#include "Archive.h"
#include <list>

CAppModule _Module;

class Point
{
public:
	int x, y;
	Point(int xx = 0, int yy = 0) : x(xx), y(yy)
	{ }
	
	void showXY()
	{ cout << "(" << x << ", " << y << ")" << endl; }

	void Serialize(CXArchive& ar)
	{
		if (ar.IsStoring())
		{
			ar << x << y;
		}
		else
		{
			ar >> x >> y;
		}
	}
};

class Points
{
public:
	list<Point> listPt;
	Points(int n = 0) { listPt.resize(n); }

	void Serialize(CXArchive& ar)
	{
		if (ar.IsStoring())
		{ ar << listPt.size(); }
		else
		{ int n;	ar >> n;	listPt.resize(n); }

		for (list<Point>::iterator it = listPt.begin(); it != listPt.end(); it++)
			(*it).Serialize(ar);
	}

	void disp()
	{
		for (list<Point>::iterator it = listPt.begin(); it != listPt.end(); it++)
			(*it).showXY();
	}
};

int main(int argc, char* argv[])
{
	//cout << "Hello WTL." << endl;

	wstring strFile = _T("demo.txt");
	
	// Storing
	CXFile file1;
	file1.Open(strFile,							// file name
		GENERIC_WRITE | GENERIC_READ,			// access mode 
		FILE_SHARE_READ | FILE_SHARE_WRITE,		// share mode 
		NULL,									// no security 
		CREATE_ALWAYS,							// create a new file, overwrite if it exists
		FILE_ATTRIBUTE_NORMAL,					// file attributes
		NULL);									// no template file
	
	CXArchive ar1(&file1, CXArchive::store);
	Points pts1(6);
	int i = 0;
	for (list<Point>::iterator it = pts1.listPt.begin(); it != pts1.listPt.end(); it++, i++)
	{
		(*it).x = i * 2; (*it).y = i * 3;
	}
	pts1.Serialize(ar1);
	ar1.Close();

	pts1.disp();
	//return 0;

	// Loading
	CXFile file2;
	file2.Open(strFile,							// file name
		GENERIC_WRITE | GENERIC_READ,			// access mode 
		FILE_SHARE_READ | FILE_SHARE_WRITE,		// share mode 
		NULL,									// no security 
		//CREATE_ALWAYS,						// create a new file, overwrite if it exists
		OPEN_EXISTING,							// open the file, if it exists
		FILE_ATTRIBUTE_NORMAL,					// file attributes
		NULL);									// no template file

	CXArchive ar2(&file2, CXArchive::load);
	//Points pts2(6);
	Points pts2;
	pts2.Serialize(ar2);
	ar2.Close();

	pts2.disp();

	system("pause");
	return 0;
}

//(0, 0)
//(2, 3)
//(4, 6)
//(6, 9)
//(8, 12)
//(10, 15)
//
//(0, 0)
//(2, 3)
//(4, 6)
//(6, 9)
//(8, 12)
//(10, 15)
