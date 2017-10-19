// Fill out your copyright notice in the Description page of Project Settings.

#include "GridScape.h"
#include "GridScapeGameController.h"


///Sets default values
AGridScapeGameController::AGridScapeGameController()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

///Called when the game starts or when spawned
void AGridScapeGameController::BeginPlay()
{
	Super::BeginPlay();

	LoadEditorLevel(true);
}

///Called to load level editor data
void AGridScapeGameController::LoadEditorLevel(bool EditorMode)
{
	// Load our default level structure if one exists.
	UDefaultLevels* LoadDefaultLevels = Cast<UDefaultLevels>(UGameplayStatics::CreateSaveGameObject(UDefaultLevels::StaticClass()));
	if (UGameplayStatics::DoesSaveGameExist(LoadDefaultLevels->SaveSlotName, LoadDefaultLevels->UserIndex))
	{
		LoadDefaultLevels = Cast<UDefaultLevels>(UGameplayStatics::LoadGameFromSlot(LoadDefaultLevels->SaveSlotName, LoadDefaultLevels->UserIndex));
		TArray<FLevelStruct> DefaultLevels = LoadDefaultLevels->DefaultLevels;

		if (DefaultLevels.Num() > 0)
		{
			FLevelStruct CurrentLevel = DefaultLevels[0];
		}

		//temp - only grabbing level 0 for now
		GameModel->SetCurrentLevel(DefaultLevels[0]);
	}
	else
	{
		FLevelStruct DefaultLevelStruct = FLevelStruct();
		GameModel->SetCurrentLevel(DefaultLevelStruct);
	}

	DespawnLevel();
	GameModel->LevelEditorMode = EditorMode;
	GeneratePuzzle();
}

///Called on BeginPlay() by the GamePawn class to the Controller to set the PlayerPawn in the Model
void AGridScapeGameController::SetPlayerPawn(AActor* thePawn)
{
	GameModel->SetPlayerPawn(thePawn);
}

///Called every frame
void AGridScapeGameController::Tick( float DeltaTime )
{
	Super::Tick( DeltaTime );

	// Does player camera rotation
	RotatePlayerCamera(DeltaTime);
	// Does player camera zoom
	ZoomPlayerCamera(DeltaTime);
}

///Called per tick to rotate the player camera
void AGridScapeGameController::RotatePlayerCamera(float DeltaTime)
{
	FVector2D Velocity = GameModel->GetTouchClickVelocity();
	float Decay = GameModel->GetRotationDecay();

	// If we are currently allowed to freely move, base the movement off of the touch click velocity
	if (!GameModel->GetSideLocked())
	{
		GameModel->SetTouchClickVelocity(FMath::Vector2DInterpTo(Velocity, FVector2D(0, 0), DeltaTime, Decay));
		GameModel->GetPlayerPawn()->AddActorLocalRotation(FRotator(-Velocity.Y, Velocity.X, 0.0f));
	}
	else
	{
		// If we're side locked but not in the correct rotation, interp to
		if (!GameModel->GetPlayerPawn()->GetActorRotation().Equals(GameModel->GetSideLockRotator(), 0.5f))
		{
			FQuat CurrentQuat = GameModel->GetPlayerPawn()->GetActorRotation().Quaternion();
			FQuat TargetQuat = GameModel->GetSideLockRotator().Quaternion();
			FRotator NewCameraRotation = (FQuat::FastLerp(CurrentQuat, TargetQuat, (15.0f * DeltaTime))).Rotator();

			//FRotator NewCameraRotation = FMath::RInterpTo(GameModel->GetPlayerPawn()->GetActorRotation(), GameModel->GetSideLockRotator(), DeltaTime, 15.0f);
			if (NewCameraRotation.Equals(GameModel->GetSideLockRotator(), 0.5f))
				NewCameraRotation = GameModel->GetSideLockRotator();
			
			GameModel->GetPlayerPawn()->SetActorRelativeRotation(NewCameraRotation);
		}
	}
}

///Called per tick to zoom the player camera to a locked position
void AGridScapeGameController::ZoomPlayerCamera(float DeltaTime)
{		
	USpringArmComponent* SpringArm = Cast<USpringArmComponent>(GameModel->GetPlayerPawn()->GetComponentByClass(USpringArmComponent::StaticClass()));
	if (!GameModel->GetSideLocked())
	{
		SpringArm->TargetArmLength = FMath::FInterpTo(SpringArm->TargetArmLength, GameModel->GetCameraZoomMax(), DeltaTime, 10.0f);
	}
	else
	{
		SpringArm->TargetArmLength = FMath::FInterpTo(SpringArm->TargetArmLength, GameModel->GetSideLockZoom(), DeltaTime, 10.0f);
	}
}

