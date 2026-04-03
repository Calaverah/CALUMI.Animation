//Copyright © 2025-2026 Calaverah. All rights reserved.
//License: https://www.gnu.org/licenses/lgpl-3.0.html
//Contact: Calaverahmedia@gmail.com


#include "utilities/CALUMI_Hashes.h"
#include <unordered_map>
#include <string>

namespace CALUMI {
    namespace Utilities {

		static const std::unordered_map<uint32_t, std::string> _known_string_hash_list = {
            { 0, "" },
            { 1025567112, "AnimObjectRoot_ChronoMark" },
            { 4294032154, "MiscA" },
            { 1644583399, "MiscA01" },
            { 4212066397, "MiscA02" },
            { 2349349067, "MiscA03" },
            { 271192881, "MiscB04" },
            { 192388137, "MiscA14" },
            { 1614866366, "MiscB01" },
            { 2953311471, "bn_Workbench_Milledpart002" },
            { 2883097307, "Magazine" },
            { 909650836, "C_00Page013" },
            { 1444148067, "AnimObjectRoot_MinerControllerBelt01" },
            { 4065686445, "Magazine01" },
            { 1094654722, "C_00Page012" },
            { 704072858, "bn_PilotSeat_C_Panel" },
            { 2341407772, "bn_PilotSeatStroud_C_Arm" },
            { 1220209087, "Trigger" },
            { 826578966, "bn_PilotSeat_R_Lever1" },
            { 309112168, "MiscA04" },
            { 356015939, "bn_PilotSeatStroud_L_FlightStick" },
            { 2501052810, "MiscA13" },
            { 1361174920, "SkinSim_13" },
            { 1685238782, "C_02BackPages1" },
            { 464203582, "C_00FrontCover3" },
            { 1701551614, "MiscA05" },
            { 2065524902, "MiscA11" },
            { 4052944902, "bn_Workbench_armlink_3" },
            { 4234333252, "MiscA06" },
            { 2338716882, "MiscA07" },
            { 467329347, "MiscA08" },
            { 3882193372, "AnimRoot" },
            { 1826476501, "MiscA09" },
            { 3906555008, "C_00WatchBody1" },
            { 912005648, "C_01BackCover1" },
            { 2892588687, "L_00WatchBand1" },
            { 895498037, "L_00WatchBand2" },
            { 2941602730, "C_01BackCover2" },
            { 300929838, "SkinSim_3" },
            { 1114048419, "L_00WatchBand3" },
            { 1095583110, "bn_Workbench_Drawer_2" },
            { 3083942686, "L_00WatchClasp1" },
            { 1541895397, "ProjectileNode" },
            { 962767972, "bn_Workbench_Milledpart02" },
            { 640078110, "SkinSim_12" },
            { 20975459, "R_00WatchBand1" },
            { 3211295783, "bn_Workbench_clutter_1" },
            { 2554945241, "R_00WatchBand2" },
            { 1541010729, "bn_PilotSeatStroud_R_FlightStick" },
            { 3747414899, "Weapon" },
            { 4252754500, "C_02BackPages2" },
            { 732261574, "bn_FabricRoll" },
            { 3731897261, "C_01HingeBottom1" },
            { 4014894671, "R_00WatchBand3" },
            { 533780119, "bn_PilotSeat_R_Engine2" },
            { 2995519171, "C_00Belt1" },
            { 1336870990, "AnimObjectRoot_Book" },
            { 1114666736, "C_00HingeTop1" },
            { 2364348306, "bn_Vial_9" },
            { 1636116385, "SkinSim_6" },
            { 1508105302, "L_00Stick1" },
            { 500586446, "bn_PilotSeat_R_KeyKnob1" },
            { 1731011495, "MiscB05" },
            { 1619291405, "R_00Stick1" },
            { 2387510930, "MiscB03" },
            { 3627436728, "C_00Page011" },
            { 1801200151, "Magazine02" },
            { 3629284156, "C_01BackCover3" },
            { 157271669, "C_00Main1" },
            { 2077380468, "bn_PilotSeat_L_Engine2" },
            { 2185201442, "Magazine04" },
            { 1668142258, "bn_FoodStation_Drawer_1" },
            { 4121251346, "C_00FrontCover1" },
            { 1823219624, "C_00FrontCover2" },
            { 2781662610, "C_01FrontPages1" },
            { 1019583528, "C_01FrontPages2" },
            { 1271049406, "C_01FrontPages3" },
            { 203053104, "MiscA10" },
            { 2323436242, "C_02BackPages3" },
            { 3792976156, "MiscA12" },
            { 3971386661, "Bolt" },
            { 218858205, "SkinSim_22" },
            { 4116903173, "Bolt01" },
            { 475730561, "Magazine03" },
            { 4182357508, "MiscB02" },
            { 4114134964, "Magazine05" },
            { 1815209486, "Magazine06" },
            { 1246468059, "bn_Workbench_Drawer04" },
            { 2087759039, "MiscA15" },
            { 3849977093, "MiscA16" },
            { 2457783699, "MiscA17" },
            { 46256130, "MiscA18" },
            { 2482925738, "bn_PilotSeat_L_FlightStick" },
            { 1975967892, "MiscA19" },
            { 2229208692, "bn_PilotSeat_R_KeyKnob2" },
            { 873699032, "Trigger01" },
            { 1727589024, "MiscB" },
            { 746910172, "Scope01" },
            { 3045957734, "Scope02" },
            { 2483213159, "SkinSim_21" },
            { 1818993855, "Bolt02" },
            { 460100649, "Bolt03" },
            { 4226426628, "bn_Vial_8" },
            { 2904213346, "Trigger02" },
            { 456578712, "Magazine07" },
            { 3740477304, "bn_cloth005" },
            { 2341063433, "Magazine08" },
            { 106415207, "AnimObjectRoot_ParasiteATransform" },
            { 3229959599, "bn_furniture_camera5" },
            { 3832031208, "SkinSim_24" },
            { 2047652427, "SkinSim_23" },
            { 3808814065, "SkinSim_20" },
            { 2985560214, "SkinSim_19" },
            { 3337828352, "SkinSim_18" },
            { 1447830929, "SkinSim_17" },
            { 558568711, "SkinSim_16" },
            { 3091350717, "SkinSim_15" },
            { 3087382590, "bn_Workbench_clutter_5" },
            { 3477435435, "SkinSim_14" },
            { 3207561380, "SkinSim_11" },
            { 3358109746, "SkinSim_10" },
            { 868131195, "bn_FoodStation_Pan" },
            { 4047125040, "SkinSim_9" },
            { 2252147366, "SkinSim_8" },
            { 377624375, "SkinSim_7" },
            { 2737320686, "bn_Workbench_Drawer02" },
            { 4169954843, "SkinSim_5" },
            { 2408269453, "SkinSim_4" },
            { 1726538680, "SkinSim_2" },
            { 4292981250, "SkinSim_1" },
            { 2296808084, "SkinSim_0" },
            { 89447045, "bn_blade_2" },
            { 910849296, "bn_Workbench_Drawer_3" },
            { 2623376191, "bn_blade_1" },
            { 3518032994, "bn_cloth03" },
            { 702969621, "bn_cloth017" },
            { 1591822211, "bn_cloth016" },
            { 3353876025, "bn_cloth015" },
            { 2968463023, "bn_cloth014" },
            { 780907276, "bn_cloth013" },
            { 210041052, "bn_FoodStation_SmallPot" },
            { 1502380954, "bn_cloth012" },
            { 3229995552, "bn_cloth011" },
            { 3078808246, "bn_cloth010" },
            { 3611650899, "bn_cloth009" },
            { 87453510, "bn_scanner" },
            { 2688703429, "bn_cloth008" },
            { 821904980, "bn_cloth007" },
            { 1207596738, "bn_cloth006" },
            { 2851346414, "bn_cloth004" },
            { 2797067508, "bn_cloth02" },
            { 1069452622, "bn_cloth01" },
            { 3504768782, "bn_handle_2" },
            { 4121980178, "bn_scissor_2" },
            { 1240454836, "bn_handle_1" },
            { 3078780217, "bn_furniture_camera4" },
            { 1824079016, "bn_scissor_1" },
            { 244226169, "bn_FoodStation_Lid" },
            { 3368456395, "bn_scissorroot" },
            { 2372122014, "bn_FoodStation_Drawer_3" },
            { 4200891656, "bn_FoodStation_Drawer_2" },
            { 3437475567, "bn_FoodStation_CarryKit" },
            { 4213781512, "bn_FoodStation_DrinkPack" },
            { 3073558690, "bn_FoodStation_Knob_1" },
            { 775657752, "bn_FoodStation_Knob_2" },
            { 1497147790, "bn_FoodStation_Knob_3" },
            { 3344448557, "bn_FoodStation_Knob_4" },
            { 1348433295, "bn_FoodStation_Pot" },
            { 2343705559, "bn_FoodStation_Spoon" },
            { 3115796101, "bn_Workbench_Button" },
            { 3262616666, "bn_Workbench_Milledpart01b" },
            { 3044058316, "bn_Workbench_Milledpart01c" },
            { 3483950142, "bn_Workbench_Panel" },
            { 1121237744, "bn_Workbench_Scanner" },
            { 3559850616, "bn_Workbench_Drawer03" },
            { 975266644, "bn_Workbench_Drawer01" },
            { 2257860752, "bn_Workbench_armlink_2" },
            { 2576514362, "bn_Workbench_Tablet" },
            { 530409770, "bn_Workbench_armlink_1" },
            { 1561077872, "bn_Capsule" },
            { 2121338998, "bn_Monitor" },
            { 1844085466, "bn_Case" },
            { 1800747669, "bn_Vial_7" },
            { 475138563, "bn_Vial_6" },
            { 3246189237, "bn_PilotSeat_L_Lever2" },
            { 3353903542, "bn_furniture_camera1" },
            { 2237356985, "bn_Vial_5" },
            { 4066143023, "bn_Vial_4" },
            { 280161018, "bn_Beaker" },
            { 2024705845, "bn_Bottle" },
            { 3753852019, "bn_PilotSeat_L_Panel" },
            { 2184617888, "bn_Vial_1" },
            { 457166362, "bn_Vial_2" },
            { 1815674508, "bn_Vial_3" },
            { 1871464200, "bn_PilotSeatStroud_R_Panel" },
            { 1483579547, "bn_PilotSeatStroud_L_Panel" },
            { 3111141296, "bn_PilotSeatStroud_C_Frame" },
            { 1591857164, "bn_furniture_camera2" },
            { 4254870552, "bn_furnitureRoot" },
            { 702996634, "bn_furniture_camera3" },
            { 1878498725, "bn_Workbench_armlink_4" },
            { 1515073415, "bn_PilotSeat_R_Throttle" },
            { 3709852352, "bn_PilotSeat_R_FlightStick" },
            { 3773489072, "bn_PilotSeat_R_ControlArm" },
            { 1663672028, "bn_PilotSeat_L_Throttle" },
            { 96723282, "bn_PilotSeat_L_ControlArm" },
            { 2463092595, "bn_PilotSeat_R_PanelKnob" },
            { 3898530784, "bn_PilotSeat_R_Panel" },
            { 2823670188, "bn_PilotSeat_R_Lever2" },
            { 853993470, "bn_PilotSeat_R_Keyboard" },
            { 1850679486, "bn_PilotSeat_L_PanelKnob" },
            { 2262402861, "bn_PilotSeat_R_Engine1" },
            { 633027107, "bn_PilotSeat_L_Knob3" },
            { 1388055221, "bn_PilotSeat_L_Knob2" },
            { 3417660175, "bn_PilotSeat_L_Knob1" },
            { 1484110607, "bn_PilotSeat_L_Lever1" },
            { 193025701, "bn_PilotSeat_L_Keyboard" },
            { 3806011086, "bn_PilotSeat_L_Engine1" },
            { 3557641591, "bn_PilotSeat_C_Arm" },
            { 777640720, "bn_PilotSeat_C_Chair" },
            { 4061648821, "bn_Workbench_Screw_2" },
            { 1797162511, "bn_Workbench_Screw_1" },
            { 2821644467, "bn_Workbench_Drawer_4" },
            { 3628340284, "bn_Workbench_Drawer_1" },
            { 1566899742, "bn_Workbench_Door" },
            { 554494340, "bn_Workbench_clutter_6" },
            { 3473049768, "bn_Workbench_clutter_4" },
            { 1365646603, "bn_Workbench_clutter_3" },
            { 643902877, "bn_Workbench_clutter_2" },
            { 4158602951, "bn_WeightBar" },
            { 2590678353, "bn_BarAdjustable" }
		};

