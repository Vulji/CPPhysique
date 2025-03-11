#include "Bumper.h"
#include "Components/StaticMeshComponent.h"
#include "Components/SphereComponent.h"
#include "Ball.h"
#include "GameFramework/Actor.h"
#include "UObject/ConstructorHelpers.h"

ABumper::ABumper()
{
    PrimaryActorTick.bCanEverTick = true;
    
    BumperMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("BumperMesh"));
    RootComponent = BumperMesh;
    
    BumperMesh->SetSimulatePhysics(false);
    BumperMesh->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
    BumperMesh->SetCollisionObjectType(ECC_WorldStatic);
    
    static ConstructorHelpers::FObjectFinder<UStaticMesh> BumperMeshAsset(TEXT("/Game/StarterContent/Shapes/Shape_Cylinder.Shape_Cylinder"));
    if (BumperMeshAsset.Succeeded())
    {
        BumperMesh->SetStaticMesh(BumperMeshAsset.Object);
    }
    
    DetectionSphere = CreateDefaultSubobject<USphereComponent>(TEXT("DetectionSphere"));
    DetectionSphere->SetupAttachment(RootComponent);
    DetectionSphere->SetSphereRadius(100.0f); 
    DetectionSphere->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
    DetectionSphere->SetCollisionObjectType(ECC_WorldDynamic);
    DetectionSphere->SetCollisionResponseToAllChannels(ECR_Overlap);
}

void ABumper::BeginPlay()
{
    Super::BeginPlay();
}

void ABumper::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);
    CheckForCollisions();
}

void ABumper::CheckForCollisions()
{
    TArray<AActor*> OverlappingActors;
    DetectionSphere->GetOverlappingActors(OverlappingActors);

    for (AActor* Actor : OverlappingActors)
    {
        UPrimitiveComponent* BallMesh = Cast<UPrimitiveComponent>(Actor->GetRootComponent());
        if (BallMesh && BallMesh->IsSimulatingPhysics())
        {
            FVector BounceDirection = (Actor->GetActorLocation() - GetActorLocation()).GetSafeNormal();
            BounceDirection *= BounceForce;
            
            BallMesh->AddImpulse(BounceDirection, NAME_None, true);

            UE_LOG(LogTemp, Warning, TEXT("Bumper: Applied bounce force to %s | Force: X=%f, Y=%f, Z=%f"), 
                *Actor->GetName(), BounceDirection.X, BounceDirection.Y, BounceDirection.Z);
        }
    }
}
