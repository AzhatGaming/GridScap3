// Fill out your copyright notice in the Description page of Project Settings.

#include "GridScape.h"
#include "GamePawn.h"


// Sets default values
AGamePawn::AGamePawn()
{
 	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	//Take control of the default Player
	AutoPossessPlayer = EAutoReceiveInput::Player0;
	AutoReceiveInput = EAutoReceiveInput::Player0;

	// Spring Arm settings
	SpringArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArm"));
	SpringArm->TargetArmLength = 1000.0f;
	SpringArm->bEnableCameraLag = true;
	SpringArm->bDoCollisionTest = false;
	RootComponent = SpringArm;

	// Camera settings
	Camera = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));
	Camera->SetupAttachment(SpringArm);
}

// Called when the game starts or when spawned
void AGamePawn::BeginPlay()
{
	Super::BeginPlay();

	GameController->SetPlayerPawn(this);
}

// Called every frame
void AGamePawn::Tick( float DeltaTime )
{
	Super::Tick( DeltaTime );

	FVector2D TouchAxis;
	bool IsCurrentlyPressed;
	this->GetWorld()->GetFirstPlayerController()->GetInputTouchState(ETouchIndex::Touch1, TouchAxis.X, TouchAxis.Y, IsCurrentlyPressed);

	GameController->SetTouchClickAxis(ETouchIndex::Touch1, TouchAxis, IsCurrentlyPressed);
}

// Called to bind functionality to input
void AGamePawn::SetupPlayerInputComponent(class UInputComponent* InputComponent)
{
	Super::SetupPlayerInputComponent(InputComponent);

	InputComponent->BindAction("UnlockCube", IE_Pressed, this, &AGamePawn::SetSideLocked);
}

void AGamePawn::SetSideLocked()
{
	GameController->SetSideLocked();
}

USpringArmComponent* AGamePawn::GetSpringArmComponent()
{
	return SpringArm;
}