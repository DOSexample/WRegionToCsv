import getFiles, { exists, fileExt, trimPath, fmtFileSize } from "https://deno.land/x/getfiles/mod.ts";
import { ByteSet, LengthType, NumberType } from "https://deno.land/x/bytes/mod.ts";

interface IDFiles
{
  path: string,
  name: string,
  ext: string,
  realPath: string,
  info: any
}

interface IWRegion
{
	mVALUE01: number;
	mIndex: number;//monster id
	mVALUE03: number;
	mNumber: number;//Number of monsters to summon
	mLocation: number[];//[3], location of monster to summon
	mRADIUS: number;
}

const files = getFiles('../../Build/DATA/SUMMON') as IDFiles[];

function appendData(obj: any)
{
  var data = "";
  for(var o in obj)
  {
    if(typeof(obj[o]) === "object")
    {
      for( var i = 0; i < obj[o].length; i++)
      {
        data += obj[o][i] + "|";
      }
    } else {
      data += obj[o] + "|";
    }
  }
  return data;
}

async function doWork()
{

for( var f of files )
{
  if(f.ext !== "WREGION") continue;
  console.log("unpacking "+ f.path + " ...");

  const text = await Deno.readFile(f.path);
  //const buf = Buffer.alloc(text.length);
  const b = ByteSet.from(text.buffer);
  
  const row = b.read.int32();
  //const rw:IWRegion[] = [];
  var data = "";
  for(var i = 0; i < row; i++)
  {
    var val:IWRegion = {} as IWRegion;
    val.mVALUE01 = b.read.int32();
    val.mIndex = b.read.int32();
    val.mVALUE03 = b.read.int32();
    val.mNumber = b.read.int32();
    val.mLocation = [];
    val.mLocation[0] = b.read.int32();
    val.mLocation[1] = b.read.int32();
    val.mLocation[2] = b.read.int32();
    val.mRADIUS = b.read.int32();
    //rw.push(val);

    data += appendData(val) + "\r\n";
  }

  //console.log(data);
  await Deno.writeTextFile(f.path+".csv", data);

  console.log("unpacked...");
}

}
doWork();