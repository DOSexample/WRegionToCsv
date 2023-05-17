#pragma once

//#define USE_GLOBAL_MEMORY
#ifdef USE_GLOBAL_MEMORY
#define ALLOC(size) GlobalAlloc( 0, size ) 
#define FREE( ptr ) if( ptr ){ GlobalFree( ptr ); ptr = NULL; }
#else
#define ALLOC(size) HeapAlloc( GetProcessHeap(), 0, size ) 
#define FREE( ptr ) if( ptr ){ HeapFree( GetProcessHeap(), 0, ptr ); ptr = NULL; }
#endif

namespace dos {
	class memory
	{
	public:

		template<typename T, typename T2>
		static int copy(T dst, T2 src, int size)
		{
			if (!dst || !src || !size) return 0;
			memcpy((void*)dst, (void*)src, size);
			return size;
		}

		static void copy(void* dst, BYTE* src, int size, int* pos)
		{
			if (!dst || !src || !size || !pos || *pos < 0) return;

			memcpy(dst, &src[*pos], size);
			*pos += size;
		}

		template<typename T>
		static T alloc(int size)
		{
			T ptr;
			ptr = (T)ALLOC(size);

			return ptr;
		}

		template<typename T>
		static T alloc(T* ptr, int size)
		{
			*ptr = (T)ALLOC(size);

			return *ptr;
		}

		template<typename T>
		static void release(T* ptr)
		{
			if (!*ptr) return;

			FREE(*ptr);
			*ptr = NULL;
		}
	};

}