///Called to generate the game puzzle
void AGridScapeGameController::GeneratePuzzle()
{
	// The cube class from the Model
	TSubclassOf<AActor> CubeClass = GameModel->GetCubeClass();
	FLevelStruct* CurrentLevel = GameModel->GetCurrentLevel(); 

	// Set the default valid moves for the current level
	SetValidMoves(CurrentLevel);

	// The puzzle dimensions
	const int32 Length = CurrentLevel->Dimensions.X;
	const int32 Width = CurrentLevel->Dimensions.Y;
	const int32 Height = CurrentLevel->Dimensions.Z;

	// The cube size plus spacing
	const float CubeSize = 100.0f;
	const float CubeSpacing = 10.0f;
	const float CubeTotalSize = CubeSize + CubeSpacing;
	// The total number of possible cubes given the dimensions of the puzzle
	const int32 NumCubes = Length * Width * Height;

	// The offset of dimensions to ensure that the center of the puzzle is the origin
	const float LengthOffset = (Length - 1) * (CubeTotalSize / 2);
	const float WidthOffset = (Width - 1) * (CubeTotalSize / 2);
	const float HeightOffset = (Height - 1) * (CubeTotalSize / 2);

	// The cube array that we will store in the Game Model
	TArray<ACube*> CubeArray;

	// Spawn each cube
	for (int32 CubeIndex = 0; CubeIndex < NumCubes; CubeIndex++)
	{
		// The normalized coordinates of the cubes
		const int32 XCoordinate = (CubeIndex % Length);
		const int32 YCoordinate = ((CubeIndex / Length) % Width);
		const int32 ZCoordinate = (CubeIndex / (Width * Length));
		const FVector CubeCoordinate = FVector(XCoordinate, YCoordinate, ZCoordinate);

		// This will only spawn cubes on the outer edge of the 3d grid
		if ((XCoordinate == 0 || XCoordinate == Length - 1)
			|| (YCoordinate == 0 || YCoordinate == Width - 1)
			|| (ZCoordinate == 0 || ZCoordinate == Height - 1))
		{
			// Set up the offset so the puzzle is in the center
			const float XOffset = (XCoordinate * CubeTotalSize) - LengthOffset;
			const float YOffset = (YCoordinate * CubeTotalSize) - WidthOffset;
			const float ZOffset = (ZCoordinate * CubeTotalSize) - HeightOffset;

			// Position of cube for grid location
			const FVector CubePosition = FVector(XOffset, YOffset, ZOffset) + GetActorLocation();

			ACube* NewCube = Cast<ACube>(GetWorld()->SpawnActor<AActor>(CubeClass, CubePosition, FRotator(0)));
			// Set up the new cube's data
			if (NewCube != nullptr)
			{
				NewCube->SetCoordinate(CubeCoordinate);
				NewCube->OnCubeClick.AddUniqueDynamic(this, &AGridScapeGameController::BindToDelegateCubeClicked);
				//NewCube->SetActorLabel(CubeCoordinate.ToCompactString());
				CubeArray.Add(NewCube);

				// Add the barriers for this cube
				GeneratePuzzleBarriers(CurrentLevel->Barriers, CubeCoordinate, CubePosition, CubeSize, CubeTotalSize);
			}
		}
	}
	// Add the puzzle sides
	GeneratePuzzleSides(CubeTotalSize, CubeSize, Length, Width, Height);
	// Add the puzzle cubes to the game model and set the materials for the entire puzzle
	GameModel->SetPuzzleCubes(CubeArray);
	SetPuzzleCubesMaterial(CurrentLevel);
	SetBarriersMaterial(CurrentLevel);
}

///Generates the puzzle sides 
void AGridScapeGameController::GeneratePuzzleSides(float CubeTotalSize, float CubeSize, int32 Length, int32 Width, int32 Height)
{
	// The side class from the Model
	TSubclassOf<AActor> SideClass = GameModel->GetSideClass();

	// The total dimensions of the puzzle - used to create collision boxes when the puzzle is zoomed out?
	const float PuzzleLength = (CubeTotalSize * (Length - 1)) + CubeSize;
	const float PuzzleWidth = (CubeTotalSize * (Width - 1)) + CubeSize;
	const float PuzzleHeight = (CubeTotalSize * (Height - 1)) + CubeSize;

	// The scale of each size based on the LWH
	const float XScale = (PuzzleLength / 100) + 0.05f;
	const float YScale = (PuzzleWidth / 100) + 0.05f;
	const float ZScale = (PuzzleHeight / 100) + 0.05f;
	const float FScale = 0.05f;

	APuzzleSide* NewSides = Cast<APuzzleSide>(GetWorld()->SpawnActor<AActor>(SideClass, FVector::ZeroVector, FRotator::ZeroRotator));
	if (NewSides != nullptr)
	{
		NewSides->SetActorScale3D(FVector(XScale, YScale, ZScale));
		NewSides->OnSideClick.AddUniqueDynamic(this, &AGridScapeGameController::BindToDelegateSideClicked);
		NewSides->Box->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Block);
		GameModel->SetPuzzleSides(NewSides);
	}
}

