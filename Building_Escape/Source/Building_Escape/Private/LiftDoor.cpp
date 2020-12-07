// GameDev.tv Team. Learn C++ and Make Video Games


#include "LiftDoor.h"
#include "Components/AudioComponent.h"
#include "GameFramework/Actor.h"
#include "GameFramework/PlayerController.h"
#include "Engine/World.h"

// Sets default values for this component's properties
ULiftDoor::ULiftDoor()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}

void ULiftDoor::BeginPlay()
{
	Super::BeginPlay();

	// ...
	InitialZ = GetOwner()->GetActorLocation().Z;
	CurrentZ = InitialZ;
	OpenHeight += InitialZ;


	FindPressurePlate();
	FindAudioComponent();
	
}

// Called every frame
void ULiftDoor::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
	if (TotalMassOfActors() >= WeightLimit)
	{
		LiftDoor(DeltaTime);
		DoorLastOpened = GetWorld()->GetTimeSeconds();
	}
	else
	{
		if (GetWorld()->GetTimeSeconds() - DoorLastOpened > DoorCloseDelay)
		{
			CloseDoor(DeltaTime);
		}
	}
}

void ULiftDoor::FindPressurePlate() const
{
	if (!PressurePlate)
	{
		UE_LOG(LogTemp, Error, TEXT("%s has OpenDoor component, but no pressureplateset"), *GetOwner()->GetName());
	}
}

void ULiftDoor::LiftDoor(float DeltaTime)
{
	CurrentZ = FMath::Lerp(CurrentZ, OpenHeight, DeltaTime * DoorOpenVelocity);
	FVector DoorLift = GetOwner()->GetActorLocation();
	DoorLift.Z = CurrentZ;
	GetOwner()->SetActorLocation(DoorLift);

	CloseDoorSound = false;
	if (!AudioComponent) { return; }
	if (!OpenDoorSound)
	{
		AudioComponent->Play();
		OpenDoorSound = true;
	}
}

void ULiftDoor::CloseDoor(float DeltaTime)
{
	CurrentZ = FMath::Lerp(CurrentZ, InitialZ, DeltaTime * DoorCloseVelocity);
	FVector DoorLift = GetOwner()->GetActorLocation();
	DoorLift.Z = CurrentZ;
	GetOwner()->SetActorLocation(DoorLift);
	
	OpenDoorSound = false;
	if (!AudioComponent) { return; }
	if (!CloseDoorSound)
	{
		AudioComponent->Play();
		CloseDoorSound = true;
	}
}

float ULiftDoor::TotalMassOfActors() const
{
	float TotalMass = 0.f;

	//find all overlapping actors
	TArray<AActor*> OverLappingActors;

	if (!PressurePlate) { return TotalMass; }
	PressurePlate->GetOverlappingActors(OverLappingActors);
	//add up their masses

	for (AActor* Actor : OverLappingActors)
	{
		TotalMass += Actor->FindComponentByClass<UPrimitiveComponent>()->GetMass();
	}

	return TotalMass;
}

void ULiftDoor::FindAudioComponent()
{
	AudioComponent = GetOwner()->FindComponentByClass<UAudioComponent>();

	if (!AudioComponent)
	{
		UE_LOG(LogTemp, Error, TEXT("%s Missing audio component!"), *GetOwner()->GetName());
	}
}


// Called when the game starts

