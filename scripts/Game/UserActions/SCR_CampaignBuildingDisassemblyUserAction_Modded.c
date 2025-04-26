modded class SCR_CampaignBuildingDisassemblyUserAction
{
	protected override void Init(IEntity pOwnerEntity, GenericComponent pManagerComponent)
	{
		m_RootEntity = pOwnerEntity.GetRootParent();
				
		m_CompositionComponent = SCR_CampaignBuildingCompositionComponent.Cast(m_RootEntity.FindComponent(SCR_CampaignBuildingCompositionComponent));
		m_EditableEntity = SCR_EditableEntityComponent.Cast(m_RootEntity.FindComponent(SCR_EditableEntityComponent));
		m_LayoutComponent = SCR_CampaignBuildingLayoutComponent.Cast(pOwnerEntity.FindComponent(SCR_CampaignBuildingLayoutComponent));
					
		GetBuildingAction();
		SetEditorManager();
		
		BaseGameMode gameMode = GetGame().GetGameMode();
		if (!gameMode)
			return;

		SCR_CampaignBuildingManagerComponent buildingManagerComponent = SCR_CampaignBuildingManagerComponent.Cast(gameMode.FindComponent(SCR_CampaignBuildingManagerComponent));
		if (!buildingManagerComponent)
			return;
		
		if (m_CompositionComponent && GetOwner() == GetOwner().GetRootParent())
		{
			m_CompositionComponent.GetOnCompositionSpawned().Insert(OnCompositionSpawned);
			
			m_bSameFactionDisassembleOnly = buildingManagerComponent.CanDisassembleSameFactionOnly();
			m_bDisassembleOnlyWhenCapturing = buildingManagerComponent.CanDisassembleOnlyWhenCapturing();
			
			if (m_bSameFactionDisassembleOnly || m_bDisassembleOnlyWhenCapturing)
				m_CompositionComponent.GetOnBuilderSet().Insert(CacheFactionAffiliationComponent);
		}
			
		// Temporary solution how to prevent disassembly of HQ in Conflict.
		SCR_GameModeCampaign campaignGameMode = SCR_GameModeCampaign.Cast(GetGame().GetGameMode());
		if (!campaignGameMode || !m_EditableEntity)
			return;
		
		SCR_EditableEntityUIInfo editableEntityUIInfo = SCR_EditableEntityUIInfo.Cast(m_EditableEntity.GetInfo(GetOwner()));
		if (!editableEntityUIInfo)
			return;
		
		array<EEditableEntityLabel> entityLabels = {};
		editableEntityUIInfo.GetEntityLabels(entityLabels);
		
		// Only block disassembly of HQ if it's NOT part of a built FOB
		if (entityLabels.Contains(EEditableEntityLabel.SERVICE_HQ) && !buildingManagerComponent.IsBuiltFOB(m_RootEntity.GetOrigin()))
			m_DoNotDisassemble = true;
	}
	
	/* vanilla method here
	protected override void Init(IEntity pOwnerEntity, GenericComponent pManagerComponent)
	{
		m_RootEntity = pOwnerEntity.GetRootParent();
				
		m_CompositionComponent = SCR_CampaignBuildingCompositionComponent.Cast(m_RootEntity.FindComponent(SCR_CampaignBuildingCompositionComponent));
		m_EditableEntity = SCR_EditableEntityComponent.Cast(m_RootEntity.FindComponent(SCR_EditableEntityComponent));
		m_LayoutComponent = SCR_CampaignBuildingLayoutComponent.Cast(pOwnerEntity.FindComponent(SCR_CampaignBuildingLayoutComponent));
					
		GetBuildingAction();
		SetEditorManager();
		
		if (m_CompositionComponent && GetOwner() == GetOwner().GetRootParent())
		{
			m_CompositionComponent.GetOnCompositionSpawned().Insert(OnCompositionSpawned);
			
			BaseGameMode gameMode = GetGame().GetGameMode();
			if (!gameMode)
				return;
	
			SCR_CampaignBuildingManagerComponent buildingManagerComponent = SCR_CampaignBuildingManagerComponent.Cast(gameMode.FindComponent(SCR_CampaignBuildingManagerComponent));
			if (!buildingManagerComponent)
				return;
			
			m_bSameFactionDisassembleOnly = buildingManagerComponent.CanDisassembleSameFactionOnly();
			m_bDisassembleOnlyWhenCapturing = buildingManagerComponent.CanDisassembleOnlyWhenCapturing();
			
			if (m_bSameFactionDisassembleOnly || m_bDisassembleOnlyWhenCapturing)
				m_CompositionComponent.GetOnBuilderSet().Insert(CacheFactionAffiliationComponent);
		}
			
		// Temporary solution how to prevent disassembly of HQ in Conflict.
		SCR_GameModeCampaign campaignGameMode = SCR_GameModeCampaign.Cast(GetGame().GetGameMode());
		if (!campaignGameMode || !m_EditableEntity)
			return;
		
		SCR_EditableEntityUIInfo editableEntityUIInfo = SCR_EditableEntityUIInfo.Cast(m_EditableEntity.GetInfo(GetOwner()));
		if (!editableEntityUIInfo)
			return;
		
		array<EEditableEntityLabel> entityLabels = {};
		editableEntityUIInfo.GetEntityLabels(entityLabels);
		if (entityLabels.Contains(EEditableEntityLabel.SERVICE_HQ))
			m_DoNotDisassemble = true;
	}
	*/
}