// GameDev.tv Team. Learn C++ and Make Video Games
#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Engine/TriggerVolume.h"
#include "OpenDoor.generated.h"

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class BUILDING_ESCAPE_API UOpenDoor : public UActorComponent
{
	GENERATED_BODY()
public:	
	// Sets default values for this component's properties
	UOpenDoor();
	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	void OpenDoor(float DeltaTime);

	void CloseDoor(float DeltaTime);

	float TotalMassOfActors() const;

	void FindAudioComponent();

	void FindPressurePlate() const;

	bool OpenDoorSound = false;
	bool CloseDoorSound = true;

protected:
	// Called when the game starts
	virtual void BeginPlay() override;


private:

	float InitialYaw;
	float CurrentYaw;
	float DoorLastOpened = 0.f;
	
	UPROPERTY(EditAnywhere)
	ATriggerVolume* PressurePlate = nullptr;

	UPROPERTY(EditAnywhere)
	float DoorCloseDelay = 2.f;

	UPROPERTY(EditAnywhere)
	float OpenAngle = 90.f;

	UPROPERTY(EditAnywhere)
	float DoorOpenVelocity = 1.f;

	UPROPERTY(EditAnywhere)
	float DoorCloseVelocity = 1.f;

	UPROPERTY(EditAnywhere)
	float WeightLimit = 100.f;

	UPROPERTY()
	UAudioComponent* AudioComponent = nullptr;
};
