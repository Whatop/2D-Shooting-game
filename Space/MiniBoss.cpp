#include "stdafx.h"
#include "MiniBoss.h"
#include "MiniMissile.h"
#include "EnemyRotationBullet.h"
#include "EnemyDirBullet.h"
#include "EffectMgr.h"
#include "Item.h"

MiniBoss::MiniBoss(Vec2 Pos)
{
    m_Enemy = Sprite::Create(L"Painting/Enemy/Enemy.png");
    m_Enemy->SetParent(this);

    // 레드라인(장판) 스프라이트
        m_Pattern = Sprite::Create(L"Painting/Enemy/Temp.png"); // 빨간 라인 리소스로 교체 가능
        m_Pattern->SetScale(1.2f, 0.2f);
        m_Pattern->A = 0;
        m_Pattern->m_Visible = false;
        ObjMgr->AddObject(m_Pattern, "Effect");

    m_HitboxScale = 1.0f;
    SetHitbox(m_HitboxScale);

    SetPosition(Pos);
    m_RandomPosition = Vec2((rand() % 100 + 400) + m_Position.x, (rand() % 1080));

    m_MaxHp = 2000 * pow(1.5f, GameInfo->Stage - 1);
    m_Hp = m_MaxHp;
    m_Rotation = D3DXToRadian(270);
    m_Speed = 450.f;
    m_LastMoveTime = 2.f;

    isMissile = false;
    isBullet = true;
    m_Layer = 2;
    AttackTime = 0.f;
    ones = true;
    SpawnMove = 0.f;

    GameInfo->isMiniBossSpawn = true;
    GameInfo->EnemyCount++;

    pattern = 0;
    OneDamege = false;

    // 2페이즈 초기 상태
    m_InPhase2 = false;
    m_P2Step = P2Step::Idle;
    m_StepT = 0.f;
    m_RedlineFires = 0;
    m_Phase1Boost = false;

}

MiniBoss::~MiniBoss() {}

void MiniBoss::Update(float deltaTime, float Time)
{
    if (GameInfo->isPause) return;

    // 폭탄 피격 무적 처리
    if (!OneDamege) ObjMgr->CollisionCheck(this, "Boom");
    else {
        DamegeCoolTime += dt;
        if (DamegeCoolTime > 4.f) { DamegeCoolTime = 0.f; OneDamege = false; }
    }

    if (!m_InPhase2) {
        // ===== 1페(기존 패턴) =====
        SpawnMove += dt;
        if (SpawnMove < 2.f) {
            m_Position.x -= (300 + rand() % 100) * dt;
            ObjMgr->CollisionCheck(this, "Bullet");
            ObjMgr->CollisionCheck(this, "ChargeBullet");
        }
        else {
            if (ones) {
                m_RandomPosition = Vec2((rand() % 100 + 400) + m_Position.x, (rand() % 360 + 73));
                ones = false;
            }
                ObjMgr->CollisionCheck(this, "Bullet");
                ObjMgr->CollisionCheck(this, "ChargeBullet");
            m_LastMoveTime += dt;
            if (m_LastMoveTime >= 5.f) Move();

          

            if (GameInfo->AutoCamera && !GameInfo->CameraStop) {
                m_Position.x += 100 * dt;
            }

            Attack(); // 미사일+총알
            GameInfo->MiniBossHpUpdate(m_MaxHp, m_Hp);

            // 50% ↓ → 2페 진입
            if (m_Hp <= m_MaxHp * 0.5f) EnterPhase2();
        }
    }
    else {
        // ===== 2페 =====
        Phase2Update(dt);

        m_LastMoveTime += dt;
        if (m_LastMoveTime >= 5.f / pow(1.5f, GameInfo->Stage - 1) && m_Phase1Boost) Move();

        m_AoETarget.x = Camera::GetInst()->m_Position.x + App::GetInst()->m_Width / 2;
        m_Pattern->SetPosition(m_AoETarget);


        if (GameInfo->AutoCamera && !GameInfo->CameraStop) {
            m_Position.x += 100 * dt;
        }
        if (m_Phase1Boost) {
            Attack(); // 미사일+총알

            ObjMgr->CollisionCheck(this, "Bullet");
            ObjMgr->CollisionCheck(this, "ChargeBullet");
        }

        if (m_Hp <= 0) {
            ObjMgr->AddObject(new Item(m_Position), "ITEM");
            ObjMgr->AddObject(new EffectMgr(L"Painting/Effect/Big/", 1, 9, 0.1f, m_Position), "Effect");
            GameInfo->EnemyCount--;
            GameInfo->MaxScore += 1000;
            GameInfo->KillScore += 1000;
            GameInfo->isScoreScene = true;
            GameInfo->isBossSpawn = false;
            GameInfo->SpawnCoin(m_Position);
            SoundMgr* effect = new SoundMgr("Sound/explosion.wav", false);
            effect->play();
            effect->volumeSetting(0.03f);
            GameInfo->MiniBossHpUpdate(m_MaxHp, 0);
            ObjMgr->RemoveObject(this);
            return;
        }
        // 2페에서도 피격 체크
        GameInfo->MiniBossHpUpdate(m_MaxHp, m_Hp);
    }

    // 공통: 충돌박스 갱신
    SetHitbox(m_HitboxScale);
}

