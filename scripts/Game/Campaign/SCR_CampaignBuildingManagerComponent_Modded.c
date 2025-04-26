modded class SCR_CampaignBuildingManagerComponent
{
	protected ref map<string, SyncFOB> m_fobStates = new map<string, SyncFOB>();
	
	// -------------------------------------------------------------------------------------------------------
	// --> Client
	
	void HandleAddFOB(vector fobPos, SyncFOB eSync)
	{
		if (!fobPos || fobPos == vector.Zero || eSync == SyncFOB.REMOVE_PLACED_FOB || eSync == SyncFOB.REMOVE_BUILT_FOB)
			return;
	
		string key = fobPos.ToString();
	
		if (m_fobStates.Contains(key))
			m_fobStates.Remove(key);
	
		m_fobStates.Set(key, eSync);
	}
	
	void HandleRemoveFOB(vector fobPos, SyncFOB eSync)
	{
		if (!fobPos || fobPos == vector.Zero || eSync == SyncFOB.ADD_PLACED_FOB || eSync == SyncFOB.ADD_BUILT_FOB)
			return;
	
		string key = fobPos.ToString();
	
		SyncFOB existingStatus;
		if (m_fobStates.Find(key, existingStatus))
		{
			if (FOB_Helper.IsRemovingFOB(eSync) || existingStatus == eSync)
				m_fobStates.Remove(key);
		}
	}
	
	// -------------------------------------------------------------------------------------------------------
	// --> Client
	
	[RplRpc(RplChannel.Reliable, RplRcver.Broadcast)]
	void RpcSyncFOBs(vector hqPos, SyncFOB eSync)
	{
		if (FOB_Helper.IsRemovingFOB(eSync))
			HandleRemoveFOB(hqPos, eSync);
	
		if (FOB_Helper.IsAddingFOB(eSync))
			HandleAddFOB(hqPos, eSync);
	}
	
	//------------------------------------------------------------------------------------------------
	// --> Server
	
	void AddPlacedFOB(vector fobPos)
	{
		if (IsProxy())
			return;
	
		if (fobPos == vector.Zero)
			return;
	
		PrintFormat("[SCR_CampaignBuildingManagerComponent :: AddPlacedFOB] : fobPos=%1", fobPos);
	
		Rpc(RpcSyncFOBs, fobPos, SyncFOB.ADD_PLACED_FOB);
	}
	
	
	void RemovePlacedFOB(vector fobPos)
	{
		if (IsProxy())
			return;
	
		if (fobPos == vector.Zero)
			return;
	
		PrintFormat("[SCR_CampaignBuildingManagerComponent :: RemovePlacedFOB] : fobPos=%1", fobPos);
	
		Rpc(RpcSyncFOBs, fobPos, SyncFOB.REMOVE_PLACED_FOB);
	}
	
	void AddBuiltFOB(vector fobPos)
	{
		if (IsProxy())
			return;
		
		if (fobPos == vector.Zero)
			return;
		
		PrintFormat("[SCR_CampaignBuildingManagerComponent :: AddBuiltFOB] : fobPos=%1", fobPos);
		
		Rpc(RpcSyncFOBs, fobPos, SyncFOB.ADD_BUILT_FOB);
	}
	
	// ---------------------------------------------------------------------------------------
	// --> Server
	
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
	// --> Server
	
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
	// --> Server
	
	[RplRpc(RplChannel.Reliable, RplRcver.Broadcast)]
	void RpcUpdateRadioCoverage()
	{
		GetGame().GetCallqueue().CallLater(SCR_RadioCoverageSystem.UpdateAll, SCR_GameModeCampaign.DEFAULT_DELAY, false, false);
	}
	
	// ---------------------------------------------------------------------------------------
	// --> Utility
	
	array<vector> GetPlacedFOBs()
	{
		array<vector> placedFobs = {};
	
		if (!m_fobStates || m_fobStates.IsEmpty())
			return placedFobs;
	
		foreach (string fobKey, SyncFOB status : m_fobStates)
		{
			if (status == SyncFOB.ADD_PLACED_FOB)
			{
				vector fobPos = Helper.StringToVector(fobKey);
				placedFobs.Insert(fobPos);
			}
		}
	
		return placedFobs;
	}
	
	bool IsBuiltFOB(vector fobPos)
	{
		if (!m_fobStates || m_fobStates.IsEmpty() || fobPos == vector.Zero)
			return false;
		
		SyncFOB state;
		string key = fobPos.ToString();
	
		if (m_fobStates.Find(key, state))
			return state == SyncFOB.ADD_BUILT_FOB;
	
		return false;
	}
	
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
	
		if (!m_fobStates || m_fobStates.IsEmpty())
			return nearestPos;
	
		foreach (string fobKey, SyncFOB status : m_fobStates)
		{
			if (status != SyncFOB.ADD_PLACED_FOB && status != SyncFOB.ADD_BUILT_FOB)
				continue;
	
			vector fobPos = Helper.StringToVector(fobKey);
			float distSq = vector.DistanceSq(playerPos, fobPos);
			if (distSq < nearestDistanceSq)
			{
				nearestDistanceSq = distSq;
				nearestPos = fobPos;
			}
		}
	
		return nearestPos;
	}
}