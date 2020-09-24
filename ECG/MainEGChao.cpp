#include "stdafx.h"
#include "LandTableInfo.h"
#include "SA2ModLoader.h"
#include "IniFile.h"
#include "TextConv.h"

#define ReplaceSET(a, b) helperFunctions.ReplaceFile("resource\\gd_PC\\" a ".bin", "resource\\gd_PC\\" b ".bin");

NJS_TEXNAME eNeutTexCount[12];
NJS_TEXLIST eNeutTexList = { arrayptrandlength(eNeutTexCount) };

extern "C"
{

	__declspec(dllexport) void Init(const char* path, const HelperFunctions& helperFunctions)
	{

		WriteCall((void*)0x0052B408, nullsub_1); //Disables Height
		WriteCall((void*)0x0052B430, nullsub_1);

		HMODULE hmodule = GetModuleHandle(__TEXT("Data_DLL_orig"));

		((NJS_TEXLIST*)GetDllData("texlist_al_stg_neut_tex"))->textures = eNeutTexCount;

		NJS_OBJECT* NeutPond = (NJS_OBJECT*)GetDllData("object_gneut_nc_water_nc_water");
		NJS_OBJECT* Falls0 = (NJS_OBJECT*)GetDllData("object_gneut_nc_fwater00_nc_fwater00");
		NJS_OBJECT* Falls1 = (NJS_OBJECT*)GetDllData("object_gneut_nc_fwater01_nc_fwater01");

		LandTable* ENeutral = (LandTable*)GetProcAddress(hmodule, "objLandTableNeut");
		*ENeutral = *(new LandTableInfo(std::string(path) + "\\Landtables\\ENeut.sa2blvl"))->getlandtable();
		ENeutral->TextureList = &eNeutTexList;
		ENeutral->TextureName = (char*)"al_stg_neut_tex";

		NJS_OBJECT* ENeutPond = ENeutral->COLList[0].Model;
		NJS_OBJECT* EFalls0 = ENeutral->COLList[1].Model;
		NJS_OBJECT* EFalls1 = ENeutral->COLList[2].Model;

		const IniFile* config = new IniFile(std::string(path) + "\\config.ini");

		bool wFix = config->getBool("GenSet", "WaterFix", true);
		if (wFix)
		{
			ENeutral->COLList[0].Flags &= ~0x80000000;
			ENeutral->COLList[1].Flags &= ~0x80000000;
			ENeutral->COLList[2].Flags &= ~0x80000000;
		}

		bool Set = config->getBool("GenSet", "Set", true);
		if (!Set)
		{
			ReplaceSET("set_chao_neut_u", "set_null")
		}

		*NeutPond = *ENeutPond;
		*Falls0 = *EFalls0;
		*Falls1 = *EFalls1;

		DataArray(NJS_VECTOR, ChaoPos, 0x01366260, 16 * 3);

		ChaoPos[1]  = { -155.0f, 20.7f, -68.19f };            // Pillar 0 (Left of Race) 
		ChaoPos[2]  = { -114.28f, 23.1f, -163.53f };          // Pillar 1 (Right of Race) 
		ChaoPos[3]  = { -165.73f, 61.1f, -131.78f };          // Pillar 2 (Above Race) 
		ChaoPos[4]  = { -206.36f, 105.0f, -95.94f };          // 2nd Highest Point 
		ChaoPos[5]  = { -16.86f, 0.3f, -67.84f };             // Ground 0 (Front of Race) 
		ChaoPos[6]  = { -30.15f, 8.0f, 36.54f };              // Ground 1 (On Hill Side Slope) 
		ChaoPos[7]  = { -145.29f, 34.0f, -28.71f };           // Highest peak
		//ChaoPos[7]  = { -152.92f, 135.0f, -229.55f };       // Highest peak // issue
		ChaoPos[8]  = { -131.14f, 4.0f, -107.18f };           // Outside Racee
		ChaoPos[9]  = { 31.78f, 0.0f, -70.36f };              // TV
		ChaoPos[10] = { 52.72f, 10.0f, 34.13f };              // Transporter
		ChaoPos[11] = { -9.0f, 0.0f, -133.0f };               // Right of Entrance 
		ChaoPos[12] = { 58.98f, 0.0f, -109.66f };             // left of entrance
		ChaoPos[13] = { -120.12f, 2.0f, -2.69f };             // Lowest Shelf 
		//ChaoPos[14] = { 172.98f, 130.4f, -94.3f };            // SSecret

		bool sSecret = config->getBool("GenSet", "SSecret", false);
		if (sSecret)
		{
			ChaoPos[14] = { 82.53f, 0.2f, -93.76f };
		}
		else
		{
			ChaoPos[14] = { 172.98f, 130.4f, -94.3f };            // SSecret
		}

		bool ndrown = config->getBool("GenSet", "NoDrown", false);
		if (ndrown)
		{
			ChaoPos[0] = { -142.5, 34.1, -15.74 };
			ChaoPos[15] = { 10.26, 0.1, -58.67 };           
		}
		else
		{
			ChaoPos[0] = { -105.47f, -3.7f, -83.69f };           // Shelf 
			ChaoPos[15] = { -68.47f, -4.3f, -74.83f };            // Splish-spash 
		}

		DataArray(NJS_VECTOR, TreePos, 0x01366B30, 30);

		//TreePos[0] = { 0, 40, 0 };                  // Normal Tree 1 
		//TreePos[1] = { 10, 20, 0 };                 // Normal Tree 2 
		//TreePos[2] = { 20, 20, 0 };                 // Normal Tree 3 
		//TreePos[3] = { 30, 20, 0 };                 // Custom Tree 1 
		TreePos[6] = { -50, 3, 7 };                   // Custom Tree 4
		//TreePos[5] = { 0, 20, 10 };                 // Custom Tree 3 
		//TreePos[6] = { 0, 20, 20 };                 // Custom Tree 4 
		//TreePos[7] = { 0, 20, 30 };                 // Unused
		//TreePos[8] = { 0, 20, 40 };                 // Unused
		//TreePos[9] = { 0, 20, 50 };                 // Unused

		ReplaceSET("set_chao_neut_s", "set_chao_eneut_s")
	}

	__declspec(dllexport) ModInfo SA2ModInfo = { ModLoaderVer };
}
