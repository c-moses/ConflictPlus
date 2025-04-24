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
		if (FOB_Helper.IsFOB(resName))
		{
			buildingManagerComponent.AddPlacedFOB(entity.GetOwner().GetOrigin());
		}
	}
	
	//	-------------------------------------------------------------------------------
	
	override void OnDelete(IEntity owner)
	{
		if (FOB_Helper.IsFOB(owner.GetPrefabData().GetPrefabName()))
			OnDeletedFOB(owner.GetOrigin());
		
		super.OnDelete(owner);
	}
	
	void OnDeletedFOB(vector fobPos)
	{
		if (IsProxy())
			return;
		
		if (fobPos == vector.Zero)
			return;
		
		SCR_GameModeCampaign campaign = SCR_GameModeCampaign.GetInstance();
		if (!campaign)
			return;
	
		SCR_CampaignBuildingManagerComponent buildingMgr = SCR_CampaignBuildingManagerComponent.Cast(campaign.FindComponent(SCR_CampaignBuildingManagerComponent));
		if (!buildingMgr)
			return;
		
		buildingMgr.RemovePlacedFOB(fobPos);
	}
}