///Generates the puzzle barriers
void AGridScapeGameController::GeneratePuzzleBarriers(TArray<FBarrierStruct> Barriers, FVector CubeCoordinate, FVector CubePosition, float CubeSize, float CubeTotalSize)
{
	TSubclassOf<AActor> BarrierClass = GameModel->GetBarrierClass();
	TArray<ABarrier*> GameBarriers;
	if (!CubeCoordinate.Equals(FVector::ZeroVector))
		GameBarriers = GameModel->GetPuzzleBarriers();

	for (int32 BarrierIndex = 0; BarrierIndex < Barriers.Num(); BarrierIndex++)
	{
		FVector BarrierCoordinate = FVector(Barriers[BarrierIndex].FirstFace);
		if (BarrierCoordinate == CubeCoordinate)
		{
			// If the barrier isn't on the current cube, move it towards that cube so it's in between
			FVector TargetCoordinate = FVector::ZeroVector;
			if (BarrierCoordinate != FVector(Barriers[BarrierIndex].SecondFace))
			{
				TargetCoordinate = FVector(Barriers[BarrierIndex].SecondFace) - BarrierCoordinate;
			}
			else
			{
				TargetCoordinate = FSide::GetVectorBySide(Barriers[BarrierIndex].SecondFace.W);
			}

			// Set the barrier's proper position
			FVector BarrierPosition = CubePosition;
			BarrierPosition += ((FVector(CubeSize) / 2.0f) * FSide::GetVectorBySide(Barriers[BarrierIndex].FirstFace.W));
			BarrierPosition += ((FVector(CubeTotalSize) / 2.0f) * TargetCoordinate);

			// Spawn the barrier
			ABarrier* NewBarrier = Cast<ABarrier>(GetWorld()->SpawnActor<AActor>(BarrierClass, BarrierPosition, FRotator::ZeroRotator));
			NewBarrier->SetIndex(BarrierIndex);
			GameBarriers.Add(NewBarrier);

			NewBarrier->OnBarrierClick.AddUniqueDynamic(this, &AGridScapeGameController::BindToDelegateBarrierClicked);

			// Set the proper rotation of the barrier
			FVector RotationVector = TargetCoordinate.GetAbs() * 90.0f;
			FRotator NewRotation = FRotator::ZeroRotator;
			if (FSide::IsX(Barriers[BarrierIndex].FirstFace.W))
				NewRotation = FRotator(RotationVector.Y, RotationVector.Z, RotationVector.X);
			if (FSide::IsY(Barriers[BarrierIndex].FirstFace.W))
				NewRotation = FRotator(RotationVector.X, RotationVector.Y, RotationVector.Z);
			if (FSide::IsZ(Barriers[BarrierIndex].FirstFace.W))
				NewRotation = FRotator(RotationVector.Z, RotationVector.X, RotationVector.Y);

			NewBarrier->SetActorRotation(NewRotation);
		}
	}
	GameModel->SetPuzzleBarriers(GameBarriers);
}

///Called at cube generation and after cube click to calculate the current set of valid moves
void AGridScapeGameController::SetValidMoves(FLevelStruct* CurrentLevel)
{
	// Grab the current position vector from the level
	FVector4 CurrentPosition = Helper::GetCurrentPosition(*CurrentLevel);
	// Extract relevant data from the position vector
	FVector CurrentPositionCoordinates = FVector(CurrentPosition);
	uint8 CurrentPositionSide = static_cast<uint8>(CurrentPosition.W);

	CurrentLevel->SetValidMoves(CurrentPosition, GameModel->LevelEditorMode);
}

