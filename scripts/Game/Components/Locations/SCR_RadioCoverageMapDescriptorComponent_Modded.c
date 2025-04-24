modded class SCR_RadioCoverageMapDescriptorComponent
{
	override protected void CreateLinks()
	{
		MapItem mapItem = Item();

		if (!mapItem)
			return;

		SCR_Faction playerFaction = SCR_Faction.Cast(SCR_FactionManager.SGetLocalPlayerFaction());

		if (!playerFaction)
			return;

		string playerFactionEncryption = playerFaction.GetFactionRadioEncryptionKey();

		if (playerFactionEncryption.IsEmpty())
			return;

		string radioEncryption = m_Radio.GetEncryptionKey();
		SCR_ERadioCoverageStatus coverage =  m_Radio.GetCoverageByEncryption(radioEncryption);

		if (radioEncryption != playerFactionEncryption)
			return;

		array<SCR_CoverageRadioComponent> radios = {};
		m_Radio.GetRadiosInRange(radios);
		
		SCR_RadioCoverageMapDescriptorComponent target;
		MapItem targetItem;
		array<MapLink> targetLinks = {};
		MapLink link;

		foreach (SCR_CoverageRadioComponent radio : radios)
		{
			target = SCR_RadioCoverageMapDescriptorComponent.Cast(radio.GetOwner().FindComponent(SCR_RadioCoverageMapDescriptorComponent));

			if (!target)
				continue;

			targetItem = target.Item();

			if (!targetItem || !targetItem.IsVisible())
				continue;

			bool linked;
			targetLinks.Clear();
			targetItem.GetLinks(targetLinks);

			// Don't create two links between the same bases
			foreach (MapLink targetLink : targetLinks)
			{
				if (targetLink.Target() == mapItem)
				{
					linked = true;
					break;
				}
			}

			if (linked)
				continue;

			link = mapItem.LinkTo(targetItem);
			ColorMapLink(link, false);
		}
	}
}