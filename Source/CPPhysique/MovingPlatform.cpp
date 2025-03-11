#include "MovingPlatform.h"
#include "Components/StaticMeshComponent.h"
#include "GameFramework/Actor.h"
#include "UObject/ConstructorHelpers.h"

AMovingPlatform::AMovingPlatform()
{
    PrimaryActorTick.bCanEverTick = true;
    
    PlatformMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("PlatformMesh"));
    RootComponent = PlatformMesh;
    
    PlatformMesh->SetSimulatePhysics(true);
    PlatformMesh->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
    PlatformMesh->SetCollisionObjectType(ECC_WorldDynamic);
    
    static ConstructorHelpers::FObjectFinder<UStaticMesh> PlatformMeshAsset(TEXT("/Game/StarterContent/Shapes/Shape_Cube.Shape_Cube"));
    if (PlatformMeshAsset.Succeeded())
    {
        PlatformMesh->SetStaticMesh(PlatformMeshAsset.Object);
    }
}

void AMovingPlatform::BeginPlay()
{
    Super::BeginPlay();

    if (Waypoints.Num() == 0)
    {
        UE_LOG(LogTemp, Error, TEXT("No waypoints set for MovingPlatform!"));
        return;
    }

    SetActorLocation(Waypoints[0]);
}

void AMovingPlatform::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);
    MovePlatform(DeltaTime);
}

void AMovingPlatform::MovePlatform(float DeltaTime)
{
    if (Waypoints.Num() == 0) return;

    FVector TargetLocation = Waypoints[CurrentWaypointIndex];
    FVector CurrentLocation = GetActorLocation();

    FVector Direction = (TargetLocation - CurrentLocation).GetSafeNormal();
    FVector Force = Direction * MoveSpeed * ForceMultiplier;

    PlatformMesh->AddForce(Force, NAME_None, true);

    float DistanceToTarget = FVector::Dist(CurrentLocation, TargetLocation);
    if (DistanceToTarget < 50.0f)
    {
        CurrentWaypointIndex = (CurrentWaypointIndex + 1) % Waypoints.Num();
    }
}