///Called at cube generation and after cube click to calculate new material parameters for a cube object
void AGridScapeGameController::SetSideMaterialParameters(FCubeStruct Cube, ACube* CubeObject, bool LevelEditorSelected)
{
	for (int32 FaceIndex = 0; FaceIndex < Cube.CubeFaces.Num(); FaceIndex++)
	{
		UMaterialInstanceDynamic* MaterialInstance = UMaterialInstanceDynamic::Create(CubeObject->GetMaterial(Cube.CubeFaces[FaceIndex].oSide), this);
		FCubeFaceStruct Face = Cube.CubeFaces[FaceIndex];

		// Level Editor Selected -- In level editor mode, this is the cube face that is currently selected by the user
		MaterialInstance->SetScalarParameterValue(FName("LevelEditorSelected"), LevelEditorSelected ? 1.0f : 0.0f);
		// Outward Facing -- The cube faces the outside of the puzzle and should therefore be rendered.
		MaterialInstance->SetScalarParameterValue(FName("OutwardFacing"), 1.0f);
		// Current Position -- The current position of the player on the puzzle
		MaterialInstance->SetScalarParameterValue(FName("CurrentPosition"), Face.bCurrentPos ? 1.0f : 0.0f);
		// Activated -- Whether or not the player has activated this cube face already
		MaterialInstance->SetScalarParameterValue(FName("Activated"), Face.bActive ? 1.0f : 0.0f);
		// Valid Move -- Whether or not the player can move to this side
		MaterialInstance->SetScalarParameterValue(FName("ValidMove"), Face.bValidMove ? 1.0f : 0.0f);
		// Is Toggleable -- Denotes if this light can turn on + off rather than just on
		MaterialInstance->SetScalarParameterValue(FName("Toggleable"), Face.bToggleable ? 1.0f : 0.0f);
		// Counter -- Denotes if this light requires a countdown to activate
		if (Face.bCounter)
		{
			if (Face.bToggleable)
				MaterialInstance->SetScalarParameterValue(FName("CounterNum"), (Face.nMaxCount - (Face.nClickCount % (Face.nMaxCount + 1))));
			else
				MaterialInstance->SetScalarParameterValue(FName("CounterNum"), (Face.nMaxCount - Face.nClickCount));
		}		
		// Brittle Broken -- Denotes if the light is broken. If so, the level must be restarted.
		MaterialInstance->SetScalarParameterValue(FName("BrittleBroken"), Face.bBrittleBroken ? 1.0f : 0.0f);

		CubeObject->SetMaterial(MaterialInstance, Cube.CubeFaces[FaceIndex].oSide);
	}
}

///Called as an event delegate when a cube is clicked
void AGridScapeGameController::BindToDelegateCubeClicked(FVector ClickedCubeCoordinate)
{
	TraceMouseOnCubeClick(ClickedCubeCoordinate, 0);
}

///Called as an event delegate when a side is clicked
void AGridScapeGameController::BindToDelegateSideClicked()
{
	TraceMouseOnCubeClick(FVector::ZeroVector, 0);
}

///Called as an event delegate when a barrier is clicked
void AGridScapeGameController::BindToDelegateBarrierClicked(int32 BarrierIndex)
{
	TraceMouseOnCubeClick(FVector::ZeroVector, BarrierIndex);
}

///Called when we touch a cube to determine whether or not we should do anything, and if so do it.
void AGridScapeGameController::CubeTouchLogic(FVector ClickedCubeCoordinates, uint8 ClickedSide)
{
	FLevelStruct* CurrentLevel = GameModel->GetCurrentLevel();
	FVector4 ClickedSideCoordinate = FVector4(ClickedCubeCoordinates, ClickedSide);
	TArray<FVector4> ValidMoves = CurrentLevel->GetValidMoves();

	if (GameModel->LevelEditorMode)
	{
		GameModel->SetLevelEditorTile(ClickedSideCoordinate);
		GameModel->SetLevelEditorBarrier(-1);

		// Call the HUD blueprint. This has to be done in this roundabout way because UMG doesn't work in the same way as actors.
		FOutputDeviceNull ar;
		GameModel->GetHUD()->CallFunctionByNameWithArguments((TEXT("%s"), *CurrentLevel->LevelEditor_GetCubeFaceParams(ClickedSideCoordinate)), ar, NULL, true);

		SetPuzzleCubesMaterial(CurrentLevel);
		SetBarriersMaterial(CurrentLevel);
	}
	else
	{
		// Cube touch logic
		if (ValidMoves.Contains(ClickedSideCoordinate))
		{
			// For all cubes, we determine how to display the tiles
			TArray<FCubeFaceStruct*> AllFaces = CurrentLevel->GetAllFaces();
			for (int32 FaceIndex = 0; FaceIndex < AllFaces.Num(); FaceIndex++)
			{
				FCubeFaceStruct* LoopFace = AllFaces[FaceIndex];
				FVector4 LoopFaceCoordinate = FVector4(LoopFace->oCubeCoordinate, LoopFace->oSide);

				if (ClickedSideCoordinate.Equals(LoopFaceCoordinate))
				{
					LoopFace->bCurrentPos = true;
					LoopFace->nClickCount++;
				}
				else
				{
					LoopFace->bCurrentPos = false;
				}
				// If we've clicked the side we'll need to calculate the active status of the sides
				if (LoopFace->nClickCount > 0)
				{
					// Toggleable tile
					if (LoopFace->bToggleable)
					{
						int32 DefaultOnMod = (LoopFace->bDefaultOn) ? 1 : 0;
						if (LoopFace->bCounter)
						{
							if (LoopFace->bBrittle && LoopFace->nClickCount >= LoopFace->nMaxCount + DefaultOnMod + 1)
							{
								LoopFace->bBrittleBroken = true;
								LoopFace->bActive = false;
							}
							else
							{
								if ((LoopFace->nMaxCount - (LoopFace->nClickCount % (LoopFace->nMaxCount + 1))) == DefaultOnMod)
								{
									LoopFace->bActive = true;
								}
								else
								{
									LoopFace->bActive = false;
								}
							}
						}
						else
						{
							if (LoopFace->bBrittle && LoopFace->nClickCount >= DefaultOnMod + 2)
							{
								LoopFace->bBrittleBroken = true;
								LoopFace->bActive = false;
							}
							else
							{
								if (LoopFace->nClickCount % 2 == DefaultOnMod)
								{
									LoopFace->bActive = false;
								}
								else
								{
									LoopFace->bActive = true;
								}
							}
						}
					}
					// UnToggleable tile
					else
					{
						if (LoopFace->bCounter)
						{
							if (LoopFace->nClickCount >= LoopFace->nMaxCount)
							{
								LoopFace->bActive = true;
							}
							else
							{
								LoopFace->bActive = false;
							}
						}
						else
						{
							LoopFace->bActive = true;
						}
					}
				}


				LoopFace->bValidMove = false;
			}
		}
		SetValidMoves(CurrentLevel);
		SetPuzzleCubesMaterial(CurrentLevel);
		// Do a win check
		if (CurrentLevel->CheckWin())
		{
			GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Green, "YOU WIN");
		}
	}
}

