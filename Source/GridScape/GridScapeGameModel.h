// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Side.h"
#include "PuzzleSide.h"
#include "Cube.h"
#include "DefaultLevels.h"
#include "Kismet/GameplayStatics.h"
#include "LevelStruct.h"
#include "Barrier.h"

#include "Runtime/UMG/Public/UMG.h"
#include "SlateBasics.h"
#include "Runtime/UMG/Public/Blueprint/UserWidget.h"

#include "GameFramework/Actor.h"
#include "GridScapeGameModel.generated.h"

UCLASS()
class GRIDSCAPE_API AGridScapeGameModel : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AGridScapeGameModel();

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	
	// Called every frame
	virtual void Tick( float DeltaSeconds ) override;

	// Grabs the cube class
	TSubclassOf<AActor> GetCubeClass();
	// Grabs the side class
	TSubclassOf<AActor> GetSideClass();
	// Grabs the barrier class
	TSubclassOf<AActor> GetBarrierClass();

	// Grabs the HUD 
	void SetHUD(UUserWidget* HUD);
	UUserWidget* GetHUD();

	// Puzzle Cube info
	void SetPuzzleCubes(TArray<ACube*> PuzzleCubes);
	TArray<ACube*> GetPuzzleCubes();

	// Puzzle Side info
	void SetPuzzleSides(APuzzleSide* PuzzleSides);
	APuzzleSide* GetPuzzleSides();

	// Puzzle Barrier info
	void SetPuzzleBarriers(TArray<ABarrier*> PuzzleBarriers);
	TArray<ABarrier*> GetPuzzleBarriers();

	// Rotation speed 
	float GetRotationSpeed();
	// Rotation decay
	float GetRotationDecay();

	// Camera zoom info
	float GetCameraZoomMax();
	void SetSideLockZoom(float SideLockZoom);
	float GetSideLockZoom();

	// Current level info
	void SetCurrentLevel(FLevelStruct newCurrentLevel);
	FLevelStruct* GetCurrentLevel();

	// Current level editor info
	void SetLevelEditorLevel(FLevelStruct newCurrentLevel);
	FLevelStruct GetLevelEditorLevel();

	// Touch/click hold info
	void SetTouchClickHold(bool TouchClickHold);
	bool GetTouchClickHold();

	// Touch/click axis info
	void SetTouchClickAxis(FVector2D TouchClickAxis);
	FVector2D GetTouchClickAxis();

	// Touch/click velocity info
	void SetTouchClickVelocity(FVector2D newVelocity);
	FVector2D GetTouchClickVelocity();

	// Side lock info
	void SetSideLocked(bool newSideLocked);
	bool GetSideLocked();

	// Side lock rotator info
	void SetSideLockRotator(FRotator newSideLockRotator);
	FRotator GetSideLockRotator();

	// Side lock info
	void SetCurrentLockedSide(uint8 newCurrentSide);
	uint8 GetCurrentLockedSide();

	// Pawn transform info
	void SetPlayerPawn(AActor* thePawn);
	AActor* GetPlayerPawn();

	// Player position info
	void SetPlayerPosition(FVector4 PlayerPosition);
	FVector4 GetPlayerPosition();

	// Valid moves info
	void SetValidMoves(TArray<FVector4> ValidMoves);
	TArray<FVector4> GetValidMoves();

	// Level editor selected object info
	void SetLevelEditorTile(FVector4 LevelEditorSelectedTile);
	FVector4 GetLevelEditorTile();
	void SetLevelEditorBarrier(int32 LevelEditorSelectedBarrier);
	int32 GetLevelEditorBarrier();

	bool LevelEditorMode;

protected:
	UPROPERTY(Category = Puzzle, EditAnywhere)
		TSubclassOf<AActor> CubeClass;
	UPROPERTY(Category = Puzzle, EditAnywhere)
		TSubclassOf<AActor> SideClass;
	UPROPERTY(Category = Puzzle, EditAnywhere)
		TSubclassOf<AActor> BarrierClass;

	UUserWidget* HUD;

	UPROPERTY(Category = UserOptions, EditAnywhere)
		float RotationSpeed;
	UPROPERTY(Category = UserOptions, EditAnywhere)
		float RotationDecay;
	UPROPERTY(Category = UserOptions, EditAnywhere)
		float CameraZoomMax;

	// The player pawn that holds the camera + a spring arm
	AActor* PlayerPawn;

	// Denotes if we're holding touch/click down
	bool TouchClickHold;
	// Denotes if we're holding touch/click down 2
	bool TouchClickHold2;
	// The axis of the mouse/touch
	FVector2D TouchClickAxis;
	// The axis of the mouse/touch 2
	FVector2D TouchClickAxis2;
	// The velocity of the mouse/touch
	FVector2D TouchClickVelocity;
	// The distance between 2 axis
	float AxisDistance;

	// Denotes if we are locked to a side
	bool SideLocked;
	// The side rotator that we are locked to and will interp to
	FRotator SideLockRotator;
	// The side we are currently locked to
	uint8 CurrentLockedSide;
	// The side lock zoom amount
	float SideLockZoom;

	// The player's current position on the cube
	FVector4 PlayerPosition;
	// The array of valid moves from the current player position
	TArray<FVector4> ValidMoves;

	// Holds the current transform of the pawn
	FTransform PawnTransform;

	// The array of puzzle cubes
	TArray<ACube*> PuzzleCubes;
	// The cube collider that acts as our puzzle sides when unlocked
	APuzzleSide* PuzzleSides;
	// The array of barriers
	TArray<ABarrier*> PuzzleBarriers;

	// The actor selected in the level editor mode
	FVector4 LevelEditorSelectedTile;
	int32 LevelEditorSelectedBarrier;

	FLevelStruct LevelEditorLevel;
	FLevelStruct CurrentLevel;
};