void MiniBoss::EnterPhase2()
{
    m_InPhase2 = true;
    m_P2Step = P2Step::Exit;   // 좌측으로 빠져나가는 연출
    m_StepT = 0.f;
    m_RedlineFires = 0;
    m_Phase1Boost = false;
    m_Hp = m_MaxHp / 2;

    // 복귀 목표(우측 내부 400px, 현재 y 근처)
}

void MiniBoss::Phase2Update(float t)
{
    m_StepT += dt;

    switch (m_P2Step)
    {
    case P2Step::Exit: {
        // 회전하면서 왼쪽 퇴장(회전 연출은 원하면 활성화)
        // m_Rotation += m_RotateSpeed * dt;	if (Acc < 7.5f)
        if (Acc < 15.5f)
        Acc += 3 * dt;
        m_Position.x -= 100 * Acc * dt;
        if (m_Position.x < Camera::GetInst()->m_Position.x - 100 && !One) {
            SetPosition(Camera::GetInst()->m_Position.x + 1920 + 300, -100);
            SetScale(4.f, 4.f);
            m_Enemy->A = 255;
            One = true;
        }

        if (m_Position.x < Camera::GetInst()->m_Position.x - 200) {
            // 화면 바깥으로 나가면 오른쪽 바깥에서 재입장 준비
        //    SetPosition(Camera::GetInst()->m_Position.x + App::GetInst()->m_Width + 220,
        //        GetPlayer->m_Position.y);
            SetScale(1.f, 1.f);
            m_StepT = 0.f;
            m_P2Step = P2Step::Idle;
        }
        break;
    }

    case P2Step::Idle: {
        // 레드라인 3회 미만 → AoE, 3회면 복귀
        if (m_RedlineFires < 3) {
            m_P2Step = P2Step::AoE_Windup;
            m_StepT = 0.f;

            m_AoEAlpha = 0;
            m_AoETarget = Vec2(Camera::GetInst()->m_Position.x + App::GetInst()->m_Width / 2,
                GetPlayer->m_Position.y);
            m_Pattern->SetPosition(m_AoETarget);
            m_Pattern->A = 0;
            m_Pattern->m_Visible = true;
        }
        else {
            m_P2Step = P2Step::Return;
            m_StepT = 0.f;
        }
        break;
    }

    case P2Step::AoE_Windup:
        AoE_WindupTick(dt);     
        if (!Two) {
            // 알파 0→255
            SoundMgr* effect = new SoundMgr("Sound/mus_sfx_segapower2.wav", false);
            effect->play();
            effect->volumeSetting(0.1f);
            Two = true;
        }
        break;

    case P2Step::AoE_Fire:
        AoE_FireTick();              // 탄막 발사 + 카운트++
        m_P2Step = P2Step::Hold;
        m_StepT = 0.f;
        Two = false;

        break;
    case P2Step::Hold: {
        m_AoEHold -= dt;
        m_FanSpawnTimer += dt;
        m_EnemySpawnTimer += dt;

        // 5초마다 FireFanWall 생성
        if (m_FanSpawnTimer >= 5.f) {
            FireFanWall();
            m_FanSpawnTimer = 0.f;
        }
        if (m_EnemySpawnTimer >= 12.f) {
            GameInfo->SpawnEnemyStageTwo();
            m_EnemySpawnTimer = 0.f;
        }


        // 30초가 끝나면 Idle로 복귀
        if (m_AoEHold <= 0.f) {
            m_P2Step = P2Step::Idle;
            m_StepT = 0.f;
        }
        break;
    }

    case P2Step::Return: {
        // 우측 목표 지점으로 복귀
        if (!m_Phase1Boost) {
            SetPosition(Camera::GetInst()->m_Position.x + App::GetInst()->m_Width + 220,
                GetPlayer->m_Position.y);
            SpawnMove = 0;
            m_StepT = 0;
            m_Phase1Boost = true;
            GameInfo->isOneMiniBoss = true;
        }
        if (m_StepT > 2.f) {
            SpawnMove += dt;
            if (SpawnMove < 0.2f) {
                m_Position.x -= 100 * Acc * dt;

            }
            else {
                m_RedlineFires = 0;
            }
        }
        break;
    }
    case P2Step::P1Boost:
        // (미사용) 1페 강화는 m_InPhase2=false + m_Phase1Boost=true로 1페 Attack에서 처리
        break;
    }
}