///Called when we touch a barrier
void AGridScapeGameController::BarrierTouchLogic(int32 Index) //needs to be cleaned up - proof of concept mode atm
{
	// Only respond to touch/click if we're in level editor mode
	if (GameModel->LevelEditorMode)
	{
		GameModel->SetLevelEditorTile(FVector4(-1));
		GameModel->SetLevelEditorBarrier(Index);

		// Call the HUD blueprint. This has to be done in this roundabout way because UMG doesn't work in the same way as actors.
		FOutputDeviceNull ar;
		GameModel->GetHUD()->CallFunctionByNameWithArguments((TEXT("%s"), *GameModel->GetCurrentLevel()->LevelEditor_GetBarrierParams(Index)), ar, NULL, true);

		FLevelStruct* CurrentLevel = GameModel->GetCurrentLevel();
		SetBarriersMaterial(CurrentLevel);
		SetPuzzleCubesMaterial(CurrentLevel);
	}
}

///Called at when we touch a barrier
void AGridScapeGameController::SetBarrierMaterialParameters(FBarrierStruct Barrier, ABarrier* BarrierObject, bool LevelEditorSelected)
{
	UMaterialInstanceDynamic* MaterialInstance = UMaterialInstanceDynamic::Create(BarrierObject->GetMaterial(0), this);

	// Level Editor Selected -- In level editor mode, this is the cube face that is currently selected by the user
	MaterialInstance->SetScalarParameterValue(FName("LevelEditorSelected"), LevelEditorSelected ? 1.0f : 0.0f);
	// Block -- Blocks player movement in between the designated cube faces
	MaterialInstance->SetScalarParameterValue(FName("Block"), Barrier.bBlockMovement ? 1.0f : 0.0f);

	BarrierObject->SetMaterial(MaterialInstance, 0);
}

///Iterates through the puzzle barriers and sets their materials
void AGridScapeGameController::SetBarriersMaterial(FLevelStruct* CurrentLevel)
{
	TArray<ABarrier*> BarrierArray = GameModel->GetPuzzleBarriers();
	for (int32 BarrierIndex = 0; BarrierIndex < BarrierArray.Num(); BarrierIndex++)
	{
		ABarrier* LoopBarrier = BarrierArray[BarrierIndex];
		FBarrierStruct* LoopBarrierStruct = CurrentLevel->GetBarrier(BarrierIndex);

		if (LoopBarrier != nullptr && LoopBarrierStruct != nullptr)
		{
			SetBarrierMaterialParameters(*LoopBarrierStruct, LoopBarrier, (GameModel->GetLevelEditorBarrier() == BarrierIndex));
		}
	}
}

///Iterates through the puzzle cubes and sets their materials
void AGridScapeGameController::SetPuzzleCubesMaterial(FLevelStruct* CurrentLevel)
{
	TArray<ACube*> CubeArray = GameModel->GetPuzzleCubes();
	for (int32 CubeIndex = 0; CubeIndex < CubeArray.Num(); CubeIndex++)
	{
		ACube* LoopCube = CubeArray[CubeIndex];
		FVector CubeCoordinate = LoopCube->GetCoordinate();
		SetSideMaterialParameters(Helper::GetCubeStructWithCoordinates(*CurrentLevel, CubeCoordinate), LoopCube, (FVector(GameModel->GetLevelEditorTile()) == CubeArray[CubeIndex]->GetCoordinate()));
	}
}

