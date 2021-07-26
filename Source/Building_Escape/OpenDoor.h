#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Engine/TriggerVolume.h"
#include "OpenDoor.generated.h" // This must be the last include


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class BUILDING_ESCAPE_API UOpenDoor : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UOpenDoor();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;
	
	void OpenDoor(float DeltaTime);
	void CloseDoor(float DeltaTime);
	float TotalMassOfActors() const;

private:
	float  InitialYaw, CurrentYaw;
	float DoorLastOpened = 0.f;

private:
	UPROPERTY(EditAnywhere) // Variables under this are exposed to the editor
	float DoorOpenAngle = 90.f; 
	
	UPROPERTY(EditAnywhere)
	float DoorCloseDelay = 0.5f; 

	UPROPERTY(EditAnywhere)
	float DoorOpenSpeed = 0.8f;
	
	UPROPERTY(EditAnywhere)
	float DoorCloseSpeed = 2.f;

	UPROPERTY(EditAnywhere) // Must set a new one for each variable we want exposed
	ATriggerVolume* PressurePlate;

	UPROPERTY(EditAnywhere)
	AActor* ActorThatOpens;	

	UPROPERTY(EditAnywhere)
	float MassToOpenDoor = 60.f;

};
