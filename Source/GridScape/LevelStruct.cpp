// Fill out your copyright notice in the Description page of Project Settings.

#include "GridScape.h"
#include "LevelStruct.h"

/// ---------------------------------
/// --FLevelStruct-------------------
/// ---------------------------------

/// Default level constructor.
FLevelStruct::FLevelStruct()
{
	Dimensions = FVector(3);
	LevelName = "Default";
	Origin = FVector4(0, 0, 0, 1);

	GenerateCubeStructs();
	GenerateBarrierStructs();
}

///Override level constructor.
FLevelStruct::FLevelStruct(FVector Dimensions, FString LevelName)
{
	this->Dimensions = Dimensions;
	this->LevelName = LevelName;
	Origin = FVector4(0, 0, 0, 1);

	GenerateCubeStructs();
	GenerateBarrierStructs();
}

///Generate the cube structs for the level.
void FLevelStruct::GenerateCubeStructs()
{
	const int32 Length = Dimensions.X;
	const int32 Width = Dimensions.Y;
	const int32 Height = Dimensions.Z;
	for (int32 CubeIndex = 0; CubeIndex < (Length * Width * Height); CubeIndex++)
	{
		const int32 XCoordinate = (CubeIndex % Length);
		const int32 YCoordinate = ((CubeIndex / Length) % Width);
		const int32 ZCoordinate = (CubeIndex / (Width * Length));

		Cubes.Add(FCubeStruct(FVector(XCoordinate, YCoordinate, ZCoordinate), Dimensions, Origin));
	}
}

///Generate the barrier structs for the level.
void FLevelStruct::GenerateBarrierStructs()
{
	TArray<FVector4> CubeFaces;
	for (int32 CubeIndex = 0; CubeIndex < Cubes.Num(); CubeIndex++)
	{
		for (int32 FaceIndex = 0; FaceIndex < Cubes[CubeIndex].CubeFaces.Num(); FaceIndex++)
		{
			CubeFaces.Add(FVector4(Cubes[CubeIndex].Coordinates, Cubes[CubeIndex].CubeFaces[FaceIndex].oSide));
		}
	}

	int32 Index = 0;
	for (int32 OuterIndex = 0; OuterIndex < CubeFaces.Num(); OuterIndex++)
	{
		for (int32 InnerIndex = 0; InnerIndex < CubeFaces.Num(); InnerIndex++)
		{
			bool BarrierExists = false;
			for (int32 BarrierIndex = 0; BarrierIndex < Barriers.Num(); BarrierIndex++)
			{
				if (Barriers[BarrierIndex].FirstFace == CubeFaces[InnerIndex])
				{
					BarrierExists = true;
					break;
				}
			}
			if (!BarrierExists)
			{
				bool AdjacentCube = ((FVector(CubeFaces[OuterIndex]) - FVector(CubeFaces[InnerIndex])).Size() == 1);
				bool SameCube = (FVector(CubeFaces[OuterIndex]) == FVector(CubeFaces[InnerIndex]));
				bool SameFace = (CubeFaces[OuterIndex].W == CubeFaces[InnerIndex].W);
				bool OppositeFace = (FSide::GetVectorBySide(CubeFaces[OuterIndex].W) == -FSide::GetVectorBySide(CubeFaces[InnerIndex].W));

				if ((SameCube && !SameFace && !OppositeFace) || (AdjacentCube && SameFace))
				{
					Barriers.Add(FBarrierStruct(CubeFaces[OuterIndex], CubeFaces[InnerIndex], Index));
					Index++;
				}
			}
		}
	}
}

///Grabs a barrier based on the index.
FBarrierStruct* FLevelStruct::GetBarrier(int32 Index)
{
	if (Index != -1)
	{
		for (int32 BarrierIndex = 0; BarrierIndex < Barriers.Num(); BarrierIndex++)
		{
			if (Barriers[BarrierIndex].BarrierIndex == Index)
				return &Barriers[BarrierIndex];
		}
	}
	return nullptr;
}

