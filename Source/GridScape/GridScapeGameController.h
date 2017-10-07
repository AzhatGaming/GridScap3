// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Helper.h"
#include "GridScapeGameModel.h"

#include "GameFramework/Actor.h"
#include "GridScapeGameController.generated.h"

UCLASS()
class GRIDSCAPE_API AGridScapeGameController : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AGridScapeGameController();
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;	
	// Called on BeginPlay() of the GamePawn class to assign the Model the PlayerPawn using the Controller
	void SetPlayerPawn(AActor* thePawn);
	// Called every frame
	virtual void Tick( float DeltaSeconds ) override;
	// Called to generate the cube grid that is each puzzle
	void GeneratePuzzle();
	// Called to generate the puzzle sides
	void GeneratePuzzleSides(float CubeTotalSize, float CubeSize, int32 Length, int32 Width, int32 Height);
	// Called to generate the puzzle barriers
	void GeneratePuzzleBarriers(TArray<FBarrierStruct> Barriers, FVector CubeCoordinate, FVector CubePosition, float CubeSize, float CubeTotalSize);
	// Called to despawn the level
	void DespawnLevel();
	// Called to save level editor data
	void SaveEditorLevel();
	// Called to load level editor data
	void LoadEditorLevel(bool EditorMode);
	// Called on a tick from the GamePawn - sets the model's axis position and velocity in the GameModel
	void SetTouchClickAxis(ETouchIndex::Type TouchIndex, FVector2D newAxis, bool isCurrentlyPressed);
	// Rotates the player camera per tick
	void RotatePlayerCamera(float DeltaTime);
	// Zooms the player camera per tick
	void ZoomPlayerCamera(float DeltaTime);
	// Snap the player camera to a side
	void SnapPlayerCamera(FVector SideNormal);
	// Sets the collision of the puzzle sides
	void SetSideCollision(bool Collision);
	// Cube click delegate function
	UFUNCTION()
		void BindToDelegateCubeClicked(FVector ClickedCubeCoordinate);
	// Side click delegate function
	UFUNCTION()
		void BindToDelegateSideClicked();
	// Barrier click delegate function
	UFUNCTION()
		void BindToDelegateBarrierClicked(int32 BarrierIndex);

	UFUNCTION(BlueprintCallable, Category = "HUD")
		void HUD_SetHUDObject(UUserWidget* HUD);

	UFUNCTION(BlueprintCallable, Category = "LevelEditor")
		void LevelEditor_SetDefaultOn(bool Checked);
	UFUNCTION(BlueprintCallable, Category = "LevelEditor")
		void LevelEditor_SetToggleable(bool Checked);
	UFUNCTION(BlueprintCallable, Category = "LevelEditor")
		void LevelEditor_SetPlayerStart(bool Checked);
	UFUNCTION(BlueprintCallable, Category = "LevelEditor")
		void LevelEditor_Unlock();
	UFUNCTION(BlueprintCallable, Category = "LevelEditor")
		void LevelEditor_Save();
	UFUNCTION(BlueprintCallable, Category = "LevelEditor")
		void LevelEditor_SetBarrierBlock(bool Checked);
	UFUNCTION(BlueprintCallable, Category = "LevelEditor")
		bool LevelEditor_SetGameMode();
	UFUNCTION(BlueprintCallable, Category = "LevelEditor")
		void LevelEditor_SetCounter(bool Checked, int32 MaxCount);
	UFUNCTION(BlueprintCallable, Category = "LevelEditor")
		void LevelEditor_NewLevel(FVector Dimensions, FString LevelName);
	UFUNCTION(BlueprintCallable, Category = "LevelEditor")
		void LevelEditor_SetBrittle(bool Checked);

	// Called by the cube click delegate function to determine the normal of the face and set the camera rotation if need be
	void TraceMouseOnCubeClick(FVector ClickedCubeCoordinates, int32 ClickedBarrier);
	// Possibly temp function used to disable the side lock
	void SetSideLocked();
	// Performs cube touch logic
	void CubeTouchLogic(FVector ClickedCubeCoordinates, uint8 ClickedSide);
	// Performs barrier touch logic
	void BarrierTouchLogic(int32 Index);
	// Calculates and sets up valid moves for the level
	void SetValidMoves(FLevelStruct* CurrentLevel);
	// Calculates and applies cube side material params
	void SetSideMaterialParameters(FCubeStruct Cube, ACube* CubeObject, bool LevelEditorSelected);
	// Sets the barrier material params
	void SetBarriersMaterial(FLevelStruct* CurrentLevel);
	// Sets the materials for the puzzle cubes
	void SetPuzzleCubesMaterial(FLevelStruct* CurrentLevel);
	// Sets the material parameters for the barriers
	void SetBarrierMaterialParameters(FBarrierStruct Barrier, ABarrier* BarrierObject, bool LevelEditorSelected);

protected:
	UPROPERTY(Category = Game, EditAnywhere)
		AGridScapeGameModel* GameModel;
};
