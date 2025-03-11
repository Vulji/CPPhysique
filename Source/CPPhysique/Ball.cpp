#include "Ball.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "GameFramework/PlayerController.h"
#include "Components/StaticMeshComponent.h"

// Sets default values
ABall::ABall()
{
    PrimaryActorTick.bCanEverTick = true;

    // Create a static mesh component for the ball
    BallMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("BallMesh"));
    RootComponent = BallMesh;

    // Enable physics
    BallMesh->SetSimulatePhysics(true);
    BallMesh->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
}

// Called when the game starts or when spawned
void ABall::BeginPlay()
{
    Super::BeginPlay();

    APlayerController* PlayerController = Cast<APlayerController>(GetController());
    if (!PlayerController)
    {
        PlayerController = GetWorld()->GetFirstPlayerController();
    }

    if (PlayerController)
    {
        PlayerController->Possess(this);
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

    FVector Start = BallMesh->GetComponentLocation();
    FVector End = Start - FVector(0, 0, 50); // Cast downward

    FHitResult Hit;
    FCollisionQueryParams QueryParams;
    QueryParams.AddIgnoredActor(this); // Ignore self

    if (GetWorld()->LineTraceSingleByChannel(Hit, Start, End, ECC_Visibility, QueryParams))
    {
        if (Hit.Normal.Z > 0.7f)
        {
            bIsGrounded = true;
            JumpCount = 0;
        }
    }
    else
    {
        bIsGrounded = false;
    }
}


// Setup Enhanced Input
void ABall::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
    Super::SetupPlayerInputComponent(PlayerInputComponent);

    if (UEnhancedInputComponent* EnhancedInputComponent = Cast<UEnhancedInputComponent>(PlayerInputComponent))
    {
        EnhancedInputComponent->BindAction(MovementAction, ETriggerEvent::Triggered, this, &ABall::Move);
        EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Triggered, this, &ABall::Jump);
        UE_LOG(LogTemp, Warning, TEXT("Jump action successfully bound!"));
    }
}

// Movement logic
void ABall::Move(const FInputActionValue& Value)
{
    FVector2D MovementVector = Value.Get<FVector2D>();

    FVector ForceDirection = FVector(MovementVector.Y, MovementVector.X, 0.0f);
    ForceDirection = GetActorRotation().RotateVector(ForceDirection);
    ForceDirection.Normalize();

    if (BallMesh->IsSimulatingPhysics())
    {
        BallMesh->AddForce(ForceDirection * MoveForce);
    }
}

// Jump logic
void ABall::Jump()
{
    if (JumpCount < MaxJumps) 
    {
        FVector JumpForce = FVector(0, 0, JumpImpulse);
        BallMesh->AddImpulse(JumpForce, NAME_None, true);
        JumpCount++;

        bIsGrounded = false; // Set to false because we are now airborne

        UE_LOG(LogTemp, Warning, TEXT("Jump! Count: %d"), JumpCount);
    }
}

// Detect landing and reset jump count
void ABall::NotifyHit(UPrimitiveComponent* MyComp, AActor* Other, UPrimitiveComponent* OtherComp, 
    bool bSelfMoved, FVector HitLocation, FVector HitNormal, FVector NormalImpulse, const FHitResult& Hit)
{
    Super::NotifyHit(MyComp, Other, OtherComp, bSelfMoved, HitLocation, HitNormal, NormalImpulse, Hit);

    // If the ball lands on a relatively horizontal surface
    if (HitNormal.Z > 0.7f && !bIsGrounded) // Make sure it's a flat-enough surface
    {
        JumpCount = 0; // Reset jumps
        bIsGrounded = true; // Ball is now grounded
        UE_LOG(LogTemp, Warning, TEXT("Landed! Jump reset."));
    }
}
