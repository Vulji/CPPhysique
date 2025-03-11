#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Bumper.generated.h"

UCLASS()
class CPPHYSIQUE_API ABumper : public AActor
{
	GENERATED_BODY()

public:
	ABumper();

protected:
	virtual void BeginPlay() override;

public:    
	virtual void Tick(float DeltaTime) override;

private:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Components", meta = (AllowPrivateAccess = "true"))
	class UStaticMeshComponent* BumperMesh;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Components", meta = (AllowPrivateAccess = "true"))
	class USphereComponent* DetectionSphere;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Bounce", meta = (AllowPrivateAccess = "true"))
	float BounceForce = 5000000.0f;
	
	void CheckForCollisions();
};
