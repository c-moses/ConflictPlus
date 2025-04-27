modded class SCR_CampaignBuildingNetworkComponent
{
	[RplRpc(RplChannel.Reliable, RplRcver.Server)]
	override protected void RpcAsk_DeleteCompositionByUserAction(RplId rplCompositionId)
	{
		IEntity composition = GetProviderFormRplId(rplCompositionId);
		if (!composition)
			return;
		
		PrintFormat("[SCR_CampaignBuildingNetworkComponent :: RpcAsk_DeleteCompositionByUserAction] called with IEntity composition %1", composition.GetPrefabData().GetPrefabName());
		
		if (FOB_Helper.IsHQ(composition.GetPrefabData().GetPrefabName()))
		{
			BaseGameMode gameMode = GetGame().GetGameMode();
			if (!gameMode)
				return;
				
			SCR_CampaignBuildingManagerComponent campaignBuildingComponent = SCR_CampaignBuildingManagerComponent.Cast(gameMode.FindComponent(SCR_CampaignBuildingManagerComponent));
			if (campaignBuildingComponent)
				campaignBuildingComponent.RemoveFOB(composition);
		}
		
		SCR_CampaignBuildingCompositionComponent buildingComponent = SCR_CampaignBuildingCompositionComponent.Cast(composition.FindComponent(SCR_CampaignBuildingCompositionComponent));
		if (buildingComponent)
			buildingComponent.SetCanPlaySoundOnDeletion(true);

		SCR_EditableEntityComponent editableEntity = SCR_EditableEntityComponent.Cast(composition.GetRootParent().FindComponent(SCR_EditableEntityComponent));
		if (editableEntity)
			editableEntity.Delete();
		else
			RplComponent.DeleteRplEntity(composition, false);
	}
}