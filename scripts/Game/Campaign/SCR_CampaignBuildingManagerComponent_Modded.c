modded class SCR_CampaignBuildingManagerComponent
{
	[RplProp()]
	protected ref array<vector> m_placedFOBs = new array<vector>();
	
	[RplProp()]
	protected ref array<vector> m_builtFOBs = new array<vector>();
	
	//------------------------------------------------------------------------------------------------
	
	array<vector> GetPlacedFOBs()
	{
		return m_placedFOBs;
	}
	
	void AddPlacedFOB(vector fobPos)
	{
		if (IsProxy())
			return;
		
		m_placedFOBs.Insert(fobPos);
		Replication.BumpMe();
	}
	
	void RemovePlacedFOB(vector pos)
	{
		if (IsProxy())
			return;
		
		int index = m_placedFOBs.Find(pos);
		if (index != -1)
		{
			m_placedFOBs.Remove(index);
			Replication.BumpMe();
			return;
		}
		
		FallbackRemovePlacedFOB(pos);
	}
	
	//------------------------------------------------------------------------------------------------
	
	void AddBuiltFOB(vector fobPos)
	{
		if (IsProxy())
			return;
		
		m_builtFOBs.Insert(fobPos);
		Replication.BumpMe();
	}
	
	// ---------------------------------------------------------------------------------------
	
	void BuildFOB(EntitySpawnParams spawnParams, SCR_ECampaignFaction eFaction)
	{
		if (IsProxy())
			return;
		
		Resource resource = Resource.Load("{1391CE8C0E255636}Prefabs/Systems/MilitaryBase/ConflictMilitaryBase.et");
		if (!resource.IsValid())
			return;
		
		SCR_GameModeCampaign campaign = SCR_GameModeCampaign.GetInstance();
		if (!campaign)
			return;
		
		SCR_CampaignFaction fobFaction = campaign.GetFactionByEnum(eFaction);
		if (!fobFaction)
			return;
		
		IEntity fob;
		fob = GetGame().SpawnEntityPrefab(resource, GetGame().GetWorld(), spawnParams);
		if (!fob)
			return;
		
		RemoveAIGroupsFromFOB(fob);
		
		SCR_CampaignMilitaryBaseComponent campaignBaseComp = SCR_CampaignMilitaryBaseComponent.Cast(fob.FindComponent(SCR_CampaignMilitaryBaseComponent));
		SCR_CoverageRadioComponent radioCoverageComp = SCR_CoverageRadioComponent.Cast(fob.FindComponent(SCR_CoverageRadioComponent));
		if (!campaignBaseComp || !radioCoverageComp)
		{
			RplComponent.DeleteRplEntity(fob, true);
			return;
		}

		campaignBaseComp.SpawnBuilding(campaign.GetFactionByEnum(eFaction).GetBuildingPrefab(EEditableEntityLabel.SERVICE_HQ), spawnParams.Transform[3], Math3D.MatrixToAngles(spawnParams.Transform), true);	
		campaignBaseComp.InitializeFOB(fobFaction);
		radioCoverageComp.InitializeRadioFOB();
		
		RemovePlacedFOB(spawnParams.Transform[3]);
		AddBuiltFOB(spawnParams.Transform[3]);
		
		if (RplComponent.Cast(GetOwner().FindComponent(RplComponent)))
			Rpc(RpcUpdateRadioCoverage);
	}
	
	// ---------------------------------------------------------------------------------------
	
	void RemoveAIGroupsFromFOB(IEntity newFob)
	{
		if (!newFob)
			return;
	
		IEntity child = newFob.GetChildren();
		while (child)
		{
			SCR_AmbientPatrolSpawnPointComponent patrolComp = SCR_AmbientPatrolSpawnPointComponent.Cast(child.FindComponent(SCR_AmbientPatrolSpawnPointComponent));
			if (patrolComp)
				patrolComp.DespawnPatrol();
			
			child = child.GetSibling();
		}
	}
	
	// ---------------------------------------------------------------------------------------
	
	[RplRpc(RplChannel.Reliable, RplRcver.Broadcast)]
	void RpcUpdateRadioCoverage()
	{
		GetGame().GetCallqueue().CallLater(SCR_RadioCoverageSystem.UpdateAll, SCR_GameModeCampaign.DEFAULT_DELAY, false, false);
	}
	
	// ---------------------------------------------------------------------------------------
	
	bool IsDistanceAwayFromAnyFOB(vector newPos, float distance)
	{
		if (newPos == vector.Zero)
			return false;
	
		vector fobPos = FindClosestFOB(newPos);
		if (fobPos == vector.Zero)
			return true;
	
		return vector.Distance(newPos, fobPos) >= distance;
	}
	
	vector FindClosestFOB(vector playerPos)
	{
		vector nearestPos = vector.Zero;
		float nearestDistanceSq = float.MAX;
	
		foreach (vector placedPos : m_placedFOBs)
		{
			float distSq = vector.DistanceSq(playerPos, placedPos);
			if (distSq < nearestDistanceSq)
			{
				nearestDistanceSq = distSq;
				nearestPos = placedPos;
			}
		}
		
		foreach (vector builtPos : m_builtFOBs)
		{
			float distSq = vector.DistanceSq(playerPos, builtPos);
			if (distSq < nearestDistanceSq)
			{
				nearestDistanceSq = distSq;
				nearestPos = builtPos;
			}
		}
	
		return nearestPos;
	}
	
	// ---------------------------------------------------------------------------------------
	
	// these never ran in my testing but we'll leave it in just in case 
	
	void FallbackRemovePlacedFOB(vector pos)
	{
		const float THRESHOLD = 1.0;
		for (int i = 0; i < m_placedFOBs.Count(); i++)
		{
			vector fob = m_placedFOBs[i];
			float distSq = vector.DistanceSq(fob, pos);
	
			PrintFormat("[FallbackRemoveFOB] Comparing %1 with %2 | DistSq: %3", pos, fob, distSq);
	
			if (distSq < THRESHOLD)
			{
				PrintFormat("[FallbackRemoveFOB] Proximity match found. Removing FOB at: %1", fob);
				m_placedFOBs.Remove(i);
				Replication.BumpMe();
				return;
			}
		}
	
		Print("[FallbackRemoveFOB] No match found even with proximity fallback.");
	}
}