///Grabs a cube based on the coordinate.
FCubeStruct* FLevelStruct::GetCube(FVector Coordinates)
{
	for (int32 CubeIndex = 0; CubeIndex < Cubes.Num(); CubeIndex++)
	{
		if (Cubes[CubeIndex].Coordinates.Equals(Coordinates))
			return &Cubes[CubeIndex];
	}
	return nullptr;
}

///Grabs a face based on the coordinate and face value.
FCubeFaceStruct* FLevelStruct::GetFace(FVector Coordinates, uint8 Face)
{
	FCubeStruct* Cube = GetCube(Coordinates);
	for (int32 FaceIndex = 0; FaceIndex < Cube->CubeFaces.Num(); FaceIndex++)
	{
		if (Cube->CubeFaces[FaceIndex].oSide == Face)
			return &Cube->CubeFaces[FaceIndex];
	}
	return nullptr;
}

///Grabs all the cube face structs for the level.
TArray<FCubeFaceStruct*> FLevelStruct::GetAllFaces()
{
	TArray<FCubeFaceStruct*> AllFaces;
	for (int32 CubeIndex = 0; CubeIndex < Cubes.Num(); CubeIndex++)
	{
		for (int32 FaceIndex = 0; FaceIndex < Cubes[CubeIndex].CubeFaces.Num(); FaceIndex++)
		{
			AllFaces.Add(&Cubes[CubeIndex].CubeFaces[FaceIndex]);
		}
	}
	return AllFaces;
}

///Sets the current player position.
void FLevelStruct::SetPlayerPosition(FVector4 PlayerPosition)
{
	for (int32 CubeIndex = 0; CubeIndex < Cubes.Num(); CubeIndex++)
	{
		for (int32 FaceIndex = 0; FaceIndex < Cubes[CubeIndex].CubeFaces.Num(); FaceIndex++)
		{
			Cubes[CubeIndex].CubeFaces[FaceIndex].bCurrentPos = false;
			if (PlayerPosition == FVector4(Cubes[CubeIndex].Coordinates, Cubes[CubeIndex].CubeFaces[FaceIndex].oSide))
				Cubes[CubeIndex].CubeFaces[FaceIndex].bCurrentPos = true;
		}
	}
}

///Sets all the valid moves for the puzzle in relation to the current player position.
void FLevelStruct::SetValidMoves(FVector4 PlayerPosition, bool LevelEditorMode)
{
	for (int32 CubeIndex = 0; CubeIndex < Cubes.Num(); CubeIndex++)
	{
		for (int32 FaceIndex = 0; FaceIndex < Cubes[CubeIndex].CubeFaces.Num(); FaceIndex++)
		{
			Cubes[CubeIndex].CubeFaces[FaceIndex].bValidMove = false;

			if (!LevelEditorMode)
			{
				bool AdjacentCube = this->CheckAdjacentCube(FVector(PlayerPosition), Cubes[CubeIndex].Coordinates);
				bool SameCube = this->CheckSameCube(FVector(PlayerPosition), Cubes[CubeIndex].Coordinates);
				bool SameFace = this->CheckSameFace(PlayerPosition.W, Cubes[CubeIndex].CubeFaces[FaceIndex].oSide);
				bool OppositeFace = this->CheckOppositeFace(PlayerPosition.W, Cubes[CubeIndex].CubeFaces[FaceIndex].oSide);
				bool CheckBarriers = this->CheckBarriers(PlayerPosition, FVector4(Cubes[CubeIndex].Coordinates, Cubes[CubeIndex].CubeFaces[FaceIndex].oSide));	
				if (((SameCube && !SameFace && !OppositeFace) || (AdjacentCube && SameFace)) && !CheckBarriers)
				{
					Cubes[CubeIndex].CubeFaces[FaceIndex].bValidMove = true;
				}
			}
		}
	}
}

///Checks to see if a barrier stands between the player and an adjacent face.
///Returns true if blocked, false if clear.
bool FLevelStruct::CheckBarriers(FVector4 PlayerPosition, FVector4 TargetPosition)
{
	for (int32 BarrierIndex = 0; BarrierIndex < Barriers.Num(); BarrierIndex++)
	{
		if (Barriers[BarrierIndex].bBlockMovement)
		{
			if ((Barriers[BarrierIndex].FirstFace.Equals(PlayerPosition)
				&& Barriers[BarrierIndex].SecondFace.Equals(TargetPosition))
				|| (Barriers[BarrierIndex].SecondFace.Equals(PlayerPosition)
					&& Barriers[BarrierIndex].FirstFace.Equals(TargetPosition)))
			{
				return true;			
			}
		}
	}
	return false;
}

