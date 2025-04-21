modded class SCR_CampaignMilitaryBaseComponent
{
	string m_world = "";
	
	override void OnPostInit(IEntity owner)
	{
		super.OnPostInit(owner);
		m_world = GetGame().GetWorldFile();
	}
	
	override void RecalculateRadioRange() // this is a vanill method 
	{
		float range = m_fRadioRangeDefault;
		float thisRange;
		array<SCR_ServicePointComponent> antennas = {};
		GetServicesByType(antennas, SCR_EServicePointType.RADIO_ANTENNA);
		BaseRadioComponent radio;

		bool seeding = m_world.StartsWith("worlds/Everon_Seed");
		if (!seeding)
		{
			// Find antenna services, read max radio range from the radio component on their owners
			foreach (SCR_ServicePointComponent service : antennas)
			{
				SCR_AntennaServicePointComponent antenna = SCR_AntennaServicePointComponent.Cast(service);
				radio = BaseRadioComponent.Cast(antenna.GetOwner().FindComponent(BaseRadioComponent));
		
				if (!radio)
					continue;
		
				// Turn off the radio so we don't hit performance too much with every antenna built
				if (radio.IsPowered())
					radio.SetPower(false);
		
				thisRange = GetRelayRadioRange(radio);
		
				if (thisRange > range)
					range = thisRange;
			}
		}

		if (m_fRadioRange == range)
			return;

		// Instead of relying on antenna radio which has been turned off, apply the antenna's signal range to the radio component on the base itself
		if (m_RadioComponent)
		{
			RelayTransceiver transceiver;

			for (int i = 0, count = m_RadioComponent.TransceiversCount(); i < count; i++)
			{
				transceiver = RelayTransceiver.Cast(m_RadioComponent.GetTransceiver(i));

				if (!transceiver)
					continue;

				transceiver.SetRange(range);
			}
		}

		m_fRadioRange = range;
		Replication.BumpMe();
		OnRadioRangeChanged();
	}
	
	override protected void HandleSpawnPointFaction()
	{
		if (!m_SpawnPoint)
			return;

		SCR_CampaignFaction owner = GetCampaignFaction();
		FactionKey currentKey = m_SpawnPoint.GetFactionKey();
		FactionKey ownerKey;
		FactionKey finalKey;

		if (owner)
		{
			ownerKey = owner.GetFactionKey();
			finalKey = ownerKey;
		}

		if (ownerKey == FactionKey.Empty)
		{
			if (currentKey != FactionKey.Empty)
				m_SpawnPoint.SetFactionKey(FactionKey.Empty);

			return;
		}

		ChimeraWorld world = GetOwner().GetWorld();
		if (world.GetServerTimestamp().Less(m_fRespawnAvailableSince) && !m_bIsHQ)
			finalKey = FactionKey.Empty;

		if (finalKey == currentKey)
			return;

		m_SpawnPoint.SetFactionKey(finalKey);
	}
	
	override protected void SupplyIncomeTimer(bool reset = false)
	{
		SCR_GameModeCampaign campaign = SCR_GameModeCampaign.GetInstance();

		if (!campaign)
			return;

		ChimeraWorld world = GetOwner().GetWorld();

		if (!world)
		{
			m_iSupplyRegenAmount = 0;
			Replication.BumpMe();
			return;
		}

		float curTime = world.GetWorldTime();

		if (reset)
		{
			m_fSuppliesArrivalTime = curTime + (campaign.GetSuppliesArrivalInterval() * 1000);
			return;
		}

		SCR_CampaignFaction owner = GetCampaignFaction();

		if (!owner || !m_SpawnPoint || (m_CapturingFaction && m_CapturingFaction != owner) || world.GetServerTimestamp().Less(m_fRespawnAvailableSince))
		{
			m_iSupplyRegenAmount = 0;
			Replication.BumpMe();
			return;
		}

		if (curTime >= m_fSuppliesArrivalTime)
		{
			AddRegularSupplyPackage(owner);
			m_fSuppliesArrivalTime = curTime + (SCR_GameModeCampaign.GetInstance().GetSuppliesArrivalInterval() * 1000);
		}
	}
}