		constexpr static uint32_t _bgs_string_lookup_table_crc32[256] = {
			0x00000000, 0x77073096, 0xEE0E612C, 0x990951BA, 0x076DC419, 0x706AF48F, 0xE963A535, 0x9E6495A3,
			0x0EDB8832, 0x79DCB8A4, 0xE0D5E91E, 0x97D2D988, 0x09B64C2B, 0x7EB17CBD, 0xE7B82D07, 0x90BF1D91,
			0x1DB71064, 0x6AB020F2, 0xF3B97148, 0x84BE41DE, 0x1ADAD47D, 0x6DDDE4EB, 0xF4D4B551, 0x83D385C7,
			0x136C9856, 0x646BA8C0, 0xFD62F97A, 0x8A65C9EC, 0x14015C4F, 0x63066CD9, 0xFA0F3D63, 0x8D080DF5,
			0x3B6E20C8, 0x4C69105E, 0xD56041E4, 0xA2677172, 0x3C03E4D1, 0x4B04D447, 0xD20D85FD, 0xA50AB56B,
			0x35B5A8FA, 0x42B2986C, 0xDBBBC9D6, 0xACBCF940, 0x32D86CE3, 0x45DF5C75, 0xDCD60DCF, 0xABD13D59,
			0x26D930AC, 0x51DE003A, 0xC8D75180, 0xBFD06116, 0x21B4F4B5, 0x56B3C423, 0xCFBA9599, 0xB8BDA50F,
			0x2802B89E, 0x5F058808, 0xC60CD9B2, 0xB10BE924, 0x2F6F7C87, 0x58684C11, 0xC1611DAB, 0xB6662D3D,
			0x76DC4190, 0x01DB7106, 0x98D220BC, 0xEFD5102A, 0x71B18589, 0x06B6B51F, 0x9FBFE4A5, 0xE8B8D433,
			0x7807C9A2, 0x0F00F934, 0x9609A88E, 0xE10E9818, 0x7F6A0DBB, 0x086D3D2D, 0x91646C97, 0xE6635C01,
			0x6B6B51F4, 0x1C6C6162, 0x856530D8, 0xF262004E, 0x6C0695ED, 0x1B01A57B, 0x8208F4C1, 0xF50FC457,
			0x65B0D9C6, 0x12B7E950, 0x8BBEB8EA, 0xFCB9887C, 0x62DD1DDF, 0x15DA2D49, 0x8CD37CF3, 0xFBD44C65,
			0x4DB26158, 0x3AB551CE, 0xA3BC0074, 0xD4BB30E2, 0x4ADFA541, 0x3DD895D7, 0xA4D1C46D, 0xD3D6F4FB,
			0x4369E96A, 0x346ED9FC, 0xAD678846, 0xDA60B8D0, 0x44042D73, 0x33031DE5, 0xAA0A4C5F, 0xDD0D7CC9,
			0x5005713C, 0x270241AA, 0xBE0B1010, 0xC90C2086, 0x5768B525, 0x206F85B3, 0xB966D409, 0xCE61E49F,
			0x5EDEF90E, 0x29D9C998, 0xB0D09822, 0xC7D7A8B4, 0x59B33D17, 0x2EB40D81, 0xB7BD5C3B, 0xC0BA6CAD,
			0xEDB88320, 0x9ABFB3B6, 0x03B6E20C, 0x74B1D29A, 0xEAD54739, 0x9DD277AF, 0x04DB2615, 0x73DC1683,
			0xE3630B12, 0x94643B84, 0x0D6D6A3E, 0x7A6A5AA8, 0xE40ECF0B, 0x9309FF9D, 0x0A00AE27, 0x7D079EB1,
			0xF00F9344, 0x8708A3D2, 0x1E01F268, 0x6906C2FE, 0xF762575D, 0x806567CB, 0x196C3671, 0x6E6B06E7,
			0xFED41B76, 0x89D32BE0, 0x10DA7A5A, 0x67DD4ACC, 0xF9B9DF6F, 0x8EBEEFF9, 0x17B7BE43, 0x60B08ED5,
			0xD6D6A3E8, 0xA1D1937E, 0x38D8C2C4, 0x4FDFF252, 0xD1BB67F1, 0xA6BC5767, 0x3FB506DD, 0x48B2364B,
			0xD80D2BDA, 0xAF0A1B4C, 0x36034AF6, 0x41047A60, 0xDF60EFC3, 0xA867DF55, 0x316E8EEF, 0x4669BE79,
			0xCB61B38C, 0xBC66831A, 0x256FD2A0, 0x5268E236, 0xCC0C7795, 0xBB0B4703, 0x220216B9, 0x5505262F,
			0xC5BA3BBE, 0xB2BD0B28, 0x2BB45A92, 0x5CB36A04, 0xC2D7FFA7, 0xB5D0CF31, 0x2CD99E8B, 0x5BDEAE1D,
			0x9B64C2B0, 0xEC63F226, 0x756AA39C, 0x026D930A, 0x9C0906A9, 0xEB0E363F, 0x72076785, 0x05005713,
			0x95BF4A82, 0xE2B87A14, 0x7BB12BAE, 0x0CB61B38, 0x92D28E9B, 0xE5D5BE0D, 0x7CDCEFB7, 0x0BDBDF21,
			0x86D3D2D4, 0xF1D4E242, 0x68DDB3F8, 0x1FDA836E, 0x81BE16CD, 0xF6B9265B, 0x6FB077E1, 0x18B74777,
			0x88085AE6, 0xFF0F6A70, 0x66063BCA, 0x11010B5C, 0x8F659EFF, 0xF862AE69, 0x616BFFD3, 0x166CCF45,
			0xA00AE278, 0xD70DD2EE, 0x4E048354, 0x3903B3C2, 0xA7672661, 0xD06016F7, 0x4969474D, 0x3E6E77DB,
			0xAED16A4A, 0xD9D65ADC, 0x40DF0B66, 0x37D83BF0, 0xA9BCAE53, 0xDEBB9EC5, 0x47B2CF7F, 0x30B5FFE9,
			0xBDBDF21C, 0xCABAC28A, 0x53B39330, 0x24B4A3A6, 0xBAD03605, 0xCDD70693, 0x54DE5729, 0x23D967BF,
			0xB3667A2E, 0xC4614AB8, 0x5D681B02, 0x2A6F2B94, 0xB40BBE37, 0xC30C8EA1, 0x5A05DF1B, 0x2D02EF8D
		};


