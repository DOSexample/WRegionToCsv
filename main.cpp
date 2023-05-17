#include <Windows.h>
#include <stdio.h>

#include "dos/csv.h"

int main(int argc, char** argv)
{
	WORLD_REGION_INFO* tRegionInfo = NULL;
	const char* tFileName = "../DATA/SUMMON/Z001_SUMMONMONSTER.WREGION.csv";
	dos::csv csv( &tRegionInfo, sizeof(WORLD_REGION_INFO), tFileName, "", "|",
		std::vector<dos::CONVERT_TO>{
		dos::TO_INT32,
		dos::TO_INT32,
		dos::TO_INT32,
		dos::TO_INT32,
		dos::TO_INT32,
		dos::TO_INT32,
		dos::TO_INT32,
		dos::TO_INT32,
	}
	);
	if ( !csv.isValid() )
	{
		printf( csv.getLastError() );
		return 0;
	}
		
	console.log( "\r\nrow: %d, column: %d", csv.getRowCount(), csv.getColumnCount() );
	for (int i = 0; i < csv.getRowCount(); i++)
	{
		printf("%d|%d|%d|%d|%d|%d|%d|%d",
		tRegionInfo[i].mVALUE01,
		tRegionInfo[i].mIndex,
		tRegionInfo[i].mVALUE03,
		tRegionInfo[i].mNumber,
		tRegionInfo[i].mLocation[0],
		tRegionInfo[i].mLocation[1],
		tRegionInfo[i].mLocation[2],
		tRegionInfo[i].mRADIUS);
		printf("\r\n=======================\r\n");
	}

	dos::memory::release(&tRegionInfo);
	
	return 0;
}