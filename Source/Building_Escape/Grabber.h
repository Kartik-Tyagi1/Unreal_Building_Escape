#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "PhysicsEngine/PhysicsHandleComponent.h"
#include "Grabber.generated.h"


struct PositionAndReach
{
	FVector PlayerViewPointLocation;
	FVector PlayerReach;
};

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class BUILDING_ESCAPE_API UGrabber : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UGrabber();

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

private:
	float Reach = 100.f;
	UPhysicsHandleComponent* PhysicsHandle = nullptr; // Protects aginst crashing due to loading times for componenets
	UInputComponent* InputComponent = nullptr;

private:
	void Grab();
	void Release();
	void FindPhysicsHandle();
	void SetupInputComponent();

	// Returns first actor within reach with a physics body
	FHitResult GetFirstPhysicsBodyInReach() const;

	// Return player reach and postion in the world
	PositionAndReach GetPlayerPositionAndReach() const;
};