		uint32_t BGS_Str_CRC32(const char* string)
		{
			uint32_t output = 0;
			const char* c = string;
			
			while (*c != 0x00)
			{
				output = (output >> 8u) ^ _bgs_string_lookup_table_crc32[(output ^ static_cast<unsigned char>(*c)) & 0xFFU];
				c++;
			}

			return output;
		}

        struct HashRegistry::Impl
        {
            ///@private
            std::unordered_map<uint32_t, std::string> _registeredHashes;
        };

        HashRegistry::HashRegistry() : pImpl(new Impl())
        {

        }
        HashRegistry::~HashRegistry()
        {
            if (pImpl)
            {
                delete pImpl;
                pImpl = nullptr;
            }
        }
        HashRegistry& HashRegistry::getInstance()
        {
            static HashRegistry instance;
            return instance;
        }
		const char* HashRegistry::getKnownHashString(uint32_t hash)
		{
			if (_known_string_hash_list.contains(hash))
				return _known_string_hash_list.at(hash).c_str();

            if (pImpl->_registeredHashes.contains(hash))
                return pImpl->_registeredHashes.at(hash).c_str();

			return nullptr;
		}
        bool HashRegistry::isKnownHash(uint32_t hash) const
        {
            return _known_string_hash_list.contains(hash) || pImpl->_registeredHashes.contains(hash);
        }
        uint32_t HashRegistry::registerHash(const char* string)
        {
            uint32_t hash = BGS_Str_CRC32(string);

            if (_known_string_hash_list.contains(hash))
                return hash;

            if (!pImpl->_registeredHashes.contains(hash))
            {
                pImpl->_registeredHashes[hash] = string;
            }

            return hash;
        }
        Utilities::VectorContainer<Utilities::StringContainer> HashRegistry::getRegisteredStrings() const
        {
            Utilities::VectorContainer<Utilities::StringContainer> output;
            
            output.reserve(pImpl->_registeredHashes.size());

            for (auto& entry : pImpl->_registeredHashes)
            {
                output.push_back(entry.second.c_str());
            }

            return output;
        }
}
}