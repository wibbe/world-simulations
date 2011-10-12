
#include "game.h"


void solve_water_flow(float dt, float * base, float * in, float * out, int width, int height)
{
	const int offset[8] = { -width - 1, -width, -width + 1, -1, 1, width - 1, width, width + 1 };
	const int size = width * height;
	float * base_it = base;
	float * in_it = in;
	float * out_it = out;
	const float * base_end = base + size;
	int i;
	float h_diff[8];
	
	// Reset output buffer
	for (i = 0; i < size; ++i, out_it++)
		*out_it = 0.0f;
	
	out_it = out;
	
	while (base_it != base_end)
	{
		float h = *base_it + *in_it;
		float sum = 0.0f;
		
		// Calculate sum
		for (i = 0; i < 8; ++i)
		{
			float * offset_base = base_it + offset[i];
			
			if (offset_base >= base && offset_base < base_end)
			{
				float * offset_water = in_it + offset[i];
				float diff = h - (*offset_base + *offset_water);
				h_diff[i] = MAX(diff, 0.0f);
				if (h_diff[i] > 0.0f)
					sum += h_diff[i];
			}
		}
		
		// Distribute water
		float water_dist = MIN(sum, *in_it) * dt;
		*out_it += *in_it - water_dist;
		
		if (water_dist > 0.0f)
		{
			for (i = 0; i < 8; ++i)
			{
				float * offset_base = base_it + offset[i];
			
				if (offset_base >= base && offset_base < base_end)
				{
					float * offset_out = out_it + offset[i];
					*offset_out += water_dist * (h_diff[i] / sum);
				}			
			}
		}
		
		base_it++;
		in_it++;
		out_it++;
	}
}