///Called on cube click to trace mouse coordinates
void AGridScapeGameController::TraceMouseOnCubeClick(FVector ClickedCubeCoordinate, int32 BarrierIndex)
{
	// Mouse coordinates and direction
	FVector MouseLocation;
	FVector MouseDirection;
	GetWorld()->GetFirstPlayerController()->DeprojectMousePositionToWorld(MouseLocation, MouseDirection);

	// Line trace parameters
	FCollisionQueryParams RV_TraceParams = FCollisionQueryParams(FName(TEXT("RV_Trace")), true, this);
	RV_TraceParams.bTraceComplex = true;
	RV_TraceParams.bTraceAsyncScene = true;
	RV_TraceParams.bReturnPhysicalMaterial = false;

	// Hit result object
	FHitResult RV_Hit(ForceInit);

	// Trace the line
	GetWorld()->LineTraceSingleByChannel(RV_Hit, MouseLocation, (MouseLocation + (MouseDirection * 1500.0f)), ECC_Visibility, RV_TraceParams);

	if (RV_Hit.IsValidBlockingHit())
	{
		if (GameModel->GetSideLocked())
		{
			if (RV_Hit.GetActor()->GetClass() == GameModel->GetCubeClass())
			{
				// If we're locked to a side, then we can perform the cube logic
				CubeTouchLogic(ClickedCubeCoordinate, FSide::GetSideByVector(RV_Hit.Normal));
			}
			else if (RV_Hit.GetActor()->GetClass() == GameModel->GetBarrierClass())
			{
				BarrierTouchLogic(BarrierIndex);
			}
		}
		else
		{
			// If we're in free camera mode, detect our side to lock to
			SnapPlayerCamera(RV_Hit.Normal);
		}
	}
}

///Called on click to raytrace and determine which cube + side we are clicking
void AGridScapeGameController::SnapPlayerCamera(FVector SideNormal)
{
	// Calculate a corrected snap rotation for the player camera based on the side normal of the clicked cube
	FRotator SideNormalRotation = (SideNormal * -1.0f).ToOrientationRotator();
	bool onFace = Helper::FacingClickedFace(GameModel->GetPlayerPawn()->GetActorRotation().Vector().GetSafeNormal(), SideNormalRotation.Vector().GetSafeNormal());
	FRotator newCameraRotator = Helper::GetCorrectedFaceSnapRotation(onFace, GameModel->GetPlayerPawn()->GetActorRotation(), SideNormalRotation);

	APuzzleSide* PuzzleBox = GameModel->GetPuzzleSides();
	UCameraComponent* Camera = Cast<UCameraComponent>(GameModel->GetPlayerPawn()->GetComponentByClass(UCameraComponent::StaticClass()));

	// Calculate the exact zoom we need to fit the puzzle side to the screen
	float TargetDistance = 0.0f;
	if (PuzzleBox != nullptr && Camera != nullptr)
	{
		// Grab camera info
		float CameraAspectRatio = Camera->AspectRatio;
		float CameraFOV = Camera->FieldOfView;

		// Need to declare all these even though we're only using the BoxExtent variable
		FVector BoxOrigin = FVector::ZeroVector;
		FVector BoxDimensions = FVector::ZeroVector;
		float BoxRadius = 0.0f;

		// Grab the dimensions of the box
		UKismetSystemLibrary::GetComponentBounds(PuzzleBox->Box, BoxOrigin, BoxDimensions, BoxRadius);

		// Grab the dimensions of the box side that we are currently facing
		FVector2D BoxSize;
		float DistanceFromOrigin;
		Helper::GetBoxSizeAndOriginDistance(SideNormalRotation, SideNormal.GetAbs(), BoxDimensions, BoxSize, DistanceFromOrigin);

		// Get the final target distance for the camera zoom
		TargetDistance = Helper::GetTargetCameraZoomDistance(BoxSize, CameraFOV, CameraAspectRatio, DistanceFromOrigin);
	}

	// Set all of our model data
	GameModel->SetSideLocked(true);
	GameModel->SetSideLockRotator(newCameraRotator);
	GameModel->SetCurrentLockedSide(FSide::GetSideByVector(SideNormal));
	GameModel->SetSideLockZoom(TargetDistance);

	// Disable collision for the sides
	SetSideCollision(false);
}

///Sets the collision status of the puzzle sides
void AGridScapeGameController::SetSideCollision(bool Collision)
{
	APuzzleSide* PuzzleSides = GameModel->GetPuzzleSides();
	PuzzleSides->SetActorEnableCollision(Collision);
}

