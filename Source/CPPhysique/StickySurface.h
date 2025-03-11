#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "StickySurface.generated.h"

UCLASS()
class CPPHYSIQUE_API AStickySurface : public AActor
{
	GENERATED_BODY()

public:
	AStickySurface();

protected:
	virtual void BeginPlay() override;

public:    
	virtual void Tick(float DeltaTime) override;

private:
	// Sticky Surface Mesh
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Components", meta = (AllowPrivateAccess = "true"))
	class UStaticMeshComponent* SurfaceMesh;

	// List of active physics constraints
	UPROPERTY()
	TArray<class UPhysicsConstraintComponent*> ActiveConstraints;

	// Function to attach objects
	UFUNCTION()
	void OnHit(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, 
			   FVector NormalImpulse, const FHitResult& Hit);

	// Function to release constraints if enough force is applied
	void CheckForDetach();
};
