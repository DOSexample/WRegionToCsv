#pragma once

namespace dos {
	namespace scope {

		class file
		{
		public:
			BOOL opened;
			BOOL owner;
			HANDLE hfile;
			DWORD fileSize;
			file( const string& fileName )
			{
				hfile = CreateFileA( fileName.toString(), GENERIC_READ, FILE_SHARE_READ, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL );
				opened = hfile != INVALID_HANDLE_VALUE;
				owner = TRUE;
				fileSize = GetFileSize(hfile, 0);
			}
			file( HANDLE hFile )
			{
				hfile = hFile;
				opened = hfile != INVALID_HANDLE_VALUE;
				owner = FALSE;
				fileSize = INVALID_FILE_SIZE;
			}
			~file()
			{
				if (owner && opened)
					CloseHandle(hfile);
			}
			BOOL Read(void* ptr, int size)
			{
				if (!opened || !ptr || !size) return 0;

				DWORD tReadBytes;
				if (!ReadFile(hfile, ptr, size, &tReadBytes, 0) || tReadBytes != size)
					return FALSE;

				return TRUE;
			}
			BOOL Close()
			{
				if (!opened || !owner) return FALSE;

				opened = FALSE;

				return CloseHandle(hfile);
			}
		};

	}

}