///Called to save level editor data
void AGridScapeGameController::SaveEditorLevel()
{
	//temp - only doing 1 level for now
	TArray<FLevelStruct> DefaultLevels;
	FLevelStruct* CurrentLevel = GameModel->GetCurrentLevel();
	DefaultLevels.Add(*CurrentLevel);

	UDefaultLevels* SaveDefaultLevels = Cast<UDefaultLevels>(UGameplayStatics::CreateSaveGameObject(UDefaultLevels::StaticClass()));
	SaveDefaultLevels->DefaultLevels = DefaultLevels;
	UGameplayStatics::SaveGameToSlot(SaveDefaultLevels, SaveDefaultLevels->SaveSlotName, SaveDefaultLevels->UserIndex);
}

///Called from GamePawn to give the GameModel the axis of 
void AGridScapeGameController::SetTouchClickAxis(ETouchIndex::Type TouchIndex, FVector2D newAxis, bool isCurrentlyPressed)
{
	// If we're not side locked, we can calculate the axis velocity of the touch event, otherwise just set it to 0
	if (!GameModel->GetSideLocked())
	{
		// If we're just starting the touch event
		if (!GameModel->GetTouchClickHold())
		{
			// If we're pressed down
			if (isCurrentlyPressed)
			{
				GameModel->SetTouchClickVelocity(FVector2D::ZeroVector);
				GameModel->SetTouchClickAxis(newAxis);
				GameModel->SetTouchClickHold(true);
			}
		}
		else
		{
			// If we've already been touching, calculate the velocity of our movements
			if (isCurrentlyPressed)
			{
				GameModel->SetTouchClickVelocity((newAxis - GameModel->GetTouchClickAxis()) / (1.0f / GameModel->GetRotationSpeed()));
				GameModel->SetTouchClickAxis(newAxis);
			}
			// If we've released
			else
			{
				GameModel->SetTouchClickHold(false);
			}
		}
	}
	else
	{
		GameModel->SetTouchClickVelocity(FVector2D::ZeroVector);
	}
}

///Called from the player pawn in order to unlock the puzzle 
void AGridScapeGameController::SetSideLocked()
{
	SetSideCollision(true);
	GameModel->SetSideLocked(false);
}

///Called to despawn the level
void AGridScapeGameController::DespawnLevel()
{
	TArray<ABarrier*> Barriers = GameModel->GetPuzzleBarriers();
	for (int32 BarrierIndex = 0; BarrierIndex < Barriers.Num(); BarrierIndex++)
	{
		ABarrier* Barrier = Barriers[BarrierIndex];
		if (Barrier != nullptr)
		{
			Barrier->Destroy();
		}
	}
	TArray<ACube*> Cubes = GameModel->GetPuzzleCubes();
	for (int32 CubeIndex = 0; CubeIndex < Cubes.Num(); CubeIndex++)
	{
		ACube* Cube = Cubes[CubeIndex];
		if (Cube != nullptr)
		{
			Cube->Destroy();
		}
	}
	APuzzleSide* Sides = GameModel->GetPuzzleSides();
	if (Sides != nullptr)
	{
		Sides->Destroy();
	}

	GameModel->LevelEditorMode = true;
}

//LEVEL EDITOR FUNCTIONS - VERY MUCH UNDER CONSTRUCTION

void AGridScapeGameController::LevelEditor_SetDefaultOn(bool Checked)
{
	if (GameModel->LevelEditorMode)
	{
		FVector4 SelectedTile = GameModel->GetLevelEditorTile();
		if (SelectedTile != FVector4(-1))
		{
			FLevelStruct* CurrentLevel = GameModel->GetCurrentLevel();
			FCubeFaceStruct* Face = CurrentLevel->GetFace(FVector(SelectedTile), SelectedTile.W);
			Face->bDefaultOn = Face->bDefaultOn ? false : true;
			Face->bActive = Face->bDefaultOn;

			SetValidMoves(CurrentLevel);
			SetPuzzleCubesMaterial(CurrentLevel);
		}
	}
}

void AGridScapeGameController::LevelEditor_SetToggleable(bool Checked)
{
	if (GameModel->LevelEditorMode)
	{
		FVector4 SelectedTile = GameModel->GetLevelEditorTile();
		if (SelectedTile != FVector4(-1))
		{
			FLevelStruct* CurrentLevel = GameModel->GetCurrentLevel();
			FCubeFaceStruct* Face = CurrentLevel->GetFace(FVector(SelectedTile), SelectedTile.W);
			Face->bToggleable = Face->bToggleable ? false : true;

			SetValidMoves(CurrentLevel);
			SetPuzzleCubesMaterial(CurrentLevel);
		}
	}
}

