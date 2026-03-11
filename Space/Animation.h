#pragma once

class Texture;

// Animation은 여러 Sprite를 프레임 단위로 관리하는 객체
class Animation : public Object
{
private:
	std::vector<Sprite*> m_Anims; // 애니메이션 프레임들

	float m_Delay;      // 프레임 전환 간격
	bool m_AutoPlay;    // 자동 재생 여부
	float m_FrameCount; // 누적 시간

	int m_FirstFrame;   // 시작 프레임 번호
	int m_LastFrame;    // 마지막 프레임 번호

public:
	int m_CurrentFrame; // 현재 재생 중인 프레임
	int A, R, G, B;     // 현재 프레임 Sprite에 넘길 색상값

public:
	Animation();
	~Animation();

	// fileName + 번호 + ".png" 형태로 연속된 프레임들을 추가
	void AddContinueFrame(std::wstring fileName, int firstFrame, int lastFrame, D3DCOLOR ColorKey = COLORKEY_GREEN);

	// 다음 프레임으로 이동
	void NextFrame();

	// 이전 프레임으로 이동
	void BackFrame();

	// 애니메이션 초기 설정
	void Init(float delay, bool play);

	// 프레임 갱신
	void Update(float deltaTime, float time);

	// 현재 프레임 렌더링
	void Render();
};