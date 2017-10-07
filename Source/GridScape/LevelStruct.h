// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Side.h"

#include "Engine/UserDefinedStruct.h"
#include "LevelStruct.generated.h"

UENUM()
enum class FSpread : uint8
{
	Up_Single,
	Up_Full,
	Down_Single,
	Down_Full,
	Left_Single,
	Left_Full,
	Right_Single,
	Right_Full,
	UpLeft,
	UpRight,
	DownLeft,
	DownRight,
	Full
};

UENUM()
enum class FSwitch : uint8
{
	Source,
	Target,
	Hybrid
};

UENUM()
enum class FWarp : uint8
{
	Target0,
	Target1,
	Target2,
	Target3,
	Target4,
	CurrentFace,
	FaceXp,
	FaceXn,
	FaceYp,
	FaceYn,
	FaceZp,
	FaceZn,
	Full
};

USTRUCT()
struct FCubeFaceStruct
{
	GENERATED_BODY()

	UPROPERTY()
		FVector oCubeCoordinate;

	UPROPERTY()
		uint8 oSide;

	UPROPERTY()
		int32 nClickCount;

	UPROPERTY()
		bool bActive;

	UPROPERTY()
		bool bCurrentPos;

	UPROPERTY()
		bool bValidMove;

	UPROPERTY()
		bool bDefaultOn;

	UPROPERTY()
		bool bToggleable;

	UPROPERTY()
		bool bCounter;

	UPROPERTY()
		int32 nMaxCount;

	UPROPERTY()
		bool bBrittle;

	UPROPERTY()
		bool bBrittleBroken;

	UPROPERTY()
		bool bSpread;

	UPROPERTY()
		TArray<FSpread> oSpreadType;

	UPROPERTY()
		bool bSwitch;

	UPROPERTY()
		FSwitch oSwitchType;

	UPROPERTY()
		int32 nSwitchGroup;

	UPROPERTY()
		bool bWarp;

	UPROPERTY()
		FWarp oWarpType;

	FCubeFaceStruct();
	FCubeFaceStruct(FVector CubeCoordinate, uint8 CubeFaceSide, bool Origin);
};

USTRUCT()
struct FCubeStruct
{
	GENERATED_BODY()

	UPROPERTY()
		FVector Coordinates;

	UPROPERTY()
		TArray<FCubeFaceStruct> CubeFaces;

	FCubeStruct();
	FCubeStruct(FVector CubeCoordinates, FVector Dimensions, FVector4 Origin);

private:
	void GenerateCubeFaceStructs(FVector4 Origin, FVector Dimensions);
};

USTRUCT()
struct FBarrierStruct
{
	GENERATED_BODY()

	UPROPERTY()
		FVector4 FirstFace;

	UPROPERTY()
		FVector4 SecondFace;

	UPROPERTY()
		int32 BarrierIndex;

	UPROPERTY()
		bool bBlockMovement;

	FBarrierStruct();
	FBarrierStruct(FVector4 FirstFace, FVector4 SecondFace, int32 BarrierIndex);
};

USTRUCT()
struct FLevelStruct
{
	GENERATED_BODY()

	UPROPERTY()
		FVector Dimensions;

	UPROPERTY()
		FString LevelName;

	UPROPERTY()
		TArray<FCubeStruct> Cubes;

	UPROPERTY()
		TArray<FBarrierStruct> Barriers;

	UPROPERTY()
		FVector4 Origin;

	FLevelStruct();
	FLevelStruct(FVector Dimensions, FString LevelName);
	FBarrierStruct* GetBarrier(int32 Index);
	FCubeStruct* GetCube(FVector Coordinates);
	FCubeFaceStruct* GetFace(FVector Coordinates, uint8 Face);
	TArray<FCubeFaceStruct*> GetAllFaces();
	void SetPlayerPosition(FVector4 PlayerPosition);
	void SetValidMoves(FVector4 PlayerPosition, bool LevelEditorMode);
	bool CheckBarriers(FVector4 PlayerPosition, FVector4 TargetPosition);
	bool CheckAdjacentCube(FVector FirstCube, FVector SecondCube);
	bool CheckSameCube(FVector FirstCube, FVector SecondCube);
	bool CheckSameFace(uint8 FirstFace, uint8 SecondFace);
	bool CheckOppositeFace(uint8 FirstFace, uint8 SecondFace);
	TArray<FVector4> GetValidMoves();
	bool CheckWin();
	FString LevelEditor_GetCubeFaceParams(FVector4 CubeFace);
	FString LevelEditor_GetBarrierParams(int32 BarrierIndex);

private:
	void GenerateCubeStructs();
	void GenerateBarrierStructs();
};