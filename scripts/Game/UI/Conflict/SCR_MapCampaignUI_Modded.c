modded class SCR_MapCampaignUI
{
	protected ResourceName m_placedFobMarker = "{6E74BEC39BED5ABF}UI/layouts/PlacedFobMarker.layout";
	protected ref array<Widget> m_placedFobWidgets = new array<Widget>();
	
	override void OnMapOpen(MapConfiguration config)
	{
		super.OnMapOpen(config);
		ShowPlacedFOBs();
	}
	
	override void OnMapClose(MapConfiguration config)
	{
		super.OnMapClose(config);
		ClearPlacedFobMarkers();
	}
	
	protected void ShowPlacedFOBs()
	{
		SCR_CampaignBuildingManagerComponent mgr = SCR_CampaignBuildingManagerComponent.Cast(m_GameMode.FindComponent(SCR_CampaignBuildingManagerComponent));
		if (!mgr)
			return;
	
		foreach (vector fobPos : mgr.GetPlacedFOBs())
		{
			Widget marker = GetGame().GetWorkspace().CreateWidgets(m_placedFobMarker, m_wIconsContainer);
			if (!marker)
				continue;
	
			FobMarkerUI handler = FobMarkerUI.Cast(marker.FindHandler(FobMarkerUI));
			if (!handler)
				continue;
	
			handler.SetPos(fobPos);
			handler.SetParent(this);
	
			m_placedFobWidgets.Insert(marker);
			m_mIcons.Set(marker, handler);
		}
		
		UpdateIcons();
	}

	protected void ClearPlacedFobMarkers()
	{
		foreach (Widget w : m_placedFobWidgets)
		{
			if (!w)
				continue;
	
			m_mIcons.Remove(w);
			w.RemoveFromHierarchy();
		}
	
		m_placedFobWidgets.Clear();
	}
}