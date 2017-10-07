// Fill out your copyright notice in the Description page of Project Settings.

#include "GridScape.h"
#include "Barrier.h"


// Sets default values
ABarrier::ABarrier()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	// Create the object
	Barrier = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Barrier"));
	RootComponent = Barrier;

	// Set up click event
	OnClicked.AddUniqueDynamic(this, &ABarrier::OnBarrierClicked);
	Barrier->OnInputTouchBegin.AddUniqueDynamic(this, &ABarrier::OnBarrierTouch);
}

// Called when the game starts or when spawned
void ABarrier::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ABarrier::Tick( float DeltaTime )
{
	Super::Tick( DeltaTime );

}

void ABarrier::SetIndex(int32 Index)
{
	this->Index = Index;
}

int32 ABarrier::GetIndex()
{
	return Index;
}

UMaterial* ABarrier::GetMaterial(int32 Channel)
{
	return Barrier->GetMaterial(Channel)->GetMaterial();
}

void ABarrier::SetMaterial(UMaterialInstanceDynamic* Material, int32 Channel)
{
	Barrier->SetMaterial(Channel, Material);
}

void ABarrier::OnBarrierClicked(AActor* TouchedActor, FKey ButtonClicked)
{
	OnBarrierClick.Broadcast(Index);
}

void ABarrier::OnBarrierTouch(ETouchIndex::Type FingerIndex, UPrimitiveComponent* TouchedComponent)
{
	OnBarrierClick.Broadcast(Index);
}