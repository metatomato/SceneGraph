#pragma once
//#include "includes.h"
//#include "md5definitions.h"
#include "float3.h"
/*
 *  Quaternion.h
 *  ShadowMap
 *
 *  Created by fabien sanglard on 16/12/08.
 *  Copyright 2008 __MyCompanyName__. All rights reserved.
 *
 */



struct Quaternion {
	float x;
	float y;
	float z;
	float w;
	
	Quaternion()
	{
		x=0;
		y=0;
		z=0;
		w=0;
	}
	
	
	void computeW () 
	{
		float t = 1.0f - (x * x) - (y * y) - (z * z);
		
		if (t < 0.0f)
			w = 0.0f;
		else
			w = -sqrt (t);
	}
	
	void normalize ()
	{
		/* compute magnitude of the quaternion */
		float mag = sqrt (x*x + y*y	+ z*z + w*w);
		
		/* check for bogus length, to protect against divide by zero */
		if (mag > 0.0f)
		{
			/* normalize it */
			float oneOverMag = 1.0f / mag;
			
			x *= oneOverMag;
			y *= oneOverMag;
			z *= oneOverMag;
			w *= oneOverMag;
		}
	}
	
	Quaternion operator* (const Quaternion& otherQ) const
	{
		Quaternion out;
		out.w = (w * otherQ.w) - (x * otherQ.x) - (y * otherQ.y) - (z * otherQ.z);
		out.x = (x * otherQ.w) + (w * otherQ.x) + (y * otherQ.z) - (z * otherQ.y);
		out.y = (y * otherQ.w) + (w * otherQ.y) + (z * otherQ.x) - (x * otherQ.z);
		out.z = (z * otherQ.w) + (w * otherQ.z) + (x * otherQ.y) - (y * otherQ.x);
		
		return out;
	}

	void operator*= (const Quaternion& otherQ) 
	{
		
		w = (w * otherQ.w) - (x * otherQ.x) - (y * otherQ.y) - (z * otherQ.z);
		x = (x * otherQ.w) + (w * otherQ.x) + (y * otherQ.z) - (z * otherQ.y);
		y = (y * otherQ.w) + (w * otherQ.y) + (z * otherQ.x) - (x * otherQ.z);
		z = (z * otherQ.w) + (w * otherQ.z) + (x * otherQ.y) - (y * otherQ.x);
	}
	
	Quaternion operator* (const float3 & v ) const
	{
		Quaternion out;
		
		out.w = - (x * v.x) - (y * v.y) - (z * v.z);
		out.x =   (w * v.x) + (y * v.z) - (z * v.y);
		out.y =   (w * v.y) + (z * v.x) - (x * v.z);
		out.z =   (w * v.z) + (x * v.y) - (y * v.x);
		
		return out;
	}
	
	Quaternion copyAndInverse() const
	{
		Quaternion inv;
		
		inv.x = -x; 
		inv.y = -y;
		inv.z = -z; 
		inv.w =  w;
		
		return inv;
	}

	
	float3 rotatePoint (const float3 & v) const
	{
		
		
		Quaternion tmp, final;

		Quaternion inv = this->copyAndInverse();
		//inv.normalize();
		
		//Quat_multVec (q, in, tmp);
		//printf("QUATERNION rotatePoint is BUGGED BITCH !!\n");
		tmp = *this * v;
		
		//Quat_multQuat (tmp, inv, final);
		final = tmp * inv;
		
		
		
		//Converting Quaternion to float3
		float3 out;
		
		out.x = final.x;
		out.y = final.y;
		out.z = final.z;
		
		return out;
	}
	
	
	
	float dotProduct (const Quaternion& otherQ) const
	{
		return (x * otherQ.x + y * otherQ.y + z * otherQ.z + w * otherQ.w);
	}
	
	
	static Quaternion slerp (const Quaternion& qa, const Quaternion& qb, float t) 
	{
		Quaternion out;

		// Check for out-of range parameter and return edge points if so 
		if (t <= 0.0)
		{
			out = qa;
			return out;
		}
		
		if (t >= 1.0)
		{
			out=qb;
			return out;
		}
		
		// Compute "cosine of angle between quaternions" using dot product 
		float cosOmega = qa.dotProduct(qb);
		
		// If negative dot, use -q1.  Two quaternions q and -q
		// represent the same rotation, but may produce
		// different slerp.  We chose q or -q to rotate using
		// the acute angle. 
		Quaternion q1 = qb;
		
		if (cosOmega < 0.0f)
		{
			q1 = qb.copyAndInverse();
			q1.w = - q1.w;
			cosOmega = -cosOmega;
		}
		
		// We should have two unit quaternions, so dot should be <= 1.0  
		//assert (cosOmega < 1.1f);
		
		// Compute interpolation fraction, checking for quaternionsalmost exactly the same 
		float k0, k1;
		
		if (cosOmega > 0.9999f)
		{
			// Very close - just use linear interpolation,
			// which will protect againt a divide by zero 
			
			k0 = 1.0f - t;
			k1 = t;
		}
		else
		{
			// Compute the sin of the angle using the
			// trig identity sin^2(omega) + cos^2(omega) = 1 
			float sinOmega = sqrt (1.0f - (cosOmega * cosOmega));
			
			// Compute the angle from its sin and cosine 
			float omega = atan2 (sinOmega, cosOmega);
			
			// Compute inverse of denominator, so we only have
			// to divide once 
			float oneOverSinOmega = 1.0f / sinOmega;
			
			// Compute interpolation parameters 
			k0 = sin ((1.0f - t) * omega) * oneOverSinOmega;
			k1 = sin (t * omega) * oneOverSinOmega;
		}
		
		// Interpolate and return new quaternion 
		out.w = (k0 * qa.w) + (k1 * q1.w);
		out.x = (k0 * qa.x) + (k1 * q1.x);
		out.y = (k0 * qa.y) + (k1 * q1.y);
		out.z = (k0 * qa.z) + (k1 * q1.z);

		return out;
	}
	 
	
			
};

