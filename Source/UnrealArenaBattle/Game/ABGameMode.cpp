// Fill out your copyright notice in the Description page of Project Settings.


#include "Game/ABGameMode.h"
//#include "Player/ABPlayerController.h"

AABGameMode::AABGameMode()
{
	// DefaultPawnClass 설정
	static ConstructorHelpers::FClassFinder<APawn> ThirdPersonClassRef(TEXT("/Game/ThirdPerson/Blueprints/BP_ThirdPersonCharacter.BP_ThirdPersonCharacter_C"));
	if (ThirdPersonClassRef.Class != nullptr)
	{
		DefaultPawnClass = ThirdPersonClassRef.Class;
	}
	// 앞서 추가했던 ThirdPerson 에셋을 가져올건데, 얘는 블루프린트임
	// 그래서 우클릭 > Copy Reference 로 주소를 가져옴.
	// 이 에셋은 APawn 클래스를 상속받았기 때문에 FClassFinder<APawn> (에셋 켜보면 우측에 Character라고 뜨는데, 이 클래스 또한 APawn 상속받음)
	// 레퍼런스 붙여넣으면 '' 도 붙는데 이건 지우고,
	// 클래스 정보를 가져올 것이기 때문에 끝에 _C 를 붙인다.
	// 생성자에서 작성하는 코드이기 때문에 ConstructorHelpers 클래스로...



	// PlayerControllerClass 설정
	//PlayerControllerClass = AABPlayerController::StaticClass();
	// 내가 만든 Controller 클래스 넣어주는 것



	static ConstructorHelpers::FClassFinder<APlayerController> PlayerControllerClassRef(TEXT("/Script/UnrealArenaBattle.ABPlayerController"));
	if (PlayerControllerClassRef.Class)
	{
		PlayerControllerClass = PlayerControllerClassRef.Class;
	}
	// 이렇게 작성하면 헤더(Player/ABPlayerController.h)를 include 하지 않고,
	// StaticClass() 함수 호출 하지 않고,
	// 컨트롤러 클래스를 가져올 수 있음
	
}