void AGridScapeGameController::LevelEditor_SetPlayerStart(bool Checked)
{
	if (GameModel->LevelEditorMode)
	{
		FVector4 SelectedTile = GameModel->GetLevelEditorTile();
		if (SelectedTile != FVector4(-1))
		{
			FLevelStruct* CurrentLevel = GameModel->GetCurrentLevel();
			CurrentLevel->SetPlayerPosition(SelectedTile);

			SetValidMoves(CurrentLevel);
			SetPuzzleCubesMaterial(CurrentLevel);
		}
	}
}

void AGridScapeGameController::LevelEditor_Unlock()
{
	SetSideLocked();
}

void AGridScapeGameController::LevelEditor_Save()
{
	if (GameModel->LevelEditorMode)
	{
		SaveEditorLevel();
		GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Cyan, "Save Successful.");
	}
	else
	{
		GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Red, "Can't save while playing the level, silly.");
	}
}

void AGridScapeGameController::LevelEditor_SetBarrierBlock(bool Checked)
{
	if (GameModel->LevelEditorMode)
	{
		int32 SelectedBarrier = GameModel->GetLevelEditorBarrier();
		if (SelectedBarrier != -1)
		{
			FLevelStruct* CurrentLevel = GameModel->GetCurrentLevel();
			FBarrierStruct* Face = CurrentLevel->GetBarrier(SelectedBarrier);
			Face->bBlockMovement = Checked;

			SetValidMoves(CurrentLevel);
			SetBarriersMaterial(CurrentLevel);
		}
	}
}

bool AGridScapeGameController::LevelEditor_SetGameMode()
{
	bool LevelEditorMode = false;
	if (GameModel->LevelEditorMode)
	{
		GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Green, "Now playing level");
		GameModel->LevelEditorMode = false;
		GameModel->SetLevelEditorLevel(*GameModel->GetCurrentLevel());
	}
	else
	{
		GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Red, "Level Editor Mode");
		//LoadEditorLevel(true);
		GameModel->LevelEditorMode = true;
		GameModel->SetCurrentLevel(GameModel->GetLevelEditorLevel());
		LevelEditorMode = true;
	}
	FLevelStruct* CurrentLevel = GameModel->GetCurrentLevel();

	GameModel->SetLevelEditorBarrier(-1);
	GameModel->SetLevelEditorTile(FVector4(-1));

	SetValidMoves(CurrentLevel);
	SetBarriersMaterial(CurrentLevel);
	SetPuzzleCubesMaterial(CurrentLevel);

	return LevelEditorMode;
}

void AGridScapeGameController::LevelEditor_SetCounter(bool Checked, int32 MaxCount)
{
	if (GameModel->LevelEditorMode)
	{
		FVector4 SelectedTile = GameModel->GetLevelEditorTile();
		if (SelectedTile != FVector4(-1))
		{
			FLevelStruct* CurrentLevel = GameModel->GetCurrentLevel();
			FCubeFaceStruct* Face = CurrentLevel->GetFace(FVector(SelectedTile), SelectedTile.W);

			if (Face->bCounter != Checked || Face->nMaxCount != MaxCount)
			{
				Face->bCounter = Checked;
				Face->nMaxCount = MaxCount;

				SetValidMoves(CurrentLevel);
				SetPuzzleCubesMaterial(CurrentLevel);
			}
		}
	}
}

void AGridScapeGameController::LevelEditor_SetBrittle(bool Checked)
{
	if (GameModel->LevelEditorMode)
	{
		FVector4 SelectedTile = GameModel->GetLevelEditorTile();
		if (SelectedTile != FVector4(-1))
		{
			FLevelStruct* CurrentLevel = GameModel->GetCurrentLevel();
			FCubeFaceStruct* Face = CurrentLevel->GetFace(FVector(SelectedTile), SelectedTile.W);

			if (Checked)
				Face->bToggleable = true;
			Face->bBrittle = Checked;

			SetValidMoves(CurrentLevel);
			SetPuzzleCubesMaterial(CurrentLevel);
		}
	}
}

void AGridScapeGameController::LevelEditor_NewLevel(FVector Dimensions, FString LevelName)
{
	if (Dimensions.Equals(FVector::ZeroVector)
		|| Dimensions.X > 6 
		|| Dimensions.Y > 6 
		|| Dimensions.Z > 6)
	{
		GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Red, "Bad input, try again bro.");
		return;
	}

	DespawnLevel();
	FLevelStruct NewLevel = FLevelStruct(Dimensions, LevelName);
	GameModel->SetCurrentLevel(NewLevel);
	GeneratePuzzle();
}

void AGridScapeGameController::HUD_SetHUDObject(UUserWidget* HUD)
{
	GameModel->SetHUD(HUD);
}