void MiniBoss::AoE_WindupTick(float t)
{
    // 1) 알파 올리기(255까지)
    if (m_AoEAlpha < 255) {
        m_AoEAlpha = min(255, m_AoEAlpha + int(300 * dt));
        m_Pattern->A = m_AoEAlpha;
        return; // 아직 255가 아니면 여기서 종료
    }

    // 2) 255 도달 이후: m_StepT를 '대기 타이머'로 사용
    //    m_StepT는 Phase2Update() 상단에서 자동으로 +=dt 되고 있음
    if (m_StepT >= m_AoEHold) {
        m_P2Step = P2Step::AoE_Fire;
        m_StepT = 0.f; // 다음 스텝을 위한 초기화
    }
    // else: 대기 중(아무것도 안 함)
}

//Camera::GetInst()->m_Position.x + 1920 + 300
void MiniBoss::AoE_FireTick()
{
    // 1) 오른쪽 바깥에서 7x3 대형으로 고속 탄막
    //    cols=7, rows=3, spacingX/spacingY는 상황에 맞춰 조절
    FireWallGrid(/*cols*/10, /*rows*/3, /*spacingX*/400.f, /*spacingY*/45.f, /*speed*/2000);

    m_P2Step = P2Step::Hold;
    m_AoEHold = 10.f;
    m_FanSpawnTimer = 0.f;
    m_EnemySpawnTimer = 10.f;
    m_Pattern->A = 0;

    ++m_RedlineFires;
}

void MiniBoss::FireWallGrid(int cols, int rows, float spacingX, float spacingY, float speed)
{
    // spawnX: off-screen right
    float spawnX = Camera::GetInst()->m_Position.x + 1920.f + 300.f;

    // centerY: pattern line (use AoE target y if you prefer)
    float centerY = m_AoETarget.y; // or m_Pattern->GetPosition().y

    // grid center align
    float startX = spawnX; // all bullets start near same X (right side)
    float startY = centerY - (rows - 1) * 0.5f * spacingY;

    Vec2 vel(-speed, 0.f); // move left

    for (int r = 0; r < rows; ++r)
    {
        float y = startY + r * spacingY;
        for (int c = 0; c < cols; ++c)
        {
            // slight x offset to make a true "grid" just off-screen
            float x = startX + c * spacingX * 0.25f; // small stagger; still off-screen
            ObjMgr->AddObject(new EnemyDirBullet(Vec2(x, y), vel,speed), "EnemyBullet");
        }
    }
}

