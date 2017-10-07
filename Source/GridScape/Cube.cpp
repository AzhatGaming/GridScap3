// Fill out your copyright notice in the Description page of Project Settings.

#include "GridScape.h"
#include "Cube.h"


// Sets default values
ACube::ACube()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	// Create our cube object
	Cube = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Cube"));
	RootComponent = Cube;

	// Set up the click event
	OnClicked.AddUniqueDynamic(this, &ACube::OnCubeClicked);
	Cube->OnInputTouchBegin.AddUniqueDynamic(this, &ACube::OnCubeTouch);
}

// Called when the game starts or when spawned
void ACube::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ACube::Tick( float DeltaTime )
{
	Super::Tick( DeltaTime );

}

FVector ACube::GetCoordinate()
{
	return Coordinate;
}

void ACube::SetCoordinate(FVector newCoordinate)
{
	Coordinate = newCoordinate;
}

UMaterial* ACube::GetMaterial(int32 Channel)
{
	return Cube->GetMaterial(Channel)->GetMaterial();
}

void ACube::SetMaterial(UMaterialInstanceDynamic* Material, int32 Channel)
{
	Cube->SetMaterial(Channel, Material);
}

void ACube::OnCubeClicked(AActor* TouchedActor, FKey ButtonClicked)
{
	OnCubeClick.Broadcast(Coordinate);
}

void ACube::OnCubeTouch(ETouchIndex::Type FingerIndex, UPrimitiveComponent* TouchedComponent)
{
	OnCubeClick.Broadcast(Coordinate);
}