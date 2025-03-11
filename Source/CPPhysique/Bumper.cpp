#include "Bumper.h"
#include "Components/StaticMeshComponent.h"
#include "Components/SphereComponent.h"
#include "Ball.h"
#include "GameFramework/Actor.h"
#include "Kismet/GameplayStatics.h"
#include "UObject/ConstructorHelpers.h"

// Sets default values
ABumper::ABumper()
{
    PrimaryActorTick.bCanEverTick = true; // Enable ticking

    // Create and set up the static mesh component
    BumperMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("BumperMesh"));
    RootComponent = BumperMesh;

    // Ensure it is a static object
    BumperMesh->SetSimulatePhysics(false);
    BumperMesh->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
    BumperMesh->SetCollisionObjectType(ECC_WorldStatic);

    // Automatically set a default mesh for the bumper
    static ConstructorHelpers::FObjectFinder<UStaticMesh> BumperMeshAsset(TEXT("/Game/StarterContent/Shapes/Shape_Cylinder.Shape_Cylinder"));
    if (BumperMeshAsset.Succeeded())
    {
        BumperMesh->SetStaticMesh(BumperMeshAsset.Object);
    }

    // Create the detection sphere for collision checking
    DetectionSphere = CreateDefaultSubobject<USphereComponent>(TEXT("DetectionSphere"));
    DetectionSphere->SetupAttachment(RootComponent);
    DetectionSphere->SetSphereRadius(100.0f); // Adjust detection radius as needed
    DetectionSphere->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
    DetectionSphere->SetCollisionObjectType(ECC_WorldDynamic);
    DetectionSphere->SetCollisionResponseToAllChannels(ECR_Overlap);
}

// Called when the game starts or when spawned
void ABumper::BeginPlay()
{
    Super::BeginPlay();
}

// Called every frame to check for collisions
void ABumper::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);
    CheckForCollisions();
}

// Check for overlapping objects
void ABumper::CheckForCollisions()
{
    TArray<AActor*> OverlappingActors;
    DetectionSphere->GetOverlappingActors(OverlappingActors);

    for (AActor* Actor : OverlappingActors)
    {
        // Ensure it's a valid physics object (like BP_Ball)
        UPrimitiveComponent* BallMesh = Cast<UPrimitiveComponent>(Actor->GetRootComponent());
        if (BallMesh && BallMesh->IsSimulatingPhysics())
        {
            // Get the normal direction to bounce away
            FVector BounceDirection = (Actor->GetActorLocation() - GetActorLocation()).GetSafeNormal();
            BounceDirection *= BounceForce;

            // Apply impulse
            BallMesh->AddImpulse(BounceDirection, NAME_None, true);

            UE_LOG(LogTemp, Warning, TEXT("Bumper: Applied bounce force to %s | Force: X=%f, Y=%f, Z=%f"), 
                *Actor->GetName(), BounceDirection.X, BounceDirection.Y, BounceDirection.Z);
        }
    }
}
