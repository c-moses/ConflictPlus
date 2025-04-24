modded class SCR_PlacingEditorComponent
{
	// -------------------------------------------------------------------------------
	
 	override protected bool CanSelectEntityPrefab(ResourceName prefab, out EEditableEntityBudget blockingBudget, bool updatePreview = true, bool showBudgetMaxNotification = true)
	{
		if (FOB_Helper.IsFOB(prefab) && !CanCreateFOB())
			return false;
		
		return super.CanSelectEntityPrefab(prefab, blockingBudget, updatePreview, showBudgetMaxNotification);
	}
	
	override bool SetSelectedPrefab(ResourceName prefab = "", bool onConfirm = false, bool showBudgetMaxNotification = true, set<SCR_EditableEntityComponent> recipients = null)
	{
		if (FOB_Helper.IsFOB(prefab) && !CanCreateFOB())
			return false;
		
		return super.SetSelectedPrefab(prefab, onConfirm, showBudgetMaxNotification, recipients);
	}
	
	override bool CreateEntity(bool unselectPrefab = true, bool canBePlayer = false, SCR_EditableEntityComponent holder = null)
	{
		if (FOB_Helper.IsFOB(m_SelectedPrefab) && !CanCreateFOB())
			return false;
		
		return super.CreateEntity(unselectPrefab, canBePlayer, holder);
	}
	
	// -------------------------------------------------------------------------------
	
	bool CanCreateFOB()
	{
		IEntity player = GetGame().GetPlayerController().GetControlledEntity();
		if (!player)
			return false;

		vector pos = player.GetOrigin();

		SCR_GameModeCampaign campaign = SCR_GameModeCampaign.GetInstance();
		if (!campaign)
			return false;

		SCR_CampaignMilitaryBaseManager baseMgr = campaign.GetBaseManager();
		if (!baseMgr)
			return false;
		
		SCR_CampaignBuildingManagerComponent buildingMgr = SCR_CampaignBuildingManagerComponent.Cast(campaign.FindComponent(SCR_CampaignBuildingManagerComponent));
		if (!buildingMgr)
			return false;

		SCR_CampaignFaction playerFaction = SCR_CampaignFaction.Cast(SCR_FactionManager.SGetLocalPlayerFaction());
		if (!playerFaction)
			return false;

		if (!baseMgr.IsEntityInFactionRadioSignal(player, playerFaction))
		{
			SCR_NotificationsComponent.SendLocal(ENotification.FOB_PLACEMENT_RADIO);
			return false;
		}
		
		if (!baseMgr.IsDistanceAwayFromAnyBase(pos, 600))
		{
			SCR_NotificationsComponent.SendLocal(ENotification.FOB_PLACEMENT_DISTANCE);
			return false;
		}
		
		if (!buildingMgr.IsDistanceAwayFromAnyFOB(pos, 600))
		{
			SCR_NotificationsComponent.SendLocal(ENotification.FOB_PLACEMENT_DISTANCE);
			return false;
		}

		return true;
	}
}