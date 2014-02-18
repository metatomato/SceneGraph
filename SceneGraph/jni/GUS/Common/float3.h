#pragma once

#include "math.h"

struct float3
{
	float x;
	float y;
	float z;
	
	float3()
	{
		x=0;
		y=0;
		z=0;
	}
	
	float3(float _x, float _y, float _z)
	{
		x=_x;
		y=_y;
		z=_z;
	}
	
	
	void normalize()
	{
		/* compute magnitude of the quaternion */
		float mag = sqrt (x*x + y*y	+ z*z );
		
		/* check for bogus length, to protect against divide by zero */
		if (mag > 0.0f)
		{
			/* normalize it */
			float oneOverMag = 1.0f / mag;
			
			x *= oneOverMag;
			y *= oneOverMag;
			z *= oneOverMag;
		}
	}
	
	float3 operator* (const float & v ) const
	{
		float3 out;
		
		out.x = x * v;
		out.y = y * v;
		out.z = z * v;
		
		return out;
	}

	float3 operator/ (const float & v ) const
	{
		float3 out;
		
		out.x = x / v;
		out.y = y / v;
		out.z = z / v;
		
		return out;
	}
	
	void operator*= (const float & v )
	{
		float3 out;
		
		x *=  v;
		y *=  v;
		z *=  v;
		
		
	}
	
	float3 operator+ (const float3 & v ) const
	{
		float3 out;
		
		out.x = x + v.x;
		out.y = y + v.y;
		out.z = z + v.z;
		
		return out;
	}
	
	float3 operator- (const float3 & v ) const
	{
		float3 out;
		
		out.x = x - v.x;
		out.y = y - v.y;
		out.z = z - v.z;
		
		return out;
	}
	
	
	void operator+= (const float3 & v )
	{
		x += v.x;
		y += v.y;
		z += v.z;
	}
	
	float3 crossProduct(const float3 v) const
	{
		float3 out;
		
		out.x = y*v.z - z*v.y;
		out.y = z*v.x - x*v.z;
		out.z = x*v.y - y*v.x;
		
	
		
		
		return out;
	}
};
