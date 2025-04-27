modded class SCR_CampaignBuildingDisassemblyUserAction
{
	// The user action is shown when the preview is visible - means player has a building tool.
	override bool CanBeShownScript(IEntity user)
	{									
		if (m_DoNotDisassemble && !m_EditableEntity.IsFOB())
			return false;
		
		if (m_bSameFactionDisassembleOnly && !IsPlayerFactionSame(user))
			return false;
				
		if (!m_GadgetManager)
		{
			m_GadgetManager = SCR_GadgetManagerComponent.GetGadgetManager(user);
			
			SCR_PlayerController playerController = SCR_PlayerController.Cast(GetGame().GetPlayerController());
			if (playerController)
				playerController.m_OnControlledEntityChanged.Insert(SetNewGadgetManager);
			
			return false;
		}
					
		if (!SCR_CampaignBuildingGadgetToolComponent.Cast(m_GadgetManager.GetHeldGadgetComponent()))
			return false;
		
		// The user action is on entity with composition component, show it if the composition is spawned.
		if (GetOwner() == GetOwner().GetRootParent())
			return m_bCompositionSpawned;
		
		if (m_BuildAction && !m_BuildAction.IsShown())
			return false;
		
		return m_LayoutComponent;
	}
}