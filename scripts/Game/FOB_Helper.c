enum SyncFOB
{
	ADD_PLACED_FOB,
	ADD_BUILT_FOB,
	REMOVE_PLACED_FOB,
	REMOVE_BUILT_FOB,
}

class FOB_Helper
{
	static ResourceName m_US_FOB = "{0E30CC8B7D39AD67}PrefabsEditable/Auto/Compositions/Slotted/SlotFlatSmall/Headquarters_US_Modded.et";
	static ResourceName m_USSR_FOB = "{2643ED049A8C622C}PrefabsEditable/Auto/Compositions/Slotted/SlotFlatSmall/Headquarters_USSR_Modded.et";
	static ResourceName m_FIA_FOB = "{FA1346F8CC67F025}PrefabsEditable/Auto/Compositions/Slotted/SlotFlatSmall/Headquarters_FIA_Modded.et";
	
	static ResourceName m_US_HQ = "{9771776551C306E0}PrefabsEditable/Auto/Compositions/Slotted/SlotFlatSmall/E_Headquarters_S_US_01.et";
	static ResourceName m_USSR_HQ = "{847605BB95F1F066}PrefabsEditable/Auto/Compositions/Slotted/SlotFlatSmall/E_Headquarters_S_USSR_01.et";
	static ResourceName m_FIA_HQ = "{7CB2EB208F93C6D4}PrefabsEditable/Auto/Compositions/Slotted/SlotFlatSmall/E_Headquarters_S_FIA_01.et";
	
	// -------------------------------------------------------------------------------------------------------
	
	static bool IsHQ(ResourceName resName)
	{
		return resName == m_US_HQ || resName == m_USSR_HQ || resName == m_FIA_HQ;
	}
	
	// -------------------------------------------------------------------------------------------------------
	
	static bool IsFOB(ResourceName resName)
	{
		return resName == m_US_FOB || resName == m_USSR_FOB || resName == m_FIA_FOB;
	}
	
	static bool IsFOB_US(ResourceName resName)
	{
		return resName == m_US_FOB;
	}
	
	static bool IsFOB_USSR(ResourceName resName)
	{
		return resName == m_USSR_FOB;
	}
	
	static bool IsFOB_FIA(ResourceName resName)
	{
		return resName == m_FIA_FOB;
	}
	
	// -------------------------------------------------------------------------------------------------------
	
	static bool IsAddingFOB(SyncFOB eSync)
	{
		return eSync == SyncFOB.ADD_PLACED_FOB || eSync == SyncFOB.ADD_BUILT_FOB;
	}
	
	static bool IsRemovingFOB(SyncFOB eSync)
	{
		return eSync == SyncFOB.REMOVE_PLACED_FOB || eSync == SyncFOB.REMOVE_BUILT_FOB;
	}
}
