
#include "game.h"


void solve_water_flow(float dt, float * rock, float * sand, float * in, float * out, int width, int height)
{
	//const int offset[8] = { -width - 1, -width, -width + 1, -1, 1, width - 1, width, width + 1 };
	const int offset[4] = { -width, -1, 1, width };
	const int size = width * height;
	float * rock_it = rock;
	float * sand_it = sand;
	float * in_it = in;
	float * out_it = out;
	const float * rock_end = rock + size;
	int i;
	float h_diff[8];
	
	// Reset output buffer
	for (i = 0; i < size; ++i, out_it++)
		*out_it = 0.0f;
	
	out_it = out;
	
	while (rock_it != rock_end)
	{
		float h = *rock_it + *sand_it + *in_it;
		float sum = 0.0f;
		
		// Calculate sum
		for (i = 0; i < 4; ++i)
		{
			float * offset_rock = rock_it + offset[i];
			
			if (offset_rock >= rock && offset_rock < rock_end)
			{
				float * offset_water = in_it + offset[i];
				float * offset_sand = sand_it + offset[i];
				float diff = h - (*offset_rock + *offset_sand + *offset_water);
				h_diff[i] = MAX(diff, 0.0f);
				if (diff > 0.0f)
					sum += h_diff[i];
			}
		}
		
		// Distribute water
		float water_dist = MIN(sum, *in_it) * dt;
		*out_it += *in_it - water_dist;
		
		if (water_dist > 0.0f)
		{
			for (i = 0; i < 4; ++i)
			{
				float * offset_rock = rock_it + offset[i];
			
				if (offset_rock >= rock && offset_rock < rock_end)
				{
					float * offset_out = out_it + offset[i];
					*offset_out += water_dist * (h_diff[i] / sum);
				}			
			}
		}
		
		rock_it++;
		sand_it++;
		in_it++;
		out_it++;
	}
}