void MiniBoss::FireFanWall()
{
    int ver = rand() % 2 + 1;   // 1=왼→오, 2=오→왼
    int count = 8;              // 총알 개수
    int hole = rand() % count;  // 피할 구멍 인덱스

    float spawnX;
    Vec2 vel;

    if (ver == 1) {
        // 왼쪽에서 오른쪽
        spawnX = Camera::GetInst()->m_Position.x - 100;
        vel = Vec2(700.f, 0.f);   // 오른쪽으로 빠르게
    }
    else {
        // 오른쪽에서 왼쪽
        spawnX = Camera::GetInst()->m_Position.x + App::GetInst()->m_Width + 100;
        vel = Vec2(-700.f, 0.f);  // 왼쪽으로 빠르게
    }

    // y 좌표 기준 [-100, 430] 범위에서 시작
    float startY = -100.f;
    float endY = 430.f;
    float spacing = (endY - startY) / (count - 1);

    for (int i = 0; i < count; i++) {
        if (i == hole) continue; // 구멍 뚫기

        float y = startY + i * spacing;
        ObjMgr->AddObject(new EnemyDirBullet(Vec2(spawnX, y), vel), "EnemyBullet");
    }
}

void MiniBoss::Render()
{
    m_Enemy->Render();
}

void MiniBoss::OnCollision(Object* obj)
{
    if (obj->m_Tag == "Bullet") {
        m_Hp -= obj->m_Atk;
        float randx = (rand() % (int)m_Size.x * m_Scale.x) + m_Position.x - m_Size.x / 2 * m_Scale.x;
        float randy = (rand() % (int)m_Size.y * m_Scale.y) + m_Position.y - m_Size.y / 2 * m_Scale.y;
        obj->SetDestroy(true);
        ObjMgr->AddObject(new EffectMgr(L"Painting/Effect/Explosion/", 1, 9, 0.1f, Vec2(randx, randy)), "Effect");
    }
    if (obj->m_Tag == "ChargeBullet") {
        m_Hp -= obj->m_Atk;
        float randx = (rand() % (int)m_Size.x * m_Scale.x) + m_Position.x - m_Size.x / 2 * m_Scale.x;
        float randy = (rand() % (int)m_Size.y * m_Scale.y) + m_Position.y - m_Size.y / 2 * m_Scale.y;
        obj->SetDestroy(true);
        ObjMgr->AddObject(new EffectMgr(L"Painting/Effect/Explosion/", 1, 9, 0.1f, Vec2(randx, randy)), "Effect");
        GameInfo->RemoveCharge();
    }
    if (obj->m_Tag == "Boom") {
        m_Hp -= obj->m_Atk;
        OneDamege = true;
    }
}

void MiniBoss::SetHitbox(float scale)
{
    float w = m_BaseColSize.x * scale;
    float h = m_BaseColSize.y * scale;
    SetRect(&m_Collision,
        int(m_Position.x - w * 0.5f),
        int(m_Position.y - h * 0.5f),
        int(m_Position.x + w * 0.5f),
        int(m_Position.y + h * 0.5f));
}

void MiniBoss::Move()
{
    Vec2 A = m_Position;
    Vec2 B = m_RandomPosition;
    Vec2 Dire = B - A;
    D3DXVec2Normalize(&Dire, &Dire);

    const int EPSILON = 10;
    if (abs(m_Position.x - m_RandomPosition.x) > EPSILON && abs(m_Position.y - m_RandomPosition.y) > EPSILON)
    {
        if (m_Position.x > Camera::GetInst()->m_Position.x + App::GetInst()->m_Width / 1.7f &&
            m_Position.x < Camera::GetInst()->m_Position.x + App::GetInst()->m_Width - 100)
            m_Position.x += Dire.x * m_Speed * dt;

        m_Position.y += Dire.y * m_Speed * dt;
    }
    else
    {
        const float h = App::GetInst()->m_Height;
        const float scaleH = h / 1080.f;
        const float minY = -60.f * scaleH;
        const float maxY = 387.f * scaleH;


        if (m_Position.x > Camera::GetInst()->m_Position.x + App::GetInst()->m_Width - 600)
            m_RandomPosition.x = (rand() % -400 - 100) + (int)m_Position.x;
        else
            m_RandomPosition.x = (rand() % 500 + 300) + (int)m_Position.x;


        m_RandomPosition.y = RandRange(minY, maxY);
        m_LastMoveTime = 2.f;
    }
}

