modded class SCR_EditableEntityComponent
{
	[RplProp()]
	protected bool m_isFob;
	
	void SetIsFOB(bool isFob)
	{
		m_isFob = isFob;
		Replication.BumpMe();
	}
	
	bool IsFOB()
	{
		return m_isFob;
	}
}