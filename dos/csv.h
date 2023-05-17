#pragma once

#include "string.h"
#include "memory.h"
#include "scope/file.h"

#include <map>

#define CSV_ERROR_FUNC dos::string("\nFunction: ").append(__FUNCTION__)
#define CSV_ERROR_FILE dos::string("\nFile: ").append((strrchr(__FILE__,'\\')+1))
#define CSV_ERROR_LINE dos::string("\nLine: ").append(__LINE__)
#define CSV_ERROR() lastError = dos::string("").append(CSV_ERROR_FUNC).append(CSV_ERROR_FILE).append(CSV_ERROR_LINE)

namespace dos {

	typedef enum {
		TO_INT32 = 0,
		TO_STRING,
	} CONVERT_TO;

	class csv
	{
	private:
		string header;
		string parser;
		string lastError;
		bool hasHeader;
		bool hasParser;
		std::map<int, std::vector<string>> rows;

		void splitLine( char* txt, const string& parser )
		{
			std::vector<string> lines;
			lines.clear();
			string cl = string(txt);
			std::vector<string> vcl = cl.split("\r\n");
			for (size_t i = 0; i < vcl.size(); i++)
			{
				lines.push_back( vcl[i] );
			}

			//printf("\n====\n");
			rows.clear();
			int firstColumnSize = -1;
			if ( hasParser )
			{
				for (size_t i = 0; i < lines.size(); i++)
				{
					std::vector<string> vc = string( lines[i] ).split( parser );

					int columnSize = (int)vc.size();
					if (firstColumnSize == -1)
					{
						firstColumnSize = columnSize;
					}

					if (columnSize != firstColumnSize)
						continue;

					rows.insert({ i, vc });
				}
			}
			else
			{
				printf("warning: no parser\r\n");
				for (size_t i = 0; i < lines.size(); i++)
				{
					std::vector<string> vc{ lines[i] };
					rows.insert({ i, vc });
				}
			}
		}

	public:

		template<typename T>
		csv( T* mDATA, int mDATASize, const string& fileName, const string& header = string(""), const string& parser = string(""), std::vector<CONVERT_TO> c = std::vector<CONVERT_TO>() )
			: header(header), parser(parser), lastError("")
		{
			scope::file file(fileName);

			memory fileData;
			char* data = NULL;

			if (!file.opened || !file.fileSize)
			{
				CSV_ERROR();
				goto RET;
			}

			if( !fileData.alloc( &data, file.fileSize + 1 ) )
			{
				CSV_ERROR();
				goto RET;
			}
			data[file.fileSize] = '\0';

			if ( !file.Read(data, file.fileSize) )
			{
				CSV_ERROR();
				goto RET;
			}

			hasHeader = header.length() > 0;
			hasParser = parser.length() > 0;

			splitLine( data, parser );

			int rowAdded = 0;
			if ( hasHeader )
			{
				rowAdded = 1;
			}

			int lineRow = (int)rows.size();
			if ( lineRow < rowAdded+1 )
			{
				CSV_ERROR();
				goto RET;
			}

			int curRow = 0;
			int conSize = (int)c.size();

			memory::alloc( &*mDATA, mDATASize * (lineRow-rowAdded) );
			if (!*mDATA)
			{
				CSV_ERROR();
				goto RET;
			}

			BYTE* bData = (BYTE*)*mDATA;
			int bPos = 0;

			int firstColumnSize = (int)rows[0].size();
			if ( hasParser )
			{
				for ( auto it = rows.begin(); it != rows.end(); it++ )
				{
					curRow++;

					//ignore header
					if ( hasHeader == 1 && curRow == 1 )
						continue;

					int columnSize = (int)it->second.size();

					//printf( "%d : >> column:%d, con:%d ", it->first, columnSize, conSize );
					for (int i = 0; i < columnSize; i++)
					{
						if (i < conSize) {
							switch (c[i])
							{
							case TO_INT32:
								//printf(">>Int(%d)", it->second[i].toInt());
							{
								int bValue = it->second[i].toInt();
								memcpy(&bData[bPos], &bValue, 4);
								bPos += 4;
							}
							break;
							//default:
							//	printf(">>Default(%s)", it->second[i].toString());
							//	break;
							}
						}
					}
					//printf( "\n" );
				}
			}

		RET:
			if(data)
				memory::release(&data);
			file.Close();
		}

		~csv()
		{
			if(!rows.empty())
				rows.clear();
		}

		BOOL isValid() const { return lastError.length() == 0; }

		const string& getLastError() { return lastError; }

		const int getRowCount() { return (int)rows.size(); }

		const int getColumnCount() {
			if(rows.size())
				return (int)rows[0].size();
			return 0;
		}

	};
}

#undef CSV_ERROR
