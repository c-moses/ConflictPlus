modded class SCR_CampaignBuildingLayoutComponent
{
	override void SpawnComposition()
	{
		IEntity ent = GetOwner().GetRootParent();
		if (!ent)
			return;

		SCR_EditorLinkComponent linkComponent = SCR_EditorLinkComponent.Cast(ent.FindComponent(SCR_EditorLinkComponent));
		if (!linkComponent)
			return;
		
		SCR_CampaignBuildingManagerComponent buildingMgr = GetBuildingManagerComponent();
		if (!buildingMgr)
			return;
		
		EntitySpawnParams spawnParams = new EntitySpawnParams;
		ent.GetWorldTransform(spawnParams.Transform);
	
		ResourceName resName = GetCompositionResourceName(m_iPrefabId);
		if (FOB_Helper.IsFOB(resName))
		{
			if (FOB_Helper.IsFOB_USSR(resName))
				buildingMgr.BuildFOB(spawnParams, SCR_ECampaignFaction.OPFOR);
			else if (FOB_Helper.IsFOB_US(resName))
				buildingMgr.BuildFOB(spawnParams, SCR_ECampaignFaction.BLUFOR);
			else if (FOB_Helper.IsFOB_FIA(resName))
				buildingMgr.BuildFOB(spawnParams, SCR_ECampaignFaction.INDFOR);
			
			LockCompositionInteraction();
			return;
		}
		
		linkComponent.SpawnComposition();
			
		SCR_EditableEntityComponent entity = SCR_EditableEntityComponent.GetEditableEntity(ent);
		if (entity)
		{
			SCR_EditorPreviewParams params = SCR_EditorPreviewParams.CreateParams(spawnParams.Transform, verticalMode: EEditorTransformVertical.TERRAIN);
			SCR_RefPreviewEntity.SpawnAndApplyReference(entity, params);
		}
		
		LockCompositionInteraction();
		SCR_EntityHelper.DeleteEntityAndChildren(GetOwner());
	}
}