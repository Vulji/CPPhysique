#include "StickySurfaceComponent.h"
#include "Components/PrimitiveComponent.h"
#include "GameFramework/Actor.h"
#include "TimerManager.h"

UStickySurfaceComponent::UStickySurfaceComponent()
{
    PrimaryComponentTick.bCanEverTick = true;
}

void UStickySurfaceComponent::BeginPlay()
{
    Super::BeginPlay();
    
    UPrimitiveComponent* ParentComp = GetOwner()->FindComponentByClass<UPrimitiveComponent>();
    if (ParentComp)
    {
        ParentComp->OnComponentBeginOverlap.AddDynamic(this, &UStickySurfaceComponent::OnComponentBeginOverlap);
        ParentComp->OnComponentEndOverlap.AddDynamic(this, &UStickySurfaceComponent::OnComponentEndOverlap);
    }
    
    GetWorld()->GetTimerManager().SetTimer(StickyCheckTimer, this, &UStickySurfaceComponent::ApplyStickyForce, 0.01f, true);
}

void UStickySurfaceComponent::OnComponentBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, 
    UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult &SweepResult)
{
    if (OtherComp && OtherComp->IsSimulatingPhysics())
    {
        StickyObjects.Add(OtherComp);
        UE_LOG(LogTemp, Warning, TEXT("Object %s is now sticking!"), *OtherActor->GetName());
    }
}

void UStickySurfaceComponent::OnComponentEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, 
    UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
    if (OtherComp && StickyObjects.Contains(OtherComp))
    {
        StickyObjects.Remove(OtherComp);
        UE_LOG(LogTemp, Warning, TEXT("Object %s is no longer sticking!"), *OtherActor->GetName());
    }
}

void UStickySurfaceComponent::ApplyStickyForce()
{
    if (StickyObjects.Num() == 0) return;

    FVector SurfaceNormal = GetOwner()->GetActorUpVector(); 

    for (UPrimitiveComponent* StickyObject : StickyObjects)
    {
        if (StickyObject && StickyObject->IsSimulatingPhysics())
        {
            StickyObject->AddForce(-SurfaceNormal * StickForce, NAME_None, true);
        }
    }
}
