#include "WindController.h"
#include "Components/SphereComponent.h"
#include "Components/PrimitiveComponent.h"
#include "GameFramework/Actor.h"
#include "Kismet/GameplayStatics.h"
#include "GameFramework/PlayerController.h"

AWindController::AWindController()
{
    PrimaryActorTick.bCanEverTick = true;

    WindArea = CreateDefaultSubobject<USphereComponent>(TEXT("WindArea"));
    RootComponent = WindArea;

    WindArea->SetSphereRadius(500.0f);
    WindArea->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
    WindArea->SetCollisionObjectType(ECC_WorldDynamic);
    WindArea->SetCollisionResponseToAllChannels(ECR_Overlap);
}

void AWindController::BeginPlay()
{
    Super::BeginPlay();

    if (APlayerController* PlayerController = GetWorld()->GetFirstPlayerController())
    {
        EnableInput(PlayerController);
        InputComponent->BindAction("IncreaseWind", IE_Pressed, this, &AWindController::IncreaseWindStrength);
        InputComponent->BindAction("DecreaseWind", IE_Pressed, this, &AWindController::DecreaseWindStrength);
    }
}

void AWindController::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);
    ApplyWind();
}

void AWindController::ApplyWind()
{
    TArray<AActor*> OverlappingActors;
    WindArea->GetOverlappingActors(OverlappingActors);

    for (AActor* Actor : OverlappingActors)
    {
        UPrimitiveComponent* PhysicsComponent = Cast<UPrimitiveComponent>(Actor->FindComponentByClass<UPrimitiveComponent>());
        if (PhysicsComponent && PhysicsComponent->IsSimulatingPhysics())
        {
            FVector Force = WindDirection * WindForce;
            PhysicsComponent->AddForce(Force, NAME_None, true);

            UE_LOG(LogTemp, Warning, TEXT("Applying Wind to %s | Force: X=%f, Y=%f, Z=%f"),
                *Actor->GetName(), Force.X, Force.Y, Force.Z);
        }
    }
}

void AWindController::IncreaseWindStrength()
{
    WindForce += 1000.0f;
    UE_LOG(LogTemp, Warning, TEXT("Wind Strength Increased: %f"), WindForce);
}

void AWindController::DecreaseWindStrength()
{
    WindForce = FMath::Max(WindForce - 1000.0f, 0.0f);
    UE_LOG(LogTemp, Warning, TEXT("Wind Strength Decreased: %f"), WindForce);
}

void AWindController::ChangeWindDirection(FVector NewDirection)
{
    WindDirection = NewDirection.GetSafeNormal();
    UE_LOG(LogTemp, Warning, TEXT("Wind Direction Changed: X=%f, Y=%f, Z=%f"), WindDirection.X, WindDirection.Y, WindDirection.Z);
}
