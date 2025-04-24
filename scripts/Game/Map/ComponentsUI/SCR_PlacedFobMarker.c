class SCR_PlacedFobMarker : SCR_MapUIBaseComponent
{
	/*protected ref array<Widget> m_placedFOBs = new array<Widget>();
	float m_fobUpdateTimer = 0;
	const float UPDATE_INTERVAL = 1.0;
	
	override protected void OnMapOpen(MapConfiguration config)
	{	
		super.OnMapOpen(config);
		UpdatePlacedFOBs();
	}
	
	override void Update(float timeSlice)
	{
		super.Update(timeSlice);
		m_fobUpdateTimer += timeSlice;
		if (m_fobUpdateTimer >= UPDATE_INTERVAL)
		{
			UpdatePlacedFOBs();
			m_fobUpdateTimer = 0;
		}
	}
	
	SCR_CampaignBuildingManagerComponent GetBuildingManager()
	{
		SCR_GameModeCampaign campaign = SCR_GameModeCampaign.GetInstance();
		if (!campaign)
			return null;
		
		return SCR_CampaignBuildingManagerComponent.Cast(campaign.FindComponent(SCR_CampaignBuildingManagerComponent));
	}
	
	protected void ClearExistingMarkers()
	{
		foreach (Widget w : m_placedFOBs)
		{
			if (w)
				w.RemoveFromHierarchy();
		}
		
		m_placedFOBs.Clear();
	}
	
	protected void UpdatePlacedFOBs()
	{
		ClearExistingMarkers();
		
		SCR_CampaignBuildingManagerComponent buildingMgr = GetBuildingManager();
		if (!buildingMgr)
			return;
		
		array<vector> placedFobs = buildingMgr.GetPlacedFOBs();
		if (!placedFobs || placedFobs.IsEmpty())
			return;
	
		foreach (vector fobPos : placedFobs)
		{
			Widget wdgt = GetGame().GetWorkspace().CreateWidgets("{6E74BEC39BED5ABF}UI/layouts/PlacedFobMarker.layout", m_RootWidget);
			if (!wdgt)
				continue;
	
			ImageWidget image = ImageWidget.Cast(wdgt.FindAnyWidget("Image"));
			if (!image)
				continue;
	
			SCR_FactionManager factionManager = SCR_FactionManager.Cast(GetGame().GetFactionManager());
			Faction playerFaction;
			if (factionManager)
				playerFaction = factionManager.SGetLocalPlayerFaction();
	
			if (playerFaction)
				image.SetColor(Color.FromInt(playerFaction.GetFactionColor().PackToInt()));
	
			float x, y;
			SCR_MapEntity.GetMapInstance().WorldToScreen(fobPos[0], fobPos[2], x, y, true);
			
			x = GetGame().GetWorkspace().DPIUnscale(x);
			y = GetGame().GetWorkspace().DPIUnscale(y);
	
			FrameSlot.SetPos(wdgt, x, y);
	
			m_placedFOBs.Insert(wdgt);
		}
	}*/
}