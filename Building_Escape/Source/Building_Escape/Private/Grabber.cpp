// GameDev.tv Team. Learn C++ and Make Video Games


#include "Grabber.h"
#include "GameFramework/PlayerController.h"
#include "DrawDebugHelpers.h"

#define OUT
// Sets default values for this component's properties
UGrabber::UGrabber()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;
	//UE_LOG(LogTemp, Warning, TEXT("Grabber reporting for duty!"));
	// ...
}


// Called when the game starts
void UGrabber::BeginPlay()
{
	Super::BeginPlay();

	// ...
	
}


// Called every frame
void UGrabber::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	FVector PlayerViewPointLocation;
	FRotator PlayerViewPointRotation;
	/* , */

	//get players viewpoint
	GetWorld()->GetFirstPlayerController()->GetPlayerViewPoint(OUT PlayerViewPointLocation, OUT PlayerViewPointRotation);
	// ...
	UE_LOG(LogTemp, Warning, TEXT("The viewpoint Location %s and Rotation: %s"), *PlayerViewPointLocation.ToString(), *PlayerViewPointRotation.ToString());
	
	//ray-cast out to a certain distance (Reach)
	FVector LineTraceEnd = PlayerViewPointLocation + PlayerViewPointRotation.Vector() * Reach;

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
}

