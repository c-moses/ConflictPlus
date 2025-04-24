class FOB_Helper
{
	static ResourceName m_US_FOB = "{0E30CC8B7D39AD67}PrefabsEditable/Auto/Compositions/Slotted/SlotFlatSmall/Headquarters_US_Modded.et";
	static ResourceName m_USSR_FOB = "{2643ED049A8C622C}PrefabsEditable/Auto/Compositions/Slotted/SlotFlatSmall/Headquarters_USSR_Modded.et";
	static ResourceName m_FIA_FOB = "{FA1346F8CC67F025}PrefabsEditable/Auto/Compositions/Slotted/SlotFlatSmall/Headquarters_FIA_Modded.et";
	
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
}