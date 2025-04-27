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
	
	void RemoveRadioFOB()
	{
	    if (!m_sSavedEncryptionKey.IsEmpty())
	    {
	        Ping(m_sSavedEncryptionKey, false, false);
	        FinishCoverageUpdate(m_sSavedEncryptionKey);
	    }
	}
}