modded class SCR_CampaignBuildingPlacingObstructionEditorComponent
{
	//! Check entity position in given bounding box.
	override protected bool CheckVehiclePosition()
	{
		if (!m_PreviewEnt)
			return false;

		BaseWorld world = GetGame().GetWorld();
		if (!world)
			return false;

		vector transform[4];
		m_PreviewEnt.GetTransform(transform);

		vector outBoundMin, outBoundMax;
		m_PreviewEnt.GetPreviewBounds(outBoundMin, outBoundMax);
		outBoundMin[1] = HEIGHT_ABOVE_GROUND_VEHICLE_BUFFER;
		outBoundMin[0] = outBoundMin[0] * BOUNDING_BOX_VEHICLE_FACTOR;
		outBoundMin[2] = outBoundMin[2] * BOUNDING_BOX_VEHICLE_FACTOR;
				
		TraceOBB paramOBB = new TraceOBB();
		Math3D.MatrixIdentity3(paramOBB.Mat);
		paramOBB.Mat[0] = transform[0];
		paramOBB.Mat[1] = transform[1];
		paramOBB.Mat[2] = transform[2];
		paramOBB.Start = transform[3] + 0.05 * paramOBB.Mat[1];
		paramOBB.Flags = TraceFlags.ENTS | TraceFlags.OCEAN | TraceFlags.WORLD;
		paramOBB.Mins = outBoundMin;
		paramOBB.Maxs = outBoundMax;
		
		// First do the sea level check as it is cheap and don't need to continue with trace if the composition is in the sea.
		//float val = world.GetOceanBaseHeight();
		//if (m_PreviewEnt.GetOrigin()[1] < world.GetOceanBaseHeight() + SEA_LEVEL_OFFSET)
			//return false;
		
		// No check for the nearby previews as we don't want to allow player to build a cliping compositions
		if (!world.QueryEntitiesByOBB(outBoundMin, outBoundMax, transform, EvaluateBlockingEntity))
			return false;
		
		// Check if the placing isn't blocked because the origin of the preview is in water.
		//if (ChimeraWorldUtils.TryGetWaterSurfaceSimple(world, m_PreviewEnt.GetOrigin()))
			//return false;			
		
		return world.TracePosition(paramOBB, EvaluateBlockingEntity) > 0;
	}
}