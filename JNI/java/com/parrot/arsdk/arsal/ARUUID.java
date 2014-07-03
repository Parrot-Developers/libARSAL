package com.parrot.arsdk.arsal;

import java.util.UUID;

public class ARUUID 
{
	public static String getShortUuid(UUID uuid)
	{
		String shortUuid = uuid.toString();
		
		if (shortUuid.length() > 4)
		{
			shortUuid = shortUuid.substring(4, 8);
		}
		return shortUuid;
	}
}
