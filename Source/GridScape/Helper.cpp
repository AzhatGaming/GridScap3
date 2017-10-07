// Fill out your copyright notice in the Description page of Project Settings.

#include "GridScape.h"
#include "Helper.h"

Helper::Helper()
{
}

Helper::~Helper()
{
}

///Returns whether or not we are facing the current clicked face
bool Helper::FacingClickedFace(FVector CameraForwardVector, FVector CubeFaceNormal)
{
	FVector FaceCheck = CameraForwardVector - CubeFaceNormal;
	if (FMath::Abs(FaceCheck.X) < 0.5f
		&& FMath::Abs(FaceCheck.Y) < 0.5f
		&& FMath::Abs(FaceCheck.Z) < 0.5f)
	{
		return true;
	}
	return false;
}

///Corrects the rotation of the camera snap in order to ensure that the cube doesn't flip around if clicked
FRotator Helper::GetCorrectedFaceSnapRotation(bool FacingClickedFace, FRotator CameraRotator, FRotator CubeFaceNormalRotator)
{
	FRotator CorrectedRotator = CubeFaceNormalRotator;
	FRotator DifferenceRotator = UKismetMathLibrary::NormalizedDeltaRotator(CubeFaceNormalRotator, CameraRotator);

	if (FMath::Abs(DifferenceRotator.Yaw) > 90.0f || (FacingClickedFace && FMath::Abs(DifferenceRotator.Yaw) > 45.0f))
	{
		CorrectedRotator.Yaw = FMath::RoundToInt(CameraRotator.Yaw / 90.0f) * 90;
	}
	if (FMath::Abs(DifferenceRotator.Pitch) > 90.0f || (FacingClickedFace && FMath::Abs(DifferenceRotator.Pitch) > 45.0f))
	{
		CorrectedRotator.Pitch = FMath::RoundToInt(CameraRotator.Pitch / 90.0f) * 90;
	}
	if (FMath::Abs(DifferenceRotator.Roll) > 90.0f || (FacingClickedFace && FMath::Abs(DifferenceRotator.Roll) > 45.0f))
	{
		CorrectedRotator.Roll = FMath::RoundToInt(CameraRotator.Roll / 90.0f) * 90;
	}
	return CorrectedRotator;
}

///Gets a cube structure using coordinates
FCubeStruct Helper::GetCubeStructWithCoordinates(FLevelStruct Level, FVector Coordinate)
{
	for (int32 CubeIndex = 0; CubeIndex < Level.Cubes.Num(); CubeIndex++)
	{
		if (Level.Cubes[CubeIndex].Coordinates == Coordinate)
		{
			return Level.Cubes[CubeIndex];
		}
	}
	return FCubeStruct();
}

///Gets the current position of the player 
FVector4 Helper::GetCurrentPosition(FLevelStruct Level)
{
	for (int32 CubeIndex = 0; CubeIndex < Level.Cubes.Num(); CubeIndex++)
	{
		for (int32 FaceIndex = 0; FaceIndex < Level.Cubes[CubeIndex].CubeFaces.Num(); FaceIndex++)
		{
			if (Level.Cubes[CubeIndex].CubeFaces[FaceIndex].bCurrentPos)
			{
				return FVector4(Level.Cubes[CubeIndex].Coordinates.X
					, Level.Cubes[CubeIndex].Coordinates.Y
					, Level.Cubes[CubeIndex].Coordinates.Z
					, Level.Cubes[CubeIndex].CubeFaces[FaceIndex].oSide);
			}
		}
	}
	// This denotes that there is no current position for the player
	return FVector4(0, 0, 0, -1);
}

///Returns whether or not 2 cubes are adjacent to one another (up/down/left/right)
bool Helper::GetCubeAdjacent(FVector TargetCubeCoordinate, FVector CurrentCubeCoordinate)
{
	return ((TargetCubeCoordinate - CurrentCubeCoordinate).Size() == 1.0f);
}

///Calculates the box size facing the screen and the distance of that face from the origin
void Helper::GetBoxSizeAndOriginDistance(FRotator TargetRotation, FVector TargetNormal, FVector BoxDimensions, FVector2D &BoxSize, float &DistanceFromOrigin)
{
	bool ClampedRotMod = (FMath::Abs(FMath::Fmod(FMath::RoundToInt(TargetRotation.Roll), 180.0f)) > 0) ? true : false;

	if (TargetNormal.Equals(FVector::ForwardVector)) //X
	{
		if (ClampedRotMod)
			BoxSize = FVector2D(BoxDimensions.Y, BoxDimensions.Z);
		else
			BoxSize = FVector2D(BoxDimensions.Z, BoxDimensions.Y);

		DistanceFromOrigin = BoxDimensions.X;
	}
	else if (TargetNormal.Equals(FVector::RightVector)) //Y
	{
		if (ClampedRotMod)
			BoxSize = FVector2D(BoxDimensions.X, BoxDimensions.Z);
		else
			BoxSize = FVector2D(BoxDimensions.Z, BoxDimensions.X);

		DistanceFromOrigin = BoxDimensions.Y;
	}
	else if (TargetNormal.Equals(FVector::UpVector)) //Z
	{
		if (ClampedRotMod)
			BoxSize = FVector2D(BoxDimensions.Y, BoxDimensions.X);
		else
			BoxSize = FVector2D(BoxDimensions.X, BoxDimensions.Y);

		DistanceFromOrigin = BoxDimensions.Z;
	}
}

///Returns the target camera zoom distance for the camera
float Helper::GetTargetCameraZoomDistance(FVector2D BoxSize, float FieldOfView, float CameraAspectRatio, float DistanceFromOrigin)
{
	float TargetWidth = BoxSize.Y / (0.9f * FMath::Tan(FMath::DegreesToRadians(FieldOfView / 2))) + DistanceFromOrigin;
	float TargetHeight = (BoxSize.X / (0.9f * FMath::Tan(FMath::DegreesToRadians(FieldOfView / 2)) * (1 / CameraAspectRatio))) + DistanceFromOrigin;

	return (TargetWidth > TargetHeight) ? TargetWidth : TargetHeight;
}