#include "StickySurface.h"
#include "Components/StaticMeshComponent.h"
#include "PhysicsEngine/PhysicsConstraintComponent.h"
#include "GameFramework/Actor.h"
#include "Components/PrimitiveComponent.h"

// Threshold force to break the sticky constraint
#define DETACH_FORCE_THRESHOLD 500000.0f

// Sets default values
AStickySurface::AStickySurface()
{
    PrimaryActorTick.bCanEverTick = true;

    // Create the static mesh component for the surface
    SurfaceMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("SurfaceMesh"));
    RootComponent = SurfaceMesh;

    // Enable physics on the surface (optional)
    SurfaceMesh->SetSimulatePhysics(false);
    SurfaceMesh->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
    SurfaceMesh->SetCollisionObjectType(ECC_WorldStatic);

    // Set collision handling for detecting hits
    SurfaceMesh->OnComponentHit.AddDynamic(this, &AStickySurface::OnHit);
}

// Called when the game starts
void AStickySurface::BeginPlay()
{
    Super::BeginPlay();
}

// Called every frame to check if objects should detach
void AStickySurface::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);
    CheckForDetach();
}

// Function to attach an object dynamically
void AStickySurface::OnHit(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, 
                           FVector NormalImpulse, const FHitResult& Hit)
{
    if (!OtherActor || !OtherComp) return;

    // Ensure the object is simulating physics before sticking
    if (OtherComp->IsSimulatingPhysics())
    {
        // Create a new physics constraint for this object
        UPhysicsConstraintComponent* NewConstraint = NewObject<UPhysicsConstraintComponent>(this);
        if (NewConstraint)
        {
            NewConstraint->RegisterComponent();
            NewConstraint->AttachToComponent(SurfaceMesh, FAttachmentTransformRules::KeepWorldTransform);

            // Set constrained components
            NewConstraint->SetConstrainedComponents(SurfaceMesh, NAME_None, OtherComp, NAME_None);

            // Correct constraint settings
            NewConstraint->SetLinearXLimit(ELinearConstraintMotion::LCM_Locked, 0.0f);
            NewConstraint->SetLinearYLimit(ELinearConstraintMotion::LCM_Locked, 0.0f);
            NewConstraint->SetLinearZLimit(ELinearConstraintMotion::LCM_Locked, 0.0f);

            NewConstraint->SetAngularSwing1Limit(EAngularConstraintMotion::ACM_Locked, 0.0f);
            NewConstraint->SetAngularSwing2Limit(EAngularConstraintMotion::ACM_Locked, 0.0f);
            NewConstraint->SetAngularTwistLimit(EAngularConstraintMotion::ACM_Locked, 0.0f);

            // Add constraint to the active list
            ActiveConstraints.Add(NewConstraint);

            UE_LOG(LogTemp, Warning, TEXT("Object %s is now stuck at Location: %s"), *OtherActor->GetName(), *Hit.Location.ToString());
        }
    }
}

// Function to check if objects should be unstuck
void AStickySurface::CheckForDetach()
{
    // for (int32 i = ActiveConstraints.Num() - 1; i >= 0; i--)
    // {
    //     UPhysicsConstraintComponent* Constraint = ActiveConstraints[i];
    //     if (Constraint)
    //     {
    //         // Get the constrained component safely
    //         UPrimitiveComponent* AttachedComponent = Constraint->ConstraintInstance.GetComponent(1); // Get the second constrained component
    //         if (AttachedComponent && AttachedComponent->IsSimulatingPhysics())
    //         {
    //             FVector CurrentForce = AttachedComponent->GetPhysicsLinearVelocity() * AttachedComponent->GetMass();
    //
    //             // If force exceeds threshold, break the constraint
    //             if (CurrentForce.Size() > DETACH_FORCE_THRESHOLD)
    //             {
    //                 UE_LOG(LogTemp, Warning, TEXT("Object %s is now unstuck!"), *AttachedComponent->GetOwner()->GetName());
    //
    //                 // Destroy constraint
    //                 Constraint->DestroyComponent();
    //                 ActiveConstraints.RemoveAt(i);
    //             }
    //         }
    //     }
    // }
}
