#pragma once

#include "atlstr.h"
#include <string>

/// <summary>
/// 
/// mp string tools namespace
/// 
/// writer : ymin
/// version : 2.0
/// version desc :
///   - change string class
///	  - data type theorem
/// 
/// </summary>


namespace mps
{
	namespace file
	{
		// path
		// get path return "c:\rootdir\bin\app.exe"
		CString		GetAppPath();

		// get return app.exe
		CString		GetAppName();

		// get dir return "c:\rootdir\bin\"
		CString		GetBinDir();

		// get dir return "c:\rootdir\ini\"
		CString		GetIniDir();

		// get dir return "c:\rootdir\"
		CString		GetRootDir();

		// in "c:\dir, app.exe" >> out "c:\dir\app.exe"
		CString		GetFullPath(CString dir, CString file);

		// in "c:\dir\app.exe" >> out app.exe
		CString		GetFileNameByPath(LPCTSTR path);

		// in "c:\dir\app.exe" >> out "c:\dir\"
		CString		GetDirectoryByPath(LPCTSTR path);



		// all directory create, remove
		BOOL		CreateDirs(LPCTSTR dir_path);
		BOOL 		RemoveDirs(LPCTSTR dir_path);


		int			GetFileNameList(LPCTSTR dir, OUT CStringList& slFileName, BOOL bIncFolder = FALSE);
		// in app.exe >> c:\app.exe
		CString		GetDirByFileName(LPCTSTR file_name);




		// file info
		BOOL		GetWriteTime(CString path, OUT CTime& time);

		LONGLONG	GetFileSize(CString path);
	}
}
	
