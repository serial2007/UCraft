#include "ImportInfo.h"

std::map<int, ImportInfo::BlockInfo> ImportInfo::binfo;

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

	

	for (unsigned int i = 0; i < value.size(); ++i)
	{
		Json::Value vl = value[i];
		int id = vl["id"].asInt();
		int type = vl["type"].asInt();
		Json::Value dx = vl["index"];
		if (type == 1)
		{
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
	}
}