///Checks to see if the a target cube is adjacent to the player cube.
///Returns true if adjacent, false if not.
bool FLevelStruct::CheckAdjacentCube(FVector FirstCube, FVector SecondCube)
{
	if ((FirstCube - SecondCube).Size() == 1)
	{
		return true;
	}
	return false;
}

///Checks to see if the target cube is the same as the player cube.
///Returns true if same, false if not.
bool FLevelStruct::CheckSameCube(FVector FirstCube, FVector SecondCube)
{
	if (FirstCube.Equals(SecondCube))
	{
		return true;
	}
	return false;
}

///Checks to see if the target face is the same as the player face.
///Returns true if same, false if not.
bool FLevelStruct::CheckSameFace(uint8 FirstFace, uint8 SecondFace)
{
	if (FirstFace == SecondFace)
	{
		return true;
	}
	return false;
}

///Checks to see if the target face is the opposite of the player face.
///Returns true if opposite, false if not.
bool FLevelStruct::CheckOppositeFace(uint8 FirstFace, uint8 SecondFace)
{
	if (FSide::GetVectorBySide(FirstFace) == -FSide::GetVectorBySide(SecondFace))
	{
		return true;
	}
	return false;
}

///Grabs an array of all valid moves from the current player position.
TArray<FVector4> FLevelStruct::GetValidMoves()
{
	TArray<FVector4> ValidMoves;
	for (int32 CubeIndex = 0; CubeIndex < Cubes.Num(); CubeIndex++)
	{
		for (int32 FaceIndex = 0; FaceIndex < Cubes[CubeIndex].CubeFaces.Num(); FaceIndex++)
		{
			if (Cubes[CubeIndex].CubeFaces[FaceIndex].bValidMove)
			{
				ValidMoves.Add(FVector4(Cubes[CubeIndex].Coordinates, Cubes[CubeIndex].CubeFaces[FaceIndex].oSide));
			}
		}
	}
	return ValidMoves;
}

///Checks to see if all tiles are active, returns true if so.
bool FLevelStruct::CheckWin()
{
	TArray<FCubeFaceStruct*> AllFaces = this->GetAllFaces();
	for (int32 FaceIndex = 0; FaceIndex < AllFaces.Num(); FaceIndex++)
	{
		if (!AllFaces[FaceIndex]->bActive)
			return false;
	}
	return true;
}

///Returns a string to feed cube data to be displayed on the level editor HUD
FString FLevelStruct::LevelEditor_GetCubeFaceParams(FVector4 CubeFace)
{
	FCubeFaceStruct* CubeFaceStruct = this->GetFace(FVector(CubeFace), CubeFace.W);
	FString CommandString = "OnCubeFocus";
	CommandString += " ";
	CommandString += CubeFaceStruct->bCurrentPos ? "true" : "false";
	CommandString += " ";
	CommandString += CubeFaceStruct->bDefaultOn ? "true" : "false";
	CommandString += " ";
	CommandString += CubeFaceStruct->bToggleable ? "true" : "false";
	CommandString += " ";
	CommandString += CubeFaceStruct->bCounter ? "true" : "false";
	CommandString += " ";
	CommandString.AppendInt(CubeFaceStruct->nMaxCount);
	CommandString += " ";
	CommandString += CubeFaceStruct->bBrittle ? "true" : "false";

	return CommandString;
}

FString FLevelStruct::LevelEditor_GetBarrierParams(int32 BarrierIndex)
{
	FBarrierStruct* BarrierStruct = this->GetBarrier(BarrierIndex);
	FString CommandString = "OnBarrierFocus";
	CommandString += " ";
	CommandString += BarrierStruct->bBlockMovement ? "true" : "false";

	return CommandString;
}

