modded class SCR_CampaignMilitaryBaseManager
{
	bool IsDistanceAwayFromAnyBase(vector newPos, float distance)
	{
		if (newPos == vector.Zero)
			return false;
	
		SCR_CampaignMilitaryBaseComponent baseComp = FindClosestBase(newPos);
		if (!baseComp)
			return true;
	
		IEntity ent = baseComp.GetOwner();
		if (!ent)
			return true;
	
		return vector.Distance(newPos, ent.GetOrigin()) >= distance;
	}
}