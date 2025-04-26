modded class SCR_CampaignBuildingCompositionComponent
{
	override void SpawnCompositionLayout(int prefabId, SCR_EditableEntityComponent entity)
	{
		super.SpawnCompositionLayout(prefabId, entity);
		
		if (!entity)
			return;
	
		BaseGameMode gameMode = GetGame().GetGameMode();
		if (!gameMode)
			return;
	
		SCR_CampaignBuildingManagerComponent buildingManagerComponent = SCR_CampaignBuildingManagerComponent.Cast(gameMode.FindComponent(SCR_CampaignBuildingManagerComponent));
		if (!buildingManagerComponent)
			return;
	
		ResourceName resName = buildingManagerComponent.GetCompositionResourceName(prefabId);
		if (!FOB_Helper.IsFOB(resName))
			return;
	
		buildingManagerComponent.AddPlacedFOB(entity.GetOwner().GetOrigin());
	}
	
	//	-------------------------------------------------------------------------------
	
	override void OnDelete(IEntity owner)
	{
		ResourceName prefabName = owner.GetPrefabData().GetPrefabName();
		if (FOB_Helper.IsFOB(prefabName))
			OnDeletedFOB(owner);
	
		super.OnDelete(owner);
	}
	
	void OnDeletedFOB(IEntity entity)
	{
		if (IsProxy())
			return;
	
		if (!entity)
			return;
	
		SCR_GameModeCampaign campaign = SCR_GameModeCampaign.GetInstance();
		if (!campaign)
			return;
	
		SCR_CampaignBuildingManagerComponent buildingMgr = SCR_CampaignBuildingManagerComponent.Cast(campaign.FindComponent(SCR_CampaignBuildingManagerComponent));
		if (!buildingMgr)
			return;
	
		buildingMgr.RemovePlacedFOB(entity.GetOrigin());
	}
}