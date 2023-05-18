#include "ImportInfo.h"

std::map<int, ImportInfo::BlockInfo> ImportInfo::binfo;
std::map<int, ImportInfo::SpecialBlockInfo> ImportInfo::spbinfo;
std::map<int, ImportInfo::NbtGoto> ImportInfo::nbtinfo;
unsigned short	ImportInfo::IsSpecialModel[1024];
bool	ImportInfo::IsNotSolid[1024];
bool	ImportInfo::IsTransmit[1024];
bool	ImportInfo::CanWalkThrough[1024];
float	ImportInfo::EmitLight[1024];



void ImportInfo::StartImport()
{
	std::ifstream fin("Resources/index.json");
	if (!fin.good()) {
		std::cout << "Can't open JSON file.\n";
		return;
	}
	std::stringstream buf;
	buf << fin.rdbuf();
	std::string jsonstr = buf.str();
	Json::Value value;
	Json::Reader reader;
	if (!reader.parse(jsonstr, value)) {
		std::cout << "Can't resolve JSON file.\n";
		return;
	}

#define __READ_NBT__(_WH) \
for (unsigned int j = 0; j < general.size(); ++j) {			\
Json::Value m = general[j];									\
															\
ImportInfo::UnitModel k;									\
k.layer = m["layer"].asInt();								\
															\
Json::Value t = m["TexStart"];								\
k.TexStartX = t["x"].asInt();								\
k.TexStartY = t["y"].asInt();								\
															\
t = m["TexEnd"];											\
k.TexEndX = t["x"].asInt();									\
k.TexEndY = t["y"].asInt();									\
															\
t = m["PosStart"];											\
k.PosStartX = t["x"].asFloat();								\
k.PosStartY = t["y"].asFloat();								\
k.PosStartZ = t["z"].asFloat();								\
															\
t = m["PosMid"];											\
k.PosMidX = t["x"].asFloat();								\
k.PosMidY = t["y"].asFloat();								\
k.PosMidZ = t["z"].asFloat();								\
															\
t = m["PosEnd"];											\
k.PosEndX = t["x"].asFloat();								\
k.PosEndY = t["y"].asFloat();								\
k.PosEndZ = t["z"].asFloat();								\
															\
															\
tmp[_WH].push_back(k);										\
}

	for (unsigned int i = 0; i < value.size(); ++i)
	{
		Json::Value vl = value[i];
		int id = vl["id"].asInt();
		int type = vl["type"].asInt();
		
		ImportInfo::IsNotSolid[id] = vl["unsolid"].asInt();
		ImportInfo::IsTransmit[id] = vl["transmit"].asInt();
		ImportInfo::CanWalkThrough[id] = vl["canwalkthrough"].asInt();
		ImportInfo::EmitLight[id] = vl["light"].asFloat();

		if (type == 1)
		{
			Json::Value dx = vl["index"];
			Json::Value basic = dx["basic"];
			Json::Value start = basic["start"];
			Json::Value end   = basic["end"];
			int stx, sty, endx, endy;
			stx = start["x"].asInt();
			sty = start["y"].asInt();
			endx = end["x"].asInt();
			endy = end["y"].asInt();
			
			binfo[id] = {
				stx, sty, endx, endy,
				stx, sty, endx, endy,
				stx, sty, endx, endy,
				stx, sty, endx, endy,
				stx, sty, endx, endy,
				stx, sty, endx, endy,
			};
		}

		if (type == 2)
		{
			Json::Value dx = vl["index"];
			Json::Value Top			= dx["top"];
			Json::Value Topstart	= Top["start"];
			Json::Value Topend		= Top["end"];

			Json::Value Middle		= dx["middle"];
			Json::Value Middlestart = Middle["start"];
			Json::Value Middleend	= Middle["end"];

			Json::Value Bottom		= dx["bottom"];
			Json::Value Bottomstart = Bottom["start"];
			Json::Value Bottomend	= Bottom["end"];

			Texture4 top, middle, bottom;

			top.stx =  Topstart["x"].asInt();
			top.sty =  Topstart["y"].asInt();
			top.endx = Topend["x"].asInt();
			top.endy = Topend["y"].asInt();

			middle.stx =  Middlestart["x"].asInt();
			middle.sty =  Middlestart["y"].asInt();
			middle.endx = Middleend["x"].asInt();
			middle.endy = Middleend["y"].asInt();

			bottom.stx =  Bottomstart["x"].asInt();
			bottom.sty =  Bottomstart["y"].asInt();
			bottom.endx = Bottomend["x"].asInt();
			bottom.endy = Bottomend["y"].asInt();


			binfo[id] = {
				middle.stx, middle.sty, middle.endx, middle.endy,
				bottom.stx, bottom.sty, bottom.endx, bottom.endy,
				middle.stx, middle.sty, middle.endx, middle.endy,
				middle.stx, middle.sty, middle.endx, middle.endy,
				top.stx,    top.sty,    top.endx,    top.endy,
				middle.stx, middle.sty, middle.endx, middle.endy
			};
		}

		if (type == -1)
		{
			Json::Value dx = vl["index"];
			ImportInfo::SpecialBlockInfo tmp;
			Json::Value general;
			general = dx["general"];	__READ_NBT__(6);
			general = dx["0"];			__READ_NBT__(0);
			general = dx["1"];			__READ_NBT__(1);
			general = dx["2"];			__READ_NBT__(2);
			general = dx["3"];			__READ_NBT__(3);
			general = dx["4"];			__READ_NBT__(4);
			general = dx["5"];			__READ_NBT__(5);




			ImportInfo::spbinfo[id] = tmp;
			ImportInfo::IsSpecialModel[id] = 1;
		}

		if (type == 0)
		{
			int nbttype = vl["nbttype"].asInt();
			ImportInfo:NbtGoto nbtinfo;
			nbtinfo.nbttype = nbttype;
			Json::Value nbt = vl["nbt"];
			if (nbttype == 1)
			{	
				nbtinfo.go.push_back(nbt["0"].asInt());
				nbtinfo.go.push_back(nbt["1"].asInt());
				nbtinfo.go.push_back(nbt["2"].asInt());
				nbtinfo.go.push_back(nbt["3"].asInt());
				nbtinfo.go.push_back(nbt["4"].asInt());
				nbtinfo.go.push_back(nbt["5"].asInt());
			}
			

			Json::Value dis = vl["display"];
			nbtinfo.DisplayTexStartX = dis["TexStartX"].asInt();
			nbtinfo.DisplayTexStartY = dis["TexStartY"].asInt();
			nbtinfo.DisplayTexEndX   = dis["TexEndX"].asInt();
			nbtinfo.DisplayTexEndY   = dis["TexEndY"].asInt();
			
			nbtinfo.DisplayPosStartX = dis["PosStartX"].asFloat();
			nbtinfo.DisplayPosStartY = dis["PosStartY"].asFloat();
			nbtinfo.DisplayPosEndX = dis["PosEndX"].asFloat();
			nbtinfo.DisplayPosEndY = dis["PosEndY"].asFloat();
				
				
				ImportInfo::nbtinfo[id] = nbtinfo;
				ImportInfo::IsSpecialModel[id] = 2;
		}
	}
}
