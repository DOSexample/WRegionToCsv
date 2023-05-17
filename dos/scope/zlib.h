#pragma once

#include "Memory.h"
#include "FileScope.h"

extern BOOL Decompress(DWORD tCompressSize, BYTE* tCompress, DWORD tOriginalSize, BYTE* tOriginal);

namespace dos {
	namespace scope {
		class zlib
		{
		public:
			DWORD mOriginalSize, mCompressSize;
			BYTE* mOriginal, * mCompress;
			BOOL mReleaseOriginal, mReleaseCompress;
			BOOL error;
			zlib()
			{
				error = FALSE;
				mOriginal = mCompress = NULL;
				mOriginalSize = mCompressSize = 0;
				mReleaseOriginal = mReleaseCompress = TRUE;
			}

			zlib(file* f)
			{
				error = FALSE;
				mOriginal = mCompress = NULL;
				mOriginalSize = mCompressSize = 0;
				mReleaseOriginal = mReleaseCompress = TRUE;
				GetZData(f);
			}

			~zlib()
			{
				Release();
				error = FALSE;
				mOriginal = mCompress = NULL;
				mOriginalSize = mCompressSize = 0;
				mReleaseOriginal = mReleaseCompress = TRUE;
			}

			//Manual Release
			void Release()
			{
				if (mReleaseOriginal)
					mem::release(&mOriginal);
				if (mReleaseCompress)
					mem::release(&mCompress);
			}

			void Free()
			{
				mReleaseOriginal = TRUE;
				mReleaseCompress = TRUE;
				Release();
			}

			BOOL GetZData(file* f)
			{
				error = TRUE;
				if (!f->Read(&mOriginalSize, 4) || !f->Read(&mCompressSize, 4))
				{
					printf("%s:%d\n", __FUNCTION__, __LINE__);
					return FALSE;
				}
				mOriginal = mem::alloc<BYTE*>(mOriginalSize);
				mCompress = mem::alloc<BYTE*>(mCompressSize);
				if (!mOriginal || !mCompress)
				{
					printf("%s:%d >> %p : %p\n", __FUNCTION__, __LINE__, mOriginal, mCompress);
					return FALSE;
				}
				if (!f->Read(mCompress, mCompressSize))
				{
					printf("%s:%d\n", __FUNCTION__, __LINE__);
					return FALSE;
				}
				if (!Decompress(mCompressSize, mCompress, mOriginalSize, mOriginal))
				{
					printf("%s:%d\n", __FUNCTION__, __LINE__);
					return FALSE;
				}
				error = FALSE;
				return TRUE;
			}

			template <typename T>
			BOOL Unpack005Copy(file* f, int* mDataNum, int mCheckDataNum, int mXorData, int mDataOffset, BYTE* mDATA)
			{
				if (!GetZData(f))
				{
					printf("%s:%d\n", __FUNCTION__, __LINE__);
					return FALSE;
				}

				CopyMemory( mDataNum, &mOriginal[0], 4 );
				*mDataNum ^= mXorData;
				if (*mDataNum != mCheckDataNum)
				{
					printf("%s:%d\n", __FUNCTION__, __LINE__);
					return FALSE;
				}

				CopyMemory( &mDATA[0], &mOriginal[mDataOffset], mCheckDataNum * sizeof(T) );
				Free();

				return TRUE;
			}
		};
	}
}