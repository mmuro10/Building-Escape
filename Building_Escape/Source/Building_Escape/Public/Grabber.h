// GameDev.tv Team. Learn C++ and Make Video Games

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "PhysicsEngine/PhysicsHandleComponent.h"
#include "Grabber.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class BUILDING_ESCAPE_API UGrabber : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UGrabber();

	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;
	void Grab();
	void Released();
	void FindPhysicsHandle();
	void SetUpInputComponent();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

private:
	float Reach = 200.f;

	UPhysicsHandleComponent* PhysicsHandle = nullptr;
	UInputComponent* InputComponent = nullptr;	

	//Return the first Actor within reach with physics body
	FHitResult GetFirstPhysicsBodyInReach() const;

	FVector GetLineTraceEnd() const;
	FVector GetPlayersWorldPos() const;
};
