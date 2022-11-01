#include "AlienTab.h"

AlienTab::AlienTab():
	Actor()
{
	for (int li = 0 ; li < 5 ; li++ )
	{
		for (int co = 0; co < 11; co++)
		{
			aliens[li][co] = new Alien();

		}
	}
}
