modded class SCR_CoverageRadioComponent
{
	void InitializeRadioFOB()
	{
		if (m_sSavedEncryptionKey.IsEmpty())
			return;

		Ping(m_sSavedEncryptionKey, false, false);
		Ping(m_sSavedEncryptionKey, true, true);
		FinishCoverageUpdate(m_sSavedEncryptionKey);
	}
}