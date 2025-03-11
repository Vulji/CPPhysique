#include "Ball.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "GameFramework/PlayerController.h"
#include "Components/StaticMeshComponent.h"

ABall::ABall()
{
    PrimaryActorTick.bCanEverTick = true;
    
    BallMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("BallMesh"));
    RootComponent = BallMesh;
    
    BallMesh->SetSimulatePhysics(true);
    BallMesh->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);

    // Set default mesh (replace with your actual asset path)
    static ConstructorHelpers::FObjectFinder<UStaticMesh> BallMeshAsset(TEXT("/Game/StarterContent/Shapes/Shape_Sphere.Shape_Sphere"));
    if (BallMeshAsset.Succeeded())
    {
        BallMesh->SetStaticMesh(BallMeshAsset.Object);
    }
}


// Called when the game starts or when spawned
void ABall::BeginPlay()
{
    Super::BeginPlay();

    APlayerController* PlayerController = Cast<APlayerController>(GetController());
    if (!PlayerController)
    {
        PlayerController = GetWorld()->GetFirstPlayerController(); // Force possession if null
    }

    if (PlayerController)
    {
        PlayerController->Possess(this); // Force the controller to possess the pawn
        EnableInput(PlayerController);
        UE_LOG(LogTemp, Warning, TEXT("Pawn is receiving input!"));
    }

    // Bind input mapping context
    if (PlayerController)
    {
        if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer()))
        {
            Subsystem->AddMappingContext(InputMappingContext, 0);
            UE_LOG(LogTemp, Warning, TEXT("Input mapping context added!"));
        }
    }
}



// Called every frame
void ABall::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);
}

// Setup Enhanced Input
void ABall::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
    Super::SetupPlayerInputComponent(PlayerInputComponent);

    if (UEnhancedInputComponent* EnhancedInputComponent = Cast<UEnhancedInputComponent>(PlayerInputComponent))
    {
        // Bind movement action
        EnhancedInputComponent->BindAction(MovementAction, ETriggerEvent::Triggered, this, &ABall::Move);
    }
}

// Movement logic
void ABall::Move(const FInputActionValue& Value)
{
    FVector2D MovementVector = Value.Get<FVector2D>(); // Get input vector (X = forward, Y = right)

    FVector ForceDirection = FVector(MovementVector.Y, MovementVector.X, 0.0f); // Convert input to world movement
    ForceDirection = GetActorRotation().RotateVector(ForceDirection); // Align with actor's rotation
    ForceDirection.Normalize();

    if (bUseImpulse)
    {
        BallMesh->AddImpulse(ForceDirection * MoveForce, NAME_None, true);
    }
    else
    {
        BallMesh->AddForce(ForceDirection * MoveForce);
    }
}
