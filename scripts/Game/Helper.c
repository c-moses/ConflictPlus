class Helper
{
	static vector StringToVector(string s)
	{
		string clean = s;
		clean.Replace("<", "");
		clean.Replace(">", "");
	
		array<string> parts = {};
		clean.Split(",", parts, true);
	
		if (parts.Count() != 3)
			return vector.Zero;
	
		return Vector(parts[0].ToFloat(), parts[1].ToFloat(), parts[2].ToFloat());
	}
}