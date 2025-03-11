#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "StickySurfaceComponent.generated.h"

UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class CPPHYSIQUE_API UStickySurfaceComponent : public UActorComponent
{
	GENERATED_BODY()

public:    
	UStickySurfaceComponent();

protected:
	virtual void BeginPlay() override;
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

private:
	UPROPERTY()
	TArray<UPrimitiveComponent*> StickyObjects;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Sticky", meta=(AllowPrivateAccess = "true"))
	float StickForce = 50000.0f;

	FTimerHandle StickyCheckTimer;

	UFUNCTION()
	void OnComponentBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, 
								 UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, 
								 const FHitResult &SweepResult);

	UFUNCTION()
	void OnComponentEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, 
							   UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

	void ApplyStickyForce();
};
