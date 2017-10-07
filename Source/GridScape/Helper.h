// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Side.h"
#include "LevelStruct.h"
#include "Kismet/KismetMathLibrary.h"

class GRIDSCAPE_API Helper
{
public:
	Helper();
	~Helper();

	// Checks to see if the clicked face is the face that the camera is currently closest to
	static bool FacingClickedFace(FVector CameraForwardVector, FVector CubeFaceNormal);
	// Corrects snap rotation of clicked face so that we don't have any unnecessary spinning
	static FRotator GetCorrectedFaceSnapRotation(bool FacingClickedFace, FRotator CameraRotator, FRotator CubeFaceNormalRotator);

	// Gets a cube struct object using the level struct and the coordinate
	static FCubeStruct GetCubeStructWithCoordinates(FLevelStruct Level, FVector Coordinate);
	// Gets the current player position of the level
	static FVector4 GetCurrentPosition(FLevelStruct Level);
	// Gets whether or not our target cube is adjacent (up/down/left/right) to the current cube
	static bool GetCubeAdjacent(FVector TargetCube, FVector CurrentCube);
	// Gets the box size and distance from the origin in order to find our camera zoom distance
	static void GetBoxSizeAndOriginDistance(FRotator TargetRotation, FVector TargetNormal, FVector BoxDimensions, FVector2D &BoxSize, float &DistanceFromOrigin);
	// Gets the target distance for the camera
	static float GetTargetCameraZoomDistance(FVector2D BoxSize, float FieldOfView, float CameraAspectRatio, float DistanceFromOrigin);
};