void MiniBoss::Attack()
{
    if (isBullet) {
        AttackTime += dt;
        if (AttackTime > 0.1f && pattern == 0) {
            ObjMgr->AddObject(new EnemyRotationBullet(Vec2(m_Position.x - 10, m_Position.y), 180), "EnemyBullet");
            ObjMgr->AddObject(new EnemyRotationBullet(Vec2(m_Position.x - 10, m_Position.y), 180 + 50), "EnemyBullet");
            ObjMgr->AddObject(new EnemyRotationBullet(Vec2(m_Position.x - 10, m_Position.y), 180 - 50), "EnemyBullet");
            pattern++;
        }
        if (AttackTime > 0.5f && pattern == 1) {
            ObjMgr->AddObject(new EnemyRotationBullet(Vec2(m_Position.x - 10, m_Position.y), 180), "EnemyBullet");
            ObjMgr->AddObject(new EnemyRotationBullet(Vec2(m_Position.x - 10, m_Position.y), 180 + 20), "EnemyBullet");
            ObjMgr->AddObject(new EnemyRotationBullet(Vec2(m_Position.x - 10, m_Position.y), 180 - 20), "EnemyBullet");
            ObjMgr->AddObject(new EnemyRotationBullet(Vec2(m_Position.x - 10, m_Position.y), 180 + 40), "EnemyBullet");
            ObjMgr->AddObject(new EnemyRotationBullet(Vec2(m_Position.x - 10, m_Position.y), 180 - 40), "EnemyBullet");
            pattern++;
        }
        if (AttackTime > 0.9f && pattern == 2) {
            ObjMgr->AddObject(new EnemyRotationBullet(Vec2(m_Position.x - 10, m_Position.y), 180), "EnemyBullet");
            ObjMgr->AddObject(new EnemyRotationBullet(Vec2(m_Position.x - 10, m_Position.y), 180 + 50), "EnemyBullet");
            ObjMgr->AddObject(new EnemyRotationBullet(Vec2(m_Position.x - 10, m_Position.y), 180 - 50), "EnemyBullet");
            pattern++;
        }
        if (AttackTime > 1.3f && pattern == 3) {
            ObjMgr->AddObject(new EnemyRotationBullet(Vec2(m_Position.x - 10, m_Position.y), 180), "EnemyBullet");
            ObjMgr->AddObject(new EnemyRotationBullet(Vec2(m_Position.x - 10, m_Position.y), 180 + 20), "EnemyBullet");
            ObjMgr->AddObject(new EnemyRotationBullet(Vec2(m_Position.x - 10, m_Position.y), 180 - 20), "EnemyBullet");
            ObjMgr->AddObject(new EnemyRotationBullet(Vec2(m_Position.x - 10, m_Position.y), 180 + 40), "EnemyBullet");
            ObjMgr->AddObject(new EnemyRotationBullet(Vec2(m_Position.x - 10, m_Position.y), 180 - 40), "EnemyBullet");
            isBullet = false;
            AttackTime = 0.f;
            pattern = 0;
        }
    }
    else {
        AttackDelay += dt;
        float need = m_Phase1Boost ? 0.5f : 2.0f;  // ★ 복귀 후 강화: 미사일 쿨 단축
        if (AttackDelay > need) {
            isBullet = true;
            AttackDelay = 0;

         
            if (m_Phase1Boost) {
                // 강화 보너스 2발
                ObjMgr->AddObject(new MiniMissile(Vec2(m_Position.x + 15, m_Position.y + 22), 190 - 45), "Missile");
                ObjMgr->AddObject(new MiniMissile(Vec2(m_Position.x + 15, m_Position.y - 22), 190 + 45), "Missile");
            }
            else {
                // 기본 4발
                ObjMgr->AddObject(new MiniMissile(Vec2(m_Position.x + 15, m_Position.y + 22), 190 - 20), "Missile");
                ObjMgr->AddObject(new MiniMissile(Vec2(m_Position.x + 15, m_Position.y - 22), 190 + 20), "Missile");
                ObjMgr->AddObject(new MiniMissile(Vec2(m_Position.x + 15, m_Position.y + 22), 190 - 45), "Missile");
                ObjMgr->AddObject(new MiniMissile(Vec2(m_Position.x + 15, m_Position.y - 22), 190 + 45), "Missile");

            }
        }
    }
}
