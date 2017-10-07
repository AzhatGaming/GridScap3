// Fill out your copyright notice in the Description page of Project Settings.

#include "GridScape.h"
#include "PuzzleSide.h"


// Sets default values
APuzzleSide::APuzzleSide()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	// Create the box object
	Box = CreateDefaultSubobject<UBoxComponent>(TEXT("Box"));
	Box->SetBoxExtent(FVector(50.f));
	RootComponent = Box;

	// Set up the click event
	OnClicked.AddUniqueDynamic(this, &APuzzleSide::OnSideClicked);
	Box->OnInputTouchBegin.AddUniqueDynamic(this, &APuzzleSide::OnSideTouch);
}

// Called when the game starts or when spawned
void APuzzleSide::BeginPlay()
{
	Super::BeginPlay();

}

// Called every frame
void APuzzleSide::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void APuzzleSide::SetSide(uint8 newSide)
{
	thisSide = newSide;
}

uint8 APuzzleSide::GetSide()
{
	return thisSide;
}

void APuzzleSide::OnSideClicked(AActor* TouchedActor, FKey ButtonClicked)
{
	OnSideClick.Broadcast();
}

void APuzzleSide::OnSideTouch(ETouchIndex::Type FingerIndex, UPrimitiveComponent* TouchedComponent)
{
	OnSideClick.Broadcast();
}