/// ---------------------------------
/// --FCubeStruct--------------------
/// ---------------------------------
FCubeStruct::FCubeStruct()
{
	CubeFaces.Add(FCubeFaceStruct(FVector::ZeroVector, FSide::Xp, false));
	CubeFaces.Add(FCubeFaceStruct(FVector::ZeroVector, FSide::Xn, false));
	CubeFaces.Add(FCubeFaceStruct(FVector::ZeroVector, FSide::Yp, false));
	CubeFaces.Add(FCubeFaceStruct(FVector::ZeroVector, FSide::Yn, false));
	CubeFaces.Add(FCubeFaceStruct(FVector::ZeroVector, FSide::Zp, false));
	CubeFaces.Add(FCubeFaceStruct(FVector::ZeroVector, FSide::Zn, false));
}

FCubeStruct::FCubeStruct(FVector CubeCoordinates, FVector Dimensions, FVector4 Origin)
{
	Coordinates = CubeCoordinates;
	GenerateCubeFaceStructs(Origin, Dimensions);
}

///Generates the cube face structs for the cube.
void FCubeStruct::GenerateCubeFaceStructs(FVector4 Origin, FVector Dimensions)
{
	FVector OriginCube = FVector(Origin);
	uint8 OriginFace = static_cast<uint8>(Origin.W);

	if (Coordinates.X == Dimensions.X - 1)
	{
		uint8 Side = FSide::Xp;
		bool IsOrigin = ((OriginCube == Coordinates) && (OriginFace == Side));
		CubeFaces.Add(FCubeFaceStruct(Coordinates, Side, IsOrigin));
	}
	if (Coordinates.X == 0)
	{
		uint8 Side = FSide::Xn;
		bool IsOrigin = ((OriginCube == Coordinates) && (OriginFace == Side));
		CubeFaces.Add(FCubeFaceStruct(Coordinates, Side, IsOrigin));
	}
	if (Coordinates.Y == Dimensions.Y - 1)
	{
		uint8 Side = FSide::Yp;
		bool IsOrigin = ((OriginCube == Coordinates) && (OriginFace == Side));
		CubeFaces.Add(FCubeFaceStruct(Coordinates, Side, IsOrigin));
	}
	if (Coordinates.Y == 0)
	{
		uint8 Side = FSide::Yn;
		bool IsOrigin = ((OriginCube == Coordinates) && (OriginFace == Side));
		CubeFaces.Add(FCubeFaceStruct(Coordinates, Side, IsOrigin));
	}
	if (Coordinates.Z == Dimensions.Z - 1)
	{
		uint8 Side = FSide::Zp;
		bool IsOrigin = ((OriginCube == Coordinates) && (OriginFace == Side));
		CubeFaces.Add(FCubeFaceStruct(Coordinates, Side, IsOrigin));
	}
	if (Coordinates.Z == 0)
	{
		uint8 Side = FSide::Zn;
		bool IsOrigin = ((OriginCube == Coordinates) && (OriginFace == Side));
		CubeFaces.Add(FCubeFaceStruct(Coordinates, Side, IsOrigin));
	}
}

/// ---------------------------------
/// --FCubeFaceStruct----------------
/// ---------------------------------
FCubeFaceStruct::FCubeFaceStruct()
{
	nClickCount = 0;
	bDefaultOn = false;
	bToggleable = false;
	bCounter = false;
	nMaxCount = 0;
}

FCubeFaceStruct::FCubeFaceStruct(FVector CubeCoordinate, uint8 CubeFaceSide, bool Origin)
{
	oCubeCoordinate = CubeCoordinate;
	oSide = CubeFaceSide;
	nClickCount = 0;
	bDefaultOn = false;
	bToggleable = false;
	bCurrentPos = Origin;
	bActive = false;
	bValidMove = false;
	bCounter = false;
	nMaxCount = 0;
	bBrittle = false;
	bBrittleBroken = false;
}

/// ---------------------------------
/// --FBarrierStruct-----------------
/// ---------------------------------
FBarrierStruct::FBarrierStruct()
{
}

FBarrierStruct::FBarrierStruct(FVector4 FirstFace, FVector4 SecondFace, int32 BarrierIndex)
{
	this->FirstFace = FirstFace;
	this->SecondFace = SecondFace;
	this->BarrierIndex = BarrierIndex;
	bBlockMovement = false;
}