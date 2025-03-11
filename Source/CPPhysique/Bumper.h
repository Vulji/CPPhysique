#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Bumper.generated.h"

UCLASS()
class CPPHYSIQUE_API ABumper : public AActor
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	ABumper();

protected:
	virtual void BeginPlay() override;

public:    
	virtual void Tick(float DeltaTime) override;

private:
	// Bumper Mesh
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Components", meta = (AllowPrivateAccess = "true"))
	class UStaticMeshComponent* BumperMesh;

	// Sphere to detect overlap with the ball
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Components", meta = (AllowPrivateAccess = "true"))
	class USphereComponent* DetectionSphere;

	// Bounce force applied to objects
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Bounce", meta = (AllowPrivateAccess = "true"))
	float BounceForce = 5000000.0f;

	// Function to check for overlapping balls
	void CheckForCollisions();
};
