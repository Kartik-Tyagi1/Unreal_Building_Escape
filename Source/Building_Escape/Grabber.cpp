#include "Grabber.h"
#include "Engine/World.h"
#include "GameFramework/PlayerController.h" // GetPlayerViewPoint()
#include "DrawDebugHelpers.h"

#define OUT // This is not functional, it is only for readability so it is easy to tell what variables are out variables 

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

	FindPhysicsHandle();
	SetupInputComponent();
}

void UGrabber::SetupInputComponent()
{
	InputComponent = GetOwner()->FindComponentByClass<UInputComponent>();
	if(InputComponent)
	{
		InputComponent->BindAction("Grab", IE_Pressed, this, &UGrabber::Grab);
		InputComponent->BindAction("Grab", IE_Released, this, &UGrabber::Release);
	}
}

void UGrabber::FindPhysicsHandle()
{
	PhysicsHandle = GetOwner()->FindComponentByClass<UPhysicsHandleComponent>();
	if(!PhysicsHandle) // PhysicsHandle == nullptr
		UE_LOG(LogTemp, Error, TEXT("%s Does not have a PhysicsHandle Component"), *GetOwner()->GetName());
}

void UGrabber::Grab()
{
	UE_LOG(LogTemp, Warning, TEXT("Grabber Pressed"));
	
	FHitResult HitResult = GetFirstPhysicsBodyInReach();
	AActor* ActorHit = HitResult.GetActor();
	PositionAndReach PR = GetPlayerPositionAndReach();
	UPrimitiveComponent* ComponentToGrab = HitResult.GetComponent();
	
	if(ActorHit)
	{
		if(!PhysicsHandle) {return;}
		PhysicsHandle->GrabComponentAtLocation
		(
			ComponentToGrab,
			NAME_None,
			PR.PlayerReach
		);
	}
}

void UGrabber::Release()
{
	UE_LOG(LogTemp, Warning, TEXT("Grabber Released"));
	if(!PhysicsHandle) {return;}
	PhysicsHandle->ReleaseComponent();
}

// Called every frame
void UGrabber::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
	PositionAndReach PR = GetPlayerPositionAndReach();
	// If physics handle is attached
	if(!PhysicsHandle) {return;}
	if(PhysicsHandle->GrabbedComponent)
	{
		// Move object we are holding
		PhysicsHandle->SetTargetLocation(PR.PlayerReach);
	}	
}

FHitResult UGrabber::GetFirstPhysicsBodyInReach() const
{
	FHitResult Hit;
	FCollisionQueryParams TraceParams{
		FName(TEXT("")),
		false,
		GetOwner()
	};

	// Ray Cast out to a certain distance
	PositionAndReach PR = GetPlayerPositionAndReach();

	GetWorld()->LineTraceSingleByObjectType(
		OUT Hit,
		PR.PlayerViewPointLocation,
		PR.PlayerReach,
		FCollisionObjectQueryParams(ECollisionChannel::ECC_PhysicsBody),
		TraceParams
	);

	// See what the ray hits
	AActor* ActorHit = Hit.GetActor();
	if(ActorHit)
		UE_LOG(LogTemp, Warning, TEXT("Line Trace Has Hit: %s"), *ActorHit->GetName());

	return Hit;
}

PositionAndReach UGrabber::GetPlayerPositionAndReach() const 
{
	PositionAndReach PR;

	FVector PlayerViewPointLocation;
	FRotator PlayerViewPointRotation;
	GetWorld()->GetFirstPlayerController()->GetPlayerViewPoint(
		OUT PlayerViewPointLocation, 
		OUT PlayerViewPointRotation
	);

	// Draw a line from the player
	// FVector LineTraceDirection = PlayerViewPointRotation.Vector() * Reach;
	// FVector LineTraceEnd = PlayerViewPointLocation + LineTraceDirection;

	PR.PlayerViewPointLocation = PlayerViewPointLocation;
	PR.PlayerReach = PlayerViewPointLocation + PlayerViewPointRotation.Vector() * Reach;

	return PR;
}

