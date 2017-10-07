// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Engine/UserDefinedEnum.h"
#include "Side.generated.h"

USTRUCT()
struct FSide
{
	GENERATED_BODY()

	static const uint8 Xp = 0;
	static const uint8 Xn = 1;
	static const uint8 Yp = 2;
	static const uint8 Yn = 3;
	static const uint8 Zp = 4;
	static const uint8 Zn = 5;

	static uint8 GetSideByVector(FVector HitNormal)
	{
		if (HitNormal.Equals(FVector::ForwardVector))
			return Xp;
		if (HitNormal.Equals(-FVector::ForwardVector))
			return Xn;
		if (HitNormal.Equals(FVector::RightVector))
			return Yp;
		if (HitNormal.Equals(-FVector::RightVector))
			return Yn;
		if (HitNormal.Equals(FVector::UpVector))
			return Zp;
		if (HitNormal.Equals(-FVector::UpVector))
			return Zn;
		return Xp;
	}

	static FVector GetVectorBySide(uint8 Side)
	{
		switch (Side)
		{
		case Xp:
			return FVector::ForwardVector;
		case Xn:
			return -FVector::ForwardVector;
		case Yp:
			return FVector::RightVector;
		case Yn:
			return -FVector::RightVector;
		case Zp:
			return FVector::UpVector;
		case Zn:
			return -FVector::UpVector;
		default:
			return FVector::ForwardVector;
		}
	}

	static FVector GetVectorBySide(float Side)
	{
		switch (static_cast<uint8>(Side))
		{
		case Xp:
			return FVector::ForwardVector;
		case Xn:
			return -FVector::ForwardVector;
		case Yp:
			return FVector::RightVector;
		case Yn:
			return -FVector::RightVector;
		case Zp:
			return FVector::UpVector;
		case Zn:
			return -FVector::UpVector;
		default:
			return FVector::ForwardVector;
		}
	}

	static bool IsX(uint8 Side)
	{
		return (Side == Xp || Side == Xn);
	}

	static bool IsY(uint8 Side)
	{
		return (Side == Yp || Side == Yn);
	}

	static bool IsZ(uint8 Side)
	{
		return (Side == Zp || Side == Zn);

	}
};