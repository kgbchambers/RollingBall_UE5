#include "RollABallPlayer.h"
#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h"


// Sets default values
ARollABallPlayer::ARollABallPlayer()
{
 	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;


	// Create Components - Before this step they do not exist on Actor, they have only been defined
	Mesh = CreateDefaultSubobject<UStaticMeshComponent>("Mesh");
	SpringArm = CreateDefaultSubobject<USpringArmComponent>("SpringArm");
	Camera = CreateDefaultSubobject<UCameraComponent>("Camera");

	// Set the root component to be our mesh
	RootComponent = Mesh;
	// Attach the SpringArm to the mesh, Spring will now follow the Mesh transform
	SpringArm->SetupAttachment(Mesh);
	// Attach the Camera to the SpringArm, Camera will now follow the SpringArm transform
	Camera->SetupAttachment(SpringArm);


	SpringArm->bInheritPitch = false;
	SpringArm->bInheritYaw = false;
	SpringArm->bInheritRoll = false;

	// Set physics to true
	Mesh->SetSimulatePhysics(true);
	Mesh->SetNotifyRigidBodyCollision(true);

	Mesh->OnComponentHit.AddDynamic(this, &ARollABallPlayer::OnHit);

}


// Called when the game starts or when spawned
void ARollABallPlayer::BeginPlay()
{
	Super::BeginPlay();

	//Account for mass in MoveForce
	MoveForce *= Mesh->GetMass();
	JumpImpulse *= Mesh->GetMass();
	
}


// Called to bind functionality to input
void ARollABallPlayer::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	// Custom Input Axis Bindings
	InputComponent->BindAxis("MoveForward", this, &ARollABallPlayer::MoveForward);
	InputComponent->BindAxis("MoveRight", this, &ARollABallPlayer::MoveRight);

	// Custom Action Binding
	InputComponent->BindAction("Jump", IE_Pressed, this, &ARollABallPlayer::Jump);


}


void ARollABallPlayer::MoveRight(float Value)
{
	// Get the Right vector of the camera as it doesn't rotate and move the player in this direction based on the input and MoveForce
	const FVector Right = Camera->GetRightVector() * MoveForce * Value;
	Mesh->AddForce(Right);
}


void ARollABallPlayer::MoveForward(float Value)
{
	const FVector Forward = Camera->GetForwardVector() * MoveForce * Value;
	Mesh->AddForce(Forward);
}


void ARollABallPlayer::Jump()
{
	//Cap number of jumps we can make
	if (JumpCount >= MaxJumpCount)
		return;

	// Add an impulse to the Mesh in the Z Axis (Up)
	Mesh->AddImpulse(FVector(0, 0, JumpImpulse));
	JumpCount++;
}


void ARollABallPlayer::OnHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit)
{

	// Get direction we hit the surface on Z-Axis (up/down)
	const float HitDirection = Hit.Normal.Z;
	// If more than 0 then we have hit something below us. 1 is flat, anything between is a slope
	
	if (HitDirection > 0)
	{
		JumpCount = 0;
	}
}




