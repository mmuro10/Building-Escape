// GameDev.tv Team. Learn C++ and Make Video Games


#include "Grabber.h"
#include "Components/AudioComponent.h"
#include "GameFramework/PlayerController.h"
#include "DrawDebugHelpers.h"

#define OUT
// Sets default values for this component's properties
UGrabber::UGrabber()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;
}


// Called when the game starts
void UGrabber::BeginPlay()
{
	Super::BeginPlay();

	// ...
	
	FindPhysicsHandle();
	SetUpInputComponent();
	FindAudioComponent();
}

void UGrabber::Grab()
{
	//Raycast when button pressed and Try and reach any Actor with collision body set

	FHitResult HitResult = GetFirstPhysicsBodyInReach();
	UPrimitiveComponent* ComponentToGrab = HitResult.GetComponent();
	AActor* ActorHit = HitResult.GetActor();

		//if hit something then attach phyics handle
	if (ActorHit)
	{
		//protection for pointer
		if (!PhysicsHandle) { return; };
		PhysicsHandle->GrabComponentAtLocation
		(
			ComponentToGrab,
			NAME_None,
			GetLineTraceEnd()
		);
		
		DropSound = false;
		if (!AudioComponent) { return; }
		if (!PickUpSound)
		{
			AudioComponent->Play();
			PickUpSound = true;
		}
	}
}

void UGrabber::Released()
{
	//protection for pointer
	if (!PhysicsHandle) { return; };
	//Remove release physics handle
	PhysicsHandle->ReleaseComponent();

	PickUpSound = false;
	if (!AudioComponent) { return; }
	if (!DropSound)
	{
		AudioComponent->Play();
		DropSound = true;
	}
}

// Called every frame
void UGrabber::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	//protection for pointer
	if (!PhysicsHandle) { return; };
	if (PhysicsHandle->GrabbedComponent)
	{
		//Physics handle is found
		PhysicsHandle->SetTargetLocation(GetLineTraceEnd());
	}
}

//Checking if PhysicsHandle component 
void UGrabber::FindPhysicsHandle()
{
	PhysicsHandle = GetOwner()->FindComponentByClass<UPhysicsHandleComponent>();

	if (!PhysicsHandle)
	{
		UE_LOG(LogTemp, Error, TEXT("%s has no PhysicsHandle."), *GetOwner()->GetName());
	}
}

void UGrabber::SetUpInputComponent()
{
	InputComponent = GetOwner()->FindComponentByClass<UInputComponent>();
	if (InputComponent)
	{
		//UE_LOG(LogTemp, Error, TEXT("%s has an InputComponent."), *GetOwner()->GetName());
		InputComponent->BindAction("Grab", IE_Pressed, this, &UGrabber::Grab);
		InputComponent->BindAction("Grab", IE_Released, this, &UGrabber::Released);
	}
}

FHitResult UGrabber::GetFirstPhysicsBodyInReach() const
{
	FHitResult Hit;
	
	//ray-cast out to a certain distance (Reach)
	FCollisionQueryParams TraceParams(FName(TEXT("")), false, GetOwner());
	
	//get players viewpoint
	//Used to visually see how far you can reach
	/*
		DrawDebugLine(
			GetWorld(),
			PlayerViewPointLocation,
			LineTraceEnd,
			FColor(255,0,0),
			false,
			0.f,
			0,
			5.f
			);
	*/
	
	GetWorld()->LineTraceSingleByObjectType(
		Hit,
		GetPlayersWorldPos(),
		GetLineTraceEnd(),
		FCollisionObjectQueryParams(ECollisionChannel::ECC_PhysicsBody),
		TraceParams
	);

	return Hit;
}

FVector UGrabber::GetLineTraceEnd() const
{

	FVector PlayerViewPointLocation;
	FRotator PlayerViewPointRotation;

	GetWorld()->GetFirstPlayerController()->GetPlayerViewPoint(OUT PlayerViewPointLocation, OUT PlayerViewPointRotation);

	return PlayerViewPointLocation + PlayerViewPointRotation.Vector() * Reach;
}

//Get Players Position in the world
FVector UGrabber::GetPlayersWorldPos() const
{
	FVector PlayerViewPointLocation;
	FRotator PlayerViewPointRotation;

	GetWorld()->GetFirstPlayerController()->GetPlayerViewPoint(OUT PlayerViewPointLocation, OUT PlayerViewPointRotation);

	return PlayerViewPointLocation;

}

void UGrabber::FindAudioComponent()
{
	AudioComponent = GetOwner()->FindComponentByClass<UAudioComponent>();
	
	if (!AudioComponent)
	{
		UE_LOG(LogTemp, Error, TEXT("%s Missing audio component!"), *GetOwner()->GetName());
	}
}