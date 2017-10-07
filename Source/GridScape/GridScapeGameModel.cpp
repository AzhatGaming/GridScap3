// Fill out your copyright notice in the Description page of Project Settings.

#include "GridScape.h"
#include "GridScapeGameModel.h"


// Sets default values
AGridScapeGameModel::AGridScapeGameModel()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	RotationSpeed = 0.2f;
	RotationDecay = 5.0f;

	LevelEditorSelectedBarrier = -1;
	LevelEditorSelectedTile = FVector4(-1);
}

// Called when the game starts or when spawned
void AGridScapeGameModel::BeginPlay()
{
	Super::BeginPlay();
}

// Called every frame
void AGridScapeGameModel::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

///Gets the class for the cubes
TSubclassOf<AActor> AGridScapeGameModel::GetCubeClass()
{
	return CubeClass;
}

///Gets the class for the sides
TSubclassOf<AActor> AGridScapeGameModel::GetSideClass()
{
	return SideClass;
}

///Gets the class for the barriers
TSubclassOf<AActor> AGridScapeGameModel::GetBarrierClass()
{
	return BarrierClass;
}

///Sets the HUD
void AGridScapeGameModel::SetHUD(UUserWidget* HUD)
{
	this->HUD = HUD;
}

///Gets the HUD 
UUserWidget* AGridScapeGameModel::GetHUD()
{
	return HUD;
}

///Sets the puzzle cube array for the current puzzle
void AGridScapeGameModel::SetPuzzleCubes(TArray<ACube*> PuzzleCubes)
{
	this->PuzzleCubes = PuzzleCubes;
}

///Gets the puzzle cube array for the current puzzle
TArray<ACube*> AGridScapeGameModel::GetPuzzleCubes()
{
	return PuzzleCubes;
}

///Sets the puzzle side array 
void AGridScapeGameModel::SetPuzzleSides(APuzzleSide* PuzzleSides)
{
	this->PuzzleSides = PuzzleSides;
}

///Gets the puzzle side array
APuzzleSide* AGridScapeGameModel::GetPuzzleSides()
{
	return PuzzleSides;
}

///Sets the puzzle barrier array for the current puzzle
void AGridScapeGameModel::SetPuzzleBarriers(TArray<ABarrier*> PuzzleBarriers)
{
	this->PuzzleBarriers = PuzzleBarriers;
}

///Gets the puzzle barrier array for the current puzzle
TArray<ABarrier*> AGridScapeGameModel::GetPuzzleBarriers()
{
	return PuzzleBarriers;
}

///Sets the current level structure
void AGridScapeGameModel::SetCurrentLevel(FLevelStruct newCurrentLevel)
{
	CurrentLevel = newCurrentLevel;
}

///Gets the current level structure
FLevelStruct* AGridScapeGameModel::GetCurrentLevel()
{
	return &CurrentLevel;
}

///Gets the current level editor level structure
FLevelStruct AGridScapeGameModel::GetLevelEditorLevel()
{
	return LevelEditorLevel;
}

///Sets the current level editor level structure
void AGridScapeGameModel::SetLevelEditorLevel(FLevelStruct LevelEditorLevel)
{
	this->LevelEditorLevel = LevelEditorLevel;
}

///Sets whether or not we are holding our touch/click
void AGridScapeGameModel::SetTouchClickHold(bool TouchClickHold)
{
	this->TouchClickHold = TouchClickHold;
}

///Gets whether or not we are holding our touch/click
bool AGridScapeGameModel::GetTouchClickHold()
{
	return TouchClickHold;
}

///Sets the velocity of the touch/click
void AGridScapeGameModel::SetTouchClickVelocity(FVector2D newVelocity)
{
	TouchClickVelocity = newVelocity;
}

///Sets the axis and velocity of the touch/click
void AGridScapeGameModel::SetTouchClickAxis(FVector2D TouchClickAxis)
{
	this->TouchClickAxis = TouchClickAxis;
}

///Gets the axis and velocity of the touch/click
FVector2D AGridScapeGameModel::GetTouchClickAxis()
{
	return TouchClickAxis;
}

///Gets the axis velocity of the touch/click
FVector2D AGridScapeGameModel::GetTouchClickVelocity()
{
	return TouchClickVelocity;
}

///Sets the current transform of the player pawn
void AGridScapeGameModel::SetPlayerPawn(AActor* thePawn)
{
	PlayerPawn = thePawn;
}

///Gets the current transform of the player pawn
AActor* AGridScapeGameModel::GetPlayerPawn()
{
	return PlayerPawn;
}

///Gets the rotational speed
float AGridScapeGameModel::GetRotationSpeed()
{
	return RotationSpeed;
}

///Gets the value of how quickly the cube decelerates
float AGridScapeGameModel::GetRotationDecay()
{
	return RotationDecay;
}

///Sets the camera zoom when side locked
void AGridScapeGameModel::SetSideLockZoom(float SideLockZoom)
{
	this->SideLockZoom = SideLockZoom;
}

///Gets the camera zoom when side locked
float AGridScapeGameModel::GetSideLockZoom()
{
	return SideLockZoom;
}

///Gets the camera zoom max
float AGridScapeGameModel::GetCameraZoomMax()
{
	return CameraZoomMax;
}

///Sets the side lock status of the puzzle
void AGridScapeGameModel::SetSideLocked(bool newSideLocked)
{
	SideLocked = newSideLocked;
}

///Gets the side lock status of the puzzle
bool AGridScapeGameModel::GetSideLocked()
{
	return SideLocked;
}

///Sets the side lock rotator that we will interp to and lock on to
void AGridScapeGameModel::SetSideLockRotator(FRotator newSideLockRotator)
{
	SideLockRotator = newSideLockRotator;
}

///Gets the side lock rotator that we will interp to and lock on to
FRotator AGridScapeGameModel::GetSideLockRotator()
{
	return SideLockRotator;
}

///Sets the side enum that we are locked on to
void AGridScapeGameModel::SetCurrentLockedSide(uint8 newCurrentSide)
{
	CurrentLockedSide = newCurrentSide;
}

///Gets the side enum that we are locked on to
uint8 AGridScapeGameModel::GetCurrentLockedSide()
{
	return CurrentLockedSide;
}

///Sets the player position on the puzzle
void AGridScapeGameModel::SetPlayerPosition(FVector4 PlayerPosition)
{
	this->PlayerPosition = PlayerPosition;
}

///Gets the player position on the puzzle
FVector4 AGridScapeGameModel::GetPlayerPosition()
{
	return PlayerPosition;
}

///Sets the valid moves from the player position
void AGridScapeGameModel::SetValidMoves(TArray<FVector4> ValidMoves)
{
	this->ValidMoves = ValidMoves;
}

///Gets the valid moves from the player position
TArray<FVector4> AGridScapeGameModel::GetValidMoves()
{
	return ValidMoves;
}

///Sets the level editor selected actor
void AGridScapeGameModel::SetLevelEditorTile(FVector4 LevelEditorSelectedTile)
{
	this->LevelEditorSelectedTile = LevelEditorSelectedTile;
}

///Gets the level editor selected actor
FVector4 AGridScapeGameModel::GetLevelEditorTile()
{
	return LevelEditorSelectedTile;
}

///Sets the level editor selected actor
void AGridScapeGameModel::SetLevelEditorBarrier(int32 LevelEditorSelectedBarrier)
{
	this->LevelEditorSelectedBarrier = LevelEditorSelectedBarrier;
}

///Gets the level editor selected actor
int32 AGridScapeGameModel::GetLevelEditorBarrier()
{
	return LevelEditorSelectedBarrier;
}